// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

/* This code is designed to compute a sequential version of the fast
 * fourier transformation, of a sound pressure reading saved into a buffer.
 */

#include <stdio.h>
#include <stdlib.h> // Deals with dynamic memory
#include <linux/limits.h>
// #include <mpi.h>
// #include <omp.h>
// #include <string.h>
// #include <unistd.h>  // Enables the use of sysconf
// #include <math.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 0

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName;
    float *fSamplesBuffer;
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
                fSamplesBuffer = (float*)calloc(iSamplesQty, sizeof(float));
            } else {
                float auxSaver;
                fscanf(fptr, "%f", &auxSaver);
                fSamplesBuffer[iLineCounter-2] = auxSaver;
            }
            iLineCounter++;
        }
        printf("Samples are: \n");
        for (int iSample = 0; iSample < iSamplesQty; iSample++) {
            printf("    Sample %d: %f\n", iSample, fSamplesBuffer[iSample]);
        }
    }
    fclose(fptr);

    // Execute the code N times
    for (int iReplica = 0; iReplica < iReplicas; iReplica++) {

        iReplica += 1;
    }






    free(fSamplesBuffer);
    free(sFileName);
    return 0;
}