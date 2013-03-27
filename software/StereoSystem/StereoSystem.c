
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
	initMemory();
	//sync database
	//dBTester();
	update();


	struct Cursor* cursor = initCursor(10, 100);
	//Test VGA Output
	struct Image* testImg;
	//struct Image* testImg1;
	while((testImg = loadSDImage("TEST.BMP")) == NULL);
	//while((testImg1 = loadSDImage("ART3.BMP")) == NULL);
	draw(35, 35, testImg);
	//draw(0, 20, testImg1);
	killImage(testImg);
	alt_up_char_buffer_string(char_buffer, "Initialization Completed", 27, 5);
	//graphicTester();
	//Test End

	int i = 2;
	/*playSong(db.songs[1], 100, 0, 0);
	updateMixer();
	enableAudioDeviceController();*/
	syncPlay(1, 100, 0);
	float x = getCursorX(cursor);
	unsigned char byte1, byte2, byte3;
	while(1) {
		cmdProcessing(scheduler);
		updateMixer();

		//i = soundTester(i);

		updateCursor(cursor, (int)x, 100);
		x+=0.005;
		if(x >= 310)
			x = 0;

	/*	if(alt_up_ps2_read_data_byte(up_dev.ps2_dev, &byte1) ==0) {
				printf("byte1 %x\n", byte1);

				while(alt_up_ps2_read_data_byte(up_dev.ps2_dev, &byte2) != 0) {
				} printf("byte2 %x\n", byte2);
				while(alt_up_ps2_read_data_byte(up_dev.ps2_dev, &byte3) !=0 ){

				}
				printf("byte3 %x\n", byte3);
		}*/
	}


	return 0;
}
