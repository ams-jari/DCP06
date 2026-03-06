/**
 * MathMatrix.cpp - Matrix operations (consolidated from MATCOP_N, MATMUL_N, MATSUM_N, MATTRA_N, MATINV_N, Minv4x4, MMUL4X4, RESETMAT)
 */
#include "stdafx.h"
#include <stdlib.h>
#include "calc.h"

#define MAX_POINTS 20

void matcopy_n(double *a, short rows, short cols, double *b)
{
	short i;
	for (i = 0; i < rows * cols; i++)
		b[i] = a[i];
}

void matmul_n(double *a, short a_rows, short a_cols, double *b, short b_rows, short b_cols, double *c)
{
	short i, j, k;
	double ab, aa, bb;
	for (j = 0; j < a_rows; j++) {
		for (i = 0; i < b_cols; i++) {
			ab = 0;
			for (k = 0; k < b_rows; k++) {
				aa = *(a + j * a_cols + k);
				bb = *(b + (k * b_cols) + i);
				ab = ab + (aa * bb);
			}
			*(c + (j * b_cols) + i) = ab;
		}
	}
}

void matsum_n(double *a, short a_rows, short a_cols, double *b, double *c)
{
	short i, j;
	for (i = 0; i < a_rows; i++) {
		for (j = 0; j < a_cols; j++) {
			c[i * a_cols + j] = a[i * a_cols + j] + b[i * a_cols + j];
		}
	}
}

void mattra_n(double *a, short a_rows, short a_cols, double *b)
{
	short i, j;
	for (i = 0; i < a_rows; i++) {
		for (j = 0; j < a_cols; j++) {
			b[j * a_rows + i] = a[i * a_cols + j];
		}
	}
}

short matinv_n(double *a, short rows, short cols, double *b)
{
	double aaa[100];
	short i, j, k, l;
	double apu, t;
	matcopy_n(a, rows, cols, aaa);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			b[i * cols + j] = 0;
		}
		b[i * cols + i] = 1;
	}
	for (j = 0; j < rows; j++) {
		for (i = j; i < rows; i++) {
			if (aaa[i * cols + j] != 0)
				goto hh;
		}
		return -1;
hh:
		for (k = 0; k < rows; k++) {
			apu = aaa[j * cols + k];
			aaa[j * cols + k] = aaa[i * cols + k];
			aaa[i * cols + k] = apu;
			apu = b[j * cols + k];
			b[j * rows + k] = b[i * rows + k];
			b[i * rows + k] = apu;
		}
		t = 1 / aaa[j * cols + j];
		for (k = 0; k < rows; k++) {
			aaa[j * cols + k] = t * aaa[j * cols + k];
			b[j * cols + k] = t * b[j * cols + k];
		}
		for (l = 0; l < rows; l++) {
			if (l != j) {
				t = -aaa[l * cols + j];
				for (k = 0; k < rows; k++) {
					aaa[l * cols + k] = aaa[l * cols + k] + t * aaa[j * cols + k];
					b[l * cols + k] = b[l * cols + k] + t * b[j * cols + k];
				}
			}
		}
	}
	return 1;
}

short matinv4x4(double a[4][4], double (*b)[4][4])
{
	short i, j, i1, j1, i2, j2, k1, k2, l1, l2;
	short ij, it, ijt, test, lask;
	double d[4][4], det;
	for (i = 0; i <= 3; i++) {
		for (j = 0; j <= 3; j++) {
			lask = 0;
			d[j][i] = 0.0;
			ijt = 1;
			for (i1 = 0; i1 <= 1; i1++) {
				if (i1 != i && lask == 0) {
					lask = 1;
					for (j1 = 0; j1 <= 3; j1++) {
						test = 0;
						if (j1 != j) {
							for (i2 = 0; i2 <= 3; i2++) {
								if (i2 != i1 && i2 != i) {
									for (j2 = 0; j2 <= 3; j2++) {
										if (j2 != j1 && j2 != j) {
											if (test == 0) {
												k1 = i2;
												l1 = j2;
												test = 1;
											} else {
												k2 = i2;
												l2 = j2;
											}
										}
									}
								}
							}
							ijt = -ijt;
							d[j][i] = d[j][i] + (a[i1][j1] * (ijt) * (a[k1][l1] * a[k2][l2] - a[k2][l1] * a[k1][l2]));
						}
					}
				}
			}
			ij = (i + j) / 2;
			it = -((i + j) - (2 * ij));
			if (it != 0) d[j][i] = -d[j][i];
		}
	}
	det = a[0][0] * d[0][0] + a[0][1] * d[1][0] + a[0][2] * d[2][0] + a[0][3] * d[3][0];
	if (det != 0.0) {
		for (i = 0; i <= 3; i++)
			for (j = 0; j <= 3; j++)
				(*b)[i][j] = d[i][j] / det;
		return 1;
	} else {
		return -1;
	}
}

short matmul4x4(double a[4][4], double b[4][4], double (*c)[4][4])
{
	short i, j, k;
	double ab;
	for (i = 0; i <= 3; i++) {
		for (j = 0; j <= 3; j++) {
			ab = 0.0;
			for (k = 0; k <= 3; k++) {
				ab = ab + a[i][k] * b[k][j];
			}
			(*c)[i][j] = ab;
		}
	}
	return 1;
}

void reset_matrix(double *a, short a_rows, short a_cols)
{
	short i;
	for (i = 0; i < a_cols * a_rows; i++) {
		a[i] = 0.0;
	}
}
