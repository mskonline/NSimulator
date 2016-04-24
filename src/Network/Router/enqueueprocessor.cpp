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
    int i = 0;
    int numerator=pBuffer->size(),l,denominator=arrivalRate;
    qDebug() << numerator << denominator;
    if (numerator < denominator*denominator*2)
    {
        l = 1;
    }
    else
    {
        while(1)
        {
            if(denominator*denominator*i < numerator && numerator < denominator*denominator*2*i)
            {
                l = i;
                numerator=numerator/l;
                break;
            }
            ++i;

        }
    }

    int b = numerator/denominator;// b=132 ,nume = 1188 c = 0 deno = 9 l = 8
    int d = b/denominator;//14.66
    int e = d%2, f=e%2;



    vector<int> ans;
    vector<int> blank;

    if(e == 0)
    {
        ans.push_back(denominator+f);// 8

        for(int i = 0; i < b/2 ; ++i)
        {
            ans.push_back(i+denominator+1+f); // 6 7 8 9

        }


        for(int j = b/2 - 1; j > 0 ; j--)
        {
            ans.push_back(denominator-j+f); // 3 4 5s


        }

    }
    else
    {
        ans.push_back(denominator+e); //6

        b = b - 1; //8
        for(int i = 0; i < b/2; i++)
        {
            ans.push_back(i+denominator+e+f); // 6 7
        }
        for(int j = b/2+1 ; j > 0 ; j--) //
        {
            ans.push_back(denominator-j+e); // 4 5
        }
    }
    for (int k=0; k<l; k++)
    {
        blank.insert(blank.end(), ans.begin(), ans.end());
    }

    int sum = 0;
    for(int i = 0; i < blank.size(); ++i)
        sum += blank[i];

    qDebug() << "Actual arrival " << denominator << "Measured " << ((sum) / blank.size());

    while(1)
    {
        if(pBuffer->empty())
            break;

        for(int i = 0; i < blank.back(); ++i){
            if(pBuffer->empty())
                break;

            packet p = pBuffer->dequeue();
            p.arrivalTime = std::time(0);
            this->queue->push(p);
            ++totalPacketsProccessed;
        }
        blank.pop_back();
        sleep(1);
    }

    cout << "Buffer flushed" << endl;
    o->notify(totalPacketsProccessed);
}
