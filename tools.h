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

int ff_print_header(uint32_t *width, uint32_t *height);
int ff_print_rgba(uint16_t *rgba);
int ff_print_value(uint16_t *value);
int ff_print_bw(uint16_t *bw_value);

#define USERERR 1
#define READERR 2
#define WRITERR 3

int ff_err_msg(int error, char *message);
int ff_err(int error);
