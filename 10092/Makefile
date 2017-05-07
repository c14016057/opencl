all: vectorAdd

%: %.c
	gcc -std=c99 $< -o $@ -lm -lOpenCL
clean:
	rm getPlatformID getPlatformInfo getDeviceID getDeviceInfo vectorAdd
