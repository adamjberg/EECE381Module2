/*
 * AudioFormat.c
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#include "AudioFormat.h"

struct AudioFormat* initAudioFormat(int sampleRate, int sampleSizeInBits,
		int channels, int byteRate) {
	struct AudioFormat* this = (struct AudioFormat*) malloc(sizeof(struct AudioFormat));
	this->sampleRate = sampleRate;
	this->sampleSizeInBits = sampleSizeInBits;
	this->channels = channels;
	this->bitRateKbps = (float) (byteRate * 8) / 1000;
	printf("AudioFormat sampleRate: %d, sampleSizeInBits: %d, channels: %d, bitRate(Kbps): %d\n",
			sampleRate, sampleSizeInBits, channels, this->bitRateKbps);
	return this;
}

bool isSampleRateValid(int sampleRate) {
	return sampleRate > 0 && sampleRate <= 96000;
}

bool isSampleSizeInBitsValid(int sampleSizeInBits) {
	return sampleSizeInBits <= 32 && sampleSizeInBits > 0;
}

bool isNumChannelsValid(int numChannels) {
	return numChannels == 1 || numChannels == 0;
}

bool isAudioFormatValid(struct AudioFormat* this) {
	return isSampleRateValid(this->sampleRate) && isSampleSizeInBitsValid(this->sampleSizeInBits) && isNumChannelsValid(this->channels);
}

float getBitRateKbps(struct AudioFormat* this) {
	return this->bitRateKbps;
}

int getSampleRate(struct AudioFormat* this) {
	return this->sampleRate;
}

int getSampleSizeInBytes(struct AudioFormat* this) {
	return this->sampleSizeInBits / 8;
}

int getNumChannels(struct AudioFormat* this) {
	return this->channels;
}

void setSampleRate(struct AudioFormat* this, int sampleRate) {
	this->sampleRate = sampleRate;
}
