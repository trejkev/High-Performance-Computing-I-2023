# Concurrent Optimization of Discrete and Fast Fourier Transform

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



## Running the code
1. There is an input file, located in test directoy, that mimics a sampled analog signal, the first line represents its samples per second (aka sampling frequency), its second line represents its samples quantity, and from the third line to the very last line represents each of the samples of the signal.
2. This input file is entered to discrete_fourier_transform code as CLI parameter, which will compute each of the components of the signal in frequency, up to the Nyquist limit, which is the mathematical limit where the signal is mirrored, and is equivalent to the sampling frequency divided by 2.
3. The output of the code will be a csv file with each component, which can be plotted later, if required.
