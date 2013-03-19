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
	this->ext[0] = 'W';
	this->ext[1] = 'A';
	this->ext[2] = 'V';
	this->isCached = false;
	this->pos = this->size = 0;
	this->volume = 1;
	this->id = 0;
	this->sound = NULL;
	return this;
}

void loadSong(struct Song* this) {
	if(this == NULL) return;
	if(this->sound != NULL)
		unloadSong(this);
	this->sound = loadWavSound(this->song_name);
	this->size = getSoundLengthMS(this->sound);
	this->isCached = true;
}

void unloadSong(struct Song* this) {
	if(this == NULL || this->sound == NULL) return;
	unloadSound(this->sound);
	this->sound = NULL;
	this->isCached = false;
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
	song_id_lock = 1;
	if(containsValue(db.curr_song_ids, this->id) == 0) return;
	int* temp = (int*)malloc(sizeof(int));
	*temp = this->id;
	enqueue(db.curr_song_ids, temp);
	this->pos = startTime;
	this->volume = (int)volume;
	if(this->sound == NULL)
		loadSong(this);
	setFadeInLength(this->sound, 0);
	setFadeOutLength(this->sound, 0);
	playSound(this->sound, volume, startTime, loops);
	temp = NULL;
	song_id_lock = 0;
}

void pauseSong(struct Song* this) {
	int* temp = (int*)dequeueValue(db.curr_song_ids, this->id);
	if(temp == NULL) return;
	free(temp);
	temp = NULL;
	pauseSound(this->sound);
}

void resumeSong(struct Song* this) {
	int* temp = (int*)malloc(sizeof(int));
	*temp = this->id;
	enqueue(db.curr_song_ids, temp);
	resumeSound(this->sound);
	temp = NULL;
}

void stopSong(struct Song* this) {
	stopSound(this->sound);
}

/**
 * Seek a song to a given position (in MilliSeconds)
 *
 * @param this - Song to seek
 * @param position - Position to seek to in MilliSeconds
 */
void seekSong(struct Song* this, unsigned int position) {
	seekSound( this->sound, position );
}

int getSongPosition(struct Song* this) {
	return getSoundPositionMS(this->sound);
}

int getLength(struct Song* this) {
	return this->size;
}
