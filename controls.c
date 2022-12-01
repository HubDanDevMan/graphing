#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "controls.h"
#include "function.h"
#include "coloring.h"
#include "graphingMain.h"
#include "bmp.h"
#include "view.h"

extern SDL_Window * window;

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
			push(key);
		case SDLK_RETURN:
			executeStack();
			/* fall through */
		case SDLK_ESCAPE:
			stack.top = 0;
			displayContent();
			break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
		case SDLK_LCTRL:
		case SDLK_RCTRL:
			return;
		default:
			break;
	}
	// Convert to cmdKey_t
	if (push(key) == -1){	// push to stack
		printf("Invalid Keycode: %d\n", kbe.keysym.sym);
	}
	displayContent();
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
			view.scaling -= times * view.scaling * 0.1;
			view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;
			break;
		case 'o':	// Zoom out
			view.scaling += times * view.scaling * 0.1;
			view.centerx += 2.0*view.centerx * view.scaling;
			view.centery += 2.0*view.centery * view.scaling;
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
			view.color1 = (view.color1+times)%COLOR_COUNT;
			redoFlag = REDRAW;
			printf("CHANGING COLOR 1: %d\n", view.color1);
			break;
		case 'p':	// Toggle Coloring mode, mod count(coloring_modes)
			view.color2 = (view.color2+times)%COLOR_COUNT;
			redoFlag = REDRAW;
			printf("CHANGING COLOR 2: %d\n", view.color2);
			break;

		case 'v':	// toggle reals || imaginaries
			redoFlag = REDRAW;
			if (compvar == &creal){
				compvar = &cimag;
				printf("Plotting imaginaries\n");
				break;
			}
			undoAction = false;
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
{
	redoFlag = RECALC;
	switch (sym) {
		case 'f':
			funcIndex = (funcIndex + times) % funcCount;
			if (funcArray[funcIndex].defaultView.iterations != 0) {
				view = funcArray[funcIndex].defaultView;
			}
			break;
		case 'q':	// shift slice coloring +
			view.shift += 10.0*times;
			redoFlag = REDRAW;
			break;
		case 'a':	// shift slice coloring -
			view.shift -= 10.0*times;
			redoFlag = REDRAW;
			break;
		case 'm':
			animation ^= animation;
			redoFlag = REDRAW;
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
		default:
			printf("Not implemented\n"); 
			redoFlag = NOTHING;
			break;
	}
}

/* Control modifier */
static void executeControlCommand(char sym, unsigned long long times)
{
	redoFlag = RECALC;
	switch (sym) {
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
		default:
			printf("Not implemented\n"); 
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
		printf("No command key found\n");
	}
	executeCommand[commandKey.mod](commandKey.sym, times);
}
