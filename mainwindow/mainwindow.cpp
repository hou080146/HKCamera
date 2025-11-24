//指定文件的编码为UTF-8
#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include <qDebug>
#include "AppConfig.h"


mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass)
{
    ui->setupUi(this);
  
    m_cameraThread = new CameraThread(this);
    // 用 OpenGL 控件替换 UI 上的 Video
    m_glWidget = new GLVideoWidget(this);
    //UI初始化
    init();
    

    connect(m_cameraThread, &CameraThread::frameReady, m_glWidget, &GLVideoWidget::updateFrame);
    connect(m_cameraThread, &CameraThread::errorMessage, this, &mainwindow::handleError);
    
    // 启动摄像头
    sendStatueBar("正在初始化相机线程...");
    m_cameraThread->initialize(IP_1, "admin", "ck145236"); 
    m_cameraThread->start();
}

//程序关闭阶段
mainwindow::~mainwindow()
{
    if (m_cameraThread) {
        //线程停止循环
        m_cameraThread->stopCapture();

        //线程退出事件循环 (虽然 run 里面没有 exec，但以防万一)
        m_cameraThread->quit();

        //阻塞等待线程彻底结束
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

void mainwindow::init()
{
    //无边框模式
    //setWindowFlags(Qt::FramelessWindowHint);

    //读取qss
    QFile file("Ubuntu.qss");    // 如果 exe 同目录
    if (file.open(QFile::ReadOnly)) {
        QString style = QLatin1String(file.readAll());
        qApp->setStyleSheet(style);
    }

    IP_1 = AppConfig::IP_1;
    IP_2 = AppConfig::IP_2;
    ui->lineEdit_ip1->setText(IP_1);
    ui->lineEdit_ip2->setText(IP_2);
    connect(ui->lineEdit_ip1, &QLineEdit::textEdited,
        this, &mainwindow::saveConfig);
    connect(ui->lineEdit_ip2, &QLineEdit::textEdited,
        this, &mainwindow::saveConfig);
    connect(ui->btn_switchCamera, &QPushButton::clicked,
        this, &mainwindow::switchCamear);

    if (ui->videoLayout) {
        ui->videoLayout->addWidget(m_glWidget);
    }
    else {
        // 如果没有 layout，可以设为中心窗口测试
        // setCentralWidget(m_glWidget);
    }
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

void mainwindow::saveConfig()
{
    AppConfig::IP_1 = ui->lineEdit_ip1->text();
    AppConfig::IP_2 = ui->lineEdit_ip2->text();
    AppConfig::writeConfig();
}

void mainwindow::switchCamear()
{
    if (m_cameraThread) {
        //线程停止循环
        m_cameraThread->stopCapture();

        //线程退出事件循环 (虽然 run 里面没有 exec，但以防万一)
        m_cameraThread->quit();

        //阻塞等待线程彻底结束
        // 如果不 wait，线程还在跑，但 UI (glWidget) 已经被 delete ui 销毁了，
        // 线程再发信号就会导致 "Access Violation"
        if (!m_cameraThread->wait(3000)) {
            // 如果3秒还没退出来，强制终止（极端情况）
            qDebug() << "线程响应超时，强制终止";
            m_cameraThread->terminate();
            m_cameraThread->wait();
        }
    }
    if (flagCamear == false)
    {
        m_cameraThread->initialize(IP_2, "admin", "ck145236");
        m_cameraThread->start();
        flagCamear = true;
    }
    else
    {
        m_cameraThread->initialize(IP_1, "admin", "ck145236");
        m_cameraThread->start();
        flagCamear = false;
    }
}