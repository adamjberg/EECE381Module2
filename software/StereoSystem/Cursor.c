/*
 * Cursor.c
 *
 *  Created on: 2013-01-25
 *      Author: Danny
 */

#include "Cursor.h"

int cursor_lock;

struct Cursor* initCursor(int x, int y, struct Frame* f) {
	up_dev.ps2_dev = alt_up_ps2_open_dev("/dev/ps2_0");
	up_dev.ps2_dev->timeout = 2000000;
	alt_up_ps2_clear_fifo(up_dev.ps2_dev);
	alt_up_ps2_init(up_dev.ps2_dev);
	unsigned char byte1;
	alt_up_ps2_read_data_byte_timeout(up_dev.ps2_dev, &byte1);
	//while(alt_up_ps2_read_data_byte_timeout(up_dev.ps2_dev, &byte1)!=0);
	struct Cursor* this = (struct Cursor*)malloc(sizeof(struct Cursor));
	struct Image* cursorImg;
	while(( cursorImg= loadSDImage("AR01.BMP")) == NULL);
	this->super = initObject(initRange(x, y, 10, 10), cursorImg, (void*)this);
	this->frame = f;
	int* image = (int*)malloc(sizeof(int)*100);
	memset(image, 0, sizeof(int)*100);
	this->overlapImg = initImage(image, 0, 10, 10);
	this->isLeftPressed = false;
	this->isRightPressed = false;
	enableCursorInterrupt(this);
	return this;
}

void enableCursorInterrupt(struct Cursor* this) {
	alt_up_ps2_enable_read_interrupt(up_dev.ps2_dev);
	alt_irq_register(PS2_0_IRQ, this, (void*)ps2_ISR);
	//alt_irq_enable(PS2_0_IRQ);
}
void moveCursor(struct Cursor* this, int dx, int dy) {
	setNextXY(this->super, this->super->r->x + dx, this->super->r->y - dy);
}
void updateCursor(struct Cursor* this) {
	//cursor_lock = 1;
	if(((this->super->next_r->x == this->super->r->x) && (this->super->next_r->y == this->super->r->y)) || this == NULL) return;
	draw_notransparent(this->super->r->x, this->super->r->y, this->overlapImg);
	setXY(this->super, this->super->next_r->x, this->super->next_r->y);
	memOverlap(this);
	draw(this->super->r->x, this->super->r->y, this->super->img);

}

void memOverlap(struct Cursor* this) {
	if(this->super->r->x < 0 || this->super->r->y < 0 || this->super->r->x > 315 || this->super->r->y > 235) return;
	int i, j;
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
		    *(this->overlapImg->buffer+i+10*j)=IORD_16DIRECT(pixel_buffer->buffer_start_address, (i+this->super->r->x+(j+this->super->r->y)*320)*2);
		}
	}
}

int getCursorX(struct Cursor* this) {
	return this->super->r->x;
}

int getCursorY(struct Cursor* this) {
	return this->super->r->y;
}

