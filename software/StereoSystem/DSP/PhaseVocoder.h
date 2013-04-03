/*
 * PhaseVocoder.h
 *
 *  Created on: Apr 1, 2013
 *      Author: Adam
 */

#ifndef PHASEVOCODER_H_
#define PHASEVOCODER_H_

#include "fft.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

void pitchShift(float, long, long, long, float, float *, float *);

#endif /* PHASEVOCODER_H_ */
