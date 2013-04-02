/*
 * Database.c
 *
 *  Created on: 2013-03-06
 *      Author: danny
 */
#include "Database.h"

void initDatabase() {
	db.curr_song_id = 1;
	db.curr_playlist_id = 0;
	db.num_of_lists = 0;
	db.num_of_songs = 0;
	db.avail_list_index = initQueue();
	memset(db.curr_song_ids, 0, MAX_SONGS_MIX*sizeof(int));
	db.total_songs_playing = 0;
	db.used_memory = 0;
	int i;
	int* temp;
	db.used_list_index[0] = 1; //index 0 should never get used
	db.index_list_order[0][0] = 0;
	db.index_list_song[0][0] = 0;
	db.playlists[0] = NULL;
	db.songs[0] = NULL;
	db.isListRepeated = 0;
	for(i = 1; i < MAX_LISTS; i++) {
		temp = (int*)malloc(sizeof(int));
		*temp = i;
		enqueue(db.avail_list_index, temp);
		db.used_list_index[i] = 0;
		db.index_list_order[i][0] = 0;
		db.index_list_song[i][0] = 0;
		db.playlists[i] = NULL;
		db.songs[i] = NULL;
	} temp = NULL;
	loadListsFromSD();
	loadSongsFromSD();
	preloadSongsToPlaylist();
}

void update() {
	int i = 0, j = 0;
	for(i = 0; i < db.num_of_lists; i++) {
		while(db.used_list_index[++j] != 1);
		if(db.playlists[j] == NULL) {
			printf("update error\n");
			return;
		}
		syncCreateExisitedPlaylist(db.playlists[j]->list_name,
									db.playlists[j]->num_of_songs,
									db.playlists[j]->id);

	}
	for(i = 1; i <= db.num_of_songs; i++) {
		syncCreateSong(db.songs[i]->song_name, db.songs[i]->size);
	}

	for(i = 1; i <= db.num_of_lists; i++){
		if (db.playlists[i] != NULL){
			for(j = 1; j <= db.playlists[i]->num_of_songs; j++){
				syncAddExisitedSongToList(i, db.index_list_order[i][j]);
			}
		}
	}
	syncSelectList(db.curr_playlist_id);
	syncDBFinish();
}
/*
 * Query the list with a given play list name
 * return the first list founded with the same name, NULL otherwise
 */
struct Playlist* queryListByName(char* list_name) {
	if(list_name == NULL || db.num_of_lists == 0) return NULL;
	int i = 0, j = 1;
	int size = db.num_of_lists;
	while(i < size) {
		if(db.used_list_index[j] == 1) {
			//make sure current index is in used
			if(db.playlists[j] == NULL) { //double check
				printf("querry list by name error\n");
				return NULL;
			}
			if(strcmp(list_name, db.playlists[j]->list_name) == 0) {
				return db.playlists[j];
			}
			i++;
		} j++;
	}
	return NULL;
}

struct Song* querySongByName(char* song_name) {
	if(song_name == NULL) return NULL;
	int i;
	int size = db.num_of_songs;
	for(i = 1; i <= size; i++) { // start from 1 since 0 always null
		if(strcmp(song_name, db.songs[i]->song_name) == 0) {
			return db.songs[i];
		}
	}
	return NULL;
}
/*
 * Add a song to the database
 */
void addSongToDB(struct Song* song) {
	db.num_of_songs++;
	setSongId(song, db.num_of_songs);
	db.songs[db.num_of_songs] = song;
}
/*
 * Add a playlist to the database; id will be automatically assigned
 */
void addListToDB(struct Playlist* playlist) {
	if(playlist == NULL || db.avail_list_index->size <= 0) return;
	db.num_of_lists++;
	int* index = dequeue(db.avail_list_index);
	db.used_list_index[*index] = 1;
	setListId(playlist, *index);
	db.playlists[*index] = playlist;
	int i;
	for(i = 0; i < MAX_SONGS; i++) {
		db.index_list_order[*index][i] = 0;
		db.index_list_song[*index][i] = 0;
	}
	free(index);
	index = NULL;
}
/*
 * Add a playlist to the database by giving its id
 * adding operation will failed if the id has been used already
 */
