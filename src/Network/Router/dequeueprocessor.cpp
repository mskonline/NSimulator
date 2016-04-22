#include "dequeueprocessor.h"
#include "Network/link.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <QDebug>

using namespace std;

DeQueueProcessor::DeQueueProcessor()
{
}

DeQueueProcessor::DeQueueProcessor(Queue **q, int serviceTime, int outrate,
                                   int pSize,
                                   bool vPacketSize,
                                   std::vector<int> qWeights)
{
    this->queue = q;
    this->serviceTime = serviceTime;
    this->pSize = pSize;
    this->vPacketSize = vPacketSize;
    this->outrate = outrate;
    this->qWeights = qWeights;
    this->doTerminate = false;
}

void DeQueueProcessor::run()
{
    totalPacketsProccessed = 0;
    int calcTime, qNum, temp = -1, i;
    int totalPacketSize = 0;
    bool ok;

    QVector<int> pResidenceTime;

    while(1)
    {
        if(doTerminate) break;

        totalPacketSize = 0;

        ++temp;
        qNum = temp % 3;

        if(queue[qNum]->empty())
        {
            msleep(10);
            continue;
        }

        queue[qNum]->logQDepth();

        for (i = 0; i < qWeights[qNum]; i++)
        {
            packet p = queue[qNum]->pop(ok);

            if(!ok)
                break;

            // Residence Time
            calcTime = std::time(0) - p.arrivalTime;
            pResidenceTime.push_back(calcTime);

            if(this->vPacketSize)
                this->pSize = p.packetv4.total_length;

            totalPacketSize += this->pSize;

            this->link->transfer(p);
            ++totalPacketsProccessed;
        }

        if(i == 0) continue;

        if(this->vPacketSize)
            this->serviceTime = ceil(((totalPacketSize * 8.0) / outrate) * 1000) ; // in milli seconds

        msleep(this->serviceTime);

        for(int i = 0; i < pResidenceTime.count(); ++i){
            queue[qNum]->residenceTime.push_back(pResidenceTime.at(i) + this->serviceTime);
        }

        pResidenceTime.clear();
    }

    qDebug() << "Dequeuing completed.";
}

void DeQueueProcessor::terminate()
{
    doTerminate = true;
}
