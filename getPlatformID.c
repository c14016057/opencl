#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXPLATFORM 8
int main() {
	//Get platform ID
	cl_platform_id plat_form_id[MAXPLATFORM];
	cl_uint platform_num;
	clGetPlatformIDs(MAXPLATFORM, plat_form_id, &platform_num);
	printf("%d platforms found\n",platform_num);

	return 0;
}
