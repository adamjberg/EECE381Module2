/*
 * MemoryMgr.h
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */

#ifndef MEMORYMGR_H_
#define MEMORYMGR_H_
#include "Global.h"

struct MemoryMgr {
	int used_memory;
	int songs[5];
	int curr_index;
};

void initMemory();
void addToMemory(struct Sound*, int);
void freeMem();
#endif /* MEMORYMGR_H_ */
