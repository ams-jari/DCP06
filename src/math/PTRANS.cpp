/*
Ttransform a point in base system to a new coordinate system

Function:	ptrans(double a[4][4], double v[4], double (*w)[4])
Call:		ptrans(&a, &v, &w)
Input:		double a[4][4]
		double v[4]
Output:		double (*w)[4]
*/
#include "stdafx.h"
#include "calc.h"
/*
vecmat(double v[4],double a[4][4],double (*w)[4]);
*/
void ptrans(double v[4],double a[4][4], double (*w)[4])
//double  a[4][4],v[4],(*w)[4];

{

	short i,j;
	double vt[4],ar[4][4];

/*
*   translate point to the origin of the new system
*/

	for(i=0; i<=3; i++)
		vt[i] = v[i] - a[i][3];

/*
*   rotate the origin of the new system by the rotation matrix of the new syst.
*/

	for(i=0; i<=3; i++)
		for(j=0; j<=3; j++)
			ar[i][j]=a[i][j];

	for(i=0; i<=2; i++)
		ar[i][3]=0.0; /* no translation */

	vecmat(vt,ar,w);

}

