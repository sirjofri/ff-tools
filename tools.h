/****************************************************************************\
 *
 *    ff-tools tools
 *
 *    Author: sirjofri
 *
\***************************************************************************/

#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#define LEN(x) (sizeof (x) / sizeof *(x))

typedef struct Coords {
	uint32_t x;
	uint32_t y;
} Coords;

#define set_c(c,_x,_y); (c).x=(_x);(c).y=(_y);

typedef struct Rgba {
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t a;
} Rgba;

int ff_print_header(Coords size);
int ff_print_rgba(Rgba rgba);
int ff_print_value(uint16_t *value);
int ff_print_bw(uint16_t *bw_value);

#define OK      0
#define USERERR 1
#define READERR 2
#define WRITERR 3
#define MEMERR  4

int ff_err_msg(int error, char *message);
int ff_err(int error);

int ff_read_header(Coords *size);
#define ff_malloc(size) (uint16_t *)malloc((size).x * (size).y * sizeof(uint16_t)*4);
int ff_read_content(uint16_t *target, Coords size);

#define FOR_X_Y(width, height, body) for(uint32_t y=0; y<height; y++) for(uint32_t x=0; x<width; x++) {body}

Rgba ff_coords(uint16_t *origin, Coords pos);
uint16_t *ff_coords_ptr(uint16_t *origin, Coords pos);
Coords ff_get_rel_coords(Coords pos,
                         Coords size,
                         Coords rel_pos);
