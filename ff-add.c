#include "tools.h"

void add(Rgba a, Rgba b, Rgba *c);

int main(int argc, char **argv)
{
	int ret;
	Coords a_size, b_size;

	Rgba result;

	Rgba *a, *b;
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
	a = malloc(a_size.x * a_size.y * sizeof(struct Rgba));
	ret = ff_read_rgba_content(a, a_size);
	switch (ret) {
	case MEMERR:
		free(a);
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
		free(a);
		return READERR;
	}

	if (a_size.x != b_size.x || a_size.y != b_size.y) {
		fprintf(stderr, "%s: different image sizes\n", argv[0]);
		free(a);
		return USERERR;
	}

	/* Read B file content */
	b = malloc(b_size.x * b_size.y * sizeof(struct Rgba));
	ret = ff_read_rgba_content(b, b_size);
	switch (ret) {
	case MEMERR:
		ff_err(ret);
		free(a);
		free(b);
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

	FOR_POS(a_size,
		add(a[pos], b[pos], &result);
		ret = ff_print_rgba(result);
		if (ff_err(ret) != 0) {
			free(a);
			free(b);
			return ret;
		}
	)

	free(a);
	free(b);
	fflush(stdout);
	return 0;
}

void
add(Rgba a,        // Source A
    Rgba b,        // Source B
    Rgba *c)       // Target C
{
	RgbaD ad = ff_i2d(a);
	RgbaD bd = ff_i2d(b);
	double r_val = ad.r + bd.r;
	double g_val = ad.g + bd.g;
	double b_val = ad.b + bd.b;
	double a_val = ad.a + bd.a;
	c->r = ff_clamp(r_val) * UINT16_MAX;
	c->g = ff_clamp(g_val) * UINT16_MAX;
	c->b = ff_clamp(b_val) * UINT16_MAX;
	c->a = ff_clamp(a_val) * UINT16_MAX;
}
