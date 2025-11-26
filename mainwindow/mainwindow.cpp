//指定文件的编码为UTF-8
#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include <qDebug>
#include "AppConfig.h"
#include <QTimer>
#include <QtConcurrent>
#include <QFileDialog>      // 【新增】文件对话框
#include <QDesktopServices> // 【新增】打开系统文件夹

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass)
{
    ui->setupUi(this);

    //由于需要支持相机线程启停，将原本在相机中的SDK初始化移到这
    if (NET_DVR_Init()) {
        qDebug() << "[NET_DVR_Init]sdk初始化成功";
    }
    else {
        qDebug() << "sdk初始化出错，错误提示：" << NET_DVR_GetLastError;
    }


    m_cameraThread = new CameraThread(this);
    // 用 OpenGL 控件替换 UI 上的 Video
    m_glWidget = new GLVideoWidget(this);
    //UI初始化
    init();
    
    // 3. 连接异步监听器
    connect(&m_testWatcher, &QFutureWatcher<QString>::finished, this, &mainwindow::onTestFinished);
    connect(m_cameraThread, &CameraThread::frameReady, m_glWidget, &GLVideoWidget::updateFrame);
    connect(m_cameraThread, &CameraThread::errorMessage, this, &mainwindow::handleError);
    
    // 启动摄像头
    sendStatueBar("正在初始化相机线程...");
    
    onBtnTestClicked();
    m_cameraThread->initialize(IP_1, "admin", "ck145236"); 
    m_cameraThread->start();
}

//程序关闭阶段
mainwindow::~mainwindow()
{
    if (m_cameraThread) {
        //线程停止循环
        if (m_cameraThread->isRunning()) {
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
            if (m_testWatcher.isRunning()) {
                m_testWatcher.waitForFinished();
            }
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
    this->setWindowTitle("智能视频监控系统 v1.0");
    this->setWindowIcon(QIcon(":/mainwindow/logo.PNG"));
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

    // 录像保存路径
    m_savepath = AppConfig::SavePath;
    ui->lineEditSavePath->setText(m_savepath);
    m_cameraThread->setSavePath(m_savepath); // 同步给线程

    connect(ui->lineEdit_ip1, &QLineEdit::textEdited,
        this, &mainwindow::saveConfig);
    connect(ui->lineEdit_ip2, &QLineEdit::textEdited,
        this, &mainwindow::saveConfig);
    connect(ui->btn_switchCamera, &QPushButton::clicked,
        this, &mainwindow::switchCamear);
    connect(ui->btn_test, &QPushButton::clicked,
        this, &mainwindow::onBtnTestClicked);

    // 连接录像按钮
    connect(ui->btnRecord, &QPushButton::clicked, this, &mainwindow::onRecordClicked);
    // 连接选择路径按钮
    connect(ui->btnSelectPath, &QPushButton::clicked, this, &mainwindow::onBtnSelectPathClicked);
    // 连接打开文件夹按钮
    connect(ui->btnOpenPath, &QPushButton::clicked, this, &mainwindow::onBtnOpenPathClicked);

    if (ui->videoLayout) {
        ui->videoLayout->addWidget(m_glWidget);
    }
    else {
        // 如果没有 layout，可以设为中心窗口测试
        // setCentralWidget(m_glWidget);
    }

    //防止相机没有初始化好用户点击
    ui->btn_switchCamera->setEnabled(false);
    ui->btn_switchCamera->setText("请稍等");
    ui->btnRecord->setEnabled(false);
    QTimer::singleShot(7000, this, [=]() {
        // 5秒后会自动执行这里的代码
        ui->btn_switchCamera->setEnabled(true);
        ui->btnRecord->setEnabled(true);
        ui->btn_switchCamera->setText("切换相机"); // 恢复原始文字
        });
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
    AppConfig::SavePath = ui->lineEditSavePath->text();
    AppConfig::writeConfig();
}

void mainwindow::switchCamear()
{
    
    ui->btn_switchCamera->setEnabled(false);
    ui->btn_switchCamera->setText("请稍等");
    QTimer::singleShot(5000, this, [=]() {
        // 5秒后会自动执行这里的代码
        ui->btn_switchCamera->setEnabled(true);
        ui->btn_switchCamera->setText("切换相机"); // 恢复原始文字
        });
    
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
        sendStatueBar("正在初始化相机线程...");
        flagCamear = true;
    }
    else
    {
        m_cameraThread->initialize(IP_1, "admin", "ck145236");
        m_cameraThread->start();
        sendStatueBar("正在初始化相机线程...");
        flagCamear = false;
    }

}


void mainwindow::onBtnTestClicked()
{
    ui->textBrowser->append("正在测试相机连接状态...");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->ensureCursorVisible();
    QString ip = IP_1;
    QString ip_1 = IP_2;
    QString user = "admin";      // ui->lineEditUser->text();
    QString pwd = "ck145236";    // ui->lineEditPwd->text();
    int port = 8000;
    


    // 3. 启动异步任务 (Lambda 表达式)
    // 这是一个后台任务，不会卡住界面
    QFuture<QString> future = QtConcurrent::run([=]() {
        // --- 这里是在后台线程运行的代码 ---
        QString result;
        // 构造登录参数
        NET_DVR_USER_LOGIN_INFO loginInfo = { 0 };
        loginInfo.bUseAsynLogin = 0; // 同步登录
        strncpy(loginInfo.sDeviceAddress, ip.toUtf8().constData(), NET_DVR_DEV_ADDRESS_MAX_LEN);
        strncpy(loginInfo.sUserName, user.toUtf8().constData(), NAME_LEN);
        strncpy(loginInfo.sPassword, pwd.toUtf8().constData(), PASSWD_LEN);
        loginInfo.wPort = port;

        NET_DVR_DEVICEINFO_V40 deviceInfo = { {0} };
       
        // 尝试登录
        LONG lUserID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);
        
        if (lUserID >= 0) {
            // 登录成功！
            // 立即注销，因为我们只是测试
            NET_DVR_Logout(lUserID);
            result.append("相机:"+ ip + " 登录成功"+ "\r\n");
            
        }
        else {
            // 登录失败
            DWORD errCode = NET_DVR_GetLastError();
            QString errStr;
            // 简单翻译几个常见错误
            if (errCode == 7) errStr = "连接超时 (IP不可达)";
            else if (errCode == 1) errStr = "用户名或密码错误";
            else errStr = QString("连接超时").arg(errCode);
            result.append("相机:" + ip + " 登录失败:" + errStr + "\r\n");
        }
        strncpy(loginInfo.sDeviceAddress, ip_1.toUtf8().constData(), NET_DVR_DEV_ADDRESS_MAX_LEN);
        // 尝试登录
        lUserID = NET_DVR_Login_V40(&loginInfo, &deviceInfo);

        if (lUserID >= 0) {
            // 登录成功！
            // 立即注销，因为我们只是测试
            NET_DVR_Logout(lUserID);
            result.append("相机:" + ip_1 + " 登录成功");
            return result;
        }
        else {
            // 登录失败
            DWORD errCode = NET_DVR_GetLastError();
            QString errStr;
            // 简单翻译几个常见错误
            if (errCode == 7) errStr = "连接超时";
            else if (errCode == 1) errStr = "用户名或密码错误";
            else errStr = QString("连接超时").arg(errCode);
            result.append("相机:" + ip_1 + " 登录失败:" + errStr);
            return result;
        }

        });

    // 4. 将任务交给 Watcher 监控
    m_testWatcher.setFuture(future);
}

