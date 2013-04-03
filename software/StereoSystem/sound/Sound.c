/*
 * Sound.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "Sound.h"

#define DEFAULT_SAMPLE_RATE 32000
#define DEFAULT_BITS_PER_SAMPLE 24
#define MP3_DECODE_MULTIPLIER 4
#define MAX_QUALITY 4

struct buffer {
	unsigned char *start;
	unsigned long length;
};

// Temporary buffer to hold mp3 bytes to be decoded
struct buffer tempBuffer;
/**
 * Helper function to convert a millisecond value to the correct position
 * in the sound buffer
 */
unsigned int convertFromMS(int value) {

	return (unsigned int) (value * 32);
}

/**
 * Helper function to convert a value to its millisecond equivalent based on the
 * sampling rate
 */
unsigned int convertToMS(unsigned int value) {
	return (unsigned int) ((value * 1000) / DEFAULT_SAMPLE_RATE);
}

/**
 * Returns the length of the sound in milliseconds
 */
unsigned int getSoundPositionMS(struct Sound* this) {
	return convertToMS(this->position);
}

/**
 * Returns the length of the sound in milliseconds
 */
unsigned int getSoundLengthMS(struct Sound* this) {
	return convertToMS(this->length);
}

int getMaxSoundValue(struct Sound* this) {
	int i, maxVal = 0, nextVal;

	for (i = 0; i < this->length; i++) {
		nextVal = this->buffer[i] < 0 ? -this->buffer[i] : this->buffer[i];
		maxVal = nextVal > maxVal ? nextVal : maxVal;
	}
	return maxVal;
}

void setSoundPitch(struct Sound* this, float pitch, int quality) {
	int i, origSampleRate, maxVal, fftFrameSize = 2048;
	float *soundBuffer;
	long oSamp, downSampleAmount;

	printf("Changing sound pitch to: %f with quality: %d\n", pitch, quality);

	switch (quality) {
	case 0:
		setSoundPlaybackSpeed(this, pitch);
		return;
	case 1:
		downSampleAmount = 10;
		oSamp = 1;
		break;
	case 2:
		downSampleAmount = 5;
		oSamp = 2;
		break;
	case 3:
		downSampleAmount = 3;
		oSamp = 3;
		break;
	case 4:
		downSampleAmount = 2;
		oSamp = 4;
		break;
	default:
		downSampleAmount = 10;
		oSamp = 1;
		break;
	}

	origSampleRate = getSampleRate(this->audioFormat);
	resampleSound(this, origSampleRate / downSampleAmount, false, 0);
	maxVal = getMaxSoundValue(this);
	soundBuffer = (float *) malloc(sizeof(float) * this->length);

	for (i = 0; i < this->length; i++) {
		soundBuffer[i] = (float) (this->buffer[i]) / maxVal;
	}

	pitchShift(pitch, this->length, fftFrameSize, oSamp, getSampleRate(this->audioFormat), soundBuffer, soundBuffer);

	for (i = 0; i < this->length; i++) {
		this->buffer[i] = (int) (soundBuffer[i] * maxVal);
	}

	resampleSound(this, origSampleRate, false, 0);
	free(soundBuffer);
	soundBuffer = NULL;
	printf("Pitch Successfully Updated");
}

void setSoundPlaybackSpeed(struct Sound* this, float speed) {
	resampleSound(this, (int)(getSampleRate(this->audioFormat) / speed), false, 0);
}

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */
static enum mad_flow input(void *data, struct mad_stream *stream) {
	struct Sound* sound = data;
	unsigned char * buf = tempBuffer.start;

	/* The buffer is the full pre-loaded song, so this function will only
	 * be called twice:
	 * 1. At the beginning of processing
	 * 2. When this buffer is exhausted (end of the song)
	 */
	if (tempBuffer.length) {
		mad_stream_buffer(stream, buf, tempBuffer.length);
		tempBuffer.length = 0;
		return MAD_FLOW_CONTINUE;
	} else {
		printf("MP3 Successfully decoded\n");
		free(tempBuffer.start);
		sound->length = sound->position;
		sound->position = 0;
		return MAD_FLOW_STOP;
	}
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 *
 * (In our case, we simply put the PCM values into a buffer)
 */
static enum mad_flow output(void *data, struct mad_header const *header,
		struct mad_pcm *pcm) {
	struct Sound* sound = data;
	unsigned int nchannels, nsamples;
	mad_fixed_t const *left_ch, *right_ch;
	int i;

