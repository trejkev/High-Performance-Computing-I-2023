# Vectorial Product of Matrices

This homework consisted in the creation of a matrices vectorial product calculator, it creates two matrices of dimensions defined by AROWS, BCOLS, and ACOLS_BROWS in the Makefile, with randomized integer values between 0 and 100, and computes, sequentially and concurrently the code, depending if -fopenmp flag is used while compiling.

The equation to compute is the following: $A \times B = C$. See the following example:

```math
\begin{bmatrix}32 & 32 & 54 & 12 & 52 & 56 & 8 \\30 & 44 & 94 & 44 & 39 & 65 & 19 \\51 & 91 & 1 & 5 & 89 & 34 & 25 \\58 & 20 & 51 & 38 & 65 & 30 & 7 \\20 & 10 & 51 & 18 & 43 & 71 & 97 \end{bmatrix} 
\times 
\begin{bmatrix}61 & 26 & 5 \\57 & 70 & 65 \\0 & 75 & 29 \\86 & 93 & 87 \\87 & 64 & 75 \\88 & 89 & 100 \\7 & 40 & 37 \end{bmatrix} 
= 
\begin{bmatrix}14316 & 16870 & 14646 \\17368 & 24043 & 19692 \\19638 & 17958 & 17634 \\16290 & 17377 & 14509 \\14006 & 19670 & 17709 \end{bmatrix}
```

It also provides you with a way to repeat the computation N times, just by modifying the variable REPEAT into the Makefile.

## Running the code
To run the code, the recommended way is to use the Makefile provided in the project as follows:
1. Modify Makefile: 

  &emsp;&emsp;&emsp; Change AROWS to set the number of rows of matrix A.
  
  &emsp;&emsp;&emsp; Change BCOLS to set the number of rows of matrix B.
  
  &emsp;&emsp;&emsp; Change ACOLS_BROWS to set the number of columns of matrix A, which is the same as number of rows of matrix B.
  
  &emsp;&emsp;&emsp; Change REPEAT to set the number of replicas of the matrices multiplication, so that you can collect more elapsed time readings.
  
2. Run make: It will clean bin directory and compile both, concurrent and sequential code.
3. Run make run_{parallel,sequential}: This will run the code for parallel (concurrent) or sequential execution, respectively.
