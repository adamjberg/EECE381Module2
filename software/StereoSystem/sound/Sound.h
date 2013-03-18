/*
 * Sound.h
 *
 *  Created on: 2013-03-06
 *      Author: adam_000
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "stdio.h"
#include "stdlib.h"
#include "altera_up_sd_card_avalon_interface.h"
#include "SoundMixer.h"

#define BITS_PER_BYTE 8
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_LENGTH_OFFSET 40

int SDIO_lock;

struct Sound {
	unsigned int position;
	unsigned int length;
	int loops;
	unsigned int *buffer;
	bool playing;
	float volume;
};

struct Sound* initSound(unsigned int);
unsigned int getSoundPositionMS(struct Sound*);
unsigned int getSoundLengthMS(struct Sound*);
void updateSoundPosition(struct Sound*, int);
struct Sound* loadWavSound(char*);
void combineSounds(struct Sound*, struct Sound*, int, int, bool);
void setSoundVolumeStatic(struct Sound*, float);
void setSoundVolume(struct Sound*, float);
void unloadSound(struct Sound*);
void seekSound(struct Sound*, unsigned int);
void playSound(struct Sound*, float, int, int);
void pauseSound(struct Sound*);
void resumeSound(struct Sound*);
void stopSound(struct Sound*);
void clearSoundBuffer(struct Sound*);

#endif /* SOUND_H_ */
