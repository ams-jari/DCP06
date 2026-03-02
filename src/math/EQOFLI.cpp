/*
Calculate equation of line 

Function:       equation_of_line(struct ams_vector *a, struct ams_vector *b , struct line *c)
Call:           equation_of_line(&a,&b,&c)
Input:          struct ams_vector a,b
Output:         struct line c

*/


#include "stdafx.h"
#include "calc.h"


void equation_of_line(struct ams_vector *a,struct ams_vector *b,struct line *c)
 {
  double dist;
  //double calc_point_dist_from_point(struct ams_vector *a, struct ams_vector *b);


  /* point in line */

  c->px = a->x;
  c->py = a->y;
  c->pz = a->z;

  /* unitvector */

  dist = calc_point_dist_from_point(a,b);

  if(dist == 0.0) /* if point are same, zerovector */
   {
    c->ux = 0.0;
    c->uy = 0.0;
    c->uz = 0.0;
   }
  else            /* if not, unitvector */
   {
    c->ux = (b->x - a->x)/dist;
    c->uy = (b->y - a->y)/dist;
    c->uz = (b->z - a->z)/dist;
   }

  //return;
}

