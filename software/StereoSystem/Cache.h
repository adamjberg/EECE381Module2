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
	volatile unsigned int memory[1500000]; //6 Mbytes
};

struct Cache* initCache();
#endif /* CACHE_H_ */
