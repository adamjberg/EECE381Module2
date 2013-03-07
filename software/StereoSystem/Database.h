/*
 * Database.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include "Global.h"
struct db db;

struct db {
	struct Playlist* playlists;
	struct Playlist* curr_playlist;
	int num_of_lists;
	struct Song** songs;
	struct Song* curr_song;
	int num_of_songs;
};

struct Playlist* queryListByName(char*);
struct Song* querySongByName(char*);
int loadPlaylist(char*);
int savePlaylist(struct Playlist*);
char* readLine(int);
void writeLine(int, char*, int);
void addPlaylist(struct Playlist*);
void update();
#endif /* DATABASE_H_ */
