#include "tools.h"

/*void
set_c(Coords c, uint32_t x, uint32_t y)
{
	c.x = x;
	c.y = y;
}*/

int
ff_print_header(Coords size)
{
	printf("farbfeld");
	uint32_t wh[2];
	wh[0] = ntohl(size.x);
	wh[1] = ntohl(size.y);

	if (fwrite(wh, sizeof(*wh), LEN(wh), stdout) != LEN(wh))
		return 3;
	return 0;
}

int
ff_print_rgba(Rgba rgba)
{
	uint16_t r[4];
	r[0] = rgba.r;
	r[1] = rgba.g;
	r[2] = rgba.b;
	r[3] = rgba.a;

	if (fwrite(r, sizeof(uint16_t), 4, stdout) != 4)
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

int
ff_read_header(Coords *size)
{
	uint32_t hdr[4];
	if (fread(hdr, sizeof(*hdr), LEN(hdr), stdin) != LEN(hdr))
		return READERR;
	if (memcmp("farbfeld", hdr, sizeof("farbfeld") - 1))
		return USERERR;

	set_c(*size, ntohl(hdr[2]), ntohl(hdr[3]));
	return OK;
}

Coords
ff_get_rel_coords(Coords pos,
                  Coords size,
                  Coords rel_pos)
{
	Coords c;

	if (pos.x%size.x+rel_pos.x < 0)
		c.x = (pos.x+size.x+rel_pos.x)%size.x;
	else
		c.x = (pos.x+rel_pos.x)%size.x;

	if (pos.y%size.y+rel_pos.y < 0)
		c.y = (pos.y+size.y+rel_pos.y)%size.y;
	else
		c.y = (pos.y+rel_pos.y)%size.y;

	return c;
}
