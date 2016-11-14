// CUDAStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h> 
#include<stdlib.h>
#include <cuda_runtime.h>
#include<time.h>
#include<string>
#include<windows.h>
#define DATA_SIZE 19484
#define BLOCK_NUM 32
#define THREAD_NUM 256

bool InitCUDA()
{
	int count;
	cudaGetDeviceCount(&count);
	if (count == 0) {
		fprintf(stderr, "There is no device.\n");
		return false;
	}
	int i;
	cudaDeviceProp prop;
	 for (i = 0; i < count; i++)
	 {
		if (cudaGetDeviceProperties(&prop, i) == cudaSuccess) {  //取得支持 CUDA 的装置的数目
			if (prop.major >= 1) {								 //装置支持的版本号码       每块最大1024线程
				break;
			}
		}
	}
	if (i == count) {											//device 0 会是一个仿真的装置，但不支持 CUDA 1.0 以上的功能。所以，要确定系统上是否有支持 CUDA 的装置
		fprintf(stderr, "There is no device supporting CUDA 1.x.\n");
		return false;
	}
	cudaSetDevice(i);
	return true;
}
//函数会产生一大堆 0-9 之间的随机数
void GenerateNumbers(int *number, int size)
{
	for (int i = 0; i < size; i++) {
		number[i] = rand() % 10;
	}
}


extern "C"  cudaError_t USECUDA(int *num, int& result);
int _tmain(int argc, _TCHAR* argv[])
{
	if (!InitCUDA())
	{
		return 0;
	}
	printf("CUDA initialized!\n");

	int data[DATA_SIZE];
	GenerateNumbers(data, DATA_SIZE);
	int sum(0);

	DWORD dwStart = GetTickCount();

	USECUDA(data,sum);
	
	
	printf("sum: %d\n", sum);
	DWORD dwUsed = GetTickCount() - dwStart;
	printf("use time: %u\n",dwUsed);

	system("PAUSE");
	return 0;
}

