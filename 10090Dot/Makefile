all: vectorDot

%: %.c
	gcc -std=c99 $< -o $@ -lm -lOpenCL
clean:
	rm vectorDot
