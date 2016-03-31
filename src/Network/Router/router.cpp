#include "router.h"
#include "Commons/commons.h"
#include "inputadaptor.h"
#include <QSettings>
#include <QFile>
#include <QByteArray>
#include <QQueue>
#include <ctime>
#include <iostream>

#include <QDebug>

using namespace std;

Router::Router(QString name, int pSize)
{
    this->name = name;
    this->allSet = false;
    pProcessed = 0;
    totalInputPackets = 0;
    nInpCount = 0;
    nOutCount = 0;
    packetSize = pSize;

    allInpPacketsProcessed = allOutPacketsProcessed = false;
}

void Router::initiate()
{
    QString settingsFile = "./config/" + this->name + ".ini";
    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    int scalefactor;

    if(settings->allKeys().count() == 0)
        return;

    settings->beginGroup("input");
        this->inputFiles = settings->value("values").toString().split(":");
        this->numInputs = settings->value("values").toString().split(":").count();
    settings->endGroup();

    settings->beginGroup("output");
        this->outputFiles = settings->value("values").toString().split(":");
        this->numOutputs = settings->value("values").toString().split(":").count();
        numQueues = settings->value("numQueues").toInt();
        scalefactor = settings->value("scalefactor").toInt();

        QStringList arrivalRateList = settings->value("arrivalrate").toString().split(":");
        arrival_rates = new int[arrivalRateList.size()];
        for(int i = 0; i < arrivalRateList.size(); ++i){
            arrival_rates[i] = arrivalRateList.at(i).toInt() * scalefactor;
        }

        QStringList outRatesList = settings->value("outputrate").toString().split(":");
        output_rates = new int[outRatesList.size()];
        for(int i = 0; i < outRatesList.size(); ++i){
            output_rates[i] = outRatesList.at(i).toInt() * scalefactor;
        }
    settings->endGroup();

    delete settings;

    // Initiate the Input and Output adaptors
    inpAdaptors = new InputAdaptor*[numInputs];
    outAdaptors = new OutputAdaptor*[numOutputs];

    interface->log("Forwarding table source : " + routingTable);

    if(packetSize > 0){
        interface->log(QString("Packet size in the network : %1 bytes").arg(packetSize));
        interface->log("Router performance : M/D/1");
    } else {
        interface->log("Packet size in the network : Variable");
        interface->log("Router performance : M/M/1");
    }

    interface->log(QString("Scale factor : %1").arg(scalefactor));

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i] = new InputAdaptor(this, routingTable, this->inputFiles.at(i), packetSize);

    int t = 0;

    for(int i = 0; i < numOutputs; ++i) {

        t = i * numQueues;

        for(int j = 0; j < numQueues; ++j)
            interface->log(QString("Arrival rate for Output %1 Queue %2 : %3 packets/sec").arg(i + 1).arg(j + 1).arg(arrival_rates[j + t]));
    }

    t = 0;

    for(int i = 0; i < numOutputs; ++i){
        vector<int> arrivalRates;

        t = i * numQueues;

        for(int j = 0; j < numQueues; ++j)
            arrivalRates.push_back(arrival_rates[j + t]);

        outAdaptors[i] = new OutputAdaptor(this,i,this->outputFiles.at(i), arrivalRates,
                                           output_rates[i], numQueues, packetSize);

        interface->log(QString("Output rate for Output %1 : %2 bits/sec").arg(i + 1).arg(output_rates[i]));
    }


    if(packetSize > 0){
        for(int i = 0; i < numOutputs; ++i)
            interface->log(QString("Calculated service time (Ts) at Output %1 : %2 msecs").arg(i + 1).arg(outAdaptors[i]->serviceTime));
    }

    allSet = true;
    interface->log("Router " + name + " initiated");
}

void Router::run()
{
    totalInputPackets = pProcessed = 0;

    cout << "Starting Input Adaptors..." << endl;

    for(int i = 0; i < numInputs; ++i)
       inpAdaptors[i]->start();

    while(1)
    {
        if(!allInpPacketsProcessed)
        {
            sleep(1);
            continue;
        }
        break;
    }

    cout << "All packets processed on the Input side : " << totalInputPackets << endl;

    cout << "Starting Output Adaptors..." << endl;

    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->start();

    return;
    while(1){
        pProcessed = 0;

        for(int i = 0; i < numOutputs; ++i)
           pProcessed += outAdaptors[i]->processedPackets;

        if(allOutPacketsProcessed)
            break;

        sleep(1);
    }

    logText.append("Router " + name + " finished\n");
    logText.append(QString("Total packets processed : %1\n").arg(this->pProcessed));

    cout << "All packets processed" << endl;
    this->performAnalysis();
}

