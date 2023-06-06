// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h> // Deals with dynamic memory
#include <linux/limits.h>
#include <complex.h>

// #include <math.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 0
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923

float complex* fastFourierTransform(float complex* samples, size_t iSamplesQty);

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName;
    float complex *fcSamplesBuffer;
    size_t iThreadsQty, iReplicas;
    size_t iSamplingFrequency, iSamplesQty;
    size_t iNyquistLimit;
    

    // -------------------- //
    // -- GET PARAMETERS -- //
    // -------------------- //
    if (argc == 4) {
        sFileName = (char*)malloc(300*sizeof(char));
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
                iNyquistLimit = iSamplingFrequency/2;
                printf("Sampling frequency: %zu\n", iSamplingFrequency);
            } else if (iLineCounter == 1) {
                fscanf(fptr, "%zu", &iSamplesQty);
                printf("Samples quantity: %zu\n", iSamplesQty);
                iInputFileLength += iSamplesQty;
                fcSamplesBuffer =
                    (float complex*)malloc(iSamplesQty*sizeof(float complex));
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

    float complex* fcSpectrum =
        fastFourierTransform(fcSamplesBuffer, iSamplesQty);

    // printf("Fast Fourier Transform coefficients:\n");
    // for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
    //     printf("F[%zu] = %.4f + %.4fi\n", iSample,
    //         creal(fcSpectrum[iSample]), cimag(fcSpectrum[iSample]));
    // }

    free(fcSpectrum); // Comes from memory reserved into FFT
    free(fcSamplesBuffer);
    free(sFileName);
    return 0;
}

/* Using Cooley-Tukey algorithm */
float complex* fastFourierTransform(float complex* samples, size_t iSamplesQty) {
    if (iSamplesQty <= 1) {
        float complex* fcSpectrum = (float complex*)malloc(sizeof(float complex));
        fcSpectrum[0] = samples[0];
        return fcSpectrum;
        // return samples;
    }

    // Allocate memory for even and odd samples
    float complex* fcEvenSamples =
        (float complex*)malloc(iSamplesQty/2*sizeof(float complex));
    float complex* fcOddSamples =
        (float complex*)malloc(iSamplesQty/2*sizeof(float complex));
    
    // Split into even and odd the samples
    for (int iSample = 0; iSample < iSamplesQty/2; iSample++) {
        fcEvenSamples[iSample] = samples[2 * iSample];
        fcOddSamples[iSample] = samples[2 * iSample + 1];
    }

    // Recursively run FFT over smaller samples
    float complex* fcEvenSpectrum =
        fastFourierTransform(fcEvenSamples, iSamplesQty/2);
    float complex* fcOddSpectrum =
        fastFourierTransform(fcOddSamples, iSamplesQty/2);

    // Combine even and odd results
    float complex* fcSpectrum =
        (float complex*)malloc(iSamplesQty*sizeof(float complex));
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        float complex fcComplexEuler =
            cexp(-I * 2 * PI * iSample/iSamplesQty) * fcOddSpectrum[iSample];
        fcSpectrum[iSample] = fcEvenSpectrum[iSample] + fcComplexEuler;
        fcSpectrum[iSample + iSamplesQty/2] =
            fcEvenSpectrum[iSample] - fcComplexEuler;
    }


    printf("Samples: %zu", iSamplesQty);

    free(fcEvenSamples);
    free(fcOddSamples);
    free(fcEvenSpectrum);
    free(fcOddSpectrum);
    return fcSpectrum;
}