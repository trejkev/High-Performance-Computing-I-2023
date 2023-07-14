// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define FAIL    0

typedef struct {
    float real;
    float imag;
} Complex;

/* Sequential implementation of DFT */
Complex* dft(float* fSampledSignal, size_t iSamplesQty, long* lTimes,
    int iRanksQty, int iMyRank);

/* Pre-processor to bring the fft the input signal formatted to be complex */
Complex* fft_preprocessor(float *signal, size_t iSamplesQty, long* lTimes);

/* Recursive implementation of the fast fourier transform, following 
 * Cooley-Tukey FFT algorithm. Its pseudocode was taken from the following 
 * reference, and validated with the dft implementation already in this code:
 * https://www.educative.io/answers/what-is-fft-cooley-tukey-algorithm
 */
Complex* fft(Complex *signal, size_t iSamplesQty);

/* Short macro to deal with time readings, it encapsulates the conversions
 * needed to ger the time measurements in nanoseconds.
 */
long timeReader();

/* Used to generate the input signal, it is capable of generating multiple
 * basic signals, like combinations of sinusoids, square signals,  and sawtooth
 * signals.
 */
int signalGenerator(int argc, char *argv[]);