	nchannels = pcm->channels;
	nsamples = pcm->length;
	left_ch = pcm->samples[0];
	right_ch = pcm->samples[1];

	for (i = 0; i < nsamples; i++) {
		if (sound->position > sound->length) {
			printf("Trying to write more space than allocated");
			break;
		}
		sound->buffer[sound->position] = (left_ch[i] & 0xFFFFFF00) >> 8;
		sound->position++;
	}

	return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */
static enum mad_flow error(void *data, struct mad_stream *stream,
		struct mad_frame *frame) {
	fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %ld\n",
			stream->error, mad_stream_errorstr(stream), stream->this_frame
					- tempBuffer.start);

	/* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */
	return MAD_FLOW_BREAK;

	return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */
static int decodeMP3(struct Sound* this, unsigned char *start,
		unsigned long length) {
	struct mad_decoder decoder;
	int result;

	/* initialize our private message structure -
	 * this structure is for our own purposes, it is
	 * not a data type that belongs to libMAD*/
	tempBuffer.start = start;
	tempBuffer.length = length;

	/* configure input, output, and error functions */
	mad_decoder_init(&decoder, this, input, 0 /* header */, 0 /* filter */,
			output, error, 0 /* message */);

	/* start decoding */
	result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	/* release the decoder */
	mad_decoder_finish(&decoder);

	return result;
}

struct Sound* loadMP3Sound(char * file) {
	int filePointer;
	int i = 0;
	unsigned char * buf = NULL;
	int byte = 0;
	unsigned int tempBufferSize;
	unsigned int decodedBufferSize;

	printf("Reading %s file size\n", file);
	filePointer = alt_up_sd_card_fopen(file, false);
	while (i != 33) {
		byte = alt_up_sd_card_read(filePointer);
		i++;
	}
	tempBufferSize = readInt(filePointer, 4, true);

	// The final buffer size will depend on the mp3 bitrate
	// For now I'm multiplying by this multiplier which will save slightly more space than needed
	decodedBufferSize = tempBufferSize * MP3_DECODE_MULTIPLIER;

	alt_up_sd_card_fclose(filePointer);
	printf("File Size: %d bytes\n", tempBufferSize);
	if (decodedBufferSize > MAX_CACHE_MEMORY) {
		alt_up_sd_card_fclose(filePointer);
		printf("FILE size too big");
		return NULL;
	}
	struct Sound* this = initSound(decodedBufferSize);
	allocateSoundBuffer(this, this->length);

	buf = malloc(tempBufferSize);
	if (!buf) {
		printf("Malloc failed\n");
		exit(0);
	}

	printf("Preloading mp3\n");
	filePointer = alt_up_sd_card_fopen(file, false);
	for (i = 0; i < tempBufferSize; i++) {
		buf[i] = alt_up_sd_card_read(filePointer);
		if (i % (1024 * 1024) == 0) {
			printf("%d MB | ", i / (1024 * 1024));
		}
	}
	alt_up_sd_card_fclose(filePointer);
	printf("\nPreloading complete\n");

