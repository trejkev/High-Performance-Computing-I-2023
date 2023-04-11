# Pi number approximation

This homework consisted in the creation of a Pi approximator by "integrating" the function f(x) = 4/(1+x^2) from 0 to 1.

The work was done by distributing between multiple processes, using MPI, the task to be done, utilizing weak scaling, so that every process will keep the same workload of computations, the difference is that adding more processes will reduce the delta X, thus, obtaining a more accurate approximation of what the integral should be. The area below the following curve is exactly pi, and that is the intention of the exercise, showing how adding more processes can enhance the results. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/18760154/231065792-b40f1d9a-76bf-4929-bb78-8be7007e7ab0.png" />
</p>

The integral computation is performed using the trapezoid approximation, which uses the following equation. Therefore, the shorter the delta x, the better the approximation.

$$ \int_{a}^{b}f(x)dx\approx \frac{1}{2}\Delta x\sum_{i=a}^{b-\Delta x}\left [ f(x_{i}) + f(x_{i} + \Delta x) \right ] $$


Since the system is using a Makefile, running the code is quite simple, use make clean to remove older output files, make lint to check correctness of the syntaxis, make to compile the code, and make run to run the code with 1 process, to change the quantity of processes you will need to modify the PROCESSES variable into the Makefile.
