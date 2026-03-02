/*
Calculate distance between a target point and a reference plane

Function : 	double calc_point_dist_from_plane(struct ams_vector *a, struct plane *b)
Call:		calc_point_dist_from_plane(&a, &p)
Input:		struct ams_vector a = coordinate values of  point
		struct plane b = equation of plane
Output		Calculated distance

*/

#include "stdafx.h"
#include "calc.h"

double calc_point_dist_from_plane(struct ams_vector *a,struct plane *b)
 {
//  double dot_product(struct ams_vector *a, struct ams_vector *b);
  struct ams_vector dif;
  struct ams_vector norm;
  double dist;

  /* distance calculated using dot product */

  dif.x = a->x - b->px;
  dif.y = a->y - b->py;
  dif.z = a->z - b->pz;

  norm.x = b->nx;
  norm.y = b->ny;
  norm.z = b->nz;

  dist = dot_product(&norm,&dif);

  return(dist);
 }
