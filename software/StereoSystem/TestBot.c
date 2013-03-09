/*
 * TestBot.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "TestBot.h"


void dBTester() {
	struct Playlist* pl = initPlaylist();
	pl->list_name = "list1";
	pl->num_of_songs = 3;
	enqueue(pl->songs, initSong("aTestSong1"));
	enqueue(pl->songs, initSong("aTestSong2"));
	enqueue(pl->songs, initSong("aTestSong3"));

	addListToDB(pl);

	addListToSD("010.TXT", pl);

	loadListFromSD("010.TXT");
	loadListFromSD("011.TXT");

	if(removeListFromDB("list1")!=0)
		printf("remove list1 failed\n");
	if(removeListFromDB("list1")!=0)
		printf("remove list1 failed\n");
	if(removeListFromDB("List2")!=0)
		printf("remove list2 failed\n");

}

void cmdTester() {
	char* paras[8];
	paras[0] = "para1";
	paras[1] = "para21";
	paras[2] = "para32";
	paras[3] = "para43";
	paras[4] = "para5";
	paras[5] = "para6";
	paras[6] = "para7";
	paras[7] = "para8";
	struct Command* cmd1 = initCmd(23, 8, paras);

	send((void*)cmd1, CMD);
}
