/*
 * Packet.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "Packet.h"

struct Packet* initPacket(int data_size, unsigned char* data) {
	struct Packet* this = (struct Packet*)malloc(sizeof(struct Packet));
	this->data_size = data_size;
	this->data = (unsigned char*)malloc(sizeof(unsigned char)*(data_size+HEADERSIZE));
	this->data[0] = 0;
	this->data[1] = (unsigned char)(data_size + HEADERSIZE);
	this->type = (enum msgType*)malloc(sizeof(enum msgType));
	int i;
	for(i = 0; i < data_size; i++) {
		this->data[i+HEADERSIZE] = data[i];
	}
	return this;
}

void killPacket(struct Packet** this) {
	if(this == NULL) {
		printf("packet is null when attempt to removed\n");
		return;
	}
	free((*this)->data);
	(*this)->data = NULL;
	free((*this)->type);
	(*this)->type = NULL;
	free(*this);
	*this = NULL;
}

void setHeader(struct Packet* this, int isHead, int isEnd, enum msgType type) {
	if(this == NULL) return;
	if((this->isHead = isHead) == 1) {
		setBit(&this->data[0], 6);
	}
	if((this->isEnd = isEnd) == 1) {
		setBit(&this->data[0], 5);
	}
	*(this->type) = type;
	this->data[0] = this->data[0] & 0xF0;
	switch(type) {
	case STRING:
		break;
	case CMD:
		this->data[0] = this->data[0] | 0x01;
		break;
	case PLAYLIST:
		this->data[0] = this->data[0] | 0x02;
		break;
	case AUDIO:
		this->data[0] = this->data[0] | 0x03;
		break;
	default:
		break;
	}
}

/*
 * Read the header of receivng packets and return new Packet with header information
 */
struct Packet* readPacketHeader(unsigned char byte1, unsigned char byte2) {
	struct Packet* result = (struct Packet*)malloc(sizeof(struct Packet));
	if(result == NULL) {
		printf("packet is null at malloc with header\n");
	}
	com.client_ack = getBit(byte1, 7);
	result->data_size = (int)byte2 - HEADERSIZE;
	result->data = (unsigned char*)malloc(sizeof(unsigned char)*((int)byte2+HEADERSIZE));
	if(result->data == NULL) {
		printf("data inside packet is null when malloc with header\n");
	}
	result->data[0] = byte1;
	result->data[1] = byte2;
	result->isHead = getBit(byte1, 6);
	result->isEnd = getBit(byte1, 5);
	result->type = (enum msgType*)malloc(sizeof(enum msgType));
	switch((int)(byte1 & 0x0F)) {
	case 0:
		*result->type = STRING;
		break;
	case 1:
		*result->type = CMD;
		break;
	case 2:
		*result->type = PLAYLIST;
		break;
	case 3:
		*result->type = AUDIO;
		break;
	default:
		break;
	}
	return result;
}

int getBit(unsigned char byte, int pos) {
	return ((byte & (1 << pos)) == 0) ? 0 : 1;
}
void setBit(unsigned char* byte, int pos) {
	*byte = (*byte | (1 << pos));
}

void setAck(struct Packet* this, int ack) {
	if(ack == 1) {
		setBit(&this->data[0], 7);
	}
}
/*
 * return a sub string from position 0 to pos and leave old string from pos to end
 * @parm pos index of string to cut
 */
char* strnsep_(char** str, int pos) {
	char* result = (char*)malloc(sizeof(char)*pos);
	int i;
	int len = strlen(*str);
	char temp[len-pos];
	for(i = 0; i < len; i++) {
		if(i < pos)
			result[i] = (*str)[i];
		else
			temp[i-pos] = (*str)[i];
	}
	*str = (char*)realloc(*str, len-pos);
	strncpy(*str, temp, len-pos);
	return result;
}
