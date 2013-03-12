/*
 * Playlist.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef PLAYLIST_H_
#define PLAYLIST_H_
#include "Global.h"
#define LISTNAME_LENGTH 101
struct Playlist {
	//struct Queue* songs;
	char list_name[LISTNAME_LENGTH];
	int num_of_songs;
	//struct Playlist* next;
	//struct Playlist* prev;
	int id;
};

struct Playlist* initPlaylist(char*);
void setListId(struct Playlist*, int);
void setListName(struct Playlist*, char*);
void killPlaylist(struct Playlist**);
void generateListNameForSD(char*, int);
#endif /* PLAYLIST_H_ */
