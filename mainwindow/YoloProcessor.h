#pragma once

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>

class YoloProcessor : public QObject
{
    Q_OBJECT
public:
    explicit YoloProcessor(QObject *parent = nullptr);

    // 加载模型
    bool loadModel(const std::string &onnxPath, bool useGPU = false);

    // 检测接口（异步触发可选）
    void detect(const QImage &image);

    // 设置类别名
    void setClassNames(const std::vector<std::string> &names);

signals:
    void processedFrame(const QImage &image);  // 检测完成后发送Qt图像

private:
    cv::Mat QImageToMat(const QImage &image);
    QImage MatToQImage(const cv::Mat &mat);

    cv::Mat runInference(const cv::Mat &frame);
    void drawDetections(cv::Mat &frame,
        const std::vector<cv::Rect> &boxes,
        const std::vector<float> &scores,
        const std::vector<int> &classIds,
        const std::vector<int> &idxs);

private:
    cv::dnn::Net net;
    std::vector<std::string> classNames;
    bool initialized = false;
    bool useGPU = false;
    cv::Size inputSize = cv::Size(640, 640);
};

