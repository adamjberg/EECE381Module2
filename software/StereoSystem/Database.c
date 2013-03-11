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
	db.curr_song_id = db.next_song_id = db.prev_song_id = 0;
	db.num_of_lists = 0;
	db.num_of_songs = 0;
}
/*
 * Query the list with a given play list name
 * return the first list founded with the same name, NULL otherwise
 */
struct Playlist* queryListByName(char* list_name) {
	if(list_name == NULL) return NULL;
	int i;
	//struct Playlist* temp = db.playlists;
	int size = db.num_of_lists;
	for(i = 0; i < size; i++) {
		if(strcmp(list_name, db.playlists[i]->list_name) == 0) {
			return db.playlists[i];
		}
	}
	return NULL;
}

struct Song* querySongByName(char* song_name) {
	if(song_name == NULL) return NULL;
	int i;
	int size = db.num_of_songs;
	for(i = 0; i < size; i++) {
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
 * Add a playlist to the database
 */
void addListToDB(struct Playlist* playlist) {
	db.num_of_lists++;
	setListId(playlist, db.num_of_lists);
	db.playlists[db.num_of_lists] = playlist;
}
/*
 * Remove a playlist from the database
 * return 0 if list is removed, -1 if cannot find list in the database
 */
int removeListFromDB(int list_id) {
	if(list_id > db.num_of_lists || list_id <= 0) return -1;
	killPlaylist(&db.playlists[list_id]);
	int i;
	for(i = list_id+1; i <= db.num_of_lists; i++) {
		db.playlists[i]->id--;
		db.playlists[i-1] = db.playlists[i];
	}
	db.playlists[i] = NULL;
	db.num_of_lists--;
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
 * Read all songs in .WAV format from SD and write it into
 * a file (fileName) that has special format in
 * storing the songs.
 * */
int loadSongsFromSD(char* txtFile){
	int i;
	char* fileName;

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
	i = alt_up_sd_card_find_first("", fileName);
	if (i != 0){
		printf("Cannot read songs from SDCard.\n");
	}
	char* strCopy;
	char* strToStore;
	int numOfWavFiles = 0;
	int size;
	while (i != -1){
		i = alt_up_sd_card_find_next(fileName);
		strcpy(strCopy, fileName);
		if (strcmp(strstr(strCopy, ".WAV"), ".WAV") == 0){
			strToStore = "";
			numOfWavFiles++;
			//printf("%s\n", fileName);
			size = sprintf(strToStore, "%d %s", numOfWavFiles, fileName);
			printf("File will be stored as %s\n", strToStore);
			writeLine(fileHandler, strToStore, size);
		}
	}
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

