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
            *output_rates;

        QString routingTable;
        QStringList inputFiles;
        QStringList outputFiles;
        Interface *interface;
        bool allSet;

        // Threading
        QMutex *mutex;
        bool allPacketsProcessed;
        int totalInputPackets;
        int nCount;

        float *qLoadFactor;
        int qSize;

    public:
        InputAdaptor **inpAdaptors;
        OutputAdaptor **outAdaptors;
        QString name;
        QString logText;

        Router(QString);
        ~Router();

        void setInterfaceObj(Interface *);
        void setRoutingTable(QString);
        void initiate();
        void fabric(packet,int,int);
        void notify(int);
        void run();
        void stop();
};

#endif // ROUTER_H
