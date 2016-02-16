#ifndef COMMONS_H
#define COMMONS_H

struct Routing
{
    unsigned char source[4];
    unsigned char destination[4];
    unsigned char destinationMask[4];
    unsigned char nextRouterHop[4];
    unsigned char outputPort[1];
    unsigned char outputPortQ[1];
    unsigned char future[4];
};

#endif // COMMONS_H
