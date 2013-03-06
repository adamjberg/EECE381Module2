/*
 * Packet.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef PACKET_H_
#define PACKET_H_
#include "Global.h"
struct Packet {
	unsigned char* data;
	int data_size;
	enum msgType* type;
	int isHead;
	int isEnd;
};

struct Packet* initPacket();
void killPacket(struct Packet**);
void setHeader(struct Packet*, int, int, enum msgType);
void setBit(unsigned char*, int);
void setAck(struct Packet*, int);
int getBit(unsigned char, int);
char* strnsep_(char**, int);
struct Packet* readPacketHeader(unsigned char, unsigned char);
#endif /* PACKET_H_ */
