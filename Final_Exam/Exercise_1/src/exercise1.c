

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define Y_SIZE 64 // Quantity of rows of the rank
#define X_SIZE 64 // Length of each of the rows
#define SUCCESS 0
#define FAILURE 1

int main(int argc, char *argv[]) {
    MPI_Init(NULL, NULL);

    int iRanksQty;
    MPI_Comm_size(MPI_COMM_WORLD, &iRanksQty);

    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

    int iRankToPrint = iRanksQty + 1;
    int ROWS = Y_SIZE;
    int COLS = X_SIZE;
    if (argc >= 2) {
        sscanf(argv[1], "%d", &iRankToPrint);
        if (iMyRank == iRankToPrint) {
            printf("DEBUG: Rank %d to be printed\n", iMyRank);
        }
    }
    if (argc >= 4) {
        sscanf(argv[2], "%d", &ROWS);
        sscanf(argv[3], "%d", &COLS);
        if (iMyRank == iRankToPrint) {
            printf("Global data structure of %dx%d\n", ROWS, COLS);
        }
    }

    // Global data structure creation
    if (iMyRank == iRankToPrint) {printf("DEBUG: Global data structure\n");}
    double dGlobalData[ROWS][COLS];
    int iBlockSize = ROWS/iRanksQty;
    for (int iRow = 0; iRow < ROWS; iRow++) {
        for (int iCol = 0; iCol < COLS; iCol++) {
            dGlobalData[iRow][iCol] = iRow/iBlockSize;
            if (iMyRank == iRankToPrint) {
                printf("%.0f ", dGlobalData[iRow][iCol]);
            }
        }
        if (iMyRank == iRankToPrint) {printf("\n");}
    }
    if (iMyRank == iRankToPrint) {printf("\n\n");}
    MPI_Barrier(MPI_COMM_WORLD);

    double dSelfData[ROWS/iRanksQty + 2][COLS]; // Self data structure

    // Filling the rank data structure
    int iOffset = iMyRank*ROWS/iRanksQty;
    for (int iRow = 0; iRow < ROWS/iRanksQty; iRow++) {
        for (int iCol = 0; iCol < COLS; iCol++) {
            dSelfData[iRow + 1][iCol] = dGlobalData[iOffset + iRow][iCol];
        }
    }
    // Filling ghost slots with -1
    for (int iCol = 0; iCol < COLS; iCol++) {
        dSelfData[0][iCol] = -1;
    }
    for (int iCol = 0; iCol < COLS; iCol++) {
        dSelfData[ROWS/iRanksQty + 1][iCol] = -1;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // DEBUG: Print self data structure before the processing
    if (iMyRank == iRankToPrint) {
        printf("DEBUG: Rank %d data BEFORE getting the results\n", iMyRank);
        // Printing lower ghost slots
        for (int iCol = 0; iCol < COLS; iCol++) {
            printf("%.0f ", dSelfData[0][iCol]);
        }
        printf("-> Lower ghost line\n");
        // Printing self data section
        for (int iRow = 1; iRow < ROWS/iRanksQty + 1; iRow++) {
            for (int iCol = 0; iCol < COLS; iCol++) {
                printf(" %.0f ", dSelfData[iRow][iCol]);
            }
            printf("\n");
        }
        // Printing upper ghost slots
        for (int iCol = 0; iCol < COLS; iCol++) {
            printf("%.0f ", dSelfData[ROWS/iRanksQty + 1][iCol]);
        }
        printf("-> Upper ghost line\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);

    /* Ghost slots transmission process:
     * -> Even transmission to odd
     *      -> self upper bound to self + 1 rank lower bound
     *      -> self lower bound to self - 1 rank upper bound
     * -> Odd transmission to even
     *      -> self upper bound to self + 1 rank lower bound
     *      -> self lower bound to self - 1 rank upper bound
    */
    double dRecvData[COLS + 1];
    double dSendData[COLS + 1];
    if (iMyRank%2 == 0) {
        if (iMyRank < iRanksQty - 1) {
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSendData[iIndex] = dSelfData[ROWS/iRanksQty][iIndex];
            }
            MPI_Send(dSendData, COLS, MPI_DOUBLE, iMyRank + 1, 0,
                MPI_COMM_WORLD);
        }
        if (iMyRank > 0) {
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSendData[iIndex] = dSelfData[1][iIndex];
            }
            MPI_Send(dSendData, COLS, MPI_DOUBLE, iMyRank - 1, 0,
                MPI_COMM_WORLD);
        }
    } else {
        if (iMyRank > 0) {
            MPI_Recv(dRecvData, COLS, MPI_DOUBLE, iMyRank - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSelfData[0][iIndex] = dRecvData[iIndex];
            }
        }
        if (iMyRank < iRanksQty - 1) {
            MPI_Recv(dRecvData, COLS, MPI_DOUBLE, iMyRank + 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSelfData[ROWS/iRanksQty + 1][iIndex] = dRecvData[iIndex];
            }
        }
    } 
    MPI_Barrier(MPI_COMM_WORLD);
    if (iMyRank%2 != 0) {
        if (iMyRank < iRanksQty - 1) {
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSendData[iIndex] = dSelfData[ROWS/iRanksQty][iIndex];
            }
            MPI_Send(dSendData, COLS, MPI_DOUBLE, iMyRank + 1, 0,
                MPI_COMM_WORLD);
        }
        if (iMyRank > 0) {
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSendData[iIndex] = dSelfData[1][iIndex];
            }
            MPI_Send(dSendData, COLS, MPI_DOUBLE, iMyRank - 1, 0,
                MPI_COMM_WORLD);
        }
    } else {
        if (iMyRank > 0) {
            MPI_Recv(dRecvData, COLS, MPI_DOUBLE, iMyRank - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSelfData[0][iIndex] = dRecvData[iIndex];
            }
        }
        if (iMyRank < iRanksQty - 1) {
            MPI_Recv(dRecvData, COLS, MPI_DOUBLE, iMyRank + 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int iIndex = 0; iIndex < COLS; ++iIndex) {
                dSelfData[ROWS/iRanksQty + 1][iIndex] = dRecvData[iIndex];
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // DEBUG: Print self data structure after the processing
    if (iMyRank == iRankToPrint) {
        printf("DEBUG: Rank %d data AFTER getting the results\n", iMyRank);
        for (int iRow = 0; iRow < ROWS/iRanksQty + 2; ++iRow) {
            for (int iCol = 0; iCol < COLS; ++iCol) {
                printf("%.0f ", dSelfData[iRow][iCol]);
            }
            if (iRow == 0) {
                printf("-> Lower ghost line \n");
            } else if (iRow == ROWS/iRanksQty + 1) {
                printf("-> Upper ghost line \n");
            } else {printf("\n");}
        }
        printf("\n");
    }

    MPI_Finalize();

    return SUCCESS;
}