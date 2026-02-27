#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QMutex>
#include <QVector>

struct EncoderData 
{
    qint64 pulseCount;
    qint8 direction;
    qint64 timestamp;

    EncoderData() : pulseCount(0), direction(1), timestamp(0) {}
};

class SerialPortComm : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortComm(QObject* parent = nullptr);
    ~SerialPortComm();

    // 串口操作
    bool openPort(const QString& portName, qint32 baudRate);
    void closePort();
    bool isOpen() const;

    // 获取可用串口列表
    static QStringList getAvailablePorts();

    // 发送数据
    bool sendEncoderData(qint64 pulseCount, bool isForward); // true=正向(+), false=反向(-)

signals:
    void encoderReceived(const EncoderData& data); 
    void errorOccurred(QString errorMsg);
    void portOpened();
    void portClosed();

private slots:
    void onReadyRead();
    void onErrorOccurred(QSerialPort::SerialPortError error);

private:
    QSerialPort* m_serialPort;
    QMutex m_mutex;  
    QVector<quint8> m_receiveBuffer;  // 接收缓冲区

    // 协议解析
    void parseProtocol();
    quint8 calculateChecksum(const quint8* data,int length);
    bool verifyFrame(const QVector<quint8>& frame);

    static const quint8 FRAME_HEADER = 0xAA;
    static const quint8 FRAME_FOOTER = 0x55;
    static const int FRAME_LENGTH = 12;//1+8+1+1+1
};