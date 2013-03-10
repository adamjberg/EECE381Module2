/*
 * Song.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef SONG_H_
#define SONG_H_
#include "Global.h"

struct Song {
	char* song_name;
	int volume;
	int pos;
	int size;
	bool isCached;
	int id;
	//struct Song* next;
	//struct Song* prev;
};

struct Song* initSong();
void killSong(struct Song**);
void setSongId(struct Song*, int);
#endif /* SONG_H_ */
