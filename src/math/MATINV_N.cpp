#include "stdafx.h"
#include <stdlib.h>
#include "calc.h"
/*
show_matrix(char *p, double *ptr, short rows, short cols);
void matcopy_n (double *a,short rows, short cols, double *b);
*/
#define MAX_POINTS 20	

short matinv_n (double *a, short rows, short cols, double *b)
{
double aaa[100];
short i,j, k, l;
double apu, t;


	matcopy_n(a,rows, cols, aaa);

	for(i = 0; i < rows; i++)
	{
		for(j = 0 ; j < cols; j++)
		{
			b[i*cols+j] = 0;
		}

		b[i*cols+i] = 1;
	}

	for(j = 0 ; j < rows; j++)
	{
		for(i = j; i < rows; i++)
		{
			if(aaa[i*cols+j] != 0)  
			  goto hh;
		}

		return -1;

hh:
		for(k = 0 ; k < rows; k++)
		{
			apu =  aaa[j*cols+k]; 			//aaa(j, k)
			aaa[j*cols+k] = aaa[i*cols+k]; 	//aaa(j, k) = aaa(i, k)
			aaa[i*cols+k] = apu;  			//aaa(i, k) = apu
			apu = b[j*cols+k];				//apu = b(j, k)

			b[j*rows+k] = b[i*rows+k];		//b(j, k) = b(i, k)

			b[i*rows+k] = apu;				//b(i, k) = apu
		}

		t = 1 / aaa[j*cols+j] ;				//1 / aaa(j, j)

		for( k = 0; k < rows; k++)
		{
		   aaa[j*cols+k] = t * aaa[j*cols+k];
       	   b[j*cols +k] = t * b[j*cols +k];
		}

		for(l = 0 ; l < rows; l++)
		{
			if (l != j )
			{
				t = -aaa[l*cols+j];

				for(k = 0; k < rows; k++)
				{
					aaa[l*cols+k] = aaa[l*cols+k] + t * aaa[j*cols+k];
					b[l*cols+k] = b[l*cols+k] + t * b[j*cols+k];
				}
			}

		 }
	}

	return 1;
}
