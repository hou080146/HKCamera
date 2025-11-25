#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "GLVideoWidget.h"
#include "camerathread.h"
#include <QCloseEvent>
#include <QFuture>          //用于异步监控
#include <QFutureWatcher>   //用于异步监控
#include "HCNetSDK.h" 

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    void sendStatueBar(const QString& msg);

protected:
    // 可选：重写 closeEvent 以便更优雅地处理关闭
    void closeEvent(QCloseEvent* event) override;

private slots:
    void handleError(const QString& msg);
    void saveConfig();
    void switchCamear();

    //测试连接按钮槽函数
    void onBtnTestClicked();

    // 测试完成后的回调
    void onTestFinished();
    void onRecordClicked();
private:
    Ui::mainwindowClass *ui;
    CameraThread* m_cameraThread = nullptr;
    GLVideoWidget* m_glWidget = nullptr;

    QString IP_1 = "";
    QString IP_2 = "";
    bool flagCamear = false;
    //用于异步等待测试结果
    QFutureWatcher<QString> m_testWatcher;
    void init();
};
