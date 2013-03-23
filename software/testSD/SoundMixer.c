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
	soundMixer->cleared = true;
	soundMixer->currIndex = soundMixer->endIndex = soundMixer->indexSize = 0;
	int i;
	for(i = 0; i < 10; i++) {
		memset(soundMixer->buffer[i], 0, MAX_SOUNDMIXBUF*sizeof(int));
	}
}

void setGlobalVolume(float volume) {
	//setSoundVolume(soundMixer->sound, volume);
}

void clearSoundMixer() {
	//clearSoundBuffer(soundMixer->sound);
	soundMixer->cleared = true;
}

void updateSoundMixerPosition(int numWritten) {
	//int numSoundsPlaying = 0;
	//int i;
	//updateSoundPosition(soundMixer->sound, numWritten);
	/*for(i = 0; i < db.total_songs_playing; i++) {
		if(db.songs[db.curr_song_ids[i]]->sound->playing) {
			soundMixer->cleared = false;
			combineSounds(soundMixer->sound, db.songs[db.curr_song_ids[i]]->sound,
							soundMixer->sound->position, numWritten, numSoundsPlaying == 0);
			updateSoundPosition(db.songs[db.curr_song_ids[i]]->sound, numWritten);
			numSoundsPlaying++;
		} else {
			syncPause(db.curr_song_ids[i]);
		}
	}*/
	//if( !soundMixer->cleared && numSoundsPlaying == 0 ) {
	//	clearSoundMixer();
	//}
}
/*
 * Load sound's 96 samples to mix buffer
 */
void loadToSoundBuffer(struct Sound* sound) {
	if(soundMixer->indexSize >=99) return;
	int i;
	for(i = 0; i < MAX_SOUNDMIXBUF; i++) {
		if(sound->position >= sound->length) break;
		soundMixer->buffer[soundMixer->endIndex][i] += sound->buffer[sound->position];
		sound->position++;
	}
}

void clearIndexBuffer(int index){
	memset(soundMixer->buffer[index], 0, MAX_SOUNDMIXBUF*sizeof(int));
}
void incIndex() {
	soundMixer->currIndex++;
	soundMixer->indexSize--;
	if(soundMixer->currIndex >= 100) {
		soundMixer->currIndex = 0;
	}
}

void updateMixer() {
	int i, isDone = 0;
	for(i = 0; i < 10; i++) {
		if(soundMixer->indexSize >=99) return;
		if(s->position < s->length) {
			loadToSoundBuffer(s);
			isDone = 1;
		}

		if(s1->position < s1->length)
			loadToSoundBuffer(s1);

		if(s2->position < s2->length) {
			loadToSoundBuffer(s2);
			isDone = 1;
		}

		if(isDone == 0)
			break;

		soundMixer->indexSize++;

		soundMixer->endIndex++;
		if(soundMixer->endIndex > 99)
			soundMixer->endIndex = 0;

		clearIndexBuffer(soundMixer->endIndex);
	}
}
