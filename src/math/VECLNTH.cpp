/*
Calculate length of vector

Function : 	double veclnth(struct ams_vector *a)
Call:		veclnth(&a)
Input:		struct ams_vector a
Output		Return value is calculated length of vector
*/

#include "stdafx.h"
#include <math.h>
#include "calc.h"


double vector_length(struct ams_vector *a)
 {
  //double sqrt();
  double lensq;
  double aax;
  double aay;
  double aaz;

  aax = a->x;	/* Components of vector */
  aay = a->y;
  aaz = a->z;

  /* Calculate square of distance */

  lensq = aax*aax + aay*aay + aaz*aaz;

  /* return length */

  return(sqrt(lensq));
 }