	decodeMP3(this, buf, tempBufferSize);
	return this;
}

/**
 * Resamples a sound from it's current sample rate
 */
int resampleSound(struct Sound* this, int toSampleRate, bool fromFile, int filePointer) {
	int fromSampleRate = getSampleRate(this->audioFormat);
	int bytesPerSample = getSampleSizeInBytes(this->audioFormat);
	int srcLength =  this->length;
	int destLength = ((float)srcLength * toSampleRate) / fromSampleRate;
	float x0 = 0, x1 = 0, x = 0;
	int y0 = 0, y1 = 0;
	int i, j = 0, k;
	int* bufferToWriteTo;

	if(fromSampleRate == toSampleRate) {
		return 0;
	}

	if(fromFile) {
		allocateSoundBuffer(this, destLength);
		bufferToWriteTo = this->buffer;
	} else {
		this->length = destLength;
		bufferToWriteTo = (int*) malloc(sizeof(int) * this->length);
	}

	for (i = 0; i < srcLength; i++) {
		if (j >= this->length)
			break;
		x1 = i / (float) fromSampleRate;
		if (fromFile) {
			for (k = 0; k < getNumChannels(this->audioFormat); k++) {
				if ((y1 = readInt(filePointer, bytesPerSample, false)) < 0) {
					free(this->audioFormat);
					this->audioFormat = NULL;
					free(this->buffer);
					this->buffer = NULL;
					free(this);
					this = NULL;
					return -1;
				}
			}
		} else {
			y1 = this->buffer[i];
		}
		if (y1 > 0x07FFFFF) {
			y1 = y1 | 0xFF000000;
		}
		while (x <= x1) {
			if (x == x1) {
				bufferToWriteTo[j++] = y1;
			} else if (x < x1) {
				bufferToWriteTo[j++] = (y1 - y0) * (x - x0) / (x1 - x0) + y0;
				if (bufferToWriteTo[j - 1] > 0x07FFFFF)
					bufferToWriteTo[j - 1] &= 0x00FFFFFF;
			}
			x = j / (float) toSampleRate;
		}
		x0 = x1;
		y0 = y1;
	}
	if (!fromFile) {
		free(this->buffer);
		this->buffer = bufferToWriteTo;
	}
	setSampleRate(this->audioFormat, toSampleRate);
	return 0;
}

/**
 * Loads the sound using linear interpolation to convert to correct sample rate
 */
int loadSoundBuffer(struct Sound* this, int filePointer) {
	int i = 0, j, bytesPerSample;

	if(this->audioFormat->sampleRate != DEFAULT_SAMPLE_RATE)
		return resampleSound(this, DEFAULT_SAMPLE_RATE, true, filePointer);
	else {
		bytesPerSample = getSampleSizeInBytes(this->audioFormat);
		allocateSoundBuffer(this, this->length);
		for (i = 0; i < this->length; i++) {
			for (j = 0; j < getNumChannels(this->audioFormat); j++) {
				if ((this->buffer[i] = readInt(filePointer, bytesPerSample,
						false)) < 0) {
					free(this->audioFormat);
					this->audioFormat = NULL;
					free(this->buffer);
					this->buffer = NULL;
					free(this);
					this = NULL;
					return -1;
				}
			}
		}
	}
	return 0;
}

/**
 * Checks to see if the values need to be shifted to match given bytesPerSample
 * @param this - sound to change values of
 * @param bitsPerSampleTo - Number of bits per sample to convert to
 */
void changeBitsPerSample(struct Sound* this, int bitsPerSampleTo) {
	if(this == NULL) return;
	int bitsPerSampleFrom = getSampleSizeInBits(this->audioFormat);
	if (bitsPerSampleTo == bitsPerSampleFrom)
		return;

	int i;
	int numToShift = bitsPerSampleTo - bitsPerSampleFrom;

	for (i = 0; i < this->length; i++) {
		if (numToShift > 1)
			this->buffer[i] = this->buffer[i] << numToShift;
		else
			this->buffer[i] = this->buffer[i] >> -numToShift;
	}
}

/**
 * Helper function to read multiple bytes and return the representative int value
 */
int readInt(int file_pointer, int numBytesToRead, bool bigEndian) {
	int ret = 0, numBytesRead = 0, i = bigEndian ? numBytesToRead - 1 : 0;
	int bytes[numBytesToRead];
	while (numBytesRead < numBytesToRead) {
		if ((bytes[i] = alt_up_sd_card_read(file_pointer)) < 0) {
			printf("read file invalid\n");
			return -1;
		}
		ret |= (bytes[i] << (8 * i));
		if (bigEndian)
			i--;
		else
			i++;
		numBytesRead++;
	}
	return ret;
}

void clearSoundBuffer(struct Sound* sound) {
	memset(sound->buffer, 0, sizeof(sound->buffer));
}

void allocateSoundBuffer(struct Sound* this, int length) {
	if (memMgr.used_memory + length > MAX_CACHE_MEMORY) {
		freeMem(length);
	}
	if (this->buffer != NULL) {
		free(this->buffer);
	}
	this->buffer = (int*) malloc(sizeof(int) * length);

	this->length = length;
	if (!this->buffer)
		printf("Failed to allocate sound buffer\n");
	clearSoundBuffer(this);
}

struct Sound* initSound(unsigned int length) {
	struct Sound* this = (struct Sound*) malloc(sizeof(struct Sound));
	if (!this)
		printf("Failed to allocate space for sound\n");
	this->length = length;
	this->position = 0;
	this->buffer = NULL;
	this->volume = 1;
	this->fadeVolume = 1;
	this->inFadePosition = 0;
	this->outFadePosition = this->length;
	this->loading_pos = 0;
	this->audioFormat = NULL;
	return this;
}

bool allowFade(struct Sound* this) {
	return !(this->inFadePosition == 0 && this->outFadePosition == this->length);
}

/**
 * Creates a Sound struct and loads the correct wav file from the SD card
 *
 * @param id - id of the sound to load
 * @param buffer - buffer to save the sound wave into
 * return NULL if failed to read the wave file
 */
struct Sound* loadWavSound(char * filename) {
	struct Sound* sound;
	SDIO_lock = 1;
	int filePointer = alt_up_sd_card_fopen(filename, false);

