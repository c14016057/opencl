#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXPLATFORM 8
#define MAXINFO 128
#define MAXDEVICE 16
int main() {
	//Get platform ID
	cl_platform_id plat_form_id[MAXPLATFORM];
	cl_uint platform_num;
	clGetPlatformIDs(MAXPLATFORM, plat_form_id, &platform_num);
	printf("%d platform found\n",platform_num);

	for (int i = 0; i < platform_num; i++) {	
		//Get platform Info
		char buffer[MAXINFO];
		size_t len;

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_NAME, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("Platform Name %s\n", buffer);
		
		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_VENDOR, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("Platform Vendor %s\n", buffer);

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_VERSION, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("Platform Version %s\n", buffer);

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_PROFILE, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("Platform Profile %s\n", buffer);


		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_EXTENSIONS, MAXINFO, buffer, &len);
		buffer[len] = 0;
		//printf("platform extensions %s\n", buffer);
	
		//Get device ID
		cl_device_id devices[MAXDEVICE];
		cl_uint device_num;
		clGetDeviceIDs(plat_form_id[i], CL_DEVICE_TYPE_ALL, MAXDEVICE, devices, &device_num);
		printf("%d Devices\n", device_num);


		clGetDeviceIDs(plat_form_id[i], CL_DEVICE_TYPE_CPU, MAXDEVICE, devices, &device_num);
		printf("%d CPU Devices\n", device_num);


		clGetDeviceIDs(plat_form_id[i], CL_DEVICE_TYPE_GPU, MAXDEVICE, devices, &device_num);
		printf("%d GPU Devices\n", device_num);
		
		//get device Info
		for (int j = 0; j < device_num; j++) {
			cl_ulong num;
			cl_uint num_cu;
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, MAXINFO, buffer, &len);
			buffer[len] = 0;
			printf("Device name %s\n", buffer);

			clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &num, &len);
			printf("Global memory size %lld\n", (long long)num);

			clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &num, &len);
			printf("Local memory size %lld\n", (long long)num);
			
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cu, &len);
			printf("# of compute units %d\n", (int)num_cu);
			
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_ulong), &num, &len);
			printf("max # of work items in a work group %lld\n", (long long)num);

		}
	}
	return 0;
}
