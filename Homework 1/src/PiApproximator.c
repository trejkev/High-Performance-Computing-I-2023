#include <mpi.h>
#include <stdio.h>
#include <math.h>  // To compute power

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    double dDeltaX;
    dDeltaX = 1/((double)iRanksQty*10000);  // Computing delta X
    double dArea = 0;
    double dLowerBound = (float)iMyRank*1/((float)iRanksQty);
    double dUpperBound = dLowerBound + 9999*dDeltaX;
    printf("INFO: Rank %d - lower bound %.8f - upper bound %.8f\n", iMyRank, dLowerBound, dUpperBound);
    double dXToEvaluate;
    for (int iTrapezoidNum = 0; iTrapezoidNum < 10000; iTrapezoidNum++) {
        dXToEvaluate = dLowerBound + (float)iTrapezoidNum*dDeltaX;
        dArea += 4/(1+(float)dXToEvaluate*(float)dXToEvaluate)*dDeltaX;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (iMyRank != 0) {
        MPI_Send(&dArea, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double dOtherProcessArea = 0;
        for (int iInBoxRank = 1; iInBoxRank < iRanksQty; iInBoxRank++) {
            MPI_Recv(&dOtherProcessArea, 1, MPI_DOUBLE, iInBoxRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            dArea += dOtherProcessArea;
        }
    }
    if (iMyRank == 0) {
        printf("Pi approximation is %.32f\n", dArea);
    }

    MPI_Finalize();
}