#ifndef _BMP_H
#define _BMP_H
#include <stdint.h>
#include "view.h"

// Bitmap header according to https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
struct bitmapHeader {
	char magnum[2]; // 'b','m'
	int32_t size;	// filesize in bytes
	int32_t unused;// keep as zeros, application specific
	int32_t offset;// fileoffset to pixel array
	int64_t width; // in pixels
	int64_t height; // in pixels
	int16_t planes;
	int16_t bpp;	// bits per pixel
	int16_t comp;	// compression; 0=uncompressed
	int32_t bmsize;// size of the bitmap data
	int64_t resx; // in pixels per meter
	int64_t resy; // in pixels per meter
	int32_t usedcols; // used colors
	int32_t impcols; // important colors
};

/* stores pixelbuffer as bitmap */
unsigned long pixelsToBmp(int width, int height, int* data, struct viewstate * view);

#endif /* _BMP_H */
