/*
 * SoundMixer.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "SoundMixer.h"

#define BUFFER_LENGTH 50000

/**
 * Initializes the sound mixer to loop indefinitely
 */
void initSoundMixer() {
	soundMixer = (struct SoundMixer*) malloc(sizeof(struct SoundMixer));
	soundMixer->sound = initSound(BUFFER_LENGTH);
	soundMixer->sound->playing = true;
	soundMixer->sound->loops = -1;
	soundMixer->cleared = true;
}

void setGlobalVolume(float volume) {
	soundMixer->sound->volume = volume;
}

void clearSoundMixer() {
	clearSoundBuffer(soundMixer->sound);
	soundMixer->cleared = true;
}

void updateSoundMixerPosition(int numWritten) {
	int numSoundsPlaying = 0;
	int i;
	updateSoundPosition(soundMixer->sound, numWritten);
	for (i = 1; i < db.num_of_songs + 1; i++) {
		if( db.songs[i] == NULL || db.songs[i]->sound == NULL)
			continue;
		if (db.songs[i]->sound->playing) {
			soundMixer->cleared = false;
			combineSounds(soundMixer->sound, db.songs[i]->sound, soundMixer->volume,
					soundMixer->sound->position, numWritten, numSoundsPlaying == 0);
			updateSoundPosition(db.songs[i]->sound, numWritten);
			numSoundsPlaying++;
		}
	}
	if( !soundMixer->cleared && numSoundsPlaying == 0 ) {
		clearSoundMixer();
	}
}
