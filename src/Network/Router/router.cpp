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

    mutex = new QMutex(QMutex::Recursive);
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

    for(int i = 0; i < numInputs; ++i){
        inpAdaptors[i] = new InputAdaptor(this, input_rates[i], routingTable, this->inputFiles.at(i));
        interface->log(QString("Rate for Input %1 : %2").arg(i + 1).arg(input_rates[i]));
    }
    for(int i = 0; i < numOutputs; ++i){
        outAdaptors[i] = new OutputAdaptor(this->outputFiles.at(i), output_rates[i]);
        interface->log(QString("Rate for Output %1 : %2").arg(i + 1).arg(output_rates[i]));
    }
}

void Router::run()
{
    nCount = totalInputPackets = pProcessed = 0;
    int total_residence_time = 0 ,maxTime = 0;
    int totalN, maxN;
    std::vector<int> meanResidenceTimePLink;

    qDebug() << "Num Inputs" << numInputs;
    qDebug() << "Num Outputs" << numOutputs;

    for(int i = 0; i < numOutputs; ++i){
       outAdaptors[i]->start();
       sleep(1);
    }


    for(int i = 0; i < numInputs; ++i){
       inpAdaptors[i]->start();
       sleep(1);
    }

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
            // Mean # of Resident Items per Output Q (r)
            for(int i = 0; i < numOutputs; ++i) {
                 totalN = 0;
                 for(int j = 0; j < outAdaptors[i]->itemsInQ.size(); ++j)
                    totalN += outAdaptors[i]->itemsInQ[j];

                 if(totalN == 0)
                    meanNumResidentItems.push_back(0);
                 else
                    meanNumResidentItems.push_back(totalN / outAdaptors[i]->itemsInQ.size());

                 qDebug() << totalN << outAdaptors[i]->itemsInQ.size();
            }

            // Max # of Resident Items across all Qs (R)
            for(int i = 0; i < numOutputs; ++i) {
                 for(int j = 0; j < outAdaptors[i]->itemsInQ.size(); ++j){
                     if(outAdaptors[i]->itemsInQ[j] >= maxN)
                         maxN = outAdaptors[i]->itemsInQ[j];
                 }
            }

            // Mean Residence Time per Output Q
            for(int i = 0; i < numOutputs; ++i) {
                 total_residence_time = 0;

                 for(int j = 0; j < outAdaptors[i]->residenceTime.size(); ++j)
                    total_residence_time += outAdaptors[i]->residenceTime[j];

                 meanResidenceTimePLink.push_back(total_residence_time/outAdaptors[i]->residenceTime.size());

                 qDebug() << total_residence_time << outAdaptors[i]->residenceTime.size();
            }

            // Max Residence Time from all output Qs
            for(int i = 0; i < numOutputs; ++i) {
                 for(int j = 0; j < outAdaptors[i]->residenceTime.size(); ++j){
                    if(outAdaptors[i]->residenceTime[j] >= maxTime)
                        maxTime = outAdaptors[i]->residenceTime[j];
                 }
            }

            for(int i = 0; i < numOutputs; ++i)
                outAdaptors[i]->terminate();
            break;
        }
        sleep(.5);
    }

    interface->update();

    interface->log("Router " + name + " finished");
    interface->log(QString("Total packets processed : %1").arg(this->pProcessed));

    // Logging
    // # packets at Input
    for(int i = 0; i < numInputs; ++i)
        interface->log(QString("Total packets processed at input %1 : %2").arg(i+1).arg(inpAdaptors[i]->processedPackets));

    // # packets at Output
    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Total packets processed at output %1 : %2").arg(i+1).arg(outAdaptors[i]->processedPackets));

    // r
    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Mean # of packets in residence at output(r) %1 : %2").arg(i+1).arg(meanNumResidentItems[i]));

    // R
    interface->log(QString("Max # of packets in residence(R): %2").arg(maxN));

    // Tr
    for(int i = 0; i < numOutputs; ++i)
        interface->log(QString("Mean Residence Time at output(Tr) %1 : %2 msecs").arg(i + 1).arg(meanResidenceTimePLink[i]));

    // Max Tr
    interface->log(QString("Max Residence Time(Max Tr) : %1 msecs").arg(maxTime));

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
    mutex->lock();
        outAdaptors[pNo]->putPacket(p);
    mutex->unlock();
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
