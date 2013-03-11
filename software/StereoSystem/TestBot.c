/*
 * TestBot.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "TestBot.h"


void dBTester() {

	int j = 0;
	for (j = 0; j < 3; j++){
		createPlaylist("List1");
	}
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
	int i = loadSongsFromSD("SONGS3.TXT");
	if (i != 0){
		printf("WARNING: Program does not load songs from SDCard properly.\n");
	}
	printf("First song in DB is %s\n", db.songs[1]->song_name);

	printf("Number of songs in DB is %d.\n", db.num_of_songs);
	if (querySongByName("LOSE.WAV") != NULL){
		printf("Test1 passed.\n");
	}
	if (querySongByName("DIANA.WAV") == NULL){
		printf("Test2 passed.\n");
	}
	int k = 0;
	for (k = 1; k < db.num_of_songs; k++){
		printf("%s.\n", db.songs[k]->song_name);
	}
	/*int file_pointer;
	char* line;
	file_pointer = alt_up_sd_card_fopen("SONGS1.TXT", false);
	if (file_pointer < 0){
		if ((file_pointer = alt_up_sd_card_fopen("SNG.TXT", true)) < 0){
			alt_up_sd_card_fclose(file_pointer);
			printf("File cannot be opened.\n");
		}
	} else {
		int j = 0;
		for (j; j < 7; j++){
			line = readLine(file_pointer);
			printf("%s\n", line);
		}
	}*/

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

void dbTester2() {
	struct Playlist* pl = initPlaylist();
	setListName(pl, "list1");
	struct Playlist* pl1 = initPlaylist();
	setListName(pl1, "list2");
	struct Playlist* pl2 = initPlaylist();
	setListName(pl2, "list3");

	addListToDB(pl);
	addListToDB(pl1);
	addListToDB(pl2);
	//pl = pl1 = pl2 = NULL;
	struct Playlist* temp = queryListByName("list2");
	printf("%s is found\n", temp->list_name);
	struct Playlist* temp1 = queryListByName("list1");
	struct Playlist* temp2 = queryListByName("list3");
	if(removeListFromDB(pl->id) != 0)
		printf("sth wrong\n");
	if(removeListFromDB(pl1->id) !=0)
		printf("sth wrong\n");
	if(removeListFromDB(pl2->id) != 0)
		printf("sth wrong\n");
	//temp = temp1= temp2 = NULL;
	pl = pl1 = pl2 = NULL;
	struct Playlist* al;
	struct Playlist* al1;
	struct Playlist* al2;
	int i;
	for(i = 0; i < 1000; i ++) {
	al = initPlaylist();
	setListName(al, "list4");
	al1 = initPlaylist();
	setListName(al1, "list5");
	al2 = initPlaylist();
	setListName(al2, "list6");

	addListToDB(al);
	addListToDB(al1);
	addListToDB(al2);

	//pl = pl1 = pl2 = NULL;
	temp = queryListByName("list4");
	printf("%d %s is found\n", temp->id, temp->list_name);
	temp1 = queryListByName("list5");
	printf("%d %s is found\n", temp1->id, temp1->list_name);
	temp2 = queryListByName("list6");
	printf("%d %s is found\n", temp2->id, temp2->list_name);
	if(removeListFromDB(al->id) != 0)
		printf("sth wrong\n");
	if(removeListFromDB(al1->id) !=0)
		printf("sth wrong\n");
	if(removeListFromDB(al2->id) != 0)
		printf("sth wrong\n");
	}
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
