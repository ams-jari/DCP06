#include "stdafx.h"

void matmul_n(double *a, short a_rows, short a_cols,
				  double *b, short b_rows, short b_cols,
				  double *c)
{

short i,j,k;
double ab;
double aa, bb;

		for( j=0; j < a_rows; j++)
		{

			for(i=0; i < b_cols; i++)
			{
			   ab = 0;

			   for(k=0; k < b_rows; k++)
			   {
					aa = *(a + j*a_cols + k);

					bb = *(b+(k*b_cols)+i);

					ab = ab +(aa*bb);
			   }

			   *(c+(j*b_cols)+i) = ab;
			}
		}
}
