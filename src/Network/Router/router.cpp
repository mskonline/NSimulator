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

Router::Router(int id, int pSize)
{
    this->id = id;
    this->rid = QString("r%1").arg(id);
    this->rname = QString("r%1").arg(id + 1);
    this->allSet = false;
    processedPackets = 0;
    processedPacketsToDest = 0;
    totalInputPackets = 0;
    nInpCount = 0;
    nOutCount = 0;
    packetSize = pSize;
    isBorder = false;
    serviceType = GENERAL_SERVICE;

    allInpPacketsProcessed = allOutPacketsProcessed = false;
}

void Router::initiate(QString service)
{
    QString settingsFile = "./config/" + service + "/" + this->rid + ".ini";
    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    int scalefactor;

    if(settings->allKeys().count() == 0)
        return;

    QString str;
    QString dsCodes;

    settings->beginGroup("config");
        this->numInputs = settings->value("numinputs").toString().toInt();
        this->numOutputs = settings->value("numoutputs").toString().toInt();
        this->routingTable = settings->value("routingTable").toString();

        str = settings->value("border").toString();
        isBorder = (str == "y") ? true : false;
    settings->endGroup();

    settings->beginGroup("input");
        this->inputs = settings->value("values").toString().split(":");
    settings->endGroup();

    settings->beginGroup("output");
        this->outputs = settings->value("values").toString().split(":");

        QStringList numQueueList = settings->value("queues").toString().split(":");
        this->numQueues = new int[numQueueList.size()];
        for(int i = 0; i < numQueueList.size(); ++i){
            this->numQueues[i] = numQueueList.at(i).toInt();
        }

        scalefactor = settings->value("scalefactor").toInt();

        if(isBorder)
        {
            QStringList arrivalRateList = settings->value("arrivalrate").toString().split(":");
            arrival_rates = new int[arrivalRateList.size()];
            for(int i = 0; i < arrivalRateList.size(); ++i)
                arrival_rates[i] = arrivalRateList.at(i).toInt() * scalefactor;

            QString arrivalType = settings->value("packetarrival").toString();
            arrivalRateType = arrivalType == "D" ? DETERMINISTIC_ARRIVAL : POISSON_ARRIVAL;
        }

        QStringList outRatesList = settings->value("outputrate").toString().split(":");
        output_rates = new int[outRatesList.size()];
        for(int i = 0; i < outRatesList.size(); ++i)
            output_rates[i] = outRatesList.at(i).toInt() * scalefactor;

        QStringList WeightList = settings->value("weights").toString().split(":");
        qWeights = new int[WeightList.size()];
        for(int i = 0; i < WeightList.size(); ++i)
            qWeights[i] = WeightList.at(i).toInt();

        if(service == "DIFFSERV")
        {
            serviceType = DIFFSERV;
            QStringList dlist = settings->value("diffservices").toStringList();

            int q = 0;
            int d = 0;
            diffServCodes = new QList<int>*[numOutputs];

            for(int i = 0; i < numOutputs; ++i)
            {
                q = numQueues[i];
                diffServCodes[i] = new QList<int>;

                for(int k = 0; k < q; ++k) {
                    diffServCodes[i]->append(dlist.at(d).toInt());
                    dsCodes.append(dlist.at(d) + " ");
                    ++d;
                }
            }
        }

        settings->endGroup();

    delete settings;

    // Initiate the Input and Output adaptors
    inpAdaptors = new InputAdaptor*[numInputs];
    outAdaptors = new OutputAdaptor*[numOutputs];

    if(isBorder)
    {
        if(packetSize > 0){
            if(arrivalRateType == DETERMINISTIC_ARRIVAL)
                interface->log("Network performance : D/D/1");
            else
                interface->log("Network performance : M/D/1");
        } else {
            if(arrivalRateType == DETERMINISTIC_ARRIVAL)
                interface->log("Network performance : D/M/1");
            else
                interface->log("Network performance : M/M/1");
        }
    }

    interface->log("*************************************");
    interface->log("Router " + rname + " configurations");
    interface->log("Forwarding table source : " + routingTable);

    interface->log(QString("Scale factor : %1").arg(scalefactor));
    interface->log("Diff Services offered : " + dsCodes);

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i] = new InputAdaptor(this, serviceType, this->inputs.at(i), routingTable, packetSize);


    int t = 0;

    if(isBorder)
    {
        for(int i = 0; i < numOutputs; ++i) {

            t = i * numQueues[i];

            for(int j = 0; j < numQueues[i]; ++j)
                interface->log(QString("Arrival rate for Output %1 Queue %2 : %3 packets/sec").arg(i + 1).arg(j + 1).arg(arrival_rates[j + t]));
        }
    }

    t = 0;
    int w = 0;

    for(int i = 0; i < numOutputs; ++i){
        vector<int> arrivalRates, queueWeights;

        if(isBorder)
        {
            t = i * numQueues[i];

            for(int j = 0; j < numQueues[i]; ++j)
                arrivalRates.push_back(arrival_rates[j + t]);
        }

        for(int j = 0; j < numQueues[i]; ++j)
        {
            queueWeights.push_back(qWeights[w]);
            ++w;
        }

        outAdaptors[i] = new OutputAdaptor(this, i, this->outputs.at(i), arrivalRates,
                                           output_rates[i], numQueues[i], packetSize, queueWeights);

        interface->log(QString("Output rate for Output %1 : %2 bits/sec").arg(i + 1).arg(output_rates[i]));

    }


    if(packetSize > 0){
        for(int i = 0; i < numOutputs; ++i)
            interface->log(QString("Calculated service time (Ts) at Output %1 : %2 msecs").arg(i + 1).arg(outAdaptors[i]->serviceTime));
    }

    t = 0;
    w = 0;

    for(int i = 0; i < numOutputs; ++i){

        for(int j = 0; j < numQueues[i]; ++j)
        {
            interface->log(QString("Weights used for weighted round robin for Output %1 Queue %2 : %3").arg(i + 1).arg(j + 1).arg(qWeights[w]));
            ++w;
        }
    }

    allSet = true;
    interface->log("Router " + rname + " initiated");
}

