/*
 * ISR_vectors.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "ISR_vectors.h"

struct RS232 com;

/**
 * The Interrupt service routine for the keys
 * This reads the values of the keys and then calls the registered key listener.
 */
void push_key_ISR(struct PushKeyController* pushKeyController, unsigned int id)
{
	pushKeyController->keys_changed = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);

	if(wasKeyJustPressed(pushKeyController, 0)) {
		if(db.curr_song_id < db.num_of_songs)
			syncNext(db.curr_song_id);
	} else if( wasKeyJustPressed(pushKeyController, 1)) {
		if(db.curr_song_id > 1)
			syncPrev(db.curr_song_id);
	} else if(wasKeyJustPressed(pushKeyController, 2)) {
		syncPause(db.curr_song_id);
	} else if(wasKeyJustPressed(pushKeyController, 3)) {
		syncPlay(db.curr_song_id, 100, 0);
	}

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP( KEYS_BASE, 0);
}
/***************************************************************************************
 * Audio - Interrupt Service Routine
 *
 * This interrupt service routine records or plays back audio, depending on which type
 * interrupt (read or write) is pending in the audio device.
****************************************************************************************/
void audio_ISR(alt_up_audio_dev* audio_dev, unsigned int id)
{
	if(soundMixer->indexSize <= 0) {
		disableAudioDeviceController();
		return;
	}
	alt_up_audio_write_fifo(audio_dev, soundMixer->buffer[soundMixer->currIndex], 96, ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(audio_dev, soundMixer->buffer[soundMixer->currIndex], 96, ALT_UP_AUDIO_RIGHT);

	incIndex();
	return;
}

void ps2_ISR(struct Cursor* cursor) {
	int tempcontext;
	unsigned char* bytes = (unsigned char*)malloc(sizeof(unsigned char)*3);
	int i, dx = 0, dy = 0;
	for(i = 0; i < 3; i++) {
		alt_up_ps2_read_data_byte_timeout(up_dev.ps2_dev, (bytes+i));
	}

	tempcontext= alt_irq_interruptible(PS2_0_IRQ);
	if((bytes[0] & 0x08) == 0x08) {
		if((bytes[0] & 0x01) == 1) {
			cursor->isLeftPressed = true;
			printf("mouse left is clicked\n");
		}
		if((bytes[0] & 0x02) == 0x02) {
			cursor->isRightPressed = true;
			printf("mouse right is clicked\n");
		}
		dx = bytes[1]; dy = bytes[2];
		if((bytes[0] & 0x10) == 0x10)
			dx |= 0xFFFFFF00;
		if((bytes[0] & 0x20) == 0x20)
			dy |= 0xFFFFFF00;
		moveCursor(cursor, dx, dy);
	}
	free(bytes);

	checkButtonCollision(cursor, cursor->frame);
	updateCursor(cursor);
	alt_irq_non_interruptible(tempcontext);

}
void mix_ISR(void) {
	int tempcontext;
	int i, j, isDone = 0;

	tempcontext= alt_irq_interruptible(AUDIOBUFFERPROCESS_IRQ);
	for(i = 0; i < 180; i++) {
		if(soundMixer->indexSize >=299) break;
		for(j = 0; j < db.total_songs_playing; j++) {
			if(!checkEnd(db.songs[db.curr_song_ids[j]]->sound)) {
				loadToSoundBuffer(db.songs[db.curr_song_ids[j]]->sound);
				db.curr_song_id = db.curr_song_ids[j];
				isDone = 1;
			} else if(db.total_songs_playing > 1) {
				stopSound(db.songs[db.curr_song_ids[j]]->sound);
				removeCurrPlaying(j);
				enableAudioDeviceController();
			}
		}

		if(isDone == 0)
			break;

		soundMixer->indexSize++;

		soundMixer->endIndex++;
		if(soundMixer->endIndex > 299)
			soundMixer->endIndex = 0;

		clearIndexBuffer(soundMixer->endIndex);
	}

	alt_irq_non_interruptible(tempcontext);
	if(soundMixer->indexSize <= 0 && db.total_songs_playing == 1) {
		stopSound(db.songs[db.curr_song_id]->sound);
		syncPause(db.curr_song_id);
		if(db.curr_playlist_id != 0)
			syncNext(db.curr_song_id);
		int timer = 2000000;
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 8, timer & 0xFFFF);
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 12, timer >> 16);
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 0, 0);
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 4, 0x08);
	} else {
		if(soundMixer->indexSize > 0)
			enableAudioDeviceController();
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 0, 0);
	}
}
void animate_ISR(struct Cursor* cursor) {
	int tempcontext;
	tempcontext= alt_irq_interruptible(TIMESTAMP_IRQ);

	int i, k, l;
	unsigned int data;
	int j[0xE0];
	int sorted_j[0xE0];
	int index, size_index, temp, temp1;
	memset(j, 0, sizeof(j));
	memset(sorted_j, 0, sizeof(sorted_j));
	index = soundMixer->currIndex;
	size_index = soundMixer->indexSize;
	for(k = 15; k <= 130; k++) {
		for(l = 0; l < 0xE0; l++) {
			IOWR_16DIRECT(pixel_buffer->buffer_start_address, (k*320+l+10)<<1, 0);
		}
	}
	if(db.total_songs_playing > 0) {
		for(k = 0; k < 96; k ++) {
			data = soundMixer->buffer[index][k] >> 16;
			if(data < 0x7F && data != 0) {
				j[data]+= 6;
			}
		}

		for(k = 0; k < 0x7F; k ++) {
			temp = k;
			for(i = 0; i < 0x7F; i++) {
				if(temp == 0 || j[temp] == 0) break;
				if(j[temp] < j[sorted_j[i]] || sorted_j[i] == 0) {
					temp1 = sorted_j[i];
					sorted_j[i] = temp;
					temp = temp1;
				}
			}
		}
		drawEqulizer(sorted_j, 0xE0);

	}

	alt_irq_non_interruptible(tempcontext);

	IOWR_16DIRECT(TIMESTAMP_BASE, 0, 0);
}
alt_u32 RS232_ISR(void* up_dev) {
	if(queue_lock == 1/* || SDIO_lock == 1*/) return alt_ticks_per_second()/1000;
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
				printf("fail communication %d at stats %d\n", com.failReceive, *com.stateMachine);
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
				printf("fail communication %d at stats %d\n", com.failReceive, *com.stateMachine);
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
				printf("fail communication %d at stats %d\n", com.failReceive, *com.stateMachine);
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
				printf("fail communication %d at stats %d\n", com.failReceive, *com.stateMachine);
				reset(serial_dev);
			}
		}
		return alt_ticks_per_second()/20;
	default:
		break;
	}

	return alt_ticks_per_second()/20;
}
