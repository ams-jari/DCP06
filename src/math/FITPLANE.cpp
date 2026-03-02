#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc.h"
#include <string.h>

//#include <bmm_exp.hpp>
//#include <ldr_exp.hpp>

//#undef PMP05
//#include "pmp05.hpp"
/*
#ifndef LEICA_DCP05_1100
	#include <tok2str.hpp>
#endif

#include INC_DCP05_HDR
*/
//short CheckEscKey(void);
//char *strbtrim(char *s);


/*
void show_matrixn(char *title, double *a, short rows, short cols);

show_matrix(char *p, double *ptr, short rows, short cols);
matvecmul_n(	double *a, short a_rows, short a_cols,
				double *b,
				double *c);

void matmul_n(double *a, short a_rows, short a_cols,
				  double *b, short b_rows, short b_cols,
				  double *c);
short matinv_n (double *a,short rows, short cols, double *b);
void mattra_n (double *a,short a_rows,short a_cols, double *b);
short equation_of_plane(struct ams_vector *a, struct ams_vector *b,struct ams_vector *c, struct plane *d);
double vector_product(struct ams_vector *a,struct ams_vector *b,struct ams_vector *c);
double vector_length(struct ams_vector *a);
*/
/************************************************************************



*************************************************************************/

short FitPlane (double *m_mat,short points, double *a, double *b, double *c)
{

double dist,dist2,len;
short ret=1,pno;
short i, j,ii,it;
struct ams_vector a1,b1,c1,aa,bb,cc,ccc,N, NTOT;
struct plane d;
//int pit;
//char tempstr0[100];
//char tempstr1[100];
//char tempstr2[100];
//char Temp[45];
//APP_ID AppId;
		/*
	   ( void ) LDR_GetApplHandle ( AppId ) ;
		sprintf(tempstr0," %-26.26s ", " ");
		BMM_WriteText(2,2*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(2,6*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
		

        #ifndef LEICA_DCP05_1100
		sprintf(tempstr0," %-26.26s ",TextTokenToString(AppId, TXT_CALCULATING_TOK));

		sprintf(tempstr1," %-26.26s ", TextTokenToString(AppId, TXT_ITERATION_TOK));

        #else
        char temp[41];
		sprintf(tempstr0,"%-26.26s",getTokenString(AppId,TXT_CALCULATING_TOK,40,temp));
		sprintf(tempstr1,"%-26.26s", getTokenString(AppId,TXT_ITERATION_TOK,40,temp));
        #endif
		strbtrim(tempstr1);
		pit = strlen(tempstr1);

        #ifndef LEICA_DCP05_1100
		sprintf(tempstr2," %-26.26s ",TextTokenToString(AppId, TXT_PRESS_ESC_TOK));
        #else
		sprintf(tempstr2,"%-26.26s",getTokenString(AppId,TXT_PRESS_ESC_TOK,40,temp));
        #endif


		BMM_WriteText(1,3*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,4*8,tempstr1, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,5*8,tempstr2, BMM_LCD_INV, CHARSET_GEO);
		*/

	// Tason 1.piste
	a1.x  = *(m_mat+0*3+0); 	// x
	a1.y  = *(m_mat+0*3+1);  // Y
	a1.z  = *(m_mat+0*3+2);	// z

	// Tason 2.piste
	b1.x  = *(m_mat+1*3+0); 	// x
	b1.y  = *(m_mat+1*3+1);  // Y
	b1.z  = *(m_mat+1*3+2);	// z

	// Tason 3.piste
	c1.x  = *(m_mat+2*3+0); 	// x
	c1.y  = *(m_mat+2*3+1);  // Y
	c1.z  = *(m_mat+2*3+2);	// z

	equation_of_plane(&a1, &b1, &c1, &d);

	if(d.nx == 0.0 && d.ny == 0.0 && d.nz == 0.0)
	{
		ret = -1;
		*a = 0.0;
		*b = 0.0;
		*c = 0.0;
		return ret;
	}

	// Lasketaan pisin et„isyys pisteest„ 1

	dist=0.0;

	//1.piste

	N.x = 0.0;
	N.y = 0.0;
	N.z = 0.0;

	NTOT.x = 0.0;
	NTOT.y = 0.0;
	NTOT.z = 0.0;

	it = 0;

	for(i = 0; i < points; i++)
	{

		it++;

		//Iteration text
		/*
		sprintf(Temp,"%-*.*s %-*d", pit,pit, tempstr1, 26-pit-1,it);
		BMM_WriteText(1,4*8,Temp,BMM_LCD_INV, CHARSET_GEO);
		*/
		N.x = 0.0;
		N.y = 0.0;
		N.z = 0.0;

		// Valitaan Pisin et„isyys pisteest„ i

		for(ii=0;ii<points;ii++)
		{

		if(ii != i)
		{
			pno = ii;

			/*x1  = *(m_mat+i*3+0); 	// x
			y1  = *(m_mat+i*3+1);  	// Y
			z1  = *(m_mat+i*3+2);	// z


			x  = *(m_mat+ii*3+0); 	// x
			y  = *(m_mat+ii*3+1);  	// Y
			z  = *(m_mat+ii*3+2);	// z

			dist1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)+(z-z1)*(z-z1));

			if(ii==0)
			{
				pno=0;
				dist = dist1;
			}
			else
			{
				if (dist1 > dist)
				{
					dist=dist1;
					pno=ii;
				}
			}

			*/


		for(j=0;j<points;j++)
		{

			if(j != pno && i != j)
			{
				aa.x =  *(m_mat+(pno)*3+0) - *(m_mat+i*3+0) ; 	// x
				aa.y =  *(m_mat+(pno)*3+1) - *(m_mat+i*3+1); 	// x
				aa.z =  *(m_mat+(pno)*3+2) - *(m_mat+i*3+2); 	// x

				bb.x =  *(m_mat+j*3+0) - *(m_mat+i*3+0);
				bb.y =  *(m_mat+j*3+1) - *(m_mat+i*3+1);
				bb.z =  *(m_mat+j*3+2) - *(m_mat+i*3+2);


				len = vector_product(&aa,&bb,&cc);

				if(len == 0.0)
				{
					ret = -1;
					*a = 0.0;
					*b = 0.0;
					*c = 0.0;
					return ret;
				}

				// TESTAUS

				ccc.x = d.nx + cc.x;
				ccc.y = d.ny + cc.y;
				ccc.z = d.nz + cc.z;

				// kerrotaan takaisin pituudella

				cc.x = cc.x * len;
				cc.y = cc.y * len;
				cc.z = cc.z * len;

				dist2 = vector_length(&ccc);

				if(dist2 <= 0.9)
				{
				  cc.x = -cc.x;
				  cc.y = -cc.y;
				  cc.z = -cc.z;
				}

				N.x = N.x + cc.x;
				N.y = N.y + cc.y;
				N.z = N.z + cc.z;
			}
		}
		}
		}
		len = vector_length(&N);

		N.x = N.x; // / len;
		N.y = N.y;// / len;
		N.z = N.z;// / len;

		NTOT.x = NTOT.x + N.x;
		NTOT.y = NTOT.y + N.y;
		NTOT.z = NTOT.z + N.z;

		//Iteration text
		// sprintf(Temp, %-*.*s %-*d ", pit,pit, tempstr1, 26-pit-1,it);
		//BMM_WriteText(2,4,Temp,BMM_LCD_INV, CHARSET_GEO);

		// Add 24.9.1996
		/*
		if(CheckEscKey()==TRUE)
		{
				ret = -2;
				*a = 0.0;
				*b = 0.0;
				*c = 0.0;
				goto end;
		}
		*/
	}

	len = vector_length(&NTOT);

	if (len == 0.0)
	{
		*a = 0.0;
		*b = 0.0;
		*c = 0.0;
		return -1;
	}

	NTOT.x = NTOT.x /len;
	NTOT.y = NTOT.y /len;
	NTOT.z = NTOT.z /len;


   *a = NTOT.x;
   *b = NTOT.y;
   *c = NTOT.z;

//end:

	return ret;

}
