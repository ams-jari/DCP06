/*
Calculate distance between a target point and a reference point

Function : 	double calc_point_dist_from_point(struct ams_vector *a, struct ams_vector *b)
Call:		calc_point_dist_from_point(&a, &p)
Input:		struct ams_vector a,b = coordinate values of  points
Output		Calculated distance
*/
#include "stdafx.h"
#include <math.h>
#include "calc.h"


double calc_point_dist_from_point(struct ams_vector *a,struct ams_vector *b)
 {
   double difx;
  double dify;
  double difz;
  double dist;

  difx = a->x - b->x;
  dify = a->y - b->y;
  difz = a->z - b->z;

  /* distance */
  dist = sqrt(difx*difx + dify*dify + difz*difz);

  return(dist);
 }