void addExisitedListToDB(struct Playlist* playlist, int id) {
	if(playlist == NULL || db.avail_list_index->size <= 0
			|| id == 0 || db.used_list_index[id] == 1) return;
	int* index = dequeueValue(db.avail_list_index, id);
	db.num_of_lists++;
	db.used_list_index[*index] = 1;
	setListId(playlist, *index);
	db.playlists[*index] = playlist;
	int i;
	for(i = 0; i < MAX_SONGS; i++) {
		db.index_list_order[*index][i] = 0;
		db.index_list_song[*index][i] = 0;
	}
	free(index);
	index = NULL;
}
/*
 * Remove a playlist from the database
 * return 0 if list is removed, -1 if cannot find list in the database
 */
int removeListFromDB(int list_id) {
	if(db.used_list_index[list_id] == 0 || list_id <= 0) return -1;
	killPlaylist(&(db.playlists[list_id]));
	db.playlists[list_id] = NULL;
	db.used_list_index[list_id] = 0;
	int* temp = (int*)malloc(sizeof(int));
	*temp = list_id;
	enqueue(db.avail_list_index, temp);
	int i;
	for(i = 0; i < MAX_SONGS; i++) {
		db.index_list_order[list_id][i] = 0;
		db.index_list_song[list_id][i] = 0;
	}
	db.num_of_lists--;
	temp = NULL;
	return 0;
}

/*
 * A helper function that read a line in a text file, require file pointer and does not close the file
 * Assumption has made that a line does not go over 100 characters
 */
int readLine(int file_pointer, char* line) {
	if(line == NULL) return -1;
	int i = -1;

	do {
		i++;
		if((line[i] = alt_up_sd_card_read(file_pointer)) < -1) {
			return -2;
		}
		if(i == 0 && line[i] == '\n') //this is to skip an empty line if any
			line[i] = alt_up_sd_card_read(file_pointer);
		if(i >= 500) {
			printf("WARNNING! reading a line that contains more than 500 characters\n");
			break;
		}
	} while(line[i] != '\n' && line[i] != '\r' && line[i] != 0 && line[i] != -1);
	line[i] = 0;
	if(i == 0)
		return -1;
	return 0;
}

/*
 * A helper function that write a segment of data to SD card and write a LINE FEED at the end
 * it does not close file pointer and require an input of file pointer
 */
void writeLine(int file_pointer, char* data, int size) {
	if(data == NULL) return;
	int i;
	for( i = 0; i < size; i++ )
	{
		if(data[i] == 0) break;
		if(!alt_up_sd_card_write(file_pointer, data[i])){
			printf("Write a character to SD card failed.\n");
		}
	}
	if(!alt_up_sd_card_write(file_pointer, '\r')) {
		printf("Write a new line ASCII failed\n");
	}
	if(!alt_up_sd_card_write(file_pointer, '\n')) {
		printf("Write a new line ASCII failed\n");
	}
}
/*
 * Helper function to open a file from SDCard
 * */
int openFileFromSD(char* file){
	int fileHandler;
	if (file == NULL) return -2;
	fileHandler = alt_up_sd_card_fopen(file, false);
	if (fileHandler < 0){
		if ((fileHandler = alt_up_sd_card_fopen(file, true)) < 0){
			alt_up_sd_card_fclose(fileHandler);
			return -1;
		}
	}
	return fileHandler;
}

/*
 * Saves all playlists with their information to
 * LISTS.TXT in SDCard.
 * Return value: 0 means success, -1 means error in fopen/fclose
 */
int saveListsToSD() {
	int fileHandler;
	if ((fileHandler = openFileFromSD(LISTFILE)) < 0){
		printf("Error opening file in addListToSD.\n");
		return -1;
	}
	int i, size;
	char* strToStore = (char*)malloc(sizeof(char)*99);
	for(i = 1; i < MAX_LISTS; i++){
		if (db.used_list_index[i] == 0){
			size = sprintf(strToStore, "%d %d %d", i, 0, 0);
		} else {
			size = sprintf(strToStore,"%d %s %d", db.playlists[i]->id, db.playlists[i]->list_name, db.playlists[i]->num_of_songs);
		}
		writeLine(fileHandler, strToStore, size);
	}
	free(strToStore);
	strToStore = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

/*
 * Reads LISTS.TXT and load all playlists into the DB.
 */
int loadListsFromSD() {

	int fileHandler;
	if ((fileHandler = openFileFromSD(LISTFILE)) < 0){
		printf("Loading list error!\n");
		return -1;
	}
	char* line = NULL;
	int i = 0;
	int stats = 0;
	for(i = 0; i < 50; i++) {
		line = (char*)malloc(sizeof(char)*501);
		stats = readLine(fileHandler, line);

		if(stats == -2) {
			alt_up_sd_card_fclose(fileHandler);
			printf("Cannot read the list file, file closed.\n");
			free(line);
			line = NULL;
			return -1;
		} else if(stats == -1) {
			free(line);
			line = NULL;
			break;
		} else {
			createPlaylistFromTxt(line);
			free(line);
			line = NULL;
		}
	}
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
		return -1;
	}
	return 0;
}

