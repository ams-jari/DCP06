#include "stdafx.h"


void mattra_n (double *a, short a_rows, short a_cols, double *b)
{
short i, j;

	for(i = 0; i < a_rows; i++)
	{
		for(j = 0; j < a_cols; j++)
		{

			b[j*a_rows+i] = a[i*a_cols+ j];

		}
	}

}
