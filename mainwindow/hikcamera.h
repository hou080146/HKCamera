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

signals:
    void errorOccurred(const QString& error);
    void frameUpdated(const QImage& frame);
private slots:
    void onFrameReady(const QImage& img);

private:
    static void CALLBACK DecodeCallback(long nPort, char* pBuf, long nSize,
        FRAME_INFO* pFrameInfo, long nUser, long nReserved2);
    static void CALLBACK RealDataCallback(LONG lPlayHandle, DWORD dwDataType,
        BYTE* pBuffer, DWORD dwBufSize, void* pUser);

    // ÃÌº”æ≤Ã¨”≥…‰±Ì∫Õª•≥‚À¯
    static QMap<LONG, HikCamera*> s_portMap;
    //static QMutex s_mapMutex;
    static QReadWriteLock s_mapLock;  //  π”√ QReadWriteLock ÃÊªª QMutex

    std::atomic<qint64> m_lastEmitMs{ 0 };
    int m_frameInterval = 32;

    LONG m_userId = -1;
    LONG m_playHandle = -1;
    LONG m_playPort = -1;
    QMutex m_mutex;
    QString m_ip;
    QString m_user;
    QString m_password;
    int m_port;
};