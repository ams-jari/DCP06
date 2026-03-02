/*

Function:	direction cosines of vector from pa to pb; result in dv
Input:		pa,pb
Output:	dv
*/
#include "stdafx.h"
#include <math.h>

short dir_vector(double pa[4], double pb[4], double (*dv)[4])
//double pa[4],pb[4],(*dv)[4];
{
	double a,b;

	(*dv)[0] = pb[0] - pa[0];
	(*dv)[1] = pb[1] - pa[1];
	(*dv)[2] = pb[2] - pa[2];
	(*dv)[3] = 1.0;

	a = (*dv)[0]*(*dv)[0]+(*dv)[1]*(*dv)[1]+(*dv)[2]*(*dv)[2];
	b = sqrt(a);

	if(b == 0)
	{
	(*dv)[0] = 0;             /* prevent division by zero */
	(*dv)[1] = 0;
	(*dv)[2] = 0;
	return(-1);
	}
	else
	{
	(*dv)[0] = (*dv)[0]/b;    /* normalize to unit vector */
	(*dv)[1] = (*dv)[1]/b;
	(*dv)[2] = (*dv)[2]/b;
	return(1);
	}
}

