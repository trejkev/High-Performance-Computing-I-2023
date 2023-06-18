# $\pi$ Approximation Using MPI, Vectorization, and Both Together

This homework consisted of the creation of a Pi approximator by performing the following integral $\pi = \int_{0}^{1}\frac{4}{1+x^2}dx$.

The work was done by distributing between multiple processes, using MPI, the task to be done, and utilizing weak scaling, so that every process will keep the same workload of computations, the difference is that adding more processes will reduce the delta X, thus, obtaining a more accurate approximation of what the integral should be. The area below the next curve is exactly pi, which is the exercise's intention, showing how adding more processes can enhance the results. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/18760154/231065792-b40f1d9a-76bf-4929-bb78-8be7007e7ab0.png" />
</p>

The integral computation is performed using the trapezoid approximation, which uses the following equation. Therefore, the shorter the delta x, the better the approximation.

$$ \int_{a}^{b}f(x)dx\approx \frac{1}{2}\Delta x\sum_{i=a}^{b-\Delta x}\left [ f(x_{i}) + f(x_{i} + \Delta x) \right ] $$

For this homework, the focus was not the measurement of the error reduction, which was already demonstrated in Homework 1, thus, for this homework, the focus was to modify the code to make it suitable for MPI + Vectorization, and also full vectorization, and then compare how long it takes for the three different configurations to complete the execution cycle.

Since the system is using a Makefile, running the code is quite simple, use make clean to remove older output files, make lint to check the correctness of the syntaxis, and then, depending on what you want, you may need to choose one of the following options
  - MPI only: Run _make PiApproximator_MPI_, and then _make run_MPI_.
  - MPI + Vectorization: Run _make PiApproximator_MPI_VECTORIZED_, and then _make run_MPI_VECTORIZED_.
  - Vectorization only: Run _make PiApproximator_VECTORIZED_, and then _make run_VECTORIZED_.
  - Make all: Simply run _make_, and then _make run_{MPI, MPI_VECTORIZED, VECTORIZED}_.

Makefile notes:
  - When using MPI, to change the number of processes you will need to modify the PROCESSES variable in the Makefile. 
  - In the case of vectorization only, you will need to modify the _TRAPEZOIDSQTY_ variable to set the number of trapezoids to be used to average $\pi$. 
  - To make a valid comparison between MPI, VECTORIZED, or their mixture, _TRAPEZOIDSQTY_ should be 10,000 times _PROCESSES_.
  - If the user wants to run the test N times, this can be done by modifying the variable _REPEAT_ in the Makefile.

## Results

There were 300 trials executed using  64 processes, and in the case of only vectorization, a total of 640,000 trapezoids, which is equivalent in computational effort. The data can be found in the results directory, the raw data for the elapsed time was plotted together with an individual value plot, and the resulting graph is shown below.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/9e55a81a-7ea8-42ea-a8fb-d6342f6239f2" />
</p>

As you can tell, vectorized version of MPI trends to late for a shorter time, compared to its non-vectorized version, but only vectorization compared to MPI and MPI vectorized trends to late for a longer time than both that were using MPI, and this can be pointed to two main reasons:
  - The compiler, they both are using different compilers, and this may be giving different asm code to be run, perhaps MPI code is more optimized.
  - The CPUs usage, even when MPI is made to be run in a cluster, in this case, the tests were executed in a single computer, thus there is no latency reduction because of the network communications, resulting simply in a 64-thread concurrent (and even vectorized) execution of 10,000 cycles, in the best scenario, versus a sequential execution of 640,000 cycles.
