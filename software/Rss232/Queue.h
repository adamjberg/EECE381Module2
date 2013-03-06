/*
 * Queue.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include "Global.h"

struct Queue {
	int size;
	struct QueueData* head;
	struct QueueData* tail;
};

struct QueueData {
	struct QueueData* next;
	struct QueueData* prev;
	void* obj;
};

struct Queue* initQueue();
void enqueue(struct Queue*, void*);
void* dequeue(struct Queue*);
void killQueueData(struct QueueData**);
#endif /* QUEUE_H_ */
