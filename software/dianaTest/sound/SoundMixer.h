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

struct SoundMixer* soundMixer; //internal global variable

struct SoundMixer {
	unsigned int buffer[100][MAX_SOUNDMIXBUF];
	int currIndex, endIndex, indexSize;
	bool cleared;
};

void initSoundMixer();
void setGlobalVolume(float);
void updateSoundMixerLength(unsigned int);
void updateSoundMixerPosition(int numWritten);
void updateMixer();
void clearSoundMixer();
int negativeToPositive(int);
int positiveToNegative(int);
void clearIndexBuffer(int);
void incIndex();
#endif /* SOUNDMIXER_H_ */
