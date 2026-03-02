/*
Function:		double degtorad(double a)
Call:			degtorad(a)
Input:			double a
Output:			return transformated anglevalues to radians

*/
#include "stdafx.h"

#define PII 3.141592654
#define RPD PII/180.0


double degtorad(double a)

 {
  return(RPD*a);
 }
