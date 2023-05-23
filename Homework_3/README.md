# Pi number approximation

This homework consisted in the creation of a Pi approximator by "integrating" the function f(x) = 4/(1+x^2) from 0 to 1.

The work was done by distributing between multiple processes, using MPI, the task to be done, utilizing weak scaling, so that every process will keep the same workload of computations, the difference is that adding more processes will reduce the delta X, thus, obtaining a more accurate approximation of what the integral should be. The area below the following curve is exactly pi, and that is the intention of the exercise, showing how adding more processes can enhance the results. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/18760154/231065792-b40f1d9a-76bf-4929-bb78-8be7007e7ab0.png" />
</p>

The integral computation is performed using the trapezoid approximation, which uses the following equation. Therefore, the shorter the delta x, the better the approximation.

$$ \int_{a}^{b}f(x)dx\approx \frac{1}{2}\Delta x\sum_{i=a}^{b-\Delta x}\left [ f(x_{i}) + f(x_{i} + \Delta x) \right ] $$

For this homework, the focus was not the measurement of the error reduction, it was already demonstrated in Homework 1, thus, for this homework, the focus was to modify the code to make is suitable for MPI + Vectorization, and also full vectorization, and then compare how long it takes for the three different configurations to complete the execution cycle.

Since the system is using a Makefile, running the code is quite simple, use make clean to remove older output files, make lint to check correctness of the syntaxis, and then, depending on what you want, you may need to chose one of the following options
  - MPI only: Run make PiApproximator_MPI, and then make run_MPI.
  - MPI + Vectorization: Run make PiApproximator_MPI_VECTORIZED, and then make run_MPI_VECTORIZED.
  - Vectorization only: Run make PiApproximator_VECTORIZED, and then make run_VECTORIZED.
  - Make all: Simply run make, and then run_{MPI, MPI_VECTORIZED, VECTORIZED}

Makefile notes:
  - When using MPI, to change the quantity of processes you will need to modify the PROCESSES variable into the Makefile. 
  - In the case of vectorization only, you will need to modify TRAPEZOIDSQTY variable to set the quantity of trapezoids to be used to average pi. 
  - To make a valid comparison between MPI, VECTORIZED, or their mixture, TRAPEZOIDSQTY should be 10,000 times PROCESSES.
  - If the user wants to run the test N times, this can be done by modifying the variable REPEAT in the Makefile.

## Results

There were 300 trials executed using  64 processes, and in the case of only vectorization, a total of 640,000 trapezoids, which is equivalent in computational effort. The data can be found into results directory, the raw data for elapsed time was plotted together with an individual value plot, and the resulting graph is shown below.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/07dd9ce1-e967-470f-a211-3b13c314fa8d" />
</p>

As you can tell, vectorized version of MPI trends to late for shorter time, compared to its non-vectorized version, but only vectorization commpared to MPI and MPI vectorized trends to late for longer timme than the both that were using MPI, and this can be pointed to two main reasons:
  - The compiler, they both are using different compilers, and this may be giving different asm code to be run, perhaps MPI code is more optimized.
  - The CPUs usage, even when MPI is made to be run in a cluster, in this case the tests were executed in a single computer, thus there is no latency reduction because of the network communications, resulting simply in a 64-threads concurrent (and even vectorized) execution of 10,000 cycles, in the best scenario, versus a sequential execution of 640,000 cycles.
