#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include "view.h"
#include "coloring.h"


/**
 * This is the interface as seen by 
 */
color_t colorArray[] = {
	{&coloring1,	"Single Color",	"One color, ignoring sign (e.g. -4 == +4)", &doNothing, &doNothing, &doNothing, &doNothing2},
	{&coloring2,	"Bicolor Index","Array lookup using 1-1/(1+x^2) scaled",  &chng1Color2, &chng1Color2, &rndColor2, &doNothing2},
	{&coloring3,	"4-Color fluid","Array lookup using 1-1/(1+x^2) scaled", &chng1Color3, &chng2Color3, &rndColor3, &prntColor3 },

};
const int colorCount = sizeof(colorArray) / sizeof(colorArray[0]);


// Static function definitions
static int mergeColor(double brightness);
static int mergeColor2(double brightness);

// Data structures used globally
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
};
int color1;
int color2;

/* 2x RGB values = 6 Values ranging from 0 - 255 */
float fluidColors[FLUID_COLOR_COUNT] = {0.0f, 255.0f, 0.0f, 0.0f, 0.0f, 0.0f};
int fColorIndex = 0;


/* placeholder */
void doNothing(unsigned long t)
{
	printf("doing nothing...\n");
}
/* placeholder */
void doNothing2(void)
{
	printf("doing nothing...\n");
}


/* COLOR 1 Functions */
/**
 * Single color black and white, ignoring sign
 */
int coloring1(complex double z)
{
	double p = compvar(z);
	double s = p/exp(view.spread);
	int x = ((int)(255.0 + (-255.0/(1.0+ s*s)))) << 8;
	return x;
}

/* COLOR 2 Functions */

int coloring2(complex double z)
{
	double p = compvar(z)+view.shift;
	double x = (p)/exp(view.spread);
	x = fabs((atan(x) / M_PI)*(1.0 - 1.0/(1.0+x*x)))*510.0;
	if (p <= 0){
		return (int)x * COLORS[color1]; // blue negative
	}
	return (int)x * COLORS[color2];	// green positive
}

void chng1Color2(unsigned long times)
{
	color1 = (color1 + times) % COLOR_COUNT;
}

void chng2Color2(unsigned long times)
{
	color2 = (color2 + times) % COLOR_COUNT;
}

void rndColor2(unsigned long times)
{
	color1 = rand() % COLOR_COUNT;
	color2 = rand() % COLOR_COUNT;
}

/* COLOR 3 Functions */

int coloring3(complex double z)
{
	// produce a value between 0 and 1
	double p = compvar(z)+view.shift;
	double x = (p)/exp(view.spread);
	x = fabs((atan(x) / M_PI)*(1.0 - 1.0/(1.0+x*x))); // codomain 0.0,1.0
	if (p <= 0){
		return mergeColor(x); // blue negative
	}
	return mergeColor2(x);	// green positive
}


void chng1Color3(unsigned long times)
{
	fluidColors[fColorIndex] = fmodf(fluidColors[fColorIndex] + times, 255.5);
}

void chng2Color3(unsigned long times)
{
	fColorIndex = (fColorIndex + times) % FLUID_COLOR_COUNT;
}

void rndColor3()
{
	for (int i = 0; i < FLUID_COLOR_COUNT; i++) {
		fluidColors[i] = fmodf((float) rand(), 255.5);
	}

}

void prntColor3()
{
	static const char * colorStrings[FLUID_COLOR_COUNT] = {
		"red", "green", "blue",
		"red", "green", "blue" 
	};
	for (int i = 0; i < FLUID_COLOR_COUNT; i++) {
		printf("%s%d: %.2f\n", colorStrings[i], i/(FLUID_COLOR_COUNT/2), fluidColors[i]);
	}
}

/**
 * scales a color's rgb values
 */
static int mergeColor(double brightness)
{
	// little endian
	return (unsigned char)(fluidColors[0] * brightness) << 24
		| (unsigned char)(fluidColors[1] * brightness) << 16
		| (unsigned char)(fluidColors[2] * brightness) << 8
		| 0xff;
}

/**
 * scales the second color's rgb values
 */
static int mergeColor2(double brightness)
{
	// little endian
	return (unsigned char)(fluidColors[3] * brightness) << 24
		| (unsigned char)(fluidColors[4] * brightness) << 16
		| (unsigned char)(fluidColors[5] * brightness) << 8
		| 0xff;
}


/* COLOR 4 Functions */

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
