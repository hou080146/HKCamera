#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "videowidget.h"
#include "camerathread.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private slots:
    void handleError(const QString& msg);
    // void updateStatus();
private:
    Ui::mainwindowClass *ui;
    CameraThread* m_cameraThread;
};
