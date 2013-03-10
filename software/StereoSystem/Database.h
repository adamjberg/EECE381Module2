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
struct database db;

struct database {
	struct Playlist* playlists[50];
	int curr_playlist_id;
	int num_of_lists;
	struct Song* songs[200];
	int curr_song_id;
	int next_song_id;
	int prev_song_id;
	int num_of_songs;
	int index_table[50][200];

	struct Cache* cache;
};

void initDatabase();
struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
int loadListFromSD();
int addListToSD(struct Playlist*);
void addListToDB(struct Playlist*);
int removeListFromDB(int);
void addSongToDB(struct Song*);
char* readLine(int);
void writeLine(int, char*, int);
void update();
#endif /* DATABASE_H_ */
