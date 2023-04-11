// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    double dDeltaX = 1/((double)iRanksQty*10000);
    double dArea = 0;  // Will store the area computed by the current rank
    double dLowerBound = (float)iMyRank*1/((float)iRanksQty);
    double dXToEvaluate;  // Will store x value to be evaluated

    // Performs summatory evaluation
    for (int iTrapezoidNum = 0; iTrapezoidNum < 10000; iTrapezoidNum++) {
        dXToEvaluate = dLowerBound + (float)iTrapezoidNum*dDeltaX;
        dArea += 4/(1+(float)dXToEvaluate*(float)dXToEvaluate)*dDeltaX;
    }
    // Missing from highest rank to complete 0-1 summatory
    if (iMyRank == iRanksQty - 1) {
        dXToEvaluate = dLowerBound + 10000*dDeltaX;
        dArea += 4/(1+(float)dXToEvaluate*(float)dXToEvaluate)*dDeltaX;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Sends the resulting area to rank 0
    if (iMyRank != 0) {
        MPI_Send(&dArea, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        // Rank 0 gets other ranks areas and adds to its current area
        double dOtherProcessArea = 0;
        for (int iInBoxRank = 1; iInBoxRank < iRanksQty; iInBoxRank++) {
            MPI_Recv(&dOtherProcessArea, 1, MPI_DOUBLE, iInBoxRank, 0, 
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            dArea += dOtherProcessArea;
        }
    }

    // Prints current pi approximation
    if (iMyRank == 0) {
        printf("Pi approximation is %.32f\n", dArea);
    }

    MPI_Finalize();
}