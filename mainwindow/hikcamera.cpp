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
    //由于需要支持相机线程启停，将SDK初始化移到mainwindow构造中
    /*NET_DVR_Init();*/        //3.调用NET_DVR_Init()
    
    //QThread::msleep(3000);  // 给 PlayM4 DLL 时间初始化
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    
}

HikCamera::~HikCamera()
{
    LONG currentPort = m_playPort.load();
    if (m_playPort != -1) {
        s_mapLock.lockForWrite();
        s_portMap.remove(m_playPort);
        s_mapLock.unlock();
    }

    // 2. 第二步：停止预览和播放
    stopPreview();

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
    //NET_DVR_Cleanup(); 由于要支持反复启停线程，防止单线程清理全局资源
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
    //海康PlayM4（Playback SDK）存在历史遗留问题
    //debug模式下SDK内部线程不安全概率导致资源访问冲突
    // 获取端口
    LONG portNum = -1;
    if (!PlayM4_GetPort(&portNum)) {
        emit errorOccurred("获取播放端口失败");
        return false;
    }
    m_playPort.store(portNum); // 原子存储

    // 注册到 map
    s_mapLock.lockForWrite();
    s_portMap[portNum] = this;
    s_mapLock.unlock();
    DWORD dwBufSize = 10 * 1024 * 1024;
    if (!PlayM4_SetStreamOpenMode(m_playPort, STREAME_REALTIME) ||
        !PlayM4_OpenStream(m_playPort, nullptr, 0, dwBufSize) ||
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
    if (m_playHandle != -1) return; // 防止重复开启
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

    LONG port = camera->m_playPort.load(std::memory_order_relaxed);

    if (port != -1 && dwBufSize > 0) {
        if (!PlayM4_InputData(port, pBuffer, dwBufSize)) {
            // 只打印一次错误，防止刷屏
            static int errCount = 0;
            if (errCount++ % 100 == 0) {
                DWORD err = PlayM4_GetLastError(port);
                //qDebug() << "[PlayM4 Error] InputData failed! Error Code:" << err<< " BufSize:" << dwBufSize;
                emit camera->errorOccurred(QString("RealDataCallback Error Code %1").arg(err));
                // 如果缓冲区溢出 (错误码通常是 11)，可以尝试重置缓冲区（慎用，会花屏一下）
                // if (err == 11) PlayM4_ResetSourceBuffer(port);
            }
        }
    }
}

// 解码回调:将YUV数据转换为BGR格式，并生成QImage发送frameUpdated信号
void CALLBACK HikCamera::DecodeCallback(long nPort, char* pBuf, long nSize,
    FRAME_INFO* pFrameInfo, long, long)
{
    // 1. 查找实例
    s_mapLock.lockForRead();
    HikCamera* camera = s_portMap.value(nPort, nullptr);
    s_mapLock.unlock();
    if (!camera) return;

    // 2. 仅处理 YV12 (海康 SDK 默认输出)
    if (pFrameInfo->nType != T_YV12) return;

    int w = pFrameInfo->nWidth;
    int h = pFrameInfo->nHeight;

    // 3. 【关键】只做内存拷贝，不做耗时操作
    // YV12 的大小是 w * h * 1.5
    // 我们构建一个 cv::Mat 来 wrap 这块内存，然后 deep copy 到 m_latestYUV
    // 注意：pBuf 是临时的，必须 clone
    {
        QMutexLocker locker(&camera->m_frameMutex);

        // 构造一个临时的 Mat 头指向 pBuf (不拷贝数据)
        // YV12 在 OpenCV 中可以看作是高度为 h * 1.5 的单通道图
        cv::Mat tempYUV(h + h / 2, w, CV_8UC1, (uchar*)pBuf);

        // 拷贝到成员变量 (Deep Copy)
        tempYUV.copyTo(camera->m_latestYUV);

        camera->m_hasNewFrame.store(true, std::memory_order_release);
    }

    // 结束！不发信号，不转 RGB，不做 QImage。耗时 < 1ms。
}

// 【新增】获取最新帧
bool HikCamera::getLatestFrame(cv::Mat& outYUV)
{
    if (!m_hasNewFrame.load(std::memory_order_acquire)) return false;

    QMutexLocker locker(&m_frameMutex);
    if (m_latestYUV.empty()) return false;

    // 拷贝出去供处理 (Deep Copy)
    m_latestYUV.copyTo(outYUV);

    // 标记已读取，避免重复处理同一帧（可选，看你是否需要丢帧策略）
    // 如果希望尽量高帧率，可以设为 false
    m_hasNewFrame.store(false, std::memory_order_release);

    return true;
}

void HikCamera::stopPreview()
{

    // 如果正在录像，先停止
    //if (m_isRecording.load()) {
     //   stopRecord();
    //}
    if (m_playHandle != -1) {
        NET_DVR_StopRealPlay(m_playHandle);
        m_playHandle = -1;
        qDebug() << "[调试] 停止实时预览";
    }
}

/*
//开始录像
bool HikCamera::startRecord(const QString& savePath)
{
    QMutexLocker locker(&m_mutex);

    if (m_playHandle == -1) {
        qDebug() << "错误：预览未开启，无法录像";
        return false;
    }

    if (m_isRecording) return true; // 已经在录了

    QByteArray pathBytes = savePath.toLocal8Bit();

    // 调用 SDK
    if (!NET_DVR_SaveRealData(m_playHandle, pathBytes.data())) {
        qDebug() << "录像失败，错误码：" << NET_DVR_GetLastError();
        return false;
    }

    // 强制关键帧
    NET_DVR_MakeKeyFrame(m_userId, 1);

    m_isRecording.store(true);
    qDebug() << "开始录像：" << savePath;
    return true;
}

// 停止录像
void HikCamera::stopRecord()
{
    QMutexLocker locker(&m_mutex);

    if (!m_isRecording) return;

    if (!NET_DVR_StopSaveRealData(m_playHandle)) {
        qDebug() << "停止录像失败，错误码：" << NET_DVR_GetLastError();
    }
    else {
        qDebug() << "录像停止";
    }
    m_isRecording.store(false);
}
*/
