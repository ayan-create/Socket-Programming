#ifndef PACKET_H
#define PACKET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>
#include<iostream>
using namespace std;
#define MAX_DATA_SIZE 24

struct Packet {
    int seq_num;
    int length;
    char data[MAX_DATA_SIZE];
};

struct ACK {
    int ack_num;
};

#endif
