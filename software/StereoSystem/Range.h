/*
 * Range.h
 *
 *  Created on: 2013-03-24
 *      Author: danny
 */

#ifndef RANGE_H_
#define RANGE_H_
#include "Global.h"

struct Range {
	int x, y;
	int height, width;
};

struct Range* initRange(int, int, int, int);
#endif /* RANGE_H_ */
