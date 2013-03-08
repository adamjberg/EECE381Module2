/*
 * Cache.c
 *
 *  Created on: 2013-03-08
 *      Author: danny
 */

#include "Cache.h"

struct Cache* initCache() {
	struct Cache* this = (struct Cache*)malloc(sizeof(struct Cache));

	return this;
}
