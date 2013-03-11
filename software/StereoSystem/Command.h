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

void syncPlay(char*, int);
void play(char*, int);
void syncPause();
void pause();
void syncStop();
void stop();
void setVolume(int);
void seek(int);
void next();
void prev();
void createPlaylist(char*);
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
