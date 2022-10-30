#ifndef _COLOR_H
#define _COLOR_H
#include <complex.h>

#define COLOR_COUNT 15

extern int COLORS[COLOR_COUNT];
/*
typedef enum {
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA
} color_t;
*/
extern double (*compvar)(complex double); // cimag | creal

int colorTest(complex double z);

int coloring(complex double z);

int coloring2(complex double z);

int colorwheel(complex double z);

#endif /* _COLOR_H */
