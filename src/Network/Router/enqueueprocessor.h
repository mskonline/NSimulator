#ifndef ENQUEUEPROCESSOR_H
#define ENQUEUEPROCESSOR_H

#include <QThread>
#include <QQueue>
#include "Queue/queue.h"

#include "../../Commons/commons.h"

class OutputAdaptor;

class EnQueueProcessor: public QThread
{

    public:
        QQueue<packet> *pBuffer;
        Queue *queue;
        int arrivalRate,
            totalPacketsProccessed;

        OutputAdaptor *o;
        EnQueueProcessor(OutputAdaptor *o, QQueue<packet> *, Queue *, int);
        void run();
};

#endif // ENQUEUEPROCESSOR_H
