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
int containsValue(struct Queue*, int);
void enqueue(struct Queue*, void*);
void* dequeue(struct Queue*);
void* dequeueValue(struct Queue*, int);
void killQueueData(struct QueueData**);
void addPacketsLengthToQueue(int);
#endif /* QUEUE_H_ */
