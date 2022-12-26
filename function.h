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

/* Eisensteinseries 2 */
complex double E2(complex double q); // q = square of the nome

/* Eisensteinseries 4*/
complex double E4(complex double q); // q = square of the nome

/* Eisensteinseries 6 */
complex double E6(complex double q); // q = square of the nome

/* Eisensteinseries 8 */
complex double E8(complex double q); // q = square of the nome

complex double J(complex double t);

complex double K(complex double t);

complex double L(complex double t);

complex double M(complex double t);

complex double N(complex double t);

complex double O(complex double t);

complex double P(complex double t);

complex double Q(complex double t);

complex double R(complex double t);

complex double S(complex double t);

complex double T(complex double t);

complex double U(complex double t);

complex double V(complex double t);

complex double W(complex double t);

complex double X(complex double t);

complex double Y(complex double t);

complex double Z(complex double t);

complex double mandelbrot(complex double c);

complex double grad(complex double c);

complex double collatz(complex double z);

complex double crex(complex double z);

complex double julia(complex double z);

complex double eta(complex double z);

complex double moddisc(complex double z);

complex double theta(complex double z);

complex double tet(complex double z);

complex double polylog(complex double z);

#endif /* _FUNCTION_H */

