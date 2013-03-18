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
  return 0;
}

