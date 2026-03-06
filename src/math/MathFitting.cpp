/**
 * MathFitting.cpp - Fitting operations (consolidated from FITLINE, FITPLANE, ccircle)
 */
#include "stdafx.h"
#include <math.h>
#include "calc.h"

#define MAX_POINTS 20
#define PII 3.1415926535897932

short FitLine(double *p_mat, short points, double *a, double *b, double *c)
{
	short i, j, ret = 1;
	double v[20 * 3];
	double k_table[20 * 3];
	double kx, ky, kz;
	short it;
	double kx_tot = 0.0, ky_tot = 0.0, kz_tot = 0.0;
	double kx_av, ky_av, kz_av, dist, dist_tot = 0.0;

	if (v == NULL || k_table == NULL) {
		*a = 0;
		*b = 0;
		*c = 0;
		ret = -1;
		goto end;
	}

	for (i = 0; i < points - 1; i++) {
		for (j = 0; j < 3; j++)
			*(v + i * 3 + j) = *(p_mat + (i * 3 + 3) + j) - *(p_mat + i * 3 + j);
	}

	it = 0;
	for (i = 0; i < points - 1; i++) {
		it++;
		dist = sqrt(v[i * 3 + 0] * v[i * 3 + 0] + v[i * 3 + 1] * v[i * 3 + 1] + v[i * 3 + 2] * v[i * 3 + 2]);
		if (dist == 0.0) {
			*a = 0;
			*b = 0;
			*c = 0;
			ret = -1;
			goto end;
		}
		dist_tot = dist_tot + dist;
		kx = v[i * 3 + 0] / dist;
		ky = v[i * 3 + 1] / dist;
		kz = v[i * 3 + 2] / dist;
		kx = dist * kx;
		ky = dist * ky;
		kz = dist * kz;
		kx_tot = kx_tot + kx;
		ky_tot = ky_tot + ky;
		kz_tot = kz_tot + kz;
	}

	kx_av = kx_tot / dist_tot;
	ky_av = ky_tot / dist_tot;
	kz_av = kz_tot / dist_tot;
	dist = sqrt(kx_av * kx_av + ky_av * ky_av + kz_av * kz_av);

	if (dist == 0.0) {
		*a = 0;
		*b = 0;
		*c = 0;
		ret = -1;
	} else {
		*a = kx_av / dist;
		*b = ky_av / dist;
		*c = kz_av / dist;
	}
	ret = 1;

end:
	return (short)ret;
}

