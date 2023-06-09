// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SUCCESS 0
#define FAIL 0

typedef struct {
    float real;
    float imag;
} Complex;

Complex*
    discreteFourierTransform(float* cSampledSignal, size_t iSamplesQty);

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName;
    float* fSamplesBuffer;
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
        return FAIL;
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
                fSamplesBuffer =
                    (float*)calloc(iSamplesQty, sizeof(float));
            } else {
                float auxSaver;
                fscanf(fptr, "%f", &auxSaver);
                fSamplesBuffer[iLineCounter-2] = auxSaver;
            }
            iLineCounter++;
        }
        printf("Samples are: \n");
        for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
            printf("    Sample %zu: %f\n", iSample, fSamplesBuffer[iSample]);
        }
    }
    fclose(fptr);

    // --------------------- //
    // -- COMPUTE THE DFT -- //
    // --------------------- //
    Complex* cSpectrum =
        discreteFourierTransform(fSamplesBuffer, iSamplesQty);

    // ------------------------- //
    // -- EXHIBIT THE RESULTS -- //
    // ------------------------- //
    printf("Fast Fourier Transform coefficients:\n");
    fptr = fopen("./results/results.csv", "w");
    fprintf(fptr, "Frequency;Real;Imaginary;Amplitude;Angle\n");
    for (size_t iFreqComp = 0; iFreqComp < iNyquistLimit; iFreqComp++) {
        float fFrequency = (float)iFreqComp*(iSamplingFrequency/iSamplesQty);
        float fAmplitude =
            sqrt(cSpectrum[iFreqComp].real*cSpectrum[iFreqComp].real +
            cSpectrum[iFreqComp].imag*cSpectrum[iFreqComp].imag)/iSamplesQty;
        float fAngle =
            atan2(cSpectrum[iFreqComp].imag, cSpectrum[iFreqComp].real);
        printf("F[%.1f Hz] = %.8f + %.8fi -> amplitude = %.8f, angle %.8f rad\n",
        fFrequency, cSpectrum[iFreqComp].real, cSpectrum[iFreqComp].imag,
            fAmplitude, fAngle);
        fprintf(fptr, "%.1f;%.8f;%.8f;%.8f;%.8f\n",
        fFrequency, cSpectrum[iFreqComp].real, cSpectrum[iFreqComp].imag,
            fAmplitude, fAngle);
    }

    // --------------------------- //
    // -- DEALLOCATE THE MEMORY -- //
    // --------------------------- //
    free(cSpectrum); // Comes from memory reserved into FFT
    free(fSamplesBuffer);
    free(sFileName);

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//                      ITERATIVE IMPLEMENTATION OF FFT                       //
////////////////////////////////////////////////////////////////////////////////
Complex* discreteFourierTransform(float* fSampledSignal, size_t iSamplesQty) {
    // FFT frequency components are stored here
    Complex* cSpectrum = (Complex*)calloc(iSamplesQty, sizeof(Complex));

    for (size_t iFreqIndex = 0; iFreqIndex < iSamplesQty; iFreqIndex++) {
        for (size_t iTimeIndex = 0; iTimeIndex < iSamplesQty; iTimeIndex++) {
            float fAngle = 2.0*M_PI*iFreqIndex*iTimeIndex/iSamplesQty;
            cSpectrum[iFreqIndex].real += fSampledSignal[iTimeIndex]*cos(fAngle);
            cSpectrum[iFreqIndex].imag -= fSampledSignal[iTimeIndex]*sin(fAngle);

        }
    }
    return cSpectrum;
}
