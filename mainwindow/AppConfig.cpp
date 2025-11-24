#include "AppConfig.h"
#include <qDebug>



QString AppConfig::ConfigFile = "Config.ini";
QString AppConfig::IP_1 = "192.168.1.64";
QString AppConfig::IP_2 = "192.168.1.65";
bool AppConfig::WriteFlag = false;
bool AppConfig::Style = false;


void AppConfig::readConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    AppConfig::WriteFlag = set.value("WriteFlag").toBool();
    AppConfig::Style = set.value("Style").toBool();
    AppConfig::IP_1 = set.value("IP_1").toString();
    AppConfig::IP_2 = set.value("IP_2").toString();
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