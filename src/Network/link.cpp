#include "link.h"
#include "Router/router.h"
#include <QDebug>

Link::Link(Router *r, int destinationPort)
{
    this->destinationRouter = r;
    this->destinationPort = destinationPort;
    this->packetsTransferred = 0;
}

void Link::transfer(packet p)
{
    // This method will be invoked by source
    // Will have pointer to the destination router
    // This pointer will be used to place the packet in the destination
    this->destinationRouter->insertPacket(p, destinationPort);
    ++packetsTransferred;
}

void Link::printInfo()
{
    qDebug() << "Destination router is : " << this->destinationRouter->name;
}
