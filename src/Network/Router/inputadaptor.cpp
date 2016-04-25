#include "inputadaptor.h"
#include "router.h"
#include <iostream>
#include <ctime>
#include <QtEndian>
#include <QDebug>

using namespace std;

InputAdaptor::InputAdaptor(Router *r, int serviceType, QString src, QString rtSrc, int pSize)
{
    this->r = r;
    this->pSize = pSize;
    this->serviceType = serviceType;
    routingTable = new RoutingTable(rtSrc);

    inpQueue = new Queue(1000);

    if(src != "L")
        this->loadQueue(src);
}

void InputAdaptor::loadQueue(QString srcFile)
{
    QFile f(srcFile);
    num_input_packets = 0;

    if(!f.open(QFile::ReadOnly))
        return;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();

    // */D/1
    if(pSize > 0)
    {
        num_input_packets = (ba.size()/pSize);
        int j = 0;

        while(j < ba.size())
        {
            packet p;
            memcpy(&p.packetv4, buffer, pSize);
            inpQueue->push(p);

            buffer += pSize;
            j += pSize;
        }
    }
    else // */M/1
    {
        int j = 0, packetSize = 0;
        int bSize = ba.size();

        while(j < bSize)
        {
            unsigned short int pLength;

            memcpy(&pLength, buffer + PACKET_LENGTH_SEEK, PACKET_LENGTH_SEEK);
            packetSize = qToBigEndian(pLength);

            packet p;
            memcpy(&p.packetv4, buffer, packetSize);

            p.packetv4.total_length = packetSize;
            inpQueue->push(p);

            buffer = buffer + packetSize;
            j += packetSize;

            ++num_input_packets;
        }
    }
    //  delete buffer;
}

void InputAdaptor::insertInQueue(packet p)
{
    p.arrivalTimeAtRouter = std::time(0);
    this->inpQueue->push(p);
}

void InputAdaptor::run()
{
    if(this->r->isBorder)
        this->borderRun();
    else
        this->coreRun();
}

void InputAdaptor::borderRun()
{
    int port, qNum;
    unsigned char dscp;
    processedPackets = 0;
    bool ok;

    while(1)
    {
        if(inpQueue->empty())
            break;

        packet p = inpQueue->pop(ok);

        if(!ok)
            break;

        routingTable->lookUp(p.packetv4.destination_addr, port, qNum, dscp);
        p.packetv4.dscp = dscp;

        r->fabric(p,port,qNum);
        ++processedPackets;

    }

    r->inpNotify(num_input_packets);
    cout << "Input Adaptor finished." << endl;
}

void InputAdaptor::coreRun()
{
    int port, qNum;
    unsigned char dscp;
    processedPackets = 0;
    bool ok;

    while(1)
    {
        if(inpQueue->empty())
        {
            msleep(100);
            continue;
        }

        packet p = inpQueue->pop(ok);

        if(!ok)
            break;

        routingTable->lookUp(p.packetv4.destination_addr, port, qNum, dscp);

        if(r->serviceType == DIFFSERV)
            qNum = r->diffServCodes[port]->indexOf(p.packetv4.dscp);

        r->fabric(p, port, qNum);
        ++processedPackets;
    }

    r->inpNotify(num_input_packets);
    cout << "Input Adaptor finished." << endl;
}
