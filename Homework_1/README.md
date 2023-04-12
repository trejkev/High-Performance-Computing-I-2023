# Pi number approximation

This homework consisted in the creation of a Pi approximator by "integrating" the function f(x) = 4/(1+x^2) from 0 to 1.

The work was done by distributing between multiple processes, using MPI, the task to be done, utilizing weak scaling, so that every process will keep the same workload of computations, the difference is that adding more processes will reduce the delta X, thus, obtaining a more accurate approximation of what the integral should be. The area below the following curve is exactly pi, and that is the intention of the exercise, showing how adding more processes can enhance the results. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/18760154/231065792-b40f1d9a-76bf-4929-bb78-8be7007e7ab0.png" />
</p>

The integral computation is performed using the trapezoid approximation, which uses the following equation. Therefore, the shorter the delta x, the better the approximation.

$$ \int_{a}^{b}f(x)dx\approx \frac{1}{2}\Delta x\sum_{i=a}^{b-\Delta x}\left [ f(x_{i}) + f(x_{i} + \Delta x) \right ] $$


Since the system is using a Makefile, running the code is quite simple, use make clean to remove older output files, make lint to check correctness of the syntaxis, make to compile the code, and make run to run the code with 1 process, to change the quantity of processes you will need to modify the PROCESSES variable into the Makefile.

## Results

There were 10 trials executed, where the processes followed the formula Processes = 2^n, where N goes from 0 to 9, the results are shown in the table below.

| **Processes** | **Pi approximation** |     **Error**     |
|:-------------:|:--------------------:|:-----------------:|
|       1       |   3.14159265192313   | 1.66666280776E-09 |
|       2       |   3.14159265317314   |  4.1665693118E-10 |
|       4       |   3.14159265348562   |  1.0417355867E-10 |
|       8       |   3.14159265356374   |  2.604938487E-11  |
|       16      |   3.14159265358329   |   6.5067951E-12   |
|       32      |   3.14159265358816   |   1.62936331E-12  |
|       64      |   3.14159265358938   |   4.1033843E-13   |
|      128      |   3.14159265358969   |   1.0036416E-13   |
|      256      |   3.14159265358977   |    2.442491E-14   |
|      512      |   3.14159265358979   |    7.10543E-15    |

By plotting the error against the processes quantity, it can be seen that the more processes the less the error, at a point that even taking error to logaritmic scale, it is widely noticeable that growing the number of processes will enhance the results.

![image]()

<p align="center">
  <img src="https://user-images.githubusercontent.com/18760154/231334251-f539e97c-d47e-4d93-9932-4f8e857f82f5.png" />
</p>
