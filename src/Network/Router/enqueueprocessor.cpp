#include "enqueueprocessor.h"
#include "outputadaptor.h"
#include <iostream>
#include <ctime>
#include <QDebug>
using namespace std;

EnQueueProcessor::EnQueueProcessor(OutputAdaptor *o, QQueue<packet> *bq, Queue *q, int rate, int arrivalType)
{
    this->queue = q;
    this->pBuffer = bq;
    this->arrivalRate = rate;
    this->arrivalType = arrivalType;
    this->o = o;

    totalPacketsProccessed = 0;
}


void EnQueueProcessor::run()
{
    if(arrivalType == POISSON_ARRIVAL)
        this->poissonRun();
    else
        this->deterministicRun();
}

void EnQueueProcessor::deterministicRun()
{
    while(1)
    {
        if(pBuffer->empty())
            break;

        for(int i = 0; i < arrivalRate; ++i){
            if(pBuffer->empty())
                break;

            packet p = pBuffer->dequeue();
            p.arrivalTimeInNetwork = p.arrivalTimeAtRouter = std::time(0);
            this->queue->push(p);
            ++totalPacketsProccessed;
        }

        sleep(1);
    }

    cout << "Buffer flushed" << endl;
    o->notify(totalPacketsProccessed);
}

void EnQueueProcessor::poissonRun()
{
    int i = 0;
    int numerator = pBuffer->size(), l, denominator = arrivalRate;

    int cValue = denominator * denominator;

    if (numerator < cValue * 2)
        l = 1;
    else
    {
        while(1)
        {
            if(cValue * i < numerator && numerator < cValue * 2 * i)
            {
                l = i;
                numerator = numerator / l;
                break;
            }
            ++i;

        }
    }

    int numValues = numerator / denominator;
    int ratio = numValues / denominator;
    int e = ratio % 2, f = e % 2;

    vector<int> pArray;
    vector<int> poissonArrivals;

    // Even case
    if(e == 0)
    {
        pArray.push_back(denominator + f);

        for(int i = 0; i < numValues/2 ; ++i)
            pArray.push_back(i + denominator + 1 + f);

        for(int j = numValues/2 - 1; j > 0 ; j--)
            pArray.push_back(denominator - j + f);
    }
    // Odd case
    else
    {
        pArray.push_back(denominator+e);

        numValues = numValues - 1;
        for(int i = 0; i < numValues/2; i++)
            pArray.push_back(i + denominator + e + f);
        for(int j = numValues/2+1 ; j > 0 ; j--)
            pArray.push_back(denominator - j + e);
    }

    // Duplicating the pArray to build the final poissonArrival array
    for (int k = 0; k < l; k++)
        poissonArrivals.insert(poissonArrivals.end(), pArray.begin(), pArray.end());

    int sum = 0;
    for(int i = 0; i < poissonArrivals.size(); ++i)
        sum += poissonArrivals[i];

    qDebug() << "Actual : " << arrivalRate << "Measured " << (sum / poissonArrivals.size());

    while(1)
    {
        if(pBuffer->empty())
            break;

        for(int i = 0; i < poissonArrivals.back(); ++i){
            if(pBuffer->empty())
                break;

            packet p = pBuffer->dequeue();
            p.arrivalTimeInNetwork = p.arrivalTimeAtRouter = std::time(0);
            this->queue->push(p);
            ++totalPacketsProccessed;
        }

        poissonArrivals.pop_back();
        sleep(1);
    }

    cout << "Buffer flushed" << endl;
    o->notify(totalPacketsProccessed);
}
