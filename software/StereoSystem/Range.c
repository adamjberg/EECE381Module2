/*
 * Range.c
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */

#include "Range.h"

struct Range* initRange(int x, int y, int w, int h) {
	struct Range* this = (struct Range*)malloc(sizeof(struct Range));
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	return this;
}
