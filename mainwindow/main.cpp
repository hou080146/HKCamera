#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include "AppConfig.h"
#include "appinit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ÖØÐ´´°¿ÚÍÏ¶¯
    AppInit::Instance()->start();

    AppConfig::ConfigFile = QString("Config.ini");
    AppConfig::readConfig();

    mainwindow w;
    w.show();
    return a.exec();
}
