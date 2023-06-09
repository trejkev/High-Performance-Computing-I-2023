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

Complex* discreteFourierTransform(float* fSampledSignal, 
    size_t iSamplesQty, long* lTimes);

long timeReader();

int main(int argc, char *argv[]) {
    FILE *fptr;
    char *sFileName, *sDesc;
    float* fSamplesBuffer;
    size_t iThreadsQty, iReplicas;
    size_t iSamplingFrequency, iSamplesQty;
    size_t iNyquistLimit;
    Complex* cSpectrum;

    long* lTimes = (long*)calloc(10, sizeof(long));

    // -------------------- //
    // -- GET PARAMETERS -- //
    // -------------------- //
    if (argc == 4) {
        sFileName = (char*)calloc(300, sizeof(char));
        sDesc     = (char*)calloc(300, sizeof(char));
        sscanf(argv[1], "%s", sDesc);
        snprintf(sFileName, 300*sizeof(char), "%s%s%s",
            "./test/", sDesc, ".txt");
        sscanf(argv[2], "%zu", &iThreadsQty);
        sscanf(argv[3], "%zu", &iReplicas);
        printf("    Input parameters:\n");
        printf("        Input file: %s\n", sFileName);
        printf("        Threads quantity: %zu\n", iThreadsQty);
        printf("        Replicas: %zu\n", iReplicas);
    } else {
        printf("    Missing arguments!\n");
        return FAIL;
    }

    for (size_t iReplica = 0; iReplica < iReplicas; iReplica++) {
        printf("    Replica %zu\n", iReplica);
        // --------------------- //
        // -- READ INPUT FILE -- //
        // --------------------- //
        lTimes[0] = timeReader();
        if ((fptr = fopen(sFileName, "r")) == NULL) {
            printf("Error opening file %s\n", sFileName);
            return FAIL;
        } else {
            size_t iInputFileLength = 3;
            lTimes[1] = timeReader();
            for (size_t iLine = 0; iLine < iInputFileLength; iLine++) {
                if (iLine == 0) {
                    fscanf(fptr, "%zu", &iSamplingFrequency);
                } else if (iLine == 1) {
                    fscanf(fptr, "%zu", &iSamplesQty);
                    iInputFileLength += iSamplesQty;
                    iNyquistLimit = iSamplesQty/2;
                    fSamplesBuffer =
                        (float*)calloc(iSamplesQty, sizeof(float));
                } else {
                    float auxSaver;
                    fscanf(fptr, "%f", &auxSaver);
                    fSamplesBuffer[iLine-2] = auxSaver;
                }
            }
            lTimes[2] = timeReader();
        }
        fclose(fptr);

        // --------------------- //
        // -- COMPUTE THE DFT -- //
        // --------------------- //
        lTimes[3] = timeReader();
        cSpectrum = 
            discreteFourierTransform(fSamplesBuffer, iSamplesQty, lTimes);
        lTimes[4] = timeReader();
        printf("        DFT elapsed time: %.9f seconds\n",
            (float)(lTimes[4] - lTimes[3])/1000000000L);

        // ------------------------- //
        // -- EXHIBIT THE RESULTS -- //
        // ------------------------- //
        char *sResultsFileName = (char*)calloc(300, sizeof(char));
        snprintf(sResultsFileName, 300*sizeof(char), "%s%s%s%zu%s%zu%s",
            "./results/DFT_Results_", sDesc, "_SampFreq_",
            iSamplingFrequency, "_SampQty_", iSamplesQty, ".csv");
        if (iReplica == 0){
            fptr = fopen(sResultsFileName, "w");
            fprintf(fptr, "Frequency;Real;Imaginary;Amplitude;Angle\n");
            fclose(fptr);
        }
        fptr = fopen(sResultsFileName, "a");
        lTimes[5] = timeReader();
        for (size_t iFreqComp = 0; iFreqComp < iNyquistLimit; iFreqComp++) {
            float fReal       = cSpectrum[iFreqComp].real;
            float fImag       = cSpectrum[iFreqComp].imag;
            float fAmplitude  = sqrt(fReal*fReal + fImag*fImag)/iSamplesQty;
            float fAngle      = atan2(fImag, fReal);
            float fFrequency  =
                (float)iFreqComp*(iSamplingFrequency/iSamplesQty);
            fprintf(fptr, "%.1f;%.8f;%.8f;%.8f;%.8f\n",
                fFrequency, fReal, fImag, fAmplitude, fAngle);
        }
        lTimes[6] = timeReader();

        // ------------------------- //
        // --      SAVE TIMES     -- //
        // ------------------------- //
        snprintf(sResultsFileName, 300*sizeof(char), "%s%s%s%zu%s%zu%s",
            "./results/Time_Results_", sDesc, "_SampFreq_",
            iSamplingFrequency, "_SampQty_", iSamplesQty, ".csv");
        if (iReplica == 0) {
            fptr = fopen(sResultsFileName, "w");
            fprintf(fptr, 
                "FileName;SamplingFreq;SamplesQty;Replica;TimeTag;Time(ns)\n");
            fclose(fptr);
        }
            fptr = fopen(sResultsFileName, "a");
        char sTimeTag[100];
        for (size_t iTCount = 0; iTCount < 9; iTCount++) {
            snprintf(sTimeTag, 100*sizeof(char), "Time_%zu", iTCount);
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
                iSamplingFrequency, iSamplesQty, iReplica, sTimeTag,
                lTimes[iTCount]);
        }
        /* Relevant delta times */
        // Reading input samples
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_1->2",
            lTimes[2] - lTimes[1]);
        // Reading input file
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_0->3",
            lTimes[3] - lTimes[0]);
        // DFT method execution
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_3->4",
            lTimes[4] - lTimes[3]);
        // Saving DFT results
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_5->6",
            lTimes[6] - lTimes[5]);
        // DFT execution as is
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_7->10",
            lTimes[10] - lTimes[7]);
        // DFT-Time loop execution, only first measurement
        fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sFileName,
            iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_8->9",
            lTimes[9] - lTimes[8]);
        fclose(fptr);
    }

    // --------------------------- //
    // -- DEALLOCATE THE MEMORY -- //
    // --------------------------- //
    free(cSpectrum);
    free(fSamplesBuffer);
    free(sFileName);
    free(lTimes);

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//                      ITERATIVE IMPLEMENTATION OF DFT                       //
////////////////////////////////////////////////////////////////////////////////
Complex* discreteFourierTransform(float* fSampledSignal, 
    size_t iSamplesQty, long* lTimes) {
    // FFT frequency components are stored here
    Complex* cSpectrum = (Complex*)calloc(iSamplesQty, sizeof(Complex));

    lTimes[7] = timeReader();
    for (size_t iFreqIndex = 0; iFreqIndex < iSamplesQty; iFreqIndex++) {
        if (iFreqIndex == 0) { // Read only the first time entering
            lTimes[8] = timeReader();
        }
        for (size_t iTimeIndex = 0; iTimeIndex < iSamplesQty; iTimeIndex++) {
            float fAngle = 2.0*M_PI*iFreqIndex*iTimeIndex/iSamplesQty;
            cSpectrum[iFreqIndex].real +=
                fSampledSignal[iTimeIndex]*cos(fAngle);
            cSpectrum[iFreqIndex].imag -=
                fSampledSignal[iTimeIndex]*sin(fAngle);
        }
        if (iFreqIndex == 0) { // Read only the first time entering
            lTimes[9] = timeReader();
        }
    }
    lTimes[10] = timeReader();
    return cSpectrum;
}

////////////////////////////////////////////////////////////////////////////////
//                                TIME READER                                 //
////////////////////////////////////////////////////////////////////////////////
long timeReader(){
    struct timespec stTimeReading;
    
    clock_gettime(CLOCK_MONOTONIC, &stTimeReading); // Get time reading

    // Compute the time in nanoseconds
    long lSeconds = stTimeReading.tv_sec;
    long lNanoseconds = stTimeReading.tv_nsec;
    return (lSeconds*1000000000L + lNanoseconds);
}
