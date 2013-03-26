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

void syncSetVol(int, int);
void setVolume(int, int);

void seek(int);

void syncNext(int);
void next(int);

void syncPrev(int);
void prev(int);

void syncCreatePlaylist(char*);
void createPlaylist(char*);

void syncCreateExisitedPlaylist(char*, int, int);
void createExisitedPlaylist(char*, int, int);

void syncCreateSong(char*, int);
void createSong(char*, int);

void syncSelectList(int);
void selectList(int);

void syncDBFinish();

void syncAddSongToList(int, int);
void addSongToList(int, int);

void syncAddExisitedSongToList(int, int);

void syncRemoveSongFromList(int, int);
void removeSongFromList(int, int);

void syncUpdatePos(int, int, int);

void modifyPlaylistName(int, char*);
void shuffle(int);

void moveSongToIndex(char*, int, char*);
void removeList(int);
void play_playlist(int);
void repeatPlaylist(int);
void repeatCurrentSong();

void reverse();
#endif /* COMMAND_H_ */
