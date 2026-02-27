#include "EncoderMileageMgr.h"
#include <QDebug>
#include <QDateTime>
#include <QtMath>

EncoderMileageMgr::EncoderMileageMgr(QObject* parent)
    : QObject(parent)
    , m_serialComm(nullptr)
    , m_tcpClient(nullptr)
    , m_updateTimer(new QTimer(this))
    , m_isRunning(false)
    , m_startMileage(0.0)
    , m_currentMileage(0.0)
    , m_lastMileage(0.0)
    , m_lastPulse(0)
    , m_lastTimestamp(0)
    , m_currentDirection(1)
    , m_lastTcpMileage(0.0)
    , m_lastTcpTimestamp(0)
    , m_lastTcpPulse(0)
    , m_isCalibrated(false)
    , deltaPulse(0)
{
    // 计算每脉冲距离 = 0.5mm (转换为km)
    m_distancePerPulse = 0.5 / 1000.0 / 1000.0;

    m_updateTimer->setInterval(10);
    connect(m_updateTimer, &QTimer::timeout, this, &EncoderMileageMgr::onUpdateTimer);
}

EncoderMileageMgr::~EncoderMileageMgr()
{
    stop();
}

void EncoderMileageMgr::setSerialComm(SerialPortComm* serialComm)
{
    m_serialComm = serialComm;
    if (m_serialComm) {
        connect(m_serialComm, &SerialPortComm::encoderReceived,this, &EncoderMileageMgr::onEncoderDataReceived);
    }
}

void EncoderMileageMgr::setTcpClient(hectometerClient* tcpClient)
{
    m_tcpClient = tcpClient;
    if (m_tcpClient) {
        connect(m_tcpClient, &hectometerClient::mileageInfoReceived,this, &EncoderMileageMgr::onTcpMileageReceived);
    }
}

void EncoderMileageMgr::start(double startMileage)
{
    QMutexLocker locker(&m_mutex);

    if (m_isRunning) {
        return;
    }

    m_startMileage = startMileage;
    m_currentMileage = startMileage;
    m_lastMileage = startMileage;
    m_lastPulse = 0;
    m_lastTimestamp = QDateTime::currentMSecsSinceEpoch();
    m_currentDirection = 1;

    m_lastTcpMileage = 0.0;
    m_lastTcpTimestamp = 0;
    m_lastTcpPulse = 0;
    m_isCalibrated = false;

    m_isRunning = true;
    m_updateTimer->start();

    qDebug() << "已启动，起始里程:" << startMileage << "km";
}

void EncoderMileageMgr::stop()
{
    QMutexLocker locker(&m_mutex);

    if (!m_isRunning)
        return;

    m_isRunning = false;
    m_updateTimer->stop();

    qDebug() << "已停止，最终里程:" << m_currentMileage << "km";
}


void EncoderMileageMgr::manualCalibrate()//是否需要手动校准呢
{
    calibrateWithTcp();
}

MileageStatus EncoderMileageMgr::getCurrentStatus() const
{
    QMutexLocker locker(&m_mutex);

    MileageStatus status;
    status.currentMileage = m_currentMileage;
    status.totalPulse = m_lastPulse;
    status.direction = m_currentDirection;
    status.isCalibrated = m_isCalibrated;
    status.timestamp = QDateTime::currentMSecsSinceEpoch();

    // 速度
    qint64 deltaTime = status.timestamp - m_lastTimestamp;
    if (deltaTime > 0) {
        double deltaMileage = m_currentMileage - m_lastMileage;
        status.speed = qAbs(deltaMileage) / (deltaTime / 1000.0) * 3600.0; // km/h
    }
    else {
        status.speed = 0.0;
    }

    return status;
}

void EncoderMileageMgr::onEncoderDataReceived(const EncoderData& data)
{
    QMutexLocker locker(&m_mutex);

    if (!m_isRunning)
        return;

    m_lastPulse = data.pulseCount;
    m_currentDirection = data.direction;
}

