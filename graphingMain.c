#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include "graphingMain.h"
#include "controls.h"
#include "function.h"
#include "coloring.h"
#include "view.h"


/* Function pointers */
complex double (*cfunc)(complex double) = &julia;	// The Complex Function to be drawn, from 'function.c'
double (*compvar)(complex double) = &creal; 		// cimag | creal
int (*colorfunc)(complex double) = &coloring2;		// coloring function


static inline double startValX()
{
	return view.centerx*view.scaling - (double) WIN_WIDTH / 2.0 * view.scaling;
}

static inline double startValY()
{
	return view.centery*view.scaling + (double) WIN_HEIGHT / 2.0 * view.scaling;
}

static void calcFunction(){
	complex double point;
	for (int i = 0; i < WIN_HEIGHT; i++){ // row
		for (int j = 0; j < WIN_WIDTH; j++){ // column
			point = cfunc(CMPLX(startValX() + (double)j*view.scaling,
				 startValY() + (double)-i*view.scaling));
			numbers[i*WIN_WIDTH+j] = point;	// number buffer for faster recoloring 
			pixels[i*WIN_WIDTH+j] = colorfunc(point);
		}
	}
}

/**
 * Colors the function
 */
static void drawFunction(){
	for (int i = 0; i < WIN_HEIGHT; i++){ // row
		for (int j = 0; j < WIN_WIDTH; j++){ // column
			pixels[i*WIN_WIDTH+j] = colorfunc(numbers[i*WIN_WIDTH+j]);
		}
	}
}

static void drawAxis()
{
	if (!showAxis){
		return;
	}
	// inverts color in axis layout
	int current;
	for (int i = 0; i < WIN_HEIGHT; i++){
		current = i * WIN_WIDTH + (WIN_WIDTH / 2);
		pixels[current] = pixels[current] ^ 0x00ffffff;
		pixels[current+1] = pixels[current+1] ^ 0x00ffffff;
		pixels[current-1] = pixels[current-1] ^ 0x00ffffff;
	}
	for (int i = 0; i < WIN_WIDTH; i++){ /* doesn't work */
		current = WIN_WIDTH * WIN_HEIGHT / 2;
		pixels[current] = pixels[current] ^ 0x00ffffff;
		pixels[current+WIN_WIDTH] = pixels[current+WIN_WIDTH] ^ 0x00ffffff;
		pixels[current-WIN_WIDTH] = pixels[current-WIN_WIDTH] ^ 0x00ffffff;
	}

}

void graphingMain(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture)
{
	bool quit = false;
	SDL_Event event;
	int shiftIndex = 0;

	/* Program loop */
	while (!quit)
	{

		/* Check if Events occured and handle them */
		while (SDL_PollEvent(&event)){

			switch (event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					handleKeyEvent(event.key);
					break;
				default:
					break;
			}
		}
		cfunc = funcArray[funcIndex].func;
		/* rerenders the function if required */
		if (recalcRequired()){
			calcFunction(); // redraws itself aswell
			printf("-");
			goto doneRecalc;
		}	
		// in/decrease iteration, partial update
		if (redrawRequired() | animation){
			if (animation) {
				view.shift = 5.0 * sin(0.5*(double)shiftIndex++);
			}
			drawAxis();
			drawFunction();
			printf(".");
		}
		doneRecalc:
		redoFlag = NOTHING;
		fflush(stdout);
		SDL_UpdateTexture(texture, NULL, pixels, WIN_WIDTH * sizeof(int)); 
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}


