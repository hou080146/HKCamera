//指定文件的编码为UTF-8
#pragma execution_character_set("utf-8")

#include "hikcamera.h"

#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QThread>

// 初始化静态成员
QMap<LONG, HikCamera*> HikCamera::s_portMap;
QReadWriteLock  HikCamera::s_mapLock;

//HikCamera构造函数
HikCamera::HikCamera(QObject *parent) : QObject(parent)
{
    /*NET_DVR_Init();*/        //3.调用NET_DVR_Init()
    if (NET_DVR_Init()) {
        qDebug() << "[NET_DVR_Init]sdk初始化成功";
    }
    else {
        qDebug() << "sdk初始化出错，错误提示：" << NET_DVR_GetLastError;
    }
    QThread::msleep(3000);  // 给 PlayM4 DLL 时间初始化
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    
}

HikCamera::~HikCamera()
{
    stopPreview();
    QMutexLocker locker(&m_mutex);

    // 析构时从映射表移除
    {
        //QMutexLocker mapLocker(&s_mapMutex);
        //if (s_portMap.contains(m_playPort)) {
        //    s_portMap.remove(m_playPort);
        //}
        s_mapLock.lockForWrite();
        s_portMap.remove(m_playPort);
        s_mapLock.unlock();

    }

    // 关闭播放端口
    if (m_playPort != -1) {
        PlayM4_Stop(m_playPort);
        PlayM4_CloseStream(m_playPort);
        PlayM4_FreePort(m_playPort);
        m_playPort = -1; // 重置端口
        qDebug() << "[调试] 析构函数中释放 m_playPort（重置为0）";
    }
    // 注销用户
    if (m_userId != -1) {
        NET_DVR_Logout(m_userId);
        m_userId = -1;
    }
    NET_DVR_Cleanup();
}

//4.sdk初始化，调用信号连接：errorMessage/newFrame
bool HikCamera::init(const QString &ip, const QString &user, const QString &pwd, int port)
{
    QMutexLocker locker(&m_mutex);
    m_ip = ip;
    m_user = user;
    m_password = pwd;
    m_port = port;

    NET_DVR_USER_LOGIN_INFO loginInfo = { 0 };
    loginInfo.bUseAsynLogin = 0;
    strncpy(loginInfo.sDeviceAddress, ip.toUtf8().constData(), NET_DVR_DEV_ADDRESS_MAX_LEN);
    strncpy(loginInfo.sUserName, user.toUtf8().constData(), NAME_LEN);
    strncpy(loginInfo.sPassword, pwd.toUtf8().constData(), PASSWD_LEN);
    loginInfo.wPort = port;

    NET_DVR_DEVICEINFO_V40 deviceInfo = { {0} };
    m_userId = NET_DVR_Login_V40(&loginInfo, &deviceInfo);  //5.获取登录句柄
    qDebug() << "[NET_DVR_Login_V40]登录句柄为:" << m_userId;

    if (m_userId < 0) {
        emit errorOccurred(QString("登录失败，错误码：%1").arg(NET_DVR_GetLastError()));
        if (m_playPort != -1) PlayM4_FreePort(m_playPort);
        m_playPort = -1; // 清理已分配的资源
        return false;
    }


    //6.解码初始化
    //海康PlayM4（Playback SDK）存在历史遗留问题（海康官方论坛有人反馈过）
    //debug模式下概率SDK内部线程不安全导致资源访问冲突
    if (!PlayM4_GetPort(&m_playPort)) {
        emit errorOccurred("获取播放端口失败");
        return false;
    }
    qDebug() << "[PlayM4_GetPort] 解码端口获取m_playPort为：" << m_playPort;

    {
        //QMutexLocker mapLocker(&s_mapMutex);
        s_mapLock.lockForWrite();
        s_portMap[m_playPort] = this;
        s_mapLock.unlock();
    }

    if (!PlayM4_SetStreamOpenMode(m_playPort, STREAME_REALTIME) ||
        !PlayM4_OpenStream(m_playPort, nullptr, 0, 1440 * 2560) ||
        !PlayM4_SetDecCallBackExMend(m_playPort, DecodeCallback, 0, 0, 0) ||
        !PlayM4_Play(m_playPort, nullptr)) {
        emit errorOccurred("播放器初始化失败");
        return false;
    }
    else {
        qDebug() << "【PlayM4_SetDecCallBackExMend】注册的this指针原始地址：" << reinterpret_cast<LONG_PTR>(this);
    }
    // 替代方案：使用 PlayM4_SetDecCallBackEx


    return true;
}

