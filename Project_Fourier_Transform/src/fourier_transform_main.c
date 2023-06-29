// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "fourier_transform_main.h"

int main(int argc, char *argv[]) {
    int iProvided, iRequired = MPI_THREAD_MULTIPLE;
    MPI_Init_thread(&argc, &argv, iRequired, &iProvided);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    FILE    *fptr;               // File input and output container
    char    *sSignPath;          // Keeps the signal path
    char    *sSignName;          // Keeps the signal name to be generated
    char    *sFourierType;       // Records if will compute dft or fft
    char    *sResultsFileName;   // Keeps the results file name
    float   *fSamplesBuffer;     // Keeps the samples to be transformed
    size_t   iThreadsQty;        // Keeps the threads quantity to utilize
    size_t   iReplicas;          // Tells the number of times to execute the ft
    size_t   iSamplingFrequency; // Tells samples per second of the signal
    size_t   iSamplesQty;        // Tells the number of samples in the file
    size_t   iNyquistLimit;      // Saves the Nyquist limit
    Complex *cSpectrum;          // Keeps the results of the ft
    long    *lTimes;             // Keeps the time readings from each stage

    lTimes = (long*)calloc(11, sizeof(long));

    // -------------------- //
    // -- GET PARAMETERS -- //
    // -------------------- //
    if (argc >= 6) {
        if (iMyRank == 0) signalGenerator(argc, argv);
        sSignPath    = (char*)calloc(300, sizeof(char));
        sSignName    = (char*)calloc(300, sizeof(char));
        sFourierType = (char*)calloc(4, sizeof(char));

        sscanf(argv[2], "%zu", &iThreadsQty);
        sscanf(argv[3], "%zu", &iReplicas);
        sscanf(argv[5], "%s" , sFourierType);
        sscanf(argv[1], "%s" , sSignName);

        snprintf(sSignPath, 300*sizeof(char), "%s%s%s",
            "./test/", sSignName, ".txt");
        omp_set_num_threads(iThreadsQty);

        if (iMyRank == 0) {
            printf("    Input parameters:\n");
            printf("        Input file: %s\n", sSignPath);
            printf("        Replicas: %zu\n", iReplicas);
            printf("        Processes quantity: %d\n", iRanksQty);
            printf("        Threads quantity: %zu\n", iThreadsQty);
        }
    } else {
        if (iMyRank == 0) printf("    Missing arguments!\n");
        return FAIL;
    }

    // Repeat the FT code and its relevant logs iReplicas times
    for (size_t iReplica = 0; iReplica < iReplicas; iReplica++) {
        if (iMyRank == 0) printf("    Replica %zu\n", iReplica);
        // --------------------- //
        // -- READ INPUT FILE -- //
        // --------------------- //
        lTimes[0] = timeReader();
        int iMutex;
        if (iMyRank != 0) {
            MPI_Recv(&iMutex, 1, MPI_INT, iMyRank - 1, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        }
        if ((fptr = fopen(sSignPath, "r")) == NULL) {
            printf("Error opening file %s\n", sSignPath);
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
                        (float*)calloc(iSamplesQty + 2, sizeof(float));
                } else {
                    float auxSaver;
                    fscanf(fptr, "%f", &auxSaver);
                    fSamplesBuffer[iLine-2] = auxSaver;
                }
            }
            lTimes[2] = timeReader();
        }
        fclose(fptr);
        if (iMyRank != iRanksQty - 1) {
            MPI_Send(&iMutex, 1, MPI_INT, iMyRank + 1, 0, MPI_COMM_WORLD);
        }

        // --------------------- //
        // -- COMPUTE THE FT  -- //
        // --------------------- //
        MPI_Barrier(MPI_COMM_WORLD);

        lTimes[3] = timeReader();

        if (strcmp(sFourierType, "dft") == 0) {
            cSpectrum = dft(fSamplesBuffer, iSamplesQty, lTimes, 
                iRanksQty  , iMyRank);
        } else if (strcmp(sFourierType, "fft") == 0) {
            if (iMyRank == 0) {
                cSpectrum = fft_preprocessor(fSamplesBuffer, iSamplesQty,
                    lTimes);
            }
        } else {
            printf("%s is not a valid ft type\n", sFourierType);
            return FAIL;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        lTimes[4] = timeReader();

        if (iMyRank == 0) {
            printf("        %s elapsed time: %.9f seconds\n", sFourierType,
                (float)(lTimes[4] - lTimes[3])/1000000000L);
        }

        // ------------------------- //
        // -- EXHIBIT THE RESULTS -- //
        // ------------------------- //
        sResultsFileName = (char*)calloc(400, sizeof(char));
        snprintf(sResultsFileName, 400*sizeof(char), "%s%s%s%s%s%zu%s%zu%s",
            "./results/", sFourierType, "_results_", sSignName, "_SampFreq_",
            iSamplingFrequency, "_SampQty_", iSamplesQty, ".csv");

        // All results will be the same, thus, save only the first replica
        if (iReplica == 0) {
            lTimes[5] = timeReader();
            // Wait for lower rank to save its results
            int iMutex;
            if (iMyRank == 0) {
                fptr = fopen(sResultsFileName, "w");
                fprintf(fptr, "Frequency;Real;Imaginary;Amplitude;Angle\n");
            } else {
                MPI_Recv(&iMutex, 1, MPI_INT, iMyRank - 1, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                fptr = fopen(sResultsFileName, "a");
            }
            if (strcmp(sFourierType, "dft") == 0) {
                size_t iInit = iMyRank*iSamplesQty/iRanksQty;
                size_t iEnd = iInit + iSamplesQty/iRanksQty;
                for (size_t iFreqComp = iInit; iFreqComp < iEnd; iFreqComp++) {
                    if (iFreqComp < iNyquistLimit) {
                        float fReal       = cSpectrum[iFreqComp].real;
                        float fImag       = cSpectrum[iFreqComp].imag;
                        float fAmplitude  = sqrt(fReal*fReal + fImag*fImag)/iSamplesQty;
                        float fAngle      = atan2(fImag, fReal);
                        float fFrequency  = 
                            (float)iFreqComp*(iSamplingFrequency/iSamplesQty);
                        fprintf(fptr, "%.1f;%.8f;%.8f;%.8f;%.8f\n",
                            fFrequency, fReal, fImag, fAmplitude, fAngle);
                    }
                }
            } else {
                if (iMyRank == 0) {
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
                }
            }
            fclose(fptr);
            lTimes[6] = timeReader();
            if (iMyRank != iRanksQty - 1) {
                MPI_Send(&iMutex, 1, MPI_INT, iMyRank + 1, 0, MPI_COMM_WORLD);
            }
        }

        // ------------------------- //
        // --      SAVE TIMES     -- //
        // ------------------------- //
        if (iMyRank == 0) {
            snprintf(sResultsFileName, 400*sizeof(char), "%s%s%s%s%s%zu%s%zu%s",
                "./results/", sFourierType, "_time_results_", sSignName,
                "_SampFreq_", iSamplingFrequency, "_SampQty_", iSamplesQty, ".csv");
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
                fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                    iSamplingFrequency, iSamplesQty, iReplica, sTimeTag,
                    lTimes[iTCount]);
            }
            /* Relevant delta times */
            // Reading input samples
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_1->2",
                lTimes[2] - lTimes[1]);
            // Reading input file
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_0->3",
                lTimes[3] - lTimes[0]);
            // DFT method execution
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_3->4",
                lTimes[4] - lTimes[3]);
            // Saving DFT results
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_5->6",
                lTimes[6] - lTimes[5]);
            // DFT execution as is
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_7->10",
                lTimes[10] - lTimes[7]);
            // DFT-Time loop execution, only first measurement
            fprintf(fptr, "%s;%zu;%zu;%zu;%s;%.9ld\n", sSignPath,
                iSamplingFrequency, iSamplesQty, iReplica, "Delta Time_8->9",
                lTimes[9] - lTimes[8]);
            fclose(fptr);
        }
    }

    MPI_Finalize();

    // --------------------------- //
    // -- DEALLOCATE THE MEMORY -- //
    // --------------------------- //
    free(lTimes);
    free(sSignPath);
    free(sSignName);
    free(sFourierType);
    free(fSamplesBuffer);
    if (strcmp(sFourierType, "dft") == 0) {
        free(cSpectrum);
    } else {
        if (iMyRank == 0) free(cSpectrum);
    }
    free(sResultsFileName);


    return SUCCESS;
}
