#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "controls.h"
#include "function.h"
#include "coloring.h"
#include "graphingMain.h"
#include "bmp.h"
#include "view.h"
#include "debug.h"
#include "animations.h"

extern SDL_Window * window;
float oscAmplifier = 0.0f;
/* duration per frame/cycle(*5) in ms */
int animationDuration = 50;
/* number of frames/cycles */
int animationCount = 5;			// ADD CONTROLS



viewhistory_t history;
cmdstack_t stack;

void displayContent()
{
	for (int i = 0; i < stack.top; i++){
		printf("%c ", stack.keystack[i].sym);
	}
	printf("\n");
}


/* SDL_Keymod to mod lookup table */
static inline char toLocalMod(short mod)
{
	if (mod & KMOD_SHIFT && mod & KMOD_CTRL)
		return 3; /* Control + Shift lookup table */
	else if (mod & KMOD_CTRL)
		return 2; /* Control lookup table */
	else if (mod & KMOD_SHIFT)
		return 1; /* Shift lookup table */
	return 0; /* Default lookup table */
}

void handleKeyEvent(SDL_KeyboardEvent kbe)
{
	DEBUG("Handling key event...");
	cmdKey_t key; 
	key.sym = kbe.keysym.sym;
	key.mod = toLocalMod(kbe.keysym.mod);
	stack.isChanged = false;
	switch (kbe.keysym.sym){
		case 'i':
		case 'd':
		case 'b':
		case 'n':
		case 'y':
		case 'u':
		case 'f':
		case 'g':
		case 'q':
		case 'a':
		case 't':
		case 'r':
		case 'e':
		case 'x':
		case 'c':
		case 'p':
		case 'w':
		case 's':
		case 'v':
		case 'z':
		case 'o':
		case 'm':
		case 'h':
		case 'j':
		case 'k':
		case 'l':
		case '$':
		case ',':
			push(key);
		case SDLK_RETURN:
			executeStack();
			/* fall through */
		case SDLK_ESCAPE:
			stack.top = 0;
			//displayContent();
			break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
		case SDLK_LCTRL:
		case SDLK_RCTRL:
			DEBUG("Done\n");
			return;
		default:
			break;
	}
	// Convert to cmdKey_t
	if (push(key) == -1){	// push to stack
		printf("Invalid Keycode: %d\n", kbe.keysym.sym);
	}
	displayContent();
	DEBUG("Done\n");
}


int push(cmdKey_t key)
{
	if (stack.top == STK_SIZE-1 || !isprint(key.sym)){
		stack.top = 0;	// CLEAR STACK
		return -1;	// Operation failed
	} 	
	stack.keystack[stack.top] = key;
	stack.top++;
	return 0;
}

cmdKey_t pop()
{
	if (stack.top == 0){
		return (cmdKey_t){0xff, 0xff}; 
	}
	stack.top--;
	stack.isChanged = true;
	return stack.keystack[stack.top];
}

/* returns to old view state */
void undoCommand(void)
{
	if (history.index == 0){
		history.index = HISTORY_MAX-1;
	} else {
		history.index--;
	}
	view = history.snapshot[history.index];
	printf("Index after undo: %d\n", history.index);
}

/* Enqueues the current view to the history */
void enqueueCurrent(void)
{
	printf("Enqueueing to index: %d\n", history.index);
	history.snapshot[history.index] = view;
	if (history.index == HISTORY_MAX-1){
		history.index = 0;
	} else {
		history.index = (history.index+1) % (HISTORY_MAX);
	}
}

static bool isInvalid(cmdKey_t key)
{
	return (key.sym == 0xff && key.mod == 0xff);
}

