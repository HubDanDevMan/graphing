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
#include "animations.h"
#include "coloring.h"
#include "view.h"
#include "debug.h"
#include "parallelism.h"


#define NUM_THREADS 4

/* Function pointers */
complex double (*cfunc)(complex double);	// The Complex Function to be drawn, from 'function.c'
//double (*compvar)(complex double) = &creal; 		// cimag | creal
int (*colorfunc)(complex double) = &coloring3;		// coloring function




static inline double startValX()
{
	return ( - (WIN_WIDTH / 2.0)) * view.scaling;
}

static inline double startValY()
{
	return ( + (WIN_HEIGHT / 2.0)) * view.scaling;
}

static void calcFunction(){
	complex double point;
	unsigned int ticks = SDL_GetTicks();
	colorfunc = colorArray[view.colorIndex].color;
	for (int i = 0; i < WIN_HEIGHT; i++){ // row
		for (int j = 0; j < WIN_WIDTH; j++){ // column
			point = cfunc(CMPLX(view.scaling*(j+view.centerx-(WIN_WIDTH/2.0L)),
				 view.scaling*(-i+view.centery+(WIN_HEIGHT/2.0))));
			numbers[i*WIN_WIDTH+j] = point;	// number buffer for faster recoloring 
			pixels[i*WIN_WIDTH+j] = colorfunc(point);
		}
	}
	DEBUG("Done calculating, took %d ms", SDL_GetTicks() - ticks); 
}

/**
 * Colors the function
 */
static void drawFunction(){
	colorfunc = colorArray[view.colorIndex].color;
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
	int currentFrameLength;
	int frameDuration;
	resetView();
	setupConcurrentGraphing(NUM_THREADS);

	// Initialize colors randomly
	colorArray[view.colorIndex].changeColorRandom(0);
	/* Program loop */
	while (!quit)
	{

		/* Check if Events occured and handle them */
		while (SDL_PollEvent(&event)){

			switch (event.type)
			{
				case SDL_QUIT:
					quit = true;
					cleanConcurrentGraphing();
					return;
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
		colorfunc = colorArray[view.colorIndex].color;


		currentFrameLength = SDL_GetTicks();
		// changes the view according to the animation
		frameDuration = playAnimation();
		/* rerenders the function if required */
		if (recalcRequired()){
			// Concurrent render
			executeConcurrentGraphing();
			SDL_SetWindowTitle(window, funcArray[funcIndex].name);
			//calcFunction(); // redraws itself aswell */
			printf("-");
			goto doneRecalc;
		}	
		// in/decrease iteration, partial update
		if (redrawRequired()){
			drawAxis();
			drawFunction();
			printf(".");
		}
		doneRecalc:
                
                if (frameDuration + currentFrameLength > SDL_GetTicks()){    
			DEBUG("Sleeping %d ms", frameDuration + currentFrameLength - SDL_GetTicks());
			SDL_Delay(frameDuration + currentFrameLength - SDL_GetTicks());  // short delay, set to performance mode by adjusting ticks    
                }

		//currentFrameLength = SDL_GetTicks() - currentFrameLength;    
		redoFlag = NOTHING;
		SDL_UpdateTexture(texture, NULL, pixels, WIN_WIDTH * sizeof(int)); 
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

}

