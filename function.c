#include <math.h>
#include <complex.h>
#include "function.h"
#include "view.h"

complex double nome(complex double t)
{
	return cexp(t * 2.0 * M_PI * I);

}

complex double zeta(complex double s)
{
	complex double sum = CMPLX(0.0, 0.0);
	for (int n = 1; n < view.iterations; n++){
		sum += 1.0/cpow(n,s);
	}
	return sum;
}

/* Eisensteinseries 4 */
complex double E4(complex double q) // q = square of the nome
{
	complex double sum = CMPLX(0.0, 0.0);
	complex double qn;
	for (int n = 1; n < view.iterations; n++){
		qn = cpow(q,n);
		sum += (n*n*n * qn) / (1.0-qn);
	}
	return 1.0 + sum*240.0;
}

/* Eisensteinseries 5 */
complex double E6(complex double q) // q = square of the nome
{
	complex double sum = CMPLX(0.0, 0.0);
	complex double qn;
	for (int n = 1; n < view.iterations; n++){
		qn = cpow(q,n);
		sum += (n*n*n*n*n * qn) / (1.0-qn);
	}
	return 1.0 - sum*504.0;
}

complex double J(complex double t)
{
	complex double q = cexp(M_PI*I*2.0*t);
	complex double e4 = E4(q);
	complex double e6 = E6(q);
	e4 *= (e4 * e4);
	return 1728.0 * (e4 / (e4-e6*e6));
}

complex double mandelbrot( complex double c)
{
	complex double z = CMPLX(0.0,0.0);
	for (int n = 0; n < view.iterations; n++){
		z = z*z + c;
	}
	return z;
}


complex double collatz(complex double z)
{
	for (int n = 0; n < view.iterations; n++){
		z = ccos(z*M_PI/((double)view.var1*0.01))/2.0 + ((double)view.var2*0.01*z+1.0)/2.0*csin(z*M_PI/2.0);
	}
	return z;
}

complex double crex(complex double z)
{	
	if (z == CMPLX(0.0, 0.0)){
		z += 0.0000000000001;
	}
	for (int n = 0; n < view.iterations; n++){
		z = cexp(1.0/(cpow(z, CMPLX(view.var1*0.05, view.var2*0.05))));
	}
	return z;
}

complex double julia(complex double z)
{
	complex double c = CMPLX((double)view.var1*0.01, (double)view.var2*0.01);
	for (int n = 0; n < view.iterations; n++){
		z = cpow(z, CMPLX(2.0, view.var3*0.005)) + c;
	}
	return z;
}

/* Dedekind Eta Function */
complex double eta(complex double z){
	complex double q = cexp(M_PI*2.0*z*I);
	complex double prod = CMPLX(1.0, 0.0);
	for (int n = 1; n < view.iterations; n++){
		prod *= 1.0 - cpow(q,(double) n);
	}
	return prod * cpow(q, 1.0/24.0);
}

/* Modular Discriminant in terms of eta */
complex double moddisc(complex double z)
{
	return pow(2.0*M_PI, 12.0)*cpow(eta(z),12.0);
}
/* Jacobi Theta function */
complex double theta(complex double z)
{
	// we define v1+v2i as the half period
	complex double q = cexp(I*M_PI*CMPLX(view.var1*0.1, view.var2*0.1));
	complex double sum = CMPLX(0.0, 0.0);
	for (int n = 1; n < view.iterations; n++){
		sum += cpow(q, n*n)*ccos(2.0 * M_PI*(double)n*z);
	}
	return 1.0+2.0*sum; 

}

complex double tet(complex double z)
{
	//complex double temp = CMPLX(0.0, 0.0);
	for (int n = 0; n < view.iterations; n++){
		z = cpow(z, z);
	}
	return z;
}
