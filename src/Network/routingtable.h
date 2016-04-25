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
        void lookUp(unsigned char [], int &, int &, unsigned char &);
        void printRoutingTable();
    private:
        int rEntrySize;

        void loadRoutingTable();
        int countMaskBits(unsigned char);
};

#endif // ROUTINGTABLE_H
