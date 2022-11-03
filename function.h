#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <math.h>
#include <complex.h>
#include "view.h"

// Find compile time Array length method (sizeof() ??)
typedef struct {
	complex double (*func)(complex double);	// function to pointer to the actual function
	// partial rerender functions
	const char * name;			// read-only at runtime
	// perhaps default values
	const struct viewstate defaultView;	// read-only at runtime
} func_t;


extern func_t funcArray[];
extern const int funcCount;

complex double nome(complex double t);

complex double zeta(complex double s);

/* Eisensteinseries 4 */
complex double E4(complex double q); // q = square of the nome

/* Eisensteinseries 5 */
complex double E6(complex double q); // q = square of the nome

complex double J(complex double t);

complex double mandelbrot(complex double c);

complex double collatz(complex double z);

complex double crex(complex double z);

complex double julia(complex double z);

complex double eta(complex double z);

complex double moddisc(complex double z);

complex double theta(complex double z);

complex double tet(complex double z);
#endif /* _FUNCTION_H */

