/*
Calculate  the dot product of two vectors

Function : 	dot_product(struct ams_vector *a,struct ams_vector *b)
Call:		dot_product(&a,&b)
Input:		struct ams_vector a,b
Output		Return the  dot product

*/
#include "stdafx.h"
#include "calc.h"


double dot_product(struct ams_vector *a,struct ams_vector *b)


 {
  double res;

  res = a->x*b->x + a->y*b->y + a->z*b->z;
  return(res);
 }
