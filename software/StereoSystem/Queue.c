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

void killQueueData(struct QueueData** this) {
	if(this == NULL) return;
	(*this)->next =NULL;
	(*this)->prev = NULL;
	(*this)->obj = NULL;
	free(*this);
	*this = NULL;
}
