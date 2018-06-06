#include "tools.h"

double cor = 2.2;
void gamma(uint16_t in, uint16_t *out);

int main(int argc, char **argv)
{
	int ret;
	uint32_t hdr[4],
	         width,
	         height;
	uint16_t result;
	uint16_t in;

	double in_cor;

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

	if (fread(hdr, sizeof(*hdr), LEN(hdr), stdin) != LEN(hdr)) {
		fprintf(stderr, "Error: can not read input image\n");
		return READERR;
	}
	if (memcmp("farbfeld", hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value\n", argv[0]);
		return USERERR;
	}

	width = ntohl(hdr[2]);
	height = ntohl(hdr[3]);

	ret = ff_print_header(&width, &height);
	if (ff_err(ret) != 0)
		return ret;

	for(uint32_t x=0; x<height*width*4; x++) {
		if (fread(&in, sizeof(uint16_t), 1, stdin) != 1) {
			fprintf(stderr, "Error: can not read input image\n");
			return READERR;
		}
		gamma(in, &result);
		ret = ff_print_value(&result);
		if (ff_err(ret) != 0)
			return ret;
	}
	return 0;
}

void
gamma(uint16_t in,
      uint16_t *out)
{
	*out = pow(((double)in/UINT16_MAX), 1.0/cor) * UINT16_MAX;
}
