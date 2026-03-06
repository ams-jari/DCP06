/**
 * MathTransform.cpp - Transform and rotation operations (consolidated from PTRANS, HROTAX, HROTAY, HROTAZ, HTRANSM)
 */
#include "stdafx.h"
#include <math.h>
#include "calc.h"

void ptrans(double v[4], double a[4][4], double (*w)[4])
{
	short i, j;
	double vt[4], ar[4][4];
	for (i = 0; i <= 3; i++)
		vt[i] = v[i] - a[i][3];
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			ar[i][j] = a[i][j];
	for (i = 0; i <= 2; i++)
		ar[i][3] = 0.0;
	vecmat(vt, ar, w);
}

void rotate_about_x_axis(double a[4][4], double b, double (*w)[4][4])
{
	double r[4][4];
	r[0][0] = 1.0;   r[0][1] = 0.0;    r[0][2] = 0.0;    r[0][3] = 0.0;
	r[1][0] = 0.0;   r[1][1] = cos(b); r[1][2] = -sin(b); r[1][3] = 0.0;
	r[2][0] = 0.0;   r[2][1] = sin(b); r[2][2] = cos(b);  r[2][3] = 0.0;
	r[3][0] = 0.0;   r[3][1] = 0.0;    r[3][2] = 0.0;    r[3][3] = 1.0;
	matmul4x4(a, r, w);
}

void rotate_about_y_axis(double a[4][4], double b, double (*w)[4][4])
{
	double r[4][4];
	r[0][0] = cos(b);  r[0][1] = 0.0;    r[0][2] = sin(b);  r[0][3] = 0.0;
	r[1][0] = 0.0;     r[1][1] = 1.0;    r[1][2] = 0.0;     r[1][3] = 0.0;
	r[2][0] = -sin(b); r[2][1] = 0.0;    r[2][2] = cos(b);  r[2][3] = 0.0;
	r[3][0] = 0.0;     r[3][1] = 0.0;    r[3][2] = 0.0;    r[3][3] = 1.0;
	matmul4x4(a, r, w);
}

void rotate_about_z_axis(double a[4][4], double b, double (*w)[4][4])
{
	double r[4][4];
	r[0][0] = cos(b); r[0][1] = -sin(b); r[0][2] = 0.0; r[0][3] = 0.0;
	r[1][0] = sin(b); r[1][1] = cos(b);  r[1][2] = 0.0; r[1][3] = 0.0;
	r[2][0] = 0.0;    r[2][1] = 0.0;     r[2][2] = 1.0; r[2][3] = 0.0;
	r[3][0] = 0.0;    r[3][1] = 0.0;     r[3][2] = 0.0; r[3][3] = 1.0;
	matmul4x4(a, r, w);
}

short htransm(double p0[4], double p1[4], double p2[4], short iyz, double (*hm)[4][4])
{
	double v0[4], v1[4], v2[4], apu[4];
	if (iyz == 'z' || iyz == 'Z') {
		dir_vector(p0, p1, &v0);
		dir_vector(p0, p2, &v2);
		vec_product(v2, v0, &apu);
		unit_vector(apu, &v1);
		vec_product(v0, v1, &apu);
		unit_vector(apu, &v2);
	} else {
		dir_vector(p0, p1, &v0);
		dir_vector(p0, p2, &v1);
		vec_product(v0, v1, &apu);
		unit_vector(apu, &v2);
		vec_product(v2, v0, &apu);
		unit_vector(apu, &v1);
	}
	(*hm)[0][0] = v0[0]; (*hm)[0][1] = v1[0]; (*hm)[0][2] = v2[0]; (*hm)[0][3] = p0[0];
	(*hm)[1][0] = v0[1]; (*hm)[1][1] = v1[1]; (*hm)[1][2] = v2[1]; (*hm)[1][3] = p0[1];
	(*hm)[2][0] = v0[2]; (*hm)[2][1] = v1[2]; (*hm)[2][2] = v2[2]; (*hm)[2][3] = p0[2];
	(*hm)[3][0] = 0.0;   (*hm)[3][1] = 0.0;   (*hm)[3][2] = 0.0;   (*hm)[3][3] = 1.0;
	return 1;
}
