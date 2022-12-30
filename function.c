#include <math.h>
#include <complex.h>
#include "function.h"
#include "view.h"
// hz 0.0,0.0,0.000015,12,1,1,2,0.0,0.3,2,9
#define DEFAULT_VIEW = {}
func_t funcArray[] = {
	{
		&nome,
		"Square of the nome",
		{

		}
	}, {
		&zeta,
		"Hurwitz Zeta function",
		{

		}
	}, {
		&E2,
		"Eisenstein Series 2",
		{

		}
	}, {
		&E4,
		"Eisenstein Series 4",
		{

		}
	}, {
		&E6,
		"Eisenstein Series 6",
		{

		}
	}, {
		&E8,
		"Eisenstein Series 8",
		{

		}
	}, {
		&J,
		"J-Invariant",
		{

		}
	}, {
		&K,
		"K-Invariant",
		{

		}
	}, {
		&L,
		"L-Invariant",
		{

		}
	}, {
		&M,
		"M-Invariant",
		{

		}
	}, {
		&N,
		"N-Invariant",
		{

		}
	}, {
		&O,
		"O-Invariant",
		{

		}
	}, {
		&P,
		"P-Invariant",
		{

		}
	}, {
		&Q,
		"Q-Invariant",
		{

		}
	}, {
		&R,
		"R-Invariant",
		{

		}
	}, {
		&S,
		"S-Invariant",
		{

		}
	}, {
		&T,
		"T-Invariant",
		{

		}
	}, {
		&U,
		"U-Invariant",
		{

		}
	}, {
		&V,
		"V-Invariant",
		{

		}
	}, {
		&W,
		"W-Invariant",
		{

		}
	}, {
		&X,
		"X-Invariant",
		{

		}
	}, {
		&Y,
		"Y-Invariant",
		{

		}
	}, {
		&Z,
		"Z-Invariant",
		{

		}
	}, {
		&mandelbrot,
		"Mandelbrot Set",
		{
			.centerx = -29.286740,
			.centery = 1791.006725,
			.scaling = 0.000421,
			.iterations = 54,
			.var1 = 1,
			.var2 = 1,
			.var3 = 1,
			.spread = -2.698186,
			.shift = -0.150188,
			.colorIndex = 2
		}
	}, {
		&grad,
		"Gradient function",
		{

		}
	}, {
		&collatz,
		"CollatzFractal",
		{
			0.0,0.0,0.000015,4,7,18,2,-1.4,0.3,2,9
		}
	}, {
		&crex,
		"Complex exp reciprocal",
		{
			308.0,125.0,0.00039,4,7,21,5,-1.1,0.3,2,6
		}
	}, {
		&julia,
		"Julia Set of Mandelbrot Set",
		{
			0.0,0.0,0.0028,30,-125,0,0,-7.5,0.0,2,7
		}
	}, {
		&eta,
		"Dedekind Eta Function",
		{
			0.0,321.303881,0.002562,12,0,0,0,-4.0,0.0,2,9
		}
	}, {
		&moddisc,
		"Modular Discriminant",
		{
			0.0,321.303881,0.002562,12,0,0,0,17.7,0.0,2,2
		}
	}, {
		&theta,
		"Jacobi Theta Function",
		{
			.centerx = 0.0,
			.centery = 0.0,
			.scaling = 0.003850,
			.iterations = 12,
			.var1 = -2,
			.var2 = 2,
			.var3 = 1,
			.spread = -1.599395,
			.shift = 0.029812,
			.colorIndex = 2
		}
	}, {
		&tet,
		"ComplexTetration",
		{
			.centerx = 220.157742,
			.centery = 0.000000,
			.scaling = 0.003874,
			.iterations = 12,
			.var1 = 0,
			.var2 = 0,
			.var3 = 0,
			.spread = -7.800000,
			.shift = 0.000000,
			.colorIndex = 2
		}
	}, {
		&polylog,
		"Polylogarithm",
		{
			220.157742,0.0,0.003874,12,0,0,0,-7.8,0.0,2,2
		}
	}
};

const int funcCount = sizeof(funcArray) / sizeof(funcArray[0]);

complex double nome(complex double t)
{
	return cexp(t * 2.0 * M_PI * I);

}

