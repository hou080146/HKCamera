#pragma once
#include <QObject>
#include <opencv2/opencv.hpp>
#include "HCNetSDK.h"
#include <QMutex>
#include <QReadWriteLock> 
#include <QMap>
#include "plaympeg4.h"
#include <QTime>
#include <atomic>


class HikCamera : public QObject
{
    Q_OBJECT
public:
    explicit HikCamera(QObject *parent = nullptr);
    ~HikCamera();

    bool init(const QString& ip, const QString& user,
        const QString& pwd, int port = 8000);
    void startPreview();
    void stopPreview();
    bool getLatestFrame(cv::Mat& yuvFrame);

    // 录像控制
    //bool startRecord(const QString& savePath);
    //void stopRecord();
    //bool isRecording() const { return m_isRecording; }

signals:
    void errorOccurred(const QString& error);
//private slots:
//    void onFrameReady(const QImage& img);

private:
    static void CALLBACK DecodeCallback(long nPort, char* pBuf, long nSize,
        FRAME_INFO* pFrameInfo, long nUser, long nReserved2);
    static void CALLBACK RealDataCallback(LONG lPlayHandle, DWORD dwDataType,
        BYTE* pBuffer, DWORD dwBufSize, void* pUser);

    // 添加静态映射表和互斥锁
    static QMap<LONG, HikCamera*> s_portMap;
    //static QMutex s_mapMutex;
    static QReadWriteLock s_mapLock;  // 使用 QReadWriteLock 替换 QMutex

    std::atomic<qint64> m_lastEmitMs{ 0 };
    int m_frameInterval = 32;

    LONG m_userId = -1;
    LONG m_playHandle = -1;
    // 使用原子变量，避免在回调中加锁
    std::atomic<LONG> m_playPort{ -1 };
    QMutex m_mutex;
    QString m_ip;
    QString m_user;
    QString m_password;
    int m_port;

    QMutex m_frameMutex;        // 专用的帧锁
    cv::Mat m_latestYUV;        // 存放 YV12 原始数据
    std::atomic<bool> m_hasNewFrame{ false }; // 标记是否有新帧

    //std::atomic<bool> m_isRecording{ false }; // 录像状态标记
};