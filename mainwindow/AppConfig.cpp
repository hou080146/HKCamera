#include "AppConfig.h"
#include <qDebug>



QString AppConfig::ConfigFile = "Config.ini";
QString AppConfig::IP_1 = "192.168.1.64";
QString AppConfig::IP_2 = "192.168.1.65";
QString AppConfig::user1 = "admin";
QString AppConfig::user2 = "admin";
QString AppConfig::password1 = "ck1234";
QString AppConfig::password2 = "ck1234";
QString AppConfig::SavePath = "D:Vedio";
bool AppConfig::WriteFlag = false;
bool AppConfig::Style = false;
bool AppConfig::ValidROI = false;
double AppConfig::ROI_x = 0.0;
double AppConfig::ROI_y = 0.0;
double AppConfig::ROI_width = 0.0;
double AppConfig::ROI_height = 0.0;

bool AppConfig::ValidROI_2 = false;
double AppConfig::ROI_x_2 = 0.0;
double AppConfig::ROI_y_2 = 0.0;
double AppConfig::ROI_width_2 = 0.0;
double AppConfig::ROI_height_2 = 0.0;

int AppConfig::reportInterval = 1000;

QString AppConfig::startMileage;
int AppConfig::upOrDown;
QString AppConfig::MileageServerIP;
int AppConfig::MileageServerPort;

QString AppConfig::SerialPortName = "COM1";
int AppConfig::SerialBaudRate = 115200;
bool AppConfig::AutoConnectMileageServer = true;
bool AppConfig::AutoConnectSerialPort = true;


void AppConfig::readConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    AppConfig::WriteFlag = set.value("WriteFlag").toBool();
    AppConfig::Style = set.value("Style").toBool();
    AppConfig::IP_1 = set.value("IP_1").toString();
    AppConfig::IP_2 = set.value("IP_2").toString();
    AppConfig::user1 = set.value("user1").toString();
    AppConfig::user2 = set.value("user2").toString();
    AppConfig::password1 = set.value("password1").toString();
    AppConfig::password2 = set.value("password2").toString();
    AppConfig::SavePath = set.value("SavePath").toString();

    AppConfig::ValidROI = set.value("ValidROI").toBool();
    AppConfig::ROI_x = set.value("ROI_x").toDouble();
    AppConfig::ROI_y = set.value("ROI_y").toDouble();
    AppConfig::ROI_width = set.value("ROI_width").toDouble();
    AppConfig::ROI_height = set.value("ROI_height").toDouble();

    AppConfig::ValidROI_2 = set.value("ValidROI_2").toBool();
    AppConfig::ROI_x_2 = set.value("ROI_x_2").toDouble();
    AppConfig::ROI_y_2 = set.value("ROI_y_2").toDouble();
    AppConfig::ROI_width_2 = set.value("ROI_width_2").toDouble();
    AppConfig::ROI_height_2 = set.value("ROI_height_2").toDouble();

    //日志采集间隔
    AppConfig::reportInterval = set.value("reportInterval").toInt();

    AppConfig::startMileage = set.value("startMileage").toString();;
    AppConfig::upOrDown = set.value("upOrDown").toInt();;
    AppConfig::MileageServerIP = set.value("MileageServerIP").toString();;
    AppConfig::MileageServerPort = set.value("MileageServerPort").toInt();;
    set.endGroup();

    if (!AppConfig::checkIniFile(AppConfig::ConfigFile))
    {
        writeConfig();
        return;
    }
}

void AppConfig::writeConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    //qDebug() << " in writeConfig";
    set.beginGroup("AppConfig");
    set.setValue("WriteFlag", AppConfig::WriteFlag);
    set.setValue("Style", AppConfig::Style);
    set.setValue("IP_1", AppConfig::IP_1);
    set.setValue("IP_2", AppConfig::IP_2);
    set.setValue("user1", AppConfig::user1);
    set.setValue("user2", AppConfig::user2);
    set.setValue("password1", AppConfig::password1);
    set.setValue("password2", AppConfig::password2);
    set.setValue("SavePath", AppConfig::SavePath);

    set.setValue("ValidROI", AppConfig::ValidROI);
    set.setValue("ROI_x", AppConfig::ROI_x);
    set.setValue("ROI_y", AppConfig::ROI_y);
    set.setValue("ROI_width", AppConfig::ROI_width);
    set.setValue("ROI_height", AppConfig::ROI_height);

    set.setValue("ValidROI_2", AppConfig::ValidROI_2);
    set.setValue("ROI_x_2", AppConfig::ROI_x_2);
    set.setValue("ROI_y_2", AppConfig::ROI_y_2);
    set.setValue("ROI_width_2", AppConfig::ROI_width_2);
    set.setValue("ROI_height_2", AppConfig::ROI_height_2);

    set.setValue("startMileage", AppConfig::startMileage);
    set.setValue("upOrDown", AppConfig::upOrDown);
    set.setValue("MileageServerIP", AppConfig::MileageServerIP);
    set.setValue("MileageServerPort", AppConfig::MileageServerPort);
    set.endGroup();
}



bool AppConfig::checkIniFile(const QString &iniFile)
{
    QFile file(iniFile);
    if (file.size() == 0)
    {
        return false;
    }

    if (file.open(QFile::ReadOnly))
    {
        bool ok = true;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            line.replace("\r", "");
            line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2)
            {
                QString key = list.at(0);
                QString value = list.at(1);
                if (value.isEmpty())
                {
                    qDebug() << TIMES << "ini node no value" << key;
                    ok = false;
                    break;
                }
            }
        }
        if (!ok)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}