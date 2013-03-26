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

	addPacketsLengthToQueue(packet_size);

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

/*
 * encode a command to packets and push to the queue
 */
void encodeCmd(struct Command* cmd, struct Queue* q) {
	if(cmd == NULL || q == NULL) return;
	int data_size = 2; //1 byte for cmd index and the other for number of parameters
	data_size += cmd->num_of_parameters; //each parameter need a byte for its length
	int i, j, k;
	for(i = 0; i < cmd->num_of_parameters; i++) {
		data_size += cmd->parameters_size[i];
	}
	char* dataBuf = (char*)malloc(sizeof(char)*data_size);
	struct Packet* result = NULL;
	int packets_size = data_size/100;
	if(data_size %100 > 0)
		packets_size+=1;

	addPacketsLengthToQueue(packets_size);

	dataBuf[0] = (unsigned char)cmd->cmd_index;
	dataBuf[1] = (unsigned char)cmd->num_of_parameters;
	j = 2;
	for(i = 0; i< cmd->num_of_parameters; i++) {
		dataBuf[j++] = (char)cmd->parameters_size[i];
		for(k = 0; k < cmd->parameters_size[i]; k++) {
			dataBuf[j++] = (char)cmd->parameters[i][k];
		}
	}
	for(i = 0; i < packets_size; i++) {
		if(i == packets_size-1) {
			result = initPacket(data_size, (unsigned char*)dataBuf);
			setHeader(result, 0, 1, CMD); //last packet
		} else {
			char* temp = strnsep_(&dataBuf, 100);
			result = initPacket(100, (unsigned char*)dataBuf);
			free(temp);
			temp = NULL;
			data_size -= 100;
		}
		if(i == 0)
			setHeader(result, 1, 0, CMD); //first packet
		else if(i != packets_size - 1)
			setHeader(result, 0, 0, CMD);
		enqueue(q, (void*)result);
	}
	result = NULL;
	free(dataBuf);
	dataBuf = NULL;
}

void decode(struct Queue* this) {
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
		printf("%s\n",(char*)result);
		free(result);
		result = NULL;
		break;
	case CMD:
		result = decodeCmd(packets, len);
		addCmd(com.scheduler, result);
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
	int length = 0;
	int cmd_index = (int)p[0]->data[2];;
	int num_parameters = (int)p[0]->data[3];;
	char** paras = (char**)malloc(sizeof(char*)*num_parameters);
	int i, j, k;
	for(i = 0; i < size; i++) {
		length += p[i]->data_size;
	}
	char* dataBuf = (char*)malloc(sizeof(char)*length);
	i = 0;
	for(j = 0; j < size; j++) {
		for(k = 0; k < p[j]->data_size; k++) {
			dataBuf[i++] = p[j]->data[k+HEADERSIZE];
		}
	}
	j = 2;
	int len = 0;
	for(i = 0; i < num_parameters; i++) {
		len = (int)dataBuf[j++];
		paras[i] = (char*)malloc(sizeof(char)*(len+1));
		for(k = 0; k < len; k++) {
			paras[i][k] = dataBuf[j++];
		} paras[i][k] = '\0';
	}
	void* result = (void*)initCmd(cmd_index, num_parameters, paras);
	for(i = 0; i < num_parameters; i++) {
		free(paras[i]);
		paras[i] = NULL;
	}
	if(num_parameters != 0) {
		free(paras);
		paras = NULL;
	}
	free(dataBuf);
	dataBuf = NULL;
	return result;
}
void* decodePlaylist(struct Packet** p, int size) {

	return NULL;
}

