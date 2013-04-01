/*
 * SoundMixer.h
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#ifndef SOUNDMIXER_H_
#define SOUNDMIXER_H_

#include "../Global.h"
#define MAX_SOUNDMIXBUF 96

struct SoundMixer* soundMixer;
struct Sound;
struct SoundMixer {
	unsigned int buffer[300][MAX_SOUNDMIXBUF];
	volatile int currIndex;
	volatile int endIndex;
	volatile int indexSize;
};

void initSoundMixer();
void setGlobalVolume(float);
void updateMixer();
void clearSoundMixer();
int negativeToPositive(int);
int positiveToNegative(int);
void clearIndexBuffer(int);
void incIndex();
void loadToSoundBuffer(struct Sound*);
#endif /* SOUNDMIXER_H_ */
