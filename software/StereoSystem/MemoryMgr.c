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
		memMgr.used_memory -= db.songs[memMgr.songs[memMgr.curr_index]]->sound->length;
		unloadSong(db.songs[memMgr.songs[memMgr.curr_index]]);
	}
	memMgr.songs[memMgr.curr_index++] = id;
	if(memMgr.curr_index >= 5)
		memMgr.curr_index = 0;
}

void freeMem() {
	printf("Free memory from buffer\n");
	printf("used memory: %d\n", memMgr.used_memory);
	int freeIndex = memMgr.curr_index+1;
	if(freeIndex >= 5)
		freeIndex = 0;
	if(memMgr.songs[freeIndex] != 0) {
		memMgr.used_memory -= db.songs[memMgr.songs[freeIndex]]->sound->length;
		unloadSong(db.songs[memMgr.songs[freeIndex]]);
	}
	printf("used memory: %d\n", memMgr.used_memory);
}
