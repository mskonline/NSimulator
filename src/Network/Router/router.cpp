#include "router.h"
#include "Commons/commons.h"
#include "inputadaptor.h"
#include <QSettings>
#include <QFile>
#include <QByteArray>
#include <QQueue>
#include <ctime>


#include <QDebug>

Router::Router(QString name)
{
    this->name = name;
    this->allSet = false;
    pProcessed = 0;
    totalInputPackets = 0;
    nCount = 0;
    allPacketsProcessed = false;
}

void Router::initiate()
{
    QString settingsFile = "./config/" + this->name + ".ini";
    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    if(settings->allKeys().count() == 0)
        return;

    settings->beginGroup("input");
        this->inputFiles = settings->value("values").toString().split(":");
        this->numInputs = settings->value("values").toString().split(":").count();
        QStringList ratesList = settings->value("rates").toString().split(":");

        input_rates = new int[ratesList.size()];
        for(int i = 0; i < ratesList.size(); ++i){
            input_rates[i] = ratesList.at(i).toInt();
        }

    settings->endGroup();

    settings->beginGroup("output");
        this->outputFiles = settings->value("values").toString().split(":");
        this->numOutputs = settings->value("values").toString().split(":").count();
        QStringList outRatesList = settings->value("rates").toString().split(":");
        output_rates = new int[outRatesList.size()];
        for(int i = 0; i < outRatesList.size(); ++i){
            output_rates[i] = outRatesList.at(i).toInt();
        }
    settings->endGroup();

    delete settings;

    allSet = true;
    interface->log("Router " + name + " initiated");

    // Initiate the Input and Output adaptors
    inpAdaptors = new InputAdaptor*[numInputs];
    outAdaptors = new OutputAdaptor*[numOutputs];

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i] = new InputAdaptor(this, input_rates[i], routingTable, this->inputFiles.at(i));

    for(int i = 0; i < numOutputs; ++i)
        outAdaptors[i] = new OutputAdaptor(this->outputFiles.at(i), output_rates[i]);
}

void Router::run()
{
    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->start();

    for(int i = 0; i < numInputs; ++i)
       inpAdaptors[i]->start();

    while(1)
    {
        if(!allPacketsProcessed)
        {
            sleep(1);
            continue;
        }
        break;
    }


    while(1){
        pProcessed = 0;
        for(int i = 0; i < numOutputs; ++i)
           pProcessed += outAdaptors[i]->numPacketsProcessed;

        if(pProcessed == totalInputPackets)
        {
            for(int i = 0; i < numOutputs; ++i)
                outAdaptors[i]->terminate();
            break;
        }
        sleep(.5);
    }

    interface->log("Router " + name + " finished");
    interface->log(QString("Total packets processed : %1").arg(this->pProcessed));
}

void Router::stop()
{
    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->terminate();

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i]->terminate();

}

void Router::fabric(packet p,int pNo,int qNo)
{
    mutex.lock();
        outAdaptors[pNo]->putPacket(p);
    mutex.unlock();
}

void Router::notify(int nPackets)
{
    ++nCount;
    totalInputPackets += nPackets;

    if(nCount == 3)
        allPacketsProcessed = true;
}

void Router::setInterfaceObj(Interface *interface)
{
    this->interface = interface;
}

void Router::setRoutingTable(QString rSrc){
    this->routingTable = rSrc;
}

Router::~Router()
{
}
