/*
 * Database.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Database.h"

/*
 * Query the list with a given play list name
 * return the list if founded, NULL otherwise
 */
struct Playlist* queryListByName(char* list_name) {
	if(list_name == NULL) return NULL;
	int i;
	struct Playlist* temp = db.playlists;
	for(i = 0; i < db.num_of_lists; i++) {
		if(strcmp(list_name, temp->list_name) == 0) {
			return temp;
		} else {
			temp = db.playlists->prev;
		}
	}
	temp = NULL;
	return NULL;
}

struct Song* querySongByName(char* song_name) {
	return NULL;
}
/*
 * Load a playlist from SD card to the the database
 * return -1 if no more list can be found in the SD card; 0 otherwise
 */
int loadListFromSD(char* filename) {
	if(filename == NULL) return -2;
	int file_pointer;
	if((file_pointer = alt_up_sd_card_fopen(filename, false)) < 0) {
		printf("No more playlist to load\n");
		alt_up_sd_card_fclose(file_pointer);
		return -1;
	}
	//a temp malloc until construct of playlist has been impelemnted
	struct Playlist* playlist = (struct Playlist*)malloc(sizeof(struct Playlist));
	playlist->list_name = readLine(file_pointer);
	//TODO: read other attributes


	addListToDB(playlist);
	alt_up_sd_card_fclose(file_pointer);
	return 0;
}
/*
 * Add a playlist to the database and set the pointer of database to this list
 */
void addListToDB(struct Playlist* playlist) {
	playlist->next = NULL;
	if(db.num_of_lists == 0)
		db.playlists = playlist;
	else {
		db.playlists->next = playlist;
		playlist->prev = db.playlists;
		db.playlists = db.playlists->next;
	}
	db.num_of_lists++;
}
/*
 * A helper function that read a line in a text file, require file pointer and does not close the file
 * Assumption has made that a line does not go over 100 characters
 */
char* readLine(int file_pointer) {
	char temp[100];
	int i = 0;
	while((temp[i] = alt_up_sd_card_read(file_pointer)) != NEWLINE) {
		i++;
		if(i >= 100) {
			printf("WARNNING! reading a line that contains more than 100 characters\n");
			break;
		}
	} temp[i] = 0;
	char* result = (char*)malloc(sizeof(char)*i);
	strncpy(result, temp, i+1);
	return result;
}
/*
 * A helper function that write or rewrite the playlist as a text file to SD card
 * return -1 if fail to write; 0 otherwise
 */
int addListToSD(char* filename, struct Playlist* playlist) {
	if(playlist == NULL) return -2;
	int file_pointer;
	if((file_pointer = alt_up_sd_card_fopen(filename, false)) < 0) {
		if((file_pointer = alt_up_sd_card_fopen(filename, true)) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			printf("Save playlist %s failed\n", filename);
			return -1;
		}
	}

	writeLine(file_pointer, playlist->list_name, strlen(playlist->list_name));
	//TODO: Write other attributes to the file

	alt_up_sd_card_fclose(file_pointer);
	return 0;
}
/*
 * A helper function that write a segment of data to SD card and write a LINE FEED at end
 * it does not close file pointer and require an input of file pointer
 */
void writeLine(int file_pointer, char* data, int size) {
	if(data == NULL) return;
	int i;
	for( i = 0; i < size; i++ )
	{
		if(!alt_up_sd_card_write(file_pointer, data[i])){
			printf("Write a character to SD card failed.\n");
		}
	}
	if(!alt_up_sd_card_write(file_pointer, NEWLINE)) {
		printf("Write a new line ascii failed\n");
	}
}
