/*
 * Global.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_audio.h"

#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"

#include "ISR_vectors.h"
#include "RS232.h"
#include "Queue.h"
#include "Packet.h"
#include "Object.h"
#include "Coder.h"

#define HEADERSIZE 2

extern alt_alarm alarm;
extern struct alt_up_dev up_dev;
extern struct RS232 com;
extern int queue_lock;

enum msgType {
	STRING=0, CMD=1, PLAYLIST=2, AUDIO=3
};
#endif /* GLOBAL_H_ */
