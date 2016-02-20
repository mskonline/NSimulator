#include "network.h"
#include "Router/router.h"


Network::Network()
{
}

void Network::setUp()
{
    Router *r1 = new Router("r1");
}

void Network::initiateRoutingTable()
{
    rTable = new RoutingTable(this->routingSrc);
    interface->log("Routing Table Initiated...");

    interface->log(QString("Total Routing entries : %1")
                   .arg(rTable->getRoutingEntriesCount()));
}

void Network::setRoutingSrc(std::string routingSrc)
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
