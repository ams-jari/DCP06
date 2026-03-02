/*
Transform a vector pa to a unit vector uv; result in uv

Function:	univec(double pa[4], double (*uv)[4]);
Call:		univec(&pa, &uv)
Input:		double	pa[4]
Output:		double uv[4]

*/
#include "stdafx.h"
#include <math.h>

short unit_vector(double pa[4], double (*uv)[4])
//double pa[4],(*uv)[4];
{
	double a,b;

	a = pa[0]*pa[0]+pa[1]*pa[1]+pa[2]*pa[2];
	b = sqrt(a);

	if(b == 0)
	{
	(*uv)[0] = 0;             /* prevent division by zero */
	(*uv)[1] = 0;
	(*uv)[2] = 0;
	return(-1);
	}
	else
	{
	(*uv)[0] = pa[0]/b;      /* normalize to unit vector */
	(*uv)[1] = pa[1]/b;
	(*uv)[2] = pa[2]/b;
	return(1);
	}
}

