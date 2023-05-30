// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>
// Copyright 2023 Daniel Alvarado <daniel.alvarado_g@ucr.ac.cr>

#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <unistd.h>  // Enables the use of sysconf

int main(int argc, char *argv[]) {
    // General variables
    int iMyProcessID, iMyRankID, iLocalNumThreads, iGlobalNumThreads;
    // Variables for threading model
    int provided, required = MPI_THREAD_FUNNELED;

    if (argc == 2) {
        sscanf(argv[1], "%d", &iLocalNumThreads);

    } else {
        iLocalNumThreads = sysconf(_SC_NPROCESSORS_ONLN);
    }

    // Initialize multiprocess
    MPI_Init_thread(&argc, &argv, required, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyProcessID);

    if (iMyProcessID == 0) {
        printf("Threads quantity settled to %d\n", iLocalNumThreads);
    }

    // Initialize multithread
    #pragma omp parallel num_threads(iLocalNumThreads) private(iMyRankID)
    {
        iLocalNumThreads = omp_get_num_threads();
        iMyRankID = omp_get_thread_num();
        printf("This is thread %d/%d from process %d\n",
            iMyRankID, iLocalNumThreads, iMyProcessID);
    }

    MPI_Reduce(&iLocalNumThreads, &iGlobalNumThreads, 1, MPI_INT, MPI_SUM, 0,
        MPI_COMM_WORLD);

    if (iMyProcessID == 0) {
        printf("Total number of threads: %d\n", iGlobalNumThreads);
    }

    MPI_Finalize();
}
