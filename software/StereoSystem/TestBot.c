/*
 * TestBot.c
 *
 *  Created on: 2013-03-07
 *      Author: danny
 */
#include "TestBot.h"

void graphicTester() {
	/*struct Image* picture;
	int i;
	for( i = 0; i < 1000; i ++) {
		while((picture = loadSDImage("WIN.BMP")) == NULL);
		draw(i%50, 0, picture);
		killImage(picture);
	}*/

	// click one playlist
	//playlistButtonCollide(mouse->frame->elements[3]->buttons[1]);

	//drawAllSongsInList();
	/*int i;
	for (i = 0; i < 5000; i++){
		drawAllSongsInList(5);
		printf("Iteration %d\n", i);
	}*/
	/*int j;
	for (i = 0; i < 50; i++){
		for (j = 0; j < 20; j++){
			highlightSongWithID(j);
			printf("highlight iteration %d\n", i);
		}
	}*/
}
void dBTester() {

	/*int i, j;
	for(i = 0; i < 50000; i++) {
		loadListsFromSD();
		for(j = 1; j < MAX_LISTS; j++) {
			if(db.used_list_index[j] == 1)
				//printf("Removing list\n");
				removeListFromDB(j);
		}
		printf("Iteration %d\n", i);
	}*/

	// test getSongsFromSD()
	// TODO: check if it contains win, lose, lone, boing, dingding
	/*char** songs = NULL;
	int i = 0;
	songs = getSongsFromSD();
	while (songs[i] != NULL){
		printf("Song is %s\n", songs[i]);
		i++;
	}*/

	// test getSongsFromSD()
	// TODO: should pass 50k, test with many wav songs
	/*char** songs = NULL;
	int i, j;
	for (i = 0; i < 50000; i++){
		songs = getSongsFromSD();
		j = 0;
		while (songs[j] != NULL){
			free(songs[j]);
			songs[j] = NULL;
			j++;
		}
		free(songs);
		songs = NULL;
		printf("Iteration is %d\n", i);
	}*/

	// test getSongsFromTxt that has createSong
	// TODO: CHECK DB, should contain your song
	/*char** songs = NULL;
	songs = getSongsFromTxt();*/

	// test content of checkTxtFile
	// TODO: check output in console, should show dingding etc
	/*char** txtsongs = NULL;
	char** sdsongs = NULL;
	char** diff = NULL;
	txtsongs = getSongsFromTxt();
	sdsongs = getSongsFromSD();
	diff = checkTxtFile(sdsongs, txtsongs);
	int j = 0;
	while (diff[j] != NULL){
		printf("diff element is %s\n", diff[j]);
		j++;
	}*/

	// test checkTxtFile
	// TODO: should pass 50000 times: take soo long, test again
	/*char** txtsongs = NULL;
	char** sdsongs = NULL;
	char** diff = NULL;
	int i, j;
	txtsongs = getSongsFromTxt();
	for (i = 0; i < 50000; i++){
		sdsongs = getSongsFromSD();
		diff = checkTxtFile(sdsongs, txtsongs);
		j = 0;
		while (sdsongs[j] != NULL){
			free(sdsongs[j]);
			sdsongs[j] = NULL;
			j++;
		}
		free(sdsongs);
		sdsongs = NULL;

		j = 0;
		while(diff[j] != NULL){
			free(diff[j]);
			diff[j] = NULL;
			j++;
		}
		free(diff);
		diff = NULL;
		printf("Iteration is %d\n", i);
	}*/



	//TODO: check getAndUpdateSongsFromTxt with writeLine
	/*char** sdsongs = NULL;
	sdsongs = getSongsFromSD();
	getAndUpdateSongsFromTxt(sdsongs);*/

	// TODO: check updateSongsTxt in 50k loop, without creating songs
	// file should be the same at the end.. (overwrite)
	/*char** txtsongs = NULL;
	char** sdsongs = NULL;
	char** diff = NULL;
	txtsongs = getSongsFromTxt();
	int i,j,k;
	for (i = 0; i < 50000; i++){
		sdsongs = getSongsFromSD();
		diff = checkTxtFile(sdsongs, txtsongs);
		updateSongsTxt(diff);
		j = 0;
		while (sdsongs[j] != NULL){
			free(sdsongs[j]);
			sdsongs[j] = NULL;
			j++;
		}
		free(sdsongs);
		sdsongs = NULL;

		j = 0;
		while(diff[j] != NULL){
			free(diff[j]);
			diff[j] = NULL;
			j++;
		}
		free(diff);
		diff = NULL;
		printf("Iteration is %d\n", i);
	}

	// TODO: check updateSongsTxt with createSong
	// check your db and txt file :]
	// remove songs from txt file first..
	char** txtsongs = NULL;
	char** sdsongs = NULL;
	char** diff = NULL;
	txtsongs = getSongsFromTxt();
	sdsongs = getSongsFromSD();
	diff = checkTxtFile(sdsongs, txtsongs);
	updateSongsTxt(diff);
*/

	//loadSongsFromSD();
	/*int fileHandler;
	if ((fileHandler = openFileFromSD(SONGFILE)) < 0){
		printf("Reading songs from SONGS.TXT error!\n");
	}
	char** song_names = (char**)malloc(sizeof(char*)*MAX_SONGS);
	int i = 0, fileStats = 0;
	song_names[i] = (char*)malloc(sizeof(char)*100);
	while((fileStats = readLine(fileHandler, song_names[i++])) != -1) {
		if(fileStats == -2) {
			printf("File cannot be read, reopening...\n");
			if (!alt_up_sd_card_fclose(fileHandler)){
				printf("File is not closed properly.\n");
			}
			break;
		}
		song_names[i] = (char*)malloc(sizeof(char)*100);
	}
	while(i > 0) {
		free(song_names[--i]);
		song_names[i] = NULL;
	} free(song_names);
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
	}*/

	//addSongToList(1,3);
	//addSongToList(1,4);
	//addSongToList(2,1);
	//addSongToList(2,2);
	//preloadSongsToPlaylist();
	//saveAllUpdatesToSDBeforeQuits();

	/*int i, j;
	for (i = 0; i < 50000; i++){
		addExistingSongToPlaylist(1, 3);
		printf("Iteration %d\n", i);
	}*/
	//	preloadSongsToPlaylist();
	//	printf("Iteration %d\n", i);
	//}
}

