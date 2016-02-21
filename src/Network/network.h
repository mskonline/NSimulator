#ifndef NETWORK_H
#define NETWORK_H
#include "Router/router.h"
#include "routingtable.h"
#include "Interface/interface.h"

class Network : public QObject
{
    Q_OBJECT
    private:
        QStringList routersList;
        int numRouters;
        Router **routers;

        QString routingSrc;
        Interface *interface;

        bool isRunning;

    public:
        RoutingTable *rTable;

        Network();
        ~Network();

        void initiate();
        void initiateRoutingTable();
        void setRoutingSrc(QString);
        void setInterfaceObj(Interface *);

    public slots:

        void run();
};

#endif // NETWORK_H
