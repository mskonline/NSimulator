#ifndef NETWORK_H
#define NETWORK_H
#include "Router/router.h"
#include "routingtable.h"
#include "Interface/interface.h"

class Network : public QObject
{
    Q_OBJECT

    private:

        QStringList routersList, linksList;
        QString routingSrc;
        Interface *nsInterface;

        bool isRunning;

    public:

        int numRouters;
        Router **routers;
        Network();
        ~Network();

        void initiate();
        void setInterfaceObj(Interface *);

    public slots:

        void run();
        void stop();
        void rFinished();
};

#endif // NETWORK_H
