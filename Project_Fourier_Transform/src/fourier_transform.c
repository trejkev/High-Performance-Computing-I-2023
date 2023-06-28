// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "fourier_transform_main.h"


////////////////////////////////////////////////////////////////////////////////
//                      ITERATIVE IMPLEMENTATION OF DFT                       //
////////////////////////////////////////////////////////////////////////////////
Complex* dft(float* fSampledSignal, size_t iSamplesQty, long* lTimes,
    int iRanksQty, int iMyRank) {
    // FFT frequency components are stored here
    Complex* cSpectrum = (Complex*)calloc(iSamplesQty, sizeof(Complex));
    size_t iBlockSize = iSamplesQty/iRanksQty;
    size_t iInit = iMyRank*iBlockSize;
    size_t iEnd = iInit + iBlockSize;
    if (iMyRank == iRanksQty - 1) {
        iEnd = iSamplesQty;
    }

    lTimes[7] = timeReader();
    for (size_t iFreqIndex = iInit; iFreqIndex < iEnd; iFreqIndex++) {
        if (iFreqIndex == 0) {  // Read only the first time entering
            lTimes[8] = timeReader();
        }
        float fReal = 0;
        float fImag = 0;
        #pragma omp parallel for reduction(+:fReal) reduction(+:fImag)
        for (size_t iTimeIndex = 0; iTimeIndex < iSamplesQty; iTimeIndex++) {
            float fAngle = 2.0*M_PI*iFreqIndex*iTimeIndex/iSamplesQty;
            fReal += fSampledSignal[iTimeIndex]*cos(fAngle);
            fImag += fSampledSignal[iTimeIndex]*sin(fAngle);
        }
        cSpectrum[iFreqIndex].real = fReal;
        cSpectrum[iFreqIndex].imag = fImag;
        if (iFreqIndex == 0) {  // Read only the first time entering
            lTimes[9] = timeReader();
        }
    }
    lTimes[10] = timeReader();
    return cSpectrum;
}


////////////////////////////////////////////////////////////////////////////////
//                      RECURSIVE IMPLEMENTATION OF FFT                       //
////////////////////////////////////////////////////////////////////////////////

Complex* fft(Complex *cSignal, size_t iSamplesQty) {
    if (iSamplesQty <= 1) {
        Complex* cSpectrum = malloc(sizeof(Complex));
        cSpectrum[0] = cSignal[0];
        return cSpectrum;
    }

    Complex* cEvenSignal = malloc(sizeof(Complex) * (iSamplesQty/2));
    Complex* cOddSignal  = malloc(sizeof(Complex) * (iSamplesQty/2));

    // Split input signal into even and odd parts
    for (size_t iIndex = 0; iIndex < iSamplesQty/2; iIndex++) {
        cEvenSignal[iIndex] = cSignal[2*iIndex];
        cOddSignal[iIndex]  = cSignal[2*iIndex + 1];
    }

    // Recursive FFT on even and odd parts
    Complex* cEvenSpectrum = fft(cEvenSignal, iSamplesQty/2);
    Complex* cOddSpectrum  = fft(cOddSignal , iSamplesQty/2);

    // Combine even and odd parts
    Complex* cSpectrum = malloc(sizeof(Complex) * iSamplesQty);
    for (size_t iFreqComp = 0; iFreqComp < iSamplesQty/2; iFreqComp++) {
        float fAngle   = -2.0*M_PI*iFreqComp/iSamplesQty;
        Complex cEuler = {cos(fAngle), sin(fAngle)};
        
        Complex cTemp =
            { cEuler.real * cOddSpectrum[iFreqComp].real
            - cEuler.imag * cOddSpectrum[iFreqComp].imag ,
              cEuler.real * cOddSpectrum[iFreqComp].imag 
            + cEuler.imag * cOddSpectrum[iFreqComp].real };
        
        cSpectrum[iFreqComp].real = cEvenSpectrum[iFreqComp].real + cTemp.real;
        cSpectrum[iFreqComp].imag = cEvenSpectrum[iFreqComp].imag + cTemp.imag;
        
        cSpectrum[iFreqComp + iSamplesQty/2].real =
            cEvenSpectrum[iFreqComp].real - cTemp.real;
        cSpectrum[iFreqComp + iSamplesQty/2].imag =
            cEvenSpectrum[iFreqComp].imag - cTemp.imag;
    }

    free(cEvenSignal);
    free(cOddSignal);
    free(cEvenSpectrum);
    free(cOddSpectrum);

    return cSpectrum;
}

Complex* fft_preprocessor(float *fSignal, size_t iSamplesQty, long* lTimes) {
    Complex* cSignal = (Complex*)calloc(iSamplesQty, sizeof(Complex));

    // Convert input signal into complex signal
    for (size_t iSample = 0; iSample < iSamplesQty; iSample++) {
        cSignal[iSample].real = fSignal[iSample];
        cSignal[iSample].imag = 0; 
    }

    // Compute fft and elapsed time
    lTimes[7]       = timeReader();
    Complex *result = fft(cSignal, iSamplesQty);
    lTimes[10]      = timeReader();

    free(cSignal);

    return result;
}