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
#define MAX_SONGS_MIX 10
struct database db;

struct database {
	struct Playlist* playlists[MAX_LISTS];
	int curr_playlist_id;
	int num_of_lists;
	struct Song* songs[MAX_SONGS];
	int curr_song_ids[MAX_SONGS_MIX];
	int song_playing_size;
	int num_of_songs;
	int* index_list_song[MAX_LISTS];
	int* index_list_order[MAX_LISTS];
	struct Queue* avail_list_index;
	int used_list_index[MAX_LISTS];

	//struct Cache* cache;
};

void initDatabase();
struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
void addListToDB(struct Playlist*);
void addExisitedListToDB(struct Playlist*, int);
int removeListFromDB(int);
void addSongToDB(struct Song*);
char* readLine(int);
void writeLine(int, char*, int);
void update();
int loadSongsFromSD();
int updateSongsFromSD();
int loadListsFromSD();
int saveListsToSD();
int openFileFromSD(char*);
char* separateString(char*, char, int);


#endif /* DATABASE_H_ */
