#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

uint32_t width, height;
uint16_t rgba[4];
double d_rgba[4];

int print_ff_header();
int print_ff_rgba();
void calculate_rgba();

int main(int argc, char **argv)
{
	if (argc < 6 || argc > 7) {
		fprintf(stderr, "Usage: %s width height red green blue [alpha]\n", argv[0]);
		return 1;
	}

	sscanf(argv[1], "%"SCNu32, &width);
	sscanf(argv[2], "%"SCNu32, &height);
	sscanf(argv[3], "%lf", &d_rgba[0]);
	sscanf(argv[4], "%lf", &d_rgba[1]);
	sscanf(argv[5], "%lf", &d_rgba[2]);

	if (argc == 7) {
		sscanf(argv[6], "%lf", &d_rgba[3]);
	} else {
		d_rgba[3] = 1.0;
	}

	calculate_rgba();

	if (width == 0 || height == 0) {
		fprintf(stderr, "Error: width and height must be >0\n");
		return 1;
	}

	if(print_ff_header() != 0) {
		fprintf(stderr, "Error: can not write\n");
		return 3;
	}

	for(uint32_t y=0; y<height; y++)
		for(uint32_t x=0; x<width; x++) {
			if(print_ff_rgba() != 0) {
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

void
calculate_rgba()
{
	rgba[0] = d_rgba[0] * UINT16_MAX;
	rgba[1] = d_rgba[1] * UINT16_MAX;
	rgba[2] = d_rgba[2] * UINT16_MAX;
	rgba[3] = d_rgba[3] * UINT16_MAX;
}

int
print_ff_rgba()
{
	if (fwrite(rgba, sizeof(*rgba), LEN(rgba), stdout) != LEN(rgba))
		return 1;
	return 0;
}
