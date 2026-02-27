#pragma once

#include <QThread>
#include "hikcamera.h"
#include "YoloV5Detector.h"
#include <QElapsedTimer>
#include "EncoderMileageMgr.h"

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    ~CameraThread();
    //void toggleRecording(); // 切换录像状态（开/关）
    void initialize(const QString& ip, const QString& user,
        const QString& pwd, int port = 8000, EncoderMileageMgr* mileageService = nullptr);
    void stopCapture();

    //录像
    void setRecordingState(bool isRecording);
    void setSavePath(const QString& path);

    // 设置ROI
    void setROI(const cv::Rect& rect);
    void clearROI();
signals:
    void errorMessage(const QString& msg);
    void frameReady(const QImage& image); // 发送给 UI 显示 (带框)
    

protected:
    void run() override;

private:
    HikCamera* m_camera = nullptr;
    YoloV5Detector* m_detector = nullptr;
    EncoderMileageMgr* m_encoderMileageMgr = nullptr;

    QString m_ip;
    QString m_user;
    QString m_pwd;
    int m_port;
    QMutex m_mutex;
    bool m_stopFlag = false;

    //录像
    bool m_isRecordingRequest = false; // UI 请求的状态
    cv::VideoWriter* m_writer = nullptr;
    QString m_savePath;

    //ROI
    QMutex m_roiMutex;
    cv::Rect m_roiRect; // 存储映射到视频真实分辨率的 ROI
    bool m_useROI = false;

    //报表生成//采样计时器
    QElapsedTimer m_reportTimer;
};