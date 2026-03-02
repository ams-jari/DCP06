#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
/*
#include <bmm_exp.hpp>
#include <ldr_exp.hpp>


#undef PMP05
#include "pmp05.hpp"
#include INC_DCP05_HDR

#ifndef LEICA_DCP05_1100
	#include <tok2str.hpp>
#endif

short CheckEscKey(void);
char *strbtrim(char *s);
*/
#include "calc.h"
/*
void matsum_n (double *a, short a_rows, short a_cols, double  *b, double *c);
void reset_matrix (double *a, short a_rows, short a_cols);
void mattra_n (double *a,short a_rows,short a_cols, double *b);
short matinv_n (double *a,short rows, short cols, double *b);
void matmul_n(double *a, short a_rows, short a_cols,
				  double *b, short b_rows, short b_cols,
				  double *c);


matvecmul_n(	double *a, short a_rows, short a_cols,
				double *b,
				double *c);

show_matrix(char *p, double *ptr, short rows, short cols);
*/

/************************************************************************



*************************************************************************/

short CoordTransf6 (	double *mea, double *nom,
					short points,
					double *par, double *M,
					double *k, double *Zdd)

{
// versio 10.2.1995 RHe & POj

double wmea=1.0, wnom=1.0;
//Static A(2, 5) As Double, Qe(2, 2) As Double, We(2, 2) As Double

double A[3][6], Qe[3][3],We[3][3];

//Static MD1(2, 2) As Double, MD2(2, 2) As Double, MD3(2, 2) As Double

double MD1[3][3], MD2[3][3], MD3[3][3];


//Static apu(2, 2) As Double, apu1(2, 2) As Double
double apu[3][3]; //, apu1[3][3];


//Static B(2, 5) As Double, N(5, 5) As Double, apu36(2, 5) As Double'
double B[3][6], N[6][6], apu36[3][6];

//Static apu63(5, 2) As Double
double apu63[6][3];


//Static I3(2, 2) As Double, x(2) As Double
double I3[3][3], x[3];

//Static t(5) As Double, apu31(3) As Double
double t[6], apu31[3];

//Static f1(2) As Double, f2(2) As Double, f(2) As Double
double f1[3], f2[3], f[3];

//Static D(5) As Double, invNN(5, 5) As Double
double D[6], invNN[6][6];

//Static Qdd(5, 5) As Double
//double Qdd[6][6];


//Static NN(5, 5) As Double, tt(5) As Double, ff(2) As Double
double NN[6][6], tt[6], ff[3];


short r;
double z02, vtWv;

//Static apu66(5, 5) As Double, ms
double apu66[6][6];

double rx, ry, rz;
short i, j,  it, ret;
//char Temp[45];
//int pit;
//char tempstr0[100];
//char tempstr1[100];
//char tempstr2[100];
//APP_ID AppId;

		/*
	   ( void ) LDR_GetApplHandle ( AppId ) ;

        #ifndef LEICA_DCP05_1100
		sprintf(tempstr0," %-26.26s ",TextTokenToString(AppId, TXT_CALCULATING_TOK));

		sprintf(tempstr1," %-26.26s ",TextTokenToString(AppId, TXT_ITERATION_TOK));
        #else
             char temp[41];
    		sprintf(tempstr0,"%-26.26s",getTokenString(AppId,TXT_CALCULATING_TOK,40,temp));
	    	sprintf(tempstr1,"%-26.26s",getTokenString(AppId,TXT_ITERATION_TOK,40,temp));
        #endif
		strbtrim(tempstr1);
		pit = strlen(tempstr1);

        #ifndef LEICA_DCP05_1100
		    sprintf(tempstr2," %-26.26s ",TextTokenToString(AppId, TXT_PRESS_ESC_TOK));
        #else
	    	sprintf(tempstr2,"%-26.26s",getTokenString(AppId,TXT_PRESS_ESC_TOK,40,temp));
        #endif

		sprintf(Temp,"%-26.26s", "");
		BMM_WriteText(1,2*8,Temp, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,3*8,tempstr0, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,4*8,tempstr1, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,5*8,tempstr2, BMM_LCD_INV, CHARSET_GEO);
		BMM_WriteText(1,6*8,Temp, BMM_LCD_INV, CHARSET_GEO);
		*/
		ret = 1;

		// Yksikkövektori
		I3[0][0] = 1.0; I3[0][1] = 0.0; I3[0][2] = 0.0;
		I3[1][0] = 0.0; I3[1][1] = 1.0; I3[1][2] = 0.0;
		I3[2][0] = 0.0; I3[2][1] = 0.0; I3[2][2] = 1.0;

		// Parametrien lähtöarvot

		// Kiertoparametrit
		rx = 0.0; ry = 0.0; rz = 0.0;
		par[0] = rx; par[1] = ry; par[2] = rz;

		// Translaatioparametrit
		k[0] = 0.0; k[1] = 0.0; k[2] = 0.0;
		par[3] = k[0]; par[4] = k[1]; par[5] = k[2];

		// Rotaatiomatriisin lähtöarvo 3 x 3
		M[0] = 1.0; M[1] = 0.0; M[2] = 0.0;
		M[3] = 0.0; M[4] = 1.0; M[5] = 0.0;
		M[6] = 0.0; M[7] = 0.0; M[8] = 1.0;

		z02 = 1000;

		it = 0;


	   do
	   {

		it = it + 1;

		// ************************
		// Iteration text
		// ************************

		//sprintf(Temp,"%-s %-*d", tempstr1,26-pit-1,it);
		//BMM_WriteText(1,4*8,Temp, BMM_LCD_INV, CHARSET_GEO);


		reset_matrix( &NN[0][0], 6, 6);  // 6 x 6

		//reset_vector ff()
		reset_matrix( &ff[0],3,1);  // 3 x 1

		//reset_vector tt()
		reset_matrix( &tt[0], 6,1); // 6 x 1


		A[0][0] = M[0];	A[0][1] = M[1]; A[0][2] = M[2];
		A[0][3] = -1.0;	A[0][4] = 0.0; 	A[0][5] = 0.0 ;


		A[1][0] = M[3]; A[1][1] = M[4]; A[1][2] = M[5];
		A[1][3] = 0.0; A[1][4] = -1.0; A[1][5] = 0.0;

		A[2][0] = M[6]; A[2][1] = M[7]; A[2][2] = M[8];
		A[2][3] = 0.0; A[2][4] = 0.0; A[2][5] = -1.0;

		Qe[0][0] = wmea + wnom; Qe[0][1] = 0.0; Qe[0][2] = 0.0;
		Qe[1][0] = 0.0; Qe[1][1] = wmea + wmea; Qe[1][2] = 0.0;
		Qe[2][0] = 0.0; Qe[2][1] = 0.0; Qe[2][2] = wmea + wnom;

		We[0][0] = 1.0 / (wmea + wnom); We[0][1] = 0.0; We[0][2] = 0.0;
		We[1][0] = 0.0; We[1][1] = 1.0 / (wmea + wnom); We[1][2] = 0.0;
		We[2][0] = 0.0; We[2][1] = 0.0; We[2][2] = 1.0 / (wmea + wnom);

		apu[0][0] = 0.0; apu[0][1] = 0.0; apu[0][2] = 0.0;
		apu[1][0] = 0.0; apu[1][1] = 0.0; apu[1][2] = 1.0;
		apu[2][0] = 0.0; apu[2][1] = -1.0; apu[2][2] = 0.0;

		matmul_n( M,3,3, &apu[0][0],3,3, &MD1[0][0]);

		//show_matrix("M",M,3,3);
		//show_matrix("apu",apu,3,3);
		//show_matrix("MD1",MD1,3,3);

		apu[0][0] = 0.0; apu[0][1] = 0.0; apu[0][2] = -cos(rz);
		apu[1][0] = 0.0; apu[1][1] = 0.0; apu[1][2] = sin(rz);
		apu[2][0] = cos(rz); apu[2][1] = -sin(rz); apu[2][2] = 0.0;

		matmul_n( &apu[0][0],3,3, &M[0],3,3, &MD2[0][0]);

		apu[0][0] = 0.0; apu[0][1] = 1.0; apu[0][2] = 0.0;
		apu[1][0] = -1.0; apu[1][1] = 0.0; apu[1][2] = 0.0;
		apu[2][0] = 0.0; apu[2][1] = 0.0; apu[2][2] = 0.0;

		matmul_n( &apu[0][0],3,3, &M[0],3,3, &MD3[0][0]);

		//rows1 = UBound(nomTms, 1)

		for(i=0; i < points; i++)
		{

			x[0] = mea[i*3+ 0];
			x[1] = mea[i*3+1];
			x[2] = mea[i*3+ 2];

			B[0][0] = MD1[0][0] * x[0] + MD1[0][1] * x[1] + MD1[0][2] * x[2];
			B[1][0] = MD1[1][0] * x[0] + MD1[1][1] * x[1] + MD1[1][2] * x[2];
			B[2][0] = MD1[2][0] * x[0] + MD1[2][1] * x[1] + MD1[2][2] * x[2];

			B[0][1] = MD2[0][0] * x[0] + MD2[0][1] * x[1] + MD2[0][2] * x[2];
			B[1][1] = MD2[1][0] * x[0] + MD2[1][1] * x[1] + MD2[1][2] * x[2];
			B[2][1] = MD2[2][0] * x[0] + MD2[2][1] * x[1] + MD2[2][2] * x[2];

			B[0][2] = MD3[0][0] * x[0] + MD3[0][1] * x[1] + MD3[0][2] * x[2];
			B[1][2] = MD3[1][0] * x[0] + MD3[1][1] * x[1] + MD3[1][2] * x[2];
			B[2][2] = MD3[2][0] * x[0] + MD3[2][1] * x[1] + MD3[2][2] * x[2];

			B[0][3] = 1.0; B[1][3] = 0.0; B[2][3] = 0.0;
			B[0][4] = 0.0; B[1][4] = 1.0; B[2][4] = 0.0;
			B[0][5] = 0.0; B[1][5] = 0.0; B[2][5] = 1.0;

			matmul_n( &We[0][0],3,3, &B[0][0],3,6, &apu36[0][0]);
			mattra_n( &B[0][0], 3,6, &apu63[0][0]);

			matmul_n (&apu63[0][0],6,3, &apu36[0][0],3,6, &N[0][0]);

			f1[0] = nom[i*3+0]; f1[1] = nom[i*3+1]; f1[2] = nom[i*3+2];

			f2[0] = M[0] * mea[i*3+ 0] + M[1] * mea[i*3+1] + M[2] * mea[i*3+ 2];
			f2[1] = M[3] * mea[i*3+ 0] + M[4] * mea[i*3+1] + M[5] * mea[i*3+2];
			f2[2] = M[6] * mea[i*3+ 0] + M[7] * mea[i*3+1] + M[8] * mea[i*3+2];

			f[0] = f1[0] - f2[0] - k[0];
			f[1] = f1[1] - f2[1] - k[1];
			f[2] = f1[2] - f2[2] - k[2];

			//matvecmul We(), f(), apu31()
			matmul_n( &We[0][0],3,3, &f[0],3,1, &apu31[0]);
			//matvecmul_n(&We[0][0],3,3, &f[0], &apu31[0]);

			//matvecmul apu63(), apu31(), t()
			matmul_n( &apu63[0][0],6,3, &apu31[0], 3,1, &t[0]);
			//matvecmul_n( &apu63[0][0],6,3, &apu31[0], &t[0]);


			matsum_n(&N[0][0],6,6,&NN[0][0], &NN[0][0]);

			matsum_n(t, 6,1, tt, tt);
			matsum_n(f, 3,1, ff, ff);

	}

		   //	show_matrix("NN", &NN[0][0], 6, 6);



			if(matinv_n(&NN[0][0],6,6, &invNN[0][0]) == -1)
			{
				return -1;

			}

			//show_matrix("invNN", &invNN[0][0], 6, 6);

			matmul_n(&NN[0][0],6,6, &invNN[0][0],6,6, &apu66[0][0]);

			//matvecmul invNN(), tt(), D()
			matmul_n(&invNN[0][0], 6,6, &tt[0],6,1, &D[0]);
			//matvecmul_n(&invNN[0][0], 6,6, &tt[0], &D[0]);


			par[0] = par[0] + D[0];
			par[1] = par[1] + D[1];
			par[2] = par[2] + D[2];
			par[3] = par[3] + D[3];
			par[4] = par[4] + D[4];
			par[5] = par[5] + D[5];

			rx = par[0];
			ry = par[1];
			rz = par[2];
			k[0] = par[3];
			k[1] = par[4];
			k[2] = par[5];

			M[0] = cos(rz) * cos(ry);
			M[1] = cos(rz) * sin(ry) * sin(rx) + sin(rz) * cos(rx);
			M[2] = -cos(rz) * sin(ry) * cos(rx) + sin(rz) * sin(rx);
			M[3] = -sin(rz) * cos(ry);
			M[4] = -sin(rz) * sin(ry) * sin(rx) + cos(rz) * cos(rx);
			M[5] = sin(rz) * sin(ry) * cos(rx) + sin(rx) * cos(rz);
			M[6] = sin(ry);
			M[7] = -cos(ry) * sin(rx);
			M[8] = cos(ry) * cos(rx);

			vtWv = (ff[0] * ff[0] + ff[1] * ff[1] + ff[2] * ff[2]) / (wmea + wnom);

			//r = ((UBound(nomTms, 1) + 1) * 3) - 6;
			r = points * 3 -6;


			z02 = vtWv / r;

			for(i = 0; i<= 5; i++)
			{
				for( j = 0; j <=5; j++)
				{
				  // Zdd(i, j) = z02 * invNN(i, j)
				  Zdd[i*6 + j] = z02 * invNN[i][j];

				}
			}
		/*
		if(CheckEscKey()==TRUE)
		{
				ret = -2;
				break;
		}
		*/		

	  }while( z02 > .000000001 && it < 500); // And cancel = False

	return ret;
}

