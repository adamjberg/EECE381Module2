/*
 * RS232.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "RS232.h"

alt_alarm alarm;
struct alt_up_dev up_dev;

/*
 * Initialization of Serail communication
 */
struct RS232 initRS232() {
	struct RS232 com_local;
	com_local.receivePackets = initQueue();
	com_local.sendPackets = initQueue();
	com_local.pendingPacketSize = initQueue();
	com_local.client_ack = 0;
	com_local.host_ack = 0;
	com_local.isRdySend = 0;
	com_local.failReceive = 0;
	com_local.stateMachine = (enum States*)malloc(sizeof(enum States));
	com_local.pastState = (enum States*)malloc(sizeof(enum States));
	*com_local.stateMachine = startInit;
	*com_local.pastState = startInit;
	com_local.num_packets = com_local.index_packets = 0;
	com_local.num_send_packets = com_local.index_send_packets = 0;
	com_local.packetBuf = NULL;

	printf("UART Initialization\n");
	alt_up_rs232_dev* uart = alt_up_rs232_open_dev(RS232_0_NAME);
	up_dev.RS232_dev = uart;

	printf("Clearing read buffer to start\n");
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart)) {
		alt_up_rs232_read_data(uart, &com.data[0], &com.parity);
	}
	alt_alarm_start(&alarm, alt_ticks_per_second(), RS232_ISR, (void*)&up_dev);

	printf("UART Initialization finished\n");
	return com_local;
}

/*
 * Send data to the other end
 * @parm data data to send; must be void*
 * @parm type type of data to send
 */
int send(void* data, enum msgType type) {
	if(data == NULL) return -1;
	switch(type) {
	case STRING:
		encodeString((char*)data, com.sendPackets);
		break;
	case CMD:
		break;
	case PLAYLIST:
		break;
	case AUDIO:
		break;
	default:
		return -1;
	}
	com.isRdySend = 1;
	return 0;
}

unsigned char* sendStartInit() {
	unsigned char* cert = (unsigned char*)malloc(sizeof(unsigned char)*3);
	cert[0] = 0xA3; //set the format as 1010+(client_ack)(host_ack)+11
	cert[1] = 0xAA; //10101010
	cert[2] = 0xAA; //10101010

	updateAcknowledge(&cert[0]);
	setStates(waitStart);
	return cert;

}

int checkStartAck(unsigned char byte) {
	if(((int)(byte & 0xF3) == 0xA3)&& getBit(byte, 2) == com.host_ack) {
		com.client_ack = getBit(byte, 3);
		switchAck();
		return 1;
	} return 0;
}

void updateAcknowledge(unsigned char* byte) {
	if(com.host_ack == 0)
		*byte &= 0xFB; //XXXXX0XX
	else
		setBit(byte, 2); //XXXXX1XX
	if(com.client_ack == 0)
		*byte &= 0xF7; //XXXX0XXX
	else
		setBit(byte, 3); //XXXX1XXX
}
/*
 * send 3 bytes acknowledge to client at sendStats stage;
 * 1 byte: client_ack + host_ack + receive/send; 2bytes: length of packets if any
 * first byte format: 1111+(client_ack)(host_ack)(send)(receive)
 */
unsigned char* sendStats() {
	unsigned char* cert = (unsigned char*)malloc(sizeof(unsigned char)*3);
	cert[0] = 0xF0; //set the format as 1111+(client_ack)(host_ack)(send)(receive)
	cert[1] = 0;
	cert[2] = 0;
	int* num_s_packets;

	updateAcknowledge(&cert[0]);

	if(com.isRdySend == 1 || com.pendingPacketSize->size > 0) {
		setStates(waitClient);
		setBit(&cert[0], 1); //1111XX10
		com.index_send_packets = 0;
		num_s_packets = (int*)dequeue(com.pendingPacketSize);
		com.num_send_packets = *num_s_packets;
		cert[1] = (unsigned char)(com.num_send_packets/256);
		cert[2] = (unsigned char)(com.num_send_packets & 0x00FF);
		free(num_s_packets);
		num_s_packets = NULL;
	} else {
		setStates(checkClient);
		setBit(&cert[0], 0); //1111XX01
	}
	return cert;
}

unsigned char* sendAck() {
	unsigned char* cert = (unsigned char*)malloc(sizeof(unsigned char)*3);
	if(cert == NULL) {
		printf("NO MEMORY to send ack!!\n");
	}
	cert[0] = 0xF0; //set the format as 1111+(client_ack)(host_ack)(send)(receive)
	cert[1] = 0;
	cert[2] = 0;

	updateAcknowledge(&cert[0]);

	setBit(&cert[0], 0); //1111XX01

	char* temp; //need to do something with this later

	if(com.index_packets >= com.num_packets) { //all packets received
		setStates(checkClient);
		com.index_packets = 0;

		//call decode & command here later
		temp = (char*)decode(com.receivePackets);
		printf("%s\n", temp);
		free(temp);
		temp = NULL;
	} else
		setStates(receiveData0);
	return cert;
}
void switchAck() {
	com.host_ack = (com.host_ack == 0) ? 1 : 0;
}
/*
 * check if the acknowledge byte is valid
 */
int checkAck(unsigned char* bytes) {
	if(((bytes[0] & 0xF0) == 0xF0) & (getBit(bytes[0], 2) == com.host_ack)) {
		com.client_ack = getBit(bytes[0], 3);
		switchAck(); //very important to switch ack for next ack
		com.failReceive = 0;
		return 1;
	} com.failReceive++;
	return 0;
}

/*
* increment the frequency of failing receive data;
* go back to past state if it has already fail receiving 100 times
*/
void failReceive(enum States pastState) {
	com.failReceive++;
	if(com.failReceive > 100) {
		setStates(pastState);
		com.failReceive=0;
	}
}

void setStates(enum States s) {
	*(com.pastState) = *(com.stateMachine);
	*(com.stateMachine) = s;
	//printf("past state: %d\n", *(com.pastState));
	//printf("current state: %d\n", *(com.stateMachine));
}
/*
 * Reset the RS232 communication to initial state
 */
void reset(alt_up_rs232_dev *serial_dev) {
	printf("Starting reseting connection\n");
	com.client_ack = com.host_ack = 0;
	com.failReceive = com.isRdySend = 0;
	com.num_packets = com.num_send_packets = 0;
	com.index_packets = com.index_send_packets = 0;

	struct Packet* p = NULL;
	int* len;
	while(com.sendPackets->size > 0) {
		p = (struct Packet*)dequeue(com.sendPackets);
		killPacket(&p);
	}

	while(com.receivePackets->size > 0) {
		p = (struct Packet*)dequeue(com.receivePackets);
		killPacket(&p);
	}

	while(com.pendingPacketSize->size > 0) {
		len = (int*)dequeue(com.pendingPacketSize);
		free(len);
		len = NULL;
	}
	if(&com.packetBuf != NULL) {
		free(com.packetBuf);
		com.packetBuf = NULL;
	}
	while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) != 0);
	alt_up_rs232_read_data(serial_dev, &com.data[0], &com.parity);
	setStates(startInit);
	printf("WARNNING!! the connection has been reseted.\n");
}
/*
 * Send sequence of bytes to serial port
 */
void sendRS232(alt_up_rs232_dev *serial_dev, unsigned char* bytes, int size) {
	int i;
	for(i = 0; i < size; i++) {
		alt_up_rs232_write_data(serial_dev, bytes[i]);
	}
}
