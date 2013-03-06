/*
 * Command.h
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "Global.h"

void pause();
void play(char*, int);
void setVolume(int);
void seek(int);
void next();
void prev();
void createPlaylist(char*);
void modifyPlaylistName(char*, char*);
void shuffle(char*);
void addSongToPlaylist(char*, char*);
void moveSongToIndex(char*, int, char*);
void removeSongFromPlaylist(char*, char*);
void removeList(char*);
void play_playlist(char*);
void repeatPlaylist(char*);
void repeatCurrentSong();

void reverse();
#endif /* COMMAND_H_ */
