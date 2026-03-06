/**
 * MathConversion.cpp - Unit and coordinate conversions (consolidated from DEGTORAD, RADTODEG, CART, SPHCART)
 */
#include "stdafx.h"
#include <math.h>
#include "calc.h"

#define PII 3.141592654
#define RPD (PII/180.0)
#define DPR (180.0/PII)

double degtorad(double a)
{
	return RPD * a;
}

double radtodeg(double a)
{
	return DPR * a;
}

short cartsph(double p0[4], double *disty, double *thetax, double *thetaz)
{
	double distxy, thetaz1;
	double pii = 3.141592654;
	*disty = sqrt(p0[0] * p0[0] + p0[1] * p0[1] + p0[2] * p0[2]);
	if (*disty == 0.0)
		return -1;
	distxy = sqrt(p0[0] * p0[0] + p0[1] * p0[1]);
	if (distxy == 0.0)
		return -1;
	*thetax = asin((p0[2]) / (*disty));
	thetaz1 = asin(p0[1] / distxy);
	if (p0[0] >= 0) {
		*thetaz = thetaz1;
	} else {
		if (p0[1] >= 0) {
			*thetaz = (pii - thetaz1);
		} else {
			*thetaz = -(pii + thetaz1);
		}
	}
	*thetaz = 2 * pii - (pii + *thetaz);
	return 1;
}

short cartsph_left(double p0[4], double *disty, double *thetax, double *thetaz)
{
	double distxy, thetaz1;
	double pii = 3.141592654;
	*disty = sqrt(p0[0] * p0[0] + p0[1] * p0[1] + p0[2] * p0[2]);
	if (*disty == 0.0)
		return -1;
	distxy = sqrt(p0[0] * p0[0] + p0[1] * p0[1]);
	if (distxy == 0.0)
		return -1;
	*thetax = asin((p0[2]) / (*disty));
	thetaz1 = asin(p0[1] / distxy);
	if (p0[0] <= 0) {
		*thetaz = pii - thetaz1;
	} else {
		if (p0[1] >= 0) {
			*thetaz = thetaz1;
		} else {
			*thetaz = 2 * pii + thetaz1;
		}
	}
	return 1;
}

void sphcart(struct sphvect *a, struct ams_vector *b)
{
	double xyproj = a->r * cos(a->fii);
	b->x = xyproj * cos(a->theta);
	b->y = xyproj * sin(a->theta);
	b->z = a->r * sin(a->fii);
}
