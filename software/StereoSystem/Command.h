/*
 * Command.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "Global.h"

struct Command {
	int cmd_index;
	int num_of_parameters;
	int* parameters_size;
	char** parameters;
};

struct Command* initCmd(int, int, char**);
void killCmd(struct Command**);

void syncPlay(int, int, int);
void play(int, int, int);

void syncPause(int);
void pause(int);

void syncStop();
void stop();

void setVolume(int);
void seek(int);
void next();
void prev();

void syncCreatePlaylist(char*);
void createPlaylist(char*);

void syncCreateExisitedPlaylist(char*, int, int);
void createExisitedPlaylist(char*, int, int);

void syncCreateSong(char*, int);
void createSong(char*, int);

void syncSelectList(int);
void selectList(int);

void syncDBFinish();

void modifyPlaylistName(int, char*);
void shuffle(int);
void addSongToPlaylist(int, int);
void moveSongToIndex(char*, int, char*);
void removeSongFromPlaylist(char*, char*);
void removeList(int);
void play_playlist(int);
void repeatPlaylist(int);
void repeatCurrentSong();

void reverse();
#endif /* COMMAND_H_ */
