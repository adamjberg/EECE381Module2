/*
 * Object.h
 *
 *  Created on: 2013-03-04
 *      Author: danny
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include "Global.h"

struct Object {
	struct Range* r;
	struct Image* img;
	void* subObj;
};

struct Object* initObject(struct Range*, struct Image*, void*);
void setXY(struct Object*, int, int);
#endif /* OBJECT_H_ */
