#ifndef INPUTADAPTOR_H
#define INPUTADAPTOR_H
#include "Commons/commons.h"
#include "../routingtable.h"
#include "Queue/queue.h"
#include <QThread>

class Router;

class InputAdaptor: public QThread
{
    public:
        int inputRate,
            processedPackets,
            num_input_packets,
            pSize,
            serviceType;

        Router *r;
        Queue *inpQueue;
        RoutingTable *routingTable;

        InputAdaptor(Router *, int, QString, QString, int);
        void loadQueue(QString);
        void insertInQueue(packet);
        void run();
        void borderRun();
        void coreRun();
};

#endif // INPUTADAPTOR_H
