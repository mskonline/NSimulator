#ifndef LINK_H
#define LINK_H

#include "Commons/commons.h"

class Router;

class Link
{
    public:
        Router *destinationRouter;
        int destinationPort;
        int packetsTransferred;

        Link(Router *, int);
        void transfer(packet);
        void printInfo();
};

#endif // LINK_H
