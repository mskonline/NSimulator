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

struct ipv4 {
    unsigned char addr[20]; // 20 bytes
    int opt_addr[5]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

struct ipv6 {
    int addr [10]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

class Packet {
    // This variable is to track the arrival time
    int arrivalTime;
    ipv4 packet;
};

#endif // COMMONS_H
