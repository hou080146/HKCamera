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
    double mileage;
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

    void packCurrentSession();//打包

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
    QString m_currentSessionDir;
    QString m_rootSaveDir = "./reports";

    // 【新增】私有成员变量：文件句柄保持在类中，避免频繁创建
    QFile m_csvFile;
    QTextStream m_csvStream;
    QFile m_jsonFile;
    QString m_currentDateStr; // 记录当前文件对应的日期，用于判断跨天

    // 【新增】拆分出来的功能函数
    void processOneItem(const ReportItem& item); // 处理单条数据的总入口
    void rotateFiles(const QString& dateStr);    // 负责按日期切换文件
    QString saveImage(const ReportItem& item, const QString& timeStr); // 保存图片
    void writeToCsv(const ReportItem& item, const QString& imgName);   // 写 CSV
    void writeToJson(const ReportItem& item, const QString& imgName);  // 写 JSON
    void initSessionDir();

    bool zipDirectory(const QString& srcDir, const QString& destZip);//打包执行系统命令
public:
    static int m_reportInterval; // 采样间隔：1000ms (1秒)
};