/**
 * Howitz Zeta Function
 */
complex double zeta(complex double s)
{
	complex double sum = CMPLX(0.0, 0.0);
	double a = 1.0L/view.var1;
	for (int n = 1; n < view.iterations; n++){
		sum += 1.0/cpow(n+a,s);
	}
	return sum;
}

/* Eisensteinseries 2 */
complex double E2(complex double q) // q = square of the nome
{
	complex double sum = CMPLX(0.0, 0.0);
	complex double qn;
	for (int n = 1; n < view.iterations; n++){
		qn = cpow(q,n);
		sum += (n * qn) / (1.0-qn);
	}
	return 1.0 - sum*24.0;
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

/* Eisensteinseries 6 */
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

/* Eisensteinseries 8 */
complex double E8(complex double q) // q = square of the nome
{
	complex double sum = CMPLX(0.0, 0.0);
	complex double qn;
	for (int n = 1; n < view.iterations; n++){
		qn = cpow(q,n);
		sum += (n*n*n*n*n*n*n * qn) / (1.0-qn);
	}
	return 1.0 + sum*480.0;
}

complex double J(complex double t)
{
	complex double q = cexp(M_PI*I*2.0*t);
	complex double e4 = E4(q);
	complex double e6 = E6(q);
	e4 *= (e4 * e4);
	return 1728.0 * (e4 / (e4-e6*e6));
}

complex double K(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e2 = E2(q);
	return 1728.0L*(e2/e2-e2*e2);
}

complex double L(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e2 = E2(q);
	complex double e4 = E4(q);
	return 1728.0L*(e2/e2-e4*e4);
}

complex double M(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e2 = E2(q);
	complex double e6 = E6(q);
	return 1728.0L*(e2/e2-e6*e6);
}

complex double N(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e2 = E2(q);
	complex double e8 = E8(q);
	return 1728.0L*(e2/e2-e8*e8);
}

complex double O(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e4 = E4(q);
	complex double e2 = E2(q);
	return 1728.0L*(e4/e4-e2*e2);
}

complex double P(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e4 = E4(q);
	return 1728.0L*(e4/e4-e4*e4);
}

complex double Q(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e4 = E4(q);
	complex double e6 = E6(q);
	return 1728.0L*(e4/e4-e6*e6);
}

complex double R(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e4 = E4(q);
	complex double e8 = E8(q);
	return 1728.0L*(e4/e4-e8*e8);
}

complex double S(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e6 = E6(q);
	complex double e2 = E2(q);
	return 1728.0L*(e6/e6-e2*e2);
}

complex double T(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e6 = E6(q);
	complex double e4 = E4(q);
	return 1728.0L*(e6/e6-e4*e4);
}

complex double U(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e6 = E6(q);
	return 1728.0L*(e6/e6-e6*e6);
}

complex double V(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e6 = E6(q);
	complex double e8 = E8(q);
	return 1728.0L*(e6/e6-e8*e8);
}

complex double W(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e8 = E8(q);
	complex double e2 = E2(q);
	return 1728.0L*(e8/e8-e2*e2);
}

complex double X(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e8 = E8(q);
	complex double e4 = E4(q);
	return 1728.0L*(e8/e8-e4*e4);
}

complex double Y(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e8 = E8(q);
	complex double e6 = E6(q);
	return 1728.0L*(e8/e8-e6*e6);
}

complex double Z(complex double z)
{	complex double q = cexp(I*M_PI*2.0L*z);
	complex double e8 = E8(q);
	return 1728.0L*(e8/e8-e8*e8);
}


complex double mandelbrot(complex double c)
{
	complex double z = CMPLX(0.0,0.0);
	for (int n = 0; n < view.iterations; n++){
		z = z*z + c;
	}
	return z;
}

complex double grad(complex double z)
{
	for (int n = 0; n < view.iterations; n++){
		z = z*z;
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

complex double polylog(complex double z)
{
	complex double sum = CMPLX(0.0,0.0);
	complex double s = CMPLX((double)view.var1*0.1, (double)view.var2*0.1);
	//complex double temp = CMPLX(0.0, 0.0);
	for (int n = 1; n < view.iterations; n++){
		sum += cpow(z, n)/cpow(n,s);
	}
	return sum;
}
