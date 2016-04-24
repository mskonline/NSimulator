#ifndef NETWORK_H
#define NETWORK_H
#include "Router/router.h"
#include "link.h"
#include "routingtable.h"
#include "Interface/interface.h"

#include <QTimer>

class Network : public QObject
{
    Q_OBJECT

    private:

        Interface *nsInterface;
        QStringList routersList,
                    linksList;
        QString routingSrc;
        bool isRunning,
             isborderRouterFinished;

        QTimer *statusTimer;

        int rCounter,
            totalPacketsinNetwork,
            packetsProcessed;

        int startTime;

    public:

        int numRouters,
            numLinks;

        Router **routers;
        Link **links;

        Network();
        ~Network();

        void initiate();
        void setInterfaceObj(Interface *);
        void performAnalysis();

    public slots:

        void run();
        void stop();
        void checkStatus();
        void rFinished(int);
};

#endif // NETWORK_H