// ---------------------------------------------------------
// 测试任务完成 (自动回到主线程)
// ---------------------------------------------------------
void mainwindow::onTestFinished()
{
    // 1. 获取结果
    QString result = m_testWatcher.result();



    // 3. 显示结果 (状态栏 或 弹窗)
    ui->textBrowser->append("测试结果: \r\n" + result);
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->ensureCursorVisible();
    

}

void mainwindow::onRecordClicked()
{
    if (!m_cameraThread || !m_cameraThread->isRunning()) return;

    static bool isRec = false;
    isRec = !isRec;

    // 只需要设置状态，具体操作由线程循环去执行
    m_cameraThread->setRecordingState(isRec);
    int h = ui->btnRecord->height();  // 记录当前高度
    ui->btnRecord->setText(isRec ? "停止录像" : "开始录像");

    // 给个样式，录像时变红
    if (isRec)
    {
        
        ui->btnRecord->setStyleSheet("background-color: red; color: white;");
        
        // 录像开始时，禁用路径选择按钮，防止出错
        ui->btnSelectPath->setEnabled(false);
        ui->lineEditSavePath->setEnabled(false);
    }
    else 
    {
        ui->btnRecord->setStyleSheet("");
        // 录像停止后，恢复可用
        ui->btnSelectPath->setEnabled(true);
        ui->lineEditSavePath->setEnabled(true);
    }
    ui->btnRecord->setMinimumHeight(h);
    ui->btnRecord->setMaximumHeight(h);
    
}

void mainwindow::onBtnSelectPathClicked()
{
    // 正在录像时禁止修改路径，防止逻辑混乱
    if (ui->btnRecord->text().contains("停止")) {
        // 提示用户先停止录像
        ui->statusBar->showMessage("请先停止录像再修改路径！", 3000);
        return;
    }

    // 打开文件夹选择对话框
    QString dir = QFileDialog::getExistingDirectory(this, "选择录像保存路径",
        ui->lineEditSavePath->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->lineEditSavePath->setText(dir);
        saveConfig();
        // 将新路径传给线程
        if (m_cameraThread) {
            m_cameraThread->setSavePath(dir);
        }
    }
}

// 打开所在文件夹
void mainwindow::onBtnOpenPathClicked()
{
    QString path = ui->lineEditSavePath->text();
    QDir dir(path);

    // 如果目录不存在，先创建，防止打开失败
    if (!dir.exists()) {
        dir.mkpath(path);
    }

    // 调用系统资源管理器打开
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}