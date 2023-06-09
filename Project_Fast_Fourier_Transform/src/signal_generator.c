#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[]) {
    FILE *fptr;
    char* sSignalDesc = (char*)malloc(200*sizeof(char));
    char* sSignalPath = (char*)malloc(400*sizeof(char));
    size_t iSamples;
    if (argc == 3) {
        sscanf(argv[1], "%s", sSignalDesc);
        sscanf(argv[2], "%zu", &iSamples);
    }

    strcpy(sSignalPath, "./test/");
    strcat(sSignalPath, sSignalDesc);
    strcat(sSignalPath, ".txt");
    printf("Signal path is %s\n", sSignalPath);

    if((fptr = fopen(sSignalPath, "w")) != NULL) {
        fprintf(fptr, "%zu\n", iSamples);
        fprintf(fptr, "%zu\n", iSamples);
        float fTimeStep = 1.0/iSamples;
        for (size_t iCounter = 0; iCounter < iSamples; iCounter++) {
            float t = iCounter*fTimeStep;
            float fSample;
            if (strcmp(sSignalDesc, "sin_50Hz") == 0) {
                fSample = sin(2.0*M_PI*50.0*t);
            } else if (strcmp(sSignalDesc, "sin_50Hz_PLUS_sin_450Hz") == 0) {
                fSample = sin(2.0*M_PI*50.0*t) + sin(2.0*M_PI*450.0*t);
            } else if (strcmp(sSignalDesc, "4_PLUS_sin_50Hz_PLUS_sin_450Hz") == 0) {
                fSample = 4 + sin(2.0*M_PI*50.0*t) + sin(2.0*M_PI*450.0*t);
            } else if (strcmp(sSignalDesc, "sin_1Hz") == 0) {
                fSample = sin(2.0*M_PI*1.0*t);
            } else if (strcmp(sSignalDesc, "cos_1Hz") == 0) {
                fSample = cos(2.0*M_PI*1.0*t);
            } else {
                printf("%s is not a valid signal.\n", sSignalDesc);
                return 1;
            }
            fprintf(fptr, "%f\n", fSample);
        }
    } else {
        printf("Failed to open %s\n", sSignalPath);
    }
    free(sSignalPath);
    free(sSignalDesc);
    return 0;
}