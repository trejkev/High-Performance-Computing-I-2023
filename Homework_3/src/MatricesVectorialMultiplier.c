#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

// Code to Perform C = AxB
int main(int argc, char** argv) {
    size_t Ndim = 3;  // Number of rows in A
    size_t Mdim = 4;  // Number of columns in B
    size_t Pdim = 5;  // Number of columns in matrix A = rows in matrix B

    // Matrix A values
    double A[] = {
        1 , 2 , 3 , 4 , 5 ,
        6 , 7 , 8 , 9 , 10,
        11, 12, 13, 14, 15
    };

    // Matrix B values
    double B[] = {
        20, 19, 18, 17,
        16, 15, 14, 13,
        12, 11, 10, 9 ,
        8 , 7 , 6 , 5 ,
        4 , 3 , 2 , 1
    };

    // Allocate memory for resulting matrix
    double* C = (double*)malloc(Ndim * Mdim * sizeof(double));

    size_t i, j, k;

    // Ndim for rows in A
    // Mdim for cols in B
    // Pdim for columns in A = rows in B
    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < Ndim ; i++) {
        for (j = 0; j < Mdim ; j++) {
            double tmp = 0;
                for (k = 0; k < Pdim ; k++) {
                    /* C (i , j ) = sum ( over k ) A (i , k ) * B (k , j ) */
                    tmp += *(A+(i*Pdim+k)) * *(B+(k*Mdim+j)); // Same as tmp += A[i*Pdim+k] * B[k*Mdim+j];
                }
            C[i*Mdim+j] = tmp;
        }
    }

    // Print the result matrix
    printf("Result:\n");
    for (size_t i = 0; i < Ndim; i++) {
        for (size_t j = 0; j < Mdim; j++) {
            printf("%.2f ", *(C + (i * Ndim + j)));
        }
        printf("\n");
    }

    // Free the allocated memory
    free(C);
}
