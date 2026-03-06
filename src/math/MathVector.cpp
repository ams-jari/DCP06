/**
 * MathVector.cpp - Vector and geometry operations (consolidated from VECLNTH, DOTPROD, VECPROD, VECPRO, VECMAT, UNIVEC, DIRVEC, PDIST, PDISL, PDISPL, EQOFLI, EQOFPL, CANGLE)
 */
#include "stdafx.h"
#include <math.h>
#include "calc.h"

double vector_length(struct ams_vector *a)
{
	double lensq = a->x * a->x + a->y * a->y + a->z * a->z;
	return sqrt(lensq);
}

double dot_product(struct ams_vector *a, struct ams_vector *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

double vector_product(struct ams_vector *a, struct ams_vector *b, struct ams_vector *c)
{
	double length;
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
	length = vector_length(c);
	if (length < 0.000001) {
		c->x = 0.0;
		c->y = 0.0;
		c->z = 0.0;
	} else {
		c->x /= length;
		c->y /= length;
		c->z /= length;
	}
	return length;
}

void vec_product(double u[4], double v[4], double (*w)[4])
{
	(*w)[0] = u[1] * v[2] - u[2] * v[1];
	(*w)[1] = -u[0] * v[2] + u[2] * v[0];
	(*w)[2] = u[0] * v[1] - u[1] * v[0];
	(*w)[3] = u[3] * v[3];
}

int vecmat(double v[4], double a[4][4], double (*w)[4])
{
	short i, j;
	double ab;
	for (i = 0; i <= 3; i++) {
		ab = 0.0;
		for (j = 0; j <= 3; j++) {
			ab = ab + v[j] * a[j][i];
		}
		(*w)[i] = ab;
	}
	return 1;
}

short unit_vector(double pa[4], double (*uv)[4])
{
	double a, b;
	a = pa[0] * pa[0] + pa[1] * pa[1] + pa[2] * pa[2];
	b = sqrt(a);
	if (b == 0) {
		(*uv)[0] = 0;
		(*uv)[1] = 0;
		(*uv)[2] = 0;
		return -1;
	} else {
		(*uv)[0] = pa[0] / b;
		(*uv)[1] = pa[1] / b;
		(*uv)[2] = pa[2] / b;
		return 1;
	}
}

short dir_vector(double pa[4], double pb[4], double (*dv)[4])
{
	double a, b;
	(*dv)[0] = pb[0] - pa[0];
	(*dv)[1] = pb[1] - pa[1];
	(*dv)[2] = pb[2] - pa[2];
	(*dv)[3] = 1.0;
	a = (*dv)[0] * (*dv)[0] + (*dv)[1] * (*dv)[1] + (*dv)[2] * (*dv)[2];
	b = sqrt(a);
	if (b == 0) {
		(*dv)[0] = 0;
		(*dv)[1] = 0;
		(*dv)[2] = 0;
		return -1;
	} else {
		(*dv)[0] = (*dv)[0] / b;
		(*dv)[1] = (*dv)[1] / b;
		(*dv)[2] = (*dv)[2] / b;
		return 1;
	}
}

double calc_point_dist_from_point(struct ams_vector *a, struct ams_vector *b)
{
	double difx = a->x - b->x;
	double dify = a->y - b->y;
	double difz = a->z - b->z;
	return sqrt(difx * difx + dify * dify + difz * difz);
}

double calc_point_dist_from_line(struct ams_vector *a, struct line *b)
{
	struct ams_vector blin, dif;
	double dist, adpu, res;
	blin.x = b->px;
	blin.y = b->py;
	blin.z = b->pz;
	dist = calc_point_dist_from_point(a, &blin);
	blin.x = b->ux;
	blin.y = b->uy;
	blin.z = b->uz;
	dif.x = a->x - b->px;
	dif.y = a->y - b->py;
	dif.z = a->z - b->pz;
	adpu = dot_product(&blin, &dif);
	if (dist < adpu)
		res = 0.0;
	else
		res = sqrt(dist * dist - adpu * adpu);
	return res;
}

double calc_point_dist_from_line2(struct ams_vector *a, struct line *b, struct ams_vector *dest_point)
{
	struct ams_vector blin, dif;
	double dist, adpu, res;
	blin.x = b->px;
	blin.y = b->py;
	blin.z = b->pz;
	dist = calc_point_dist_from_point(a, &blin);
	blin.x = b->ux;
	blin.y = b->uy;
	blin.z = b->uz;
	dif.x = a->x - b->px;
	dif.y = a->y - b->py;
	dif.z = a->z - b->pz;
	adpu = dot_product(&blin, &dif);
	if (dist < adpu) {
		dest_point->x = 0.0;
		dest_point->y = 0.0;
		dest_point->z = 0.0;
		res = 0.0;
	} else {
		res = sqrt(dist * dist - adpu * adpu);
		dest_point->x = b->px + adpu * b->ux;
		dest_point->y = b->py + adpu * b->uy;
		dest_point->z = b->pz + adpu * b->uz;
	}
	return res;
}

double calc_point_dist_from_plane(struct ams_vector *a, struct plane *b)
{
	struct ams_vector dif, norm;
	dif.x = a->x - b->px;
	dif.y = a->y - b->py;
	dif.z = a->z - b->pz;
	norm.x = b->nx;
	norm.y = b->ny;
	norm.z = b->nz;
	return dot_product(&norm, &dif);
}

void equation_of_line(struct ams_vector *a, struct ams_vector *b, struct line *c)
{
	double dist;
	c->px = a->x;
	c->py = a->y;
	c->pz = a->z;
	dist = calc_point_dist_from_point(a, b);
	if (dist == 0.0) {
		c->ux = 0.0;
		c->uy = 0.0;
		c->uz = 0.0;
	} else {
		c->ux = (b->x - a->x) / dist;
		c->uy = (b->y - a->y) / dist;
		c->uz = (b->z - a->z) / dist;
	}
}

short equation_of_plane(struct ams_vector *a, struct ams_vector *b, struct ams_vector *c, struct plane *d)
{
	struct ams_vector dif1, dif2, norm;
	d->px = a->x;
	d->py = a->y;
	d->pz = a->z;
	dif1.x = b->x - a->x;
	dif1.y = b->y - a->y;
	dif1.z = b->z - a->z;
	dif2.x = c->x - a->x;
	dif2.y = c->y - a->y;
	dif2.z = c->z - a->z;
	vector_product(&dif1, &dif2, &norm);
	d->nx = norm.x;
	d->ny = norm.y;
	d->nz = norm.z;
	return 1;
}

double angle(struct ams_vector *a, struct ams_vector *b)
{
	double alen, blen, adpb, arg, ang;
	alen = vector_length(a);
	blen = vector_length(b);
	adpb = dot_product(a, b);
	if (alen == 0.0 || blen == 0.0) {
		ang = 0.0;
		return ang;
	} else {
		arg = adpb / (alen * blen);
		ang = acos(arg);
		return ang;
	}
}