void dbTester2() {
/*	int* tempVal = dequeueValue(db.avail_list_index, 1);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 10);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 5);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 25);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 3);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 3);
	enqueue(db.avail_list_index, (void*)tempVal);
	tempVal = dequeueValue(db.avail_list_index, 7);
	enqueue(db.avail_list_index, (void*)tempVal);


	struct Playlist* pl = initPlaylist("list1");
	//setListName(pl, "list1");
	struct Playlist* pl1 = initPlaylist("list2");
	//setListName(pl1, "list2");
	struct Playlist* pl2 = initPlaylist("list3");
	//setListName(pl2, "list3");

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
	int i;*/
/*	for(i = 0; i < 1000; i ++) {
		al = initPlaylist("list4");
		//setListName(al, "list4");
		al1 = initPlaylist("list5");
		//setListName(al1, "list5");
		al2 = initPlaylist("list6");
		//setListName(al2, "list6");

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
	}*/

/*	for(i = 0; i < 1000; i ++) {
		al = initPlaylist("list4");
		al1 = initPlaylist("list5");
		al2 = initPlaylist("list6");

		addExisitedListToDB(al, 8);
		addExisitedListToDB(al1, 7);
		addExisitedListToDB(al2, 15);


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

	createExisitedPlaylist("list1", 0, 1);
	createExisitedPlaylist("list2", 0, 5);
	createExisitedPlaylist("list3", 0, 2);
	createExisitedPlaylist("list4", 0, 15);
	createExisitedPlaylist("list5", 0, 11);
	createExisitedPlaylist("list1", 0, 7);
	createExisitedPlaylist("list2", 0, 25);
	createExisitedPlaylist("list3", 0, 12);
	createExisitedPlaylist("list4", 0, 35);
	createExisitedPlaylist("list5", 0, 19);*/



	/*createSong("new song 1");
	createSong("new song 2");
	createSong("new song 3");
	createSong("new song 4");
	createSong("new song 5");
	createSong("new song 6");*/

	int i;
	for(i = 0; i < 50000; i++) {
		loadListsFromSD();

		removeListFromDB(1);
		removeListFromDB(6);
		removeListFromDB(9);
		removeListFromDB(12);
		removeListFromDB(17);
		removeListFromDB(22);
		removeListFromDB(29);
		removeListFromDB(33);
		removeListFromDB(36);
		removeListFromDB(47);
		removeListFromDB(49);
		removeListFromDB(50);
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

int soundTester(int i) {
	if(isCurrPlaying(i-1) < 0) {
		if(i > db.num_of_songs)
			i = 1;
		syncPlay(i, 100, 0);
		//syncPlay(1, 100, 0);
		//playSong(db.songs[i], 100, 0, 0);
		//updateMixer();
		//enableAudioDeviceController();
		i++;
	}
	return i;
}
