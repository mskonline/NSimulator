#ifndef LINK_H
#define LINK_H

#include "Commons/commons.h"

class Router;

class Link
{

    public:
        Router *destRouter;
        Link(Router *);
        void transfer(packet);
};

#endif // LINK_H
