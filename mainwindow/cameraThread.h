#pragma once

#include <QThread>
#include "hikcamera.h"
#include "YoloV5Detector.h"


class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    ~CameraThread();
    //void toggleRecording(); // 切换录像状态（开/关）
    void initialize(const QString& ip, const QString& user,
        const QString& pwd, int port = 8000);
    void stopCapture();
    void setRecordingState(bool isRecording);
    void setSavePath(const QString& path);
signals:
    void errorMessage(const QString& msg);
    void frameReady(const QImage& image); // 发送给 UI 显示 (带框)
    

protected:
    void run() override;

private:
    HikCamera* m_camera = nullptr;
    YoloV5Detector* m_detector = nullptr; // 使用你的新类
    QString m_ip;
    QString m_user;
    QString m_pwd;
    int m_port;
    QMutex m_mutex;
    bool m_stopFlag = false;

    bool m_isRecordingRequest = false; // UI 请求的状态
    cv::VideoWriter* m_writer = nullptr;
    QString m_savePath;
};