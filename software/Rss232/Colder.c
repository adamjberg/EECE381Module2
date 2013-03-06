/*
 * Colder.c
 *
 *  Created on: 2013-03-04
 *      Author: danny
 */

#include "Coder.h"

/*
 * encode the string into multiply packets and put into queue
 *
 */
void encodeString(char* str, struct Queue* q) {
	if(str == NULL || q == NULL) return;
	int i;
	int length = strlen(str);
	char* strBuf = (char*)malloc(sizeof(char)*length);
	strncpy(strBuf, str, length);
	int packet_size = length / 100;
	if(length % 100 >0) {
		packet_size+=1;
	}
	int* num_packets = (int*)malloc(sizeof(int));
	*num_packets = packet_size;
	enqueue(com.pendingPacketSize, (void*)num_packets);
	struct Packet* result;
	for(i = 0; i < packet_size; i++) {
		if(i == packet_size - 1) {
			result = initPacket(length, (unsigned char*)strBuf);
			setHeader(result, 0, 1, STRING); //last packet
		}
		else {
			char* temp = strnsep_(&strBuf, 100);
			result = initPacket(100, (unsigned char*)temp);
			free(temp);
			temp = NULL;
			length -= 100;
		}
		if(i == 0)
			setHeader(result, 1, 0, STRING); //first packet
		else if(i != packet_size - 1)
			setHeader(result, 0, 0, STRING);
		enqueue(q, (void*)result);
	}
	result = NULL;
	free(strBuf);
	strBuf = NULL;
}

void* decode(struct Queue* this) {
	int len = this->size;
	struct Packet** packets = (struct Packet**)malloc(sizeof(struct Packet*)*len);
	void* result = NULL;
	int i = 0;
	for(i = 0; i < len; i++) {
		packets[i] = (struct Packet*)dequeue(this);
	}

	switch(*(packets[0]->type)) {
	case STRING:
		result = decodeString(packets, len);
		break;
	case CMD:
		result = decodeCmd(packets, len);
		break;
	case PLAYLIST:
		result = decodePlaylist(packets, len);
		break;
	default:
		break;
	}
	for(i = 0; i < len ; i++ ) {
		killPacket(&packets[i]);
	}
	free(packets);
	packets = NULL;
	return result;
}

void* decodeString(struct Packet** p, int size) {
	int i = 0, j = 0, k = 0;
	int char_len = (size-1)*100+p[size-1]->data_size;
	char* result = (char*)malloc(sizeof(char)*char_len);
	for(i = 0; i < size; i++) {
		for(j = 0; j < p[i]->data_size; j++) {
			result[k] = p[i]->data[j+HEADERSIZE];
			k++;
		}
	} result[k] = '\0';
	return (void*)result;
}
void* decodeCmd(struct Packet** p, int size) {

	return NULL;
}
void* decodePlaylist(struct Packet** p, int size) {

	return NULL;
}
