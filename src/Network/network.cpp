#include "network.h"
#include <QString>
#include <QSettings>
#include <QStringList>
#include <ctime>
#include <windows.h>
#include <QDebug>
#include "Router/inputadaptor.h"

Network::Network()
{
    isRunning = false;
}

void Network::initiate()
{
    int pSize = 0;
    isborderRouterFinished = false;
    rCounter = totalPacketsinNetwork = packetsProcessed = 0;
    packetsProcessedToDest = 0;

    QSettings *settings = new QSettings(NETWORK_SETTINGS,QSettings::IniFormat);
    QString service;

    settings->beginGroup("Network");
        this->routersList = settings->value("routers").toStringList();
        this->routingSrc = settings->value("routingtable").toString();
        service = settings->value("service").toString();
        pSize = settings->value("packetsize").toInt();
    settings->endGroup();

    nsInterface->log("Service : " + service);

    this->numRouters = this->routersList.count();

    routers = new Router*[this->numRouters];

    for(int i = 0; i < this->numRouters; ++i){
        routers[i] = new Router(i, pSize);
        routers[i]->setInterfaceObj(this->nsInterface);
        connect(routers[i], SIGNAL(rfinished(int)), this, SLOT(rFinished(int)));
    }

    settings->beginGroup("Links");
        this->linksList = settings->value("routerlinks").toStringList();
        this->numLinks = this->linksList.count();

    links = new Link*[this->numLinks];

    // Initiate the links
    QStringList routerConns;
    QString dRouterStr, dPortStr;
    int dRouter, dPort;

    QString sRouterStr, sPortStr;
    int sRouter, sPort;

    for(int i = 0; i < this->numRouters; ++i)
        routers[i]->initiate(service);

    qDebug() << "Routers initiated.";

    for(int i = 0; i < this->numLinks; ++i)
    {
        routerConns = settings->value(this->linksList.at(i)).toStringList();
        sRouterStr = routerConns.at(0).split(":")[0];
        sPortStr = routerConns.at(0).split(":")[1];

        sRouter = sRouterStr.right(1).toInt();
        sPort = sPortStr.toInt();

        dRouterStr = routerConns.at(1).split(":")[0];
        dPortStr = routerConns.at(1).split(":")[1];

        dRouter = dRouterStr.right(1).toInt();
        dPort = dPortStr.toInt();

        links[i] = new Link(routers[dRouter], dPort);

        this->routers[sRouter]->setOutputLink(links[i], sPort);
    }

    settings->endGroup();

    delete settings;


    nsInterface->log(QString("Routers initiated. Total : %1").arg(this->numRouters));

    for(int i = 0; i < numRouters;++i)
        routers[i]->printOutputLinkInfo();
}

void Network::run()
{
    startTime = std::time(0);

    nsInterface->pb_run->hide();

    FreeConsole();

    // create a separate new console window
    AllocConsole();

    // attach the new console to this application's process
    AttachConsole(GetCurrentProcessId());

    // reopen the std I/O streams to redirect I/O to the new console
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);

    nsInterface->log("Running the network...");

    if(!isRunning)
    {
        isRunning = true;

        for(int i = 0; i < numRouters; ++i)
            routers[i]->start();

        isRunning = false;
    }

    statusTimer = new QTimer(this);
    connect(statusTimer,SIGNAL(timeout()),this,SLOT(checkStatus()));
    statusTimer->start(3000);
}

void Network::checkStatus()
{
    packetsProcessedToDest = 0;

    for(int i = 1; i < numRouters; ++i)
        packetsProcessedToDest += routers[i]->processedPacketsToDest;

    qDebug() << "Total processed " << packetsProcessedToDest;

    if(isborderRouterFinished)
    {
        if(packetsProcessedToDest == totalPacketsinNetwork)
        {
            statusTimer->stop();

            for(int i = 1; i < numRouters; ++i){
                routers[i]->stop();
            }
        }
    }
 }

void Network::rFinished(int id)
{
    if(id == 0)
    {
        isborderRouterFinished = true;
        totalPacketsinNetwork = routers[0]->processedPackets;
    }

    ++rCounter;

    if(rCounter == this->numRouters)
    {
        for(int i = 0; i < this->numRouters; ++i)
            qDebug() << "Packets at destination router " << (i + 1) << routers[i]->processedPacketsToDest;

        for(int i = 0; i < this->numLinks; ++i)
            qDebug() << "Packets transferred at link " << i << " " << this->links[i]->packetsTransferred;

        this->performAnalysis();
    }
}

