/*
Rotate transformation matrix about z-axis

Function:		rotate_about_z_axis( double a[4][4], double b, double (*w)[4])
Call:			rotate_about_z_axis(&a, b, &c)
Input:			a[4][4] = transformation matrix
			b = rotation angle (radians)
Output:			w[4] = rotated transformation matrix
*/
#include "stdafx.h"
#include <math.h>
#include "calc.h"

//matmul4x4(double a[4][4],double b[4][4], double (*c)[4][4]);

void rotate_about_z_axis(double a[4][4], double b, double (*w)[4][4])
//double  a[4][4],b,(*w)[4];

{
	double r[4][4];

	r[0][0]=cos(b); r[0][1]=-sin(b); r[0][2]=0.0; r[0][3]=0.0;
	r[1][0]=sin(b); r[1][1]=cos(b) ; r[1][2]=0.0; r[1][3]=0.0;
	r[2][0]=0.0   ; r[2][1]=0.0    ; r[2][2]=1.0; r[2][3]=0.0;
	r[3][0]=0.0   ; r[3][1]=0.0    ; r[3][2]=0.0; r[3][3]=1.0;


   matmul4x4(a,r,w);
}