void Router::performAnalysis()
{
    for(int i = 0; i < numOutputs; ++i)
        outAdaptors[i]->terminate();

    float total_residence_time = 0.0,
        maxTime = 0.0;
    int totalN = 0,
        maxN = 0;

    std::vector<float> meanResidenceTimePerQueue,
                       meanNumResidentItemsPerQueue;

    std::vector<int> maxNPacketsPerQueue, maxResTimePerQueue;


    // Mean # of Resident Items per Output Q (r)
    for(int i = 0; i < numOutputs; ++i) {

         for(int j = 0; j < numQueues; ++j)
         {
            totalN = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->itemsInQ.size(); ++k)
               totalN += outAdaptors[i]->queues[j]->itemsInQ[k];

            if(totalN == 0)
                meanNumResidentItemsPerQueue.push_back(0.0);
            else
                meanNumResidentItemsPerQueue.push_back(totalN / outAdaptors[i]->queues[j]->itemsInQ.size());

         }
    }

    // Max # of Resident Items across all Qs (R)
    for(int i = 0; i < numOutputs; ++i) {

        for(int j = 0; j < numQueues; ++j)
        {
            maxN = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->itemsInQ.size(); ++k)
            {
                if(outAdaptors[i]->queues[j]->itemsInQ[k] >= maxN)
                    maxN = outAdaptors[i]->queues[j]->itemsInQ[k];
            }

            maxNPacketsPerQueue.push_back(maxN);
        }
    }

    // Mean Residence Time per Output Q
    for(int i = 0; i < numOutputs; ++i) {

        for(int j = 0; j < numQueues; ++j)
        {
            total_residence_time = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->residenceTime.size(); ++k)
                total_residence_time += outAdaptors[i]->queues[j]->residenceTime[k];

            if(total_residence_time == 0)
                meanResidenceTimePerQueue.push_back(0.0);
            else
                meanResidenceTimePerQueue.push_back(total_residence_time / outAdaptors[i]->queues[j]->residenceTime.size());
        }
    }

    // Max Residence Time from all output Qs
    for(int i = 0; i < numOutputs; ++i) {

        for(int j = 0; j < numQueues; ++j)
        {
            maxTime = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->residenceTime.size(); ++k){
                if(outAdaptors[i]->queues[j]->residenceTime[k] >= maxTime)
                    maxTime = outAdaptors[i]->queues[j]->residenceTime[k];
            }

            maxResTimePerQueue.push_back(maxTime);
        }
    }

    // Logging
    // # packets at Input
    for(int i = 0; i < numInputs; ++i)
        logText.append(QString("Total packets processed at input %1 : %2\n").arg(i+1).arg(inpAdaptors[i]->processedPackets));

    // # packets at Output
    for(int i = 0; i < numOutputs; ++i)
        logText.append(QString("Total packets processed at output %1 : %2\n").arg(i+1).arg(outAdaptors[i]->processedPackets));

    // # Packets per Queue
    for(int i = 0; i < numOutputs; ++i)
    {
        logText.append(QString("For output link : %1\n").arg(i + 1));

        for(int j = 0; j < numQueues; ++j)
            logText.append(QString(" Total packets processed at Queue %1 : %2\n").arg(j + 1).arg(outAdaptors[i]->pPerQueue[j]));
    }

    logText.append("Mean # of packets in residence (r)\n");

    int t;
    // r
    for(int i = 0; i < numOutputs; ++i)
    {
        logText.append(QString(" For output link : %1\n").arg(i + 1));

        t = i * 2;

        for(int j = 0; j < numQueues; ++j)
            logText.append(QString("  Queue %1 : %2\n").arg(j + 1).arg(meanNumResidentItemsPerQueue[t + j]));
    }

    logText.append("Max # of packets in residence (R)\n");

    // R
    for(int i = 0; i < numOutputs; ++i)
    {
        logText.append(QString(" For output link : %1\n").arg(i + 1));

        t = i * 2;

        for(int j = 0; j < numQueues; ++j)
            logText.append(QString("  Queue %1 : %2\n").arg(j + 1).arg(maxNPacketsPerQueue[t + j]));
    }

    logText.append("Mean Residence Time (Tr)\n");

    // Tr
    for(int i = 0; i < numOutputs; ++i)
    {
        logText.append(QString(" For output link : %1\n").arg(i + 1));

        t = i * 2;

        for(int j = 0; j < numQueues; ++j)
            logText.append(QString("  Queue %1 : %2 msecs\n").arg(j + 1).arg(meanResidenceTimePerQueue[t + j]));

    }

    logText.append("Max Residence Time (Max Tr)\n");

    // Max Tr
    for(int i = 0; i < numOutputs; ++i)
    {
        logText.append(QString(" For output link : %1\n").arg(i + 1));

        t = i * 2;

        for(int j = 0; j < numQueues; ++j)
            logText.append(QString("  Max Residence Time(Max Tr) at Queue %1 : %2 msecs\n").arg(j + 1).arg(maxResTimePerQueue[t + j]));

    }
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
    outAdaptors[pNo]->putPacket(p, qNo);
}

void Router::inpNotify(int nPackets)
{
    ++nInpCount;
    totalInputPackets += nPackets;

    if(nInpCount == 3)
        allInpPacketsProcessed = true;
}


void Router::outNotify()
{
    ++nOutCount;

    if(nOutCount == 3)
        allOutPacketsProcessed = true;
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
