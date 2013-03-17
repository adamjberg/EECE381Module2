
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

	initDatabase();
	//dBTester();

	/*syncCreateSong("LONE", 0);
	createSong("LONE", 0);
	syncCreateSong("LTWO", 0);
	createSong("LTWO", 0);*/

	initAudioDeviceController();
	initSoundMixer();
	//dBTester();
	//dbTester2();
	struct CmdScheduler* scheduler = initCmdScheduler();

	//Serial device initialization
	com = initRS232(scheduler);

	//sync database
	update();

// Testing code start

	int i;
	//dBTester();

//	cmdTester();
	//for(i = 0; i < 39999999; i++) {}
	//send((void*)temp1, STRING);
//Testing code end

	enableAudioDeviceController();


	while(1) {
	//	loadSong(song);
	//	playSong(song, 1.0, 0, 5);
//Testing code start
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp2, STRING);
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp, STRING);
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp1, STRING);
	//	for(i = 0; i < 3999999; i++) {}
		//syncPause();
	//	for(i = 0; i < 3999999; i++) {}
//Testing code end
		//for(i = 0; i < 49999999; i++) {}
	//	pauseSong(song);
	//	unloadSong(song);
		cmdProcessing(scheduler);
	}


	return 0;
}
