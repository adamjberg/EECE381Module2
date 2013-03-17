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
	db.curr_song_ids = initQueue();
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
	loadListsFromSD();
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
		syncCreateSong(db.songs[i]->song_name, db.songs[i]->size);
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
	char* res = (char*)malloc(sizeof(char)*(i+1));
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
	char array[100];
	int i = 0;
	while (i < 50){
		line = readLine(fileHandler);
		memset(&array[0], 0, sizeof(array)/sizeof(array[0]));
		strcpy(array, line);
		if (array[0] == 0) {
			break;
		}
		createPlaylistFromTxt(line);
		free(line);
		line = NULL;
		i++;
	}
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}


void createPlaylistFromTxt(char* line){
	char temp[20];
	char substr[20];
	int id = 0;
	int i, last_position;
	int iteration = 0;

	memset(&temp[0], 0 , sizeof(temp)/sizeof(temp[0]));
	strcpy(temp, line);
	for (i = 0; i < strlen(temp); i++){
		if (temp[i] == ' '){
			if (iteration == 0){
				memset(&substr[0], 0 , sizeof(substr)/sizeof(substr[0]));
				strncpy(substr, line, i);
				id = strtol(substr, NULL, 10);
				last_position = i;
				iteration++;
			} else {
				memset(&temp[0], 0 , sizeof(temp)/sizeof(temp[0]));
				strncpy(temp, line+last_position+1, i-last_position-1);
				temp[strlen(temp)] = '\0';
				if(temp[0] != '0'){
					memset(&substr[0], 0 , sizeof(substr)/sizeof(substr[0]));
					strcpy(substr, line+i+1);
					createExisitedPlaylist(temp, strtol(substr, NULL, 10), id);
					break;
				}
			}
		}
	}
}

/*
 * Find all .WAV files in SDCard and return their names in an
 * array of strings.
 * Return: array containing names of all .WAV files in SDCard
 * */
char** getSongsFromSD(){
	int fileStatus = 0;
	char fileName[15];
	int numOfWavFiles = 0;

	// songNames is array of strings that store song names.
	char** songNames = malloc(MAX_SONGS *sizeof(char*));

	fileStatus = alt_up_sd_card_find_first("", fileName);
	if (fileStatus != 0){
		printf("ERROR: updateSongsFromSD.\n");
		return -2;
	}
	while (fileStatus != -1){
		if (strstr(fileName, ".WAV") != NULL){
			songNames[numOfWavFiles] = malloc(20 * sizeof(char));
			strcpy(songNames[numOfWavFiles], fileName);
			numOfWavFiles++;
		}
		fileStatus = alt_up_sd_card_find_next(fileName);
	}
	// set ending condition, NULL means end of the array
	songNames[numOfWavFiles] = NULL;
	return songNames;
}

/*
 * Stores all songs stored in SONGS.TXT file and return them
 * in an array of strings.
 * Return: array containing names of all songs in SONGS.TXT
 * *//*
char** getSongsFromTxt(){
	int fileHandler;
	if ((fileHandler = openFileFromSD(SONGFILE)) < 0){
		printf("Reading songs from SONGS.TXT error!\n");
	}

	char** songNames = malloc(MAX_SONGS *sizeof(char*));
	char* line = NULL;
	char* str = NULL;
	char temp[20];
	int numOfSongs = 0;
	int id, length, cursorPos;
	while (1){
		line = readLine(fileHandler);
		strcpy(temp, line);
		if (temp[0] == NULL) {
			break;
		}
		// separate line being read to ID NAME LENGTH
		cursorPos = 0;
		str = separateString(line, ' ', cursorPos);
		id = atoi(str);
		cursorPos = strlen(str) + 1;
		free(str);
		str = NULL;
		str = separateString(line, ' ', cursorPos);
		strcpy(temp, str);
		cursorPos = cursorPos + strlen(str) + 1;
		free(str);
		str = NULL;
		str = separateString(line, ' ', cursorPos);
		length = atoi(str);
		free(str);
		str = NULL;
		//store song names to array of strings
		songNames[numOfSongs] = malloc(20 * sizeof(char));
		strcpy(songNames[numOfSongs], temp);
		numOfSongs++;
		//make a new song and add it to database
		free(line);
		line = NULL;

	}
	//set an end sign, null means end of the array.
	songNames[numOfSongs] = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
	}
	return songNames;
}*/
/*
 * Check if arrFromTXT contains all songs in arrFromSDFiles.
 * Store whatever arrFromTXT missing to an array of strings,
 * return that array.
 * */
