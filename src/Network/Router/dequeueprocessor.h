#ifndef DEQUEUEPROCESSOR_H
#define DEQUEUEPROCESSOR_H

#include <QThread>
#include <QFile>
#include "Queue/queue.h"

#include "../../Commons/commons.h"

class Link;

class DeQueueProcessor: public QThread
{
    public:
        Queue **queue;
        int outputrate,
            serviceTime,
            outrate,
            pSize,
            totalPacketsProccessed;

        Link *link;
        bool vPacketSize, doTerminate;

        std::vector<int> qWeights;

        DeQueueProcessor();
        DeQueueProcessor(Queue **, int, int, int, bool, std::vector<int>);
        void run();
        void terminate();
};

#endif // DEQUEUEPROCESSOR_H
