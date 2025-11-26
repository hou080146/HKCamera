#include "camerathread.h"
#include <QDebug>
#include <QImage> 
#include <QDateTime>
#include <QDir>

std::vector<std::string> class_names = {
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train",
    "truck", "boat", "traffic light", "fire hydrant", "stop sign", "parking meter",
    "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella",
    "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard",
    "sports ball", "kite", "baseball bat", "baseball glove", "skateboard",
    "surfboard", "tennis racket", "bottle", "wine glass", "cup", "fork",
    "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange",
    "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair",
    "sofa", "pottedplant", "bed", "diningtable", "toilet", "tvmonitor",
    "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave",
    "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
    "scissors", "teddy bear", "hair drier", "toothbrush"
};

CameraThread::CameraThread(QObject *parent) : QThread(parent) {}

CameraThread::~CameraThread()
{
    if (m_camera) {
        if (isRunning()) {
            stopCapture();
        }
        quit();
        wait(); // 等待线程结束
        
    }
    
}

void CameraThread::initialize(const QString& ip, const QString& user,
    const QString& pwd, int port)
{
    QMutexLocker locker(&m_mutex);
    m_ip = ip;
    m_user = user;
    m_pwd = pwd;
    m_port = port;
    m_stopFlag = false;
}

void CameraThread::stopCapture()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
}

