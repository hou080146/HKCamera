//指定文件的编码为UTF-8
#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include <qDebug>


mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass)
{
    ui->setupUi(this);

  
    m_cameraThread = new CameraThread(this);
    
    // 用 OpenGL 控件替换 UI 上的 Video
    m_glWidget = new GLVideoWidget(this);

    if (ui->videoLayout) { 
        ui->videoLayout->addWidget(m_glWidget);
    }
    else {
        // 如果没有 layout，可以设为中心窗口测试
        // setCentralWidget(m_glWidget);
    }

    connect(m_cameraThread, &CameraThread::frameReady, m_glWidget, &GLVideoWidget::updateFrame);
    connect(m_cameraThread, &CameraThread::errorMessage, this, &mainwindow::handleError);
    
    // 启动摄像头
    sendStatueBar("正在初始化相机线程...");
    m_cameraThread->initialize("192.168.1.64", "admin", "ck145236"); 
    m_cameraThread->start();
}

//程序关闭阶段
mainwindow::~mainwindow()
{
    sendStatueBar("主窗口正在关闭，正在清理资源...");
    if (m_cameraThread) {
        // 1. 告诉线程停止循环
        m_cameraThread->stopCapture();

        // 2. 告诉线程退出事件循环 (虽然 run 里面没有 exec，但以防万一)
        m_cameraThread->quit();

        // 3. 【最重要】阻塞等待线程彻底结束
        // 如果不 wait，线程还在跑，但 UI (glWidget) 已经被 delete ui 销毁了，
        // 线程再发信号就会导致 "Access Violation"
        if (!m_cameraThread->wait(3000)) {
            // 如果3秒还没退出来，强制终止（极端情况）
            qDebug() << "线程响应超时，强制终止";
            m_cameraThread->terminate();
            m_cameraThread->wait();
        }
    }

    // 4. 销毁 UI
    // 此时线程已死，不会再有 frameReady 信号发给 glWidget，安全销毁
    delete ui;

    qDebug() << "主窗口资源清理完毕";
}

//提示错误
void mainwindow::handleError(const QString &msg)
{
    sendStatueBar("ERROR: " + msg);
}
void mainwindow::closeEvent(QCloseEvent* event)
{
    // 可以在这里做一些询问 "是否确认退出" 的逻辑
    // 目前直接接受关闭
    event->accept();
}

void mainwindow::sendStatueBar(const QString &msg)
{
    if (ui->statusBar && !msg.isEmpty()) {
        ui->statusBar->showMessage(msg, 5000);
    }
}