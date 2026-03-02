#include "stdafx.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
//#include <string.h>
//#include <conio.h>


//#include <bmm_exp.hpp>
//#include <ldr_exp.hpp>


#undef PMP05
//#include "i:\dcp05tps\pmp05.hpp"
//#include "i:\dcp05tps\math\calc.h"

//#include "pmp05.hpp"
#include "calc.h"

#ifndef LEICA_DCP05_1100
//	#include <tok2str.hpp>
#endif

//#include INC_DCP05_HDR

#define PII 3.1415926535897932

/************************************************************************



*************************************************************************/

short FitLine(double *p_mat, short points, double *a, double *b, double *c)
{

short i,j,ret=1;
double v[20*3];
//double *k_table;
double k_table[20*3];

double kx, ky, kz; //xtar;
short it;
double kx_tot=0.0, ky_tot = 0.0, kz_tot = 0.0;
double kx_av, ky_av,kz_av, dist, dist_tot=0.0;
//int pit;
//char tempstr0[100];
//char tempstr1[100];
//char tempstr2[100];
//char Temp[45];
//APP_ID AppId;
		
//	   ( void ) LDR_GetApplHandle ( AppId ) ;
//		sprintf(tempstr0," %-26.26s " , " ");
//		BMM_WriteText(2,2*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
//		BMM_WriteText(2,6*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);

//       #ifndef LEICA_DCP05_1100
//		sprintf(tempstr0," %-26.26s ",TextTokenToString(AppId, TXT_CALCULATING_TOK));
//		sprintf(tempstr1," %-26.26s ",TextTokenToString(AppId, TXT_ITERATION_TOK));
//        #else
//        char temp[41];
//        sprintf(tempstr0,"%-26.26s",	getTokenString(AppId,TXT_CALCULATING_TOK,40,temp));
//        sprintf(tempstr1,"%-26.26s",	getTokenString(AppId,TXT_ITERATION_TOK,40,temp));
//        #endif
//		strbtrim(tempstr1);
//		pit = strlen(tempstr1);

//        #ifndef LEICA_DCP05_1100
//		sprintf(tempstr2," %-26.26s ",TextTokenToString(AppId, TXT_PRESS_ESC_TOK));
//        #else
//        char temp1[41];
//        sprintf(tempstr2,"%-26.26s",	getTokenString(AppId,TXT_PRESS_ESC_TOK,40,temp1));
//        #endif

//		BMM_WriteText(1/*2*/,3*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
//		BMM_WriteText(1/*2*/,4*8,tempstr1, BMM_LCD_INV, CHARSET_GEO);
//		BMM_WriteText(1/*2*/,5*8,tempstr2, BMM_LCD_INV, CHARSET_GEO);
		


	//v = (double *) calloc(points-1,sizeof(double) *3);


	//k_table = (double *) calloc(points-1,sizeof(double)*2);
	//k_table = (double *) malloc((points-1)*sizeof(double)*2);


	if(v == NULL || k_table == NULL )
	{

		*a = 0;
		*b = 0;
		*c = 0;
		ret = -1;
		goto end;
	}


	//show_matrixnf("p_mat",p_mat, points, 3);

	for(i=0; i < points-1; i++)
	{
		for(j = 0; j < 3; j++)

			*(v+i*3+j) = *(p_mat+(i*3+3)+j)  - *(p_mat+i*3+j);

	}

	it = 0;
	for(i=0; i < points-1; i++)
	{
		it++;

		//Iteration text
//		sprintf(Temp,"%-*.*s %-*d", pit,pit, tempstr1, 26-pit-1,it);
//		BMM_WriteText(1/*2*/,4*8,Temp,BMM_LCD_INV, CHARSET_GEO);

		dist = sqrt(v[i*3+0]* v[i*3+0] + v[i*3+1]* v[i*3+1] + v[i*3+2]* v[i*3+2]);

		if(dist == 0.0)
		{
			*a = 0;
			*b = 0;
			*c = 0;
			ret = -1;
			goto end;
		}

		dist_tot = dist_tot + dist;

		kx = v[i*3+0] / dist; // X
		ky = v[i*3+1] / dist; // Y
		kz = v[i*3+2] / dist; // Z

		kx = dist * kx;
		ky = dist * ky;
		kz = dist * kz;

		/***********************/

		kx_tot = kx_tot+kx;
		ky_tot = ky_tot+ky;
		kz_tot = kz_tot+kz;
		/*
		if(CheckEscKey()==TRUE)
		{
				ret = -2;
				return (short) ret;
		}
		*/
	}

	kx_av = kx_tot / dist_tot;
	ky_av = ky_tot / dist_tot;
	kz_av = kz_tot / dist_tot;

	dist = sqrt(kx_av * kx_av + ky_av * ky_av + kz_av * kz_av);

	if(dist == 0.0)
	{
		*a = 0;
		*b = 0;
		*c = 0;
		ret = -1;
	}
	else
	{
		*a = kx_av / dist;
		*b = ky_av / dist;
		*c = kz_av / dist;
	}

	ret = 1;

end:

	return (short) ret;
}



