/*
Multiply vector v by matrix a = vector w

Function:	vecmat(double  a[4][4], double v[4], double (*w)[4];
Call:		vecmat(&a, &v, &w)
Input:		double a[4][4]
		double v[4]
Output:		double w[4]
*/

#include "stdafx.h"


int vecmat(double v[4], double a[4][4],double (*w)[4])
//double  a[4][4],v[4],(*w)[4];

{
	short i,j;
	double ab;


	for(i=0; i<=3; i++)
	{
		ab = 0.0;
		for(j=0; j<=3; j++)
		{
			ab = ab + v[j]*a[j][i];
		}
		(*w)[i] = ab;
	}

	return 1;

}


