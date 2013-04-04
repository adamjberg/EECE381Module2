/*
 * AudioFormat.h
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#ifndef AUDIOFORMAT_H_
#define AUDIOFORMAT_H_

#include "../Global.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

struct AudioFormat {
	int channels;
	int sampleRate;
	int sampleSizeInBits;
	int bitRateKbps;
};

struct AudioFormat* initAudioFormat(int, int, int, int);
bool isAudioFormatValid(struct AudioFormat*);
float getBitRateKbps(struct AudioFormat*);
int getSampleRate(struct AudioFormat*);
int getSampleSizeInBytes(struct AudioFormat*);
int getNumChannels(struct AudioFormat*);
void setSampleRate(struct AudioFormat*, int);

#endif /* AUDIOFORMAT_H_ */
