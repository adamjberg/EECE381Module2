/*
 * Playlist.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef PLAYLIST_H_
#define PLAYLIST_H_
#include "Global.h"

struct Playlist {
	//struct Queue* songs;
	char list_name[30];
	int num_of_songs;
	//struct Playlist* next;
	//struct Playlist* prev;
	int id;
};

struct Playlist* initPlaylist();
void setListId(struct Playlist*, int);
void setListName(struct Playlist*, char*);
void killPlaylist(struct Playlist**);
void generateListNameForSD(char*, int);
#endif /* PLAYLIST_H_ */
