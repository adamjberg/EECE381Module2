/*
 * Database.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Database.h"

void initDatabase() {
	//db.cache = initCache();
	db.curr_playlist_id = 0;
	db.song_playing_size = 0;
	db.num_of_lists = 0;
	db.num_of_songs = 0;
	db.avail_list_index = initQueue();
	int i;
	int* temp;
	db.used_list_index[0] = 1; //index 0 should never get used
	db.index_list_order[0] = NULL;
	db.index_list_song[0] = NULL;
	db.playlists[0] = NULL;
	db.songs[0] = NULL;
	for(i = 1; i < MAX_LISTS; i++) {
		temp = (int*)malloc(sizeof(int));
		*temp = i;
		enqueue(db.avail_list_index, temp);
		db.used_list_index[i] = 0;
		db.index_list_order[i] = NULL;
		db.index_list_song[i] = NULL;
		db.playlists[i] = NULL;
		db.songs[i] = NULL;
	} temp = NULL;
	//loadListsFromSD();
}

void update() {
	int i = 0, j = 0;
	for(i = 0; i < db.num_of_lists; i++) {
		while(db.used_list_index[++j] != 1);
		if(db.playlists[j] == NULL) {
			printf("update error\n");
			return;
		}
		syncCreateExisitedPlaylist(db.playlists[j]->list_name,
									db.playlists[j]->num_of_songs,
									db.playlists[j]->id);

	}
	for(i = 1; i <= db.num_of_songs; i++) {
		syncCreateSong(db.songs[i]->song_name);
	}

	syncDBFinish();
}
/*
 * Query the list with a given play list name
 * return the first list founded with the same name, NULL otherwise
 */
struct Playlist* queryListByName(char* list_name) {
	if(list_name == NULL || db.num_of_lists == 0) return NULL;
	int i = 0, j = 1;
	int size = db.num_of_lists;
	while(i < size) {
		if(db.used_list_index[j] == 1) {
			//make sure current index is in used
			if(db.playlists[j] == NULL) { //double check
				printf("querry list by name error\n");
				return NULL;
			}
			if(strcmp(list_name, db.playlists[j]->list_name) == 0) {
				return db.playlists[j];
			}
			i++;
		} j++;
	}
	return NULL;
}

struct Song* querySongByName(char* song_name) {
	if(song_name == NULL) return NULL;
	int i;
	int size = db.num_of_songs;
	for(i = 1; i <= size; i++) { // start from 1 since 0 always null
		if(strcmp(song_name, db.songs[i]->song_name) == 0) {
			return db.songs[i];
		}
	}
	return NULL;
}
/*
 * Add a song to the database
 */
void addSongToDB(struct Song* song) {
	db.num_of_songs++;
	setSongId(song, db.num_of_songs);
	db.songs[db.num_of_songs] = song;
}
/*
 * Add a playlist to the database; id will be automatically assigned
 */
void addListToDB(struct Playlist* playlist) {
	if(playlist == NULL || db.avail_list_index->size <= 0) return;
	db.num_of_lists++;
	int* index = dequeue(db.avail_list_index);
	db.used_list_index[*index] = 1;
	setListId(playlist, *index);
	db.playlists[*index] = playlist;
	db.index_list_order[*index] = (int*)malloc(sizeof(int)*MAX_SONGS);
	db.index_list_song[*index] = (int*)malloc(sizeof(int)*MAX_SONGS);
	int i;
	for(i = 1; i < MAX_SONGS; i++) {
		db.index_list_order[*index][i] = 0;
		db.index_list_song[*index][i] = 0;
	}
	free(index);
	index = NULL;
}
/*
 * Add a playlist to the database by giving its id
 * adding operation will failed if the id has been used already
 */
void addExisitedListToDB(struct Playlist* playlist, int id) {
	if(playlist == NULL || db.avail_list_index->size <= 0
			|| id == 0 || db.used_list_index[id] == 1) return;
	int* index = dequeueValue(db.avail_list_index, id);
	db.num_of_lists++;
	db.used_list_index[*index] = 1;
	setListId(playlist, *index);
	db.playlists[*index] = playlist;
	db.index_list_order[*index] = (int*)malloc(sizeof(int)*MAX_SONGS);
	db.index_list_song[*index] = (int*)malloc(sizeof(int)*MAX_SONGS);
	int i;
	for(i = 1; i < MAX_SONGS; i++) {
		db.index_list_order[*index][i] = 0;
		db.index_list_song[*index][i] = 0;
	}
	free(index);
	index = NULL;
}
/*
 * Remove a playlist from the database
 * return 0 if list is removed, -1 if cannot find list in the database
 */
int removeListFromDB(int list_id) {
	if(db.used_list_index[list_id] == 0 || list_id <= 0) return -1;
	killPlaylist(&(db.playlists[list_id]));
	db.playlists[list_id] = NULL;
	db.used_list_index[list_id] = 0;
	int* temp = (int*)malloc(sizeof(int));
	*temp = list_id;
	enqueue(db.avail_list_index, temp);
	free(db.index_list_order[list_id]);
	db.index_list_order[list_id] = NULL;
	free(db.index_list_song[list_id]);
	db.index_list_song[list_id] = NULL;
	db.num_of_lists--;
	temp = NULL;
	return 0;
}

/*
 * A helper function that read a line in a text file, require file pointer and does not close the file
 * Assumption has made that a line does not go over 100 characters
 */
