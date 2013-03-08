/*
 * Command.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Command.h"

struct Command* initCmd() {
	struct Command* this = (struct Command*)malloc(sizeof(struct Command));

	return this;
}

//pause current play song
void pause() {

}
void play(char* song_name, int pos) {
	struct Song* aSong = querySongByName(song_name);
}
void setVolume(int vol) {

}
void seek(int pos) {

}
void next() {

}
void prev() {

}
void createPlaylist(char* listname) {

}
void modifyPlaylistName(char* listname, char* new_listname) {
	struct Playlist* pl = queryListByName(listname);
	pl->list_name = new_listname;

}
void shuffle(char* listname) {

}
void addSongToPlaylist(char* song, char* listname) {

}
void moveSongToIndex(char* song, int index, char* listname) {

}
void removeSongFromPlaylist(char* song, char* listname) {

}
void removeList(char* listname) {

}
void play_playlist(char* listname) {

}
void repeatPlaylist(char* listname) {

}
void repeatCurrentSong() {

}

void reverse() {

}
