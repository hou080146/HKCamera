#pragma once
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QTime>

#define TIMES qPrintable(QTime::currentTime().toString("HH:mm:ss zz"))

class AppConfig
{
public:
    static QString ConfigFile;
    static bool Style;
    static bool WriteFlag;
    //相机配置
    static QString IP_1;
    static QString IP_2;
    static QString user1;
    static QString user2;
    static QString password1;
    static QString password2;
    static QString SavePath;
    //相机１ 的ROI
    static bool ValidROI;
    static double ROI_x;
    static double ROI_y;
    static double ROI_width;
    static double ROI_height;
    //相机2 的ROI
    static bool ValidROI_2;
    static double ROI_x_2;
    static double ROI_y_2;
    static double ROI_width_2;
    static double ROI_height_2;

    static int reportInterval;
    static QString startMileage;
    static int upOrDown;
    static QString MileageServerIP;
    static int MileageServerPort;

    static QString SerialPortName;
    static int SerialBaudRate;
    static bool AutoConnectMileageServer;
    static bool AutoConnectSerialPort ;

    static void readConfig();
    static void writeConfig();


    static bool checkIniFile(const QString &iniFile);
};

