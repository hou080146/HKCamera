#include "hectometerClient.h"
#include <qbytearray.h>
#include <QDateTime>

hectometerClient::hectometerClient(QObject* parent) 
	:QObject(parent)
	, m_socket(new QTcpSocket(this))
	, m_heartbeatTimer(new QTimer(this))
	 ,m_serverPort(0)
	, m_sharedMemory(new MileageSharedMemory())
{
	m_latestMileage.mileage = 0;
	m_latestMileage.encoder = 0;
	m_latestMileage.timestamp = 0;

	connect(m_socket, &QTcpSocket::connected, this, &hectometerClient::onConnected);
	connect(m_socket, &QTcpSocket::disconnected, this, &hectometerClient::onDisconnected);
	connect(m_socket, &QTcpSocket::readyRead, this, &hectometerClient::onReadyRead);
	connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),this,&hectometerClient::onSocketError);

	//设置心跳定时器30s
	m_heartbeatTimer->setInterval(30000);
	connect(m_heartbeatTimer, &QTimer::timeout, this, &hectometerClient::sendHeartbeat);

	m_sharedMemory->initialize();
}

hectometerClient::~hectometerClient()
{
	disconnectServer();
	if (m_sharedMemory)
	{
		delete m_sharedMemory;
		m_sharedMemory = nullptr;
	}
}

void hectometerClient::connectServer(const QString& ip, qint64 port)
{
	if (m_socket->state() == QAbstractSocket::ConnectedState)
		disconnectServer();
	m_serverIp = ip;
	m_serverPort = port;
	m_socket->connectToHost(ip, port);
}

void hectometerClient::disconnectServer()
{
	m_heartbeatTimer->stop();
	if (m_socket->state() == QAbstractSocket::ConnectedState)
	{
		m_socket->disconnectFromHost();
		if (m_socket->state() == QAbstractSocket::UnconnectedState)
			m_socket->waitForDisconnected(1000);
	}
	m_receiveBuffer.clear();
}

bool hectometerClient::isConnected() const
{
	return m_socket->state() == QAbstractSocket::ConnectedState;
}

MileageInfo hectometerClient::getLatestMileageInfo() const
{
	return m_latestMileage;
}

void hectometerClient::onConnected()
{
	m_receiveBuffer.clear();
	m_heartbeatTimer->start();
	sendHeartbeat();
	emit connected();
}

void hectometerClient::onDisconnected()
{
	m_heartbeatTimer->stop();
	emit disconnected();
}

void hectometerClient::onReadyRead()
{
	QByteArray newData = m_socket->readAll();
	qDebug() << "Received data (hex):" << newData.toHex(' ');
	qDebug() << "Received data (string):" << newData;    
	qDebug() << "Data size:" << newData.size(); 
	m_receiveBuffer.append(newData);
	qDebug() << "Buffer contains \\0?" << m_receiveBuffer.contains('\0'); 
	qDebug() << "Buffer indexOf \\0:" << m_receiveBuffer.indexOf('\0');
	//按\0分割
	while (true)
	{
		int nullPos = m_receiveBuffer.indexOf('\0');
		if (nullPos == -1)
			break;
		//提取一个完整的json
		QByteArray jsonData = m_receiveBuffer.left(nullPos);
		m_receiveBuffer.remove(0, nullPos + 1);

		if (!jsonData.isEmpty())
			parseJsonData(jsonData);
		//if(!newData.isEmpty())
		//	parseJsonData(newData);
	}
}

void hectometerClient::onSocketError(QAbstractSocket::SocketError socketError)
{

}

void hectometerClient::sendHeartbeat()
{
	if (!isConnected())
		return;
	qint64 timestamp = QDateTime::currentSecsSinceEpoch();
	QJsonObject json;
	json["heartbeat"] = timestamp;

	sendJsonData(json);
}

void hectometerClient::sendJsonData(const QJsonObject& json)
{
	QJsonDocument doc(json);
	QByteArray data = doc.toJson(QJsonDocument::Compact);
	
	data.append("\0");
	qint64 written = m_socket->write(data);
	if (written == -1)
	{
		//发送失败
	}
}

void hectometerClient::parseJsonData(const QByteArray& data)
{
	QJsonParseError parseError;
	QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
	if (parseError.error != QJsonParseError::NoError)
		return;
	if (!doc.isObject())
		return;
	QJsonObject json = doc.object();
	if (json.contains("mileage"))
	{
		handleMileageInfo(json);
	}
	else if (json.contains("heartbeat"))
	{
		handleHeartbeat(json);
	}
	else {
		qWarning() << "未知格式" << data;
	}
}

void hectometerClient::handleHeartbeat(const QJsonObject& json)
{
	qint64 timestamp = json["heartbeat"].toDouble();
	qDebug() << timestamp;
}

void hectometerClient::handleMileageInfo(const QJsonObject& json)
{
	MileageInfo info;
	info.mileage = json["mileage"].toDouble();
	info.position = json["position"].toString();
	info.encoder = json["encoder"].toDouble();
	info.timestamp = json["timestamp"].toDouble();

	m_latestMileage = info;

	//写入共享内存
	if (m_sharedMemory)
		m_sharedMemory->writeMileageInfo(info.mileage, info.timestamp);

	emit mileageInfoReceived(info);
}