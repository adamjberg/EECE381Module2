/*
 * Command.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Command.h"

struct Command* initCmd(int index, int num_parameters, char** paras) {
	struct Command* this = (struct Command*)malloc(sizeof(struct Command));
	this->cmd_index = index;
	this->num_of_parameters = num_parameters;
	this->parameters = (char**)malloc(sizeof(char*)*num_parameters);
	this->parameters_size = (int*)malloc(sizeof(int)*num_parameters);
	int i ;
	for( i = 0; i < num_parameters; i++) {
		this->parameters_size[i] = strlen(paras[i]);
		this->parameters[i] = (char*)malloc(sizeof(char)*(this->parameters_size[i]+1));
		strncpy(this->parameters[i], paras[i], this->parameters_size[i]);
		this->parameters[i][this->parameters_size[i]] = '\0';
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
	printf("The music is paused.\n");
}
void play(char* song_name, int pos) {
	//struct Song* aSong = querySongByName(song_name);
	printf("A song is played.\n");
}
void setVolume(int vol) {
	printf("Volume is set to %d\n", vol);
}
void seek(int pos) {
	printf("Seek is set to %d\n", pos);
}
void next() {
	printf("Next song is selected and played.\n");
}
void prev() {
	printf("Previous song is selected and played.\n");
}
void createPlaylist(char* listname) {
	printf("A playlist %s is created.\n", listname);
}
void modifyPlaylistName(char* listname, char* new_listname) {
	struct Playlist* pl = queryListByName(listname);
	pl->list_name = new_listname;

}
void shuffle(char* listname) {
	printf("Playlist: %s is shuffled\n", listname);
}
void addSongToPlaylist(char* song, char* listname) {
	printf("Song %s is added to %s\n", song, listname);
}
void moveSongToIndex(char* song, int index, char* listname) {

}
void removeSongFromPlaylist(char* song, char* listname) {

}
void removeList(char* listname) {
	printf("Playlist: %s is removed\n", listname);
}
void play_playlist(char* listname) {
	printf("Playlist: %s is selected and played\n", listname);
}
void repeatPlaylist(char* listname) {
	printf("Playlist: %s is set to repeated\n", listname);
}
void repeatCurrentSong() {

}

void reverse() {

}
