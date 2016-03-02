#ifndef INPUTADAPTOR_H
#define INPUTADAPTOR_H
#include "Commons/commons.h"
#include "../routingtable.h"
#include <QThread>
#include <QQueue>

class Router;

class InputAdaptor: public QThread
{
    public:
        int inputRate,
            processedPackets,
            num_input_packets,
            delay;

        Router *r;
        QQueue<packet> inpQueue;
        RoutingTable *routingTable;

        InputAdaptor(Router *, int, int, QString);
        InputAdaptor(Router *, int, int, QString, QString);
        void loadQueue(QString);
        void run();
};

#endif // INPUTADAPTOR_H
