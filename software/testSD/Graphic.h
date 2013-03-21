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
int* loadSDImage(char*);
int getColor(int, int, int);
int getColor555(int);
void draw(int, int, int*, int);
#endif /* GRAPHIC_H_ */
