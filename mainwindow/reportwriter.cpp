#include "reportwriter.h"
#include <QDebug>
#include <QCoreApplication>
#include "AppConfig.h"

ReportWriter* ReportWriter::s_instance = nullptr;
int ReportWriter::m_reportInterval = 1000;
ReportWriter::ReportWriter(QObject* parent) : QThread(parent)
{
    // 确保保存目录存在
    setSaveDir("./reports");
    m_reportInterval = AppConfig::reportInterval;
}

ReportWriter::~ReportWriter()
{
    stop();
    wait();
}

ReportWriter* ReportWriter::instance()
{
    if (!s_instance) {
        s_instance = new ReportWriter(qApp); // 挂载在全局 Application 上
        s_instance->start(); // 自动启动
    }
    return s_instance;
}

void ReportWriter::setSaveDir(const QString& path)
{
    QMutexLocker locker(&m_mutex);
    m_saveDir = path;
    QDir dir(m_saveDir);
    if (!dir.exists()) dir.mkpath(".");

    // 创建图片子目录
    if (!dir.exists("images")) dir.mkdir("images");
}

void ReportWriter::pushItem(const ReportItem& item)
{
    QMutexLocker locker(&m_mutex);
    // 深拷贝一份图片，防止外部修改，虽然 item 传入时已经是拷贝的，但为了安全
    // 这里其实由调用者保证传入的是 clone 过的 mat 即可
    m_queue.enqueue(item);

    // 唤醒消费者线程
    m_condition.wakeOne();
}

void ReportWriter::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
    m_condition.wakeAll();
}

void ReportWriter::run()
{
    m_stopFlag = false;

    // 每天生成一个新的 CSV 文件，避免文件过大
    QString currentCsvName;
    QFile csvFile;
    QTextStream csvStream;

    while (!m_stopFlag) {
        ReportItem item;
        bool hasData = false;

        // 1. 从队列取数据 (加锁区)
        {
            QMutexLocker locker(&m_mutex);
            if (m_queue.isEmpty()) {
                // 如果没数据，就挂起线程，等待 pushItem 唤醒
                // 这样不占 CPU
                m_condition.wait(&m_mutex);

                // 唤醒后再次检查
                if (m_stopFlag) break;
                if (!m_queue.isEmpty()) {
                    item = m_queue.dequeue();
                    hasData = true;
                }
            }
            else {
                item = m_queue.dequeue();
                hasData = true;
            }
        }

        // 2. 处理数据 (无锁区，耗时操作在这里做)
        if (hasData) {
            //生成文件名
            QString dateStr = item.timestamp.toString("yyyy_MM_dd");
            QString timeStr = item.timestamp.toString("HH_mm_ss");

            // 检查是否需要创建新日期的 CSV
            QString csvPath = QString("%1/%2_report.csv").arg(m_saveDir).arg(dateStr);
            if (csvFile.fileName() != csvPath) {
                if (csvFile.isOpen()) csvFile.close();
                csvFile.setFileName(csvPath);

                bool isNew = !csvFile.exists();
                // Append 模式打开
                if (csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                    csvStream.setDevice(&csvFile);
                    csvStream.setCodec("GBK");
                    if (isNew) {
                        // 写入 BOM 防止 Excel 乱码
                        //csvStream << "\xEF\xBB\xBF";
                        csvStream << "time, class, confidence, imageURL\n";
                    }
                }
            }

            //保存图片
            QString imgName = QString("%1_%2.jpg").arg(timeStr).arg(item.className);
            QString imgFullPath = QString("%1/images/%2").arg(m_saveDir).arg(imgName);

            // OpenCV保存 (路径需处理中文)
#ifdef Q_OS_WIN
            std::string savePathStr = imgFullPath.toLocal8Bit().constData();
#else
            std::string savePathStr = imgFullPath.toStdString();
#endif
            // 可以在这里 resize 图片以减小体积，或者保存原图
            if (!item.img.empty()) {
                cv::imwrite(savePathStr, item.img);
            }

            // C. 写入 CSV
            if (csvFile.isOpen()) {
                // Excel 超链接格式: =HYPERLINK("路径", "显示文字")
                // 这里使用相对路径，方便文件夹移动
                //QString link = QString("=HYPERLINK(\"./images/%1\")").arg(imgName);
                QString link = QString("\"=HYPERLINK(\"\"./images/%1\"\", \"\"%1\"\")\"").arg(imgName);
                csvStream << "\t" << item.timestamp.toString("HH:mm:ss") << ","
                    << item.className << ","
                    << QString::number(item.confidence, 'f', 2) << ","
                    << link << "\n";
                
                csvStream.flush(); // 立即刷新到磁盘
            }

            // qDebug() << "已导出一条报表:" << item.className;
        }
    }

    if (csvFile.isOpen()) csvFile.close();
}

