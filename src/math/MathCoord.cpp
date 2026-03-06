/**
 * MathCoord.cpp - 6-parameter coordinate transformation (consolidated from COORD6TR)
 */
#include "stdafx.h"
#include <math.h>
#include "calc.h"

short CoordTransf6(double *mea, double *nom, short points, double *par, double *M, double *k, double *Zdd)
{
	double wmea = 1.0, wnom = 1.0;
	double A[3][6], Qe[3][3], We[3][3];
	double MD1[3][3], MD2[3][3], MD3[3][3];
	double apu[3][3];
	double B[3][6], N[6][6], apu36[3][6];
	double apu63[6][3];
	double I3[3][3], x[3];
	double t[6], apu31[3];
	double f1[3], f2[3], f[3];
	double D[6], invNN[6][6];
	double NN[6][6], tt[6], ff[3];
	short r;
	double z02, vtWv;
	double apu66[6][6];
	double rx, ry, rz;
	short i, j, it, ret;

	ret = 1;

	I3[0][0] = 1.0; I3[0][1] = 0.0; I3[0][2] = 0.0;
	I3[1][0] = 0.0; I3[1][1] = 1.0; I3[1][2] = 0.0;
	I3[2][0] = 0.0; I3[2][1] = 0.0; I3[2][2] = 1.0;

	rx = 0.0; ry = 0.0; rz = 0.0;
	par[0] = rx; par[1] = ry; par[2] = rz;

	k[0] = 0.0; k[1] = 0.0; k[2] = 0.0;
	par[3] = k[0]; par[4] = k[1]; par[5] = k[2];

	M[0] = 1.0; M[1] = 0.0; M[2] = 0.0;
	M[3] = 0.0; M[4] = 1.0; M[5] = 0.0;
	M[6] = 0.0; M[7] = 0.0; M[8] = 1.0;

	z02 = 1000;
	it = 0;

	do {
		it = it + 1;

		reset_matrix(&NN[0][0], 6, 6);
		reset_matrix(&ff[0], 3, 1);
		reset_matrix(&tt[0], 6, 1);

		A[0][0] = M[0];  A[0][1] = M[1];  A[0][2] = M[2];
		A[0][3] = -1.0; A[0][4] = 0.0;  A[0][5] = 0.0;
		A[1][0] = M[3];  A[1][1] = M[4];  A[1][2] = M[5];
		A[1][3] = 0.0;  A[1][4] = -1.0; A[1][5] = 0.0;
		A[2][0] = M[6];  A[2][1] = M[7];  A[2][2] = M[8];
		A[2][3] = 0.0;  A[2][4] = 0.0;  A[2][5] = -1.0;

		Qe[0][0] = wmea + wnom; Qe[0][1] = 0.0; Qe[0][2] = 0.0;
		Qe[1][0] = 0.0; Qe[1][1] = wmea + wmea; Qe[1][2] = 0.0;
		Qe[2][0] = 0.0; Qe[2][1] = 0.0; Qe[2][2] = wmea + wnom;

		We[0][0] = 1.0 / (wmea + wnom); We[0][1] = 0.0; We[0][2] = 0.0;
		We[1][0] = 0.0; We[1][1] = 1.0 / (wmea + wnom); We[1][2] = 0.0;
		We[2][0] = 0.0; We[2][1] = 0.0; We[2][2] = 1.0 / (wmea + wnom);

		apu[0][0] = 0.0; apu[0][1] = 0.0; apu[0][2] = 0.0;
		apu[1][0] = 0.0; apu[1][1] = 0.0; apu[1][2] = 1.0;
		apu[2][0] = 0.0; apu[2][1] = -1.0; apu[2][2] = 0.0;
		matmul_n(M, 3, 3, &apu[0][0], 3, 3, &MD1[0][0]);

		apu[0][0] = 0.0; apu[0][1] = 0.0; apu[0][2] = -cos(rz);
		apu[1][0] = 0.0; apu[1][1] = 0.0; apu[1][2] = sin(rz);
		apu[2][0] = cos(rz); apu[2][1] = -sin(rz); apu[2][2] = 0.0;
		matmul_n(&apu[0][0], 3, 3, &M[0], 3, 3, &MD2[0][0]);

		apu[0][0] = 0.0; apu[0][1] = 1.0; apu[0][2] = 0.0;
		apu[1][0] = -1.0; apu[1][1] = 0.0; apu[1][2] = 0.0;
		apu[2][0] = 0.0; apu[2][1] = 0.0; apu[2][2] = 0.0;
		matmul_n(&apu[0][0], 3, 3, &M[0], 3, 3, &MD3[0][0]);

		for (i = 0; i < points; i++) {
			x[0] = mea[i * 3 + 0];
			x[1] = mea[i * 3 + 1];
			x[2] = mea[i * 3 + 2];

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

			matmul_n(&We[0][0], 3, 3, &B[0][0], 3, 6, &apu36[0][0]);
			mattra_n(&B[0][0], 3, 6, &apu63[0][0]);
			matmul_n(&apu63[0][0], 6, 3, &apu36[0][0], 3, 6, &N[0][0]);

			f1[0] = nom[i * 3 + 0]; f1[1] = nom[i * 3 + 1]; f1[2] = nom[i * 3 + 2];
			f2[0] = M[0] * mea[i * 3 + 0] + M[1] * mea[i * 3 + 1] + M[2] * mea[i * 3 + 2];
			f2[1] = M[3] * mea[i * 3 + 0] + M[4] * mea[i * 3 + 1] + M[5] * mea[i * 3 + 2];
			f2[2] = M[6] * mea[i * 3 + 0] + M[7] * mea[i * 3 + 1] + M[8] * mea[i * 3 + 2];

			f[0] = f1[0] - f2[0] - k[0];
			f[1] = f1[1] - f2[1] - k[1];
			f[2] = f1[2] - f2[2] - k[2];

			matmul_n(&We[0][0], 3, 3, &f[0], 3, 1, &apu31[0]);
			matmul_n(&apu63[0][0], 6, 3, &apu31[0], 3, 1, &t[0]);

			matsum_n(&N[0][0], 6, 6, &NN[0][0], &NN[0][0]);
			matsum_n(t, 6, 1, tt, tt);
			matsum_n(f, 3, 1, ff, ff);
		}

		if (matinv_n(&NN[0][0], 6, 6, &invNN[0][0]) == -1) {
			return -1;
		}

		matmul_n(&NN[0][0], 6, 6, &invNN[0][0], 6, 6, &apu66[0][0]);
		matmul_n(&invNN[0][0], 6, 6, &tt[0], 6, 1, &D[0]);

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
		r = points * 3 - 6;
		z02 = vtWv / r;

		for (i = 0; i <= 5; i++) {
			for (j = 0; j <= 5; j++) {
				Zdd[i * 6 + j] = z02 * invNN[i][j];
			}
		}
	} while (z02 > .000000001 && it < 500);

	return ret;
}
