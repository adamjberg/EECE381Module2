/*
 * AudioFormat.h
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#ifndef AUDIOFORMAT_H_
#define AUDIOFORMAT_H_

#include "stdlib.h"
#include "stdio.h"

struct AudioFormat {
	int channels;
	int sampleRate;
	int sampleSizeInBits;
	int bitRateKbps;
};

struct AudioFormat* initAudioFormat(int, int, int, int);
float getBitRateKbps(struct AudioFormat*);
int getSampleRate(struct AudioFormat*);
int getSampleSizeInBits(struct AudioFormat*);
int getSampleSizeInBytes(struct AudioFormat*);
int getChannels(struct AudioFormat*);
void setSampleRate(struct AudioFormat*, int);

#endif /* AUDIOFORMAT_H_ */
