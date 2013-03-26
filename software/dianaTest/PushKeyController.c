/*
 * PushKeyController.c
 *
 *  Created on: 2013-03-19
 *      Author: Adam
 */

#include "PushKeyController.h"

bool isBitSet(char num, int bit) {
	return num & 1 << bit;
}

struct PushKeyController* initPushKeyController(void) {
	struct PushKeyController* this = (struct PushKeyController*) malloc(
			sizeof(struct PushKeyController));
	this->keys_changed = 0;

	alt_irq_register(KEYS_IRQ, this, (void*) push_key_ISR);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 15);
	return this;
}

/**
 * Returns true if the key was pressed when the last ISR was called
 */
bool wasKeyJustPressed(struct PushKeyController* this, int key) {
	return isBitSet(this->keys_changed, key) && isKeyDown(key);
}

/**
 * Returns true if the key was released when the last ISR was called
 */
bool wasKeyJustReleased(struct PushKeyController* this, int key) {
	return isBitSet(this->keys_changed, key) && isKeyUp(key);
}

/**
 * The two following functions allow direct access to the current value of the keys
 */

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is down
 */
bool isKeyDown(int key) {
	return !isBitSet(IORD_8DIRECT(KEYS_BASE, 0), key);
}

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is up
 */
bool isKeyUp(int key) {
	return isBitSet(IORD_8DIRECT(KEYS_BASE, 0), key);
}
