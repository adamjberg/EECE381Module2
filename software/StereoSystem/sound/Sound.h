/*
 * Sound.h
 *
 *  Created on: 2013-03-06
 *      Author: adam_000
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "../Global.h"
#include "../libMad/mad.h"

#define SAMPLE_RATE_OFFSET 24
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_LENGTH_OFFSET 40

int SDIO_lock;

struct Sound {
	unsigned int inFadePosition;
	unsigned int outFadePosition;
	unsigned int position;
	unsigned int loading_pos;
	unsigned int length;
	int loops;
	unsigned int *buffer;
	bool playing;
	volatile float volume;
	volatile float fadeVolume;
};

struct Sound* initSound(unsigned int);
void setFadeInLength(struct Sound*, unsigned int);
void setFadeOutLength(struct Sound*, unsigned int);
unsigned int getSoundPositionMS(struct Sound*);
unsigned int getSoundLengthMS(struct Sound*);
struct Sound* loadWavSound(char*);
void setSoundVolume(struct Sound*, float);
void unloadSound(struct Sound*);
void seekSound(struct Sound*, unsigned int);
void playSound(struct Sound*, float, int, int);
void pauseSound(struct Sound*);
void resumeSound(struct Sound*);
void stopSound(struct Sound*);
bool checkEnd(struct Sound*);
int readInt(int, int);
void updatePos(struct Sound*);
int* loadWavHeader(char*);
struct Sound* loadSound(struct Song*);
struct Sound* loadMP3Sound(char*);
#endif /* SOUND_H_ */