void EncoderMileageMgr::onTcpMileageReceived(const MileageInfo& info)
{
    {//避免死锁
        QMutexLocker locker(&m_mutex);
        calibrateWithTcp();
        deltaPulse = m_lastTcpPulse - m_lastPulse;
        if (m_isCalibrated && m_lastTcpTimestamp > 0) {
            m_currentMileage = m_startMileage + (m_lastPulse + deltaPulse) * m_distancePerPulse;
        }
    }
    MileageStatus status = getCurrentStatus();
    emit mileageUpdated(status);
    m_lastMileage = m_currentMileage;
    m_lastTimestamp = QDateTime::currentMSecsSinceEpoch();
}

void EncoderMileageMgr::onUpdateTimer()
{
    QMutexLocker locker(&m_mutex);

    if (!m_isRunning)
        return;

    //calibrateWithTcp();

    //计算当前里程
    if (m_isCalibrated && m_lastTcpTimestamp > 0) {
        // 有TCP校准：基于最近的校准点计算
        //deltaPulse = m_lastTcpPulse - m_lastPulse;
        //double deltaDistance = deltaPulse * m_distancePerPulse * m_currentDirection;
        //m_currentMileage = m_lastTcpMileage + deltaDistance;
        m_currentMileage = m_startMileage + (m_lastPulse + deltaPulse) * m_distancePerPulse;
    }
    else {
        // 无校准
        double mileageFromPulse = calculateMileageFromPulse(m_lastPulse);
        m_currentMileage = m_startMileage + mileageFromPulse;
    }

    //计算速度并发送更新
    MileageStatus status = getCurrentStatus();
    emit mileageUpdated(status);

    qDebug() << QString("里程更新 - 当前:%1km, 速度:%2km/h, 脉冲:%3, 方向:%4, 校准:%5")
        .arg(m_currentMileage, 0, 'f', 5)
        .arg(status.speed, 0, 'f', 2)
        .arg(m_lastPulse)
        .arg(m_currentDirection)
        .arg(m_isCalibrated ? "是" : "否");

    m_lastMileage = m_currentMileage;
    m_lastTimestamp = QDateTime::currentMSecsSinceEpoch();
}

bool EncoderMileageMgr::calibrateWithTcp()
{
    if (!m_tcpClient || !m_tcpClient->isConnected())
        return false;

    MileageInfo tcpInfo = m_tcpClient->getLatestMileageInfo();

    // 是否更新
    if (tcpInfo.timestamp <= m_lastTcpTimestamp || tcpInfo.timestamp == 0)
        return false;

    // 记录校准点
    double oldMileage = m_currentMileage;
    m_lastTcpMileage = tcpInfo.mileage/1000;
    m_lastTcpTimestamp = tcpInfo.timestamp;
    m_lastTcpPulse = tcpInfo.encoder;
    m_isCalibrated = true;

    double offset = tcpInfo.mileage - oldMileage;

    qDebug() << QString("校准成功 - TCP里程:%1km, 本地里程:%2km, 偏移:%3km, 脉冲:%4")
        .arg(tcpInfo.mileage/1000, 0, 'f', 6)
        .arg(oldMileage, 0, 'f', 6)
        .arg(offset * 1000, 0, 'f', 6)
        .arg(m_lastPulse);

    emit calibrated(tcpInfo.mileage, oldMileage, offset);

    return true;
}

double EncoderMileageMgr::calculateMileageFromPulse(qint64 pulse)
{
    // 里程 = 脉冲数 × 每脉冲距离 × 方向
    return pulse * m_distancePerPulse * m_currentDirection;
}

void EncoderMileageMgr::GetCurrentMileage(double& currentMileage)
{
    if (!m_isRunning) {
        return;
    }
    {
        QMutexLocker locker(&m_mutex);
        currentMileage = m_currentMileage;
    }
}