//1.摄像头线程启动阶段
void CameraThread::run()
{
    m_stopFlag = false; // 每次启动前重置标志位
    m_camera = new HikCamera();    //2.创建HikCamera对象--->转到hikcamera
    m_writer = new cv::VideoWriter();
    // 创建 YOLO 对象
    // 加载 YOLO (确保 .onnx 文件路径正确)
    m_detector = new YoloV5Detector("yolov5s.onnx", cv::Size(640, 640), true);
    // 连接 YOLO 检测完成信号到 CameraThread::newFrame
    //connect(m_yolo, &YoloProcessor::processedFrame, this, &CameraThread::newFrame);
    //用于显示状态
    connect(m_camera, &HikCamera::errorOccurred, this, [this](const QString& err) { emit errorMessage(err); });

    //14（2）.信号传递到CameraThread::newFrame
    //connect(m_camera, &HikCamera::frameUpdated, this, &CameraThread::newFrame);

    QString ip, user, pwd;
    int port;
    {
        QMutexLocker locker(&m_mutex);
        ip = m_ip;
        user = m_user;
        pwd = m_pwd;
        port = m_port;
    }

     if (!m_camera->init(ip, user, pwd, port)) {
        delete m_camera;
        m_camera = nullptr;
        delete m_detector;
        m_detector = nullptr;
        return;
        }

    m_camera->startPreview();
    // 3. 开始处理循环 (替代原来的 exec())
    cv::Mat yuvFrame;
    cv::Mat bgrFrame;

    while (!m_stopFlag) {
        // A. 主动去取最新帧
        if (m_camera->getLatestFrame(yuvFrame)) {

            // B. 格式转换 YUV -> BGR (必须做，YOLO 需要 BGR)
            //    cv::COLOR_YUV2RGB_YV12 会转成 RGB，注意 OpenCV 默认是 BGR
            //    YOLO通常是用 BGR 训练的 (cv::imread)，所以这里最好转 BGR
            cv::cvtColor(yuvFrame, bgrFrame, cv::COLOR_YUV2BGR_YV12);


            // ==========================================
            // 录像逻辑
            // ==========================================
            bool shouldRecord = false;
            QString currentPathDir;
            {
                QMutexLocker locker(&m_mutex);
                shouldRecord = m_isRecordingRequest;
                currentPathDir = m_savePath; // 获取当前设置的路径
            }

            if (shouldRecord) {
                // 如果 Writer 没打开，就打开它
                if (!m_writer->isOpened()) {
                    QDir dir;
                    if (!dir.exists(currentPathDir)) dir.mkpath(currentPathDir);

                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
                    // 注意：为了兼容性，文件名转成标准 std::string
                    // 1920x1088 分辨率
                    // 25.0 FPS
                    // MJPG 编码 (文件大但CPU占用低，不卡顿)
                    // 拼接完整路径
                    QString fullPath = QString("%1/%2.avi").arg(currentPathDir).arg(fileName);

                    // 【关键】Windows下 OpenCV 处理中文路径可能需要 Local8Bit
                    // 如果路径包含中文，toStdString() 可能会乱码导致录像失败
#ifdef Q_OS_WIN
                    std::string savePathStr = fullPath.toLocal8Bit().constData();
#else
                    std::string savePathStr = fullPath.toStdString();
#endif

                    m_writer->open(savePathStr, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25.0, bgrFrame.size(), true);
                    if (m_writer->isOpened()) {
                        //qDebug() << "开始录像:" << fileName.c_str();
                    }
                    else {
                        qDebug() << "录像创建失败！";
                    }
                }

                // 写入帧
                if (m_writer->isOpened()) {
                    m_writer->write(bgrFrame);
                }
            }
            else {
                // 如果要求停止，且 Writer 是开着的，则关闭
                if (m_writer->isOpened()) {
                    m_writer->release();
                    qDebug() << "录像结束，文件已保存";
                }
            }
            // ==========================================


            // C. YOLO 推理
            //    直接把 bgrFrame 传进去
            auto detections = m_detector->detect(bgrFrame, 0.45f, 0.45f);

            // D. 绘制框 (在 bgrFrame 上直接画)
            for (const auto& det : detections) {
                cv::rectangle(bgrFrame, det.box, cv::Scalar(0, 255, 0), 2);
                std::string label = class_names[det.class_id] + ": " + std::to_string((int)(det.confidence * 100)) + "%";
                cv::putText(bgrFrame, label, cv::Point(det.box.x, det.box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            }


            


            // E. 转 QImage 发给 UI
            //    注意：QImage 格式是 RGB888，而 bgrFrame 是 BGR
            //    为了显示颜色正确，需要 cvtColor BGR2RGB，或者在 QImage 里交换 RB
            cv::cvtColor(bgrFrame, bgrFrame, cv::COLOR_BGR2RGB);

            QImage img((const uchar*)bgrFrame.data, bgrFrame.cols, bgrFrame.rows, bgrFrame.step, QImage::Format_RGB888);
            // 必须 deep copy，因为 bgrFrame 在下一次循环会被覆盖
            emit frameReady(img.copy());
        }
        else {
            // 如果没有新帧，稍微睡一下，避免 CPU 100%
            QThread::msleep(5);
        }
    }

    // 4. 清理
    if (m_camera) {
        m_camera->stopPreview();
        delete m_camera;
        m_camera = nullptr;
    }
    if (m_detector) {
        delete m_detector;
        m_detector = nullptr;
    }
}
/*
void CameraThread::toggleRecording()
{
    if (!m_camera) return;

    if (m_camera->isRecording()) {
        // 正在录 -> 停止
        m_camera->stopRecord();
        // 可以发信号告诉 UI 录像已停止
    }
    else {
        // 未录像 -> 开始

        // 1. 生成文件名 (例如: ./video/20231027_120000.mp4)
        QString dirPath = "./video";
        QDir dir;
        if (!dir.exists(dirPath)) dir.mkpath(dirPath);

        QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString fullPath = QString("%1/%2.mp4").arg(dirPath).arg(fileName);

        // 2. 调用相机开始录像
        m_camera->startRecord(fullPath);
    }
}
*/

void CameraThread::setRecordingState(bool isRecording)
{
    QMutexLocker locker(&m_mutex);
    m_isRecordingRequest = isRecording;
}

void CameraThread::setSavePath(const QString& path)
{
    QMutexLocker locker(&m_mutex);
    m_savePath = path;
}