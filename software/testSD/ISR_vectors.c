/*
 * ISR_vectors.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "ISR_vectors.h"

struct alt_up_dev up_dev;

/***************************************************************************************
 * Audio - Interrupt Service Routine
 *
 * This interrupt service routine records or plays back audio, depending on which type
 * interrupt (read or write) is pending in the audio device.
****************************************************************************************/
void audio_ISR(alt_up_audio_dev* audio_dev, unsigned int id)
{
	if (alt_up_audio_write_interrupt_pending(audio_dev))	// check for write interrupt
	{
		//int numToWrite = 0;
		//int spaceAvailable = alt_up_audio_write_fifo_space(audio_dev, ALT_UP_AUDIO_LEFT);
		int temp = 0, temp1 = 0;
		if(soundMixer->indexSize <= 0) return;
		temp = alt_up_audio_write_fifo(audio_dev, soundMixer->buffer[soundMixer->currIndex], 96, ALT_UP_AUDIO_LEFT);
		temp1 = alt_up_audio_write_fifo(audio_dev, soundMixer->buffer[soundMixer->currIndex], 96, ALT_UP_AUDIO_RIGHT);
		/*s->position += 96;
		if(s->position>=s->length)
			s->position = 0;*/
		incIndex();
	}
	return;
}
/*
alt_u32 RS232_ISR(void* up_dev) {
	if(queue_lock == 1 || SDIO_lock == 1) return alt_ticks_per_second()/1000;
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
					//printf("num of packets will be receiving: %d\n", com.num_packets);
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
}*/
