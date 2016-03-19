#ifndef COMMONS_H
#define COMMONS_H

struct Routing
{
    unsigned char source_addr[4]; // 4 Bytes
    unsigned char destination_addr[4]; // 4 Bytes
    unsigned char destination_mask[4]; // 4 Bytes
    unsigned char nextRouterHop[4]; // 4 Bytes
    unsigned char outputPort[1];// 1 Bytes
    unsigned char outputPortQ[1];// 1 Bytes
    unsigned char dscp[1];// 1 Bytes
    unsigned char padding[3]; // 3 Bytes
};

/*
 * sizeof of below structure is 24 instead of 22
 * due to padding.
 *
 * FUTURE Task: Find solution. Routing table lookups
 * can be done using int instead of char[4]
 */
struct Routing2
{
    unsigned char source_addr[4]; // 4 Bytes
    unsigned int destination_addr; // 4 Bytes
    unsigned int destination_mask; // 4 Bytes
    unsigned char nextRouterHop[4]; // 4 Bytes
    unsigned char outputPort[1];// 1 Bytes
    unsigned char outputPortQ[1];// 1 Bytes
    unsigned char dscp[1];// 1 Bytes
    unsigned char padding[3]; // 3 Bytes
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
    int version;
    ipv4 packetv4;
    ipv6 packetv6;
};

struct generic_packet{
    int version:4, extra:28; // 1 Byte
    unsigned char data[499]; // 499 Bytes
};

#define NETWORK_SETTINGS "./config/network.ini"
#define PACKET_SIZE 500
#define ROUTING_ENTRY_SIZE sizeof(Routing)

#define TEST_MODE 1
#define NORMAL_RUN_MODE 0

#endif // COMMONS_H
