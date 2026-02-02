#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <opencv2/opencv.hpp>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

struct ReportItem {
	QDateTime timestamp;
	QString className;
	float confidence;
	cv::Mat img;
};

class ReportWriter  : public QThread
{
	Q_OBJECT
public:
    // 单例访问点
    static ReportWriter* instance();

    // 添加一条记录到队列（生产者调用）
    void pushItem(const ReportItem& item);

    // 设置保存根目录
    void setSaveDir(const QString& path);

    // 停止线程
    void stop();

protected:
    void run() override;

private:
    explicit ReportWriter(QObject* parent = nullptr);
    ~ReportWriter();

    static ReportWriter* s_instance;

    QQueue<ReportItem> m_queue;
    QMutex m_mutex;
    QWaitCondition m_condition; // 用于挂起和唤醒线程
    bool m_stopFlag = false;
    QString m_saveDir = "./reports";
public:
    static int m_reportInterval; // 采样间隔：1000ms (1秒)
};

