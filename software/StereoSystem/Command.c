/*
 * Command.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Command.h"

struct Command* initCmd(int index, int num_of_parameters, char** parameters) {
	struct Command* this = (struct Command*)malloc(sizeof(struct Command));
	this->cmd_index = index;
	this->num_of_parameters = num_of_parameters;
	this->parameters = (char**)malloc(sizeof(char*)*num_of_parameters);
	this->parameters_size = (int*)malloc(sizeof(int)*num_of_parameters);
	int i ;
	for( i = 0; i < num_of_parameters; i++) {
		this->parameters[i] = parameters[i];
		this->parameters_size[i] = strlen(parameters[i]);
	}
	return this;
}

void killCmd(struct Command** this) {
	free((*this)->parameters_size);
	(*this)->parameters_size = NULL;
	int i = 0;
	for(i = 0; i < (*this)->num_of_parameters ; i++) {
		free((*this)->parameters[i]);
		(*this)->parameters[i] = NULL;
	}
	free((*this)->parameters);
	(*this)->parameters = NULL;
	free(*this);
	*this = NULL;
}

//pause current play song
void pause() {

}
void play(char* song_name, int pos) {
	struct Song* aSong = querySongByName(song_name);
}
void setVolume(int vol) {

}
void seek(int pos) {

}
void next() {

}
void prev() {

}
void createPlaylist(char* listname) {

}
void modifyPlaylistName(char* listname, char* new_listname) {
	struct Playlist* pl = queryListByName(listname);
	pl->list_name = new_listname;

}
void shuffle(char* listname) {

}
void addSongToPlaylist(char* song, char* listname) {

}
void moveSongToIndex(char* song, int index, char* listname) {

}
void removeSongFromPlaylist(char* song, char* listname) {

}
void removeList(char* listname) {

}
void play_playlist(char* listname) {

}
void repeatPlaylist(char* listname) {

}
void repeatCurrentSong() {

}

void reverse() {

}