/* determines the number of times a command should be run */
static unsigned long long getTimes()
{
	cmdKey_t reg;
	unsigned long long magnitude = 1;
	unsigned long long times = 0;
	int currentTop = stack.top-1;

	while (currentTop != 0){
		reg = pop();
		if (isInvalid(reg)){
			printf("Done parsing stack content in %d... current stack top is %d\n", __LINE__, currentTop);
			break;
		}
		if (isdigit(reg.sym)){ // 0x30 - 0x3a
			times += magnitude * (reg.sym - 0x30);
			printf("digit: %d := %llu\n", reg.sym-0x30, times);
			magnitude *= 10;
		} else {
			printf("KEY IGNORED: %c\n", reg.sym);
		}
		currentTop--;
	}
	if (times == 0){
		times = 1;
	}
	return times;
}

/* No modifiers */
static void executeDefaultCommand(char sym, unsigned long long times)
{
	double old;
	undoAction = true;
	redoFlag = RECALC;		// if needed, command handler must set to false
	switch (sym){ 
		case 'q':	// shift slice coloring +
			view.shift += 0.1*times;
			redoFlag = REDRAW;
			break;
		case 'a':	// shift slice coloring -
			view.shift -= 0.1*times;
			redoFlag = REDRAW;
			break;
		case 'b':	// export image as bitmap
			printf("Wrote %lu bytes.\n", sizeof(int)*
					pixelsToBmp(WIN_WIDTH, WIN_HEIGHT, pixels, &view));
			undoAction = false;
			redoFlag = NOTHING;
			break;
		case 'z':	// Zoom in
			old = view.scaling;
			view.scaling -= times * view.scaling * 0.05;
			//view.centerx += ((view.centerx*view.scaling)-old*view.scaling)*WIN_WIDTH/2.0L;
			//view.centerx += (view.centerx-WIN_WIDTH/2.0L) * view.scaling;
			/*view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			break;
		case 'o':	// Zoom out
			view.scaling += times * view.scaling * 0.05;
			/*view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			printf("Set scaling to %lf\n", view.scaling);
			break;
		case 'w':	// Increase spread
			view.spread += times*0.1;
			redoFlag = REDRAW;
			break;
		case 's':	// decrease spread
			view.spread -= times*0.1;
			redoFlag = REDRAW;
			break;
		case 'c':	// Toggle Coloring mode, mod count(coloring_modes)
			colorArray[view.colorIndex].changeColor1(times);
			redoFlag = REDRAW;
			break;
		case 'p':	// Toggle Coloring mode, mod count(coloring_modes)
			colorArray[view.colorIndex].changeColor2(times);
			redoFlag = REDRAW;
			printf("CHANGING COLOR 2: %d\n", view.color2);
			break;
		case '$':	// Toggle Coloring mode, mod count(coloring_modes)
			colorArray[view.colorIndex].changeColorRandom(times);
			redoFlag = REDRAW;
			printf("CHANGING COLOR 2: %d\n", view.color2);
			break;

		case 'v':	// toggle reals || imaginaries
			redoFlag = REDRAW;
			undoAction = false;
			if (compvar == &creal){
				compvar = &cimag;
				printf("Plotting imaginaries\n");
				break;
			}
			compvar = &creal;
			printf("Plotting reals\n");
			break;
		case 'x':	// toggle axis IC
			showAxis = showAxis^1;
			redoFlag = REDRAW;
			undoAction = false;
			break;
		case 'e':	// export params IC
			printView();
			colorArray[view.colorIndex].printColorInfo();
			redoFlag = NOTHING;
			undoAction = false;
			break;
		case 'h':
			view.centerx -= times * WIN_WIDTH * FACTOR * view.scaling;
			break;
		case 'l':
			view.centerx += times * WIN_WIDTH * FACTOR * view.scaling;
			break;
		case 'j':
			view.centery -= times * WIN_HEIGHT * FACTOR * view.scaling;
			break;
		case 'k':
			view.centery += times * WIN_HEIGHT * FACTOR * view.scaling;
			break;
		case 'm':	// clears most params
			resetView();
			break;
		case 'r':	/* Decrease iterations */
			view.iterations -= times;
			break;
		case 't':	/* Increase iterations */
			view.iterations += times;
			break;
		case 'f':	/* Decrement variable */
			view.var1 += times;
			break;
		case 'g':	/* Increment variable */
			view.var1 -= times;
			break;
		case 'n':	/* Decrement variable */
			view.var2 += times;
			break;
		case 'y':	/* Increment variable */
			view.var2 -= times;
			break;
		case 'i':	/* Decrement variable */
			view.var3 += times;
			break;
		case 'd':	/* Increment variable */
			view.var3 -= times;
			break;
		case 'u':	// Optimise for multiple undos
			for (int i = 0; i < times; i++){
				undoCommand(); 			// sets current view to previous view
				if (i == HISTORY_MAX-1){
					printf("Reached oldest change");
					break;
				}
			}				

			redoFlag = fromViewDiff();
			undoAction = false;
			break;
		default:
			printf("invalid command top of stack: %d, mag: %llu\n", sym, times);
			redoFlag = NOTHING;
			break;


	}
	/* Add the view modification to the history PRE RENDER*/
	if (undoAction){
		enqueueCurrent();
	}
}

