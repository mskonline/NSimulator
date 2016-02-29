#include "inputadaptor.h"
#include "router.h"
#include <QQueue>
#include <ctime>

#include <QDebug>

InputAdaptor::InputAdaptor(Router *r, int rate, QString rtSrc)
{
    this->r = r;
    this->inputRate = rate;
    routingTable = new RoutingTable(rtSrc);
}

InputAdaptor::InputAdaptor(Router *r, int rate, QString rtSrc, QString file)
{
    this->r = r;
    this->inputRate = rate;
    this->loadQueue(file);
    routingTable = new RoutingTable(rtSrc);
}

void InputAdaptor::loadQueue(QString srcFile)
{
    QFile f(srcFile);

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }


    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();
    num_input_packets = (ba.size()/PACKET_SIZE);
    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, PACKET_SIZE);
        p.arrivalTime = std::time(0);
        inpQueue.enqueue(p);

        buffer += PACKET_SIZE;
        j += PACKET_SIZE;
    }

  //  delete buffer;
}

void InputAdaptor::run()
{
    int port;
    processedPackets = 0;

    while(1)
    {
        if(inpQueue.isEmpty())
            break;

        for(int i=0; i < inputRate; ++i)
        {
            if(inpQueue.isEmpty())
                break;

            packet p = inpQueue.dequeue();

            port = routingTable->lookUp(p.packetv4.destination_addr);
            --port;
            r->fabric(p,port,0);
            ++processedPackets;
        }

        msleep(5);
    }

    r->notify(num_input_packets);
    qDebug() << "IAdaptor finished";
}
