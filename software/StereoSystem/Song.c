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
	this->ext[0] = 'W';
	this->ext[1] = 'A';
	this->ext[2] = 'V';
	this->isCached = false;
	this->pos = this->size = 0;
	this->volume = 100;
	this->id = 0;
	this->sound = NULL;
	this->property = NULL;
	return this;
}

void loadSong(struct Song* this) {
	if(this == NULL) return;
	if(this->sound != NULL)
		return;
	while((this->sound = loadWavSound(this->song_name)) == NULL) {
		printf("load %s failed, reloading..\n", this->song_name);
	}
	addToMemory(this->sound, this->id);
	this->size = getSoundLengthMS(this->sound);
	this->isCached = true;
}

void loadStream(struct Song* this, int* property, int weight) {
	if(this == NULL || this->sound == NULL || this->sound->loading_pos >= this->sound->length) return;
	if(!loadStreamBuffer(this->sound, this->property, weight))
		alt_up_sd_card_fclose(this->property[3]);
}

void unloadSong(struct Song* this) {
	if(this == NULL || this->sound == NULL) return;
	printf("unloading song id: %d\n", this->id);
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
	//song_id_lock = 1;
	if(this == NULL) return;
	if(isCurrPlaying(this->id) >= 0|| db.total_songs_playing >= MAX_SONGS_MIX - 1) return;
	this->pos = startTime;
	this->volume = (int)volume;
	db.isPaused = false;
	if(this->sound == NULL)
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
	//song_id_lock = 0;
}

void playStream(struct Song* this, float volume, int startTime, int loops) {
	if(this == NULL) return;
	if(isCurrPlaying(this->id) >= 0|| db.total_songs_playing >= MAX_SONGS_MIX - 1) return;

	this->pos = startTime;
	this->volume = (int)volume;
	if(this->sound == NULL) {
		this->property = loadWavHeader(this->song_name);
		if(memMgr.used_memory + this->property[2] > MAX_CACHE_MEMORY) {
			freeMem(this->property[2]);
		}
		this->sound = initSound(this->property[2]);
		loadStream(this, this->property, 30);
		addToMemory(this->sound, this->id);
	}

	db.curr_song_ids[db.total_songs_playing++] = this->id;
	db.curr_song_id = this->id;
	playSound(this->sound, volume/100, startTime, loops);

}
void pauseSong(struct Song* this) {
	printf("The music %d start to pause.\n", this->id);
	int index;
	if(this == NULL) return;
	if((index = isCurrPlaying(this->id)) < 0) return;
	db.isPaused = true;
	removeCurrPlaying(index);
	pauseSound(this->sound);
	printf("The music is paused.\n");
}

void resumeSong(struct Song* this) {
	resumeSound(this->sound);
}

void stopSong(struct Song* this) {
	printf("The music start to stop.\n");
	int index;
	if(this == NULL) return;
	if((index = isCurrPlaying(this->id)) < 0) return;
	removeCurrPlaying(index);
	db.curr_song_id = 0;
	stopSound(this->sound);
	printf("The song is stoped.\n");
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