/* Shift modifiers */
static void executeShiftCommand(char sym, unsigned long long times)
{	double old;
	float endcols[6];
	redoFlag = RECALC;
	switch (sym) {
		case 'z':	// Zoom in
			old = view.scaling;
			view.scaling -= times * view.scaling*0.1;
			//view.centerx += (view.centerx*view.scaling*WIN_WIDTH/2.0L)-old*view.scaling*WIN_WIDTH/2.0L;
			/*view.centerx += (view.centerx-WIN_WIDTH) * view.scaling;
			/ *view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			break;
		case 'o':	// Zoom out
			view.scaling += times * view.scaling*0.1;
			/*view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			printf("Set scaling to %lf\n", view.scaling);
			break;
		case 'f':
			funcIndex = (funcIndex + times) % funcCount;
			if (funcArray[funcIndex].defaultView.iterations != 0) {
				view = funcArray[funcIndex].defaultView;
			}
			printf("Plotting %s\n",funcArray[funcIndex].name);
			break;
		case 'q':	// shift slice coloring +
			view.shift += 10.0*times;
			redoFlag = REDRAW;
			break;
		case 'a':	// shift slice coloring -
			view.shift -= 10.0*times;
			redoFlag = REDRAW;
			break;
		case 'm': /* unused */
			oscAmplifier += 0.1 * times;
			printf("OscAmp as exp(%.2f): %.4f\n", oscAmplifier, exp(oscAmplifier));
			break;
		case ',': /* unused */
			oscAmplifier -= 0.1 * times;
			printf("OscAmp as exp(%.2f): %.4f\n", oscAmplifier, exp(oscAmplifier));
			break;

		case 'h':
			view.centerx -= times * WIN_WIDTH * 100.0 * FACTOR * view.scaling;
			break;
		case 'l':
			view.centerx += times * WIN_WIDTH * 100.0 * FACTOR * view.scaling;
			break;
		case 'j':
			view.centery -= times * WIN_HEIGHT * 100.0 * FACTOR * view.scaling;
			break;
		case 'k':
			view.centery += times * WIN_HEIGHT * 100.0 * FACTOR * view.scaling;
			break;
		case 'w':	// Increase spread
			view.spread += times*1.0;
			redoFlag = REDRAW;
			break;
		case 's':	// decrease spread
			view.spread -= times*1.0;
			redoFlag = REDRAW;
			break;
		case 'x':	/* Decrement variable */
			view.var1 += 10 * times;
			break;
		case 'g':	/* Increment variable */
			view.var1 -= 10 * times;
			break;
		case 'n':	/* Decrement variable */
			view.var2 += 10 * times;
			break;
		case 'y':	/* Increment variable */
			view.var2 -= 10 * times;
			break;
		case 'i':	/* Decrement variable */
			view.var3 += 10 * times;
			break;
		case 'd':	/* Increment variable */
			view.var3 -= 10 * times;
			break;		
		case 'c':	/* change color function*/
			view.colorIndex = (view.colorIndex + times) % colorCount;
			redoFlag = REDRAW;
			break;		
		case 'p':	/* change fluid color */
			// USELESS
			view.color2 = (view.color2 + times) % colorCount;
			redoFlag = REDRAW;
			break;		
		case 'v': 	/* Full Animation */
			view.shift = rand() % 4 -8.0;
			view.spread = rand() % 4 - 2.0;
		case 'b':	/* pure Color Animation*/
			for (int i = 0; i < 6; i++) {
				endcols[i] = (float) (rand() % 256);
			}
			createFullAnimation(32, 50, endcols, view.spread, view.shift);
			//createColorAnimation(16,100,endcols);
			/*createShiftAnimation(16,100,rand() % 8-8.0);
			createSpreadAnimation(32,50,rand() % 4 -8.0);*/
			break;
		case '$':	/* Random Color, no bounds REALLY FUNKY */
			for (int i = 0; i < 6; i++) {
				fluidColors[i] = (float) (rand() % 20000 - 10000);
			}
			break;
		default:
			printf("Not implemented: %d %c\n", sym, sym); 
			redoFlag = NOTHING;
			break;
	}
}

