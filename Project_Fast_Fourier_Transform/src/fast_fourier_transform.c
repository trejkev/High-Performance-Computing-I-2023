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
                iNyquistLimit = iSamplingFrequency/2;
                printf("Sampling frequency: %zu\n", iSamplingFrequency);
            } else if (iLineCounter == 1) {
                fscanf(fptr, "%zu", &iSamplesQty);
                printf("Samples quantity: %zu\n", iSamplesQty);
                iInputFileLength += iSamplesQty;
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
        for (int iSample = 0; iSample < iSamplesQty; iSample++) {
            printf("    Sample %d: %f + %fi\n", iSample, 
                creal(fcSamplesBuffer[iSample]),
                cimag(fcSamplesBuffer[iSample]));
        }
    }
    fclose(fptr);
    free(fcSamplesBuffer);
    free(sFileName);
    return 0;
}