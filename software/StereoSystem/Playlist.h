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
};

struct Playlist* initPlaylist();
#endif /* PLAYLIST_H_ */