/* Control modifier */
static void executeControlCommand(char sym, unsigned long long times)
{
	double old = view.scaling;	// used for calculating offset change when zooming
	redoFlag = RECALC;
	switch (sym) {
		case 'z':	// Zoom in
			view.scaling -= times * view.scaling * 0.01;
			/*view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			break;
		case 'o':	// Zoom out
			view.scaling += times * view.scaling *0.01;
			/*view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;*/
			printf("Set scaling to %lf\n", view.scaling);
			break;
		case 'h':
			view.centerx -= times * WIN_WIDTH * 0.01 * FACTOR * view.scaling;
			break;
		case 'l':
			view.centerx += times * WIN_WIDTH * 0.01 * FACTOR * view.scaling;
			break;
		case 'j':
			view.centery -= times * WIN_HEIGHT * 0.01 * FACTOR * view.scaling;
			break;
		case 'k':
			view.centery += times * WIN_HEIGHT * 0.01 * FACTOR * view.scaling;
			break;
		case 'q':	// shift slice coloring +
			view.shift += 0.01*times;
			redoFlag = REDRAW;
			break;
		case 'a':	// shift slice coloring -
			view.shift -= 0.01*times;
			redoFlag = REDRAW;
			break;
		case 'w':	// shift slice coloring +
			view.spread += 0.01*times;
			redoFlag = REDRAW;
			break;
		case 's':	// shift slice coloring -
			view.spread -= 0.01*times;
			redoFlag = REDRAW;
			break;
		case 'c':	/* Shift Animation */
			createShiftAnimation(16,100,rand() % 8-8.0);
			break;
		case 'x':	/* Shift Oscillation */ 
			createShiftAnimationOsc(8, animationDuration*25, 9, oscAmplifier);
			break;
		case 'y':	/* Spread Oscillation */
			createSpreadAnimationOsc(8, animationDuration*10, 9, oscAmplifier);
			break;
		case 'v':
			createSpreadAnimation(32,50,rand() % 4 -8.0);
			break;
		default:
			printf("Not implemented: %d %c\n", sym, sym); 
			redoFlag = NOTHING;
			break;
	}

	printf("Not implemented command handler %s\n", __func__); 
}

/* Control + Shift modifier */
static void executeControlShiftCommand(char sym, unsigned long long times)
{

	printf("Not implemented command handler %s\n", __func__); 
}

/* Controls are handled here */
void executeStack()
{
	/* command execution lookup table with array of function pointers */
	static void (*executeCommand[])(char, unsigned long long) = {
		&executeDefaultCommand,
		&executeShiftCommand,
		&executeControlCommand,
		&executeControlShiftCommand
	};

	cmdKey_t commandKey = pop();
	unsigned long long times = getTimes();
	if (isInvalid(commandKey)){
		printf("No command key found!\n");
	}
	executeCommand[commandKey.mod](commandKey.sym, times);
}
