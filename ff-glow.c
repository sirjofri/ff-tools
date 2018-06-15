#include "tools.h"

Coords size;

uint16_t get_pixel_value(uint32_t x, uint32_t y);

int main(int argc, char **argv)
{
	int ret;

	setvbuf(stdout, 0x0, _IOFBF, BUFSIZ);

	if(argc != 2) {
		fprintf(stderr, "Usage: %s size\n", argv[0]);
		return USERERR;
	}

	sscanf(argv[1], "%"SCNu32, &size.x);
	size.y = size.x;

	if(size.x == 0) {
		fprintf(stderr, "Error: size must be >0\n");
		return USERERR;
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
	
	fflush(stdout);
	return 0;
}

uint16_t
get_pixel_value(uint32_t x, uint32_t y)
{
	double mul = ff_clamp(1-(pow((double)x/size.x*2-1, 2) + pow((double)y/size.y*2-1, 2)));
	uint16_t value = mul * UINT16_MAX;
	return value;
}
