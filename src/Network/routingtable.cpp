#include "routingtable.h"
#include <QFile>
#include <iostream>
#include <QDebug>

RoutingTable::RoutingTable(QString routingFile)
{
    this->routingTable = NULL;
    this->routingFile = routingFile;
    this->rEntrySize = sizeof(Routing);

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
        memcpy(&r, buffer, ROUTING_ENTRY_SIZE);
        routingTable->append(r);

        buffer += rEntrySize;
        i += rEntrySize;
    }
}

void RoutingTable::lookUp(unsigned char destinationAddr[], int &port, int &qNum)
{
    int longestMatchEntry = 0;
    int matchCount, cBits;
    unsigned char longestMatch = 0;
    unsigned char mask_result, xor_result;

    for(int i = 0; i < routingTable->size(); ++i)
    {
        matchCount = 0;
        Routing r = routingTable->at(i);

        for(unsigned int j = 0; j <= sizeof(r.destination_mask) - 1; ++j){

            if(r.destination_mask[j] == 0){
                break;
            }

            // Masking the destination addr in the rTable
            mask_result = destinationAddr[j] & r.destination_mask[j];

            // XOR operation
            xor_result = (mask_result ^ r.destination_addr[j]);

            if(xor_result == 0)
            {
                // 8 bits are matched
                matchCount += 8;
                continue;
            }
            else
            {
                cBits = countMaskBits(r.destination_mask[j]);

                // Count number of Zero's in the result
                for(int k = 0; k < cBits; ++k)
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

        if(matchCount > longestMatch){
            longestMatchEntry = i;
            longestMatch = matchCount;
        }
    }

    longestMatchEntry = longestMatch != 0 ?
                        longestMatchEntry : (routingTable->length() - 1);

    //qDebug() << longestMatchEntry;
    port = routingTable->at(longestMatchEntry).outputPort[0] - 1;
    qNum = routingTable->at(longestMatchEntry).outputPortQ[0] - 1;
}

/*
 * Counting Bits, Brian Kernighan's way
 * Credit: http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
 */
int RoutingTable::countMaskBits(unsigned char mask)
{
    unsigned char c;
    for (c = 0; mask; c++)
      mask &= mask - 1;

    return c;
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
