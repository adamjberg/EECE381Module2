/*
 * Sound.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "Sound.h"

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

void clearSoundBuffer(struct Sound* sound) {
	int i;
	for (i = 0; i < sound->length; i++) {
		sound->buffer[i] = 0;
	}
}

struct Sound* initSound(unsigned int length) {
	struct Sound* this = (struct Sound*) malloc(sizeof(struct Sound));
	this->length = length;
	this->position = 0;
	this->buffer = (unsigned int*) malloc(sizeof(int) * length);
	this->playing = false;
	clearSoundBuffer(this);
	return this;
}

void handleSoundEnd(struct Sound* this) {
	if (this->loops == 0) {
		stopSound(this);
	} else {
		this->position = 0;
		if (this->loops > 0)
			this->loops--;
	}
}

/**
 * Update the position index for this sound.
 * Determines if the sound is complete and allows it to continue playing if
 * it should loop.
 *
 * @param numWritten - The number of values written to the buffer
 */
void updateSoundPosition(struct Sound* this, int numWritten) {
	if (!this->playing)
		return;
	this->position += numWritten;

	if (this->position >= this->length) {
		handleSoundEnd(this);
	}
}

/**
 * Creates a Sound struct and loads the correct wav file from the SD card
 *
 * @param id - id of the sound to load
 * @param buffer - buffer to save the sound wave into
 * NOTE: If a NULL buffer is supplied the sound will be saved in the Sound struct
 */
struct Sound* loadWavSound(char * filename) {
	printf("loading sound: %s\n", filename);

	struct Sound* sound;
	int i, index = 0;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if (file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return false;
	}

	//Start reading the wav header
	while (index < BITS_PER_SAMPLE_OFFSET) {
		alt_up_sd_card_read(file_pointer);
		index++;
	}

	int bytes_per_sample = readInt(file_pointer, 2) / BITS_PER_BYTE;
	index += 2;

	while (index < DATA_LENGTH_OFFSET) {
		alt_up_sd_card_read(file_pointer);
		index++;
	}

	sound = initSound(readInt(file_pointer, 4) / bytes_per_sample);

	printf("length: %u\n", sound->length);

	for (i = 0; i < sound->length; i++) {
		sound->buffer[i] = readInt(file_pointer, bytes_per_sample);
	}

	printf("Sound loading complete\n");

	alt_up_sd_card_fclose(file_pointer);

	return sound;
}

void combineSounds(struct Sound* sound, struct Sound* soundToAdd, int startIndex, int numToWrite, bool overwrite) {
	int i, indexToWrite = startIndex, indexToRead =
			soundToAdd->position;
	for (i = 0; i < numToWrite; i++) {
		if (indexToWrite >= sound->length) {
			indexToWrite = 0;
		}
		if (indexToRead >= soundToAdd->length) {
			handleSoundEnd(soundToAdd);
			if (!soundToAdd->playing) {
				return;
			}
			indexToRead = 0;
		}
		if (overwrite) {
			sound->buffer[indexToWrite] = soundToAdd->buffer[indexToRead];
		} else {
			sound->buffer[indexToWrite] += soundToAdd->buffer[indexToRead];
		}
		indexToRead++;
		indexToWrite++;
	}
}

void playSound(struct Sound* sound, float volume, int startTime, int loops) {
	sound->position = startTime;
	sound->volume = volume;
	sound->playing = true;
	sound->loops = loops;
}

void pauseSound(struct Sound* sound) {
	sound->playing = false;
}

void resumeSound(struct Sound* sound) {
	sound->playing = true;
}

void stopSound(struct Sound* sound) {
	sound->playing = false;
	sound->position = 0;
}

void unloadSound(struct Sound* sound) {
	free(sound->buffer);
	free(sound);
}
