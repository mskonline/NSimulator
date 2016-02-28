#ifndef OUTPUTADAPTOR_H
#define OUTPUTADAPTOR_H
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QFile>
#include "Commons/commons.h"


class OutputAdaptor: public QThread
{
    public:
        QQueue<packet> outQueue;
        //QQueue<packet> **outQueues;

        int outputRate,
            numPacketsProcessed;

        QFile *outFile;
        QMutex mutex;
        OutputAdaptor();
        OutputAdaptor(QString,int);
        ~OutputAdaptor();

        void run();
        void terminate();
        void putPacket(packet);
};

#endif // OUTPUTADAPTOR_H
