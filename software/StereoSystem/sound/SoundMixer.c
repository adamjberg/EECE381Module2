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
	int size = db.curr_song_ids->size;
	int* temp;
	/*for(i = 0; i < size; i++) {
		temp = dequeue(db.curr_song_ids);
		if(!db.songs[*temp]->sound->playing) {
			free(temp);
		} else {
		soundMixer->cleared = false;
		combineSounds(soundMixer->sound, db.songs[*temp]->sound,
						soundMixer->sound->position, numWritten, numSoundsPlaying == 0);
		updateSoundPosition(db.songs[*temp]->sound, numWritten);
		numSoundsPlaying++;
		enqueue(db.curr_song_ids, temp);
		}
	}
	temp = NULL;*/
	for (i = 1; i < db.num_of_songs + 1; i++) {
		if( db.songs[i] == NULL || db.songs[i]->sound == NULL)
			continue;
		if (db.songs[i]->sound->playing) {
			soundMixer->cleared = false;
			combineSounds(soundMixer->sound, db.songs[i]->sound,
					soundMixer->sound->position, numWritten, numSoundsPlaying == 0);
			updateSoundPosition(db.songs[i]->sound, numWritten);
			numSoundsPlaying++;
		}
	}
	if( !soundMixer->cleared && numSoundsPlaying == 0 ) {
		clearSoundMixer();
	}
}
