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
	int currIndex;
	int endIndex;
	int indexSize;
	bool cleared;
};

void initSoundMixer();
void setGlobalVolume(float);
int updateMixer();
void clearSoundMixer();
int negativeToPositive(int);
int positiveToNegative(int);
void clearIndexBuffer(int);
void incIndex();
void loadToSoundBuffer(struct Sound*);
#endif /* SOUNDMIXER_H_ */
