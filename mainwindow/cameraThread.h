#pragma once

#include <QThread>
#include "hikcamera.h"

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    ~CameraThread();

    void initialize(const QString& ip, const QString& user,
        const QString& pwd, int port = 8000);
    void stopCapture();

signals:
    void errorMessage(const QString& msg);
    void newFrame(const QImage& frame);

protected:
    void run() override;

private:
    HikCamera* m_camera = nullptr;
    QString m_ip;
    QString m_user;
    QString m_pwd;
    int m_port;
    QMutex m_mutex;
};