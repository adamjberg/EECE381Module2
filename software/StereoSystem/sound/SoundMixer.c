/*
 * SoundMixer.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "SoundMixer.h"

int MIX_LOCK;
/**
 * Initializes the sound mixer to loop indefinitely
 */
void initSoundMixer() {
	soundMixer = (struct SoundMixer*) malloc(sizeof(struct SoundMixer));
	soundMixer->currIndex = soundMixer->endIndex = soundMixer->indexSize = 0;
	clearSoundMixer();
}

void setGlobalVolume(float volume) {
	db.songs[db.curr_song_id]->volume = volume;
	setSoundVolume(db.songs[db.curr_song_id]->sound, (float)volume/100.0);
}

void clearSoundMixer() {
	int i;
	for(i = 0; i < 249; i++) {
		clearIndexBuffer(i);
	}
}
/*
 * Load sound's 96 samples to mix buffer
 */
void loadToSoundBuffer(struct Sound* sound) {
	//if(soundMixer->indexSize >=299) return;
	int i;
	unsigned int data;
	for(i = 0; i < MAX_SOUNDMIXBUF; i++) {
		if(sound->position >= sound->length) return;

		if (sound->position > sound->outFadePosition) {
			sound->fadeVolume *= 0.9999;
		}

		if (sound->position >= 0) {
			data = sound->buffer[sound->position];
			if (data != 0) {
				if (data > 0x07FFFFF)
					soundMixer->buffer[soundMixer->endIndex][i]
							+= positiveToNegative((negativeToPositive(data)
									* sound->fadeVolume));
				else
					soundMixer->buffer[soundMixer->endIndex][i] += data
							* sound->fadeVolume;
			}
		}
		sound->position++;
	}
}

void clearIndexBuffer(int index){
	memset(soundMixer->buffer[index], 0, MAX_SOUNDMIXBUF*sizeof(int));
}

void incIndex() {
	soundMixer->currIndex++;
	if(soundMixer->currIndex > 249) {
		soundMixer->currIndex = 0;
	}
	soundMixer->indexSize--;
	if(soundMixer->indexSize <= 0 && db.total_songs_playing <= 1) {
		soundMixer->currIndex = soundMixer->endIndex;
		disableAudioDeviceController();
	}
}

void updateMixer() {
	int i, j;
	for(i = 0; i < 150; i++) {
		//if(soundMixer->indexSize >=299) return;
		for(j = 0; j < db.total_songs_playing; j++) {
			if(!checkEnd(db.songs[db.curr_song_ids[j]]->sound)) {
				loadToSoundBuffer(db.songs[db.curr_song_ids[j]]->sound);
			}
		}

		soundMixer->indexSize++;
		soundMixer->endIndex++;

		if(soundMixer->endIndex > 249)
			soundMixer->endIndex = 0;

		clearIndexBuffer(soundMixer->endIndex);
	}
}

int negativeToPositive(int value) {
	return (0xFFFFFF - (value - 1)) & 0xFFFFFF;
}
int positiveToNegative(int value) {
	return ((0xFFFFFF - value) + 1) & 0xFFFFFF;
}
