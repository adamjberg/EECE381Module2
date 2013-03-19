/*
 * Sound.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "Sound.h"

#define DEFAULT_SAMPLE_RATE 32000
#define DEFAULT_BITS_PER_SAMPLE 24

/**
 * Helper function to convert a millisecond value to the correct position
 * in the sound buffer
 */
unsigned int convertFromMS(struct Sound* this, unsigned int value) {
	return (unsigned int) (value * DEFAULT_SAMPLE_RATE);
}

/**
 * Helper function to convert a value to its millisecond equivalent based on the
 * sampling rate
 */
unsigned int convertToMS(struct Sound* this, unsigned int value) {
	return (unsigned int) ((value * 1000)/ DEFAULT_SAMPLE_RATE);
}

/**
 * Returns the length of the sound in milliseconds
 */
unsigned int getSoundPositionMS(struct Sound* this) {
	return convertToMS(this, this->position);
}

/**
 * Returns the length of the sound in milliseconds
 */
unsigned int getSoundLengthMS(struct Sound* this) {
	return convertToMS(this, this->length);
}

/**
 * Resample the sound using linear interpolation
 * @param this sound to resample
 * @param toSampleRate - sampling rate to convert to
 * @param fromSampleRate - current sound sample rate
 */
void linearResample(struct Sound* this, int toSampleRate, int fromSampleRate) {

	if (toSampleRate == fromSampleRate) {
		return;
	}

	int i = 0;
	unsigned int srcLength = this->length;
	unsigned int destLength = srcLength * toSampleRate / fromSampleRate;
	double dx = (double) srcLength / destLength;
	double x = 0;
	unsigned int currentIndex;

	unsigned int *new_buffer =
			(unsigned int *) malloc(sizeof(int) * destLength);
	double slope;

	while (i < destLength) {
		currentIndex = (int) x;
		slope = (currentIndex < this->length ? this->buffer[currentIndex + 1] : 0 - this->buffer[currentIndex]);
		new_buffer[i] = slope * (x - currentIndex) + this->buffer[currentIndex];
		x += dx;
		i++;
	}

	free(this->buffer);
	this->length = destLength;
	this->buffer = (unsigned int*) malloc(sizeof(int) * this->length);
	for (i = 0; i < this->length; i++) {
		this->buffer[i] = new_buffer[i];
	}
	free(new_buffer);
	new_buffer = NULL;
}

/**
 * Checks to see if the values need to be shifted to match given bytesPerSample
 * @param this - sound to change values of
 * @param bitsPerSampleTo - Number of bits per sample to convert to
 * @param bitsPerSampleFrom - Number of bits used for current sample
 */
void changeBitsPerSample(struct Sound* this, int bitsPerSampleTo, int bitsPerSampleFrom) {
	if (bitsPerSampleTo == bitsPerSampleFrom)
		return;

	int i;
	int numToShift = bitsPerSampleTo - bitsPerSampleFrom;

	for (i = 0; i < this->length; i++) {
		if( numToShift > 1)
			this->buffer[i] = this->buffer[i] << numToShift;
		else
			this->buffer[i] = this->buffer[i] >> -numToShift;
	}
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
	this->volume = 1;
	this->inFadePosition = 0;
	this->outFadePosition = this->length;
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

	if( this->position < this->inFadePosition )
		this->volume = 1 - ((float) (this->inFadePosition - this->position) / this->inFadePosition);
	else if(this->position > this->outFadePosition)
		this->volume = 1 - ((float) (this->position - this->outFadePosition) / (this->length - this->outFadePosition));
	else
		this->volume = 1;

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
	SDIO_lock = 1;
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if (file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		printf("sound file open failed\n");
		return false;
	}
	char temp;
	//Start reading the wav header
	while (index < SAMPLE_RATE_OFFSET) {
		temp = alt_up_sd_card_read(file_pointer);
		index++;
	}

	int sampleRate = readInt(file_pointer, 4);
	index += 4;

	while (index < BITS_PER_SAMPLE_OFFSET) {
		temp = alt_up_sd_card_read(file_pointer);
		//printf("%d %x\n", index, temp);
		index++;
	}

	int bits_per_sample = readInt(file_pointer, 2);
	int bytes_per_sample = bits_per_sample / BITS_PER_BYTE;
	index += 2;

	while (index < DATA_LENGTH_OFFSET) {
		temp = alt_up_sd_card_read(file_pointer);
		//printf("%d %x\n", index, temp);
		index++;
	}

	sound = initSound(readInt(file_pointer, 4) / bytes_per_sample);

	printf("length: %u\n", sound->length);

	for (i = 0; i < sound->length; i++) {
		sound->buffer[i] = readInt(file_pointer, bytes_per_sample);
	}

	alt_up_sd_card_fclose(file_pointer);
	SDIO_lock = 0;

	changeBitsPerSample(sound, DEFAULT_BITS_PER_SAMPLE, bits_per_sample);
	linearResample(sound, DEFAULT_SAMPLE_RATE, sampleRate);

	printf("Sound loading complete\n");

	return sound;
}

/**
 * TODO: If a sounds volume is 0 this function should return right away to save processing time
 * Right now there's an ugly bug where the sound won't stop if the volume is set to 0
 */
void combineSounds(struct Sound* sound, struct Sound* soundToAdd, int startIndex, int numToWrite, bool overwrite) {
	int i;
	int indexToWrite = startIndex;
	int indexToRead = soundToAdd->position;

	bool useVolume = sound->volume != 1 || soundToAdd->volume != 1;
	float combinedVolume = sound->volume * soundToAdd->volume;

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
			if(useVolume)
				sound->buffer[indexToWrite] = (int) (soundToAdd->buffer[indexToRead] * combinedVolume);
			else
				sound->buffer[indexToWrite] = soundToAdd->buffer[indexToRead];
		} else {
			if(useVolume)
				sound->buffer[indexToWrite] += (int) (soundToAdd->buffer[indexToRead] * combinedVolume);
			else
				sound->buffer[indexToWrite] += soundToAdd->buffer[indexToRead];
		}
		indexToRead++;
		indexToWrite++;
	}
}

void setFadeInLength(struct Sound* this, unsigned int inFadeLength) {
	this->inFadePosition = convertFromMS(this, inFadeLength);
}

void setFadeOutLength(struct Sound* this, unsigned int outFadeLength) {
	this->outFadePosition = this->length - convertFromMS(this, outFadeLength);
}

/**
 * Overwrites the current sound buffer with updated volume
 *
 * IMPORTANT: This will change the raw sound data and after several
 * changes the quality of the sound will degrade.
 */
void setSoundVolumeStatic(struct Sound* this, float volume) {
	int i;
	for (i = 0; i < this->length; i++) {
		this->buffer[i] = this->buffer[i] * volume;
	}
	this->volume = 1;
}

/**
 * Changes the sound volume so that it can be taken into account when
 * adding the sound to the SoundMixer.
 *
 * IMPORTANT: This allows the sound to maintain all of its data
 * even if the volume is brought to 0 and then back to 1
 */
void setSoundVolume(struct Sound* this, float volume) {
	this->volume = volume;
}

/**
 * Seeks the sound to the given position
 *
 * @param this - The sound to change
 * @param position - The position to seek to in MilliSeconds
 */
void seekSound(struct Sound* this, unsigned int position) {
	this->position = convertFromMS(this, position);
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
