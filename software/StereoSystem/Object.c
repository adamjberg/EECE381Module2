/*
 * Object.c
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */

#include "Object.h"

struct Object* initObject(struct Range* r, struct Image* img, void* subObj) {
	struct Object* this = (struct Object*)malloc(sizeof(struct Object));
	this->r = r;
	this->img = img;
	this->subObj = subObj;
	return this;
}

void setXY(struct Object* this, int x, int y) {
	if(this == NULL) return;
	this->r->x = x;
	this->r->y = y;
}
