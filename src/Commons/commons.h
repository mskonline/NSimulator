#ifndef COMMONS_H
#define COMMONS_H

struct Routing
{
    unsigned char source_addr[4];
    unsigned char destination_addr[4];
    unsigned char destinationMask[4];
    unsigned char nextRouterHop[4];
    unsigned char outputPort[1];
    unsigned char outputPortQ[1];
    unsigned char future[4];
};

struct ipv4 {
    unsigned char version:4, ihl:4; // 1 Byte
    unsigned char qos; // 1 Byte
    unsigned short int total_length; // 2 Bytes
    unsigned short int identification; // 2 Bytes
    unsigned short int flags:3, frag_offset:13; // 2 Bytes
    unsigned char ttl; // 1 Byte
    unsigned char protocol; // 1 Byte
    unsigned short int checksum; // 2 Bytes
    unsigned char source_addr[4]; // 4 Bytes
    unsigned char destination_addr[4]; // 4 Bytes
    unsigned int opt_addr[5]; // 20 bytes
    unsigned int tcp_header[5]; // 20 bytes
    unsigned int data[110]; // 440 bytes
};

struct ipv6 {
    unsigned int version:4, traffic_class:8, flow_lable:20; // 4 Byte
    unsigned short int payload_length; // 2 Bytes
    unsigned char next_header; // 1 Byte
    unsigned char hop_limit; // 1 Byte
    unsigned char source_addr[16]; // 16 Bytes
    unsigned char destination_addr[16]; // 16 Bytes
    unsigned int tcp_header[5]; // 20 bytes
    unsigned int data[110]; // 440 bytes
};

struct packet {
    int arrivalTime;
    ipv4 packet;
};

#endif // COMMONS_H
