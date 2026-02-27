#pragma once
#include <Windows.h>
#include <QString>

struct MileageSharedData {
    double mileage;          // 当前里程(米)
    qint64 timestamp;        // 时间戳
    bool isValid;            // 数据是否有效
};

class MileageSharedMemory
{
public:
    // 共享内存名称
    static const TCHAR* MILEAGE_MAP_NAME;  // "ThirdTrackHectometer"

    MileageSharedMemory();
    ~MileageSharedMemory();

    bool initialize();
    bool writeMileageInfo(double mileage, qint64 timestamp);

    // 读取里程信息
    //bool readMileageInfo(MileageSharedData& data);

    void cleanup();
    bool isInitialized() const;

private:
    bool createSharedMemory(const TCHAR* mapName, HANDLE& hMapFile, LPVOID& pBuf);
    bool writeToMemory(LPVOID pBuf, const MileageSharedData& data);

private:
    // 第一个共享内存
    HANDLE m_hMapFile;
    LPVOID m_pBuf;

    bool m_initialized;
    qint64 lastTimeStamp;
};