/*
 * Parse a string containing playlist information
 * into a playlist object and add it to DB.
 * */
void createPlaylistFromTxt(char* line){
	int i = 0, j = 0;
	char id[4];
	char listName[100];
	char numOfSong[4];
	while((id[j++] = line[i++]) != ' ');
	id[j] = 0; j = 0;
	while((listName[j++] = line[i++]) != ' ');
	listName[j-1] = 0; j = 0;
	if(listName[0] == '0') return;
	while((numOfSong[j++] = line[i++]) != 0);
	createExisitedPlaylist(listName, atoi(numOfSong), atoi(id));
}

/*
 * Find all .WAV files in SDCard and return their names in an
 * array of strings.
 * Return: array containing names of all .WAV files in SDCard
 * */
char** getSongsFromSD(){
	int fileStatus = 0;
	char fileName[15];
	int numOfSounds = 0;

	// songNames is array of strings that store song names.
	char** songNames = malloc(MAX_SONGS *sizeof(char*));

	memset(fileName, 0 , sizeof(fileName));
	fileStatus = alt_up_sd_card_find_first("", fileName);
	if (fileStatus != 0){
		printf("ERROR: updateSongsFromSD.\n");
		return NULL;
	}
	while (fileStatus != -1){
		if (strstr(fileName, ".WAV") != NULL){
			songNames[numOfSounds] = malloc(20 * sizeof(char));
			strcpy(songNames[numOfSounds], fileName);
			//createSong(fileName, 0);
			numOfSounds++;
		} else if(strstr(fileName, ".MP3") != NULL) {
			songNames[numOfSounds] = malloc(20 * sizeof(char));
			strcpy(songNames[numOfSounds], fileName);
			//createSong(fileName, 0);
			numOfSounds++;
		}
		memset(fileName, 0 , sizeof(fileName));
		fileStatus = alt_up_sd_card_find_next(fileName);
	}
	// set ending condition, NULL means end of the array
	songNames[numOfSounds] = NULL;
	return songNames;
}

void updateSongToSD() {
	int i;
	char temp[25];
	int len, filepointer;
	if ((filepointer = openFileFromSD(SONGFILE)) < 0){
		printf("Loading list error!\n");
		return;
	}
	for(i = 1; i <= db.num_of_songs; i++) {
		len = sprintf(temp, "%d %s %d", i, db.songs[i]->song_name, getLength(db.songs[i]));
		writeLine(filepointer, temp, len);
	}
	if (!alt_up_sd_card_fclose(filepointer)){
		printf("File is not closed properly.\n");
	}
}
/*
 * Get all songs from SONGS.TXT and add it to DB.
 * It also checks if there exist any songs in SDCard that are newly
 * added and has not been written to SONGS.TXT.
 * Those songs will also get added to DB.
 * */