	if (filePointer < 0) {
		alt_up_sd_card_fclose(filePointer); //close the file
		printf("sound file open failed\n");
		return NULL;
	}

	sound = loadWavHeader(filePointer);
	if (!sound) {
		alt_up_sd_card_fclose(filePointer);
		return NULL;
	}

	if(loadSoundBuffer(sound, filePointer) != 0) {
		alt_up_sd_card_fclose(filePointer);
		return NULL;
	}

	alt_up_sd_card_fclose(filePointer);
	SDIO_lock = 0;

	changeBitsPerSample(sound, DEFAULT_BITS_PER_SAMPLE);
	printf("Sound loading complete\n");

	return sound;
}

int readFileTilOffset(int filePointer, int startIndex, int offset) {
	while (startIndex < offset) {
		if (alt_up_sd_card_read(filePointer) < 0) {
			printf("read file invalid\n");
			return -1;
		}
		startIndex++;
	}
	return offset;
}

struct Sound* loadWavHeader(int filePointer) {
	int index = 0;

	if((index = readFileTilOffset(filePointer, index, NUM_CHANNELS_OFFSET)) < 0) {
		return NULL;
	}

	int numChannels;
	if((numChannels = readInt(filePointer, 2, false)) < 0)
		return NULL;
	index += 2;

	//Start reading the wav header
	int sampleRate;
	if((sampleRate = readInt(filePointer, 4, false)) < 0)
		return NULL;
	index += 4;
	int byteRate;
	if((byteRate = readInt(filePointer, 4, false)) < 0)
		return NULL;
	index += 4;

	if((index = readFileTilOffset(filePointer, index, BITS_PER_SAMPLE_OFFSET)) < 0)
		return NULL;
	int sampleSizeInBits;
	if((sampleSizeInBits = readInt(filePointer, 2, false)) < 0)
		return NULL;

	index += 2;

	if((index = readFileTilOffset(filePointer, index, DATA_LENGTH_OFFSET)) < 0)
		return NULL;

	struct AudioFormat* audioFormat = initAudioFormat(sampleRate,
			sampleSizeInBits, numChannels, byteRate);
	int read = readInt(filePointer, 4, false);
	int srcLength = (read / getSampleSizeInBytes(audioFormat)) / getNumChannels(audioFormat);
	printf("length: %u\n", srcLength);
	struct Sound* this = initSound(srcLength);
	this->audioFormat = audioFormat;
	return this;
}

struct Sound* loadSound(struct Song* this) {
	if (this == NULL)
		return NULL;
	if (strcmp(this->ext, "MP3") == 0)
		return loadMP3Sound(this->song_name);
	else if (strcmp(this->ext, "WAV") == 0)
		return loadWavSound(this->song_name);
	return NULL;
}

void setFadeInLength(struct Sound* this, unsigned int inFadeLength) {
	this->inFadePosition = inFadeLength;
}

void setFadeOutLength(struct Sound* this, unsigned int len) {
	this->outFadePosition = this->length - len;
}

/**
 * Changes the sound volume so that it can be taken into account when
 * adding the sound to the SoundMixer.
 */
void setSoundVolume(struct Sound* this, float volume) {
	this->volume = volume;
	this->fadeVolume = volume;
}

/**
 * Seeks the sound to the given position
 *
 * @param this - The sound to change
 * @param position - The position to seek to in MilliSeconds
 */
void seekSound(struct Sound* this, unsigned int position) {
	this->position = convertFromMS(position);
}

void playSound(struct Sound* sound, float volume, int startTime, int loops) {
	seekSound(sound, startTime);
	setSoundVolume(sound, volume);
	sound->loops = loops;
}

void stopSound(struct Sound* sound) {
	sound->position = 0;
}
/*
void unloadSound(struct Sound* sound) {
	if (sound == NULL)
		return;
	free(sound->buffer);
	sound->buffer = NULL;
	free(sound);
	sound = NULL;
}*/

bool checkEnd(struct Sound* this) {
	return (this->position >= this->length) && this->length != 0;
}
