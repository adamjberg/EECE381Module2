/*
 * SoundMixer.h
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#ifndef SOUNDMIXER_H_
#define SOUNDMIXER_H_

#include "../Global.h"

struct SoundMixer* soundMixer;

struct SoundMixer {
	struct Sound* sound;
	bool cleared;
	float volume;
};

void initSoundMixer();
void setGlobalVolume(float);
void updateSoundMixerLength(unsigned int);
void updateSoundMixerPosition(int numWritten);

#endif /* SOUNDMIXER_H_ */
