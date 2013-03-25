
#include "Global.h"

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
	//SD device initialization
	alt_up_sd_card_dev *device_reference = NULL;
	while(!loadSDCard(device_reference)) {
		printf("SD card is not connected.\n");
	}

	initVGA();
	initAudioDeviceController();
	initSoundMixer();

	struct CmdScheduler* scheduler = initCmdScheduler();

	//Serial device initialization
	com = initRS232(scheduler);

	initPushKeyController();

	initDatabase();
	//sync database
	update();

	initMemory();

	//Test VGA Output
//	struct Image* testImg;
//	while((testImg = loadSDImage("TEST.BMP")) == NULL);
//	draw(35, 35, testImg);
//	killImage(testImg);
	alt_up_char_buffer_string(char_buffer, "Initialization Completed", 27, 5);
	//graphicTester();
	//Test End

	while(1) {
		cmdProcessing(scheduler);
		updateMixer();
	}


	return 0;
}
