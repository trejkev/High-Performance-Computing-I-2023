# Final Exam Guidelines

## Exercise 1

Create a parallel data structure. This structure is a point grid divided by blocks, each block shall be assigned to a different processor. For simplicity in C, the data structure shall be:

$$x[maxn][maxn]$$

And we want it to be structured in a way that each processor has its local portion:

$$xlocal[maxn/size][maxn]$$

Where "size" is the size of the global communicator (which means, the number of processes up). For the computation for which we will be using this data structure, we will need adjacent values. Which means, to compute $x[i][j]$, we will need:

$$x[i][j+1]$$
$$x[i][j-1]$$
$$x[i+1][j]$$
$$x[i-1][j]$$

Some of these may be a problem if they are not allocated in xlocal, but in an adjacent process. To deal with these cases we will define "ghost" slots, which will contain the values of those adjacent slots.

Write a code that divides the input data structure into equal portions, and copy the adjacent borders to the neighbor processes. Assume that the input data structure is of size maxn by maxn, and that maxn is a multiple of the processes number. For simplicity, you can assume that the input data structure is a fixed-size matrix, and a fixed quantity of processes.

To test your solution, each process shall fill its section with the process rank number, and ghost slots with -1. After completing the exchange, check that ghost slots have the correct value. IMPORTANT: Assume that the data structure is not periodic, which means, rank 0 shall not send any data to rank size-1 or vice versa.

The following picture shows an example with maxn = 8 and 4 processes.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/c3bf47ab-bc45-4d42-9012-6ce2ae9476bf" />
</p>
