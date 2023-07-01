# Concurrent Optimization of Discrete and Fast Fourier Transforms

This homework consisted of creating a Discrete and Fast Fourier Transform, optimized in time by executing it concurrently using multi-threading and multi-process methodologies.

## Introduction

The implemented system for this project consists of the implementation of the fourier transform for discrete-time, on its both versions, Discrete Fourier Transform, aka DFT, and Fast Fourier Transform, aka FFT.
For both cases results are the same, the difference is on how to resolve the problem. DFT is mathematically equivalent to Fourier Transform in continuous time, simply changes the interpretation of some variables, like using summation instead of integration, and sampled version of f(t), instead of f(t) itself, both equations are shown below.

$$F(\omega ) = \int_{-\infty }^{\infty }f(t)e^{-jwt}dt = \sum_{n=0}^{N-1} x_{n}e^{-\frac{2\pi i}{N}kn} = \sum_{n=0}^{N-1} x_{n}(cos(\frac{2\pi kn}{N})-j sin(\frac{2\pi kn}{N}))$$

Where:
1. $\omega = 2\pi f$
2. $f \approx k/N$
3. $n  = frequencyComponent$
4. $k \approx t$
5. $j = i$

Having this context, it is possible to appreciate that programming the code that brings the DFT is not that complex, it is a single summatory that can be taken as a for loop, and in which is computed a product of two terms. However, it shall be considered that this product shall be made considering all time-components for the given frequency, which means that the algorithmic complexity for this code would be something like $O(n^2)$.

Resolving this problem in an acceptable computational time for real applications, like audio processing or something similar (see _digital signal processing_) may look initially impossible, given that, for example, the maximum audible frequency for humans is about 20 kHz, and according to Nyquist theorem, we should be sampling the signal at least ad double of its maximum frequency to collect enough data to reconstruct the signal. Having this in mind, for a single second of processing the signal, we shall be sampling during about 1.6 G cycles (assuming FT is a single-liner code), which is computationally out of bounds, and considering that most applications will not be single-liners, this time is not acceptable.

Here is where FFT comes into play, which brings a clever solution to DFT, it works by dividing to conquer, splitting the FT equation into even and odd from trial to trial, then each sub-FT will only need to compute half frequencies of its parent FT, this implementation in recursion brings a computational complexity of about $O(n*log(n))$, where $log(n)$ comes from the nature of splitting by two on each recursion, and $n$ from the number of recursions to perform.

This FFT is a high-valued approach, since high-density signals, $log(n)$ is quite short in comparison with $n$, reducing the computational time drastically.

## Running the Code

Running the code is quite simple, you can use the Makefile provided to reproduce any of the standard scenarios:
1. To run the code simply execute _make_, and then _make run_.
2. To run the code with Valgrind memory check, simply run _make memcheck_.
3. To run the code with Address Sanitizer (aka asan) simply run _make asan_, and then _make run_.

Into the makefile are some variables of interest to try with different scenarios, these are enumerated below:
1. PROCESSES: Unused for now, will bring to the game MPI library to concurrently execute the code with distributed memory.
2. THREADS: Unused for now, will bring to the game OpenMP to concurrently execute the code with shared memory.
3. REPEAT: Executes the code N times, and saves the time for each of the trials into a csv file located in the results directory.
4. SAMPLING_FREQ: This will be the same as samples quantity, and denotes how many samples per second were taken from the original signal.
5. SIGNAL_DESC: Defines which signal will be sampled, the list of signals available can be found in src/signal_generator.c.
6. FOURIERTYPE: Defines if FFT or DFT will be executed, it is case sensitive and shall be sent lowercase.

Note: The code will use both, distributed and shared memory concurrency, to reduce as much as possible the execution time.

## Code Functionality

This code simply computes the Discrete Fourier Transformation to any discrete/sampled signal, where the sampling frequency is well known. For verification purposes, some standard functions were sampled, their resulting samples for a total of four periods, and their corresponding Fourier Transformation amplitude is shown below.

