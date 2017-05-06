#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXPLATFORM 8
#define MAXINFO 128
int main() {
	//Get platform ID
	cl_platform_id plat_form_id[MAXPLATFORM];
	cl_uint platform_num;
	clGetPlatformIDs(MAXPLATFORM, plat_form_id, &platform_num);
	printf("%d platforms found\n",platform_num);

	//Get platform Info
	for (int i = 0; i < platform_num; i++) {
		char buffer[MAXINFO];
		size_t len;

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_NAME, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("platform name %s\n", buffer);
		
		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_VENDOR, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("platform vendor %s\n", buffer);

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_VERSION, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("platform version %s\n", buffer);

		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_PROFILE, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("platform profile %s\n", buffer);


		clGetPlatformInfo(plat_form_id[i], CL_PLATFORM_EXTENSIONS, MAXINFO, buffer, &len);
		buffer[len] = 0;
		printf("platform extensions %s\n", buffer);
	}
	return 0;
}
