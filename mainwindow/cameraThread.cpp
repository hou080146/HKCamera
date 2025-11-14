
#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent) {}

CameraThread::~CameraThread()
{
    stopCapture();
    quit();
    wait(1000); // 等待线程结束
    delete m_camera; // 确保在析构前删除
}

void CameraThread::initialize(const QString& ip, const QString& user,
    const QString& pwd, int port)
{
    QMutexLocker locker(&m_mutex);
    m_ip = ip;
    m_user = user;
    m_pwd = pwd;
    m_port = port;
}

void CameraThread::stopCapture()
{
    if (m_camera) m_camera->stopPreview();
}

//1.摄像头线程启动阶段
void CameraThread::run()
{
    m_camera = new HikCamera();    //2.创建HikCamera对象--->转到hikcamera

    //用于显示状态
    connect(m_camera, &HikCamera::errorOccurred, this,
        [this](const QString& err) { emit errorMessage(err); });

    //14（2）.信号传递到CameraThread::newFrame
    //15（1）.信号传递到VideoWidget::updateFrame
    connect(m_camera, &HikCamera::frameUpdated, this,
        &CameraThread::newFrame);

    QString ip, user, pwd;
    int port;
    {
        QMutexLocker locker(&m_mutex);
        ip = m_ip;
        user = m_user;
        pwd = m_pwd;
        port = m_port;
    }

     if (!m_camera->init(ip, user, pwd, port)) {
        delete m_camera;
        m_camera = nullptr;
        return;
        }

    m_camera->startPreview();
    exec();

    delete m_camera;
    m_camera = nullptr;
}