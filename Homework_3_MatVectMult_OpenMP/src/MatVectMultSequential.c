// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include "timeReader.h"
#include <errno.h>

#define UPPER 100
#define LOWER 0

// Code to Perform C = AxB
int main(int argc, char** argv) {
    size_t replicas;
    size_t rowsOfA;  // Number of rows in A
    size_t colsOfB;  // Number of columns in B
    size_t colsOfA_rowsOfB;  // Number of columns in matrix A = rows in matrix B
    if (argc == 5) {
        if (sscanf(argv[1], "%zu", &replicas) != 1 || errno) {
            fprintf(stderr, "Invalid number of replicas.\n");
            return EXIT_FAILURE;
        }
        if (sscanf(argv[2], "%zu", &rowsOfA) != 1 || errno) {
            fprintf(stderr, "Invalid rows for A.\n");
            return EXIT_FAILURE;
        }
        if (sscanf(argv[3], "%zu", &colsOfB) != 1 || errno) {
            fprintf(stderr, "Invalid cols for B.\n");
            return EXIT_FAILURE;
        }
        if (sscanf(argv[4], "%zu", &colsOfA_rowsOfB) != 1 || errno) {
            fprintf(stderr, "Invalid cols for A / rows for B.\n");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Invalid number of arguments.\n");
        return EXIT_FAILURE;
    }
    printf("Replicas: %zu\n", replicas);
    printf("Rows for A: %zu\n", rowsOfA);
    printf("Cols for B: %zu\n", colsOfB);
    printf("Cols for A / Rows for B: %zu\n", colsOfA_rowsOfB);
    size_t row, col;

    // Allocate memory for matrix A and its values
    size_t** A = (size_t**)calloc(rowsOfA, sizeof(size_t*));
    if (A == NULL) {
        return 1;
    }
    for (row = 0; row < rowsOfA; row++) {
        if ( (A[row] = (size_t*)calloc(colsOfA_rowsOfB, sizeof(size_t))) == NULL) {
            for (int rowToBeFreed = 0; rowToBeFreed <= row; rowToBeFreed++) {
                free(A[rowToBeFreed]);
            }
            free(A);
            return 1;
        }
    }
    // Assign random values to matrix A cells
    for (row = 0; row < rowsOfA; row++) {
        for(col = 0; col < colsOfA_rowsOfB; col++) {
            A[row][col] =  (size_t)(rand() % (UPPER - LOWER + 1)) + LOWER;
        }
    }

    // Allocate memory for matrix B and its values
    size_t** B = (size_t**)calloc(colsOfA_rowsOfB, sizeof(size_t*));
    if (B == NULL) {
        return 1;
    }
    for (row = 0; row < colsOfA_rowsOfB; ++row) {
        if ( (B[row] = (size_t*)calloc(colsOfB, sizeof(size_t))) == NULL) {
            for (int rowToBeFreed = 0; rowToBeFreed <= row; ++rowToBeFreed) {
                free(B[rowToBeFreed]);
            }
            free(B);
            return 1;
        }
    }
    // Assign random values to matrix B cells
    for (row = 0; row < colsOfA_rowsOfB; row++) {
        for(col = 0; col < colsOfB; col++) {
            B[row][col] =  (rand() % (UPPER - LOWER + 1)) + LOWER;
        }
    }

    // Allocate memory for resulting matrix
    size_t** C = (size_t**)calloc(rowsOfA, sizeof(size_t*));
    if( C == NULL) {
        free(C);
        return 1;
    }
    for (row = 0; row < rowsOfA; row++) {
        if ( (C[row] = (size_t*)calloc(colsOfB, sizeof(size_t))) == NULL) {
            for (int rowToBeFreed = 0; rowToBeFreed <= row; rowToBeFreed++) {
                free(C[rowToBeFreed]);
            }
            free(C);
            return 1;
        }
    }

    size_t rowA, colB, colA_rowB;

    for (size_t replica = 0; replica < replicas; ++replica) {
        long start_time = timeReader();
        for (rowA = 0; rowA < rowsOfA ; rowA++) {
            for (colB = 0; colB < colsOfB ; colB++) {
                size_t tmp = 0;
                for (colA_rowB = 0; colA_rowB < colsOfA_rowsOfB ; colA_rowB++) {
                    tmp += A[rowA][colA_rowB] * B[colA_rowB][colB];
                }
                C[rowA][colB] = tmp;
            }
        }

        long finish_time = timeReader();
        double fElapsedTimeNanoSec = (finish_time - start_time);
        float fElapsedTimeSec = fElapsedTimeNanoSec/1000000000L;

        // Print the result matrix
        printf("%zu - Elapsed time: %.9lf s\n", replica, fElapsedTimeSec);
    }

    printf("\nMatrix A:\n");
    for (row = 0; row < rowsOfA; row++) {
        for (col = 0; col < colsOfA_rowsOfB; col++) {
            printf("%zu, ", A[row][col]);
        }
        printf("\n");
    }
    printf("\nMatrix B:\n");
    for (row = 0; row < colsOfA_rowsOfB; row++) {
        for (col = 0; col < colsOfB; col++) {
            printf("%zu, ", B[row][col]);
        }
        printf("\n");
    }

    printf("\nResult:\n");
    for (size_t row = 0; row < rowsOfA; row++) {
        for (size_t col = 0; col < colsOfB; col++) {
            printf("%zu, ", C[row][col]);
        }
        printf("\n");
    }

    // Free the allocated memory
    for (row = 0; row < rowsOfA; row++) {
        free(A[row]);
    }
    free(A);
    printf("Freed A\n");

    for (row = 0; row < colsOfA_rowsOfB; row++) {
        free(B[row]);
    }
    free(B);
    printf("Freed B\n");
    
    for (row = 0; row < rowsOfA; row++) {
        free(C[row]);
    }
    free(C);
    printf("Freed C\n");
}
