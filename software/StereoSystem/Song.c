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
	this->pos = this->size = 0;
	this->volume = 1;
	this->id = 0;
	this->sound = NULL;
	return this;
}

void loadSong(struct Song* this) {
	this->sound = loadWavSound(this->song_name);
}

void unloadSong(struct Song* this) {
	unloadSound(this->sound);
	this->sound = NULL;
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

void setSongVolume(struct Song* this, float volume) {
	setSoundVolume(this->sound, volume);
}

void playSong(struct Song* this, float volume, int startTime, int loops) {
	playSound(this->sound, volume, startTime, loops);
}

void pauseSong(struct Song* this) {
	pauseSound(this->sound);
}

void resumeSong(struct Song* this) {
	resumeSound(this->sound);
}

void stopSong(struct Song* this) {
	stopSound(this->sound);
}