int getAndUpdateSongsFromTxt(char** arrFromSDFiles){
	int fileHandler;
	if ((fileHandler = openFileFromSD(SONGFILE)) < 0){
		printf("Reading songs from SONGS.TXT error!\n");
		return -1;
	}

	// add songs from
	char** songNames = malloc(MAX_SONGS *sizeof(char*));
	char* line = NULL;
	char temp[501];
	char substr[501];
	int start = 0, end, i, iteration;
	int numOfSongs = 0, fileStats = 0;
	while (numOfSongs < MAX_SONGS){
		line = (char*)malloc(sizeof(char)*501);
		if((fileStats = readLine(fileHandler, line)) == -2) {
			printf("File SONGS.TXT cannot be read, reopening...\n");
			if (!alt_up_sd_card_fclose(fileHandler)){
				printf("File is not closed properly.\n");
			}
			free(line);
			line = NULL;
			return -1;
		} else if(fileStats == -1) {
			free(line);
			line = NULL;
			break;
		} else {
			memset(temp, 0 , sizeof(temp)/sizeof(temp[0]));
			strcpy(temp, line);
			iteration = 0;
			for (i = 0; i < strlen(temp); i++){
				if (temp[i] == ' '){
					if (iteration == 0){
						start = i+1;
						iteration++;
					} else {
						end = i+1;
						memset(temp, 0 , sizeof(temp)/sizeof(temp[0]));
						strncpy(temp, line+start, end-start-1);
						temp[strlen(temp)] = '\0';
						if (temp[0] != '0'){
							memset(substr, 0 , sizeof(substr)/sizeof(substr[0]));
							strcpy(substr, line+i+1);
							createSong(temp, strtol(substr, NULL, 10));
							songNames[numOfSongs] = malloc(20*sizeof(char));
							strcpy(songNames[numOfSongs], temp);
						}
						break;
					}
				}
			}
			numOfSongs++;
		}
		free(line);
		line = NULL;
	}
	//set an end sign, null means end of the array.
	songNames[numOfSongs] = NULL;

	char strToStore[20];
	int size = 0;
	// update the txt file and db
	int sdFiles_index = 0;
	int txtFiles_index = 0;

	while (arrFromSDFiles[sdFiles_index] != NULL){
		for (txtFiles_index = 0; txtFiles_index < MAX_SONGS; txtFiles_index++){
			if (songNames[txtFiles_index] == NULL){
				memset(strToStore, 0 , sizeof(strToStore)/sizeof(strToStore[0]));
				while((size = sprintf(strToStore, "%d %s 0", numOfSongs+1, arrFromSDFiles[sdFiles_index])) <= 0);
				writeLine(fileHandler, strToStore, size);
				createSong(arrFromSDFiles[sdFiles_index], 0);
				numOfSongs++;
				break;
			} else {
				if (strcmp(songNames[txtFiles_index], arrFromSDFiles[sdFiles_index]) == 0)
					break;
			}
		}
		sdFiles_index++;
	}
	memset(&strToStore[0], 0 , sizeof(strToStore)/sizeof(strToStore[0]));
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("File is not closed properly.\n");
	}

	int j = 0;
	while (arrFromSDFiles[j] != NULL){
		free(arrFromSDFiles[j]);
		arrFromSDFiles[j++] = NULL;
	}
	free(arrFromSDFiles);
	arrFromSDFiles = NULL;
	j = 0;
	while (songNames[j] != NULL){
		free(songNames[j]);
		songNames[j++] = NULL;
	}
	free(songNames);
	songNames = NULL;
	return 0;
}
/*
 * Load songs from SDCARD and added it to DB
 * This also updates the SONGS.TXT if the text file misses
 * any song titles.
 * */
void loadSongsFromSD(){
	char** sdsongs = NULL;
	sdsongs = getSongsFromSD();
	//getAndUpdateSongsFromTxt(sdsongs);
	while(getAndUpdateSongsFromTxt(sdsongs) < 0);

}

/*
 * This function check if the song is currently playing in the SoundMixer
 */
int isCurrPlaying(int song_id) {
	int i = 0;
	while(db.curr_song_ids[i] != 0) {
		if(db.curr_song_ids[i++] == song_id) {
			return i-1;
		}
	} return -1;
}

void removeCurrPlaying(int index) {
	int j;
	db.total_songs_playing--;
	//stopSound(db.songs[db.curr_song_ids[index]]->sound);
	for(j = index; j <= db.total_songs_playing; j++) {

		db.curr_song_ids[j] = db.curr_song_ids[j+1];
	}
}

/*
 * Saves all changes related to songs and playlists to SDCard
 * Called before quitting the app
 * */
void saveAllUpdatesToSDBeforeQuits(){
	saveListChangesToSD(0);
	saveListChangesToSD(1);
}

/*
 * Reads the content of db.index_list_song/db.index_list_order
 * (depends on param) and writes it to corresponding file
 * if param is 0, then function will read from db.index_list_song and
 * updates LSONG.TXT.
 * if param is 1, then function will read from db.idnex_list_order and
 * updates LORDER.TXT
 *
 * LSONG.TXT will contain information regarding which songs belong to
 * each playlist.
 * LORDER.TXT will contain have id of each playlist along with their songs
 * in correct order.
 * */
