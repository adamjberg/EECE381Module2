/*
 * AudioDeviceController.h
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#ifndef AUDIODEVICECONTROLLER_H_
#define AUDIODEVICECONTROLLER_H_

#include <stdlib.h>
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "sys/alt_irq.h"
#include "../ISR_vectors.h"
#include "../Global.h"

void initAudioDeviceController();
void enableAudioDeviceController();
void disableAudioDeviceController();

#endif /* AUDIODEVICECONTROLLER_H_ */
