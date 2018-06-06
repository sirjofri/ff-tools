#include "tools.h"

int steps;
uint32_t width, height;

uint16_t get_pixel_value(uint32_t x, uint32_t y);

int main(int argc, char **argv)
{
	int ret;

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

	ret = ff_print_header(&width, &height);
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	FOR_X_Y(width, height,
		uint16_t value = get_pixel_value(x, y);
		ret = ff_print_bw(&value);
		if (ret != 0) {
			ff_err(ret);
			return ret;
		}
	)

	return 0;
}

uint16_t
get_pixel_value(uint32_t x, uint32_t y)
{
	uint16_t value = (cos(x*M_PI/width*2*steps)/2.+0.5)*UINT16_MAX;
	return value;
}
