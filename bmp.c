#include "view.h"
#include "bmp.h"
#include "view.h"
#include <stdio.h>
#include <stdlib.h>

/* stores pixelbuffer as bitmap */
unsigned long pixelsToBmp(int width, int height, int* data, struct viewstate * view)
{
	#define BUFF_LEN 255
	struct bitmapHeader header = {
		{'B','M'},
		36+width * height * sizeof(int),
		0,
		36,
		width,
		-(long)height,
		1,
		24,	// change to 24
		0,
		width * height * sizeof(int),
		2835,
		2835,
		0,
		0
	};
	char filename[BUFF_LEN];
	unsigned long bwritten; 
	char * pxbuffer = malloc( (sizeof(int)-1) * width * height);
	if (pxbuffer == NULL){
		fprintf(stderr, "Unable to allocate pxbuffer\n");
		exit(-1);
	}

	// convert viewdata to string and use as filename. img_X_Y-spread-shift.bmp
	snprintf(filename, BUFF_LEN, "img%lf_%lf-%lf-%lf_%d.bmp",
		view->centerx, view->centery, view->spread, view->shift, view->iterations);
	FILE * f = fopen(filename, "wb");
	bwritten = fwrite(&header, sizeof(struct bitmapHeader), 1, f);
	for (int i = 0; i < width*height; i++){
		bwritten += fwrite(&data[i], 3, 1, f);
	}
	fclose(f);
	return bwritten;
}

