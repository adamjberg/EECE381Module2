/*
 * ISR_vectors.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "ISR_vectors.h"

struct RS232 com;

alt_u32 RS232_ISR(void* up_dev) {
	if(queue_lock == 1) return alt_ticks_per_second()/1000;
	alt_up_rs232_dev *serial_dev = ((struct alt_up_dev*)up_dev)->RS232_dev;
	unsigned char* cert;
	int i = 0;
	int len = 0;
	switch(*(com.stateMachine)) {
	case startInit:
		cert = sendStartInit();
		sendRS232(serial_dev, cert, 3);
		free(cert);
		cert = NULL;
		com.failReceive = 0;
		return alt_ticks_per_second()/20;
	case waitStart:
		i = 0;
		if((len = alt_up_rs232_get_used_space_in_read_FIFO(serial_dev)) >= 3) {
			do {
				while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) == 0);
				alt_up_rs232_read_data(serial_dev, &com.data[i], &com.parity);
				i++;
			} while(i < 3);
			if(checkStartAck(com.data[0]) == 1) {
				setStates(sendStates);
			} else
				setStates(startInit);
		} else {
			com.failReceive++;
			if(com.failReceive > 10)
				setStates(startInit);
		}
		return alt_ticks_per_second()/20;
	case checkClient:
		i = 0;
		if((len = alt_up_rs232_get_used_space_in_read_FIFO(serial_dev)) >= 3) {
			do {
				while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) == 0);
				alt_up_rs232_read_data(serial_dev, &com.data[i], &com.parity);
				i++;
			} while(i < 3);
			if(checkAck(com.data) == 1) {
				if(getBit(com.data[0], 1) == 1) {
					setStates(sendAck0);
					com.num_packets = ((int)(com.data[1] << 8))+(int)com.data[2];
					printf("num of packets will be receiving: %d\n", com.num_packets);
				} else
					setStates(sendStates);
			} else
				setStates(sendStates);
		} else {
			com.failReceive++;
			if(com.failReceive > 100) {
				reset(serial_dev);
			}
		}
		return alt_ticks_per_second()/20;
	case sendStates:
		cert = sendStats();
		sendRS232(serial_dev, cert, 3);
		free(cert);
		cert = NULL;
		com.failReceive = 0;
		return alt_ticks_per_second()/20;
	case sendAck0:
		cert = sendAck();
		sendRS232(serial_dev, cert, 3);
		if(cert[1] != 0)
			printf("sth is wrong");
		free(cert);
		cert = NULL;
		com.failReceive = 0;
		return alt_ticks_per_second()/20;
	case receiveData0:
		if((len = alt_up_rs232_get_used_space_in_read_FIFO(serial_dev)) > 2) {
			//read first 2 bytes for header information
			alt_up_rs232_read_data(serial_dev, &com.data[0], &com.parity);
			alt_up_rs232_read_data(serial_dev, &com.data[1], &com.parity);
			struct Packet* p = readPacketHeader(com.data[0], com.data[1]);
			i = 0;
			do {
				while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) == 0);
				alt_up_rs232_read_data(serial_dev, &(p->data[i+HEADERSIZE]), &com.parity);
			//	printf("%c", p->data[i+HEADERSIZE]);
				i++;
			} while(p->data_size > i );
			//printf("\n");
			enqueue(com.receivePackets, (void*)p);
			com.index_packets++;
			setStates(sendAck0);
		} else {
			com.failReceive++;
			if(com.failReceive > 100) {
				reset(serial_dev);
			}
		}
		return alt_ticks_per_second()/20;
	case waitClient:
		i = 0;
		if((len = alt_up_rs232_get_used_space_in_read_FIFO(serial_dev)) >= 3) {
			do {
				while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) == 0);
				alt_up_rs232_read_data(serial_dev, &com.data[i], &com.parity);
				i++;
			} while(i < 3);
			if(checkAck(com.data) == 1) {
				setStates(sendData0);
			} else {
				com.failReceive++;
				if(com.failReceive > 100)
					setStates(sendStates);
			}
		} else {
			com.failReceive++;
			if(com.failReceive > 100) {
				reset(serial_dev);
			}
		}
		return alt_ticks_per_second()/20;
	case sendData0:
		if(com.packetBuf == NULL) {
			if((com.packetBuf = (struct Packet*)dequeue(com.sendPackets))==NULL) {
				printf("Packet sending fail, queue is empty");
				setStates(sendStates);
				return alt_ticks_per_second()/50;
			}
		} setAck(com.packetBuf, com.host_ack);

		for(i = 0; i< com.packetBuf->data_size+HEADERSIZE; i++) {
			alt_up_rs232_write_data(serial_dev, com.packetBuf->data[i]);
		}
		setStates(waitAck0);
		com.failReceive = 0;
		return alt_ticks_per_second()/20;
	case waitAck0:
		i = 0;
		if((len = alt_up_rs232_get_used_space_in_read_FIFO(serial_dev)) >= 3) {
			do {
				while (alt_up_rs232_get_used_space_in_read_FIFO(serial_dev) == 0);
				alt_up_rs232_read_data(serial_dev, &com.data[i], &com.parity);
				i++;
			} while(i < 3);
			if(checkAck(com.data) == 1) {
				com.index_send_packets++;
				killPacket(&(com.packetBuf));
				if(com.index_send_packets < com.num_send_packets)
					setStates(sendData0);
				else {
					com.index_send_packets = com.num_send_packets = 0;
					setStates(sendStates);
					com.isRdySend = 0;
				}
			} else {
				setStates(sendData0);
			}
		} else {
			com.failReceive++;
			if(com.failReceive > 100) {
				reset(serial_dev);
			}
		}
		return alt_ticks_per_second()/20;
	default:
		break;
	}

	return alt_ticks_per_second()/20;
}
