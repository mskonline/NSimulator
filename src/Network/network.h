#ifndef NETWORK_H
#define NETWORK_H
#include "routingtable.h"
#include "Interface/interface.h"

class Network
{
    private:

    std::string routingSrc;
    Interface *interface;

    public:
        RoutingTable *rTable;

        Network();
        ~Network();

        void initiateRoutingTable();
        void setRoutingSrc(std::string);
        void setInterfaceObj(Interface *);
};

#endif // NETWORK_H
