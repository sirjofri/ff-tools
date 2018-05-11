#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

void multiply(uint16_t a, uint16_t b, uint16_t *c);
int print_p(uint16_t *c);

int main(int argc, char **argv)
{
	uint32_t a_hdr[4], b_hdr[4],
	         a_width,  b_width,
	         a_height, b_height;
	uint16_t result;
	uint16_t *a, *b;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return 1;
	}

	/* Read A file header */
	if (fread(a_hdr, sizeof(*a_hdr), LEN(a_hdr), stdin) != LEN(a_hdr)) {
		fprintf(stderr, "Error: can not read (A)\n");
		return 2;
	}
	if (memcmp("farbfeld", a_hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value (A)\n", argv[0]);
		return 1;
	}

	a_width = ntohl(a_hdr[2]);
	a_height = ntohl(a_hdr[3]);

	/* Read A file content */
	a = (uint16_t *) malloc(a_width * a_height * sizeof(uint16_t) * 4);
	if (fread(a, sizeof(uint16_t), a_width*a_height*4, stdin) != a_width*a_height*4) {
		fprintf(stderr, "Error: can not read file (A)\n");
		return 2;
	}

	/* Read B file header */
	if (fread(b_hdr, sizeof(*b_hdr), LEN(b_hdr), stdin) != LEN(b_hdr)) {
		fprintf(stderr, "Error: can not read (B)\n");
		return 2;
	}

	if (memcmp("farbfeld", b_hdr, sizeof("farbfeld") - 1)) {
		fprintf(stderr, "%s: invalid magic value (B)\n", argv[0]);
		return 1;
	}

	b_width = ntohl(b_hdr[2]);
	b_height = ntohl(b_hdr[3]);

	if (a_width != b_width || a_height != a_width) {
		fprintf(stderr, "%s: different image sizes\n", argv[0]);
		return 1;
	}

	/* Read B file content */
	b = (uint16_t *) malloc(b_width * b_height * sizeof(uint16_t) * 4);
	if (fread(b, sizeof(uint16_t), b_width*b_height*4, stdin) != b_width*b_height*4) {
		fprintf(stderr, "Error: can not read (B)\n");
		return 2;
	}

	/* Write header */
	if (fwrite(a_hdr, sizeof(*a_hdr), LEN(a_hdr), stdout) != 4) {
		fprintf(stderr, "%s: Write error\n", argv[0]);
		return 3;
	}

	/* Calculate and output data */
	for(uint32_t x=0; x<a_height*a_width*2*sizeof(uint16_t); x++) {
		multiply(ntohs(*(a+x)), ntohs(*(b+x)), &result);
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
multiply(uint16_t a,        // Source A
         uint16_t b,        // Source B
         uint16_t *c)       // Target C
{
	*c = ((double)a/UINT16_MAX) * ((double)b/UINT16_MAX) * UINT16_MAX;
}
