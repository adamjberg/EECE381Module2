/*
 * Database.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "Global.h"
struct database db;

struct database {
	struct Playlist* playlists;
	struct Playlist* curr_playlist;
	int num_of_lists;
	struct Song** songs;
	struct Song* curr_song;
	struct Song* next_song;
	struct Song* prev_song;
	int num_of_songs;

	struct Cache* cache;
};

void initDatabase();
struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
int loadListFromSD(char*);
int addListToSD(char* filename, struct Playlist*);
void addListToDB(struct Playlist*);
int removeListFromDB(char*);
char* readLine(int);
void writeLine(int, char*, int);
void update();
#endif /* DATABASE_H_ */
