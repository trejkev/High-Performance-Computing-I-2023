// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <mpi.h>
#include <stdio.h>
#include <math.h>  // Use floating abs

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

    // Performs summatory evaluation using trapezoid approximation
    for (int iTrapezoidNum = 0; iTrapezoidNum < 10000; iTrapezoidNum++) {
        // Trapezoid approximation requires evaluation at both bounds
        for (int iOffset = 0; iOffset <= 1; iOffset++) {
            dXToEvaluate = dLowerBound + (float)(iTrapezoidNum+iOffset)*dDeltaX;
            dArea += 4/(1+(float)dXToEvaluate*(float)dXToEvaluate);
        }
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
    // Reduction of trapezoid approximation performs calculations to all evals
    dArea *= dDeltaX;
    dArea /= 2;

    // Prints current pi approximation
    if (iMyRank == 0) {
        printf("Pi approximation using %d ranks is %.32f\n", iRanksQty, dArea);
        double dError = fabs(dArea - 3.1415926535897932384626433832795028841971693993751058209749445923);
        printf("Error is: %.32f\n", dError);
    }

    MPI_Finalize();
}