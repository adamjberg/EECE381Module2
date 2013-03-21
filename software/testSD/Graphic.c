/*
 * Graphic.c
 *
 *  Created on: 2013-03-21
 *      Author: danny
 */
#include "Graphic.h"

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev* char_buffer;
/*
* this function clear the screen and set up pixel buffers for graphics
*/
void initVGA() {
	// Use the name of your pixel buffer DMA core
	pixel_buffer =alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma_0");

	//text on screen initialization
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	// Set the background buffer address � Although we don�t use thebackground,
	// they only provide a function to change the background buffer address, so
	// we must set that, and then swap it to the foreground.
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320*240*2);

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr1);
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	// Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr2);
	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	//Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	//Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

}

/*
* Load bitmap image from SD card. Fixed problem with color coding
* file name is required to be upper-case and bitmap pixels has to be even size x even size
*/
int* loadSDImage(char* filename) {
	int i, j, bytes = 0, offset = 0, byte = 0;
	int a, b;
	int height = 0, width = 0;
	char bmpId[3];
	int* result = NULL;
	memset(bmpId, 0, 3);
	short int file_pointer = alt_up_sd_card_fopen(filename, false);
	if(file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return NULL;
	}
	//Start reading the bitmap header
	while(bytes < 2) {
		if((bmpId[bytes++] = alt_up_sd_card_read(file_pointer)) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			printf("fail reading %s\n", filename);
			return NULL;
		}
	}
	if(strcmp(bmpId, "BM") != 0) {
		alt_up_sd_card_fclose(file_pointer);
		printf("fail reading %s at ID %s\n", filename, bmpId);
		return NULL;
	}
	short int temp;
	while(bytes < 10) {
		if((temp = alt_up_sd_card_read(file_pointer)) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			printf("fail reading %s\n", filename);
			return NULL;
		} printf("%x ", temp);
		bytes++;
	} printf("\n");

	if((offset = alt_up_sd_card_read(file_pointer))< 0) {
		alt_up_sd_card_fclose(file_pointer);
		return NULL;
	} printf("offset: %x\n", offset);
	bytes++;
	while(bytes < offset){
		if(bytes == 18) {
			if((width = alt_up_sd_card_read(file_pointer))< 0) {
				alt_up_sd_card_fclose(file_pointer);
				return false;
			} printf("width: %d, ", width);
		} else if(bytes == 22) {
			if((height = alt_up_sd_card_read(file_pointer))< 0) {
				alt_up_sd_card_fclose(file_pointer);
				return false;
			} printf("height: %d\n", height);
			result = (int*)malloc(sizeof(int)*width*height);
		} else if((temp = alt_up_sd_card_read(file_pointer)) < 0) {
			alt_up_sd_card_fclose(file_pointer);
			return NULL;
		} //printf("%d %x\n", bytes, temp);
		bytes++;
	}
	//Start reading the pixel data
	for(j = height-1; j >= 0; j--) {
		for(i = 0; i < width; i++) {
			a = alt_up_sd_card_read(file_pointer);
			b = alt_up_sd_card_read(file_pointer);
			if(a < 0 || b < 0) {
				printf("%s invalid at pixel[%d, %d]!\n", filename, i, j);
				free(result);
				result = NULL;
				alt_up_sd_card_fclose(file_pointer);
				return NULL;
			}
			byte = getColor555(b*256+a);
			*(result + j*height+i) = byte;
		}
	}
	alt_up_sd_card_fclose(file_pointer);
	return result;
}

void draw(int pos_x, int pos_y, int* img, int size) {
	int i, j;
	if(pos_x < 0 || pos_y < 0 || pos_x >= 320 || pos_y >= 240) return;
		for(i = 0; i < size; i++) {
			for(j = 0; j < size; j++) {
				if(*(img + j*size+i) != 0) {
					IOWR_16DIRECT(pixel_buffer->buffer_start_address, ((pos_y+j)*320+pos_x+i)*2, *(img + j*size+i));
			}
		}
	}
}
/*helper function to convert 32 bit color code to 16 bit color*/

int getColor(int red, int green, int blue) {
	return (int)(((red>>3)<<11) + ((green>>2)<<5) + (blue>>3));
}
int getColor555(int color555) {
	int color = color555&0x7FFF;
	return (color&0x7C00)*2+(color&0x03E0)*2+(color&0x1F);
}
