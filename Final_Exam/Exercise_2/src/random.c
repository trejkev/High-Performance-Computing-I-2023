
//**********************************************************
// Pseudo random number generator:
//     double random
//     void seed (lower_limit, higher_limit)
//**********************************************************
//
// A simple linear congruential random number generator
// (Numerical Recipies chapter 7, 1st ed.) with parameters
// from the table on page 198j.
//
//  Uses a linear congruential generator to return a value between
//  0 and 1, then scales and shifts it to fill the desired range.  This
//  range is set when the random number generator seed is called.
// 
// USAGE:
//
//      pseudo random sequence is seeded with a range
//
//            void seed(lower_limit, higher_limit)
//   
//      and then subsequent calls to the random number generator generates values
//      in the sequence:
//
//            double drandom()
//
// History: 
//      Written by Tim Mattson, 9/2007.
//      changed to drandom() to avoid collision with standard libraries, 11/2011

#include "random.h"

static long MULTIPLIER  = 1366;
static long ADDEND      = 150889;
static long PMOD        = 714025;
double random_low, random_hi;

double drandom() {
    // compute an integer random number from zero to mod
    long random_last;
    long random_next = (MULTIPLIER  * random_last + ADDEND)% PMOD;
    random_last = random_next;

    // shift into preset range
    return ((double)random_next/(double)PMOD)*(random_hi-random_low)+random_low;
}

// set the seed and the range
void seed(double low_in, double hi_in) {
    if(low_in < hi_in) { 
        random_low = low_in;
        random_hi  = hi_in;
    } else {
        random_low = hi_in;
        random_hi  = low_in;
    }
}

