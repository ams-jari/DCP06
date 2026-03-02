/*
Calculate  the vector product of two vectors

Function : 	vecprod(struct ams_vector *a,struct ams_vector *b,struct ams_vector *c)
Call:		vecprod(&a,&b,&c)
Input:		struct ams_vector a,b
Output		struct ams_vector c = a dot b
*/
#include "stdafx.h"
#include <math.h>

#include "calc.h"

double vector_length(struct ams_vector *a);

double vector_product(struct ams_vector *a,struct ams_vector *b,struct ams_vector *c)

 {
  //double veclnth();
  double length;

  /* Components of results */

  c->x = a->y*b->z - a->z*b->y;
  c->y = a->z*b->x - a->x*b->z;
  c->z = a->x*b->y - a->y*b->x;

  length = vector_length(c);

  /* if length < 0.000001, results is zerovector */
  if(length < 0.000001)
   {
	c->x = 0.0;
	c->y = 0.0;
	c->z = 0.0;
   }
  else /* if not, rsults is unitvector */
   {
	c->x /= length;
	c->y /= length;
	c->z /= length;
   }

  return length;
 }