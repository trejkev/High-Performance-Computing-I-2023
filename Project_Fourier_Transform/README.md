# Concurrent Optimization of Discrete and Fast Fourier Transform

This homework consisted of creating a Discrete and Fast Fourier Transform, optimized in time by executing it concurrently using multi-threading and multi-process methodologies.

The code works this way:
1. There is an input file, located in test directoy, that mimics a sampled analog signal, the first line represents its samples per second (aka sampling frequency), its second line represents its samples quantity, and from the third line to the very last line represents each of the samples of the signal.
2. This input file is entered to discrete_fourier_transform code as CLI parameter, which will compute each of the components of the signal in frequency, up to the Nyquist limit, which is the mathematical limit where the signal is mirrored, and is equivalent to the sampling frequency divided by 2.
3. The output of the code will be a csv file with each component, which can be plotted later, if required.
