// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <mpi.h>
#include <stdio.h>
#include <math.h>  // Use floating abs
#include <time.h>  // To deal with delta time measurements

int main(int argc, char** argv) {
    short iRepeatNTimes = 1;
    if (argc > 1) {
        sscanf(argv[1], "%hd", &iRepeatNTimes);  // Asign arg[2] to i_Variable
    }

    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);
    
    if (iMyRank == 0) {
        printf("Running %s times\n", argv[1]);
    }

    for (short iTrial = 0; iTrial < iRepeatNTimes; iTrial++) {
        // Gets initial time reading
        float start_time = clock();

        float fDeltaX = 1/((float)iRanksQty*10000);
        float fArea = 0;  // Will store the area computed by the current rank
        float fLowerBound = iMyRank/(float)iRanksQty;
        float fXToEvaluate;  // Will store x value to be evaluated
        float fAreas[10000];

        // Performs summatory evaluation using trapezoid approximation
        for (int iTrapezoidNum = 0; iTrapezoidNum < 10000; iTrapezoidNum++) {
            fAreas[iTrapezoidNum] = 0;
            // Trapezoid approximation requires evaluation at both bounds
            for (int iOffset = 0; iOffset <= 1; iOffset++) {
                float fNextTrapezoid = iTrapezoidNum + iOffset;
                fXToEvaluate = fLowerBound + fNextTrapezoid*fDeltaX;
                // fArea += 4/(1 + fXToEvaluate*fXToEvaluate);
                fAreas[iTrapezoidNum] += 4/(1 + fXToEvaluate*fXToEvaluate);
            }
        }
        for (int iCounter = 0; iCounter < 10000; iCounter++) {
            fArea += fAreas[iCounter];
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // Sends the resulting area to rank 0
        if (iMyRank != 0) {
            MPI_Send(&fArea, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        } else {
            // Rank 0 gets other ranks areas and adds to its current area
            float fOtherProcessArea = 0;
            for (int iInBoxRank = 1; iInBoxRank < iRanksQty; iInBoxRank++) {
                MPI_Recv(&fOtherProcessArea, 1, MPI_FLOAT, iInBoxRank, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                fArea += fOtherProcessArea;
            }
        }

        // Prints current pi approximation
        if (iMyRank == 0) {
            // Trapezoid approximation performs calculations to all evals
            fArea *= fDeltaX;
            fArea /= 2;
            // printf("Pi approximation using %d ranks is %.32f\n", iRanksQty, fArea);
            float fError = fabs(fArea - 3.141592653589793238462643383279502884197);
            // printf("Error is: %.32f\n", fError);

            // Gets final time reading
            float finish_time = clock();
            // Computing elapsed time
            float fElapsedTime = (float)(finish_time - start_time)/CLOCKS_PER_SEC;
            // printf("Elapsed time: %.9lf s\n", fElapsedTime);
            printf("Ranks;%d;PiApprox;%.32f;Error;%.32f;ElapsedTime;%.9lf\n",
                iRanksQty, fArea, fError, fElapsedTime);
        }
    }
    MPI_Finalize();
}
