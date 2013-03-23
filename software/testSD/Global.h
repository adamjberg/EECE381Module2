/*
 * Global.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_audio.h"
#include <alt_types.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"

#include "Graphic.h"
#include "AudioDeviceController.h"
#include "Sound.h"
#include "SoundMixer.h"
#include "ISR_vectors.h"

#define HEADERSIZE 2
#define SPACE 32
#define NEWLINE 10

extern alt_alarm alarm;
extern struct alt_up_dev up_dev;
extern alt_up_pixel_buffer_dma_dev* pixel_buffer;
extern alt_up_char_buffer_dev* char_buffer;
extern struct SoundMixer* soundMixer;
extern struct Sound* s;
extern struct Sound* s1;
extern struct Sound* s2;

enum msgType {
	STRING=0, CMD=1, PLAYLIST=2, AUDIO=3
};
#endif /* GLOBAL_H_ */
