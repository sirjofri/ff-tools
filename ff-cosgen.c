#include "tools.h"

int steps;
Coords size;

uint16_t get_pixel_value(uint32_t x, uint32_t y);

int main(int argc, char **argv)
{
	int ret;

	if(argc != 4) {
		fprintf(stderr, "Usage: %s width height steps\n", argv[0]);
		return 1;
	}

	sscanf(argv[1], "%"SCNu32, &size.x);
	sscanf(argv[2], "%"SCNu32, &size.y);
	steps = atoi(argv[3]);

	if(size.x == 0 || size.y == 0) {
		fprintf(stderr, "Error: width and height must be >0\n");
		return 1;
	}

	if(steps < 1) {
		fprintf(stderr, "Error: steps must be >0\n");
		return 1;
	}

	ret = ff_print_header(size);
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	FOR_X_Y(size.x, size.y,
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
	uint16_t value = (cos(x*M_PI/size.x*2*steps)/2.+0.5)*UINT16_MAX;
	return value;
}
