#include "tools.h"

uint32_t width, height;
Rgba rgba;
double d_rgba[4];

void calculate_rgba();

int main(int argc, char **argv)
{
	int ret;
	Coords size;

	if (argc < 6 || argc > 7) {
		fprintf(stderr, "Usage: %s width height red green blue [alpha]\n", argv[0]);
		return USERERR;
	}

	sscanf(argv[1], "%"SCNu32, &size.x);
	sscanf(argv[2], "%"SCNu32, &size.y);
	sscanf(argv[3], "%lf", &d_rgba[0]);
	sscanf(argv[4], "%lf", &d_rgba[1]);
	sscanf(argv[5], "%lf", &d_rgba[2]);

	if (argc == 7) {
		sscanf(argv[6], "%lf", &d_rgba[3]);
	} else {
		d_rgba[3] = 1.0;
	}

	calculate_rgba();

	if (size.x == 0 || size.y == 0) {
		fprintf(stderr, "Error: width and height must be >0\n");
		return USERERR;
	}

	ret = ff_print_header(size);
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	FOR_X_Y(size.x, size.y,
		ret = ff_print_rgba(rgba);
		if (ret != 0) {
			ff_err(ret);
			return ret;
		}
	)
	return 0;
}

void
calculate_rgba()
{
	rgba.r = d_rgba[0] * UINT16_MAX;
	rgba.g = d_rgba[1] * UINT16_MAX;
	rgba.b = d_rgba[2] * UINT16_MAX;
	rgba.a = d_rgba[3] * UINT16_MAX;
}
