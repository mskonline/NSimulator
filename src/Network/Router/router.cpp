#include "router.h"
#include <QSettings>
#include <QFile>
#include <QDebug>

Router::Router(QString name)
{
    this->name = name;
    this->allSet = false;
}

void Router::initiate()
{
    QString settingsFile = "./config/" + this->name + ".ini";
    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    if(settings->allKeys().count() == 0)
        return;

    settings->beginGroup("input");
        this->numInputs = settings->value("values").toString().split(":").count();
    settings->endGroup();

    delete settings;

    allSet = true;
    interface->log("Router " + name + " initiated");
}

void Router::setInterfaceObj(Interface *interface)
{
    this->interface = interface;
}

void Router::run()
{
    if(allSet){
        interface->log("Router " + name + " running");
    }
}

Router::~Router()
{

}
