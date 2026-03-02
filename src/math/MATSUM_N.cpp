#include "stdafx.h"



void matsum_n (double *a, short a_rows, short a_cols, double  *b, double *c)
{
//Dim rows%, cols%
short i, j;


	for(i = 0; i < a_rows; i++)
	{
		for(j = 0; j < a_cols; j++)
		{
		  c[i*a_cols + j] = a[i*a_cols + j] + b[i*a_cols+ j];
		}
	}

}
/*

Sub matsum1 (a() As Double, b() As Double, c() As Double)
Dim rows%, cols%
Dim i%, j%, k%

	rows = UBound(a)


	For i = 0 To rows

		  c(i) = a(i) + b(i)

	Next i


End Sub
*/
