// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h> // Deals with dynamic memory
#include <apr_pools.h> // Deals with dynamic memory through memory pools
#include <linux/limits.h>
#include <complex.h>

#include <math.h>

// #define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 0
#ifndef PI
    #define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#endif

apr_pool_t* aprMemoryPool;

float complex* fastFourierTransform(float complex* samples, size_t iSamplesQty);

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName;
    float complex* fcSamplesBuffer;
    size_t iThreadsQty, iReplicas;
    size_t iSamplingFrequency, iSamplesQty;
    // size_t iNyquistLimit;

    // Create the memory pool
    apr_initialize();
    apr_pool_create(&aprMemoryPool, NULL);
    

    // -------------------- //
    // -- GET PARAMETERS -- //
    // -------------------- //
    if (argc == 4) {
        sFileName = apr_pcalloc(aprMemoryPool, 300*sizeof(char));
        // sFileName = (char*)malloc(300*sizeof(char));
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
                // iNyquistLimit = iSamplingFrequency/2;
                printf("Sampling frequency: %zu\n", iSamplingFrequency);
            } else if (iLineCounter == 1) {
                fscanf(fptr, "%zu", &iSamplesQty);
                printf("Samples quantity: %zu\n", iSamplesQty);
                iInputFileLength += iSamplesQty;
                // fcSamplesBuffer =
                //     (float complex*)malloc(iSamplesQty*sizeof(float complex));
                fcSamplesBuffer = apr_pcalloc(aprMemoryPool,
                    iSamplesQty*sizeof(float complex));
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

    printf("Fast Fourier Transform coefficients:\n");
    fptr = fopen("./results/results.csv", "w");
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        float fReal = creal(fcSpectrum[iSample])/iSamplesQty;
        float fImag = cimag(fcSpectrum[iSample])/iSamplesQty;
        float fAngle = carg(fcSpectrum[iSample]);
        float fAmplitude = sqrt(fReal*fReal + fImag*fImag);
        printf("F[%zu Hz] = %.8f + %.8fi -> amplitude = %.8f, angle %.8f rad\n",
        iSample + 1, fReal, fImag, fAmplitude, fAngle);
        fprintf(fptr, "%zu;%.8f;%.8f;%.8f;%.8f\n",
        iSample + 1, fReal, fImag, fAmplitude, fAngle);
    }

    if (aprMemoryPool != NULL) {
        printf("Destroy the pool\n");
        apr_pool_destroy(aprMemoryPool);
        apr_terminate();

    }
    // free(fcSpectrum); // Comes from memory reserved into FFT
    // free(fcSamplesBuffer);
    // free(sFileName);
    return 0;
}





/* Using Cooley-Tukey algorithm */
float complex* fastFourierTransform(float complex* samples, size_t iSamplesQty) {
    if (iSamplesQty <= 1) {
        return samples;
    }

    // Allocate memory for even and odd samples
    float complex* fcEvenSamples =
        apr_pcalloc(aprMemoryPool, (iSamplesQty/2)*sizeof(float complex));
    float complex* fcOddSamples =
        apr_pcalloc(aprMemoryPool, (iSamplesQty/2)*sizeof(float complex));
    
    // Split into even and odd the samples
    for (size_t iSample = 0; iSample < iSamplesQty/2; iSample++) {
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
        apr_pcalloc(aprMemoryPool, (iSamplesQty + 2)*sizeof(float complex));
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        float complex fcComplexEuler =
            cexp(-I * 2 * PI * iSample/iSamplesQty) * fcOddSpectrum[iSample];
        fcSpectrum[iSample] = fcEvenSpectrum[iSample] + fcComplexEuler;
        fcSpectrum[iSample + iSamplesQty/2] =
            fcEvenSpectrum[iSample] - fcComplexEuler;
    }

    return fcSpectrum;
}