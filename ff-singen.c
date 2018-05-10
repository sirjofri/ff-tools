#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#define LEN(x) (sizeof (x) / sizeof *(x))

int steps;
uint32_t width, height;

int print_ff_header();
uint16_t get_pixel_value(uint32_t x, uint32_t y);
int print_ff_value(uint16_t value);

int main(int argc, char **argv)
{
	if(argc != 4) {
		fprintf(stderr, "Usage: %s width height steps\n", argv[0]);
		return 1;
	}

	sscanf(argv[1], "%"SCNu32, &width);
	sscanf(argv[2], "%"SCNu32, &height);
	steps = atoi(argv[3]);

	if(width == 0 || height == 0) {
		fprintf(stderr, "Error: width and height must be >0\n");
		return 1;
	}

	if(steps < 1) {
		fprintf(stderr, "Error: steps must be >0\n");
		return 1;
	}

	if(print_ff_header() != 0) {
		fprintf(stderr, "Error: can not write\n");
		return 3;
	}

	for(uint32_t y=0; y<height; y++)
		for(uint32_t x=0; x<width; x++) {
			uint16_t value = get_pixel_value(x, y);
			if(print_ff_value(value) != 0) {
				fprintf(stderr, "Error: can not write\n");
				return 3;
			}
		}
	return 0;
}

int
print_ff_header()
{
	printf("farbfeld");
	uint32_t wh[2];
	wh[0] = ntohl(width);
	wh[1] = ntohl(height);
	if(fwrite(wh, sizeof(*wh), LEN(wh), stdout) != LEN(wh))
		return 1;
	return 0;
}

int
print_ff_value(uint16_t value)
{
	uint16_t rgba[4];
	rgba[0] = value;
	rgba[1] = value;
	rgba[2] = value;
	rgba[3] = 0xffff;
	if(fwrite(rgba, sizeof(*rgba), LEN(rgba), stdout) != LEN(rgba))
		return 1;
	return 0;
}

uint16_t
get_pixel_value(uint32_t x, uint32_t y)
{
	uint16_t value = (sin(x*M_PI/width*2*steps)/2.+0.5)*0xffff;
	return ntohs(value);
}
