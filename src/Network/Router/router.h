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
class Link;

class Router: public QThread
{
    Q_OBJECT
    private:
        int pProcessed,
            *input_rates,
            *arrival_rates,
            *output_rates;
            *qWeights;


        QString routingTable;
        QStringList inputs;
        QStringList outputs;
        Interface *interface;
        bool allSet;

        // Threading
        QMutex *mutex;
        bool allInpPacketsProcessed,
            allOutPacketsProcessed;

        int totalInputPackets;
        int nInpCount, nOutCount;
        int w;
        int packetSize;
        float *qLoadFactor;
        int qSize;

    public:
        int id;
        int numInputs, numOutputs, *numQueues;
        int processedPackets, processedPacketsToDest;
        int serviceType;

        int startTime;
        float endTime;

        InputAdaptor **inpAdaptors;
        OutputAdaptor **outAdaptors;
        QString rid, rname;
        QString logText;

        bool isBorder;
        int arrivalRateType;

        std::vector<float> meanResidenceTimePerQueue,
                           meanNumResidentItemsPerQueue;

        std::vector<int> maxNPacketsPerQueue, maxResTimePerQueue;


        QList<int> diffServCodes;

        Router(int, int);
        ~Router();

        void setInterfaceObj(Interface *);
        void setRoutingTable(QString);
        void setOutputLink(Link *, int);
        void initiate(QString);
        void fabric(packet,int,int);
        void inpNotify(int);
        void outNotify();
        void run();
        void borderRun();
        void coreRun();
        void stop();
        void insertPacket(packet, int);
        void performAnalysis();

        void printOutputLinkInfo();

    signals:
        void rfinished(int);
};

#endif // ROUTER_H
