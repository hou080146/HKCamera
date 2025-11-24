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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QLineEdit *lineEdit_ip1;
    QLabel *label_ip1;
    QLabel *label_ip2;
    QLineEdit *lineEdit_ip2;
    QPushButton *btn_switchCamera;
    QWidget *tab_2;
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
        lineEdit_ip1 = new QLineEdit(tab);
        lineEdit_ip1->setObjectName(QStringLiteral("lineEdit_ip1"));
        lineEdit_ip1->setGeometry(QRect(70, 20, 171, 20));
        label_ip1 = new QLabel(tab);
        label_ip1->setObjectName(QStringLiteral("label_ip1"));
        label_ip1->setGeometry(QRect(30, 20, 54, 12));
        label_ip2 = new QLabel(tab);
        label_ip2->setObjectName(QStringLiteral("label_ip2"));
        label_ip2->setGeometry(QRect(30, 60, 54, 12));
        lineEdit_ip2 = new QLineEdit(tab);
        lineEdit_ip2->setObjectName(QStringLiteral("lineEdit_ip2"));
        lineEdit_ip2->setGeometry(QRect(70, 60, 171, 20));
        btn_switchCamera = new QPushButton(tab);
        btn_switchCamera->setObjectName(QStringLiteral("btn_switchCamera"));
        btn_switchCamera->setGeometry(QRect(1100, 40, 71, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

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

        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", Q_NULLPTR));
        label_ip1->setText(QApplication::translate("mainwindowClass", "IP_1:", Q_NULLPTR));
        label_ip2->setText(QApplication::translate("mainwindowClass", "IP_1:", Q_NULLPTR));
        btn_switchCamera->setText(QApplication::translate("mainwindowClass", "\345\210\207\346\215\242\347\233\270\346\234\272", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "Tab 1", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainwindowClass", "Tab 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
