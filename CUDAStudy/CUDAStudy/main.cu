#include "cuda_runtime.h"  
#include "device_launch_parameters.h" 
#include <stdio.h>
#include<time.h>

#define DATA_SIZE 19484
#define BLOCK_NUM 32
#define THREAD_NUM 256
//不要在一个cpp文件中声明__device__和__global__函数
__global__ void sumOfSquares(int *num, int* result)
{
	extern __shared__ int shared[];
	const int tid = threadIdx.x;
	const int bid = blockIdx.x;
	int sum = 0;
	int i;
	shared[tid] = 0;
	for (i = bid * THREAD_NUM + tid; i < DATA_SIZE;i += BLOCK_NUM * THREAD_NUM) 
	{
		shared[tid] += num[i] * num[i];
	}
	__syncthreads();
	if (tid == 0) 
	{
		result[bid] = shared[0];
	}
}
extern "C"  cudaError_t USECUDA(int* data, int& sum)
{
	int* gpudata;
	int* result;
	cudaError_t cudaStatus;

	cudaStatus = cudaMalloc((void**)&gpudata, sizeof(int)* DATA_SIZE);
	cudaStatus = cudaMalloc((void**)&result, sizeof(int)*BLOCK_NUM);

	cudaStatus = cudaMemcpy(gpudata, data, DATA_SIZE * sizeof(int), cudaMemcpyHostToDevice);


	sumOfSquares << <BLOCK_NUM, THREAD_NUM, THREAD_NUM*sizeof(int) >> >(gpudata, result);//注意共享内存设为0时 结果出错但是不报错

	int sum2[BLOCK_NUM];
	cudaMemcpy(sum2, result, sizeof(int)*BLOCK_NUM, cudaMemcpyDeviceToHost);
	for (int i = 0; i < BLOCK_NUM; i++) {
		sum += sum2[i];
	}

	cudaStatus = cudaGetLastError();

	cudaFree(gpudata);
	cudaFree(result);
	return cudaStatus;
}