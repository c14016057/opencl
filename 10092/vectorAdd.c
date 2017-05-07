#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXKS 25600
#define NUMGPU 3
#define NUMPF 1
#define N 100
int main() {
	
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
	char fname[64];
	scanf("%s", fname);
	FILE *kfile = fopen(fname,"r");
	assert(kfile != NULL);
	char ks[MAXKS];
	const char *conks = ks;
	size_t kl = fread(ks, sizeof(char), MAXKS, kfile);
	assert(feof(kfile));
	cl_program pm = clCreateProgramWithSource(ct, 1, &conks, &kl, &st);// 1 is num of ks
	assert(st == CL_SUCCESS);

	//compile
	st = clBuildProgram(pm, NUMGPU, gpu, NULL, NULL, NULL);
//	assert(st == CL_SUCCESS);

	//get info
	char log[2048];
	size_t len;	
	st = clGetProgramBuildInfo(pm, gpu[0], CL_PROGRAM_BUILD_LOG, 2048, log, &len);
	assert(st == CL_SUCCESS);
	log[len] = 0;
	printf("%s", log);
	
	return 0;
}
