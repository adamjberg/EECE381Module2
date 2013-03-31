/*
 * AudioFormat.h
 *
 *  Created on: Mar 31, 2013
 *      Author: Adam
 */

#ifndef AUDIOFORMAT_H_
#define AUDIOFORMAT_H_

struct AudioFormat {
	int channels;
	int sampleRate;
	int sampleSizeInBits;
};

struct AudioFormat* initAudioFormat(int, int, int);
int getSampleRate(struct AudioFormat*);
int getSampleSizeInBits(struct AudioFormat*);
int getSampleSizeInBytes(struct AudioFormat*);
int getChannels(struct AudioFormat*);

#endif /* AUDIOFORMAT_H_ */
