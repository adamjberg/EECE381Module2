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
struct SoundMixer {
	unsigned int buffer[300][MAX_SOUNDMIXBUF];
	unsigned int empty_buffer[MAX_SOUNDMIXBUF];
	volatile int currIndex, endIndex, indexSize;
	bool cleared;
};

void initSoundMixer();
void setGlobalVolume(float);
void updateMixer();
void clearSoundMixer();
int negativeToPositive(int);
int positiveToNegative(int);
void clearIndexBuffer(int);
void incIndex();
#endif /* SOUNDMIXER_H_ */
