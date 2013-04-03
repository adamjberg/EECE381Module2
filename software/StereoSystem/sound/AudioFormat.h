/*
 * AudioFormat.h
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#ifndef AUDIOFORMAT_H_
#define AUDIOFORMAT_H_

#include "../Global.h"

struct AudioFormat {
	int channels;
	int sampleRate;
	int sampleSizeInBits;
	int bitRateKbps;
};

struct AudioFormat* initAudioFormat(int, int, int, int);
int getSampleRate(struct AudioFormat*);
int getSampleSizeInBytes(struct AudioFormat*);
void setSampleRate(struct AudioFormat*, int);

#endif /* AUDIOFORMAT_H_ */
