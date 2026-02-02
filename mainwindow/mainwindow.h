#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "GLVideoWidget.h"
#include "camerathread.h"
#include <QCloseEvent>
#include <QFuture>          //用于异步监控
#include <QFutureWatcher>   //用于异步监控
#include "HCNetSDK.h" 
#include "videofilethread.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    void sendStatueBar(const QString& msg);
    void onBtnOpenPathClicked();
    void onBtnSelectPathClicked();
private:
    void init();
    void setROI();
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

    //视频回放
    void onBtnOpenVideoClicked(); // 打开视频按钮槽函数
    void onVideoFinished();       // 视频播完的处理

    //ROI
    //响应 GLWidget 的画框信号
    void onRoiSelected(QRect uiRect, QSize widgetSize);

    //清除选区，恢复全屏检测
    void onBtnClearRoiClicked();
    void onBtnSelectRoiClicked();
private:
    Ui::mainwindowClass *ui;
    //海康监控
    CameraThread* m_cameraThread = nullptr;
    //显示控件
    GLVideoWidget* m_glWidget = nullptr;
    //视频回放
    VideoFileThread* m_videoThread = nullptr;

    QString IP_1 = "";
    QString IP_2 = "";
    QString user1 = "";
    QString user2 = "";
    QString password1 = "";
    QString password2 = "";
    bool flagCamear = false;//相机标志位
    //用于异步等待测试结果
    QFutureWatcher<QString> m_testWatcher;
    QString m_savepath = "";
    
};
