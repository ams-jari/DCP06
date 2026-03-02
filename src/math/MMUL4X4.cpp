/*
Function:	matmul(double  a[4][4], double b[4][4], double (*c)[4][4])
Call:		matmul(&a, &b, &c)
Input:		double a[4][4], b[4][4]  matrices
Output:	double (*c) [4][4]

*/
#include "stdafx.h"
short matmul4x4(double a[4][4],double b[4][4], double (*c)[4][4])
{
	short i,j,k;
	double ab;


	for(i=0; i<=3; i++)
	{
		for(j=0; j<=3; j++)
		{
			ab = 0.0;
			for(k=0; k<=3; k++)
			{
				ab = ab + a[i][k]*b[k][j];
			}
			(*c)[i][j] = ab;
		}
	}

	return 1;
}
