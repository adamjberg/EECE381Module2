/****************************************************************************
 *
 * NAME: smbPitchShift.cpp
 * VERSION: 1.2
 * HOME URL: http://www.dspdimension.com
 * KNOWN BUGS: none
 *
 * SYNOPSIS: Routine for doing pitch shifting while maintaining
 * duration using the Short Time Fourier Transform.
 *
 * DESCRIPTION: The routine takes a pitchShift factor value which is between 0.5
 * (one octave down) and 2. (one octave up). A value of exactly 1 does not change
 * the pitch. numSampsToProcess tells the routine how many samples in indata[0...
 * numSampsToProcess-1] should be pitch shifted and moved to outData[0 ...
 * numSampsToProcess-1]. The two buffers can be identical (ie. it can process the
 * data in-place). fftFrameSize defines the FFT frame size used for the
 * processing. Typical values are 1024, 2048 and 4096. It may be any value <=
 * MAX_FRAME_LENGTH but it MUST be a power of 2. osamp is the STFT
 * oversampling factor which also determines the overlap between adjacent STFT
 * frames. It should at least be 4 for moderate scaling ratios. A value of 32 is
 * recommended for best quality. sampleRate takes the sample rate for the signal
 * in unit Hz, ie. 44100 for 44.1 kHz audio. The data passed to the routine in
 * indata[] should be in the range [-1.0, 1.0), which is also the output range
 * for the data, make sure you scale the data accordingly (for 16bit signed integers
 * you would have to divide (and multiply) by 32768).
 *
 * COPYRIGHT 1999-2006 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
 *
 * 						The Wide Open License (WOL)
 *
 * Permission to use, copy, modify, distribute and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice and this license appear in all source copies.
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
 * ANY KIND. See http://www.dspguru.com/wol.htm for more information.
 *
 *****************************************************************************/

#include "PhaseVocoder.h"

