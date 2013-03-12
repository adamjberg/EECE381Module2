/*
 * Song.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "Song.h"

struct Song* initSong(char* songname) {
	struct Song* this = (struct Song*)malloc(sizeof(struct Song));
	setSongName(this, songname);
	this->isCached = false;
	this->pos = this->size = this->volume = 0;
	this->id = 0;
	return this;
}

void killSong(struct Song** this) {

}
void setSongName(struct Song* this, char* name) {
	if(this == NULL || name == NULL) return;
	int size = strlen(name);
	if(size > SONGNAME_LENGTH-1) {
		strncpy(this->song_name, name, SONGNAME_LENGTH-1);
		this->song_name[SONGNAME_LENGTH-1] = '\0';
	} else
		strcpy(this->song_name, name);

}
void setSongId(struct Song* this, int id) {
	this->id = id;
}
