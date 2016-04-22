#ifndef OUTPUTADAPTOR_H
#define OUTPUTADAPTOR_H
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QFile>
#include <vector>
#include "Commons/commons.h"

#include "Queue/queue.h"
#include "enqueueprocessor.h"
#include "dequeueprocessor.h"

class Router;
class Link;

class OutputAdaptor: public QThread
{
    public:
        QQueue<packet> **pBuffers;
        Queue **queues;
        EnQueueProcessor **enqProc;
        DeQueueProcessor *deqProc;

        int outputRate,
            bprocessedPackets,
            processedPackets,
            processedPacketsToDest,
            droppedPCount,
            numQueues,
            nEnqProcs,
            id,
            pSize,
            serviceTime,
            outRate;

        int *pPerQueue;
        Router *r;
        Link *link;
        bool bPacketsComplete, vPacketSize, doTerminate;
        QFile *outFile;
        QMutex mutex;

        std::vector<int> qWeights;

        OutputAdaptor(int);
        OutputAdaptor(Router *,int, QString, std::vector<int>, int, int, int, std::vector<int>);
        ~OutputAdaptor();

        void run();
        void borderRun();
        void coreRun();
        void terminate();
        void notify(int);
        void putPacket(packet, int);
        void setLink(Link *);

        void printLinkInfo();
};

#endif // OUTPUTADAPTOR_H
