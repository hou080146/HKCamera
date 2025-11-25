/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindowClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QWidget *videoWidgetContainer;
    QGridLayout *gridLayout;
    QGridLayout *videoLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *btn_switchCamera;
    QTextBrowser *textBrowser;
    QPushButton *btn_test;
    QPushButton *btnRecord;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_ip1;
    QLineEdit *lineEdit_ip1;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_ip2;
    QLineEdit *lineEdit_ip2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QStringLiteral("mainwindowClass"));
        mainwindowClass->resize(1298, 951);
        mainwindowClass->setMaximumSize(QSize(1298, 951));
        centralWidget = new QWidget(mainwindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        videoWidgetContainer = new QWidget(centralWidget);
        videoWidgetContainer->setObjectName(QStringLiteral("videoWidgetContainer"));
        videoWidgetContainer->setMinimumSize(QSize(1280, 720));
        videoWidgetContainer->setMaximumSize(QSize(1280, 720));
        gridLayout = new QGridLayout(videoWidgetContainer);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        videoLayout = new QGridLayout();
        videoLayout->setSpacing(6);
        videoLayout->setObjectName(QStringLiteral("videoLayout"));

        gridLayout->addLayout(videoLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(videoWidgetContainer);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 150));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btn_switchCamera = new QPushButton(tab);
        btn_switchCamera->setObjectName(QStringLiteral("btn_switchCamera"));
        btn_switchCamera->setGeometry(QRect(280, 30, 71, 61));
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(1000, 10, 256, 101));
        btn_test = new QPushButton(tab);
        btn_test->setObjectName(QStringLiteral("btn_test"));
        btn_test->setGeometry(QRect(860, 30, 75, 23));
        btnRecord = new QPushButton(tab);
        btnRecord->setObjectName(QStringLiteral("btnRecord"));
        btnRecord->setGeometry(QRect(860, 70, 75, 23));
        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 20, 241, 41));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_ip1 = new QLabel(widget);
        label_ip1->setObjectName(QStringLiteral("label_ip1"));

        horizontalLayout->addWidget(label_ip1);

        lineEdit_ip1 = new QLineEdit(widget);
        lineEdit_ip1->setObjectName(QStringLiteral("lineEdit_ip1"));

        horizontalLayout->addWidget(lineEdit_ip1);

        widget1 = new QWidget(tab);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(30, 60, 241, 41));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_ip2 = new QLabel(widget1);
        label_ip2->setObjectName(QStringLiteral("label_ip2"));

        horizontalLayout_2->addWidget(label_ip2);

        lineEdit_ip2 = new QLineEdit(widget1);
        lineEdit_ip2->setObjectName(QStringLiteral("lineEdit_ip2"));

        horizontalLayout_2->addWidget(lineEdit_ip2);

        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);

        mainwindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1298, 23));
        mainwindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainwindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainwindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainwindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainwindowClass->setStatusBar(statusBar);

        retranslateUi(mainwindowClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", Q_NULLPTR));
        btn_switchCamera->setText(QApplication::translate("mainwindowClass", "\345\210\207\346\215\242\347\233\270\346\234\272", Q_NULLPTR));
        btn_test->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\347\212\266\346\200\201", Q_NULLPTR));
        btnRecord->setText(QApplication::translate("mainwindowClass", "\345\275\225\345\203\217", Q_NULLPTR));
        label_ip1->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\270\200IP:", Q_NULLPTR));
        label_ip2->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\272\214IP:", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "\345\212\237\350\203\275\346\240\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
