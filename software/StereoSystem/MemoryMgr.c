/*
 * MemoryMgr.c
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */
#include "MemoryMgr.h"

struct MemoryMgr memMgr;

void initMemory() {
	memMgr.used_memory = 0;
	memset(memMgr.songs, 0, sizeof(memMgr.songs));
	memMgr.curr_index = 0;
}

void addToMemory(struct Sound* sound, int id) {
	memMgr.used_memory += sound->length;
	if(memMgr.songs[memMgr.curr_index] != 0) {
		printf("used memory: %u\n", memMgr.used_memory);
		memMgr.used_memory -= db.songs[memMgr.songs[memMgr.curr_index]]->sound->length;
		unloadSong(db.songs[memMgr.songs[memMgr.curr_index]]);
		printf("used memory: %u\n", memMgr.used_memory);
	}
	memMgr.songs[memMgr.curr_index++] = id;
	if(memMgr.curr_index >= 3)
		memMgr.curr_index = 0;
}

void freeMem(int mem) {
	printf("Free memory from buffer\n");
	int temp = memMgr.used_memory;
	printf("used memory: %u\n", memMgr.used_memory);
	int freeIndex = memMgr.curr_index;
	if(freeIndex >= 3)
		freeIndex = 0;
	while(memMgr.used_memory + mem > MAX_CACHE_MEMORY) {
		if(memMgr.songs[freeIndex] != 0) {
			memMgr.used_memory -= db.songs[memMgr.songs[freeIndex]]->sound->length;
			unloadSong(db.songs[memMgr.songs[freeIndex]]);
			memMgr.songs[freeIndex] = 0;
		}
		freeIndex++;
		if(freeIndex >= 3)
			freeIndex = 0;
	}
	if(temp == memMgr.used_memory) {
		printf("???\n");
	}
	printf("used memory: %u\n", memMgr.used_memory);
}
