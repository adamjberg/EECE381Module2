/*
 * Database.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Database.h"

void initDatabase() {
	db.cache = initCache();
	db.curr_playlist_id = 0;
	db.curr_song_id = 0;
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
	for(i = 1; i < size; i++) { // start from 1 since 0 always null
		if(strcmp(song_name, db.songs[i]->song_name) == 0) {
			return db.songs[i];
		}
	}
	return NULL;
}
/*
 * Load a playlist from SD card to the the database
 * return -1 if no more list can be found in the SD card; 0 otherwise
 */

int loadListFromSD() {
	int file_pointer;
	if((file_pointer = alt_up_sd_card_fopen(LISTFILE, false)) < 0) {
		printf("No more playlist to load\n");
		alt_up_sd_card_fclose(file_pointer);
		return -1;
	}
/*
	int i;
	struct Playlist* playlist = initPlaylist();
	playlist->list_name = readLine(file_pointer);
	char* num_of_song = readLine(file_pointer);
	playlist->num_of_songs = atoi(num_of_song);
	for(i = 0; i<playlist->num_of_songs; i++) {
		enqueue(playlist->songs, initSong(readLine(file_pointer)));
	}

	free(num_of_song);
	num_of_song = NULL;
	addListToDB(playlist);
	playlist = NULL;*/
	alt_up_sd_card_fclose(file_pointer);
	return 0;
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
	char* result = (char*)malloc(sizeof(char)*i);
	strncpy(result, temp, i+1);
	return result;
}
/*
 * A function that writes or rewrites the playlist to a text file to SD card
 * return -1 if fail to write; 0 otherwise
 * @parm filename the file name of text file; must end with .TXT
 * @parm playlist the playlist object
 */
int addListToSD(struct Playlist* playlist) {
	if(playlist == NULL) return -2;
	int file_pointer;
	if((file_pointer = alt_up_sd_card_fopen(LISTFILE, false)) < 0) {
		if((file_pointer = alt_up_sd_card_fopen(LISTFILE, true)) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			printf("Save playlist failed\n");
			return -1;
		}
	}

/*	char num_of_songs[4];
	int num = playlist->num_of_songs;
	int i = 0;
	struct Song* song;

	writeLine(file_pointer, playlist->list_name, strlen(playlist->list_name));
	sprintf(num_of_songs, "%d", num);
	writeLine(file_pointer, num_of_songs, 3);
	for(i = 0; i < num; i++) {
		song = dequeue(playlist->songs);
		writeLine(file_pointer, song->song_name, strlen(song->song_name));
		enqueue(playlist->songs, song);
	}

	song = NULL;*/
	alt_up_sd_card_fclose(file_pointer);
	return 0;
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
 * Read and load all songs in .WAV format from SD to DB.
 * This function writes all songs with their IDs into
 * a file (fileName) that has special format in
 * storing the songs.
 * */
int loadSongsFromSD(char* txtFile){
	int index;
	char* fileName = NULL;

	// open txtFile
	int fileHandler;
	if (txtFile == NULL) return -2;
	fileHandler = alt_up_sd_card_fopen(txtFile, false);
	if (fileHandler < 0){
		if ((fileHandler = alt_up_sd_card_fopen(txtFile, true)) < 0){
			alt_up_sd_card_fclose(fileHandler);
			printf("Loading songs error %d.\n", fileHandler);
			return -1;
		}
	}
	// iterate through files in sdcard, load only .WAV files
	index = alt_up_sd_card_find_first("", fileName);
	if (index != 0){
		printf("Cannot read songs from SDCard.\n");
	}
	char* strToStore;
	int numOfWavFiles = 0;
	int size;
	char file[11];
	while (index != -1){
		strcpy(file, fileName);
		if (strstr(file, ".WAV") != NULL){
			strToStore = "";
			numOfWavFiles++;
			//printf("%s\n", fileName);
			addSongToDB(initSong(fileName));
			size = sprintf(strToStore, "%d %s", numOfWavFiles, file);
			//printf("Song will be stored as %s\n", strToStore);
			writeLine(fileHandler, strToStore, size);
		}
		index = alt_up_sd_card_find_next(fileName);
	}
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