short FitPlane(double *m_mat, short points, double *a, double *b, double *c)
{
	double dist, dist2, len;
	short ret = 1, pno;
	short i, j, ii, it;
	struct ams_vector a1, b1, c1, aa, bb, cc, ccc, N, NTOT;
	struct plane d;

	a1.x = *(m_mat + 0 * 3 + 0);
	a1.y = *(m_mat + 0 * 3 + 1);
	a1.z = *(m_mat + 0 * 3 + 2);
	b1.x = *(m_mat + 1 * 3 + 0);
	b1.y = *(m_mat + 1 * 3 + 1);
	b1.z = *(m_mat + 1 * 3 + 2);
	c1.x = *(m_mat + 2 * 3 + 0);
	c1.y = *(m_mat + 2 * 3 + 1);
	c1.z = *(m_mat + 2 * 3 + 2);

	equation_of_plane(&a1, &b1, &c1, &d);

	if (d.nx == 0.0 && d.ny == 0.0 && d.nz == 0.0) {
		ret = -1;
		*a = 0.0;
		*b = 0.0;
		*c = 0.0;
		return ret;
	}

	dist = 0.0;
	N.x = 0.0;
	N.y = 0.0;
	N.z = 0.0;
	NTOT.x = 0.0;
	NTOT.y = 0.0;
	NTOT.z = 0.0;
	it = 0;

	for (i = 0; i < points; i++) {
		it++;
		N.x = 0.0;
		N.y = 0.0;
		N.z = 0.0;

		for (ii = 0; ii < points; ii++) {
			if (ii != i) {
				pno = ii;
				for (j = 0; j < points; j++) {
					if (j != pno && i != j) {
						aa.x = *(m_mat + (pno) * 3 + 0) - *(m_mat + i * 3 + 0);
						aa.y = *(m_mat + (pno) * 3 + 1) - *(m_mat + i * 3 + 1);
						aa.z = *(m_mat + (pno) * 3 + 2) - *(m_mat + i * 3 + 2);
						bb.x = *(m_mat + j * 3 + 0) - *(m_mat + i * 3 + 0);
						bb.y = *(m_mat + j * 3 + 1) - *(m_mat + i * 3 + 1);
						bb.z = *(m_mat + j * 3 + 2) - *(m_mat + i * 3 + 2);

						len = vector_product(&aa, &bb, &cc);

						if (len == 0.0) {
							ret = -1;
							*a = 0.0;
							*b = 0.0;
							*c = 0.0;
							return ret;
						}

						ccc.x = d.nx + cc.x;
						ccc.y = d.ny + cc.y;
						ccc.z = d.nz + cc.z;

						cc.x = cc.x * len;
						cc.y = cc.y * len;
						cc.z = cc.z * len;

						dist2 = vector_length(&ccc);

						if (dist2 <= 0.9) {
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

		NTOT.x = NTOT.x + N.x;
		NTOT.y = NTOT.y + N.y;
		NTOT.z = NTOT.z + N.z;
	}

	len = vector_length(&NTOT);

	if (len == 0.0) {
		*a = 0.0;
		*b = 0.0;
		*c = 0.0;
		return -1;
	}

	NTOT.x = NTOT.x / len;
	NTOT.y = NTOT.y / len;
	NTOT.z = NTOT.z / len;

	*a = NTOT.x;
	*b = NTOT.y;
	*c = NTOT.z;

	return ret;
}

short center_of_circle(double x1, double y1, double x2, double y2, double x3, double y3, double *radius, double *cx, double *cy)
{
	double l, m, n, a, b, c;
	double in_table[3][3];
	double out_table[3][3];
	double c11, c12, c13, c21, c22, c23, c31, c32, c33;

	if ((y1 - y2 != 0) && (y2 - y3 != 0)) {
		if (((x1 - x2) / (y1 - y2)) == ((x2 - x3) / (y2 - y3))) {
			return -3;
		}
	} else if (((x1 == x2) && (x2 == x3)) || ((y1 == y2) && (y2 == y3))) {
		return -3;
	}

	in_table[0][0] = x1;
	in_table[0][1] = y1;
	in_table[0][2] = 1.0;
	in_table[1][0] = x2;
	in_table[1][1] = y2;
	in_table[1][2] = 1.0;
	in_table[2][0] = x3;
	in_table[2][1] = y3;
	in_table[2][2] = 1.0;

	if (matinv_n(&in_table[0][0], 3, 3, &out_table[0][0]) == 1) {
		c11 = out_table[0][0];
		c12 = out_table[0][1];
		c13 = out_table[0][2];
		c21 = out_table[1][0];
		c22 = out_table[1][1];
		c23 = out_table[1][2];
		c31 = out_table[2][0];
		c32 = out_table[2][1];
		c33 = out_table[2][2];
	} else {
		return -1;
	}

	l = -(x1 * x1 + y1 * y1);
	m = -(x2 * x2 + y2 * y2);
	n = -(x3 * x3 + y3 * y3);

	a = c11 * l + c12 * m + c13 * n;
	b = c21 * l + c22 * m + c23 * n;
	c = c31 * l + c32 * m + c33 * n;

	*cx = a / -2.0;
	*cy = b / -2.0;
	*radius = sqrt(*cx * *cx + *cy * *cy - c);
	return 1;
}
