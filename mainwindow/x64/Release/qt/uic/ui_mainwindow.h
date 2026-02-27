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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
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
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_switchCamera;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btnSelectRoi;
    QSpacerItem *verticalSpacer;
    QPushButton *btnClearRoi;
    QPushButton *btnPack;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget_3;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QLineEdit *lineEditStartMileage;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_11;
    QLineEdit *lineEditSpeed;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QComboBox *upOrDownCom;
    QWidget *widget3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_12;
    QComboBox *directionCom;
    QPushButton *btnOpenVideo;
    QWidget *widget4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btn_test;
    QPushButton *btnRecord;
    QPushButton *btnOpenPath;
    QTextBrowser *textBrowser;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_ip1;
    QLineEdit *lineEdit_ip1;
    QLabel *label_2;
    QLineEdit *lineEdit_user_1;
    QLabel *label_3;
    QLineEdit *lineEdit_password_1;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_2;
    QLabel *label_ip2;
    QLineEdit *lineEdit_ip2;
    QLabel *label_5;
    QLineEdit *lineEdit_user_2;
    QLabel *label_4;
    QLineEdit *lineEdit_password_2;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_3;
    QLabel *label_8;
    QLineEdit *lineEdit_mileageIP;
    QLabel *label_9;
    QSpinBox *spinBox_mileagePort;
    QPushButton *btn_connectMileage;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *lineEditSavePath;
    QPushButton *btnSelectPath;
    QSpacerItem *horizontalSpacer;
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
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        btn_switchCamera = new QPushButton(tab);
        btn_switchCamera->setObjectName(QStringLiteral("btn_switchCamera"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_switchCamera->sizePolicy().hasHeightForWidth());
        btn_switchCamera->setSizePolicy(sizePolicy);
        btn_switchCamera->setMinimumSize(QSize(71, 71));
        btn_switchCamera->setMaximumSize(QSize(71, 71));

        horizontalLayout_4->addWidget(btn_switchCamera);

        horizontalSpacer_2 = new QSpacerItem(110, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        widget_2 = new QWidget(tab);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_4 = new QVBoxLayout(widget_2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        btnSelectRoi = new QPushButton(widget_2);
        btnSelectRoi->setObjectName(QStringLiteral("btnSelectRoi"));
        btnSelectRoi->setMinimumSize(QSize(71, 0));
        btnSelectRoi->setMaximumSize(QSize(71, 16777215));

        verticalLayout_4->addWidget(btnSelectRoi);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer);

        btnClearRoi = new QPushButton(widget_2);
        btnClearRoi->setObjectName(QStringLiteral("btnClearRoi"));
        btnClearRoi->setMinimumSize(QSize(71, 0));
        btnClearRoi->setMaximumSize(QSize(71, 16777215));

        verticalLayout_4->addWidget(btnClearRoi);

        btnPack = new QPushButton(widget_2);
        btnPack->setObjectName(QStringLiteral("btnPack"));

        verticalLayout_4->addWidget(btnPack);


        horizontalLayout_4->addWidget(widget_2);

        horizontalSpacer_3 = new QSpacerItem(110, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        widget_3 = new QWidget(tab);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        gridLayout_2 = new QGridLayout(widget_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget = new QWidget(widget_3);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);

        lineEditStartMileage = new QLineEdit(widget);
        lineEditStartMileage->setObjectName(QStringLiteral("lineEditStartMileage"));

        horizontalLayout->addWidget(lineEditStartMileage);


        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        widget1 = new QWidget(widget_3);
        widget1->setObjectName(QStringLiteral("widget1"));
        horizontalLayout_5 = new QHBoxLayout(widget1);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(widget1);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_5->addWidget(label_11);

        lineEditSpeed = new QLineEdit(widget1);
        lineEditSpeed->setObjectName(QStringLiteral("lineEditSpeed"));
        lineEditSpeed->setReadOnly(true);

        horizontalLayout_5->addWidget(lineEditSpeed);


        gridLayout_2->addWidget(widget1, 0, 1, 1, 1);

        widget2 = new QWidget(widget_3);
        widget2->setObjectName(QStringLiteral("widget2"));
        horizontalLayout_2 = new QHBoxLayout(widget2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(widget2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_2->addWidget(label_7);

        upOrDownCom = new QComboBox(widget2);
        upOrDownCom->setObjectName(QStringLiteral("upOrDownCom"));
        upOrDownCom->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(upOrDownCom);


        gridLayout_2->addWidget(widget2, 1, 0, 1, 1);

        widget3 = new QWidget(widget_3);
        widget3->setObjectName(QStringLiteral("widget3"));
        horizontalLayout_6 = new QHBoxLayout(widget3);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(widget3);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_6->addWidget(label_12);

        directionCom = new QComboBox(widget3);
        directionCom->setObjectName(QStringLiteral("directionCom"));
        directionCom->setMinimumSize(QSize(0, 25));
        directionCom->setDuplicatesEnabled(false);
        directionCom->setFrame(true);

        horizontalLayout_6->addWidget(directionCom);


        gridLayout_2->addWidget(widget3, 1, 1, 1, 1);


        horizontalLayout_4->addWidget(widget_3);

        btnOpenVideo = new QPushButton(tab);
        btnOpenVideo->setObjectName(QStringLiteral("btnOpenVideo"));
        sizePolicy.setHeightForWidth(btnOpenVideo->sizePolicy().hasHeightForWidth());
        btnOpenVideo->setSizePolicy(sizePolicy);
        btnOpenVideo->setMinimumSize(QSize(71, 71));
        btnOpenVideo->setMaximumSize(QSize(71, 71));
        btnOpenVideo->setFlat(false);

        horizontalLayout_4->addWidget(btnOpenVideo);

        widget4 = new QWidget(tab);
        widget4->setObjectName(QStringLiteral("widget4"));
        verticalLayout_2 = new QVBoxLayout(widget4);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 3, -1, 3);
        btn_test = new QPushButton(widget4);
        btn_test->setObjectName(QStringLiteral("btn_test"));

        verticalLayout_2->addWidget(btn_test);

        btnRecord = new QPushButton(widget4);
        btnRecord->setObjectName(QStringLiteral("btnRecord"));

        verticalLayout_2->addWidget(btnRecord);

        btnOpenPath = new QPushButton(widget4);
        btnOpenPath->setObjectName(QStringLiteral("btnOpenPath"));

        verticalLayout_2->addWidget(btnOpenPath);


        horizontalLayout_4->addWidget(widget4);

        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        horizontalLayout_4->addWidget(textBrowser);

        tabWidget->addTab(tab, QString());
        widget->raise();
        btn_switchCamera->raise();
        textBrowser->raise();
        btnOpenVideo->raise();
        widget_2->raise();
        widget_3->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_3 = new QHBoxLayout(tab_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_ip1 = new QLabel(groupBox);
        label_ip1->setObjectName(QStringLiteral("label_ip1"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_ip1);

        lineEdit_ip1 = new QLineEdit(groupBox);
        lineEdit_ip1->setObjectName(QStringLiteral("lineEdit_ip1"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_ip1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_user_1 = new QLineEdit(groupBox);
        lineEdit_user_1->setObjectName(QStringLiteral("lineEdit_user_1"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_user_1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_password_1 = new QLineEdit(groupBox);
        lineEdit_password_1->setObjectName(QStringLiteral("lineEdit_password_1"));
        lineEdit_password_1->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_password_1);


        horizontalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_2 = new QFormLayout(groupBox_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_ip2 = new QLabel(groupBox_2);
        label_ip2->setObjectName(QStringLiteral("label_ip2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_ip2);

        lineEdit_ip2 = new QLineEdit(groupBox_2);
        lineEdit_ip2->setObjectName(QStringLiteral("lineEdit_ip2"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEdit_ip2);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        lineEdit_user_2 = new QLineEdit(groupBox_2);
        lineEdit_user_2->setObjectName(QStringLiteral("lineEdit_user_2"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_user_2);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

        lineEdit_password_2 = new QLineEdit(groupBox_2);
        lineEdit_password_2->setObjectName(QStringLiteral("lineEdit_password_2"));
        lineEdit_password_2->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_password_2);


        horizontalLayout_3->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        formLayout_3 = new QFormLayout(groupBox_3);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);

        lineEdit_mileageIP = new QLineEdit(groupBox_3);
        lineEdit_mileageIP->setObjectName(QStringLiteral("lineEdit_mileageIP"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEdit_mileageIP);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_9);

        spinBox_mileagePort = new QSpinBox(groupBox_3);
        spinBox_mileagePort->setObjectName(QStringLiteral("spinBox_mileagePort"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, spinBox_mileagePort);

        btn_connectMileage = new QPushButton(groupBox_3);
        btn_connectMileage->setObjectName(QStringLiteral("btn_connectMileage"));
        btn_connectMileage->setMinimumSize(QSize(0, 25));

        formLayout_3->setWidget(2, QFormLayout::SpanningRole, btn_connectMileage);


        horizontalLayout_3->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(250, 0));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lineEditSavePath = new QLineEdit(groupBox_4);
        lineEditSavePath->setObjectName(QStringLiteral("lineEditSavePath"));
        lineEditSavePath->setEnabled(true);
        lineEditSavePath->setFrame(false);
        lineEditSavePath->setReadOnly(true);

        verticalLayout_3->addWidget(lineEditSavePath);

        btnSelectPath = new QPushButton(groupBox_4);
        btnSelectPath->setObjectName(QStringLiteral("btnSelectPath"));
        btnSelectPath->setMinimumSize(QSize(0, 25));

        verticalLayout_3->addWidget(btnSelectPath);


        horizontalLayout_3->addWidget(groupBox_4);

        horizontalSpacer = new QSpacerItem(330, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

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

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", Q_NULLPTR));
        btn_switchCamera->setText(QApplication::translate("mainwindowClass", "\345\210\207\346\215\242\347\233\270\346\234\272", Q_NULLPTR));
        btnSelectRoi->setText(QApplication::translate("mainwindowClass", "\351\200\211\345\217\226ROI", Q_NULLPTR));
        btnClearRoi->setText(QApplication::translate("mainwindowClass", "\346\270\205\351\231\244ROI", Q_NULLPTR));
        btnPack->setText(QApplication::translate("mainwindowClass", "\346\227\245\345\277\227\345\216\213\347\274\251", Q_NULLPTR));
        label_6->setText(QApplication::translate("mainwindowClass", "\345\274\200\345\247\213\351\207\214\347\250\213:", Q_NULLPTR));
        label_11->setText(QApplication::translate("mainwindowClass", "\351\200\237\345\272\246:", Q_NULLPTR));
        label_7->setText(QApplication::translate("mainwindowClass", "\344\270\212\344\270\213:", Q_NULLPTR));
        upOrDownCom->clear();
        upOrDownCom->insertItems(0, QStringList()
         << QApplication::translate("mainwindowClass", "\344\270\212", Q_NULLPTR)
         << QApplication::translate("mainwindowClass", "\344\270\213", Q_NULLPTR)
        );
        label_12->setText(QApplication::translate("mainwindowClass", "\350\277\233\351\200\200:", Q_NULLPTR));
        directionCom->clear();
        directionCom->insertItems(0, QStringList()
         << QApplication::translate("mainwindowClass", "\350\277\233", Q_NULLPTR)
         << QApplication::translate("mainwindowClass", "\351\200\200", Q_NULLPTR)
        );
        btnOpenVideo->setText(QApplication::translate("mainwindowClass", "\345\233\236\346\224\276", Q_NULLPTR));
        btn_test->setText(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\347\212\266\346\200\201", Q_NULLPTR));
        btnRecord->setText(QApplication::translate("mainwindowClass", "\345\275\225\345\203\217", Q_NULLPTR));
        btnOpenPath->setText(QApplication::translate("mainwindowClass", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "\347\233\221\346\216\247", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\270\200", Q_NULLPTR));
        label_ip1->setText(QApplication::translate("mainwindowClass", "IP:", Q_NULLPTR));
        label_2->setText(QApplication::translate("mainwindowClass", "\347\224\250\346\210\267\345\220\215:", Q_NULLPTR));
        label_3->setText(QApplication::translate("mainwindowClass", "\345\257\206\347\240\201:", Q_NULLPTR));
        lineEdit_password_1->setText(QString());
        groupBox_2->setTitle(QApplication::translate("mainwindowClass", "\347\233\270\346\234\272\344\272\214", Q_NULLPTR));
        label_ip2->setText(QApplication::translate("mainwindowClass", "IP:", Q_NULLPTR));
        label_5->setText(QApplication::translate("mainwindowClass", "\347\224\250\346\210\267\345\220\215:", Q_NULLPTR));
        label_4->setText(QApplication::translate("mainwindowClass", "\345\257\206\347\240\201:", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("mainwindowClass", "\351\207\214\347\250\213\346\234\215\345\212\241\345\231\250", Q_NULLPTR));
        label_8->setText(QApplication::translate("mainwindowClass", "IP", Q_NULLPTR));
        label_9->setText(QApplication::translate("mainwindowClass", "\347\253\257\345\217\243\345\217\267:", Q_NULLPTR));
        btn_connectMileage->setText(QApplication::translate("mainwindowClass", "\350\277\236\346\216\245\351\207\214\347\250\213\346\234\215\345\212\241\345\231\250", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("mainwindowClass", "\345\275\225\345\203\217\345\255\230\345\202\250\350\267\257\345\276\204", Q_NULLPTR));
        btnSelectPath->setText(QApplication::translate("mainwindowClass", "\344\277\256\346\224\271\350\267\257\345\276\204", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainwindowClass", "\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
