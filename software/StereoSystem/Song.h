/*
 * Song.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef SONG_H_
#define SONG_H_
#include "Global.h"
#define SONGNAME_LENGTH 20

struct Song {
	struct Sound* sound;
	char song_name[SONGNAME_LENGTH];
	char ext[4];
	int volume;
	int pos;
	int size;
	int id;
};

struct Song* initSong(char*);
void killSong(struct Song**);
void setSongName(struct Song*, char*);
void setSongId(struct Song*, int);
void setSongVolume(struct Song*, float);
void setSongPitch(struct Song*, float, int);
void setSongPlaybackSpeed(struct Song*, int);
void playSong(struct Song*, float, int, int);
void stopSong(struct Song*);
void pauseSong(struct Song*);
void seekSong(struct Song*, int);
void unloadSong(struct Song*);
void loadSong(struct Song*);
int getSongPosition(struct Song*);
int getLength(struct Song*);
#endif /* SONG_H_ */
