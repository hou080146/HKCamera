#include "videofilethread.h"
#include "reportwriter.h"
#include <QDebug>
#include <QElapsedTimer>


VideoFileThread::VideoFileThread(QObject* parent, EncoderMileageMgr* encodermileagemgr) : QThread(parent)
{
    m_encoderMileageMgr = encodermileagemgr;
}

VideoFileThread::~VideoFileThread()
{
    stop();
    wait();
}

void VideoFileThread::setVideoPath(const QString& path)
{
    QMutexLocker locker(&m_mutex);
    m_videoPath = path;
}

void VideoFileThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
}

void VideoFileThread::run()
{
    m_stopFlag = false;

    // 1. 初始化 YOLO
    //m_detector = new YoloV5Detector("yolov5s.onnx", cv::Size(640, 640), true);
    m_detector = new YoloV5Detector("best.onnx", cv::Size(640, 640), true);

    // 2. 打开视频文件
    // 【注意】Windows下 OpenCV 处理中文路径建议使用 toLocal8Bit
    cv::VideoCapture cap;
    QString currentPath;
    {
        QMutexLocker locker(&m_mutex);
        currentPath = m_videoPath;
    }

#ifdef Q_OS_WIN
    // 解决中文路径读取失败问题
    cap.open(currentPath.toLocal8Bit().constData());
#else
    cap.open(currentPath.toStdString());
#endif

    if (!cap.isOpened()) {
        emit errorMessage("无法打开视频文件: " + currentPath);
        delete m_detector;
        return;
    }

    //保存视频分辨率
    m_videoSize.width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    m_videoSize.height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // 3. 获取视频帧率，计算帧间隔
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0) fps = 25.0; // 默认值防止除0
    int frameDelayMs = 1000 / fps; // 比如 25fps -> 40ms

    cv::Mat frame;
    QElapsedTimer timer; // 用于计算处理耗时
    m_reportTimer.start(); // 启动计时器
    int reportInterval = ReportWriter::m_reportInterval;
    while (!m_stopFlag) {
        timer.restart(); // 开始计时

        // 读取一帧
        if (!cap.read(frame)) {
            // 视频播放结束
            // 策略 A: 循环播放
            // cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            // continue;

            // 策略 B: 停止并发送信号 (这里采用策略B)
            qDebug() << "视频播放结束";
            emit playFinished();
            break;
        }

        if (frame.empty()) continue;


        // ==========================================
        // ROI 处理逻辑
        // ==========================================
        cv::Mat detectFrame;
        cv::Rect currentROI;
        bool isCropped = false;

        {
            QMutexLocker locker(&m_roiMutex);
            if (m_useROI && !m_roiRect.empty()) {
                // 边界交集保护
                currentROI = m_roiRect & cv::Rect(0, 0, frame.cols, frame.rows);

                if (currentROI.width > 0 && currentROI.height > 0) {
                    detectFrame = frame(currentROI).clone(); // 裁剪
                    isCropped = true;
                }
                else {
                    detectFrame = frame;
                }
            }
            else {
                detectFrame = frame;
            }
        }


        // --- YOLO 检测 ---
        auto detections = m_detector->detect(detectFrame, 0.80f, 0.45f);
        

#if 0:
        int debug_counter = 0;
        //测试裁剪
        if (isCropped && (debug_counter++ % 30 == 0)) {
            // A. 保存裁剪下来的原图 -> 检查 UI 坐标映射是否正确
            // 如果这张图和你框选的内容不一样，说明 MainWindow 的 scaleX/Y 算错了
            cv::imwrite("D:/debug_video_crop_raw.jpg", detectFrame);

            // B. 保存画了框的裁剪图 -> 检查 YOLO 对小图的检测是否准
            cv::Mat debugImg = detectFrame.clone();
            for (const auto& det : detections) {
                // 此时坐标是相对小图的，直接画
                cv::rectangle(debugImg, det.box, cv::Scalar(0, 0, 255), 2);
            }
            cv::imwrite("D:/debug_video_crop_result.jpg", debugImg);

            qDebug() << "视频回放调试：已保存调试图片到 D:/";
        }
#endif
        if (isCropped) {

            // 在 ROI 左上角显示 "ROI Mode"
            cv::putText(frame, "ROI Area", cv::Point(currentROI.x, currentROI.y - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
        }
        // --- 绘制结果 ---
        for (auto& det : detections) {
            //如果由ROI
            if (isCropped) {
                det.box.x += currentROI.x;
                det.box.y += currentROI.y;
            }
            cv::rectangle(frame, det.box, cv::Scalar(0, 255, 0), 2);

            std::string label = "sundries";
            //if (det.class_id < m_classNames.size()) {
            //    label = m_classNames[det.class_id];
            //}
            label += ": " + std::to_string((int)(det.confidence * 100)) + "%";

            cv::putText(frame, label, cv::Point(det.box.x, det.box.y - 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
        //采样保存到报表
        if (m_reportTimer.elapsed() > reportInterval) {

            // 2. 检查是否有需要保存的目标
            // 策略：保存当前帧所有检测到的目标，或者置信度最高的目标
            if (!detections.empty()) {

                // 这里演示：只保存第一个检测到的，或者你可以遍历保存所有
                // 为了演示简单，我们取第一个
                const auto& det = detections[0];

                // 3. 构造数据包
                ReportItem item;
                item.timestamp = QDateTime::currentDateTime();

                double currentMileage = 0.0;
                m_encoderMileageMgr->GetCurrentMileage(currentMileage);
                item.mileage = currentMileage;

#if 0:
                if (det.class_id < class_names.size()) {
                    item.className = QString::fromStdString(class_names[det.class_id]);
                }
                else {
                    item.className = "Unknown";
                }
#endif
                item.className = "sundries";//类别名称统一称为杂物

                item.confidence = det.confidence;

                //深拷贝  bgrFrame 在下一次循环会被覆盖，而 Writer 线程可能还没来得及写盘
                item.img = frame.clone();

                // 4. 推送到后台队列
                ReportWriter::instance()->pushItem(item);

                // 重置计时器
                m_reportTimer.restart();
            }
        }
        // --- 转 QImage 发送 ---
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage img((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameReady(img.copy());

        // --- 帧率控制 ---
        // 计算处理这一帧花了多少毫秒
        qint64 processingTime = timer.elapsed();

        // 如果处理太快（小于帧间隔），就需要睡一会儿，保证播放速度正常
        // 如果处理太慢（大于帧间隔），就不睡了
        int sleepTime = frameDelayMs - processingTime;
        if (sleepTime > 0) {
            QThread::msleep(sleepTime);
        }
    }

    // 清理
    cap.release();
    delete m_detector;
    m_detector = nullptr;
}


//设置 ROI
void VideoFileThread::setROI(const cv::Rect& rect)
{
    QMutexLocker locker(&m_roiMutex);
    m_roiRect = rect;
    m_useROI = true;
}

//清除 ROI
void VideoFileThread::clearROI()
{
    QMutexLocker locker(&m_roiMutex);
    m_useROI = false;
    m_roiRect = cv::Rect();
}