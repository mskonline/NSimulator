#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <QList>
#include "Commons/commons.h"

class RoutingTable
{
    public:
        QList<Routing> *routingTable;
        QString routingFile;

        RoutingTable(QString);
        ~RoutingTable();

        int getRoutingEntriesCount();
        int lookUp(unsigned char []);
    private:
        int rEntrySize;

        void loadRoutingTable();
};

#endif // ROUTINGTABLE_H
