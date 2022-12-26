#include <stdio.h>
#include <string.h>
#include <complex.h>
#include "view.h"
#include "controls.h"

struct viewstate view = {
	0.0,		// centerx
	0.0, 		// centery
	SCALE_START,
	ITERATIONS,
	1,		// vars
	1,
	1,
	SPREAD_START,
	0.0,		// shift
	DEFAULT_COLOR_INDEX,	// Colorfunc array index
	DEFAULT_COLOR_2
};

void resetView()
{
	view.centerx = 0.0;
	view.centery = 0.0;
	view.spread = SPREAD_START;
	view.scaling = SCALE_START;
	view.shift = 0.0;
	view.iterations = ITERATIONS;
	view.colorIndex = DEFAULT_COLOR_INDEX;
	view.color2 = DEFAULT_COLOR_2;
	view.var1 = 1;
	view.var2 = 1;
	view.var3 = 1;
}

void printView()
{
	printf("Position: %lf %lf\nScaling: %lf\n"
			"Iterations: %d\nSpread: %lf\nShift: %lf\n"
			/*"Color1: %d\nColor2: %d\n"*/
			"Var1: %d\nVar2: %d\nVar3: %d\n",
			view.centerx, view.centery, view.scaling,
			view.iterations, view.spread, view.shift,
			/*view.color1, view.color2,*/
			view.var1, view.var2, view.var3);



}

/*enum viewdiff_t getViewDiffType()*/


/**
 * determines what changed in the view and triggers the most efficient
 * rerender approach. Apply BEFORE undoing the current change
 */
RedoFlag fromViewDiff()
{
	// Calculate offset of 'spread' member in viewstate
	size_t offsetBytes =   (char*) &view.spread - (char*) &view;
	struct viewstate *view2 = &history.snapshot[history.index == 0 ? HISTORY_MAX-1 : history.index-1];
	if (memcmp(&view, &view2, offsetBytes-8) == 0){
		printf("Redrawing\n");
		return REDRAW;
	}
	printf("Recalculating\n");
	return RECALC;
}

double (*compvar)(complex double) = &creal; 		// cimag | creal
int funcIndex = 23;
RedoFlag redoFlag = RECALC;
complex double *numbers;
int * pixels;
bool animation = false;
bool showAxis = true;
bool undoAction = false;
