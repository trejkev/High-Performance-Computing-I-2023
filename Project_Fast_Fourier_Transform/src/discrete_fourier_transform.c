// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <complex.h>

#include <math.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 0

#ifndef PI
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#endif

float complex*
    fastFourierTransform(float complex* fcSampledSignal, size_t iSamplesQty);

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName;
    float complex* fcSamplesBuffer;
    size_t iThreadsQty, iReplicas;
    size_t iSamplingFrequency, iSamplesQty;
    size_t iNyquistLimit;
    

    // -------------------- //
    // -- GET PARAMETERS -- //
    // -------------------- //
    if (argc == 4) {
        sFileName = (char*)calloc(300, sizeof(char));
        sscanf(argv[1], "%s", sFileName);
        sscanf(argv[2], "%zu", &iThreadsQty);
        sscanf(argv[3], "%zu", &iReplicas);
        printf("Input file: %s\n", sFileName);
        printf("Threads quantity: %zu\n", iThreadsQty);
        printf("Replicas: %zu\n", iReplicas);
    } else {
        printf("Missing arguments!\n");
        return FAIL;
    }

    // --------------------- //
    // -- READ INPUT FILE -- //
    // --------------------- //
    if ((fptr = fopen(sFileName, "r")) == NULL) {
        printf("Error opening file %s\n", sFileName);
        return FALSE;
    } else {
        int iLineCounter = 0;
        int iInputFileLength = 3;
        while (iLineCounter < iInputFileLength) {
            if (iLineCounter == 0) {
                fscanf(fptr, "%zu", &iSamplingFrequency);
                printf("Sampling frequency: %zu\n", iSamplingFrequency);
            } else if (iLineCounter == 1) {
                fscanf(fptr, "%zu", &iSamplesQty);
                printf("Samples quantity: %zu\n", iSamplesQty);
                iInputFileLength += iSamplesQty;
                iNyquistLimit = iSamplesQty/2;
                fcSamplesBuffer =
                    (float complex*)calloc(iSamplesQty, sizeof(float complex));
            } else {
                float auxSaver;
                fscanf(fptr, "%f", &auxSaver);
                fcSamplesBuffer[iLineCounter-2] = auxSaver + 0.0 * I;
            }
            iLineCounter++;
        }
        printf("Samples are: \n");
        for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
            printf("    Sample %zu: %f + %fi\n", iSample, 
                creal(fcSamplesBuffer[iSample]),
                cimag(fcSamplesBuffer[iSample]));
        }
    }
    fclose(fptr);

    // Compute the FFT
    float complex* fcSpectrum =
        fastFourierTransform(fcSamplesBuffer, iSamplesQty);

    printf("Fast Fourier Transform coefficients:\n");
    fptr = fopen("./results/results.csv", "w");
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        float fReal = creal(fcSpectrum[iSample])/iNyquistLimit;
        float fImag = cimag(fcSpectrum[iSample])/iNyquistLimit;
        float fAngle = carg(fcSpectrum[iSample]);
        float fAmplitude = sqrt(fReal*fReal + fImag*fImag);
        printf("F[%zu Hz] = %.8f + %.8fi -> amplitude = %.8f, angle %.8f rad\n",
        iSample + 1, fReal, fImag, fAmplitude, fAngle);
        fprintf(fptr, "%zu;%.8f;%.8f;%.8f;%.8f\n",
        iSample + 1, fReal, fImag, fAmplitude, fAngle);
    }

    free(fcSpectrum); // Comes from memory reserved into FFT
    free(fcSamplesBuffer);
    free(sFileName);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//                      ITERATIVE IMPLEMENTATION OF FFT                       //
////////////////////////////////////////////////////////////////////////////////
float complex*
    fastFourierTransform(float complex* fcSampledSignal, size_t iSamplesQty) {
    // FFT frequency components are stored here
    float complex* fcSpectrum =
        (float complex*)calloc(iSamplesQty, sizeof(float complex));

    // Initialize the spectrum with the sampled input
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        fcSpectrum[iSample] = fcSampledSignal[iSample];
    }

    // Number of iterations splitting the DFT equation into even and odd
    size_t iNumStages = log2(iSamplesQty);

    // Iteratively compute the FFT
    for (size_t iStage = 0; iStage < iNumStages; iStage++) {
        size_t iGroupsQty = pow(2, iStage);
        size_t iGroupSize = iSamplesQty/(iGroupsQty*2);
        for (size_t iGroup = 0; iGroup < iGroupsQty; iGroup++) {
            for (size_t iGroupElement = 0; iGroupElement < iGroupSize;
                iGroupElement++) {
                // Compute complex exponential of the equation
                float complex fcEuler = cexp(-I*2*PI*iGroup/iSamplesQty);

                // Compute Fourier coefficients
                size_t iIndex1 = iGroup*2*iGroupSize + iGroupElement;
                size_t iIndex2 = iIndex1 + iGroupSize;
                float complex fcTemp = fcSpectrum[iIndex2]*fcEuler;
                fcSpectrum[iIndex2] = fcSpectrum[iIndex1] - fcTemp;
                fcSpectrum[iIndex1] = fcSpectrum[iIndex1] + fcTemp;
            }
        }
    }
    return fcSpectrum;
}






















// /* Using Cooley-Tukey algorithm */
// float complex* fastFourierTransform(float complex* samples, size_t iSamplesQty, size_t depth) {
//     if (iSamplesQty <= 1) {
//         return samples;
//     }

//     // Allocate memory for even and odd samples
//     fcEvenPole[depth] = 
//     // float complex* fcEvenSamples =
//         apr_pcalloc(aprMemoryPool, (iSamplesQty/2)*sizeof(float complex));
//     fcOddPole[depth] = 
//     // float complex* fcOddSamples =
//         apr_pcalloc(aprMemoryPool, (iSamplesQty/2)*sizeof(float complex));
    
//     // Split into even and odd the samples
//     for (size_t iSample = 0; iSample < iSamplesQty/2; iSample++) {
//         fcEvenPole[depth][iSample] = samples[2 * iSample];
//         // fcEvenSamples[iSample] = samples[2 * iSample];
//         fcOddPole[depth][iSample] = samples[2 * iSample + 1];
//         // fcOddSamples[iSample] = samples[2 * iSample + 1];
//     }

//     // Recursively run FFT over smaller samples
//     float complex* fcEvenSpectrum =
//         fastFourierTransform(fcEvenPole[depth], iSamplesQty/2, depth + 1);
//     float complex* fcOddSpectrum =
//         fastFourierTransform(fcOddPole[depth], iSamplesQty/2, depth + 1);

//     // Combine even and odd results
//     // float complex* fcSpectrume =
//     float complex* fcSpectrumPole[depth][0] =
//         apr_pcalloc(aprMemoryPool, (iSamplesQty + 2)*sizeof(float complex));
//     for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
//         float complex fcComplexEuler =
//             cexp(-I * 2 * PI * iSample/iSamplesQty) * fcOddSpectrum[iSample];
//         fcSpectrumPole[depth][iSample] = fcEvenSpectrum[iSample] + fcComplexEuler;
//         fcSpectrumPole[depth][iSample + iSamplesQty/2] =
//             fcEvenSpectrum[iSample] - fcComplexEuler;
//     }

//     return fcSpectrumPole[depth][0];
// }