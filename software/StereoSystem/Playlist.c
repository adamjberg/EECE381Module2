/*
 * Playlist.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Playlist.h"

struct Playlist* initPlaylist(char* listname) {
	struct Playlist* this = (struct Playlist*)malloc(sizeof(struct Playlist));
	if(this == NULL) {
		printf("playlist cannot malloc\n");
		return NULL;
	}
	strcpy(this->list_name, listname);
	//this->songs = initQueue();
	this->num_of_songs = 0;
	this->id = 0;
	//this->next = this->prev = NULL;
	return this;
}

void setListId(struct Playlist* this, int id) {
	this->id = id;
}
void setListName(struct Playlist* this, char* name) {
	if(this == NULL || name == NULL) return;
	int size = strlen(name);
	if(size >29) {
		strncpy(this->list_name, name, 29);
		this->list_name[29] = '\0';
	} else
		strcpy(this->list_name, name);

}
void killPlaylist(struct Playlist** this) {
	if(this == NULL || *this == NULL) return;
	free(*this);
	*this = NULL;
}

void generateListNameForSD(char* str, int list_index_in_db){
	char index[2];
	index[0] = list_index_in_db + '0';
	index[1] = '\0';
	strcpy(str, "LIST");
	strcat(str, index);
	strcat(str, ".TXT");
}
