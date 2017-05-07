#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>
#include <inttypes.h>
#define MAXKS 10240
#define NUMGPU 1
#define NUMPF 1
#define MAXN 16777216
#define MAXC 640
#define MAXT 640
int main() {
	cl_uint C[MAXC] ={};
	
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
	FILE *kfile = fopen("vecdot.cl", "r");
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
	cl_kernel kfun = clCreateKernel(pm, "mydot", &st);
	assert(st == CL_SUCCESS);

	//buffer

	cl_mem bufferC =
		clCreateBuffer(ct, CL_MEM_WRITE_ONLY, MAXC*sizeof(cl_uint), C, &st);
	assert(st == CL_SUCCESS);
	

	//ndrange
	size_t globalThread[] = {(size_t)MAXT};
	size_t localThread[] = {32};
	
	int N;
	cl_uint  k1, k2, count, kerT = MAXT;
	while (scanf("%d %" PRIu32 " %" PRIu32, &N, &k1, &k2) == 3) {
		//link
		st = clSetKernelArg(kfun, 0, sizeof(cl_uint), (void *)&k1);
		assert(st == CL_SUCCESS);

		st = clSetKernelArg(kfun, 1, sizeof(cl_uint), (void *)&k2);
		assert(st == CL_SUCCESS);

		st = clSetKernelArg(kfun, 2, sizeof(cl_mem), (void *)&bufferC);
		assert(st == CL_SUCCESS);

		st = clSetKernelArg(kfun, 3, sizeof(cl_int), (void *)&N);
		assert(st == CL_SUCCESS);
	 
		st = clSetKernelArg(kfun, 4, sizeof(cl_uint), (void *)&kerT);
		assert(st == CL_SUCCESS);
		
		st = clEnqueueNDRangeKernel(cq, kfun, 1, NULL, globalThread, localThread, 0, NULL, NULL);// 1 is dim
		assert(st == CL_SUCCESS);

		//GET RESULT
		clEnqueueReadBuffer(cq, bufferC, CL_TRUE, 0, MAXC*sizeof(cl_uint), C, 0, NULL, NULL);
		count = 0;
		for (int i = 0; i < MAXC; i++) {
			count += C[i];
			C[i] = 0;
		}
		printf("%" PRIu32 "\n", count);
	}



	//FREE
	clReleaseContext(ct);
	clReleaseCommandQueue(cq);
	clReleaseProgram(pm);
	clReleaseMemObject(bufferC);
	return 0;
}
