/*
 * Sound.h
 *
 *  Created on: 2013-03-06
 *      Author: adam_000
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "math.h"
#include "../Global.h"
#include "../libMad/mad.h"
#include "../DSP/PhaseVocoder.h"
#include "AudioFormat.h"

#define NUM_CHANNELS_OFFSET 22
#define SAMPLE_RATE_OFFSET 24
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_LENGTH_OFFSET 40

int SDIO_lock;

struct Sound {
	struct AudioFormat* audioFormat;
	unsigned int inFadePosition;
	unsigned int outFadePosition;
	unsigned int position;
	unsigned int loading_pos;
	unsigned int length;
	int loops;
	int *buffer;
	volatile float volume;
	volatile float fadeVolume;
};

unsigned int convertToMS(unsigned int);
struct Sound* initSound(unsigned int);
void allocateSoundBuffer(struct Sound*, int);
void setFadeInLength(struct Sound*, unsigned int);
void setFadeOutLength(struct Sound*, unsigned int);
unsigned int getSoundPositionMS(struct Sound*);
unsigned int getSoundLengthMS(struct Sound*);
struct Sound* loadWavSound(char*);
void setSoundVolume(struct Sound*, float);
void setSoundPitch(struct Sound*, float, int);
void setSoundPlaybackSpeed(struct Sound*, float);
int resampleSound(struct Sound*, int, bool, int);
void unloadSound(struct Sound*);
void seekSound(struct Sound*, unsigned int);
void playSound(struct Sound*, float, int, int);
void stopSound(struct Sound*);
bool checkEnd(struct Sound*);
int readInt(int, int, bool);
struct Sound* loadWavHeader(int);
struct Sound* loadSound(struct Song*);
struct Sound* loadMP3Sound(char*);
#endif /* SOUND_H_ */
