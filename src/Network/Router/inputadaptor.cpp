#include "inputadaptor.h"
#include "router.h"
#include <QQueue>
#include <iostream>
#include <QtEndian>
#include <QDebug>

using namespace std;

InputAdaptor::InputAdaptor(Router *r, int rate, int delay, QString rtSrc)
{
    this->r = r;
    routingTable = new RoutingTable(rtSrc);
}

InputAdaptor::InputAdaptor(Router *r, QString rtSrc, QString file, int pSize)
{
    this->r = r;
    this->pSize = pSize;
    this->loadQueue(file);
    routingTable = new RoutingTable(rtSrc);
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

    // M/D/1
    if(pSize > 0)
    {
        num_input_packets = (ba.size()/pSize);
        int j = 0;

        while(j < ba.size())
        {
            packet p;
            memcpy(&p.packetv4, buffer, pSize);
            inpQueue.enqueue(p);

            buffer += pSize;
            j += pSize;
        }
    }
    else // M/M/1
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
            inpQueue.enqueue(p);

            buffer = buffer + packetSize;
            j += packetSize;

            ++num_input_packets;
        }
    }
    //  delete buffer;
}

void InputAdaptor::run()
{
    int port, qNum;
    processedPackets = 0;

    while(1)
    {
        if(inpQueue.isEmpty())
            break;

        packet p = inpQueue.dequeue();
        routingTable->lookUp(p.packetv4.destination_addr, port, qNum);

        r->fabric(p,port,qNum);
        ++processedPackets;

    }

    r->inpNotify(num_input_packets);
    cout << "Input Adaptor finished." << endl;
}
