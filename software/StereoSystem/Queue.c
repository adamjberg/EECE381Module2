/*
 * Queue.c
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#include "Queue.h"
int queue_lock = 0;

struct Queue* initQueue() {
	struct Queue* this = (struct Queue*)malloc(sizeof(struct Queue));
	this->size = 0;
	this->tail = NULL;
	this->head = NULL;
	return this;
}

int containsValue(struct Queue* this, int val) {
	if(this == NULL) return -1;
	int i;
	int size = this->size;
	struct QueueData* temp = this->head;
	for(i = 0; i < size; i++) {
		if(*((int*)(temp->obj)) == val) {
			temp = NULL;
			queue_lock = 0;
			return 0;
		}
		temp = temp->next;
	}
	temp = NULL;
	return -1;
}
void enqueue(struct Queue* this, void* obj) {
	if(this == NULL || obj == NULL || obj == this->tail->obj) {
		printf("ENQUEUE fail\n");
		return;
	}
	queue_lock = 1;
	if(this->size == 0) {
		this->head = (struct QueueData*)malloc(sizeof(struct QueueData));
		this->head->next = NULL;
		this->head->prev = NULL;
		this->head->obj = obj;
		this->tail = this->head;
	} else {
		this->tail->next = (struct QueueData*)malloc(sizeof(struct QueueData));
		this->tail->next->prev = this->tail;
		this->tail = this->tail->next;
		this->tail->obj = obj;
		this->tail->next = NULL;
	}
	this->size++;
	queue_lock = 0;
}

void* dequeue(struct Queue* this) {
	if(this == NULL || this->size <= 0) {
		printf("dequeue returns null\n");
		return NULL;
	}
	queue_lock = 1;
	if(this->size == 1)
		this->tail = NULL;
	struct QueueData* temp = this->head;
	void* result = this->head->obj;
	this->head = this->head->next;
	temp->obj = NULL;
	if(this->head != NULL)
		this->head->prev = NULL;
	temp->next = NULL;
	free(temp);
	temp = NULL;
	this->size--;
	queue_lock = 0;
	return result;
}

void* dequeueValue(struct Queue* this, int value) {
	if(this == NULL || this->size <= 0) {
		printf("dequeue returns null\n");
		return NULL;
	}
	queue_lock = 1;
	struct QueueData* temp = this->head;
	void* result;

	while(*((int*)(temp->obj)) != value) {
		if(temp != this->tail)
			temp = temp->next;
		else {
			printf("dequeueValue cannot find the value\n");
			return NULL;
		}
	}
	if(temp == this->head) {
		this->head = temp->next;
	}
	if(temp == this->tail) {
		this->tail = temp->prev;
	}
	if(temp->prev != NULL)
		temp->prev->next = temp->next;
	if(temp->next != NULL)
		temp->next->prev = temp->prev;
	temp->next = NULL;
	temp->prev = NULL;
	result = temp->obj;
	killQueueData(&temp);
	this->size--;
	queue_lock = 0;
	return result;
}

void killQueueData(struct QueueData** this) {
	if(this == NULL) return;
	(*this)->next =NULL;
	(*this)->prev = NULL;
	(*this)->obj = NULL;
	free(*this);
	*this = NULL;
}

void addPacketsLengthToQueue(int packet_size) {
	int* num_packets = (int*)malloc(sizeof(int));
	if(num_packets == NULL) {
		printf("NO MEMORY\n");
	}
	*num_packets = packet_size;
	enqueue(com.pendingPacketSize, (void*)num_packets);
	num_packets = NULL;
}
