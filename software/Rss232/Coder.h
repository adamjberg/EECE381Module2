/*
 * Coder.h
 *
 *  Created on: 2013-03-04
 *      Author: danny
 */

#ifndef CODER_H_
#define CODER_H_
#include "Global.h"

void encodeString(char*, struct Queue*);

void* decode(struct Queue*);
void* decodeString(struct Packet**, int);
void* decodeCmd(struct Packet**, int);
void* decodePlaylist(struct Packet**, int);
#endif /* CODER_H_ */
