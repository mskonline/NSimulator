#ifndef ROUTER_H
#define ROUTER_H
#include "../../Interface/interface.h"
#include "../routingtable.h"
#include "outputadaptor.h"
#include <QString>
#include <QStringList>
#include <QFile>

class InputAdaptor;

class Router
{
    private:
        QQueue<packet> **inpQueues;
        QQueue<packet> **outQueues;
        QFile **oFiles;

        int numInputs,
            numOutputs,
            num_input_packets,
            num_output_packets,
            *input_rates,
            *output_rates;

        RoutingTable *routingTable;
        QStringList inputFiles;
        QStringList outputFiles;
        Interface *interface;
        bool allSet;
    public:
        QString name;

        Router(QString);
        ~Router();

        void setInterfaceObj(Interface *);
        void setRoutingTable(RoutingTable *);
        void initiate();
        void fabric(packet,int,int);
        void run();
};

#endif // ROUTER_H
