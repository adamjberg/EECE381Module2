/*
 * SoundMixer.h
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#ifndef SOUNDMIXER_H_
#define SOUNDMIXER_H_

#include "Global.h"
#define MAX_SOUNDMIXBUF 96

struct SoundMixer* soundMixer;

struct SoundMixer {
	unsigned int buffer[100][MAX_SOUNDMIXBUF];
	int currIndex, endIndex, indexSize;
	bool cleared;
};

void initSoundMixer();
void setGlobalVolume(float);
void updateSoundMixerLength(unsigned int);
void updateSoundMixerPosition(int numWritten);
void loadToSoundBuffer(struct Sound*);
void incIndex();
void updateMixer();
#endif /* SOUNDMIXER_H_ */
