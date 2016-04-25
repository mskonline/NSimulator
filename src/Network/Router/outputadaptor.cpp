#include "outputadaptor.h"
#include "router.h"
#include "Network/link.h"

#include <QQueue>
#include <QFile>
#include <iostream>
#include <math.h>
#include <ctime>
#include <QDebug>

using namespace std;

OutputAdaptor::OutputAdaptor(int delay)
{
    droppedPCount = 0;
}

OutputAdaptor::OutputAdaptor(Router *r, int id, QString file, std::vector<int> arrivalRate, int outRate,
                             int numQueues, int pSize, std::vector<int> qWeights)
{
    droppedPCount = 0;
    nEnqProcs = 0;
    bPacketsComplete = false;
    processedPackets = 0;
    processedPacketsToDest = 0;
    bprocessedPackets = 0;
    this->id = id + 1;
    this->outFile = NULL;
    this->deqProc = NULL;
    this->r = r;
    this->numQueues = numQueues;
    this->outRate = outRate;
    this->qWeights = qWeights;
    this->pSize = pSize;
    pPerQueue = new int(numQueues);

    for(int i = 0; i < numQueues; ++i)
        pPerQueue[i] = 0;

    // TODO
    queues = new Queue*[this->numQueues];

    for(int i = 0; i < this->numQueues; ++i)
        queues[i] = new Queue(1000);

    if(pSize > 0){
        this->vPacketSize = false;
        this->serviceTime = ceil(((pSize * 8.0) / outRate) * 1000); // in milli seconds
    } else
        this->vPacketSize = true;

    if(this->r->isBorder)
    {
        pBuffers = new QQueue<packet>*[this->numQueues];
        enqProc = new EnQueueProcessor*[this->numQueues];


        for(int i = 0; i < this->numQueues; ++i)
            pBuffers[i] = new QQueue<packet>();

        deqProc = new DeQueueProcessor(queues, serviceTime, outRate, pSize, vPacketSize, qWeights);

        for(int i = 0; i < this->numQueues; ++i) {
            enqProc[i] = new EnQueueProcessor(this, pBuffers[i], queues[i], arrivalRate[i], r->arrivalRateType);
        }
    }

    if(file != "L"){
        outFile = new QFile(file);
        if(!outFile->open(QFile::WriteOnly | QFile::Truncate)){
        }
    }

    doTerminate = false;
    this->link = NULL;
}

void OutputAdaptor::run()
{
    if(this->r->isBorder)
        this->borderRun();
    else
        this->coreRun();
}

void OutputAdaptor::coreRun()
{
    //Run for all downstream routers
    processedPackets = 0;
    int calcTime, qNum, temp = -1, i;
    int totalPacketSize = 0;
    bool ok;

    int fTime = 0,
        fCount = 0,
        flow,
        flowCount = 1,
        newFlow = 0,
        flowTime;

    QVector<int> pResidenceTime;

    while(1)
    {
        if(doTerminate) break;

        totalPacketSize = 0;

        ++temp;
        qNum = temp % this->numQueues;

        if(queues[qNum]->empty())
        {
            msleep(10);
            continue;
        }

        queues[qNum]->logQDepth();

        for (i = 0; i < qWeights[qNum]; i++)
        {
            packet p = queues[qNum]->pop(ok);

            if(!ok)
                break;

            // Residence Time
            calcTime = std::time(0) - p.arrivalTimeAtRouter;
            pResidenceTime.push_back(calcTime);

            if(this->vPacketSize)
                this->pSize = p.packetv4.total_length;

            totalPacketSize += this->pSize;

            p.flow = (p.flow * 10) + r->id + 1;

            if(this->link)
                link->transfer(p);
            else
            {
                flowTime = std::time(0) - p.arrivalTimeInNetwork;
                outFile->write(reinterpret_cast<char*>(&p.packetv4), this->pSize);

                /*
                 * Calculating flow parameters
                 */

                if(!flowMap.contains(p.flow))
                {
                    newFlow = flowCount;

                    flowMap.insert(p.flow, newFlow);
                    flowTotalTime.insert(newFlow,0);
                    flowTotalPackets.insert(newFlow,0);

                    ++flowCount;
                }

                flow = flowMap.value(p.flow);

                fTime = flowTotalTime.value(flow);
                flowTotalTime.insert(flow, fTime + flowTime);

                fCount = flowTotalPackets.value(flow);
                flowTotalPackets.insert(flow, fCount + 1);

                ++processedPacketsToDest;
            }

            ++pPerQueue[qNum];
            ++processedPackets;
        }

        if(i == 0) continue;

        if(this->vPacketSize)
            this->serviceTime = ceil(((totalPacketSize * 8.0) / outRate) * 1000) ; // in milli seconds

        msleep(this->serviceTime);

        for(int i = 0; i < pResidenceTime.count(); ++i){
            queues[qNum]->residenceTime.push_back(pResidenceTime.at(i) + this->serviceTime);
        }

        pResidenceTime.clear();
    }
}

void OutputAdaptor::borderRun()
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
    doTerminate = true;

    if(!this->link)
    {
        outFile->flush();
        outFile->close();
    }

    if(this->r->isBorder)
        deqProc->terminate();

    QThread::terminate();
}

void OutputAdaptor::putPacket(packet p, int qNum)
{
   QMutexLocker mlocker(&mutex);

   if(r->isBorder){
       pBuffers[qNum]->enqueue(p);
   }
   else {
       queues[qNum]->push(p);
   }
}

void OutputAdaptor::notify(int bprocessedPackets)
{
    ++nEnqProcs;

    this->bprocessedPackets += bprocessedPackets;

    if(nEnqProcs == this->numQueues)
        bPacketsComplete = true;
}

void OutputAdaptor::setLink(Link *l)
{
    this->link = l;

    if(deqProc)
        deqProc->link = l;
}

void OutputAdaptor::printLinkInfo()
{
    if(this->link)
        this->link->printInfo();
    else if(outFile)
        qDebug() << "Destination file is : " << outFile->fileName();
    else
        qDebug() << "Destination undefined";
}

OutputAdaptor::~OutputAdaptor()
{
}
