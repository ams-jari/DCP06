/*
Radians to degree

Function:	double radtodeg(double a)
Call		x = radtodeg(a)
Input:		double a
Output:		Return converted radians to degree
*/

#include "stdafx.h"

#define PII 3.141592654
#define DPR 180.0/PII


double radtodeg(double a)
{
  return(DPR*a);
}
