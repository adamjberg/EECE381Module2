/*
 * Song.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "Song.h"

#define LENGTH_OF_FADE_PERCENT 0.3
#define MAX_FADE_LENGTH 50000

struct Song* initSong(char* songname) {
	struct Song* this = (struct Song*)malloc(sizeof(struct Song));
	setSongName(this, songname);
	int len = strlen(songname);
	int i;
	for(i = 1; i <= 3; i ++) {
		this->ext[3-i] = songname[len-i];
	} this->ext[3] = '\0';
	this->pos = this->size = 0;
	this->volume = 100;
	this->id = 0;
	this->sound = NULL;
	return this;
}

void loadSong(struct Song* this) {
	if(this == NULL) return;
	if(this->sound != NULL)
		return;
	while((this->sound = loadSound(this)) == NULL) {
		printf("load %s failed, reloading..\n", this->song_name);
	}
	addToMemory(this->sound, this->id);
	this->size = getSoundLengthMS(this->sound);
}

void unloadSong(struct Song* this) {
	if(this == NULL || this->sound == NULL) return;
	printf("unloading song id: %d\n", this->id);
	if(this->sound->audioFormat != NULL) {
		free(this->sound->audioFormat);
		this->sound->audioFormat = NULL;
	}
	if(this->sound->buffer != NULL) {
		free(this->sound->buffer);
		this->sound->buffer = NULL;
	}
	free(this->sound);
	this->sound = NULL;
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

void setSongPitch(struct Song* this, float pitch, int quality) {
	setSoundPitch(this->sound, (pitch * 0.01), quality);
}

void setSongPlaybackSpeed(struct Song* this, int speed) {
	setSoundPlaybackSpeed(this->sound, (speed * 0.01));
}

void playSong(struct Song* this, float volume, int startTime, int loops) {
	if(this == NULL) return;
	if(isCurrPlaying(this->id) >= 0|| db.total_songs_playing >= MAX_SONGS_MIX - 1) return;
	this->pos = startTime;
	this->volume = (int)volume;
	db.curr_song_id = 0;
	loadSong(this);
	int fadeLength = (int) this->sound->length * LENGTH_OF_FADE_PERCENT;
	if(fadeLength > MAX_FADE_LENGTH) {
		fadeLength = MAX_FADE_LENGTH;
	}
	setFadeInLength(this->sound, fadeLength);
	setFadeOutLength(this->sound, fadeLength);
	this->sound->fadeVolume = volume/100.0;
	db.curr_song_ids[db.total_songs_playing++] = this->id;
	db.curr_song_id = this->id;
	playSound(this->sound, volume/100, startTime, loops);
}

void pauseSong(struct Song* this) {
	printf("The music %d start to pause.\n", this->id);
	int index;
	if(this == NULL) return;
	if((index = isCurrPlaying(this->id)) < 0) return;
	removeCurrPlaying(index);
	printf("The music is paused.\n");
}

void stopSong(struct Song* this) {
	printf("The music start to stop.\n");
	int index;
	if(this == NULL) return;
	if((index = isCurrPlaying(this->id)) < 0) return;
	stopSound(db.songs[db.curr_song_ids[index]]->sound);
	removeCurrPlaying(index);
	db.curr_song_id = 0;
	this->pos = 0;
	printf("The song is stoped.\n");
}

/**
 * Seek a song to a given position (in MilliSeconds)
 *
 * @param this - Song to seek
 * @param position - Position to seek to in MilliSeconds
 */
void seekSong(struct Song* this, int position) {
	seekSound( this->sound, position );
}

int getSongPosition(struct Song* this) {
	return getSoundPositionMS(this->sound);
}

int getLength(struct Song* this) {
	return this->size;
}
