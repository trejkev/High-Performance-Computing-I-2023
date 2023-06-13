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

## Running the code

1. There is an input file, located in test directory, that mimics a sampled analog signal, the first line represents its samples per second (aka sampling frequency), its second line represents its samples quantity, and from the third line to the very last line represents each of the samples of the signal.
2. This input file is entered to discrete_fourier_transform code as CLI parameter, which will compute each of the components of the signal in frequency, up to the Nyquist limit, which is the mathematical limit where the signal is mirrored, and is equivalent to the sampling frequency divided by 2.
3. The output of the code will be a csv file with each component, which can be plotted later, if required.
