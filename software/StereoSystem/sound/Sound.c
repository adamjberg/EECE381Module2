/*
 * Sound.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "Sound.h"

#define DEFAULT_SAMPLE_RATE 32000
#define DEFAULT_BITS_PER_SAMPLE 24

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

	return (unsigned int)(value * 32);
}

/**
 * Helper function to convert a value to its millisecond equivalent based on the
 * sampling rate
 */
unsigned int convertToMS(unsigned int value) {
	return (unsigned int) ((value * 1000)/ DEFAULT_SAMPLE_RATE);
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
		if(sound->position > sound->length) {
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

static int decodeMP3(struct Sound* this, unsigned char *start, unsigned long length) {
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
	int i;
	unsigned char * buf = NULL;
	int byte = 0;
	unsigned int size = 0;

	printf("Reading %s file size\n", file);
	size = 0;
	filePointer = alt_up_sd_card_fopen(file, false);
	while(size != 33) {
		byte = alt_up_sd_card_read(filePointer);
		size++;
	}
	size = (alt_up_sd_card_read(filePointer) << (3*8)) +
			(alt_up_sd_card_read(filePointer) << (2*8)) +
			(alt_up_sd_card_read(filePointer) << 8) +
			alt_up_sd_card_read(filePointer);
	/*
	while (byte != -1) {
		byte = alt_up_sd_card_read(filePointer);
		printf("%i %x\n", size, byte);
		if (size % (1024 * 1024) == 0) {
			printf("%d MB | ", size / (1024 * 1024));
		}
		size++;
	}*/
	alt_up_sd_card_fclose(filePointer);
	printf("File Size: %d bytes\n", size);
	if( size*4 > MAX_CACHE_MEMORY) {
		printf("FILE size too big");
		return NULL;
	}
	if(memMgr.used_memory + size*4 > MAX_CACHE_MEMORY) {
			freeMem(size*4);
		}
	struct Sound* sound = initSound(size*4);

	buf = malloc(size);
	if (!buf) {
		printf("Malloc failed\n");
		exit(0);
	}

	printf("Preloading mp3\n");
	filePointer = alt_up_sd_card_fopen(file, false);
	for (i = 0; i < size; i++) {
		buf[i] = alt_up_sd_card_read(filePointer);
		if (i % (1024 * 1024) == 0) {
			printf("%d MB | ", i / (1024 * 1024));
		}
	}
	alt_up_sd_card_fclose(filePointer);
	printf("\nPreloading complete\n");

	decodeMP3(sound, buf, size);
	return sound;
}

/**
 * Loads the sound using linear interpolation to convert to correct sample rate
 *
 * @param filePointer - current filePointer
 * @param srcLength - Length of wav file
 * @param toSampleRate - sampling rate to convert to
 * @param fromSampleRate - current sound sample rate
 */
struct Sound* loadSoundBuffer(int* property, int bytesPerSample, int srcLength, int toSampleRate, int fromSampleRate) {
	int i = 0;
	int destLength = ((float) srcLength * toSampleRate) / fromSampleRate;
	float x0 = 0, x1 = 0, x = 0;
	int y0 = 0, y1 = 0;
	int j = 0;
	if(memMgr.used_memory + destLength > MAX_CACHE_MEMORY) {
		freeMem(destLength);
	}
	struct Sound* this = initSound(destLength);

	if(toSampleRate != fromSampleRate) {
		for(i = 0; i < srcLength; i++) {
			if(j >= destLength) break;
			x1 = i/(float)fromSampleRate;
			if((y1 = readInt(property[3], bytesPerSample)) < 0) {
				free(this->buffer);
				free(this);
				return NULL;
			}
			if(y1 > 0x07FFFFF) {
				y1 = y1 | 0xFF000000;
			}
			if(x <= x1) {
				if(x == x1) {
					this->buffer[j++] = y1;
				} else if(x < x1) {
					this->buffer[j++] = (y1 - y0) * (x - x0) / (x1 - x0) + y0;
					if(this->buffer[j-1] > 0x07FFFFF)
						this->buffer[j-1] &= 0x00FFFFFF;
				}
				x = j/(float)toSampleRate;
			}
			x0 = x1;
			y0 = y1;
		}
	} else {
		for(i = 0; i < destLength; i++) {
			if((this->buffer[i] = readInt(property[3], bytesPerSample)) < 0) {
				free(this->buffer);
				free(this);
				return NULL;
			}
		}
	}
	return this;
}

/**
 * Checks to see if the values need to be shifted to match given bytesPerSample
 * @param this - sound to change values of
 * @param bitsPerSampleTo - Number of bits per sample to convert to
 * @param bitsPerSampleFrom - Number of bits used for current sample
 */
void changeBitsPerSample(struct Sound* this, int bitsPerSampleTo, int bitsPerSampleFrom) {
	if (bitsPerSampleTo == bitsPerSampleFrom || this == NULL)
		return;

	int i;
	int numToShift = bitsPerSampleTo - bitsPerSampleFrom;

	for (i = 0; i < this->length; i++) {
		if( numToShift > 1)
			this->buffer[i] = this->buffer[i] << numToShift;
		else
			this->buffer[i] = this->buffer[i] >> -numToShift;
	}
}

/**
 * Helper function to read multiple bytes and return the representative int value
 */
int readInt(int file_pointer, int numBytesToRead) {
	int ret = 0;
	int i;
	int bytes[numBytesToRead];
	memset(bytes, 0, numBytesToRead);
	for (i = 0; i < numBytesToRead; i++) {
		if((bytes[i] = alt_up_sd_card_read(file_pointer)) < 0) {
			printf("read file invalid\n");
			return -1;
		}
		ret |= (bytes[i] << (8 * i));
	}
	return ret;
}

void clearSoundBuffer(struct Sound* sound) {
	int i;
	for (i = 0; i < sound->length; i++) {
		sound->buffer[i] = 0;
	}
}

struct Sound* initSound(unsigned int length) {
	struct Sound* this = (struct Sound*) malloc(sizeof(struct Sound));
	if(!this)
		printf("Failed to allocate space for sound\n");
	this->length = length;
	this->position = 0;
	this->buffer = (unsigned int*) malloc(sizeof(int) * length);
	if(!this->buffer)
		printf("Failed to allocate sound buffer\n");
	this->playing = false;
	this->volume = 1;
	this->fadeVolume = 1;
	this->inFadePosition = 0;
	this->outFadePosition = this->length;
	this->loading_pos = 0;
	clearSoundBuffer(this);
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
	int* property;
	SDIO_lock = 1;

	if((property = loadWavHeader(filename)) == NULL) {
		return NULL;
	}

	sound = loadSoundBuffer(property, property[1]/8, property[2], DEFAULT_SAMPLE_RATE, property[0]);

	if(sound == NULL) {
		alt_up_sd_card_fclose(property[3]);
		free(property);
		property = NULL;
		return NULL;
	}

	alt_up_sd_card_fclose(property[3]);
	SDIO_lock = 0;

	changeBitsPerSample(sound, DEFAULT_BITS_PER_SAMPLE, property[1]);
	free(property);
	property = NULL;

	printf("Sound loading complete\n");

	return sound;
}

int* loadWavHeader(char* filename) {
	int index = 0;
	char temp;
	int* ret = (int*)malloc(sizeof(int)*4);
	printf("loading sound: %s\n", filename);
	int file_pointer = alt_up_sd_card_fopen(filename, false);
	if (file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		printf("sound file open failed\n");
		return NULL;
	}
	//Start reading the wav header
	while (index < SAMPLE_RATE_OFFSET) {
		temp = alt_up_sd_card_read(file_pointer);
	//	printf("%d %x\n", index, temp);
		index++;
	}

	int sampleRate = readInt(file_pointer, 4);
	index += 4;
	printf("sample rate: %d\n", sampleRate);
	ret[0] = sampleRate;
	while (index < BITS_PER_SAMPLE_OFFSET) {
		temp = alt_up_sd_card_read(file_pointer);
	//	printf("%d %x\n", index, temp);
		index++;
	}

	int bits_per_sample = readInt(file_pointer, 2);
	int bytes_per_sample = bits_per_sample / 8;
	ret[1] = bits_per_sample;
	index += 2;
	printf("bits/sample %d\n", bits_per_sample);
	while (index < DATA_LENGTH_OFFSET) {
		if((temp = alt_up_sd_card_read(file_pointer)) < 0) {
			printf("read file invalid\n");
			alt_up_sd_card_fclose(file_pointer);
			free(ret);
			ret = NULL;
			return NULL;
		}
	//	printf("%d %x\n", index, temp);
		index++;
	}

	int srcLength = readInt(file_pointer, 4) / bytes_per_sample;
	ret[2] = srcLength;
	if(srcLength == -1) {
		printf("read file invalid\n");
		alt_up_sd_card_fclose(file_pointer);
		free(ret);
		ret = NULL;
		return NULL;
	}
	printf("length: %u\n", srcLength);
	ret[3] = file_pointer;
	return ret;
}

struct Sound* loadSound(struct Song* this) {
	if(this == NULL) return NULL;
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
 *
 * IMPORTANT: This allows the sound to maintain all of its data
 * even if the volume is brought to 0 and then back to 1
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

void updatePos(struct Sound* this) {
	this->position += 96;
	if(this->position > this->length)
		this->position = this->length;
}
void playSound(struct Sound* sound, float volume, int startTime, int loops) {

	seekSound(sound, startTime);
	setSoundVolume(sound, volume);
	sound->playing = true;
	sound->loops = loops;
}

void pauseSound(struct Sound* sound) {
	sound->playing = false;
}

void resumeSound(struct Sound* sound) {
	sound->playing = true;
}

void stopSound(struct Sound* sound) {
	sound->playing = false;
	sound->position = 0;
}

void unloadSound(struct Sound* sound) {
	if(sound == NULL) return;
	free(sound->buffer);
	sound->buffer = NULL;
	free(sound);
	sound = NULL;
}

bool checkEnd(struct Sound* this) {
	return (this->position >= this->length) & this->length != 0;
}
