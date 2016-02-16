#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <QList>
#include "Commons/commons.h"

class RoutingTable
{
    public:
        QList<Routing> *routingTable;
        std::string routingFile;

        RoutingTable(std::string);
        ~RoutingTable();

        int getRoutingEntriesCount();
        int lookUp(char []);
    private:

        void loadRoutingTable();
};

#endif // ROUTINGTABLE_H
