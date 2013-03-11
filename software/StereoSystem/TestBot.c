/*
 * TestBot.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "TestBot.h"


void dBTester() {

	//createPlaylist("List1");
	/*printf("---- Test Adding Song To DB ----\n");
	struct Song* s1 = initSong("song1");
	addSongToDB(s1);
	printf("Good Test, should have no error after this line.\n");
	addSongToPlaylist("song1", "List1");
	struct Song* s2 = initSong("song2");
	printf("Bad Tests, expect to have 2 errors after this line.\n");
	addSongToPlaylist("song2", "List1");
	addSongToPlaylist("song1", "List2");
	addSongToDB(s2);*/
	int i = loadSongsFromSD("SNG.TXT");
	if (i != 0){
		printf("WARNING: Program does not load songs from SDCard properly.\n");
	}


	int file_pointer;
	char* line;
	file_pointer = alt_up_sd_card_fopen("SNG.TXT", false);
	if (file_pointer < 0){
		if ((file_pointer = alt_up_sd_card_fopen("SNG.TXT", true)) < 0){
			alt_up_sd_card_fclose(file_pointer);
			printf("File cannot be opened.\n");
		}
	} else {
		int j = 0;
		for (j; j < 8; j++){
			line = readLine(file_pointer);
			printf("%s\n", line);
		}
	}

/*	struct Playlist* pl = initPlaylist();
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
		printf("remove list2 failed\n");*/

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
