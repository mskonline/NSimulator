#include "inputadaptor.h"
#include "router.h"
#include <QQueue>
#include <ctime>

InputAdaptor::InputAdaptor(Router *r)
{
    this->r = r;
}

InputAdaptor::InputAdaptor(Router *r, QString file)
{
    this->r = r;
    this->loadQueue(file);
}


void InputAdaptor::loadQueue(QString srcFile)
{
    QFile f(srcFile);

    if(!f.open(QFile::ReadOnly))
    {
        return;
    }

    char *buffer;
    QByteArray ba;

    ba = f.readAll();
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

    delete buffer;
}

void InputAdaptor::run()
{
    int pk = 0, port;
    while(1)
    {
        for(int i=0; i < inputRate; ++i)
        {
            if(inpQueue.isEmpty())
                break;
            packet p = inpQueue.dequeue();
            r->fabric(p,port,0);
        }
        sleep(.2);
    }
}
