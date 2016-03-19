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

OutputAdaptor::OutputAdaptor(QString file, int rate, int qSize, int delay, float loadFactor):oQueue(1000)
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

        itemsInQ.push_back(oQueue.qSize);

        int i;
        for(i=0; i < 21; ++i)
        {
            if(oQueue.empty())
                break;

            packet p;
            if(!oQueue.pop(p))
                break;

            // Residence Time
            calcTime = std::time(0) - p.arrivalTime;
            residenceTime.push_back(calcTime);

            outFile->write(reinterpret_cast<char*>(&p.packetv4), PACKET_SIZE);
            ++processedPackets;
        }

        msleep(this->delay);
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
    /*if(outQueue.count() >= this->maxQSize)
   {
      // Dropping the packets
      ++droppedPCount;
      return;
   }*/

    oQueue.push(p);
}

OutputAdaptor::~OutputAdaptor()
{
}
