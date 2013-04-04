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
	temp[0] = (char*)malloc(sizeof(char)*4);
	temp[1] = (char*)malloc(sizeof(char)*8);
	temp[2] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", id);
	sprintf(temp[1], "%d", vol);
	sprintf(temp[2], "%d", pos);
	struct Command* cmd = initCmd(1, 3, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
	free(temp[0]);
	free(temp[1]);
	free(temp[2]);
}
//index 1
void play(int id, int vol, int pos) {
	if(db.songs[id] == NULL || id <= 0 || id > db.num_of_songs) return;
	char temp[30];
	printf("A song %d is started at %d volume.\n", id, vol);
	playSong(db.songs[id], vol, pos, 0);
	if(db.total_songs_playing <= 1) {
		updateMixer();
		syncUpdatePos(id, pos, 1);
		IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 4, 0x07);
		enableAudioDeviceController();
		alt_up_char_buffer_string(char_buffer, db.songs[db.curr_song_id]->song_name, 3, 37);
		sprintf(temp, "%.2f seconds", db.songs[db.curr_song_id]->size/1000.0);
		alt_up_char_buffer_string(char_buffer, temp, 3, 38);
		if(db.songs[db.curr_song_id]->sound->audioFormat != NULL) {
			memset(temp, 0, 30);
			sprintf(temp, "%d kbps", db.songs[db.curr_song_id]->sound->audioFormat->bitRateKbps);
			alt_up_char_buffer_string(char_buffer, temp, 3, 39);
			memset(temp, 0, 30);
			sprintf(temp, "%d channel(s)", db.songs[db.curr_song_id]->sound->audioFormat->channels);
			alt_up_char_buffer_string(char_buffer, temp, 3, 40);
			printf("A song %d is played at %d position.\n", id, pos);
		}
	}
}
/*
 * Function to call when need to sync with Android
 * It will also put command to scheduler
 */
void syncPause(int id) {
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", id);
	struct Command* cmd = initCmd(2, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, (struct Command*)cmd);
	free(temp[0]);
}
//pause current play song; index 2
void pause(int id) {
	disableAudioDeviceController();
	pauseSong(db.songs[id]);
	alt_up_char_buffer_string(char_buffer, "                          ", 3, 37);
	alt_up_char_buffer_string(char_buffer, "                          ", 3, 38);
	alt_up_char_buffer_string(char_buffer, "                          ", 3, 39);
	alt_up_char_buffer_string(char_buffer, "                          ", 3, 40);
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
	printf("The music start to stop.\n");
	disableAudioDeviceController();
	clearSoundMixer();
	int i;
	int size = db.total_songs_playing;
	for(i = 0; i < size; i++) {
		stopSound(db.songs[db.curr_song_ids[i]]->sound);
		removeCurrPlaying(i);
	}
	db.curr_song_id = 0;
	printf("Music has stopped.\n");
}

void syncSetVol(int id, int vol) {
	char* temp[2];
	temp[0] = (char*)malloc(sizeof(char)*4);
	temp[1] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", id);
	sprintf(temp[1], "%d", vol);
	struct Command* cmd = initCmd(4, 2, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);
	free(temp[1]);
}
//index 4
void setVolume(int id, int vol) {
	setGlobalVolume(vol);
	updateVolumeValue(id);
	printf("Volume is set to %d percent\n", vol);
}

void seek(int pos) {
	printf("Seek is set to %d\n", pos);
}
void syncNext(int song_id) {
	syncStop();
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", song_id);
	struct Command* cmd = initCmd(6, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);
}
//index 6
void next(int song_id) {
	int id = 0;
	printf("Next song is selected.\n");
	if(db.curr_playlist_id == 0 && song_id < db.num_of_songs) {
		id = song_id+1;
		playSongFromAllSongs(id, db.songs[id]->volume, 0);
	} else if(db.curr_playlist_id != 0) {
		if(db.index_list_order[db.curr_playlist_id][db.index_list_song[db.curr_playlist_id][song_id]+1] != 0)
			id = db.index_list_order[db.curr_playlist_id][db.index_list_song[db.curr_playlist_id][song_id]+1];
		else if(db.isListRepeated == 1)
			id = db.index_list_order[db.curr_playlist_id][1];

		if(id == 0) return;
		playSongFromList(id, db.curr_playlist_id);
		//syncPlay(id, db.songs[id]->volume, 0);
	}

	printf("Next song is played.\n");
}
void syncPrev(int song_id) {
	syncStop();
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", song_id);
	struct Command* cmd = initCmd(7, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);

}
//index 7
void prev(int song_id) {
	printf("Previous song is selected.\n");
	int id = 0;
	if(db.curr_playlist_id == 0 && song_id > 1) {
		id = song_id-1;
		playSongFromAllSongs(id, db.songs[id]->volume, 0);
	} else if(db.curr_playlist_id != 0) {
		id = db.index_list_order[db.curr_playlist_id][db.index_list_song[db.curr_playlist_id][song_id]-1];

		if(id == 0) return;
		playSongFromList(id, db.curr_playlist_id);
		//syncPlay(id, db.songs[id]->volume, 0);
	}

	printf("Previous song is played.\n");
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
	temp[0] = (char*)malloc(sizeof(char)*15);
	temp[1] = (char*)malloc(sizeof(char)*4);
	temp[2] = (char*)malloc(sizeof(char)*4);
	strcpy(temp[0], listname);
	sprintf(temp[1], "%d", num_of_songs);
	sprintf(temp[2], "%d", id);
	struct Command* cmd = initCmd(9, 3, temp);
	send(cmd, CMD);
	killCmd(&cmd);
	free(temp[0]);
	free(temp[1]);
	free(temp[2]);
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
	temp[0] = (char*)malloc(sizeof(char)*15);
	temp[1] = (char*)malloc(sizeof(char)*4);
	strcpy(temp[0], song_name);
	sprintf(temp[1], "%d", len);
	struct Command* cmd = initCmd(10, 2, temp);
	send(cmd, CMD);
	killCmd(&cmd);
	free(temp[0]);
	free(temp[1]);
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
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", id);
	struct Command* cmd = initCmd(11, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);
}

