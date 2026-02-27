#include "camerathread.h"
#include <QDebug>
#include <QImage> 
#include <QDateTime>
#include <QDir>
#include "reportwriter.h"
#include "EncoderMileageMgr.h"

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
    const QString& pwd, int port, EncoderMileageMgr* mileageService)
{
    QMutexLocker locker(&m_mutex);
    m_ip = ip;
    m_user = user;
    m_pwd = pwd;
    m_port = port;
    m_stopFlag = false;
    m_encoderMileageMgr = mileageService;
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
    //m_detector = new YoloV5Detector("yolov5s.onnx", cv::Size(640, 640), true);
    m_detector = new YoloV5Detector("best.onnx", cv::Size(640, 640), true);
    //用于显示状态
    connect(m_camera, &HikCamera::errorOccurred, this, [this](const QString& err) { emit errorMessage(err); });

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
    m_reportTimer.start(); //报表采样计时器
    m_camera->startPreview();
    cv::Mat yuvFrame;
    //bgr原图
    cv::Mat bgrFrame;
    int reportInterval = ReportWriter::m_reportInterval;

    while (!m_stopFlag) {
        // A. 主动去取最新帧
        if (m_camera->getLatestFrame(yuvFrame)) {

            // B. 格式转换 YUV -> BGR (必须做，YOLO 需要 BGR)
            //    cv::COLOR_YUV2RGB_YV12 会转成 RGB，注意 OpenCV 默认是 BGR
            cv::cvtColor(yuvFrame, bgrFrame, cv::COLOR_YUV2BGR_YV12);

            cv::Mat detectFrame;     // 输入检测器
            cv::Rect currentROI;     // 当前使用的 ROI
            bool isCropped = false;

            //录像标志位和保存路径
            bool shouldRecord = false;
            QString currentPathDir;
            {
                QMutexLocker locker(&m_mutex);
                shouldRecord = m_isRecordingRequest;
                currentPathDir = m_savePath; // 获取当前设置的路径
            }


            {
                QMutexLocker locker(&m_roiMutex);
                if (m_useROI && !m_roiRect.empty()) {
                    // 边界保护：防止画框画出视频外导致崩馈
                    currentROI = m_roiRect & cv::Rect(0, 0, bgrFrame.cols, bgrFrame.rows);

                    if (currentROI.width > 0 && currentROI.height > 0) {
                        // 裁剪图像 (浅拷贝，不耗时)
                        detectFrame = bgrFrame(currentROI);
                        isCropped = true;
                    }
                    else {
                        detectFrame = bgrFrame; // ROI 无效，回退到全图
                    }
                }
                else {
                    detectFrame = bgrFrame; // 全图检测
                }
            }


            // ==========================================
            // 是否录像
            // ==========================================
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
            //    直接把 detectFrame 传进去
            auto detections = m_detector->detect(detectFrame, 0.80f, 0.45f);

            // D. 绘制框
            for (auto& det : detections) {
                //是否裁剪
                if (isCropped) {
                    det.box.x += currentROI.x;
                    det.box.y += currentROI.y;
                }
                // 在 ROI 左上角显示 "ROI Mode"
                cv::putText(bgrFrame, "ROI Area", cv::Point(currentROI.x, currentROI.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
                cv::rectangle(bgrFrame, det.box, cv::Scalar(0, 255, 0), 2);
                //std::string label = class_names[det.class_id] + ": " + std::to_string((int)(det.confidence * 100)) + "%";
                std::string label = "sundries: " + std::to_string((int)(det.confidence * 100)) + "%";
                cv::putText(bgrFrame, label, cv::Point(det.box.x, det.box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            }

            // 1. 时间检查：是否达到采样间隔
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

                    // 深拷贝 因为 bgrFrame 在下一次循环会被覆盖，而 Writer 线程可能还没来得及写盘
                    item.img = bgrFrame.clone();

                    // 4. 推送到后台队列
                    ReportWriter::instance()->pushItem(item);

                    // 重置计时器
                    m_reportTimer.restart();
                }
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


//设置 ROI
void CameraThread::setROI(const cv::Rect& rect)
{
    QMutexLocker locker(&m_roiMutex);
    m_roiRect = rect;
    m_useROI = true;
}

//清除 ROI
void CameraThread::clearROI()
{
    QMutexLocker locker(&m_roiMutex);
    m_useROI = false;
    m_roiRect = cv::Rect();
}