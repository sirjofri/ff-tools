#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

void gamma(uint16_t in, uint16_t *out);
int print_p(uint16_t *c);

double cor = 2.2;

int main(int argc, char **argv)
{
	uint32_t a_hdr[4],
	         a_width,
	         a_height;
	uint16_t result;
	uint16_t *a;

	double in_cor;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return 1;
	}

	if (argc == 2) {
		sscanf(argv[1], "%lf", &in_cor);
		if (in_cor <= 0) {
			fprintf(stderr, "Error: Gamma must be > 0 (You tried %lf).", in_cor);
			return 1;
		}
		cor = in_cor;
	}

	/* Read A file header */
	if (fread(a_hdr, sizeof(*a_hdr), LEN(a_hdr), stdin) != LEN(a_hdr)) {
		fprintf(stderr, "Error: can not read input image\n");
		return 2;
	}
	if (memcmp("farbfeld", a_hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value\n", argv[0]);
		return 1;
	}

	a_width = ntohl(a_hdr[2]);
	a_height = ntohl(a_hdr[3]);

	/* Read A file content */
	a = (uint16_t *) malloc(a_width * a_height * sizeof(uint16_t) * 4);
	if (fread(a, sizeof(uint16_t), a_width*a_height*4, stdin) != a_width*a_height*4) {
		fprintf(stderr, "Error: can not read input image\n");
		return 2;
	}

	/* Write header */
	if (fwrite(a_hdr, sizeof(*a_hdr), LEN(a_hdr), stdout) != 4) {
		fprintf(stderr, "%s: Write error\n", argv[0]);
		return 3;
	}

	/* Calculate and output data */
	for(uint32_t x=0; x<a_height*a_width*2*sizeof(uint16_t); x++) {
		gamma(ntohs(*(a+x)), &result);
		if (print_p(&result) != 0) {
			fprintf(stderr, "%s: Write error\n", argv[0]);
			return 3;
		}
	}
	return 0;
}

int
print_p(uint16_t *c)
{
	uint16_t buf = htons(*c);
	if (fwrite(&buf, 1, 2, stdout) != 2)
		return 1;
	return 0;
}

void
gamma(uint16_t in,
      uint16_t *out)
{
	*out = pow(((double)in/UINT16_MAX), 1.0/cor) * UINT16_MAX;
}
