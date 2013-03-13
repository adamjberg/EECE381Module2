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
	if(num_parameters == 0 || paras == NULL) return  this;
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
	int i = 0;
	if((*this)->num_of_parameters != 0) {
		free((*this)->parameters_size);
		(*this)->parameters_size = NULL;
		for(i = 0; i < (*this)->num_of_parameters ; i++) {
			free((*this)->parameters[i]);
			(*this)->parameters[i] = NULL;
		}
		free((*this)->parameters);
		(*this)->parameters = NULL;
	}
	free(*this);
	*this = NULL;
}
/*
 * Function to call when need to sync with Android
 * It will also put command to scheduler
 */
void syncPlay(int id, int pos) {
	char* temp[2];
	char tempId[4];
	sprintf(tempId, "%d", id);
	temp[0] = tempId;
	char tempPos[4];
	sprintf(tempPos, "%d", pos);
	temp[1] = tempPos;
	struct Command* cmd = initCmd(1, 2, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
}
//index 1
void play(int id, int pos) {
	//struct Song* aSong = querySongByName(song_name);
	printf("A song %d is played at %d position.\n", id, pos);
}
/*
 * Function to call when need to sync with Android
 * It will also put command to scheduler
 */
void syncPause() {
	struct Command* cmd = initCmd(2, 0, NULL);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
}
//pause current play song; index 2
void pause() {
	printf("The music is paused.\n");
}
/*
 * Function to call when need to sync with Android
 * It will also put command to scheduler
 */
void syncStop() {
	struct Command* cmd = initCmd(3, 0, NULL);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
}
//index 3
void stop() {
	printf("The song is stoped.\n");
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
	struct Playlist* pl = initPlaylist(listname);
	addListToDB(pl);
	pl = NULL;
}
/*
 * send command to tell android phone to create a playlist with given information
 * index: 9
 */
void syncCreateExisitedPlaylist(char* listname, int num_of_songs, int id) {
	char* temp[3];
	temp[0] = listname;
	char temp1[4];
	char temp2[4];
	sprintf(temp1, "%d", num_of_songs);
	sprintf(temp2, "%d", id);
	temp[1] = temp1;
	temp[2] = temp2;
	struct Command* cmd = initCmd(9, 3, temp);
	send(cmd, CMD);
}
void createExisitedPlaylist(char* listname, int num_of_songs, int id) {
	struct Playlist* pl = initPlaylist(listname);
	pl->num_of_songs = num_of_songs;
	addExisitedListToDB(pl, id);
	pl = NULL;
}
void modifyPlaylistName(int index, char* new_listname) {
	setListName(db.playlists[index], new_listname);

}
void shuffle(int index) {
	printf("Playlist %d is shuffled\n", index);
}
void addSongToPlaylist(int song_index, int list_index) {
	printf("Song %d is added to %d\n", song_index, list_index);
}
void removeList(int index) {
	printf("Playlist: %d is removed\n", index);
}
void play_playlist(int index) {
	printf("Playlist %d is selected and played\n", index);
}
void repeatPlaylist(int index) {
	printf("Playlist: %d is set to repeated\n", index);
}

void moveSongToIndex(char* song, int index, char* listname) {

}
void removeSongFromPlaylist(char* song, char* listname) {

}
void repeatCurrentSong() {

}

void reverse() {

}

void updateDBIndex() {

}

