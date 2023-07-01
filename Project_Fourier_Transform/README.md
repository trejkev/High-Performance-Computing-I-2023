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

## Sequential Execution Time Results

There were executed some tests to be able to compare the time DFT takes, compared to FFT, both in sequential execution.

### Discrete Fourier Transform Sequential Time Results using RYZEN 7 4000 SERIES

Since DFT takes more computational time, the trials were executed up to  65536, which is, for this code, the minimum frequency to accomplish Nyquist theorem. As shown in the figure below, the elapsed time starts growing exponentially, ending with an elapsed time of about 140 seconds for 65536 samples per second, analyzed through a single second.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/5eef58f6-dfee-4530-ad93-14aef8b0b95d" />
</p>

### Fast Fourier Transform Sequential Time Results using RYZEN 7 4000 SERIES

Since FFT takes less computational time, the trials were executed up to  524288, which is, 8 times the maximum frequency used with DFT. As shown in the figure below, the elapsed time starts growing linearly, however, because of the enhanced procedure, it ends up with an elapsed time of about 0.18 seconds, which is about 778 times faster than the DFT, making it evident that FFT is more convenient than DFT from time complexity point of view.

<p align="center">
  <img src="https://github.com/trejkev/High-Performance-Computing-I-2023/assets/18760154/69789601-23b1-4daa-a1b8-b6aedb0471b1" />
</p>


