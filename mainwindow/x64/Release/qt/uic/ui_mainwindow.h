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
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_ip1;
    QLineEdit *lineEdit_ip1;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_ip2;
    QLineEdit *lineEdit_ip2;
    QLineEdit *lineEditSavePath;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *btnSelectPath;
    QWidget *widget3;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btn_test;
    QPushButton *btnRecord;
    QPushButton *btnOpenPath;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QStringLiteral("mainwindowClass"));
        mainwindowClass->resize(1298, 951);
        mainwindowClass->setMinimumSize(QSize(0, 0));
        mainwindowClass->setMaximumSize(QSize(1298, 951));
        centralWidget = new QWidget(mainwindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMaximumSize(QSize(1298, 894));
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
        tabWidget->setMinimumSize(QSize(1280, 150));
        tabWidget->setMaximumSize(QSize(1280, 150));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btn_switchCamera = new QPushButton(tab);
        btn_switchCamera->setObjectName(QStringLiteral("btn_switchCamera"));
        btn_switchCamera->setGeometry(QRect(280, 25, 71, 71));
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(1000, 10, 256, 101));
        textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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

        lineEditSavePath = new QLineEdit(tab);
        lineEditSavePath->setObjectName(QStringLiteral("lineEditSavePath"));
        lineEditSavePath->setEnabled(true);
        lineEditSavePath->setGeometry(QRect(380, 70, 241, 20));
        lineEditSavePath->setFrame(false);
        lineEditSavePath->setReadOnly(true);
        widget2 = new QWidget(tab);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(380, 20, 241, 41));
        horizontalLayout_3 = new QHBoxLayout(widget2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        btnSelectPath = new QPushButton(widget2);
        btnSelectPath->setObjectName(QStringLiteral("btnSelectPath"));

        horizontalLayout_3->addWidget(btnSelectPath);

        widget3 = new QWidget(tab);
        widget3->setObjectName(QStringLiteral("widget3"));
        widget3->setGeometry(QRect(890, 5, 91, 111));
        verticalLayout_2 = new QVBoxLayout(widget3);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 3, -1, 3);
        btn_test = new QPushButton(widget3);
        btn_test->setObjectName(QStringLiteral("btn_test"));

        verticalLayout_2->addWidget(btn_test);

        btnRecord = new QPushButton(widget3);
        btnRecord->setObjectName(QStringLiteral("btnRecord"));

        verticalLayout_2->addWidget(btnRecord);

        btnOpenPath = new QPushButton(widget3);
        btnOpenPath->setObjectName(QStringLiteral("btnOpenPath"));

        verticalLayout_2->addWidget(btnOpenPath);

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
        label_ip1->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\270\200IP:", Q_NULLPTR));
        label_ip2->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\272\214IP:", Q_NULLPTR));
        label->setText(QApplication::translate("mainwindowClass", "\345\275\225\345\203\217\344\277\235\345\255\230\350\267\257\345\276\204:", Q_NULLPTR));
        btnSelectPath->setText(QApplication::translate("mainwindowClass", "\344\277\256\346\224\271\350\267\257\345\276\204", Q_NULLPTR));
        btn_test->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\347\212\266\346\200\201", Q_NULLPTR));
        btnRecord->setText(QApplication::translate("mainwindowClass", "\345\275\225\345\203\217", Q_NULLPTR));
        btnOpenPath->setText(QApplication::translate("mainwindowClass", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "\345\212\237\350\203\275\346\240\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
