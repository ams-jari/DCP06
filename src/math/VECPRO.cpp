/*
Vector product u[4] x v[4] = w[4]    (u[3] = v[3] = w[3] = 1)

Function:	vecpro(double  u[4], double v[4], double (*w)[4]);
Call:		vecpro(&u, &v, &w)
Input:		double u[4]
		double v[4]
Output:		double w[4]
*/
#include "stdafx.h"
//#include <math.h>
#include <math.h>

void vec_product(double u[4], double v[4], double (*w)[4])

//double  u[4],v[4],(*w)[4];

{

	(*w)[0] = u[1]*v[2] - u[2]*v[1];
	(*w)[1] = - u[0]*v[2] + u[2]*v[0];
	(*w)[2] = u[0]*v[1] - u[1]*v[0];
	(*w)[3] = u[3]*v[3];

}

