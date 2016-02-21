#include "network.h"
#include <QSettings>

Network::Network()
{
    isRunning = false;
}

void Network::initiate()
{
    QSettings *settings = new QSettings(NETWORK_SETTINGS,QSettings::IniFormat);

    settings->beginGroup("Network");
        this->routersList = settings->value("routers").toString().split(":");
        this->routingSrc = settings->value("routingtable").toString();
    settings->endGroup();

    delete settings;

    this->initiateRoutingTable();
    this->numRouters = this->routersList.count();

    routers = new Router*[this->numRouters];

    for(int i = 0; i < this->numRouters; ++i){
        routers[i] = new Router(this->routersList.at(i));
        routers[i]->setInterfaceObj(this->interface);
        routers[i]->initiate();
    }

    interface->log(QString("Routers initiated. Total : %1").arg(this->numRouters));
}

void Network::initiateRoutingTable()
{
    rTable = new RoutingTable(this->routingSrc);
    interface->log("Routing table Initiated...");

    interface->log(QString("Total Routing entries : %1")
                   .arg(rTable->getRoutingEntriesCount()));
}

void Network::run()
{
    interface->log("Running the network...");

    if(!isRunning)
    {
        isRunning = true;

        for(int i = 0; i < numRouters; ++i)
            routers[i]->run();

        isRunning = false;
    }
}

void Network::setRoutingSrc(QString routingSrc)
{
    this->routingSrc = routingSrc;
}

void Network::setInterfaceObj(Interface *interface)
{
    this->interface = interface;
}

Network::~Network()
{
    delete rTable;
}
