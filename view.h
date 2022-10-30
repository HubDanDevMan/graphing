#ifndef _VIEW_H
#define _VIEW_H
#include <stdbool.h>
#include <complex.h>

// large spread implies only sparse highlight and only large values
#define SPREAD_START 3.2
// small scale means huge zoom
#define SCALE_START 0.01
//  must be more than two since starts at 1
#define ITERATIONS 12 /* depends on type of function, some converge faster than others */
#define WIN_WIDTH (1260)
#define WIN_HEIGHT (820)
#define COLOR_MODE_CNT 5 /* col2 phase-linear arg-linear phase-s arg-s NIMP */ 
#define FACTOR 20.0
#define DEFAULT_COLOR_1 4
#define DEFAULT_COLOR_2 8
typedef enum {
	NOTHING	= 0b0000,
	REDRAW	= 0b0001,
	RECALC	= 0b0011,
	INCCALC	= 0b0101, /* Iteration adjustements */
	DECCALC	= 0b1001 /* Iteration adjustements */
} RedoFlag;

#define redrawRequired() (redoFlag == REDRAW)
#define recalcRequired() (redoFlag == RECALC)
#define inccalcRequired() (redoFlag == INCCALC)
#define deccalcRequired() (redoFlag == DECCALC)

struct viewstate {
	double centerx;
	double centery;
	double scaling;
	int iterations;
	int var1;
	int var2;
	int var3;
	/* ^ ALL ABOVE will set redoFlag to recalc */
	/* v ALL BELOW will set redoFlag to redraw */
	double spread; /* <== MUST ALWAYS IMMEDIATELY FOLLOW THE 'RECALC' MEMBERS*/
	double shift;
	int color1;
	int color2;
};
/*
typedef enum {
	
} viewdiff_t;
*/
void printView(void);
void resetView(void);
RedoFlag fromViewDiff();
extern struct viewstate view;
extern int *pixels;
extern complex double *numbers;
extern RedoFlag redoFlag;
extern bool showAxis;
extern bool undoAction;
#endif /* _VIEW_H */
