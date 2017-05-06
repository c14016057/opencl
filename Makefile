all: getPlatformID getPlatformInfo getDeviceID getDeviceInfo

%: %.c
	gcc -std=c99 $< -o $@ -lm -lOpenCL
