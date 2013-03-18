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
void syncPlay(int id, int vol, int pos) {
	char* temp[3];
	char tempId[4];
	char tempVol[4];
	sprintf(tempId, "%d", id);
	temp[0] = tempId;
	sprintf(tempVol, "%d", vol);
	temp[1] = tempVol;
	char tempPos[4];
	sprintf(tempPos, "%d", pos);
	temp[2] = tempPos;
	struct Command* cmd = initCmd(1, 3, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
}
//index 1
void play(int id, int vol, int pos) {
	if(db.songs[id] == NULL) return;
	playSong(db.songs[id], vol, pos, 0);
	printf("A song %d is played at %d position.\n", id, pos);
}
/*
 * Function to call when need to sync with Android
 * It will also put command to scheduler
 */
void syncPause(int id) {
	char* temp[1];
	char tempId[4];
	sprintf(tempId, "%d", id);
	temp[0] = tempId;
	struct Command* cmd = initCmd(2, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
}
//pause current play song; index 2
void pause(int id) {
	pauseSong(db.songs[id]);
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

void syncSetVol(int id, int vol) {
	char* temp[2];
	char tempId[4];
	char tempVol[4];
	sprintf(tempId, "%d", id);
	sprintf(tempVol, "%d", vol);
	temp[0] = tempId;
	temp[1] = tempVol;
	struct Command* cmd = initCmd(4, 2, temp);
	addCmd(com.scheduler, cmd);
}
//index 4
void setVolume(int id, int vol) {
	db.songs[id]->sound->volume = (float)vol/100.0;
	printf("Volume is set to %d percent\n", vol);
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

/*
 * send command to a create playlist on android side
 * index: 8
 */
void syncCreatePlaylist(char* list_name) {
	char* temp[1];
	temp[0] = list_name;
	struct Command* cmd = initCmd(8, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
}
//index 8
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
	killCmd(&cmd);
}
void createExisitedPlaylist(char* listname, int num_of_songs, int id) {
	struct Playlist* pl = initPlaylist(listname);
	pl->num_of_songs = num_of_songs;
	addExisitedListToDB(pl, id);
	pl = NULL;
}
/*
 * Send command to create a song on android phone
 * index 10
 */
void syncCreateSong(char* song_name, int len) {
	char* temp[2];
	temp[0] = song_name;
	char temp1[4];
	sprintf(temp1, "%d", len);
	temp[1] = temp1;
	struct Command* cmd = initCmd(10, 2, temp);
	send(cmd, CMD);
	killCmd(&cmd);
}
void createSong(char* song_name, int len) {
	struct Song* song = initSong(song_name);
	song->size = len;
	addSongToDB(song);
	song = NULL;
}

/*
 * Send command to select a list to play
 * index: 11
 */
void syncSelectList(int id) {
	char* temp[0];
	char temp1[4];
	sprintf(temp1, "%d", id);
	temp[0] = temp1;
	struct Command* cmd = initCmd(11, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
}

void selectList(int id) {
	printf("list %d is selected\n", id);
	db.curr_playlist_id = id;
}

/*
 * Send command to notify android app that the sync process at start up is finished
 * index: 12
 */

void syncDBFinish() {
	struct Command* cmd = initCmd(12, 0, NULL);
	send(cmd, CMD);
	killCmd(&cmd);
}

void syncAddSongToList(int list_id, int song_id) {
	char temp[2][4];
	sprintf(temp[0], "%d", list_id);
	sprintf(temp[1], "%d", song_id);
	struct Command* cmd = initCmd(13, 2, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
}
void addSongToList(int list_index, int song_index) {
	if(db.playlists[list_index] == NULL || db.songs[song_index] == NULL) return;
	int i;
	for(i = 1; i < MAX_SONGS; i++) {
		if(db.index_list_order[list_index][i] == 0) {
			db.index_list_order[list_index][i] = song_index;
			db.index_list_song[list_index][song_index] = i;
			return;
		}
	}
	printf("Song %d is added to %d\n", song_index, list_index);
}

void syncRemoveSongFromList(int list_id, int song_id) {
	char temp[2][4];
	sprintf(temp[0], "%d", list_id);
	sprintf(temp[1], "%d", song_id);
	struct Command* cmd = initCmd(14, 2, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
}
void removeSongFromList(int list_id, int song_id) {

}

void modifyPlaylistName(int index, char* new_listname) {
	setListName(db.playlists[index], new_listname);

}
void shuffle(int index) {
	printf("Playlist %d is shuffled\n", index);
}

void updateSongToPlaylist(int song_index, int list_index, int order) {

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

void repeatCurrentSong() {

}

void reverse() {

}

void updateDBIndex() {

}

