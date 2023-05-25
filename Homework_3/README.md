# Vectorial Product of Matrices

This homework consisted in the creation of a matrices vectorial product calculator, it creates two matrices of dimensions defined by AROWS, BCOLS, and ACOLS_BROWS in the Makefile, with randomized integer values between 0 and 100, and computes, sequentially and concurrently the code, depending if -fopenmp flag is used while compiling.

The equation to compute is the following: $A \times B = C$

It also provides you with a way to repeat the computation N times, just by modifying the variable REPEAT into the Makefile.

## Running the code
To run the code, the recommended way is to use the Makefile provided in the project as follows:
1. Modify Makefile: 
* Change AROWS to set the number of rows of matrix A.
* Change BCOLS to set the number of rows of matrix B.
* Change ACOLS_BROWS to set the number of columns of matrix A, which is the same as number of rows of matrix B.
* Change REPEAT to set the number of replicas of the matrices multiplication, so that you can collect more elapsed time readings.
2. Run make: It will clean bin directory and compile both, concurrent and sequential code.
3. Run make run_{parallel,sequential}: This will run the code for parallel (concurrent) or sequential execution, respectively.
