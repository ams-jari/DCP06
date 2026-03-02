#include "stdafx.h"
void reset_matrix (double *a, short a_rows, short a_cols)
{
short i;


	for(i = 0; i < a_cols*a_rows; i++)
	{
			a[i] = 0.0;
	}

}
