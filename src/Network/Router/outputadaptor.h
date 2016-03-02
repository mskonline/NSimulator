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
            processedPackets,
            delay,
            qSize,
            maxQSize,
            droppedPCount;

        QFile *outFile;
        QMutex oMutex;
        OutputAdaptor(int);
        OutputAdaptor(QString, int, int, int, float);
        ~OutputAdaptor();

        std::vector<int> residenceTime, procsTime, itemsInQ;

        void run();
        void terminate();
        void putPacket(packet);
};

#endif // OUTPUTADAPTOR_H
