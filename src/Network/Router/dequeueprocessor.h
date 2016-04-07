#ifndef DEQUEUEPROCESSOR_H
#define DEQUEUEPROCESSOR_H

#include <QThread>
#include <QFile>
#include "Queue/queue.h"

#include "../../Commons/commons.h"


class DeQueueProcessor: public QThread
{
    public:
        Queue **queue;
        int outputrate,
            serviceTime,
            outrate,
            pSize,
            totalPacketsProccessed;

        QFile *outFile;
        bool vPacketSize;

        DeQueueProcessor();
        DeQueueProcessor(QString, Queue **, int, int, int, bool);
        void run();
        void terminate();
};

#endif // DEQUEUEPROCESSOR_H
