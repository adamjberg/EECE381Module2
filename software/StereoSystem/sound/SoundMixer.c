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
	setSoundVolume(db.songs[db.curr_song_id]->sound, volume);
}

void clearSoundMixer() {
	int i;
	soundMixer->cleared = true;
	for(i = 0; i < 299; i++) {
		clearIndexBuffer(i);
	}
}
/*
 * Load sound's 96 samples to mix buffer
 */
void loadToSoundBuffer(struct Sound* sound) {
	if(soundMixer->indexSize >=299) return;
	int i;
	unsigned int data;
	if (sound->position <= sound->inFadePosition) {
		sound->fadeVolume = sound->volume;
	}
	for(i = 0; i < MAX_SOUNDMIXBUF; i++) {
		if(sound->position >= sound->length) return;
		//if (allowFade(sound)) {

		if (sound->position > sound->outFadePosition) {
			sound->fadeVolume *= 0.9999;
		}
		//}
		data = sound->buffer[sound->position];
		if(data > 0x07FFFFF)
			soundMixer->buffer[soundMixer->endIndex][i] += positiveToNegative((negativeToPositive(data)*sound->fadeVolume));
		else
			soundMixer->buffer[soundMixer->endIndex][i] += data *sound->fadeVolume;
		sound->position++;
	}
}

void clearIndexBuffer(int index){
	memset(soundMixer->buffer[index], 0, MAX_SOUNDMIXBUF*sizeof(int));
}

void incIndex() {
	soundMixer->currIndex++;
	if(soundMixer->currIndex > 299) {
		soundMixer->currIndex = 0;
	}
	soundMixer->indexSize--;
	if(soundMixer->indexSize <= 0)
		disableAudioDeviceController();
}

int updateMixer() {
//	MIX_LOCK = 1;
	int i, j, isDone = 0, isFinished = 1;
	for(i = 0; i < 80; i++) {
		if(soundMixer->indexSize >=299) return 0;
		for(j = 0; j < db.total_songs_playing; j++) {
			if(!checkEnd(db.songs[db.curr_song_ids[j]]->sound)) {
				loadToSoundBuffer(db.songs[db.curr_song_ids[j]]->sound);
				isDone = 1;
			}
		}

		if(isDone == 0)
			break;

		soundMixer->indexSize++;

		soundMixer->endIndex++;
		if(soundMixer->endIndex > 299)
			soundMixer->endIndex = 0;

		clearIndexBuffer(soundMixer->endIndex);
	}

	if(soundMixer->indexSize <= 0 && !db.isPaused && db.total_songs_playing > 0) {
		isFinished = 1;
		db.isPaused = true;
		syncPause(db.curr_song_id);
		if(db.curr_playlist_id != 0)
			syncNext(db.curr_song_id);
	} else
		isFinished= 0;
	return isFinished;
	// else
		//IOWR_16DIRECT(AUDIOBUFFERPROCESS_BASE, 0, 0);
	//else if(!db.isPaused&& db.total_songs_playing > 0)
		//enableAudioDeviceController();
	//MIX_LOCK = 0;
}

int negativeToPositive(int value) {
	return (0xFFFFFF - (value - 1)) & 0xFFFFFF;
}
int positiveToNegative(int value) {
	return ((0xFFFFFF - value) + 1) & 0xFFFFFF;
}