### 4 + sin at 50 Hz + sin at 450 Hz

#### Time Samples

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/cce6c024-a84b-481e-b748-4c37ea39e398" />
</p>

#### Fourier Transform of the Signal

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/e9d36f5f-2df5-45bd-b919-f6508f54d0c4" />
</p>

### Square at 50 Hz

#### Time Samples

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/7ed63dd7-f985-4873-8ca1-ba52747f8014" />
</p>

#### Fourier Transform of the Signal

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/7f9767b3-1e0d-40bd-89c2-fe03d0412edb" />
</p>

### Sawtooth at 150 Hz

#### Time Samples

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/ef84d39e-8fb0-410d-abe5-ea854c638e56" />
</p>

#### Fourier Transform of the Signal

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/7c89f36b-77c6-4516-a089-cbb62411882e" />
</p>

## Sequential Execution Time Trending Results: Sizing the Problem

There were executed some tests to be able to compare the time DFT takes, compared to FFT, both in sequential execution. The intention of this section is to provide the reader with enough information to understand how big is the problem with performing a DFT efficiently.

The tests were executed using a relatively new AMD RYZEN 7 4000 SERIES processor for user-side purposes, with 8 cores and 16 threads, and an Intel Xeon Phi KNL, a relatively old processor, but intended for server-side purposes, which means, more cores and threads, 64 cores and 256 threads to be more precise. The advantage of RYZEN over Xeon here is that RYZEN can run up to 4.4 GHz, while Xeon Phi up to 1.5 GHz, almost 3 times faster.

Because of the limited capabilities of RYZEN processor, it was only used to take a look at the sequential results, since performing concurrent testing using this processor, and trying to conclude relevant things, is quite difficult with the limited resources provided.

### Discrete Fourier Transform Sequential Time Results using RYZEN 7 4000 SERIES

Since DFT takes more computational time, the trials were executed up to  65536, which is, for this code, the minimum frequency to accomplish Nyquist theorem for a human-audible signal. As shown in the figure below, the elapsed time starts growing exponentially, ending with an elapsed time of about 140 seconds for 65536 samples per second, analyzed through a single second.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/507c3ad7-50a7-4d45-9898-9918f181c82e" />
</p>

### Fast Fourier Transform Sequential Time Results using RYZEN 7 4000 SERIES

Since FFT takes less computational time, the trials were executed up to  524288, which is, 8 times the maximum frequency used with DFT. As shown in the figure below, the elapsed time starts growing linearly, however, because of the enhanced procedure, it ends up with an elapsed time of about 0.18 seconds, which is about 778 times faster than the DFT, making it evident that FFT is more convenient than DFT from time complexity point of view.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/e5216e6f-da3b-4d16-8486-5db141bfc76b" />
</p>

## Concurrency Strategy

Since this code has both, DFT and FFT, it was analyzed the need for parallelization of they both.


### DFT Concurrency Analysis

This transformation is composed by two nested for loops, which points directly to something that can be executed concurrently. See the figure below.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/6449b331-fd8b-463f-ba5e-98e9b12d80a7" />
</p>

To avoid issues with shared memory dealing with distributed memory, a strategy of distributed memory dealing with shared memory was used. Therefore, the outer loop is to be concurrently executed with OpenMPI, while the inner loop is to be executed with OpenMP, reason why only synchronization is needed to record the results, and the complexity of the threads are fully managed by each of the processes.

In the end, since both OpenMPI and OpenMP were used, MPI communicator initializer was changed to be the thread equivalent, which is $MPI\_Init\_thread()$, requesting to be MPI_THREAD_MULTIPLE, the processes quantity is a number sent by the user when running the code, internally into DFT code, since OpenMPI is taking the outer loop, it was needed to dimension the block that each rank will be taking, so that block size is $iSamplesQty/iRanksQty$, initial position is $iMyRank*iBlockSize$, and final position is $iInit + iBlockSize$.

