#include "tools.h"

double cor = 2.2;
void gamma(uint16_t in, uint16_t *out);

int main(int argc, char **argv)
{
	int ret;
	Coords size;

	uint16_t result;
	uint16_t in;

	double in_cor;

	setvbuf(stdout, 0x0, _IOFBF, BUFSIZ);

	if (argc > 2) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return USERERR;
	}

	if (argc == 2) {
		sscanf(argv[1], "%lf", &in_cor);
		if (in_cor <= 0) {
			fprintf(stderr, "Error: Gamma must be > 0 (You tried %lf).", in_cor);
			return USERERR;
		}
		cor = in_cor;
	}

	if (ff_read_header(&size) != OK) {
		fprintf(stderr, "Error: can not read input.");
	}

	ret = ff_print_header(size);
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	for(uint32_t x=0; x<size.y*size.x*4; x++) {
		if (fread(&in, sizeof(uint16_t), 1, stdin) != 1) {
			fprintf(stderr, "Error: can not read input image\n");
			return READERR;
		}
		gamma(in, &result);
		ret = ff_print_value(&result);
		if (ret != 0) {
			ff_err(ret);
			return ret;
		}
	}

	fflush(stdout);
	return 0;
}

void
gamma(uint16_t in,
      uint16_t *out)
{
	*out = pow(((double)ntohs(in)/UINT16_MAX), 1.0/cor) * UINT16_MAX;
}
