/*
Calculate distance between a target point and a reference line

Function : 	double calc_point_dist_from_line(struct ams_vector *a, struct line *b)
Call:		calc_point_dist_from_line(&a, &p)
Input:		struct ams_vector a = coordinate values of  point
			struct plane b = equation of line
Output		Calculated distance
*/

#include "stdafx.h"
#include <math.h>
#include "calc.h"

double calc_point_dist_from_line(struct ams_vector *a,struct line *b)
 {
  // double pdist();
//  double calc_point_dist_from_point(struct ams_vector *a, struct ams_vector *b);
//  double dot_product(struct ams_vector *a,struct ams_vector *b);
//  double dotprod();
//  double sqrt();
  struct ams_vector blin;
  struct ams_vector dif;
  double dist;
  double adpu;
  double res;

  /* distance between points */

  blin.x = b->px;
  blin.y = b->py;
  blin.z = b->pz;

  dist = calc_point_dist_from_point(a,&blin);

  /* dot product */

  blin.x = b->ux;
  blin.y = b->uy;
  blin.z = b->uz;

  dif.x = a->x - b->px;
  dif.y = a->y - b->py;
  dif.z = a->z - b->pz;

  adpu = dot_product(&blin,&dif);

  /* distance between point and line */
  if(dist < adpu)
	res = 0.0;
  else
	res = sqrt(dist*dist - adpu*adpu);

  return(res);
 }

double calc_point_dist_from_line2(struct ams_vector *a,struct line *b, struct ams_vector *dest_point)
 {
  // double pdist();
//  double calc_point_dist_from_point(struct ams_vector *a, struct ams_vector *b);
//  double dot_product(struct ams_vector *a,struct ams_vector *b);
//  double dotprod();
//  double sqrt();
  struct ams_vector blin;
  struct ams_vector dif;
  double dist;
  double adpu;
  double res;

  /* distance between points */

  blin.x = b->px;
  blin.y = b->py;
  blin.z = b->pz;

  dist = calc_point_dist_from_point(a,&blin);

  /* dot product */

  blin.x = b->ux;
  blin.y = b->uy;
  blin.z = b->uz;

  dif.x = a->x - b->px;
  dif.y = a->y - b->py;
  dif.z = a->z - b->pz;

  adpu = dot_product(&blin,&dif);

  /* distance between point and line */
  if(dist < adpu)
  {
	  dest_point->x = 0.0;
	dest_point->y = 0.0;
	dest_point->z = 0.0;
	res = 0.0;
  }
  else
  {
	res = sqrt(dist*dist - adpu*adpu);

	dest_point->x = b->px + adpu*b->ux;
	dest_point->y = b->py + adpu*b->uy;
	dest_point->z = b->pz + adpu*b->uz;
  }

  return(res);
 }
