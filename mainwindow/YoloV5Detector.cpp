#include "YoloV5Detector.h"
#include <iostream>
using namespace cv;
using namespace std;
using namespace Ort;



// 构造函数实现
YoloV5Detector::YoloV5Detector(const string& modelPath, const Size& inputSize, bool useGPU, QObject* parent)
    : QObject(parent), // 初始化 QObject
    env(ORT_LOGGING_LEVEL_WARNING, "YoloV5"),
    input_geometry(inputSize)
{
    input_node_dims = { 1, 3, inputSize.height, inputSize.width };
    input_tensor_size = 1 * 3 * inputSize.height * inputSize.width;
    results.reserve(200);
    initSession(modelPath, useGPU);
}


vector<YoloV5Detector::Detection> YoloV5Detector::detect(const Mat& rawImage, float confThreshold, float nmsThreshold) {
    results.clear();
    if (rawImage.empty()) return results;

    // --- 1. 预处理 (Resize + Normalize) ---
    //Mat blob;
    // 将 1920x1088 缩放到 640x640
    // 这里的 swapRB=true (BGR->RGB), crop=false
    dnn::blobFromImage(rawImage, blob, 1.0 / 255.0, input_geometry, Scalar(0, 0, 0), true, false);

    // 创建输入 Tensor
    // 1 x 3 x 640 x 640 = 1,228,800 元素
    //size_t inputTensorSize = 1 * 3 * input_geometry.width * input_geometry.height;
    //vector<int64_t> inputDims = { 1, 3, input_geometry.height, input_geometry.width };

    MemoryInfo memoryInfo = MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Value inputTensor = Value::CreateTensor<float>(
        memoryInfo, blob.ptr<float>(), input_tensor_size, input_node_dims.data(), input_node_dims.size()
        );

    // --- 2. 推理 (Inference) ---
    // 获取输入输出节点名
    const char* input_names[] = { input_name.c_str() };
    const char* output_names[] = { output_name.c_str() };

    auto outputTensors = session->Run(
        RunOptions{ nullptr }, input_names, &inputTensor, 1, output_names, 1
    );

    // --- 3. 后处理 (Post-Process) ---
    // YOLOv5 输出通常是 [1, 25200, 85] (对于640输入)
    // 85 = x, y, w, h, obj_conf, class0, class1, ...
    float* rawOutput = outputTensors[0].GetTensorMutableData<float>();
    auto outputShape = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();

    int batchSize = outputShape[0];
    int num_anchors = outputShape[1]; // 25200
    int channels = outputShape[2];    // 85 (如果只有1类则是 6)

    // 计算缩放因子：从 640x640 还原回 1920x1088
    // 这里我们使用的是直接 Resize，所以 x 和 y 的缩放比例不同
    float scale_x = (float)rawImage.cols / input_geometry.width;
    float scale_y = (float)rawImage.rows / input_geometry.height;

    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    // 遍历所有框
    for (int i = 0; i < num_anchors; ++i) {
        // 指针偏移到当前框的起始位置
        float* pdata = rawOutput + (i * channels);

        // 优化：先看 Object Confidence (index 4)
        float obj_conf = pdata[4];
        if (obj_conf < confThreshold) continue; // 快速过滤背景

        // 找最大类别分数
        // 类别分数从 index 5 开始
        float* classes_scores = pdata + 5;
        int classId = -1;
        float maxClassScore = 0.0f;

        // 简单循环找最大值
        int num_classes = channels - 5;
        for (int c = 0; c < num_classes; ++c) {
            if (classes_scores[c] > maxClassScore) {
                maxClassScore = classes_scores[c];
                classId = c;
            }
        }

        // 最终置信度
        float final_score = obj_conf * maxClassScore;

        if (final_score > confThreshold) {
            // 解析坐标 (x, y, w, h) 都是相对于 640x640 的
            float cx = pdata[0];
            float cy = pdata[1];
            float w = pdata[2];
            float h = pdata[3];

            // 还原到原图坐标
            int left = int((cx - 0.5 * w) * scale_x);
            int top = int((cy - 0.5 * h) * scale_y);
            int width = int(w * scale_x);
            int height = int(h * scale_y);

            boxes.push_back(Rect(left, top, width, height));
            confidences.push_back(final_score);
            classIds.push_back(classId);
        }
    }

    // NMS (非极大值抑制)
    vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    for (int idx : indices) {
        Detection det;
        det.class_id = classIds[idx];
        det.confidence = confidences[idx];
        det.box = boxes[idx];
        results.push_back(det);
    }

    return results;
}

void YoloV5Detector::initSession(const string& modelPath, bool useGPU) {
    SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(1); // 减少CPU争抢
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    if (useGPU) {
        try {
            // ORT 1.14 写法
            OrtCUDAProviderOptions cuda_options;
            cuda_options.device_id = 0;
            // 显存不够时可调：cuda_options.gpu_mem_limit = 2UL * 1024 * 1024 * 1024;
            sessionOptions.AppendExecutionProvider_CUDA(cuda_options);
            cout << "[ORT] CUDA Provider Enabled." << endl;
        }
        catch (const exception& e) {
            cerr << "[ORT] Warning: Failed to enable CUDA. " << e.what() << endl;
        }
    }

    // 宽字符路径支持 (Windows)
    wstring w_modelPath(modelPath.begin(), modelPath.end());
    session = make_unique<Session>(env, w_modelPath.c_str(), sessionOptions);

    // 自动获取节点名称
    AllocatorWithDefaultOptions allocator;
    input_name = session->GetInputNameAllocated(0, allocator).get();
    output_name = session->GetOutputNameAllocated(0, allocator).get();

    cout << "[ORT] Model loaded: " << input_name << " -> " << output_name << endl;
}