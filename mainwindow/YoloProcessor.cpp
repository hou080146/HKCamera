#include "YoloProcessor.h"
#include <QDebug>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace cv::dnn;

YoloProcessor::YoloProcessor(QObject *parent)
    : QObject(parent)
{
}

bool YoloProcessor::loadModel(const std::string &onnxPath, bool useGPU)
{
    this->useGPU = useGPU;
    if (!std::ifstream(onnxPath)) {
        qWarning() << "[ERROR] Model file not found:" << QString::fromStdString(onnxPath);
        return false;
    }

    try {
        net = readNetFromONNX(onnxPath);
        if (useGPU) {
            net.setPreferableBackend(DNN_BACKEND_CUDA);
            net.setPreferableTarget(DNN_TARGET_CUDA_FP16);
            qDebug() << "[INFO] YOLO model running on GPU (CUDA)";
        }
        else {
            net.setPreferableBackend(DNN_BACKEND_OPENCV);
            net.setPreferableTarget(DNN_TARGET_CPU);
            qDebug() << "[INFO] YOLO model running on CPU";
        }
    }
    catch (const std::exception &e) {
        qWarning() << "[ERROR] Failed to load YOLO model:" << e.what();
        return false;
    }

    initialized = true;
    qDebug() << "[INFO] YOLO model loaded successfully!";
    return true;
}

void YoloProcessor::setClassNames(const std::vector<std::string> &names)
{
    classNames = names;
}

// 主检测接口（可直接在主线程调用）
void YoloProcessor::detect(const QImage &image)
{
    if (!initialized) {
        qWarning() << "[WARN] YOLO not initialized!";
        emit processedFrame(image);
        qDebug() << "[INFO] send signal: processedFrame(image)";
        return;
    }

    cv::Mat frame = QImageToMat(image);
    cv::Mat result = runInference(frame);
    emit processedFrame(MatToQImage(result));
    qDebug() << "[INFO] send signal: processedFrame(result)";
}

cv::Mat YoloProcessor::runInference(const cv::Mat &frame)
{
    cv::Mat blob;
    blobFromImage(frame, blob, 1 / 255.0, inputSize, cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    // 合并所有输出
    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    std::vector<int> classIds;

    const float confThreshold = 0.5f;   // 置信度阈值
    const float nmsThreshold = 0.45f;   // NMS 阈值

    for (const auto &output : outputs)
    {
        for (int i = 0; i < output.rows; ++i)
        {
            const float* data = output.ptr<float>(i);
            float obj_conf = data[4];
            if (obj_conf < confThreshold) continue;

            // 找到类别置信度最大的位置
            int numClasses = output.cols - 5;
            int bestClass = -1;
            float bestClassScore = 0.f;
            for (int c = 0; c < numClasses; ++c)
            {
                float s = data[5 + c];
                if (s > bestClassScore) { bestClassScore = s; bestClass = c; }
            }

            float score = obj_conf * bestClassScore;
            if (score < confThreshold) continue;

            float cx = data[0], cy = data[1], w = data[2], h = data[3];
            int x = static_cast<int>((cx - w / 2.0f) * frame.cols);
            int y = static_cast<int>((cy - h / 2.0f) * frame.rows);
            int width = static_cast<int>(w * frame.cols);
            int height = static_cast<int>(h * frame.rows);

            // 修正越界
            x = std::max(0, std::min(x, frame.cols - 1));
            y = std::max(0, std::min(y, frame.rows - 1));
            if (x + width > frame.cols) width = frame.cols - x;
            if (y + height > frame.rows) height = frame.rows - y;
            if (width <= 0 || height <= 0) continue;

            boxes.emplace_back(x, y, width, height);
            scores.emplace_back(score);
            classIds.emplace_back(bestClass);
        }
    }

    // NMS 过滤
    std::vector<int> idxs;
    cv::dnn::NMSBoxes(boxes, scores, confThreshold, nmsThreshold, idxs);

    // 调用绘制函数，只绘制经过 NMS 的结果
    cv::Mat result = frame.clone();
    drawDetections(result, boxes, scores, classIds, idxs);

    return result;
}

cv::Mat YoloProcessor::QImageToMat(const QImage &image)
{
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(image.height(), image.width(), CV_8UC3,
        const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
}

QImage YoloProcessor::MatToQImage(const cv::Mat &mat)
{
    cv::Mat rgb;
    if (mat.channels() == 3)
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    else
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);

    return QImage((const uchar*)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}

// 在 YoloProcessor 类中实现
void YoloProcessor::drawDetections(cv::Mat &frame,
    const std::vector<cv::Rect> &boxes,
    const std::vector<float> &scores,
    const std::vector<int> &classIds,
    const std::vector<int> &idxs)
{
    const cv::Scalar palette[] = {
        {0,255,0}, {0,0,255}, {255,0,0}, {0,255,255}, {255,0,255},
        {255,255,0}, {128,255,0}, {0,128,255}
    };
    const int paletteSize = sizeof(palette) / sizeof(palette[0]);

    for (int i : idxs)
    {
        cv::Rect box = boxes[i];
        int cls = classIds[i];
        float score = scores[i];
        cv::Scalar color = palette[cls % paletteSize];

        cv::rectangle(frame, box, color, 2);

        std::string label;
        if (cls >= 0 && cls < (int)classNames.size())
            label = classNames[cls] + " " + cv::format("%.2f", score);
        else
            label = "cls" + std::to_string(cls) + " " + cv::format("%.2f", score);

        int baseLine = 0;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = std::max(box.y, labelSize.height);
        cv::rectangle(frame, cv::Point(box.x, top - labelSize.height - 4),
            cv::Point(box.x + labelSize.width, top + baseLine - 4),
            color, cv::FILLED);
        cv::putText(frame, label, cv::Point(box.x, top - 4),
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    }
}
