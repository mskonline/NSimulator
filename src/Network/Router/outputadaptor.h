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
            droppedPCount,
            numQueues,
            nEnqProcs,
            id,
            serviceTime;

        int *pPerQueue;
        Router *r;
        bool bPacketsComplete, vPacketSize;
        QFile *outFile;
        QMutex mutex;

        OutputAdaptor(int);
        OutputAdaptor(Router *,int, QString, std::vector<int>, int, int, int, int *);
        ~OutputAdaptor();

        void run();
        void terminate();
        void notify(int);
        void putPacket(packet, int);
};

#endif // OUTPUTADAPTOR_H