char* readLine(int file_pointer) {
	char temp[100];
	int i = -1;

	do {
		i++;
		temp[i] = alt_up_sd_card_read(file_pointer);
		if(i == 0 && temp[i] == '\n') //this is to skip an empty line if any
			temp[i] = alt_up_sd_card_read(file_pointer);
		if(i >= 100) {
			printf("WARNNING! reading a line that contains more than 100 characters\n");
			break;
		}
	} while(temp[i] != '\n' && temp[i] != '\r' && temp[i] != 0 && temp[i] != -1);
	temp[i] = 0;
	if(i == 0)
		return NULL;
	char* res = (char*)malloc(sizeof(char)*i);
	strncpy(res, temp, i+1);
	return res;
}

/*
 * A helper function that write a segment of data to SD card and write a LINE FEED at the end
 * it does not close file pointer and require an input of file pointer
 */
void writeLine(int file_pointer, char* data, int size) {
	if(data == NULL) return;
	int i;
	for( i = 0; i < size; i++ )
	{
		if(data[i] == 0) break;
		if(!alt_up_sd_card_write(file_pointer, data[i])){
			printf("Write a character to SD card failed.\n");
		}
	}
	if(!alt_up_sd_card_write(file_pointer, '\r')) {
		printf("Write a new line ASCII failed\n");
	}
	if(!alt_up_sd_card_write(file_pointer, '\n')) {
		printf("Write a new line ASCII failed\n");
	}
}

/*
 * Helper function to open a file from SDCard
 * */
int openFileFromSD(char* file){
	int fileHandler;
	if (file == NULL) return -2;
	fileHandler = alt_up_sd_card_fopen(file, false);
	if (fileHandler < 0){
		if ((fileHandler = alt_up_sd_card_fopen(file, true)) < 0){
			alt_up_sd_card_fclose(fileHandler);
			return -1;
		}
	}
	return fileHandler;
}

/*
 * Find all .WAV files in SDCard and store their names to
 * SONGS.TXT
 * Return values: -1 means error in fopen/fclose, 0 means success
 * -2 means error in searching through the SDCard.
 * */
int updateSongsFromSD(){
	int index, size;
	char fileName[15];
	char* strToStore;
	int numOfWavFiles = 0;

	int fileHandler;
	if ((fileHandler = openFileFromSD(SONGFILE)) < 0){
		printf("Error opening file in updateSongsFromSD.\n");
		return -1;
	}
	index = alt_up_sd_card_find_first("", fileName);
	if (index != 0){
		printf("ERROR: updateSongsFromSD.\n");
		return -2;
	}
	while (index != -1){
		if (strstr(fileName, ".WAV") != NULL){
			strToStore = "";
			numOfWavFiles++;
			size = sprintf(strToStore, "%d %s", numOfWavFiles, fileName);
			writeLine(fileHandler, strToStore, size);
		}
		index = alt_up_sd_card_find_next(fileName);
	}
	strToStore = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly in updateSongsFromSD.\n");
		return -1;
	}
	return 0;
}
/*
 * Saves all playlists with their information to
 * LISTS.TXT in SDCard.
 * Return value: 0 means success, -1 means error in fopen/fclose
 */
int saveListsToSD() {
	int fileHandler;
	if ((fileHandler = openFileFromSD(LISTFILE)) < 0){
		printf("Error opening file in addListToSD.\n");
		return -1;
	}
	int i, size;
	char* strToStore = (char*)malloc(sizeof(char)*99);
	for(i = 1; i < MAX_LISTS; i++){
		//writeLine(fileHandler, "     ", 5);
		if (db.used_list_index[i] == 0){
			size = sprintf(strToStore, "%d %d %d", i, 0, 0);
			writeLine(fileHandler, strToStore, size);
		} else {
			size = sprintf(strToStore,"%d %s %d", db.playlists[i]->id, db.playlists[i]->list_name, db.playlists[i]->num_of_songs);
			writeLine(fileHandler, strToStore, size);
		}
	}
	free(strToStore);
	strToStore = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

/*
 * Reads LISTS.TXT and load all playlists into the DB.
 * Return values: 0 means success, -1 means error in fopen/fclose.
 */
int loadListsFromSD() {

	int fileHandler;
	if ((fileHandler = openFileFromSD(LISTFILE)) < 0){
		printf("Loading list error!\n");
		return -1;
	}
	char* line = NULL;
	char* str = NULL;

	int cursorPos = 0;
	int count = 0;
	int numOfSongs, id;
	char array[100];
	while (1){
		line = readLine(fileHandler);
		if (line == NULL) { break;}
		//it's always ID NAME NUMOFSONGS
		str = separateString(line, ' ', cursorPos);
		id = atoi(str);
		cursorPos = strlen(str) + 1;
		free(str); str = NULL;
		str = separateString(line, ' ', cursorPos);
		cursorPos = cursorPos + strlen(str) + 1;
		strcpy(array, str);
		free(str); str = NULL;
		str = separateString(line, ' ', cursorPos);
		numOfSongs = atoi(str);
		cursorPos = 0;
		free(str);
		str =NULL;
		free(line);
		line = NULL;
		// add the playlist
		if (array[0] != '0') {
			createExisitedPlaylist(array, numOfSongs, id);
		}
		if (id == 50) {break;}
	}
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

/*
 * Separates a string from position specified in
 * 'pos' parameter to the position of
 * delimiter specified in 'delim'.
 * Original string does not get modified.
 * Return the splitted string.
 * */
char* separateString(char* strToSep, char delim, int pos){
	char temp[101];
	char strCut[100];
	strcpy(temp, strToSep);
	// find position of delimiter
	int i;
	for(i = pos; i < strlen(strToSep); i++){
		if (temp[i] == delim){
			break;
		}
	}
	// copy string from pos to delimiter position
	int j;
	int k = 0;
	for(j = pos; j < i; j++){
		strCut[k] = temp[j];
		k++;
	}
	strCut[k] = '\0';
	// return the result
	int size = k+1;
	char* result = (char*)malloc(sizeof(char)*(size));
	strcpy(result, strCut);
	return result;
}




