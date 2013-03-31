/*
 * AudioFormat.c
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#include "AudioFormat.h"

struct AudioFormat* initAudioFormat(int sampleRate, int sampleSizeInBits, int channels) {
	struct AudioFormat* this = (struct AudioFormat*) malloc(sizeof(struct AudioFormat));
	this->sampleRate = sampleRate;
	this->sampleSizeInBits = sampleSizeInBits;
	this->channels = channels;
	printf("intializing AudioFormat sampleRate: %d, sampleSizeInBits: %d, channels: %d", sampleRate, sampleSizeInBits, channels);
	return this;
}

int getSampleRate(struct AudioFormat* this) {
	return this->sampleRate;
}

int getSampleSizeInBits(struct AudioFormat* this) {
	return this->sampleSizeInBits;
}

int getSampleSizeInBytes(struct AudioFormat* this) {
	return this->sampleSizeInBits / 8;
}

int getChannels(struct AudioFormat* this) {
	return this->channels;
}
