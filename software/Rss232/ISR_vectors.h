/*
 * ISR_vectors.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef ISR_VECTORS_H_
#define ISR_VECTORS_H_

#include "Global.h"

/*
 * Structure that hold a pointer to each open I/O device
 */
struct alt_up_dev {
	alt_up_rs232_dev *RS232_dev;
	alt_up_audio_dev *audio_dev;
};

alt_u32 RS232_ISR(void*);
#endif /* ISR_VECTORS_H_ */
