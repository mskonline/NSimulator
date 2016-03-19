#ifndef OUTPUTADAPTOR_H
#define OUTPUTADAPTOR_H
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QFile>
#include "Commons/commons.h"

#include <boost/lockfree/queue.hpp>


class OutputAdaptor: public QThread
{
    public:
        //QQueue<packet> **outQueues;

        boost::lockfree::queue<packet> oQueue;
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

        std::vector<double> residenceTime, procsTime, itemsInQ;

        void run();
        void terminate();
        void putPacket(packet);
};

#endif // OUTPUTADAPTOR_H
