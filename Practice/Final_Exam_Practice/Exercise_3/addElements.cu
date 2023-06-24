#include <iostream>
#include <math.h>
#include <cuda_runtime.h>
#include <cstdio>
#include <stddef.h>

// function to add the elements of two arrays
__global__ void add(int n, float *x, float *y) {
  int iSliceSize = n/(blockDim.x*gridDim.x);
  int iInit      = threadIdx.x*iSliceSize + blockIdx.x*iSliceSize*blockDim.x;
  int iEnd       = iInit + iSliceSize;
  
  // Adjust iEnd if this is the last block and thread
  if (blockIdx.x == gridDim.x - 1 && threadIdx.x == blockDim.x - 1) {
    iEnd = n;
  }
  
  // add the input data
  for (int iIndex = iInit; iIndex < iEnd; ++iIndex) {
    y[iIndex] += x[iIndex];
  }
}

int main(void) {
  int N = 1<<20; // 1M elements
  printf("N is %d\n", N);

  float *fXHost = (float*)calloc(N, sizeof(float));
  float *fYHost = (float*)calloc(N, sizeof(float));

  // initialize x and y arrays on the host
  for (int i = 0; i < N; i++) {
    fXHost[i] = 1.0f;
    fYHost[i] = 2.0f;
  }
  
  // Allocate memory for device
  float *fXDevice;
  float *fYDevice;
  cudaMalloc((void**)&fXDevice, N*sizeof(float));
  cudaMalloc((void**)&fYDevice, N*sizeof(float));
  cudaMemcpy(fXDevice, fXHost, N*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(fYDevice, fYHost, N*sizeof(float), cudaMemcpyHostToDevice);

  int iBlocks  = 10000;
  int iThreads = 100;
  printf("Will run GPU concurrency with %d blocks and %d threads per block\n", iBlocks, iThreads);
  printf("Just about to execute GPU\n");
  // Run kernel on 1M elements on the CPU
  add<<<iBlocks, iThreads>>>(N, fXDevice, fYDevice);
  cudaDeviceSynchronize();
  printf("Out of GPU region\n");

  cudaMemcpy(fYHost, fYDevice, N*sizeof(float), cudaMemcpyDeviceToHost);

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(fYHost[i]-3.0f));
  std::cout << "Max error: " << maxError << std::endl;

  // Free memory
  //delete [] x;
  //delete [] y;
  cudaFree(fXDevice);
  cudaFree(fYDevice);
  free(fXHost);
  free(fYHost);

  return 0;
}