void Network::performAnalysis()
{
    Router *r;
    QList<int> flows;
    int flowId;

    for(int i = 0; i < this->numRouters; ++i)
    {
        r = routers[i];

        nsInterface->mlog("********************************");
        nsInterface->mlog("Measurements for Router " + r->rname);

        nsInterface->mlog(QString("Total packets processed : %1").arg(r->processedPackets));

        // Logging
        // # packets at Input
        for(int i = 0; i < r->numInputs; ++i)
            nsInterface->mlog(QString("Total packets processed at input %1 : %2").arg(i+1).arg(r->inpAdaptors[i]->processedPackets));

        // # packets at Output
        for(int i = 0; i < r->numOutputs; ++i)
            nsInterface->mlog(QString("Total packets processed at output %1 : %2").arg(i+1).arg(r->outAdaptors[i]->processedPackets));

        // # Packets per Queue
        for(int i = 0; i < r->numOutputs; ++i)
        {
            nsInterface->mlog(QString("For output link : %1").arg(i + 1));

            for(int j = 0; j < r->numQueues[i]; ++j)
                nsInterface->mlog(QString(" Total packets processed at Queue %1 : %2").arg(j + 1).arg(r->outAdaptors[i]->pPerQueue[j]));
        }

        nsInterface->mlog("Mean # of packets in residence (r)");

        int t = 0;
        // r
        for(int i = 0; i < r->numOutputs; ++i)
        {
            nsInterface->mlog(QString(" For output link : %1").arg(i + 1));

            for(int j = 0; j < r->numQueues[i]; ++j)
            {
                nsInterface->mlog(QString("  Queue %1 : %2").arg(j + 1).arg(r->meanNumResidentItemsPerQueue[t]));
                ++t;
            }
        }

        nsInterface->mlog("Max # of packets in residence (R)");

        t = 0;

        // R
        for(int i = 0; i < r->numOutputs; ++i)
        {
            nsInterface->mlog(QString(" For output link : %1").arg(i + 1));

            for(int j = 0; j < r->numQueues[i]; ++j)
            {
                nsInterface->mlog(QString("  Queue %1 : %2").arg(j + 1).arg(r->maxNPacketsPerQueue[t]));
                ++t;
            }
        }

        t = 0;
        nsInterface->mlog("Mean Residence Time (Tr)");

        // Tr
        for(int i = 0; i < r->numOutputs; ++i)
        {
            nsInterface->mlog(QString(" For output link : %1").arg(i + 1));

            for(int j = 0; j < r->numQueues[i]; ++j)
            {
                nsInterface->mlog(QString("  Queue %1 : %2 secs").arg(j + 1).arg(r->meanResidenceTimePerQueue[t]));
                ++t;
            }
        }

        t = 0;
        nsInterface->mlog("Max Residence Time (Max Tr)");

        // Max Tr
        for(int i = 0; i < r->numOutputs; ++i)
        {
            nsInterface->mlog(QString(" For output link : %1").arg(i + 1));

            for(int j = 0; j < r->numQueues[i]; ++j)
            {
                nsInterface->mlog(QString("  Max Residence Time(Max Tr) at Queue %1 : %2 secs").arg(j + 1).arg(r->maxResTimePerQueue[t]));
                ++t;
            }
        }


        for(int j = 0; j < r->numOutputs; ++j)
        {
            if(r->outAdaptors[j]->flowMap.count() > 0)
            {

                flows = r->outAdaptors[j]->flowMap.keys();

                for(int k = 0; k < flows.count(); ++k)
                {
                    qDebug() << "Flow " << flows.at(k);
                    flowId = r->outAdaptors[j]->flowMap.value(flows.at(k));

                    qDebug() << "Total Number of packets : " << r->outAdaptors[j]->flowTotalPackets.value(flowId);
                    qDebug() << "Total time : " <<  r->outAdaptors[j]->flowTotalTime.value(flowId);
                    qDebug() << "Avg Time spent in flow : " << ((r->outAdaptors[j]->flowTotalTime.value(flowId) * 1.0)
                                / r->outAdaptors[j]->flowTotalPackets.value(flowId)) << " secs";
                }
            }
        }
    }

    float endTime = std::time(0) - startTime;

    //for(int i = 0; i < numRouters; ++i)
        //nsInterface->mlog(QString("Total execution time router r%1 : %2 secs").arg(i + 1).arg(routers[i]->endTime));

    nsInterface->mlog(QString("Simulation complete. Total execution time : %1 secs").arg(endTime));
}

void Network::stop()
{
    for(int i = 0; i < numRouters; ++i)
        routers[i]->stop();

    statusTimer->stop();

    for(int i = 0; i < this->numRouters; ++i)
    {
        for(int j = 0; j < routers[i]->numOutputs; ++j)
        {
            for(int k = 0; k < routers[i]->outAdaptors[j]->numQueues; ++k)
                qDebug() << "Router " << (i + 1) << " queue " << j << k <<  " size " << routers[i]->outAdaptors[j]->queues[k]->size();
        }
    }
}

void Network::setInterfaceObj(Interface *nsInterface)
{
    this->nsInterface = nsInterface;
}

Network::~Network()
{
    for(int i = 0; i < numRouters; ++i)
        delete routers[i];
}
