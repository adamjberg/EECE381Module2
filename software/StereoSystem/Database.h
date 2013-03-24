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
#define LISTORDERFILE "LORDER.TXT"
#define LISTSONGFILE "LSONG.TXT"
#define MAX_LISTS 51
#define MAX_SONGS 101
#define MAX_SONGS_MIX 11
#define MAX_CACHE_MEMORY 1500000
struct database db;

struct database {
	int curr_song_id;
	struct Playlist* playlists[MAX_LISTS];
	int curr_playlist_id;
	int num_of_lists;
	struct Song* songs[MAX_SONGS];
	int curr_song_ids[MAX_SONGS_MIX];
	int total_songs_playing;
	int num_of_songs;
	int index_list_song[MAX_LISTS][MAX_SONGS];
	int index_list_order[MAX_LISTS][MAX_SONGS];
	struct Queue* avail_list_index;
	int used_list_index[MAX_LISTS];
	int used_memory;
};

void initDatabase();
struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
void addListToDB(struct Playlist*);
void addExisitedListToDB(struct Playlist*, int);
int removeListFromDB(int);
void addSongToDB(struct Song*);
int readLine(int, char*);
void writeLine(int, char*, int);
void update();
void loadSongsFromSD();
int loadListsFromSD();
int saveListsToSD();
int openFileFromSD(char*);
char** getSongsFromSD();
int getAndUpdateSongsFromTxt(char**);
void createPlaylistFromTxt(char*);
void updateSongToSD();
int isCurrPlaying(int);
void removeCurrPlaying(int);
void saveAllUpdatesToSDBeforeQuits();
void saveListChangesToSD(int);
void preloadSongsToPlaylist();
void initializeListWithSongs(char*);
#endif /* DATABASE_H_ */
