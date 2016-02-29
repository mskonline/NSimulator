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
    nCount = totalInputPackets = pProcessed = 0;
    int total_residence_time = 0 ,maxTime = 0;
    float meanResidenceTime = 0;


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

    qDebug() << "All packets processed on the input side" << totalInputPackets;

    while(1){
        pProcessed = 0;
        for(int i = 0; i < numOutputs; ++i)
           pProcessed += outAdaptors[i]->processedPackets;

        if(pProcessed == totalInputPackets)
        {
            int t_residentItems = 0;
            // Mean Residence Time
            for(int i = 0; i < numOutputs; ++i) {
                 for(int j = 0; j < outAdaptors[i]->residenceTime.size(); ++j)
                    total_residence_time += outAdaptors[i]->residenceTime[j];

                 t_residentItems += outAdaptors[i]->residenceTime.size();
            }

            meanResidenceTime = (float) total_residence_time / t_residentItems;

            // Max Residence Time
            for(int i = 0; i < numOutputs; ++i) {
                 for(int j = 0; j < outAdaptors[i]->residenceTime.size(); ++j){
                    if(outAdaptors[i]->residenceTime[j] >= maxTime)
                        maxTime = outAdaptors[i]->residenceTime[j];
                 }
            }

            int totalN, maxN;

            for(int i = 0; i < numOutputs; ++i) {
                 totalN = maxN = 0;
                 for(int j = 0; j < outAdaptors[i]->itemsInQ.size(); ++j){
                    totalN += outAdaptors[i]->itemsInQ[j];

                    if(outAdaptors[i]->itemsInQ[j] >= maxN)
                        maxN = outAdaptors[i]->itemsInQ[j];
                 }

                 if(totalN == 0)
                    meanNumResidentItems.push_back(0);
                 else
                    meanNumResidentItems.push_back(totalN / outAdaptors[i]->itemsInQ.size());

                 maxNumResidentItems.push_back(maxN);
            }

            for(int i = 0; i < numOutputs; ++i)
                outAdaptors[i]->terminate();
            break;
        }
        sleep(.5);
    }

    interface->log("Router " + name + " finished");
    interface->log(QString("Total packets processed : %1").arg(this->pProcessed));

    // Logging
    for(int i = 0; i < numInputs; ++i)
        interface->log(QString("Total packets processed at input %1 : %2").arg(i).arg(inpAdaptors[i]->processedPackets));

    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Total packets processed at output %1 : %2").arg(i).arg(outAdaptors[i]->processedPackets));

    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Mean # of packets at output %1 : %2").arg(i).arg(meanNumResidentItems[i]));

    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Max # of packets at output %1 : %2").arg(i).arg(maxNumResidentItems[i]));

    interface->log(QString("Mean Residence Time : %1 msecs").arg(meanResidenceTime));
    interface->log(QString("Max Residence Time : %1 msecs").arg(maxTime));
    interface->log("Router " + name + " finished");

    interface->update();
}

void Router::stop()
{
    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->terminate();

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i]->terminate();

    this->terminate();
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
