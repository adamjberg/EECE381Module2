#include "Global.h"

int main()
{
	com = initRS232();
	char* temp = "Constructs a new String by decoding the specified subarray of bytes using the specified charset. The length of the new String is a function of the charset, and hence may not be equal to the length of the subarray. ";
	char* temp1 = "this is a testing message";
	char* temp2 = "I hate debugging and testing";
	int i;
	for(i = 0; i < 39999999; i++) {}
	send((void*)temp1, STRING);

	while(1) {
		for(i = 0; i < 9999999; i++) {}
		send((void*)temp2, STRING);
		for(i = 0; i < 9999999; i++) {}
		send((void*)temp, STRING);
		for(i = 0; i < 9999999; i++) {}
		send((void*)temp1, STRING);
	//	usleep(100000);
	//	printf("%s\n", com.data);
	}


	return 0;
}
