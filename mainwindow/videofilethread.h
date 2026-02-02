#pragma once
#pragma execution_character_set("utf-8")

#include <QThread>
#include <QMutex>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "YoloV5Detector.h"
#include <QElapsedTimer>

class VideoFileThread  : public QThread
{
	Q_OBJECT

public:
	explicit VideoFileThread(QObject *parent = nullptr);
	~VideoFileThread();
    // 设置视频文件路径
    void setVideoPath(const QString& path);

    // 停止播放
    void stop();

    //ROI
    void setROI(const cv::Rect& rect);
    void clearROI();
    //获取视频真实分辨率（用于主界面计算坐标映射）
    cv::Size getVideoSize() const { return m_videoSize; }
signals:
    void frameReady(const QImage& image); // 发送画面给界面
    void playFinished(); // 视频播放结束信号
    void errorMessage(const QString& msg);

protected:
    void run() override;

private:
    QString m_videoPath;
    YoloV5Detector* m_detector = nullptr;

    QMutex m_mutex;
    bool m_stopFlag = false;

    //视频尺寸信息
    cv::Size m_videoSize = cv::Size(0, 0);

    //ROI 相关变量
    QMutex m_roiMutex;
    cv::Rect m_roiRect;
    bool m_useROI = false;

    //采样计时器
    QElapsedTimer m_reportTimer;


    // 类别名称
    const std::vector<std::string> m_classNames = {
        "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
        "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
        "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa",
        "pottedplant", "bed", "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard",
        "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
        "scissors", "teddy bear", "hair drier", "toothbrush"
    };
};

