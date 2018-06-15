#include "tools.h"

void multiply(uint16_t a, uint16_t b, uint16_t *c);

int main(int argc, char **argv)
{
	int ret;
	Coords a_size, b_size;

	uint16_t result;
	uint16_t *a, *b;
	a = 0x0;
	b = 0x0;

	setvbuf(stdout, 0x0, _IOFBF, BUFSIZ);

	if (argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return USERERR;
	}

	if (ff_read_header(&a_size) != OK) {
		fprintf(stderr, "Error: can not read (A)\n");
		return READERR;
	}

	/* Read A file content */
	a = ff_malloc(a_size);
	ret = ff_read_content(a, a_size);
	switch (ret) {
	case MEMERR:
		ff_err(ret);
		return MEMERR;
	case USERERR:
	case READERR:
	case WRITERR:
		ff_err(ret);
		free(a);
		return ret;
	default:
		;
	}

	if (ff_read_header(&b_size) != OK) {
		fprintf(stderr, "Error: can not read (B)\n");
		return READERR;
	}

	if (a_size.x != b_size.x || a_size.y != b_size.y) {
		fprintf(stderr, "%s: different image sizes\n", argv[0]);
		free(a);
		return USERERR;
	}

	/* Read B file content */
	b = ff_malloc(b_size);
	ret = ff_read_content(b, b_size);
	switch (ret) {
	case MEMERR:
		ff_err(ret);
		return MEMERR;
	case USERERR:
	case READERR:
	case WRITERR:
		ff_err(ret);
		free(a);
		free(b);
		return ret;
	default:
		;
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
	fflush(stdout);
	return 0;
}

void
multiply(uint16_t a,        // Source A
         uint16_t b,        // Source B
         uint16_t *c)       // Target C
{
	*c = ((double)a/UINT16_MAX) * ((double)b/UINT16_MAX) * UINT16_MAX;
}
