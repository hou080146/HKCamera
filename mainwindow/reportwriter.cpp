#include "reportwriter.h"
#include <QDebug>
#include <QCoreApplication>
#include "AppConfig.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QCoreApplication>


ReportWriter* ReportWriter::s_instance = nullptr;
int ReportWriter::m_reportInterval = 1000;
ReportWriter::ReportWriter(QObject* parent) : QThread(parent)
{
    // 默认根目录
    m_rootSaveDir = "./reports";

    // 立即初始化本次运行的文件夹
    initSessionDir();
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
    m_rootSaveDir = path;

    // 如果外部更改了根目录，重新生成 Session 目录
    initSessionDir();
}

void ReportWriter::initSessionDir()
{
    // 1. 获取当前启动时间作为文件夹名
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");

    // 2. 拼接完整路径: ./reports/2023_10_27_14_30_05
    m_currentSessionDir = QString("%1/%2").arg(m_rootSaveDir).arg(timeStr);

    // 3. 创建目录结构
    QDir dir(m_currentSessionDir);
    if (!dir.exists()) {
        dir.mkpath("."); // 创建 session 文件夹
    }

    // 4. 在 session 文件夹下创建 images 子文件夹
    if (!dir.exists("images")) {
        dir.mkdir("images");
    }

    qDebug() << "本次运行报表保存路径:" << m_currentSessionDir;
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
    m_currentDateStr.clear(); //以此强制第一次运行时创建文件

    while (!m_stopFlag) {
        ReportItem item;
        bool hasData = false;

        // --- 1. 线程安全地取数据 ---
        {
            QMutexLocker locker(&m_mutex);
            if (m_queue.isEmpty()) {
                m_condition.wait(&m_mutex);
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

        // --- 2. 交给处理函数 ---
        if (hasData) {
            processOneItem(item);
        }
    }

    // 线程结束，关闭所有文件
    if (m_csvFile.isOpen()) m_csvFile.close();
    if (m_jsonFile.isOpen()) m_jsonFile.close();
}


void ReportWriter::processOneItem(const ReportItem& item)
{
    QString dateStr = item.timestamp.toString("yyyy-MM-dd");
    QString timeStr = item.timestamp.toString("HH-mm-ss"); // 文件名用下划线
    QString fullTimeStr = item.timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz"); // 文本内容用标准格式

    // 1. 检查是否跨天（需要创建新文件）
    if (dateStr != m_currentDateStr) {
        rotateFiles(dateStr);
    }

    // 2. 保存图片 (返回文件名用于写入报表)
    QString imgName = saveImage(item, timeStr);

    // 3. 写入 CSV
    writeToCsv(item, imgName);

    // 4. 写入 JSON
    writeToJson(item, imgName);
}


void ReportWriter::rotateFiles(const QString& dateStr)
{
    m_currentDateStr = dateStr;

    // --- CSV 切换 ---
    if (m_csvFile.isOpen()) m_csvFile.close();

    QString csvPath = QString("%1/%2_report.csv").arg(m_currentSessionDir).arg(dateStr);
    m_csvFile.setFileName(csvPath);
    bool isNewCsv = !m_csvFile.exists();

    if (m_csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_csvStream.setDevice(&m_csvFile);
        m_csvStream.setCodec("GBK"); // Excel 友好编码
        if (isNewCsv) {
            m_csvStream << "time, mileage, class, confidence, imageURL\n";
        }
    }

    // --- JSON 切换 ---
    if (m_jsonFile.isOpen()) m_jsonFile.close();

    QString jsonPath = QString("%1/%2_report.json").arg(m_currentSessionDir).arg(dateStr);
    m_jsonFile.setFileName(jsonPath);

    // JSON 需要 ReadWrite 以便回退光标
    if (m_jsonFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        if (m_jsonFile.size() == 0) {
            m_jsonFile.write("[]"); // 初始化空数组
        }
    }
}


QString ReportWriter::saveImage(const ReportItem& item, const QString& timeStr)
{
    if (item.img.empty()) return "";

    // 构造文件名: 2023_10_27_14_00_00_person_0.95.jpg
    QString imgName = QString("%1-%2_%3_%4_%5.jpg")
        .arg(m_currentDateStr)
        .arg(timeStr)
        .arg(item.mileage)
        .arg(item.className)
        .arg(QString::number(item.confidence, 'f', 2));

    QString fullPath = QString("%1/images/%2").arg(m_currentSessionDir).arg(imgName);

    // OpenCV 保存
#ifdef Q_OS_WIN
    std::string savePathStr = fullPath.toLocal8Bit().constData();
#else
    std::string savePathStr = fullPath.toStdString();
#endif

    cv::imwrite(savePathStr, item.img);

    return imgName;
}


void ReportWriter::writeToCsv(const ReportItem& item, const QString& imgName)
{
    if (!m_csvFile.isOpen()) return;

    QString link = QString("\"=HYPERLINK(\"\"./images/%1\"\", \"\"%1\"\")\"").arg(imgName);

    // 使用 \t 防止 Excel 自动转换日期格式
    m_csvStream << "\t" << item.timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz") << ","
        << item.mileage << ","
        << item.className << ","
        << QString::number(item.confidence, 'f', 2) << ","
        << link << "\n";

    m_csvStream.flush();
}


void ReportWriter::writeToJson(const ReportItem& item, const QString& imgName)
{
    if (!m_jsonFile.isOpen()) return;

    // 1. 构建对象
    QJsonObject jsonObj;
    jsonObj["time"] = item.timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz");
    jsonObj["mileage"] = item.mileage;
    jsonObj["class"] = item.className;
    jsonObj["confidence"] = double(item.confidence);
    jsonObj["imagePath"] = "./images/" + imgName;

    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact); // 使用紧凑格式

    // 2. 插入逻辑 (回退覆盖 ']')
    if (m_jsonFile.size() > 1) {
        m_jsonFile.seek(m_jsonFile.size() - 1); // 倒退 1 字节

        if (m_jsonFile.pos() > 1) {
            m_jsonFile.write(","); // 如果不是第一个元素，加逗号
        }

        m_jsonFile.write(jsonData);
        m_jsonFile.write("]"); // 补上结尾
        m_jsonFile.flush();
    }
}

#if 1:
// 【新增】执行 ZIP 压缩的核心逻辑
bool ReportWriter::zipDirectory(const QString& srcDir, const QString& destZip)
{
    // 将路径转换为 Windows 原生格式 (反斜杠)，防止 PowerShell 识别出错
    QString nativeSrc = QDir::toNativeSeparators(srcDir);
    QString nativeDest = QDir::toNativeSeparators(destZip);

    // 构造 PowerShell 命令
    // Compress-Archive -Path "C:\..." -DestinationPath "D:\..." -Force
    // -Force 表示如果压缩包已存在则覆盖
    QString program = "powershell";
    QStringList arguments;
    arguments << "Compress-Archive"
        << "-Path" << "\"" + nativeSrc + "\""
        << "-DestinationPath" << "\"" + nativeDest + "\""
        << "-Force";

    qDebug() << "正在打包目录：" << nativeSrc;

    QProcess process;
    process.start(program, arguments);

    // 等待压缩完成（设置超时时间为 30秒，视文件大小而定）
    bool finished = process.waitForFinished(30000);

    if (finished && process.exitCode() == 0) {
        qDebug() << "打包成功：" << nativeDest;
        return true;
    }
    else {
        qDebug() << "打包失败：" << process.readAllStandardError();
        return false;
    }
}

// 【新增】对外接口
void ReportWriter::packCurrentSession()
{
    // 1. 暂时锁住，防止写入冲突
    QMutexLocker locker(&m_mutex);

    // 2. 强制刷新并关闭文件，释放句柄
    if (m_csvFile.isOpen()) {
        m_csvStream.flush();
        m_csvFile.close();
    }
    if (m_jsonFile.isOpen()) {
        m_jsonFile.close();
    }

    // 3. 确定压缩包路径
    // 假设目录是 ./reports/2023_10_27_10_00_00
    // 压缩包就是 ./reports/2023_10_27_10_00_00.zip
    QString zipPath = m_currentSessionDir + ".zip";

    // 4. 执行压缩
    zipDirectory(m_currentSessionDir, zipPath);

    // 5. 恢复文件打开状态（如果是软件运行中手动点击打包，还需要继续写）
    // 如果是软件关闭时调用，这里重新打开也无所谓，反正马上就析构了
    // 简单的做法：只在需要写下一条数据时，processOneItem 内部会自动重新打开
    // 所以这里不需要手动重新 open，非常方便！
}
#endif

#if 0:
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
            QString imgName = QString("%1_%2_%3.jpg").arg(item.timestamp.toString("yyyy_MM_dd_HH_mm_ss")).arg(item.className).arg(item.confidence);
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

#endif

