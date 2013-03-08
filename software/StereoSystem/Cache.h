/*
 * Cache.h
 *
 *  Created on: 2013-03-08
 *      Author: danny
 */

#ifndef CACHE_H_
#define CACHE_H_
#include "Global.h"

struct Cache {
	unsigned char memory[100000]; //100k bytes
};

struct Cache* initCache();
#endif /* CACHE_H_ */
