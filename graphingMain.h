#ifndef _GRAPHING_MAIN_H
#define _GRAPHING_MAIN_H
#include <SDL2/SDL.h>
#include <complex.h>

extern complex double (*cfunc)(complex double);	// The Complex Function to be drawn
extern double (*compvar)(complex double); 		// cimag | creal
extern int (*colorfunc)(complex double);		// coloring function


void graphingMain(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

#endif  /* _GRAPHING_MAIN_H */
