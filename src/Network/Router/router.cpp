#include "router.h"
#include <QSettings>
#include <QFile>
#include <QDebug>

Router::Router(QString name)
{
    this->name = name;
    this->setUp();
}

void Router::setUp()
{
    QString settingsFile = "./config/" + this->name + ".ini";

    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    settings->beginGroup("input");
    this->numInputs = settings->value("values").toString().split(":").count();
    settings->endGroup();

    delete settings;
}

void Router::run()
{

}
