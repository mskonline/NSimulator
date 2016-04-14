#include "network.h"
#include <QSettings>

#include <windows.h>
Network::Network()
{
    isRunning = false;
}

void Network::initiate()
{
    int pSize = 0;
    QSettings *settings = new QSettings(NETWORK_SETTINGS,QSettings::IniFormat);

    settings->beginGroup("Network");
        this->routersList = settings->value("routers").toString().split(",");
        this->routingSrc = settings->value("routingtable").toString();
        pSize = settings->value("packetsize").toInt();
    settings->endGroup();

    settings->beginGroup("Links");
        this->linksList = settings->value("links").toString().split(",");
    settings->endGroup();

    delete settings;

    this->numRouters = this->routersList.count();

    routers = new Router*[this->numRouters];

    for(int i = 0; i < this->numRouters; ++i){
        routers[i] = new Router(this->routersList.at(i), pSize);
        routers[i]->setInterfaceObj(this->nsInterface);
        routers[i]->setRoutingTable(this->routingSrc);
        routers[i]->initiate();
        connect(routers[i],SIGNAL(finished()),this,SLOT(rFinished()));
    }

    // Intiate the links


    // Connect the links

    nsInterface->log(QString("Routers initiated. Total : %1").arg(this->numRouters));
}

void Network::run()
{
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
}

void Network::rFinished()
{
    for(int i = 0; i < this->numRouters; ++i){
        nsInterface->log(routers[i]->logText);
    }
}

void Network::stop()
{
    for(int i = 0; i < numRouters; ++i)
        routers[i]->stop();
}

void Network::setInterfaceObj(Interface *nsInterface)
{
    this->nsInterface = nsInterface;
}

Network::~Network()
{
}
