#ifndef _COLOR_H
#define _COLOR_H
#include <complex.h>
#include "view.h"



typedef struct {
	int (*color)(complex double);	// function pointer to the actual function
	// partial rerender functions
	const char * name;			// read-only at runtime
	const char * explanation;			// read-only at runtime
	void (*changeColor1)(unsigned long);	// function pointer to the actual function
	void (*changeColor2)(unsigned long);	// function pointer to the actual function
	void (*changeColorRandom)(unsigned long);	// function pointer to the actual function
	void (*printColorInfo)(void);
} color_t;

#define COLOR_COUNT 15
#define FLUID_COLOR_COUNT 6

extern float fluidColors[FLUID_COLOR_COUNT]; 
extern color_t colorArray[];
extern const int colorCount;


/*
extern int COLORS[COLOR_COUNT];

extern int fColorIndex;
*/

void doNothing(unsigned long t);
void doNothing2(void);

int colorTest(complex double z);

/* color1 */
int coloring1(complex double z);

/* color2 */
int coloring2(complex double z);
void chng1Color2(unsigned long times);
void chng2Color2(unsigned long times);
void rndColor2(unsigned long times);

/* color3 */
int coloring3(complex double z);
void chng1Color3(unsigned long times);
void chng2Color3(unsigned long times);
void rndColor3(unsigned long times);
void prntColor3(void);


int colorwheel(complex double z);

#endif /* _COLOR_H */
