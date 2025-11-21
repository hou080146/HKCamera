#pragma once
#include <qobject.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <onnxruntime_cxx_api.h>
//#include <cuda_runtime.h>



class YoloV5Detector :
    public QObject
{
    Q_OBJECT

public:
    struct Detection {
        int class_id;
        float confidence;
        cv::Rect box;
    };

    // 构造函数：加载模型
    explicit YoloV5Detector(const std::string& modelPath, const cv::Size& inputSize = cv::Size(640, 640), bool useGPU = true, QObject* parent = nullptr);

    // 核心检测函数
    std::vector<Detection> detect(const cv::Mat& rawImage, float confThreshold = 0.4f, float nmsThreshold = 0.45f);

private:
    cv::Mat blob;
    std::vector<float> input_tensor_values;
    std::vector<Detection> results;

    size_t input_tensor_size;
    std::vector<int64_t> input_node_dims;

    Ort::Env env;
    std::unique_ptr<Ort::Session> session;
    std::string input_name;
    std::string output_name;
    cv::Size input_geometry;

    void initSession(const std::string& modelPath, bool useGPU);
};

