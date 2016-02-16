#include "routingtable.h"
#include <QFile>

RoutingTable::RoutingTable(std::string routingFile)
{
    routingTable = NULL;
    this->routingFile = routingFile;

    //TODO
    //this->loadRoutingTable();
}


void RoutingTable::loadRoutingTable()
{
    if(routingTable){
        routingTable->clear();
    } else
        routingTable = new QList<Routing>();


    QFile f(QString::fromStdString(this->routingFile));
    char *buffer;
    QByteArray ba;

    if(f.open(QFile::ReadOnly)){
        ba = f.readAll();
    }

    buffer = ba.data();

    int bufferSize = f.size();
    int rEntrySize = sizeof(Routing);
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
            mask_result = r.destination[j] & r.destinationMask[j];

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