void pitchShift(float newPitch, long numSampsToProcess, long fftFrameSize,
		long osamp, float sampleRate, float *inData, float *outData) {
	long gRover = false;
	double magn, phase, tmp, window, real, imag;
	double freqPerBin, expct;
	long i, k, qpd, index, inFifoLatency, stepSize, fftFrameSize2;

	float *gInFIFO = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);
	float *gOutFIFO = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);
	float *gFFTworksp = (float *) malloc(sizeof(float) * 2 * MAX_FRAME_LENGTH);
	float *gLastPhase = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH / 2 + 1);
	float *gSumPhase = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH / 2 + 1);
	float *gOutputAccum = (float *) malloc(sizeof(float) * 2 * MAX_FRAME_LENGTH);
	float *gAnaFreq = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);
	float *gAnaMagn = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);
	float *gSynFreq = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);
	float *gSynMagn = (float *) malloc(sizeof(float) * MAX_FRAME_LENGTH);

	memset(gInFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gOutFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gFFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
	memset(gLastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
	memset(gSumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
	memset(gOutputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaFreq, 0, MAX_FRAME_LENGTH * sizeof(float));
	memset(gAnaMagn, 0, MAX_FRAME_LENGTH * sizeof(float));

	fftFrameSize2 = fftFrameSize / 2;
	stepSize = fftFrameSize / osamp;
	freqPerBin = sampleRate / (double) fftFrameSize;
	expct = 2. * M_PI * (double) stepSize / (double) fftFrameSize;
	inFifoLatency = fftFrameSize - stepSize;
	if (gRover == false)
		gRover = inFifoLatency;

	/* main processing loop */
	for (i = 0; i < numSampsToProcess; i++) {

		/* As long as we have not yet collected enough data just read in */
		gInFIFO[gRover] = inData[i];
		outData[i] = gOutFIFO[gRover - inFifoLatency];
		gRover++;

		/* now we have enough data for processing */
		if (gRover >= fftFrameSize) {
			gRover = inFifoLatency;

			/* do windowing and re,im interleave */
			for (k = 0; k < fftFrameSize; k++) {
				window = -.5 * cos(2. * M_PI * (double) k
						/ (double) fftFrameSize) + .5;
				gFFTworksp[2 * k] = gInFIFO[k] * window;
				gFFTworksp[2 * k + 1] = 0.;
			}

			/* ***************** ANALYSIS ******************* */
			/* do transform */
			fft(gFFTworksp, fftFrameSize, -1);

			/* this is the analysis step */
			for (k = 0; k <= fftFrameSize2; k++) {

				/* de-interlace FFT buffer */
				real = gFFTworksp[2 * k];
				imag = gFFTworksp[2 * k + 1];

				/* compute magnitude and phase */
				magn = 2. * sqrt(real * real + imag * imag);
				phase = atan2(imag, real);

				/* compute phase difference */
				tmp = phase - gLastPhase[k];
				gLastPhase[k] = phase;

				/* subtract expected phase difference */
				tmp -= (double) k * expct;

				/* map delta phase into +/- Pi interval */
				qpd = tmp / M_PI;
				if (qpd >= 0)
					qpd += qpd & 1;
				else
					qpd -= qpd & 1;
				tmp -= M_PI * (double) qpd;

				/* get deviation from bin frequency from the +/- Pi interval */
				tmp = osamp * tmp / (2. * M_PI);

				/* compute the k-th partials' true frequency */
				tmp = (double) k * freqPerBin + tmp * freqPerBin;

				/* store magnitude and true frequency in analysis arrays */
				gAnaMagn[k] = magn;
				gAnaFreq[k] = tmp;

			}

			/* ***************** PROCESSING ******************* */
			/* this does the actual pitch shifting */
			memset(gSynMagn, 0, fftFrameSize * sizeof(float));
			memset(gSynFreq, 0, fftFrameSize * sizeof(float));
			for (k = 0; k <= fftFrameSize2; k++) {
				index = k * newPitch;
				if (index <= fftFrameSize2) {
					gSynMagn[index] += gAnaMagn[k];
					gSynFreq[index] = gAnaFreq[k] * newPitch;
				}
			}

			/* ***************** SYNTHESIS ******************* */
			/* this is the synthesis step */
			for (k = 0; k <= fftFrameSize2; k++) {

				/* get magnitude and true frequency from synthesis arrays */
				magn = gSynMagn[k];
				tmp = gSynFreq[k];

				/* subtract bin mid frequency */
				tmp -= (double) k * freqPerBin;

				/* get bin deviation from freq deviation */
				tmp /= freqPerBin;

				/* take osamp into account */
				tmp = 2. * M_PI * tmp / osamp;

				/* add the overlap phase advance back in */
				tmp += (double) k * expct;

				/* accumulate delta phase to get bin phase */
				gSumPhase[k] += tmp;
				phase = gSumPhase[k];

				/* get real and imag part and re-interleave */
				gFFTworksp[2 * k] = magn * cos(phase);
				gFFTworksp[2 * k + 1] = magn * sin(phase);
			}

			/* zero negative frequencies */
			for (k = fftFrameSize + 2; k < 2 * fftFrameSize; k++)
				gFFTworksp[k] = 0.;

			/* do inverse transform */
			fft(gFFTworksp, fftFrameSize, 1);

			/* do windowing and add to output accumulator */
			for (k = 0; k < fftFrameSize; k++) {
				window = -.5 * cos(2. * M_PI * (double) k
						/ (double) fftFrameSize) + .5;
				gOutputAccum[k] += 2. * window * gFFTworksp[2 * k]
						/ (fftFrameSize2 * osamp);
			}
			for (k = 0; k < stepSize; k++)
				gOutFIFO[k] = gOutputAccum[k];

			/* shift accumulator */
			memmove(gOutputAccum, gOutputAccum + stepSize, fftFrameSize
					* sizeof(float));

			/* move input FIFO */
			for (k = 0; k < inFifoLatency; k++)
				gInFIFO[k] = gInFIFO[k + stepSize];
		}
	}

	free(gInFIFO);
	free(gOutFIFO);
	free(gFFTworksp);
	free(gLastPhase);
	free(gSumPhase);
	free(gOutputAccum);
	free(gAnaFreq);
	free(gAnaMagn);
	free(gSynFreq);
	free(gSynMagn);
}
