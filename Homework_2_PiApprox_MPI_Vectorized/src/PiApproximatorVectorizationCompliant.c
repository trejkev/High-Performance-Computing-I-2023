// Copyright 2023 Kevin Trejos Vargas <kevin.trejosvargas@ucr.ac.cr>

#include <stdio.h>
#include <math.h>  // Use floating abs
#include <stdlib.h>
#include "timeReader.h"

#define PI 3.141592653589793238462643383279502884197

int main(int argc, char** argv) {
    int iColumnsQty = 1000;
    if (argc > 1) {
        sscanf(argv[1], "%d", &iColumnsQty);  // Asign arg[2] to i_Variable
        printf("Running %s trapezoids\n", argv[1]);
    }
    float fAreas[iColumnsQty];
    short iRepeatNTimes = 1;
    if (argc > 2) {
        sscanf(argv[2], "%hd", &iRepeatNTimes);  // Asign arg[2] to i_Variable
        printf("Running %s times\n", argv[2]);
    }

    for (short iTrial = 0; iTrial < iRepeatNTimes; iTrial++) {
        // Gets initial time reading
        long start_time = timeReader();
        float fDeltaX = 1.0/iColumnsQty;
        float fArea = 0;  // Will store the area computed by the current rank
        float fLowerBound = 0;
        float fXToEvaluate;  // Will store x value to be evaluated

        // Performs summatory evaluation using trapezoid approximation
        for (int iTrapezoidNum = 0; iTrapezoidNum < iColumnsQty; iTrapezoidNum++) {
            fAreas[iTrapezoidNum] = 0;
            // Trapezoid approximation requires evaluation at both bounds
            for (int iOffset = 0; iOffset <= 1; iOffset++) {
                float fNextTrapezoid = iTrapezoidNum + iOffset;
                fXToEvaluate = fLowerBound + fNextTrapezoid*fDeltaX;
                fAreas[iTrapezoidNum] += 4.0/(1 + fXToEvaluate*fXToEvaluate);
            }
        }
        for (int iCounter = 0; iCounter < iColumnsQty; iCounter++) {
            fArea += fAreas[iCounter];
        }

        // Trapezoid approximation performs calculations to all evals
        fArea *= fDeltaX;
        fArea /= 2;
        float fError = fabs(fArea - PI);

        // Gets final time reading
        long finish_time = timeReader();
        // Computing elapsed time
        long fElapsedTimeNanoSec = finish_time - start_time;
        float fElapsedTimeSec    = (float)fElapsedTimeNanoSec/1000000000L;
        printf("Columns;%d;PiApprox;%.32f;Error;%.32f;ElapsedTime;%.9lf\n",
            iColumnsQty, fArea, fError, fElapsedTimeSec);
    }

    return EXIT_SUCCESS;
}
