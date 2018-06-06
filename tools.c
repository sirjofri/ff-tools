#include "tools.h"

int
ff_print_header(uint32_t *width, uint32_t *height)
{
	printf("farbfeld");
	uint32_t wh[2];
	wh[0] = ntohl(*width);
	wh[1] = ntohl(*height);

	if (fwrite(wh, sizeof(*wh), LEN(wh), stdout) != LEN(wh))
		return 3;
	return 0;
}

int
ff_print_rgba(uint16_t *rgba)
{
	if (fwrite(rgba, sizeof(uint16_t), 4, stdout) != 4)
		return 3;
	return 0;
}

int
ff_print_value(uint16_t *value)
{
	uint16_t val = htons(*value);
	if (fwrite(&val, 1, sizeof(uint16_t), stdout) != sizeof(uint16_t))
		return 3;
	return 0;
}

int
ff_print_bw(uint16_t *value)
{
	uint16_t bw[4];
	bw[0] = ntohs(*value);
	bw[1] = ntohs(*value);
	bw[2] = ntohs(*value);
	bw[3] = 0xffff;
	if (fwrite(bw, sizeof(*bw), LEN(bw), stdout) != LEN(bw))
		return 3;
	return 0;
}

int
ff_err_msg(int error, char *msg)
{
	switch (error) {
	case USERERR:
		fprintf(stderr, "Usage: %s\n", msg);
		return 1;
	case READERR:
		fprintf(stderr, "Read: %s\n", msg);
		return 1;
	case WRITERR:
		fprintf(stderr, "Write: %s\n", msg);
		return 1;
	default:
		return 0;
	}
}

int
ff_err(int error)
{
	switch (error) {
	case USERERR:
		fprintf(stderr, "Usage error\n");
		return 1;
	case READERR:
		fprintf(stderr, "Read error\n");
		return 1;
	case WRITERR:
		fprintf(stderr, "Write error\n");
		return 1;
	default:
		return 0;
	}
}
