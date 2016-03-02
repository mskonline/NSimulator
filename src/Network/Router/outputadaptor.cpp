#include "outputadaptor.h"
#include <QQueue>
#include <QFile>
#include <iostream>
#include <ctime>
#include <math.h>
#include <QDebug>

using namespace std;

OutputAdaptor::OutputAdaptor(int delay):oMutex(QMutex::Recursive)
{
    droppedPCount = 0;
}

OutputAdaptor::OutputAdaptor(QString file, int rate, int qSize, int delay, float loadFactor)
{
    droppedPCount = 0;
    outputRate = rate;
    this->delay = delay;
    outFile = new QFile(file);

    this->qSize = qSize;
    this->maxQSize = ceil(loadFactor * qSize);

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

        // Dummy Service
        msleep(this->delay);

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
    }
}

void OutputAdaptor::terminate()
{
    outFile->flush();
    outFile->close();  
    cout << "OAdaptor finished " << endl;

    QThread::terminate();
}

void OutputAdaptor::putPacket(packet p)
{
   QMutexLocker m(&oMutex);

   if(outQueue.count() >= this->maxQSize)
   {
      // Dropping the packets
      ++droppedPCount;
      return;
   }

   outQueue.enqueue(p);
}

OutputAdaptor::~OutputAdaptor()
{
}

