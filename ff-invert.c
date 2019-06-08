#include "tools.h"

void invert(uint16_t *value);

int main(int argc, char **argv)
{
	int ret;
	Coords size;

	uint16_t result;

	setvbuf(stdout, 0x0, _IOFBF, BUFSIZ);

	if (ff_read_header(&size) != OK) {
		fprintf(stderr, "Error: can not read input.");
	}

	ret = ff_print_header(size);
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	for (uint32_t x = 0; x < size.y*size.x*4; x += 4) {
		for (uint32_t b = 0; b < 4; b++) {
			if (fread(&result, sizeof(uint16_t), 1, stdin) != 1) {
				fprintf(stderr, "Error: can not read input image\n");
				return READERR;
			}
			if (b != 3) // if not alpha
				invert(&result);
			ret = ff_print_value(&result);
			if (ret != 0) {
				ff_err(ret);
				return ret;
			}
		}
	}

	fflush(stdout);
	return 0;
}

void
invert(uint16_t *value)
{
	*value = UINT16_MAX * (1.0-((double)ntohs(*value)/UINT16_MAX));
}
