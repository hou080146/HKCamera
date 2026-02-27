#include "SerialPortComm.h"
#include <QDebug>
#include <QDateTime>
SerialPortComm::SerialPortComm(QObject* parent)
    : QObject(parent)
    , m_serialPort(nullptr)
{
    m_serialPort = new QSerialPort(this);

    connect(m_serialPort, &QSerialPort::readyRead,this, &SerialPortComm::onReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred,this, &SerialPortComm::onErrorOccurred);
}

SerialPortComm::~SerialPortComm()
{
    closePort();
}

bool SerialPortComm::openPort(const QString& portName, qint32 baudRate)
{
    QMutexLocker locker(&m_mutex);

    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        m_receiveBuffer.clear();
        emit portOpened();
        qDebug() << "串口已打开:" << portName << "波特率:" << baudRate;
        return true;
    }
    else
    {
        QString errMsg = QString("打开串口失败: %1").arg(m_serialPort->errorString());
        emit errorOccurred(errMsg);
        qDebug() << errMsg;
        return false;
    }
}

void SerialPortComm::closePort()
{
    QMutexLocker locker(&m_mutex);

    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
        m_receiveBuffer.clear();
        emit portClosed();
        qDebug() << "串口已关闭";
    }
}

bool SerialPortComm::isOpen() const
{
    return m_serialPort->isOpen();
}

QStringList SerialPortComm::getAvailablePorts()
{
    QStringList portList;
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        portList << info.portName();
    }
    return portList;
}

bool SerialPortComm::sendEncoderData(qint64 pulseCount,bool isForward)
{
    QMutexLocker locker(&m_mutex);

    if (!m_serialPort->isOpen())
    {
        emit errorOccurred("串口未打开");
        return false;
    }

    /*
     * 二进制协议格式（4字节）：
     * [0] 帧头: 0xAA
     * [1-8] 脉冲计数：8字节
     * [9] 方向: 0x01=正向(+), 0x00=反向(-)
     * [10] 校验和: 0xFF
     * [11] 帧尾: 0x55
     */

    quint8 frame[12];
    frame[0] = 0xAA;  // 帧头
    memcpy(&frame[1], &pulseCount, sizeof(qint64));
    frame[9] = isForward ? 0x01 : 0x00;
    frame[10] = 0xFF;  // 校验和 、固定值
    frame[11] = 0x55;  // 帧尾

    qint64 bytesWritten = m_serialPort->write((const char*)frame, FRAME_LENGTH);
    m_serialPort->flush();

    if (bytesWritten == FRAME_LENGTH)
    {
        //qDebug() << "发送方向数据";
        return true;
    }
    else
    {
        emit errorOccurred("发送数据失败");
        return false;
    }
}

void SerialPortComm::onReadyRead()
{
    QMutexLocker locker(&m_mutex);

    QByteArray data = m_serialPort->readAll();

    for (int i = 0; i < data.size(); ++i)
    {
        m_receiveBuffer.append(static_cast<quint8>(data[i]));
    }

    parseProtocol();
}

void SerialPortComm::parseProtocol()
{
    // 至少需要12字节才能构成一帧
    while (m_receiveBuffer.size() >= FRAME_LENGTH)
    {
        // 查找帧头 0xAA
        int headerIndex = -1;
        for (int i = 0; i < m_receiveBuffer.size(); ++i)
        {
            if (m_receiveBuffer[i] == FRAME_HEADER)
            {
                headerIndex = i;
                break;
            }
        }

        // 没找到帧头，清空缓冲区
        if (headerIndex == -1)
        {
            m_receiveBuffer.clear();
            break;
        }

        // 移除帧头之前的数据
        if (headerIndex > 0)
        {
            m_receiveBuffer.remove(0, headerIndex);
        }

        // 检查是否有完整的帧 
        if (m_receiveBuffer.size() < FRAME_LENGTH)
        {
            break;
        }

        // 提取一帧数据
        QVector<quint8> frame;
        for (int i = 0; i < FRAME_LENGTH; ++i)
        {
            frame.append(m_receiveBuffer[i]);
        }

        // 验证帧
        if (verifyFrame(frame))
        {
            EncoderData encoderData;
            memcpy(&encoderData.pulseCount, &frame[1], sizeof(qint64));
            encoderData.direction = (frame[9] == 0x01) ? 1 : 0;
            encoderData.timestamp = QDateTime::currentMSecsSinceEpoch();
            emit encoderReceived(encoderData);
            m_receiveBuffer.remove(0, 4);
        }
        else
        {
            // 帧校验失败，移除帧头，继续查找
            m_receiveBuffer.remove(0, 1);
        }
    }
}

quint8 SerialPortComm::calculateChecksum(const quint8* data,int length)
{
    return 0xFF;
}

bool SerialPortComm::verifyFrame(const QVector<quint8>& frame)
{
    if (frame.size() != 4)
        return false;

    // 验证帧头
    if (frame[0] != FRAME_HEADER)
        return false;

    // 验证方向值 (只能是0x00或0x01)
    if (frame[9] != 0x00 && frame[9] != 0x01)
        return false;

    // 验证校验和
    quint8 calculatedSum = calculateChecksum((const quint8*)&frame[1], 9);
    if (frame[10] != calculatedSum)
        return false;

    // 验证帧尾
    if (frame[11] != FRAME_FOOTER)
        return false;

    return true;
}

void SerialPortComm::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError && error != QSerialPort::TimeoutError)
    {
        QString errMsg = QString("串口错误: %1").arg(m_serialPort->errorString());
        emit errorOccurred(errMsg);
        qDebug() << errMsg;
    }
}