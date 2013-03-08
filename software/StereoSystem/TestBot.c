/*
 * TestBot.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "TestBot.h"


void dBTester() {
	struct Playlist* pl = (struct Playlist*)malloc(sizeof(struct Playlist));
	pl->list_name = "list1";
	addListToDB(pl);
	printf("%s\n", db.playlists->list_name);
	addListToSD("010.TXT", pl);
	loadListFromSD("010.TXT");
	printf("%s\n", db.playlists->list_name);
	printf("%s\n", db.playlists->prev->list_name);
}
