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
	soundMixer->currIndex = soundMixer->endIndex = soundMixer->indexSize = 0;
	clearSoundMixer();
}

void setGlobalVolume(float volume) {
	setSoundVolume(db.songs[db.curr_song_id]->sound, volume);
}

void clearSoundMixer() {
	int i;
	soundMixer->cleared = true;
	for(i = 0; i < 100; i++) {
		clearIndexBuffer(i);
	}
}
/*
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
}*/
/*
 * Load sound's 96 samples to mix buffer
 */
void loadToSoundBuffer(struct Sound* sound) {
	if(soundMixer->indexSize >=99) return;
	int i;
	unsigned int data;
	for(i = 0; i < MAX_SOUNDMIXBUF; i++) {
		if(sound->position >= sound->length) break;
		data = sound->buffer[sound->position];
		if(data > 0x07FFFFF)
			soundMixer->buffer[soundMixer->endIndex][i] += positiveToNegative((negativeToPositive(data)*sound->volume));
		else
			soundMixer->buffer[soundMixer->endIndex][i] += data *sound->volume;
		sound->position++;
	}
}

void clearIndexBuffer(int index){
	memset(soundMixer->buffer[index], 0, MAX_SOUNDMIXBUF*sizeof(int));
}

void incIndex() {
	soundMixer->currIndex++;
	soundMixer->indexSize--;
	if(soundMixer->currIndex > 99) {
		soundMixer->currIndex = 0;
	}
	//syncUpdatePos(db.curr_song_id, db.songs[db.curr_song_id]->sound->position);
}

void updateMixer() {
	int i, j, isDone = 0;
	for(i = 0; i < 10; i++) {
		if(soundMixer->indexSize >=99) return;
		for(j = 0; j < db.total_songs_playing; j++) {
			if(!checkEnd(db.songs[db.curr_song_ids[j]]->sound)) {
				loadToSoundBuffer(db.songs[db.curr_song_ids[j]]->sound);
				isDone = 1;
			} else {
				syncPause(db.curr_song_ids[j]);
			}
		}

		if(isDone == 0)
			break;

		soundMixer->indexSize++;

		soundMixer->endIndex++;
		if(soundMixer->endIndex > 99)
			soundMixer->endIndex = 0;

		clearIndexBuffer(soundMixer->endIndex);
	}

	if(soundMixer->indexSize <= 0) {
		disableAudioDeviceController();
	}
}

int negativeToPositive(int value) {
	return (0xFFFFFF - (value - 1)) & 0xFFFFFF;
}
int positiveToNegative(int value) {
	return ((0xFFFFFF - value) + 1) & 0xFFFFFF;
}