// TODO: simplify this function :(
void saveListChangesToSD(int param){

	int fileHandler;
	char* fileName = NULL;

	if (param == 0){
		fileName = LISTSONGFILE;
	} else if (param == 1){
		fileName = LISTORDERFILE;
	} else {
		printf("Incorrect parameter value.\n");
		return;
	}
	if ((fileHandler = openFileFromSD(fileName)) < 0){
		printf("Can't open LSONG.TXT error!\n");
		return;
	}
	int i, j;
	char line[1024];
	char temp[5];
	char* emptyPlaylist = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";

	if (param == 0)
	{
		for (i = 1; i < MAX_LISTS-1; i++){
			if (db.playlists[i] != NULL) {
				sprintf(line, "%d", db.playlists[i]->id);
				for (j = 1; j < MAX_SONGS; j++){
					memset(temp, 0 , sizeof(temp)/sizeof(temp[0]));
					sprintf(temp, " %d", db.index_list_song[i][j]);
					if (strlen(line) >= 1024) {
						printf("this line has more than 1024 words\n");
						emptyPlaylist = NULL;
						return;
					}
					strncat(line, temp, strlen(temp));
				}
				line[strlen(line)] = '\0';
				writeLine(fileHandler, line, strlen(line));
				memset(line, 0, sizeof(line)/sizeof(line[0]));
			} else {
				writeLine(fileHandler, emptyPlaylist, strlen(emptyPlaylist));
			}
		}
	}else{
		for (i = 1; i < MAX_LISTS-1; i++){
			if (db.playlists[i] != NULL) {
				sprintf(line, "%d", db.playlists[i]->id);
				for (j = 1; j < MAX_SONGS; j++){
					memset(temp, 0 , sizeof(temp)/sizeof(temp[0]));
					sprintf(temp, " %d", db.index_list_order[i][j]);
					if (strlen(line) >= 1024) {
						printf("this line has more than 1024 words\n");
						emptyPlaylist = NULL;
						return;
					}
					strncat(line, temp, strlen(temp));
				}
				line[strlen(line)] = '\0';
				writeLine(fileHandler, line, strlen(line));
				memset(line, 0, sizeof(line)/sizeof(line[0]));
			} else {
				writeLine(fileHandler, emptyPlaylist, strlen(emptyPlaylist));
			}
		}
	}
	emptyPlaylist = NULL;
	fileName = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("file is not closed properly.\n");
		return;
	}
}

/*
 * After all playlists are loaded to database,
 * initialize each playlist with the correct songs that belong to
 * that playlist.
 * This information is stored in LORDER.TXT
 * */
void preloadSongsToPlaylist(){
	int fileHandler;
	if ((fileHandler = openFileFromSD(LISTORDERFILE)) < 0){
		printf("Can't open file LORDER.TXT error!\n");
		return;
	}

	char* line = NULL;
	int i, fileStats;

	i = 0;
	while (i < 50){
		line = (char*)malloc(sizeof(char)*501);
		fileStats = readLine(fileHandler, line);
		if (fileStats == -2){
			printf("LORDER.TXT cannot be read, reopening...\n");
			if (!alt_up_sd_card_fclose(fileHandler)){
				printf("File is not closed properly.\n");
			}
			free(line);
			line = NULL;
			break;
		} else if (fileStats == -1){
			//printf("Quit after iteration %d\n", i);
			free(line);
			line = NULL;
			break;
		} else {
			initializeListWithSongs(line);
			free(line);
			i++;
		}
	}
	line = NULL;
	if (!alt_up_sd_card_fclose(fileHandler)){
		printf("file is not closed properly.\n");
		return;
	}
}

/*
 * Parses a string that contains playlist & songs that
 * belong to that playlist, load all songs to the playlist
 * in correct order.
 * */
void initializeListWithSongs(char* input){

	char line[1024];
	char temp[1024];
	int i, list_id = 0, song_id, cursorPos = 0;
	int iteration = 0;
	int order = 1;
	memset(line, 0, sizeof(line)/sizeof(line[0]));
	strcpy(line, input);
	for (i = 0; i < strlen(input); i++){
		if (line[i] == ' '){
			memset(temp, 0, sizeof(temp)/sizeof(temp[0]));
			if (iteration == 0){
				cursorPos = i+1;
				strncpy(temp, line, cursorPos);
				iteration++;
				list_id = strtol(temp, NULL, 10);
				//printf("List id is %d\n", list_id);
				if (list_id == 0){ break;}
			} else {
				strncpy(temp, line+cursorPos, i-cursorPos+1);
				cursorPos = i;
				song_id = strtol(temp, NULL, 10);
				if (song_id == 0) { break;}
				db.index_list_song[list_id][song_id] = order;
				db.index_list_order[list_id][order] = song_id;
				order++;
			}
		}
	}
}


