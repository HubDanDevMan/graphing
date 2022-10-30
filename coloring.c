#include <math.h>
#include <complex.h>
#include "view.h"
#include "coloring.h"

int COLORS[COLOR_COUNT] = {
	0x00000001,
	0x00000100,
	0x00000101,
	0x00010000,
	0x00010001,
	0x00010100,
	0x00010101,
	0x01000000,
	0x01000001,
	0x01000100,
	0x01000101,
	0x01010000,
	0x01010001,
	0x01010100,
	0x01010101
	/*0x0000ff01,  // PINK GREEN RED
	0x0001ff01,
	0x0001ff00,
	0x0101ff00,
	0x0100ff00,
	0x0100ff01*/
	/*0x00ff0001,  // PINK GREEN RED
	0x00ff0101,
	0x00ff0100,
	0x01ff0100,
	0x01ff0000,
	0x01ff0001*/
	/*0x000001ff,  // PINK CYAN BLUE WHITE 
	0x000101ff,
	0x000100ff,
	0x010100ff,
	0x010000ff,
	0x010001ff*/
	/*0xff000001, // RED BLUE CYAN GREEN
	0xff000101,
	0xff000100,
	0xff010100,
	0xff010000,
	0xff010001*/
};

int coloring(complex double z)
{
	double p = compvar(z);
	double s = p/exp(view.spread);
	int x = ((int)(255.0 + (-255.0/(1.0+ s*s)))) << 8;
	return x;
}

int coloring2(complex double z)
{
	double p = compvar(z)+view.shift;
	double x = (p)/exp(view.spread);
	x = fabs((atan(x) / M_PI)*(1.0 - 1.0/(1.0+x*x)))*510.0;
	if (p <= 0){
		return (int)x * COLORS[view.color1]; // blue negative
	}
	return (int)x * COLORS[view.color2];	// green positive
	
}

/* under construction */
int colorwheel(complex double z)
{
	// phase to rgb
	double angle = atan2(creal(z), cimag(z));	
	double lightness = carg(z); // find good lightness plotter, log seems aight
	return 0;
}

/* unused */
int colorTest(complex double z)
{
	if (creal(z) < 5.0 && creal(z) > -5.0){
		return 0xff00ff00;
	} else if (cimag(z) < 5.0 && cimag(z) > -5.0){
		return 0xffff0000;
	}
	return 0xff000fff;
}
