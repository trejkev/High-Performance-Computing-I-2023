#include <mpi.h>
#include <stdio.h>
#include <unistd.h>  // To use sleep

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    
    // Get the number of processes
    int iWorldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &iWorldSize);
    if (iWorldSize != 2){
        printf("World size shall be equal to 2\n");
        MPI_Finalize();
        return 1;
    }

    // Get my rank number
    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    int iIteration = 0;
    while (iIteration < 100) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (iMyRank == 0) {
            // printf("Rank %d: Ping - iteration %d\n", iMyRank, iIteration);
            // iIteration += 1;
            MPI_Send(&iIteration, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&iIteration, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&iIteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank %d: Iteration %d\n", iMyRank, iIteration);
            iIteration += 1;
            MPI_Send(&iIteration, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}