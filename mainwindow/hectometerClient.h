#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include "MileageSharedMemory.h"

struct MileageInfo {
    double mileage;
    QString position;
    qint64 encoder;
    qint64 timestamp;
};

class hectometerClient :public QObject
{
    Q_OBJECT

public:
    explicit hectometerClient(QObject* parent = nullptr);
    ~hectometerClient();

    void connectServer(const QString& ip, qint64 port);
    void disconnectServer();
    bool isConnected() const;
    MileageInfo getLatestMileageInfo() const;

    bool initSharedMemory();

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& error);
    void mileageInfoReceived(const MileageInfo& info);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

    void sendHeartbeat();//心跳

private:
    void sendJsonData(const QJsonObject& json);
    void parseJsonData(const QByteArray & data);
    void handleHeartbeat(const QJsonObject& json);
    void handleMileageInfo(const QJsonObject& json);

    QTcpSocket* m_socket;
    QTimer* m_heartbeatTimer; //心跳计时器
    QByteArray m_receiveBuffer;
    MileageInfo m_latestMileage;
    QString m_serverIp;
    quint16 m_serverPort;

    MileageSharedMemory* m_sharedMemory;
};

