#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphingMain.h"
#include "view.h"


int main(int argc, char ** argv)
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Texture * texture;

	/* Initialize all SDL related stuff */
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-2);
	}

	window = SDL_CreateWindow("Complex Graphing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, 0);
	if (window == NULL) {
		fprintf(stderr, "Unable to create SDL window: %s\n", SDL_GetError());
		exit(-2);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Unable to create SDL window: %s\n", SDL_GetError());
		exit(-2);
	}

	 texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, WIN_WIDTH, WIN_HEIGHT);
	if (texture == NULL) {
		fprintf(stderr, "Unable to create SDL window: %s\n", SDL_GetError());
		exit(-2);
	}
	pixels = malloc( sizeof(int) * WIN_WIDTH * WIN_HEIGHT);
	if (pixels == NULL){
		printf("Malloc failed for pixel buffer\n");
		exit(-1);
	}
	numbers = malloc( sizeof(complex double) * WIN_WIDTH * WIN_HEIGHT);
	if (pixels == NULL){
		printf("Malloc failed for number buffer\n");
		exit(-1);
	}

	/* Program loop */
	graphingMain(window, renderer, texture);


	/* Clean up */
	free(pixels);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


