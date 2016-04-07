#include "enqueueprocessor.h"
#include "outputadaptor.h"
#include <iostream>
#include <ctime>
#include <QDebug>
using namespace std;

EnQueueProcessor::EnQueueProcessor(OutputAdaptor *o, QQueue<packet> *bq, Queue *q, int rate)
{
    this->queue = q;
    this->pBuffer = bq;
    this->arrivalRate = rate;
    this->o = o;

    totalPacketsProccessed = 0;
}

void EnQueueProcessor::run()
{
    while(1)
    {
        if(pBuffer->empty())
            break;

        for(int i = 0; i < arrivalRate; ++i){
            if(pBuffer->empty())
                break;

            packet p = pBuffer->dequeue();
            p.arrivalTime = std::time(0);
            this->queue->push(p);
            ++totalPacketsProccessed;
        }

        sleep(1);
    }

    cout << "Buffer flushed" << endl;
    o->notify(totalPacketsProccessed);
}
