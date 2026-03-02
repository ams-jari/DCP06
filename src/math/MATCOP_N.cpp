#include "stdafx.h"

void matcopy_n (double *a,short rows, short cols, double *b)
{
short i;


	for(i = 0; i <  rows*cols; i++)
		b[i] = a[i];


}
