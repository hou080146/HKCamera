#include "camerathread.h"
#include <QDebug>
#include <QImage> 

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
        stopCapture();
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
    m_camera = new HikCamera();    //2.创建HikCamera对象--->转到hikcamera

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