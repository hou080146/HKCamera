#include "mainwindow.h"
//#include "videowidget.h"
#include "GLVideoWidget.h"
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass)
{
    ui->setupUi(this);

    // 初始化摄像头线程
    // 假设你希望窗口固定为 1280x720
    setFixedSize(1280, 720);
    //ui->Video->setFixedSize(640, 480);
    m_cameraThread = new CameraThread(this);
    connect(m_cameraThread, &CameraThread::errorMessage, this, &mainwindow::handleError);
    // 用 OpenGL 控件替换 UI 上的 Video
    GLVideoWidget *glWidget = new GLVideoWidget(this);
    ui->videoLayout->addWidget(glWidget);

    connect(m_cameraThread, &CameraThread::newFrame,
        glWidget, &GLVideoWidget::updateFrame);
    //16（2）.触发重绘
    //connect(m_cameraThread, &CameraThread::newFrame, ui->Video, &VideoWidget::updateFrame);

    // 启动摄像头
    m_cameraThread->initialize("192.168.1.64", "admin", "ck145236"); 
    m_cameraThread->start();
}

//程序关闭阶段
mainwindow::~mainwindow()
{
    m_cameraThread->stopCapture();
    m_cameraThread->quit();
    m_cameraThread->wait();
    delete ui;
}

//提示错误
void mainwindow::handleError(const QString &msg)
{
    ui->statusBar->showMessage(msg, 5000);
}
