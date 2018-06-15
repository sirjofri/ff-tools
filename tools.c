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
	case MEMERR:
		fprintf(stderr, "Memory error\n");
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

int
ff_read_content(uint16_t *target, Coords size)
{
	if (target == 0x0)
		return MEMERR;
	if (fread(target, sizeof(uint16_t), size.x*size.y*4, stdin) != size.x*size.y*4)
		return READERR;
	return OK;
}

Rgba
ff_coords(uint16_t *origin, Coords pos)
{
	uint16_t *ptr = ff_coords_ptr(origin, pos);
	Rgba rgba;
	rgba.r = ptr[0];
	rgba.g = ptr[1];
	rgba.b = ptr[2];
	rgba.a = ptr[3];

	return rgba;
}

uint16_t*
ff_coords_ptr(uint16_t *origin, Coords pos)
{
	if (origin == 0x0)
		return 0x0;

	return origin + pos.x*pos.y*sizeof(uint16_t)*4;
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

double
ff_clamp(double value)
{
	return value > 0 ? (value > 1 ? 1.0 : value) : 0;
}
