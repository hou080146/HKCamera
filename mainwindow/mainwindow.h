#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "GLVideoWidget.h"
#include "camerathread.h"
#include <QCloseEvent>

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
    // void updateStatus();
private:
    Ui::mainwindowClass *ui;

    CameraThread* m_cameraThread = nullptr;
    GLVideoWidget* m_glWidget = nullptr;
};
