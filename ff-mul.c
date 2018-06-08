#include "tools.h"

void multiply(uint16_t a, uint16_t b, uint16_t *c);

int main(int argc, char **argv)
{
	int ret;
	Coords a_size, b_size;

	uint32_t a_hdr[4], b_hdr[4];
	uint16_t result;
	uint16_t *a, *b;
	a = 0x0;
	b = 0x0;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return USERERR;
	}

	/* Read A file header */
	if (fread(a_hdr, sizeof(*a_hdr), LEN(a_hdr), stdin) != LEN(a_hdr)) {
		fprintf(stderr, "Error: can not read (A)\n");
		return READERR;
	}
	if (memcmp("farbfeld", a_hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value (A)\n", argv[0]);
		return USERERR;
	}

	set_c(a_size, ntohl(a_hdr[2]), ntohl(a_hdr[3]));

	/* Read A file content */
	a = (uint16_t *) malloc(a_size.x * a_size.y * sizeof(uint16_t) * 4);
	if (a == 0x0) {
		fprintf(stderr, "Error: can not allocate memory\n");
		return 4;
	}
	if (fread(a, sizeof(uint16_t), a_size.x*a_size.y*4, stdin) != a_size.x*a_size.y*4) {
		fprintf(stderr, "Error: can not read file (A)\n");
		free(a);
		return READERR;
	}

	/* Read B file header */
	if (fread(b_hdr, sizeof(*b_hdr), LEN(b_hdr), stdin) != LEN(b_hdr)) {
		fprintf(stderr, "Error: can not read (B)\n");
		free(a);
		return READERR;
	}

	if (memcmp("farbfeld", b_hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value (B)\n", argv[0]);
		free(a);
		return USERERR;
	}

	set_c(b_size, ntohl(b_hdr[2]), ntohl(b_hdr[3]));

	if (a_size.x != b_size.x || a_size.y != b_size.y) {
		fprintf(stderr, "%s: different image sizes\n", argv[0]);
		free(a);
		return USERERR;
	}

	/* Read B file content */
	b = (uint16_t *) malloc(b_size.x * b_size.y * sizeof(uint16_t) * 4);
	if (b == 0x0) {
		fprintf(stderr, "Error: can not allocate memory\n");
		return 4;
	}
	if (fread(b, sizeof(uint16_t), b_size.x*b_size.y*4, stdin) != b_size.x*b_size.y*4) {
		fprintf(stderr, "Error: can not read (B)\n");
		free(a);
		free(b);
		return READERR;
	}

	ret = ff_print_header(a_size);
	if (ff_err(ret) != 0) {
		free(a);
		free(b);
		return ret;
	}

	/* Calculate and output data */
	for(uint32_t x=0; x<a_size.y*a_size.x*2*sizeof(uint16_t); x++) {
		multiply(ntohs(*(a+x)), ntohs(*(b+x)), &result);
		ret = ff_print_value(&result);
		if (ff_err(ret) != 0) {
			free(a);
			free(b);
			return ret;
		}
	}
	free(a);
	free(b);
	return 0;
}

void
multiply(uint16_t a,        // Source A
         uint16_t b,        // Source B
         uint16_t *c)       // Target C
{
	*c = ((double)a/UINT16_MAX) * ((double)b/UINT16_MAX) * UINT16_MAX;
}
