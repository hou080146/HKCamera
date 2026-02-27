#include "MileageSharedMemory.h"
#include <QDebug>
#include <cstring>
#include <tchar.h> 

// 定义共享内存名称
const TCHAR* MileageSharedMemory::MILEAGE_MAP_NAME = _T("ThirdTrackHectometer");

MileageSharedMemory::MileageSharedMemory()
    : m_hMapFile(NULL)
    , m_pBuf(NULL)
    , m_initialized(false)
    , lastTimeStamp(0)
{
}

MileageSharedMemory::~MileageSharedMemory()
{
    cleanup();
}

bool MileageSharedMemory::initialize()
{
    if (m_initialized) {
        qDebug() << "共享内存已经初始化";
        return true;
    }

    if (!createSharedMemory(MILEAGE_MAP_NAME, m_hMapFile, m_pBuf)) {
        qWarning() << "创建共享内存失败:" << MILEAGE_MAP_NAME;
        cleanup();
        return false;
    }

    m_initialized = true;
    return true;
}

bool MileageSharedMemory::createSharedMemory(const TCHAR* mapName, HANDLE& hMapFile, LPVOID& pBuf)
{
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    
        NULL,                   
        PAGE_READWRITE,          
        0,                       // 高位
        sizeof(MileageSharedData), // 低位
        mapName);                

    if (hMapFile == NULL) {
        DWORD error = GetLastError();
        //qWarning() << "CreateFileMapping失败，错误代码:" << error;
        return false;
    }

    // 是否创建呢
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        //qDebug() << "共享内存已存在，打开已有的:" << QString::fromWCharArray(mapName);
    }
    else {
        //qDebug() << "创建新的共享内存:" << QString::fromWCharArray(mapName);
    }

    // 映射到进程地址
    pBuf = MapViewOfFile(
        hMapFile,               
        FILE_MAP_ALL_ACCESS,    
        0,                      
        0,                      
        sizeof(MileageSharedData)); 

    if (pBuf == NULL) {
        DWORD error = GetLastError();
        qWarning() << "MapViewOfFile失败，错误代码:" << error;
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return false;
    }

    MileageSharedData initData = { 0 };
    initData.isValid = false;
    memcpy(pBuf, &initData, sizeof(MileageSharedData));

    return true;
}

bool MileageSharedMemory::writeMileageInfo(double mileage, qint64 timestamp)
{
    if (!m_initialized) {
        qWarning() << "共享内存未初始化";
        return false;
    }
    MileageSharedData data;
    data.mileage = mileage;
    data.timestamp = timestamp;
    //data.encoder = encoder;
    data.isValid = true;

    // 转换位置字符串
    //QByteArray posBytes = position.toUtf8();
    //strncpy_s(data.position, sizeof(data.position), posBytes.constData(), _TRUNCATE);

    // 写入共享内存
    bool result = false;
   // if (lastTimeStamp != timestamp)
    {
       result = writeToMemory(m_pBuf, data);
    }
    
    lastTimeStamp = timestamp;
    if (result) {
        qDebug() << QString("写入共享内存成功: mileage=%1, timestamp=%2")
            .arg(mileage, 0, 'f', 3)
            .arg(timestamp);
    }

    return result;
}

bool MileageSharedMemory::writeToMemory(LPVOID pBuf, const MileageSharedData& data)
{
    if (pBuf == NULL) {
        return false;
    }

    try {
        // 互斥
        memcpy(pBuf, &data, sizeof(MileageSharedData));
        return true;
    }
    catch (...) {
        qWarning() << "写入异常";
        return false;
    }
}


//bool MileageSharedMemory::readMileageInfo(MileageSharedData& data)
//{
//    if (!m_initialized || m_pBuf == NULL) {
//        qWarning() << "共享内存未初始化";
//        return false;
//    }
//
//    try {
//        memcpy(&data, m_pBuf, sizeof(MileageSharedData));
//        return data.isValid;
//    }
//    catch (...) {
//        qWarning() << "读取异常";
//        return false;
//    }
//}


void MileageSharedMemory::cleanup()
{
    if (m_pBuf != NULL) {
        UnmapViewOfFile(m_pBuf);
        m_pBuf = NULL;
    }
    if (m_hMapFile != NULL) {
        CloseHandle(m_hMapFile);
        m_hMapFile = NULL;
    }
    m_initialized = false;
}

bool MileageSharedMemory::isInitialized() const
{
    return m_initialized;
}