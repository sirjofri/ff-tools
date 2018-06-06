#include "tools.h"

int print_shuffled(uint16_t *pixel);
int map_rgba[4];

int main(int argc, char **argv)
{
	int ret;

	uint32_t hdr[4],
	         width,
	         height;
	uint16_t in_pixel[4];


	if (argc != 5) {
		fprintf(stderr, "Usage: %s r g b a\n", argv[0]);
		return USERERR;
	}

	for (int i=0; i<4; i++) {
		switch (argv[i+1][0]) {
		case 'r':
			map_rgba[i] = 0;
			break;
		case 'g':
			map_rgba[i] = 1;
			break;
		case 'b':
			map_rgba[i] = 2;
			break;
		case 'a':
			map_rgba[i] = 3;
			break;
		default:
			fprintf(stderr, "Error: Parameter %c not ( r | g | b | a )\n", argv[i+1][0]);
			return USERERR;
		}
	}

	/* Read A file header */
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
	if (ret != 0) {
		ff_err(ret);
		return ret;
	}

	FOR_X_Y(width, height,
		if (fread(in_pixel, sizeof(*in_pixel), LEN(in_pixel), stdin) != LEN(in_pixel)) {
			fprintf(stderr, "Error: can not read input image\n");
			return READERR;
		}
		ret = print_shuffled(in_pixel);
		if (ret != 0) {
			ff_err(ret);
			return ret;
		}
	)
	return 0;
}

int
print_shuffled(uint16_t *pixel)
{
	uint16_t shuffled[4];
	shuffled[0] = pixel[map_rgba[0]];
	shuffled[1] = pixel[map_rgba[1]];
	shuffled[2] = pixel[map_rgba[2]];
	shuffled[3] = pixel[map_rgba[3]];

	return ff_print_rgba(shuffled);
}
