// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <mpi.h>
#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1

int main(int argc, char *argv[]) {

    // Get the input number as terminal parameter
    size_t iInputNumber;
    if (argc >= 2) {
        sscanf(argv[1], "%zu", &iInputNumber);
    } else {
        printf("Missing arguments!\n");
        return FAILURE;
    }

    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    printf("Process %d - input number %zu\n", iMyRank, iInputNumber);
    
    MPI_Finalize();
    
    return SUCCESS;
}