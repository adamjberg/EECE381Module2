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
	struct Queue* songs;
	char* list_name;
	int num_of_songs;
	struct Playlist* next;
	struct Playlist* prev;
};

<<<<<<< HEAD
void initList();
void killList(struct Playlist**);
=======
struct Playlist* initPlaylist();
>>>>>>> f9685626a469454e92a071d08aa12b49bf2f5b16
#endif /* PLAYLIST_H_ */