//7.启动预览
void HikCamera::startPreview()
{
    QMutexLocker locker(&m_mutex);
    NET_DVR_PREVIEWINFO previewInfo = { 0 };
    previewInfo.lChannel = 1;
    previewInfo.dwStreamType = 0;
    previewInfo.dwLinkMode = 0;
    previewInfo.bBlocked = 1;

    m_playHandle = NET_DVR_RealPlay_V40(m_userId, &previewInfo, RealDataCallback, this);     //8.获取窗口句柄    9.注册RealDataCallback回调
    qDebug() << "【NET_DVR_RealPlay_V40】注册的this指针原始地址：" << this;

    qDebug() << "[NET_DVR_RealPlay_V40]初始窗口句柄：" << m_playHandle;
    if (m_playHandle < 0) {
        emit errorOccurred(QString("启动预览失败，错误码：%1").arg(NET_DVR_GetLastError()));
    }
}

// 实时数据回调（SDK内部线程）
void CALLBACK HikCamera::RealDataCallback(LONG, DWORD dwDataType,
    BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
    //qDebug() << "【RealDataCallback】pUser值：" << pUser;

    HikCamera* camera = reinterpret_cast<HikCamera*>(pUser);
    if (camera && dwDataType == NET_DVR_STREAMDATA) {
        //qDebug() << "[RealDataCallback]接受到的视频流数据不为空且满足格式要求！";
    }
    if (!camera || dwDataType != NET_DVR_STREAMDATA) return;
    //qDebug() << "[RealDataCallback]pBuffer地址为：" << pBuffer << " 大小为：" << dwBufSize;
    QMutexLocker locker(&camera->m_mutex); // 加锁
    if (camera->m_playPort == -1) {
        qDebug() << "资源已释放";
        return;
    }

    //10.收到视频流数据,发送到解码回调DecodeCallback
    if (dwBufSize > 0) {
        PlayM4_InputData(camera->m_playPort, pBuffer, dwBufSize);
    }
    //qDebug() << "[RealDataCallback] m_playPort:" << camera->m_playPort;
}

// 解码回调:将YUV数据转换为BGR格式，并生成QImage发送frameUpdated信号
void CALLBACK HikCamera::DecodeCallback(long nPort, char* pBuf, long nSize,
    FRAME_INFO* pFrameInfo, long, long)
{
    // 1) 从静态映射表查找 camera 实例
    s_mapLock.lockForRead();
    HikCamera* camera = s_portMap.value(nPort, nullptr);
    s_mapLock.unlock();
    if (!camera) return;

    // 2) 仅处理 YV12 数据
    if (pFrameInfo->nType != T_YV12) return;

    int w = pFrameInfo->nWidth;
    int h = pFrameInfo->nHeight;

    // 3) YUV → RGB（OpenCV）
    cv::Mat yuv(h + h / 2, w, CV_8UC1, (uchar*)pBuf);
    cv::Mat rgb;
    cv::cvtColor(yuv, rgb, cv::COLOR_YUV2RGB_YV12);

    // 4) 复制到 QImage（必须 deep copy，否则 rgb 会释放）
    QImage img(rgb.data, w, h, rgb.step, QImage::Format_RGB888);
    QImage deepCopy = img.copy();  // 必须复制

    // 5) 限制频率（无锁原子变量）
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 last = camera->m_lastEmitMs.load(std::memory_order_relaxed);

    if (now - last < camera->m_frameInterval) return;
    camera->m_lastEmitMs.store(now, std::memory_order_relaxed);

    // 6) 用 Qt::QueuedConnection 把图像发送给 Qt 主线程
    QMetaObject::invokeMethod(
        camera,
        "onFrameReady",
        Qt::QueuedConnection,
        Q_ARG(QImage, deepCopy)
    );
}

void HikCamera::onFrameReady(const QImage &img)
{
    emit frameUpdated(img);
}


void HikCamera::stopPreview()
{
    if (m_playHandle != -1) {
        NET_DVR_StopRealPlay(m_playHandle);
        m_playHandle = -1;
        qDebug() << "[调试] 停止实时预览";
    }
}
