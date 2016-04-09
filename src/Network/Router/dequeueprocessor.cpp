#include "dequeueprocessor.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <QDebug>

using namespace std;

DeQueueProcessor::DeQueueProcessor()
{
}

DeQueueProcessor::DeQueueProcessor(QString file, Queue **q, int serviceTime, int outrate,
                                   int pSize,
                                   bool vPacketSize,
                                   int *qWeights)
{
    this->queue = q;
    outFile = new QFile(file);
    this->serviceTime = serviceTime;
    this->pSize = pSize;
    this->vPacketSize = vPacketSize;
    this->outrate = outrate;
    this->qWeights = qWeights;

    if(!outFile->open(QFile::WriteOnly | QFile::Truncate)){
    }
}

void DeQueueProcessor::run()
{
    totalPacketsProccessed = 0;
    int calcTime, qNum, temp = 0, i;
    bool ok;

    while(1)
    {
        qNum = temp % 3;

        queue[qNum]->logQDepth();

        if(queue[qNum]->empty())
        {
            msleep(this->serviceTime);
            ++temp;
            continue;
        }

        for (i = 0; i < qWeights[qNum]; i++)
        {
            packet p = queue[qNum]->pop(ok);

            if(!ok)
            {
                msleep(this->serviceTime);
                ++temp;
                continue;
            }

            // Residence Time
            calcTime = std::time(0) - p.arrivalTime;

            if(this->vPacketSize)
            {
                this->serviceTime = ceil(((p.packetv4.total_length  * 8.0) / outrate) * 1000) ; // in milli seconds
                this->pSize = p.packetv4.total_length;
            }

            queue[qNum]->residenceTime.push_back(calcTime + this->serviceTime);
            outFile->write(reinterpret_cast<char*>(&p.packetv4), this->pSize);
            ++totalPacketsProccessed;

            msleep(this->serviceTime);
            ++temp;
        }
    }
}
void DeQueueProcessor::terminate()
{
    outFile->flush();
    outFile->close();
}
