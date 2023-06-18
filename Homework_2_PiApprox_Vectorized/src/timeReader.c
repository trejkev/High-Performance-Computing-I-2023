// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include "timeReader.h"

long timeReader() {
    struct timespec stTimeReading;

    clock_gettime(CLOCK_MONOTONIC, &stTimeReading);  // Get time reading

    // Compute the time in nanoseconds
    long lSeconds = stTimeReading.tv_sec;
    long lNanoseconds = stTimeReading.tv_nsec;
    return (lSeconds*1000000000L + lNanoseconds);
}