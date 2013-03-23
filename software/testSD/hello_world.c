/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#include "Global.h"

struct Sound* s;
struct Sound* s1;
struct Sound* s2;
bool loadSDCard(alt_up_sd_card_dev* device) {
	if((device = alt_up_sd_card_open_dev("/dev/SD_Card")) != NULL) {
		if (alt_up_sd_card_is_Present()) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
				return true;
			} else {
				printf("Unknown file system.\n");
			}
		}
	}
	return false;
}

int main()
{
	alt_up_sd_card_dev *device_reference = NULL;
		while(!loadSDCard(device_reference)) {
			printf("SD card is not connected.\n");
		}
		initVGA();
		int* pic;

		while((pic = loadSDImage("WIN.BMP")) == NULL);
		draw(60, 0, pic, 240);

		initAudioDeviceController();
		initSoundMixer();

		//enableAudioDeviceController();

		s = loadWavSound("LONE.WAV");
		int i;
		//for(i = 0; i < 100; i ++) {
			//loadToSoundBuffer(s, soundMixer->endIndex++);
		//}
		s1 = loadWavSound("LTWO.WAV");
		s2 = loadWavSound("LTHREE.WAV");
		updateMixer();
		enableAudioDeviceController();
	while(1) {

		updateMixer();
		if(soundMixer->indexSize <= 0)
			disableAudioDeviceController();
		//disableAudioDeviceController();
			//s->position = 0;

		//if(soundMixer->currIndex == soundMixer->endIndex+1) {
			//disableAudioDeviceController();
		//}
	}
  return 0;
}
