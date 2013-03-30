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
	this->next_r = initRange(r->x, r->y, r->width, r->width);
	this->img = img;
	this->subObj = subObj;
	return this;
}

void setXY(struct Object* this, int x, int y) {
	if(this == NULL || x <= 0 || y <= 0 || x >= 310 || y >= 230) return;
	this->r->x = x;
	this->r->y = y;
}

void setNextXY(struct Object* this, int x, int y) {
	if(this == NULL) return;
	this->next_r->x = x;
	this->next_r->y = y;
}
