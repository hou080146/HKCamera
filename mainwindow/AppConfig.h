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
    static QString IP_1;
    static QString IP_2;

    static void readConfig();
    static void writeConfig();

    static bool checkIniFile(const QString &iniFile);
};

