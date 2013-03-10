
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

	struct CmdScheduler* scheduler = initCmdScheduler();

	//Serial device initialization
	com = initRS232(scheduler);

// Testing code start
	char* temp = "Constructs a new String by decoding the specified subarray of bytes using the specified charset. The length of the new String is a function of the charset, and hence may not be equal to the length of the subarray. ";
	char* temp1 = "this is a testing message";
	char* temp2 = "I hate debugging and testing";

	int i;
	dBTester();

//	cmdTester();
	//for(i = 0; i < 39999999; i++) {}
	//send((void*)temp1, STRING);
//Testing code end


	while(1) {
//Testing code start
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp2, STRING);
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp, STRING);
	//	for(i = 0; i < 9999999; i++) {}
	//	send((void*)temp1, STRING);
		for(i = 0; i < 3999999; i++) {}
		syncPause();
		for(i = 0; i < 3999999; i++) {}
		syncPlay("a new song", 5);
//Testing code end

		cmdProcessing(scheduler);
	}


	return 0;
}
