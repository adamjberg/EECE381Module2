/*
 * AudioDeviceController.c
 *
 *  Created on: 2013-03-06
 *      Author: Adam
 */

#include "AudioDeviceController.h"

/**
 * Mandatory setup function in order for audio to function
 */
void av_config_setup()
{
	alt_up_av_config_dev * av_config =
	alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	while (!alt_up_av_config_read_ready(av_config));
}

void initAudioDeviceController() {
	av_config_setup();
	up_dev.audio_dev = alt_up_audio_open_dev(AUDIO_NAME);
	alt_up_audio_reset_audio_core(up_dev.audio_dev);
}

void enableAudioDeviceController() {
	alt_up_audio_enable_write_interrupt(up_dev.audio_dev);
	alt_irq_register(AUDIO_IRQ, up_dev.audio_dev, (void*) audio_ISR);
	//alt_irq_enable(AUDIO_IRQ);
}

void disableAudioDeviceController() {
	alt_up_audio_disable_write_interrupt(up_dev.audio_dev);
}
