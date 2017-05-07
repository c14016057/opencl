#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXKS 256
#define NUMGPU 3
#define NUMPF 1
#define N 100
int main() {
	cl_uint *A = (cl_uint *) malloc(N * sizeof(cl_uint));
	cl_uint *B = (cl_uint *) malloc(N * sizeof(cl_uint));
	cl_uint *C = (cl_uint *) malloc(N * sizeof(cl_uint));
	
	for (int i = 0; i < N; i++) {
		A[i] = i;
		B[i] = N - i;
	}
	//PLATFORM
	cl_int st;
	cl_platform_id pf_id;
	cl_uint pf_nu;
	st = clGetPlatformIDs(NUMPF, &pf_id, &pf_nu);
	assert(st == CL_SUCCESS);

	//DEVICE
	cl_device_id gpu[NUMGPU];
	cl_uint gpu_nu;
	st = clGetDeviceIDs(pf_id, CL_DEVICE_TYPE_GPU, NUMGPU, gpu, &gpu_nu);
	assert(st == CL_SUCCESS);

	//CONTEXT
	cl_context ct;
	ct = clCreateContext(NULL, NUMGPU, gpu, NULL, NULL, &st);
	assert(st == CL_SUCCESS);

	//COMMAND QUEUE
	cl_command_queue cq;
	cq = clCreateCommandQueue(ct, gpu[0], 0, &st);// 0 is properity
	assert(st == CL_SUCCESS);
	
	//program
	FILE *kfile = fopen("vectorAdd.cl","r");
	assert(kfile != NULL);
	char ks[MAXKS];
	const char *conks = ks;
	size_t kl = fread(ks, sizeof(char), MAXKS, kfile);
	assert(feof(kfile));
	cl_program pm = clCreateProgramWithSource(ct, 1, &conks, &kl, &st);// 1 is num of ks
	assert(st == CL_SUCCESS);

	//compile
	st = clBuildProgram(pm, NUMGPU, gpu, NULL, NULL, NULL);
	assert(st == CL_SUCCESS);

	//kernel function
	cl_kernel kfun = clCreateKernel(pm, "add", &st);
	assert(st == CL_SUCCESS);
	//buffer
	cl_mem bufferA =
		clCreateBuffer(ct, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N*sizeof(cl_uint), A, &st);
	assert(st == CL_SUCCESS);

	cl_mem bufferB =
		clCreateBuffer(ct, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N*sizeof(cl_uint), B, &st);
	assert(st == CL_SUCCESS);

	cl_mem bufferC =
		clCreateBuffer(ct, CL_MEM_WRITE_ONLY, N*sizeof(cl_uint), C, &st);
	assert(st == CL_SUCCESS);
	
	//link
	st = clSetKernelArg(kfun, 0, sizeof(cl_mem), (void *)&bufferA);
	assert(st == CL_SUCCESS);

	st = clSetKernelArg(kfun, 1, sizeof(cl_mem), (void *)&bufferB);
	assert(st == CL_SUCCESS);

	st = clSetKernelArg(kfun, 2, sizeof(cl_mem), (void *)&bufferC);
	assert(st == CL_SUCCESS);

	//ndrange
	size_t globalThread[] = {(size_t)N};
	size_t localThread[] = {1};
	st = clEnqueueNDRangeKernel(cq, kfun, 1, NULL, globalThread, localThread, 0, NULL, NULL);// 1 is dim
	assert(st == CL_SUCCESS);

	//GET RESULT
	clEnqueueReadBuffer(cq, bufferC, CL_TRUE, 0, N*sizeof(cl_uint), C, 0, NULL, NULL);
	for (int i = 0; i < N; i++)
		assert(A[i] + B[i] == C[i]);
	printf("success\n");
	//FREE
	free(A);
	free(B);
	free(C);
	clReleaseContext(ct);
	clReleaseCommandQueue(cq);
	clReleaseProgram(pm);
	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	return 0;
}
