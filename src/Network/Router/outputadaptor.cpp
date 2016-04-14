#include "outputadaptor.h"
#include "router.h"
#include <QQueue>
#include <QFile>
#include <iostream>
#include <math.h>
#include <QDebug>

using namespace std;

OutputAdaptor::OutputAdaptor(int delay)
{
    droppedPCount = 0;
}

OutputAdaptor::OutputAdaptor(Router *r, int id, QString file, std::vector<int> arrivalRate, int outRate,
                             int numQueues, int pSize, int *qWeights)
{
    droppedPCount = 0;
    nEnqProcs = 0;
    bPacketsComplete = false;
    processedPackets = 0;
    bprocessedPackets = 0;
    this->id = id + 1;

    this->r = r;
    this->numQueues = numQueues;
    pPerQueue = new int(numQueues);

    pBuffers = new QQueue<packet>*[this->numQueues];
    queues = new Queue*[this->numQueues];
    enqProc = new EnQueueProcessor*[this->numQueues];

    for(int i = 0; i < this->numQueues; ++i)
        pBuffers[i] = new QQueue<packet>();

    for(int i = 0; i < this->numQueues; ++i)
        queues[i] = new Queue(1000);

    for(int i = 0; i < this->numQueues; ++i) {
        enqProc[i] = new EnQueueProcessor(this,pBuffers[i], queues[i],arrivalRate[i]);
    }

    if(pSize > 0){
        this->vPacketSize = false;
        this->serviceTime = ceil(((pSize * 8.0) / outRate) * 1000); // in milli seconds
    } else
        this->vPacketSize = true;

    deqProc = new DeQueueProcessor(file, queues, serviceTime, outRate, pSize, vPacketSize, qWeights);
}

void OutputAdaptor::run()
{
    for(int i = 0; i < this->numQueues; ++i){
        pPerQueue[i] = pBuffers[i]->size();
        enqProc[i]->start();
    }

    deqProc->start();

    while(1){
        if(bPacketsComplete)
            break;
        msleep(100);
    }

    msleep(500);

    while(1)
    {
        processedPackets = deqProc->totalPacketsProccessed;

        cout << "Processed packets at Output Adaptor " << this->id << " : " << processedPackets << endl;

        if(deqProc->totalPacketsProccessed == bprocessedPackets){
            deqProc->terminate();
            break;
        }
        sleep(2);
    }

    r->outNotify();
}

void OutputAdaptor::terminate()
{
    cout << "Output Adaptor finished " << endl;
    QThread::terminate();
}

void OutputAdaptor::putPacket(packet p, int qNum)
{
   QMutexLocker mlocker(&mutex);
   pBuffers[qNum]->enqueue(p);
}

void OutputAdaptor::notify(int bprocessedPackets)
{
    ++nEnqProcs;

    this->bprocessedPackets += bprocessedPackets;

    if(nEnqProcs == this->numQueues)
        bPacketsComplete = true;
}

OutputAdaptor::~OutputAdaptor()
{
}
