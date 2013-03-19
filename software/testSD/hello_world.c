/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_audio.h"
#include <alt_types.h>
#include <altera_up_sd_card_avalon_interface.h>

#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"


bool loadSDCard(alt_up_sd_card_dev* device) {
	if((device = alt_up_sd_card_open_dev("/dev/SD_Card")) != NULL) {
		if (alt_up_sd_card_is_Present()) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
				return true;
			} else {
				printf("Unknown file system.\n");
			}
		}
	}
	return false;
}
/**
 * Helper function to read multiple bytes and return the representative int value
 */
int readInt(int file_pointer, int numBytesToRead) {
	int ret = 0;
	int i;
	int bytes[numBytesToRead];
	for (i = 0; i < numBytesToRead; i++) {
		bytes[i] = alt_up_sd_card_read(file_pointer);
		ret |= (bytes[i] << (8 * i));
	}
	return ret;
}
void loadWavSound(char * filename) {
	printf("loading sound: %s\n", filename);

	//struct Sound* sound;
	int i, index = 0;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if (file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		printf("sound file open failed\n");
		return;
	}
	char temp;
	//Start reading the wav header
	while (index < 34) {
		temp = alt_up_sd_card_read(file_pointer);
		printf("%d %x\n", index, temp);
		index++;
	}

	int bytes_per_sample = readInt(file_pointer, 2) / 8;
	index += 2;

	while (index < 40) {
		temp = alt_up_sd_card_read(file_pointer);
		printf("%d %x\n", index, temp);
		index++;
	}

	int len = readInt(file_pointer, 4) / bytes_per_sample;

	printf("length: %u\n", len);

	for (i = 0; i < len; i++) {
		readInt(file_pointer, bytes_per_sample);
	}

	printf("Sound loading complete\n");

	alt_up_sd_card_fclose(file_pointer);
}

/*
 * A helper function that read a line in a text file, require file pointer and does not close the file
 * Assumption has made that a line does not go over 100 characters
 */
int readLine(int file_pointer, char* line) {
	if(line == NULL) return -1;
	//char temp[100];
	int i = -1;

	do {
		i++;
		if((line[i] = alt_up_sd_card_read(file_pointer)) < -1) {
			return -2;
		}
		if(i == 0 && line[i] == '\n') //this is to skip an empty line if any
			line[i] = alt_up_sd_card_read(file_pointer);
		if(i >= 100) {
			printf("WARNNING! reading a line that contains more than 100 characters\n");
			break;
		}
	} while(line[i] != '\n' && line[i] != '\r' && line[i] != 0 && line[i] != -1);
	line[i] = 0;
	if(i == 0)
		return -1;/*
	char* res = (char*)malloc(sizeof(char)*(i+1));
	strncpy(res, temp, i+1);*/
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
int main()
{
	alt_up_sd_card_dev *device_reference = NULL;
		while(!loadSDCard(device_reference)) {
			printf("SD card is not connected.\n");
		}
	/*	int file_pointer = alt_up_sd_card_fopen("WIN.WAV", false);
		if (file_pointer < 0) {
			alt_up_sd_card_fclose(file_pointer); //close the file
			printf("sound file open failed\n");
		}
		char data[50000];
		int i;
		for(i = 0; i < 50; i++) {
			data[i] = alt_up_sd_card_read(file_pointer);
			printf("%d %x\n", i , data[i]);
		}alt_up_sd_card_fclose(file_pointer);*/
		loadWavSound("WIN.WAV");
		loadSongsFromSD();
  return 0;
}

/*
 * Find all .WAV files in SDCard and return their names in an
 * array of strings.
 * Return: array containing names of all .WAV files in SDCard
 * */
char** getSongsFromSD(){
	int fileStatus = 0;
	char fileName[15];
	int numOfWavFiles = 0;

	// songNames is array of strings that store song names.
	char** songNames = malloc(101 *sizeof(char*));

	memset(fileName, 0 , sizeof(fileName)/sizeof(fileName[0]));
	fileStatus = alt_up_sd_card_find_first("", fileName);
	if (fileStatus != 0){
		printf("ERROR: updateSongsFromSD.\n");
		return NULL;
	}
	while (fileStatus != -1){
		if (strstr(fileName, ".WAV") != NULL){
			songNames[numOfWavFiles] = malloc(20 * sizeof(char));
			strcpy(songNames[numOfWavFiles], fileName);
			numOfWavFiles++;
		}
		memset(fileName, 0 , sizeof(fileName)/sizeof(fileName[0]));
		fileStatus = alt_up_sd_card_find_next(fileName);
	}
	// set ending condition, NULL means end of the array
	songNames[numOfWavFiles] = NULL;
	return songNames;
}

/*
 * Get all songs from SONGS.TXT and add it to DB.
 * It also checks if there exist any songs in SDCard that are newly
 * added and has not been written to SONGS.TXT.
 * Those songs will also get added to DB.
 * */

int getAndUpdateSongsFromTxt(char** arrFromSDFiles){
	int fileHandler;
	if ((fileHandler = openFileFromSD("SONGS.TXT")) < 0){
		printf("Reading songs from SONGS.TXT error!\n");
		return -1;
	}

	// add songs from
	char** songNames = malloc(101 *sizeof(char*));
	char* line = NULL;
	char temp[25];
	char substr[25];
	int start, end, i, iteration;
	int numOfSongs = 0, fileStats = 0;
	while (numOfSongs < 101){
		line = (char*)malloc(sizeof(char)*100);
		if((fileStats = readLine(fileHandler, line)) == -2) {
			printf("File cannot be read, reopening...\n");
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
							//createSong(temp, strtol(substr, NULL, 10));
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
		for (txtFiles_index = 0; txtFiles_index < 101; txtFiles_index++){
			if (songNames[txtFiles_index] == NULL){
				memset(strToStore, 0 , sizeof(strToStore)/sizeof(strToStore[0]));
				while((size = sprintf(strToStore, "%d %s 0", numOfSongs+1, arrFromSDFiles[sdFiles_index])) <= 0);
				writeLine(fileHandler, strToStore, size);
				//createSong(arrFromSDFiles[sdFiles_index], 0);
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
	getAndUpdateSongsFromTxt(sdsongs);
	//while(getAndUpdateSongsFromTxt() != 0);

}
