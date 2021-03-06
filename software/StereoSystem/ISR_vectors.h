/*
 * ISR_vectors.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef ISR_VECTORS_H_
#define ISR_VECTORS_H_

#include "Global.h"
#include "altera_up_avalon_audio.h"
#include "sound/AudioDeviceController.h"
#include "sound/SoundMixer.h"
#include "PushKeyController.h"

struct Cursor;
struct PushKeyController;
/*
 * Structure that hold a pointer to each open I/O device
 */
struct alt_up_dev {
	alt_up_rs232_dev *RS232_dev;
	alt_up_audio_dev *audio_dev;
	alt_up_ps2_dev *ps2_dev;
	alt_up_sd_card_dev *device_reference;
};

void push_key_ISR(struct PushKeyController*, unsigned int);
void audio_ISR(alt_up_audio_dev*, unsigned int);
void ps2_ISR(struct Cursor*);
alt_u32 RS232_ISR(void*);
void animate_ISR(struct Cursor*);
void mix_ISR(void);
#endif /* ISR_VECTORS_H_ */