void Router::run()
{
    startTime = std::time(0);;

    if(isBorder)
        this->borderRun();
    else
        this->coreRun();

    emit rfinished(id);
}

void Router::borderRun()
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

    while(1){
        processedPackets = 0;

        for(int i = 0; i < numOutputs; ++i)
           processedPackets += outAdaptors[i]->processedPackets;

        if(allOutPacketsProcessed)
            break;

        sleep(1);
    }

    endTime = ((std::time(0) - startTime) * 1.0) / 1000;

    logText.append("Router " + rname + " finished\n");
    logText.append(QString("Total packets processed : %1\n").arg(this->processedPackets));

    cout << "All packets processed" << endl;

    this->performAnalysis();
}

void Router::coreRun()
{
    for(int i = 0; i < numInputs; ++i)
       inpAdaptors[i]->start();

    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->start();

    while(1){
        processedPacketsToDest = 0;

        for(int i = 0; i < numOutputs; ++i)
           processedPacketsToDest += outAdaptors[i]->processedPacketsToDest;

        sleep(1);
    }
}

void Router::performAnalysis()
{
    float total_residence_time = 0.0,
        maxTime = 0.0;
    int totalN = 0,
        maxN = 0;

    // Mean # of Resident Items per Output Q (r)
    for(int i = 0; i < numOutputs; ++i) {

         for(int j = 0; j < numQueues[i]; ++j)
         {
            totalN = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->itemsInQ.size(); ++k)
               totalN += outAdaptors[i]->queues[j]->itemsInQ[k];

            if(totalN == 0)
                meanNumResidentItemsPerQueue.push_back(0.0);
            else
                meanNumResidentItemsPerQueue.push_back((totalN * 1.0) / outAdaptors[i]->queues[j]->itemsInQ.size());
         }
    }

    // Max # of Resident Items across all Qs (R)
    for(int i = 0; i < numOutputs; ++i) {

        for(int j = 0; j < numQueues[i]; ++j)
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

        for(int j = 0; j < numQueues[i]; ++j)
        {
            total_residence_time = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->residenceTime.size(); ++k)
                total_residence_time += outAdaptors[i]->queues[j]->residenceTime[k];

            if(total_residence_time == 0)
                meanResidenceTimePerQueue.push_back(0.0);
            else
                meanResidenceTimePerQueue.push_back((total_residence_time * 1.0) / outAdaptors[i]->queues[j]->residenceTime.size());
        }
    }

    // Max Residence Time from all output Qs
    for(int i = 0; i < numOutputs; ++i) {

        for(int j = 0; j < numQueues[i]; ++j)
        {
            maxTime = 0;

            for(int k = 0; k < outAdaptors[i]->queues[j]->residenceTime.size(); ++k){
                if(outAdaptors[i]->queues[j]->residenceTime[k] >= maxTime)
                    maxTime = outAdaptors[i]->queues[j]->residenceTime[k];
            }

            maxResTimePerQueue.push_back(maxTime);
        }
    }

    processedPackets = 0;

    for(int i = 0; i < numOutputs; ++i)
        processedPackets += outAdaptors[i]->processedPackets;
}

void Router::stop()
{
    endTime = ((std::time(0) - startTime) * 1.0) / 1000;

    qDebug() << "Stoping router..." << this->rname;

    for(int i = 0; i < numOutputs; ++i)
       outAdaptors[i]->terminate();

    for(int i = 0; i < numInputs; ++i)
        inpAdaptors[i]->terminate();

    this->performAnalysis();
    this->terminate();


    emit rfinished(id);
}

void Router::fabric(packet p,int pNo, int qNo)
{
    outAdaptors[pNo]->putPacket(p, qNo);
}

void Router::inpNotify(int nPackets)
{
    ++nInpCount;
    totalInputPackets += nPackets;

    if(nInpCount == numInputs)
        allInpPacketsProcessed = true;
}


void Router::outNotify()
{
    ++nOutCount;

    if(nOutCount == numOutputs)
        allOutPacketsProcessed = true;
}

void Router::insertPacket(packet p, int inputPort)
{
    this->inpAdaptors[inputPort]->insertInQueue(p);
}

void Router::setOutputLink(Link *l, int outputPort)
{
    this->outAdaptors[outputPort]->setLink(l);
}

void Router::setInterfaceObj(Interface *interface)
{
    this->interface = interface;
}

void Router::setRoutingTable(QString rSrc){
    this->routingTable = rSrc;
}

void Router::printOutputLinkInfo()
{
    qDebug() << "Router - " << rname;

    for(int i = 0; i < numOutputs; ++i)
        outAdaptors[i]->printLinkInfo();
}

Router::~Router()
{
}