char** checkTxtFile(char** arrFromSDFiles, char** arrFromTxt){
	int sdFiles_index, txt_index;
	int diffArray_index = 0;
	char** diffArray = malloc(MAX_SONGS*(sizeof(char*)));

	for (sdFiles_index = 0; sdFiles_index < MAX_SONGS; sdFiles_index++){
		if (arrFromSDFiles[sdFiles_index] == NULL){
			// we reach the end of array that contains song names from SDFiles directory.
			// no more songs to compare, quit the loop & function.
			printf("Reach the end of sdFiles array. Loop should quit.\n");
			break;
		} else {
			printf("Array from SDFiles being compared is %s\n", arrFromSDFiles[sdFiles_index]);
			for (txt_index = 0; txt_index < MAX_SONGS; txt_index++){
				if (arrFromTxt[txt_index] == NULL){
					// we reach the end of song names array from SONGS.TXT.
					// SONGS.TXT does not contain this song stored in arrFromSDFiles[sdFiles_index]
					// store the song name in diffArray, quit the loop.
					printf("Reach the end of txtFiles array. Storing the diff, move on.\n");
					diffArray[diffArray_index] = malloc(20*(sizeof(char)));
					strcpy(diffArray[diffArray_index], arrFromSDFiles[sdFiles_index]);
					diffArray_index++;
					break;
				} else {
					// compare the song name from arrFromTxt and arrFromSDFiles.
					// quit the loop once we find a pair.
					printf("Comparing that array with %s\n", arrFromTxt[txt_index]);
					if (strcmp(arrFromTxt[txt_index], arrFromSDFiles[sdFiles_index]) == 0){
						printf("They are the same!\n");
						break;
					} else {
						printf("They're not the same.. Moving on..\n");
					}
				}
			}
		}
	}
	// set end sign to the diffArray
	diffArray[diffArray_index] = NULL;
	return diffArray;
}
/*
 * Helper function to free string array.
 * Array needs to have a null element at the end
 * */
void freeStrArray(char** str_arr){
	int i = 0;
	while(str_arr[i] != NULL){
		free(str_arr[i]);
		str_arr[i] = NULL;
		i++;
	}
	free(str_arr);
	str_arr = NULL;
}
/*
 * Update SONGS.TXT with all songs that is found in SDCard but
 * not in SONGS.TXT. All these new songs will have 0 length.
 * */
int updateSongsTxt(char** songsToAdd){
	if (songsToAdd == NULL) {return -2;}
	if (songsToAdd[0] == NULL) {
		// there are no songs to add to SONGS.TXT.
		// quit.
		return 0;
	}
	int fileHandler, i, size;
	if ((fileHandler = openFileFromSD(SONGFILE)) < 0){
		printf("Reading songs from SONGS.TXT error!\n");
	}

	char* line = NULL;
	char* strToStore = NULL;
	char temp[100];
	while (1){
		line = readLine(fileHandler);
		strcpy(temp, line);
		if (temp[0] == NULL) {
			printf("Last line being read is %s\n", line);
			break;
		}
		free(line);
		line = NULL;
	}

	for (i = 0; i < MAX_SONGS; i++){
		if (songsToAdd[i] = NULL){
			// we reach the end of the array,
			// all songs should be added to SONGS.TXT. Quit the loop
			printf("No more songs to add. Quitting\n");
			break;
		}
		size = sprintf(strToStore, "%d %s 0", db.num_of_songs, songsToAdd[0]);
		printf("Song will be added is %s\n", strToStore);
		//writeLine(fileHandler, strToStore, size);
		strToStore = NULL;
	}

	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;

}
