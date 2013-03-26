/*
 * PushKeyController.h
 *
 *  Created on: 2013-03-19
 *      Author: Adam
 */

#ifndef PUSHKEYCONTROLLER_H_
#define PUSHKEYCONTROLLER_H_

#include "ISR_vectors.h"
#include "altera_avalon_pio_regs.h"
#include "stdbool.h"
#include "stdlib.h"
#include "system.h"

struct PushKeyController {
	volatile int keys_changed;
};

struct PushKeyController* initPushKeyController(void);
void key_ISR(struct PushKeyController*, unsigned int);
bool wasKeyJustPressed(struct PushKeyController*, int);
bool wasKeyJustReleased(struct PushKeyController*, int);

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is down
 */
bool isKeyDown(int key);

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is up
 */
bool isKeyUp(int key);

#endif /* PUSHKEYCONTROLLER_H_ */
