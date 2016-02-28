#include "outputadaptor.h"
#include <QQueue>
#include <QFile>
#include <QDebug>

OutputAdaptor::OutputAdaptor()
{
    numPacketsProcessed = 0;
}

OutputAdaptor::OutputAdaptor(QString file,int rate)
{
    numPacketsProcessed = 0;
    outputRate = rate;
    outFile = new QFile(file);

    if(!outFile->open(QFile::WriteOnly | QFile::Truncate)){
    }
}

void OutputAdaptor::run()
{
    float slp = outputRate / 10.0;
    while(1)
    {
        if(outQueue.count() == 0)
        {
            sleep(slp);
            continue;
        }

        mutex.lock();
            packet p = outQueue.dequeue();
        mutex.unlock();
        outFile->write(reinterpret_cast<char*>(&p.packetv4), PACKET_SIZE);
        ++numPacketsProcessed;

        sleep(slp);
    }
}

void OutputAdaptor::terminate()
{
    outFile->flush();
    outFile->close();
    qDebug() << "OAdaptor finished";

    QThread::terminate();
}

void OutputAdaptor::putPacket(packet p)
{
    mutex.lock();
        outQueue.enqueue(p);
    mutex.unlock();
}

OutputAdaptor::~OutputAdaptor()
{

}

