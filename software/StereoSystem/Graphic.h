/*
 * Graphic.h
 *
 *  Created on: 2013-03-21
 *      Author: danny
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_
#include "Global.h"

void initVGA();
bool loadSDImage(char*, int**);
int getColor(int, int, int);
int getColor555(int);
#endif /* GRAPHIC_H_ */
