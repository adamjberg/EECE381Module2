/*
 * Database.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "Global.h"
#define LISTFILE "LISTS.TXT"
#define SONGFILE "SONGS.TXT"
#define MAX_LISTS 51
#define MAX_SONGS 101
struct database db;

struct database {
	struct Playlist* playlists[MAX_LISTS];
	int curr_playlist_id;
	int num_of_lists;
	struct Song* songs[MAX_SONGS];
	int curr_song_id;
	int num_of_songs;
	int* index_list_song[MAX_LISTS];
	int* index_list_order[MAX_LISTS];
	struct Queue* avail_list_index;
	int used_list_index[MAX_LISTS];

	struct Cache* cache;
};

void initDatabase();
struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
int addListToSD(struct Playlist*);
void addListToDB(struct Playlist*);
void addExisitedListToDB(struct Playlist*, int);
int removeListFromDB(int);
void addSongToDB(struct Song*);
char* readLine(int);
void writeLine(int, char*, int);
void update();
int loadSongsFromSD(char*);
int loadListsFromSD();
int openFileFromSD(char*);
char* getListNameFromListFile(char*);


#endif /* DATABASE_H_ */
