
// FIXME using strides

#include <iostream>
#include <cstdio>
#include <stddef.h>
#include <cuda_runtime.h>


void init(int *a, int N) {
  for (int i = 0; i < N; ++i) {
    a[i] = i;
  }
}

__global__ void doubleElements(int *a, int N) {
  int iSliceSize = N/(blockDim.x*gridDim.x);
  int iInit      = threadIdx.x*iSliceSize + blockIdx.x*iSliceSize*blockDim.x;
  int iEnd       = iInit + iSliceSize;

  // Adjust iEnd if this is the last block and thread
  if (blockIdx.x == gridDim.x - 1 && threadIdx.x == blockDim.x - 1) {
    iEnd = N;
  }
  
  // Double the input data
  for (int iIndex = iInit; iIndex < iEnd; iIndex++) {
    a[iIndex] *= 2;
  }
}

bool checkElementsAreDoubled(int *a, int N) {
  for (int i = 0; i < N; ++i) {
    if (a[i] != i*2) {
      printf("Element %d not doubled\n", a[i]);
      return false;
    }
  }
  return true;
}

int main()
{
  /*
   * 'N' is greater than the size of the grid (see below).
   */

  int N = 10000;
  int *b = (int*)calloc(N, sizeof(int)); // Host memory allocation

  init(b, N);

  int *a;
  cudaMalloc((void**)&a, N*sizeof(int)); // Device memory pointer
  cudaMemcpy(a, b, N * sizeof(int), cudaMemcpyHostToDevice); // Copy memory contents from host to device

  /*
   * The size of this grid is 256*32 = 8192.
   */

  size_t threads_per_block = 256;
  size_t number_of_blocks = 32;
  
  doubleElements<<<number_of_blocks, threads_per_block>>>(a, N);
  cudaDeviceSynchronize();

  cudaMemcpy(b, a, N * sizeof(int), cudaMemcpyDeviceToHost); // Copy memory contents from device to host

  bool areDoubled = checkElementsAreDoubled(b, N);
  printf("All elements were doubled? %s\n", areDoubled ? "TRUE" : "FALSE");

  cudaFree(a);
  free(b);
}
