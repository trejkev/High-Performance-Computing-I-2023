#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef PI
    #define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#endif

int main(int argc, char *argv[]) {
    FILE *fptr;
    char* sFileName = (char*)malloc(200*sizeof(char));
    size_t iSamples;
    if (argc == 3) {
        sscanf(argv[1], "%s", sFileName);
        sscanf(argv[2], "%zu", &iSamples);
    }
    if((fptr = fopen("./test/signalA.txt", "w")) != NULL) {
        fprintf(fptr, "%zu\n", iSamples);
        fprintf(fptr, "%zu\n", iSamples);
        for (int iCounter = 0; iCounter < iSamples; iCounter++) {
            float fSample = 2 + 0.5*sin(2*PI*iCounter/iSamples) +
                0.3*cos(2*PI*iCounter/iSamples+0.3);
            fprintf(fptr, "%f\n", fSample);
        }
    } else {
        printf("Failed to open signalA.txt\n");
    }
    free(sFileName);
    return 0;
}