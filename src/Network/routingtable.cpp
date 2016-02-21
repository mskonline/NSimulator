#include "routingtable.h"
#include <QFile>

RoutingTable::RoutingTable(QString routingFile)
{
    this->routingTable = NULL;
    this->routingFile = routingFile;
    this->rEntrySize = sizeof(Routing);

    //TODO
    this->loadRoutingTable();
}


void RoutingTable::loadRoutingTable()
{
    if(routingTable){
        routingTable->clear();
    } else
        routingTable = new QList<Routing>();


    QFile f(this->routingFile);
    char *buffer;
    QByteArray ba;

    if(f.open(QFile::ReadOnly)){
        ba = f.readAll();
    }

    buffer = ba.data();

    int bufferSize = ba.size();
    int i = 0;

    while(i < bufferSize)
    {
        Routing r;
        memcpy(&r, buffer, 22);
        routingTable->append(r);

        buffer += rEntrySize;
        i += rEntrySize;
    }
}

int RoutingTable::lookUp(char destinationAddr[])
{
    int longestMatchEntry = 0;
    int matchCount = 0;
    unsigned char longestMatch = 0;
    unsigned char mask_result, xor_result;

    for(int i = 0; i < routingTable->size(); ++i)
    {
        Routing r = routingTable->at(i);

        for(int j = 0; j < sizeof(r.destinationMask); ++j){

            if(r.destinationMask[j] == 0){
                break;
            }

            // Masking the destination addr in the rTable
            mask_result = r.destination_addr[j] & r.destinationMask[j];

            // XOR operation
            xor_result = (mask_result ^ destinationAddr[j]);

            if(xor_result == 0)
            {
                // 8 bits are matched
                matchCount += 8;
                continue;
            }
            else
            {
                // Count number of Zero's in the result
                for(int k = 0; k < 8; ++k)
                {
                    int kthbit = ((xor_result & (128 >> k)) >> (7 - k));
                    if(kthbit == 0)
                        ++matchCount;
                    else
                        break;
                }
                break;
            }
        }

        if(matchCount >= longestMatch){
            longestMatchEntry = i;
            longestMatch = matchCount;
        }

        matchCount = 0;
    }

    return routingTable->at(longestMatchEntry).outputPort[0];

}

int RoutingTable::getRoutingEntriesCount()
{
    if(routingTable)
        return this->routingTable->size();
    else
        return 0;
}

RoutingTable::~RoutingTable()
{
    if(routingTable){
        this->routingTable->clear();
        delete this->routingTable;
    }
}
