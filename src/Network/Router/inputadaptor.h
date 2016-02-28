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
            num_input_packets;

        Router *r;
        QQueue<packet> inpQueue;
        RoutingTable *routingTable;

        InputAdaptor(Router *);
        InputAdaptor(Router *,QString);
        void loadQueue(QString);
        void run();
};

#endif // INPUTADAPTOR_H
