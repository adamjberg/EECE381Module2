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
	setSoundVolume(soundMixer->sound, volume);
}

void clearSoundMixer() {
	clearSoundBuffer(soundMixer->sound);
	soundMixer->cleared = true;
}

void updateSoundMixerPosition(int numWritten) {
	int numSoundsPlaying = 0;
	int i;
	updateSoundPosition(soundMixer->sound, numWritten);
	for(i = 0; i < db.total_songs_playing; i++) {
		if(db.songs[db.curr_song_ids[i]]->sound->playing) {
			soundMixer->cleared = false;
			combineSounds(soundMixer->sound, db.songs[db.curr_song_ids[i]]->sound,
							soundMixer->sound->position, numWritten, numSoundsPlaying == 0);
			updateSoundPosition(db.songs[db.curr_song_ids[i]]->sound, numWritten);
			numSoundsPlaying++;
		} else {
			syncPause(db.curr_song_ids[i]);
		}
	}
	if( !soundMixer->cleared && numSoundsPlaying == 0 ) {
		clearSoundMixer();
	}
}
