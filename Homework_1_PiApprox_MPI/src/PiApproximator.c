// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <math.h>  // Use floating abs
#include <mpi.h>

#define SUCCESS 0

int main() {
    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);
    printf("My rank is %d\n", iMyRank);

    double dDeltaX = 1/((double)iRanksQty*10000);
    double dArea = 0;  // Will store the area computed by the current rank
    double dLowerBound = iMyRank/(float)iRanksQty;
    double dXToEvaluate;  // Will store x value to be evaluated

    // Performs summatory evaluation using trapezoid approximation
    for (int iTrapezoidNum = 0; iTrapezoidNum < 10000; iTrapezoidNum++) {
        // Trapezoid approximation requires evaluation at both bounds
        for (int iOffset = 0; iOffset <= 1; iOffset++) {
            double dNextTrapezoid = iTrapezoidNum + iOffset;
            dXToEvaluate = dLowerBound + dNextTrapezoid*dDeltaX;
            dArea += 4/(1 + dXToEvaluate*dXToEvaluate);
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

    // Prints current pi approximation
    if (iMyRank == 0) {
        // Trapezoid approximation performs calculations to all evals
        dArea *= dDeltaX;
        dArea /= 2;
        printf("Pi approximation using %d ranks is %.32f\n", iRanksQty, dArea);
        double dError = fabs(dArea - 3.141592653589793238462643383279502884197);
        printf("Error is: %.32f\n", dError);
    }

    MPI_Finalize();

    return SUCCESS;
}
