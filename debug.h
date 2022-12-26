#ifndef _DEBUG_GRAPHING_H
#define _DEBUG_GRAPHING_H

// In case of no stdio header file already included
#include <stdio.h>
#include <stdbool.h>


#define DEBUG_STATE 1
#define DEBUG_OUTPUT_STREAM stderr
// hacky C way with do while to run it once
#define DEBUG(fmt, ...)												\
	do {													\
		if (DEBUG_STATE == 1) {										\
			fprintf( DEBUG_OUTPUT_STREAM, "[%s]:%d\t" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);	\
			fflush(DEBUG_OUTPUT_STREAM);								\
		}												\
	} while (0);


/* _DEBUG_GRAPHING_H */
#endif
