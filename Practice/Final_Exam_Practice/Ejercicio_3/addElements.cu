#include <iostream>
#include <math.h>
#include <cuda_runtime.h>
#include <cstdio>
#include <stddef.h>

// function to add the elements of two arrays
__global__ void add(int n, float *x, float *y) {
  int iSliceSize = n/(blockDim.x*gridDim.x);
  int iInit      = threadIdx.x*iSliceSize + blockIdx.x*iSliceSize*blockDim.x;
  printf("Thread  %d, block %d, silice size %d, init %d, end %d\n",
    threadIdx.x, blockIdx.x, iSliceSize, iInit, iInit + iSliceSize - 1);
  for (int iIndex = iInit; iIndex < iInit + iSliceSize; iInit++) {
    y[iIndex] = x[iIndex] + y[iIndex];
  }
}

int main(void) {
  int N = 1<<20; // 1M elements
  printf("N is %d\n", N);

  float *x = new float[N];
  float *y = new float[N];
  //float *x;
  //float *y;
  //cudaMallocManaged(&x, N*sizeof(float));
  //cudaMallocManaged(&y, N*sizeof(float));

  // initialize x and y arrays on the host
  for (int i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }
  
  int iBlocks  = 10000;
  int iThreads = 100;
  printf("Will run GPU concurrency with %d blocks and %d threads per block\n",
    iBlocks, iThreads);
  printf("Just about to execute GPU\n");
  // Run kernel on 1M elements on the CPU
  add<<<iBlocks, iThreads>>>(N, x, y);
  cudaDeviceSynchronize();
  printf("Out of GPU region\n");

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i]-3.0f));
  std::cout << "Max error: " << maxError << std::endl;

  // Free memory
  delete [] x;
  delete [] y;
  //cudaFree(x);
  //cudaFree(y);

  return 0;
}
