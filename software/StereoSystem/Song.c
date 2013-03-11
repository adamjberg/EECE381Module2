/*
 * Song.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "Song.h"

struct Song* initSong(char* songname) {
	struct Song* this = (struct Song*)malloc(sizeof(struct Song));
	strcpy(this->song_name, songname);
	this->isCached = false;
	this->pos = this->size = this->volume = 0;
	this->id = 0;
	return this;
}

void killSong(struct Song** this) {

}

void setSongId(struct Song* this, int id) {
	this->id = id;
}
