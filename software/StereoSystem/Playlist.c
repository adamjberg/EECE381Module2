/*
 * Playlist.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Playlist.h"

struct Playlist* initPlaylist() {
	struct Playlist* this = (struct Playlist*)malloc(sizeof(struct Playlist));
	this->songs = initQueue();
	this->list_name = NULL;
	this->num_of_songs = 0;
	this->next = this->prev = NULL;
	return this;
}

void killPlaylist(struct Playlist** this) {
	printf("killPlaylist function is called.\n");

	*this = NULL;
}