void selectList(int id) {
	printf("list %d is selected\n", id);
	db.curr_playlist_id = id;
	mouse->frame->elements[3]->currentPlaylist = id;
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
	char* temp[2];
	temp[0] = (char*)malloc(sizeof(char)*4);
	temp[1] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", list_id);
	sprintf(temp[1], "%d", song_id);
	struct Command* cmd = initCmd(13, 2, temp);
	send(cmd, CMD);
	killCmd(&cmd);
	free(temp[0]);
	free(temp[1]);
	//addCmd(com.scheduler, cmd);
}
void addSongToList(int list_index, int song_index) {
	printf("adding song %d\n", song_index);
	if(db.playlists[list_index] == NULL || db.songs[song_index] == NULL) return;
	int i;
	for(i = 1; i < MAX_SONGS; i++) {
		if(db.index_list_order[list_index][i] == 0) {
			db.index_list_order[list_index][i] = song_index;
			db.index_list_song[list_index][song_index] = i;
			db.playlists[list_index]->num_of_songs++;
			printf("Song %d is added to %d\n", song_index, list_index);
			return;
		}
	}
}

//index 14
void syncAddExisitedSongToList(int list_id, int song_id) {
	char* temp[2];
	temp[0] = (char*)malloc(sizeof(char)*4);
	temp[1] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", list_id);
	sprintf(temp[1], "%d", song_id);
	struct Command* cmd = initCmd(14, 2, temp);
	send(cmd, CMD);
	killCmd(&cmd);
	free(temp[0]);
	free(temp[1]);
}

//index 15
void syncRemoveSongFromList(int list_id, int song_id) {
	/*char temp[2][4];
	sprintf(temp[0], "%d", list_id);
	sprintf(temp[1], "%d", song_id);
	struct Command* cmd = initCmd(14, 2, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);*/
}
void removeSongFromList(int list_id, int song_id) {
	printf("remove song %d from list %d\n", song_id, list_id);
	db.index_list_order[list_id][db.index_list_song[list_id][song_id]] = 0;
	db.index_list_song[list_id][song_id] = 0;
	db.playlists[list_id]->num_of_songs--;
}

//index 16
void syncUpdatePos(int song_id, int pos, int isStart) {
	char* temp[3];
	temp[0] = (char*)malloc(sizeof(char)*4);
	temp[1] = (char*)malloc(sizeof(char)*4);
	temp[2] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", song_id);
	sprintf(temp[1], "%d", pos);
	sprintf(temp[2], "%d", isStart);
	struct Command* cmd = initCmd(16, 3, temp);
	send(cmd, CMD);
	killCmd(&cmd);
	free(temp[0]);
	free(temp[1]);
	free(temp[2]);
}

//index 17
void syncRepeatList(int index) {
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", index);
	struct Command* cmd = initCmd(17, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);
}
void repeatList(int index) {
	db.isListRepeated = (db.isListRepeated == 1) ? 0 : 1;
	printf("Repeat list %d is %d\n", index, db.isListRepeated);
}

//index18
void modifyListName(int index, char* new_listname) {
	setListName(db.playlists[index], new_listname);

}
//index 19
void syncRemoveList(int index) {
	char* temp[1];
	temp[0] = (char*)malloc(sizeof(char)*4);
	sprintf(temp[0], "%d", index);
	struct Command* cmd = initCmd(19, 1, temp);
	send(cmd, CMD);
	addCmd(com.scheduler, cmd);
	free(temp[0]);

}
void removeList(int index) {
	if(removeListFromDB(index) == -1) {
		printf("List does not existed\n");
		return;
	} printf("Playlist %d is removed.\n", index);
}

//index 20
void playSongFromAllSongs(int id, int vol, int pos) {
	playSongsFromSongPanel(id, vol, pos);
}
//index 21
void openAllSongPanel() {
	drawAllSongs();
}
//index 22
void openPlaylistsPanel() {
	drawAllLists();
}
//index 23
void openSongsFromList(int list_id) {
	drawAllSongsInList(list_id);
}
//index 24
void playSongFromList(int song_id, int list_id) {
	syncSelectList(list_id);
	highlightSongInList(list_id, song_id);
	play(song_id, db.songs[song_id]->volume, db.songs[song_id]->pos);
	printf("play song %d from list %d\n", song_id, list_id);
}

// index 25
void setPitch(int id, int pitch, int quality) {
	if(db.songs[id] == NULL || id <= 0 || id > db.num_of_songs) return;
	setSongPitch(db.songs[id], pitch, quality);
}

// index 26
void setPlaybackSpeed(int id, int speed) {
	if(db.songs[id] == NULL || id <= 0 || id > db.num_of_songs) return;
	setSongPlaybackSpeed(db.songs[id], speed);
}

// index 27
void reloadSongById(int id) {
	if(db.songs[id] == NULL || id <= 0 || id > db.num_of_songs) return;
	unloadSong(db.songs[id]);
	loadSong(db.songs[id]);
}

void shuffle(int index) {
	printf("Playlist %d is shuffled\n", index);
}

void moveSongToIndex(char* song, int index, char* listname) {

}

void repeatCurrentSong() {

}

