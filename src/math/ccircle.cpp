#include "stdafx.h"
#include <math.h>
// #include "calc.h"

#define MAX_POINTS 20

/*
show_matrix(char *p, double *ptr, int rows, int cols);
void matcopy_n (double *a,int rows, int cols, double *b);
*/
 short matinv_n (double *a, short rows, short cols, double *b);

short center_of_circle(double x1,double y1,double x2,double y2,double x3,double y3,double *radius,double *cx,double *cy)
	{
		double l, m , n, a, b, c; 		//Tuntemattomat joista keskipioste ja s‰de lasketaan
		double in_table[3][3];
		double out_table[3][3];
		double c11, c12, c13, c21, c22, c23, c31, c32, c33;

		if ((y1-y2 != 0) && (y2-y3 != 0))
		{
			if( ((x1-x2)/(y1-y2))==((x2-x3)/(y2-y3)) )
				{
					return -3;
				}
		}
		else if (((x1==x2) && (x2==x3)) || ((y1==y2) && (y2==y3)))
				{
					return -3;
				}


			in_table[0][0] = x1;       //Asetetaan mitatut arvot matriisiin
			in_table[0][1] = y1;
			in_table[0][2] = 1.0;
			in_table[1][0] = x2;
			in_table[1][1] = y2;
			in_table[1][2] = 1.0;
			in_table[2][0] = x3;
			in_table[2][1] = y3;
			in_table[2][2] = 1.0;

	//kutsutaan k‰‰nteismatriisifunktiota
	if(matinv_n(&in_table[0][0],3,3, &out_table[0][0]) == 1)
	{
			c11=out_table[0][0];       //K‰‰nteismatriisin alkiot saavat arvot
			c12=out_table[0][1];
			c13=out_table[0][2];
			c21=out_table[1][0];
			c22=out_table[1][1];
			c23=out_table[1][2];
			c31=out_table[2][0];
			c32=out_table[2][1];
			c33=out_table[2][2];
	}
	else
			return -1;

	// Lasketaan s‰de ja keskipiste
		l =-(x1*x1 + y1*y1);
		m =-(x2*x2 + y2*y2);
		n =-(x3*x3 + y3*y3);

		a = c11*l + c12*m + c13*n;
		b = c21*l + c22*m + c23*n;
		c = c31*l + c32*m + c33*n;

		*cx = a / -2.0;
		*cy = b / -2.0;
		*radius = sqrt(*cx * *cx + *cy * *cy -c);
	return 1;
	}
