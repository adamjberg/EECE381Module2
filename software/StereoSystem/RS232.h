/*
 * RS232.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef RS232_H_
#define RS232_H_

#include "Global.h"
enum msgType;
struct RS232 {
	unsigned char data[50];
	unsigned char parity;
	struct Queue* sendPackets;
	struct Queue* receivePackets;
	struct Queue* pendingPacketSize;
	struct Packet* packetBuf;
	enum States* stateMachine;
	enum States* pastState;
	int client_ack, host_ack, isRdySend, failReceive;
	int num_packets, index_packets;
	int num_send_packets, index_send_packets;
};

enum States {
	checkClient = 0, sendStates = 1, waitClient = 2, sendData0 = 3, waitAck0 = 4, sendData1 = 5,
	waitAck1 = 6, sendAck0 = 7, receiveData0 = 8, startInit = 9, waitStart = 10
};
struct RS232 initRS232();
int send(void*, enum msgType);
void sendRS232(alt_up_rs232_dev*, unsigned char*, int);
unsigned char* sendStats();
unsigned char* sendAck();
void setStates(enum States);
void failReceive(enum States);
int checkAck(unsigned char*);
void switchAck();
void reset(alt_up_rs232_dev *);
void updateAcknowledge(unsigned char*);
unsigned char* sendStartInit();
int checkStartAck(unsigned char);
#endif /* RS232_H_ */