The inner loop, the one that was using OpenMP, was required to be modified, since it modifies a data structure that must exist out of this loop, however, solving this is quite simple, since this loop modifies the same member of the data structure for all the trials, thus, a reduction using a temporary variable, and an assignment afterward was used.

### FFT Concurrency Analysis

Looking at FFT code, it is already well optimized, with a computational complexity of $O(n*log(n)$, however, it was analyzed to define which concurrency strategy may fit the best. The FFT code has only one critical section that could be used for parallelization, which is a for loop embedded in it, see the figure below.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/406c7422-5cd8-4d19-8068-03ca8a2f8490" />
</p>

In regards to OpenMPI, since it is a process-based concurrency, it separates the memory between each of the ranks, and because FFT is a concurrent method, it is not possible to use it, since for each iteration there may be the need for each rank to get other ranks data.

Regarding OpenMP, the problem resides in the complexity of the data structure being used, and the operations being applied to it, because the data structure is composed of multiple components, it is not possible to declare it as private, because of the complexity of its operations, it is not possible to perform atomic operations with this data structure, and since it is a shared data structure, OpenMP will internally perform synchronization strategies that will reduce heavily the performance of the application, likely beyond its concurrent version.

Since FFT is an already-optimized algorithm, no concurrent strategy will be applied, but will be used for reference of DFT results.


## Concurrent Results

To generate data regarding the concurrency implemented it was created the following test plan. Since the system is using a Hybrid version of OpenMPI and OpenMP, it was needed to mix the number of processes and threads to bring here what the enhancements provided by each of them are. The test plan followed is shown below.

| **TestCase** | **ProcessesQty** | **ThreadsQty** | **FT_Type** | **Replicas** | **SamplesQty** |  **Signal** |
|:------------:|:----------------:|:--------------:|:-----------:|:------------:|:--------------:|:-----------:|
|      -1      |         1        |        1       |     fft     |      30      |      16384     | square_50Hz |
|       0      |         1        |        1       |     dft     |      30      |      16384     | square_50Hz |
|       1      |         1        |        2       |     dft     |      30      |      16384     | square_50Hz |
|       2      |         1        |        4       |     dft     |      30      |      16384     | square_50Hz |
|       3      |         1        |        8       |     dft     |      30      |      16384     | square_50Hz |
|       4      |         1        |       16       |     dft     |      30      |      16384     | square_50Hz |
|       5      |         1        |       32       |     dft     |      30      |      16384     | square_50Hz |
|       6      |         1        |       64       |     dft     |      30      |      16384     | square_50Hz |
|       7      |         2        |        1       |     dft     |      30      |      16384     | square_50Hz |
|       8      |         4        |        1       |     dft     |      30      |      16384     | square_50Hz |
|       9      |         8        |        1       |     dft     |      30      |      16384     | square_50Hz |
|      10      |        16        |        1       |     dft     |      30      |      16384     | square_50Hz |
|      11      |        32        |        1       |     dft     |      30      |      16384     | square_50Hz |
|      12      |        64        |        1       |     dft     |      30      |      16384     | square_50Hz |
|      13      |        128       |        1       |     dft     |      30      |      16384     | square_50Hz |
|      14      |        256       |        1       |     dft     |      30      |      16384     | square_50Hz |
|      15      |         2        |        2       |     dft     |      30      |      16384     | square_50Hz |
|      16      |         3        |        3       |     dft     |      30      |      16384     | square_50Hz |
|      17      |         4        |        4       |     dft     |      30      |      16384     | square_50Hz |
|      18      |         5        |        5       |     dft     |      30      |      16384     | square_50Hz |
|      19      |         6        |        6       |     dft     |      30      |      16384     | square_50Hz |
|      20      |         7        |        7       |     dft     |      30      |      16384     | square_50Hz |
|      21      |         8        |        8       |     dft     |      30      |      16384     | square_50Hz |
|      22      |        16        |       16       |     dft     |      30      |      16384     | square_50Hz |
|      23      |        32        |       32       |     dft     |      30      |      16384     | square_50Hz |
