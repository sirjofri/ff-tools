#include "tools.h"

uint32_t width, height;
uint16_t rgba[4];
double d_rgba[4];

void calculate_rgba();

int main(int argc, char **argv)
{
	int ret;

	if (argc < 6 || argc > 7) {
		fprintf(stderr, "Usage: %s width height red green blue [alpha]\n", argv[0]);
		return USERERR;
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
		return USERERR;
	}

	ret = ff_print_header(&width, &height);
	if (ff_err(ret) != 0)
		return ret;

	for(uint32_t y=0; y<height; y++)
		for(uint32_t x=0; x<width; x++) {
			ret = ff_print_rgba(rgba);
			if (ff_err(ret) != 0)
				return ret;
		}
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
