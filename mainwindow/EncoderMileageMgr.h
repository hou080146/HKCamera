#pragma once

#include <QObject>
#include <QTimer>
#include <QMutex>
#include "SerialPortComm.h"
#include "hectometerClient.h"

// 实时里程信息
struct MileageStatus {
    double currentMileage;      // 当前里程(km)
    double speed;               // 速度(km/h)
    qint64 totalPulse;          // 总脉冲数
    qint8 direction;            // 方向
    bool isCalibrated;          // 是否已校准
    qint64 timestamp;           // 时间戳
};

class EncoderMileageMgr : public QObject
{
    Q_OBJECT

public:
    explicit EncoderMileageMgr(QObject* parent = nullptr);
    ~EncoderMileageMgr();

    void setSerialComm(SerialPortComm* serialComm);
    void setTcpClient(hectometerClient* tcpClient);
    void start(double startMileage = 0.0);
    void stop();
    bool isRunning() const { return m_isRunning; }
    void manualCalibrate();
    MileageStatus getCurrentStatus() const;
    void GetCurrentMileage(double& currentMileage);

signals:
    void mileageUpdated(const MileageStatus& status);
    void calibrated(double tcpMileage, double localMileage, double offset);
    void errorOccurred(QString error);

private slots:
    void onEncoderDataReceived(const EncoderData& data);
    void onTcpMileageReceived(const MileageInfo& info);
    void onUpdateTimer();

private:
    bool calibrateWithTcp();

    double calculateMileageFromPulse(qint64 pulse);

    SerialPortComm* m_serialComm;
    hectometerClient* m_tcpClient;
    QTimer* m_updateTimer;
    mutable QMutex m_mutex;

    double m_distancePerPulse;           // 每脉冲距离(km)

    bool m_isRunning;
    double m_startMileage;               // 起始里程(km)
    double m_currentMileage;             // 当前里程(km)
    double m_lastMileage;                // 上次里程(km)
    qint64 m_lastPulse;                  // 上次脉冲
    qint64 m_lastTimestamp;              // 上次时间戳
    qint8 m_currentDirection;            // 当前方向

    double m_lastTcpMileage;             // 最后TCP里程
    qint64 m_lastTcpTimestamp;           // 最后TCP时间戳
    qint64 m_lastTcpPulse;               // TCP校准时的脉冲数
    bool m_isCalibrated;                 // 是否已校准

    qint64 deltaPulse;
};