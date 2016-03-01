#include "outputadaptor.h"
#include <QQueue>
#include <QFile>
#include <QDebug>
#include <ctime>

OutputAdaptor::OutputAdaptor():oMutex(QMutex::Recursive)
{
}

OutputAdaptor::OutputAdaptor(QString file,int rate)
{
    outputRate = rate;
    outFile = new QFile(file);

    if(!outFile->open(QFile::WriteOnly | QFile::Truncate)){
    }
}

void OutputAdaptor::run()
{
    processedPackets = 0;
    int calcTime;

    while(1)
    {
        if(outQueue.count() == 0)
        {
            sleep(.5);
            continue;
        }

        itemsInQ.push_back(outQueue.size());

        for(int i=0; i < outputRate; ++i)
        {
            if(outQueue.isEmpty())
                break;

            QMutexLocker m(&oMutex);
                packet p = outQueue.dequeue();
            m.unlock();

            // Residence Time
            calcTime = std::time(0) - p.arrivalTime;
            residenceTime.push_back(calcTime);

            outFile->write(reinterpret_cast<char*>(&p.packetv4), PACKET_SIZE);
            ++processedPackets;
        }

        msleep(50);
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
   QMutexLocker m(&oMutex);
   outQueue.enqueue(p);
}

OutputAdaptor::~OutputAdaptor()
{
}

