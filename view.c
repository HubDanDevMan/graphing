#include <stdio.h>
#include <string.h>
#include <complex.h>
#include "view.h"
#include "controls.h"
#include "function.h"

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
	if (funcArray[funcIndex].defaultView.iterations != 0)  {
		view = funcArray[funcIndex].defaultView;	
		return;
	}

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
	printf(
	"\n{\n"
	"\t\t.centerx = %lf,\n"
	"\t\t.centery = %lf,\n"
	"\t\t.scaling = %lf,\n"
	"\t\t.iterations = %d,\n"
	"\t\t.var1 = %d,\n"
	"\t\t.var2 = %d,\n"
	"\t\t.var3 = %d,\n"
	"\t\t.spread = %lf,\n"
	"\t\t.shift = %lf\n"
	"}\n",
	view.centerx,
	view.centery,
	view.scaling,
	view.iterations,
	view.var1,
	view.var2,
	view.var3,
	view.spread,
	view.shift);
}

/* determines what changed in the view and triggers the most efficient
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
