#ifndef ROUTER_H
#define ROUTER_H
#include "../../Interface/interface.h"
#include "../routingtable.h"
#include "outputadaptor.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QMutex>

class InputAdaptor;

class Router: public QThread
{
    private:
        int numInputs,
            numOutputs,
            pProcessed,
            *input_rates,
            *arrival_rates,
            *output_rates;

        QString routingTable;
        QStringList inputFiles;
        QStringList outputFiles;
        Interface *interface;
        bool allSet;

        // Threading
        QMutex *mutex;
        bool allInpPacketsProcessed,
            allOutPacketsProcessed;

        int totalInputPackets;
        int nInpCount, nOutCount;
        int numQueues;
        int packetSize;
        float *qLoadFactor;
        int qSize;

    public:
        InputAdaptor **inpAdaptors;
        OutputAdaptor **outAdaptors;
        QString name;
        QString logText;

        Router(QString, int);
        ~Router();

        void setInterfaceObj(Interface *);
        void setRoutingTable(QString);
        void initiate();
        void fabric(packet,int,int);
        void inpNotify(int);
        void outNotify();
        void run();
        void stop();
        void performAnalysis();
};

#endif // ROUTER_H
