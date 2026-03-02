
/*
Function:	double angle(struct ams_vector *a, struct ams_vector *b)
Call:		angle(&a, &b)
Input:		struct ams_vector a,b
Output:		Return smallest angle of two vectors (0.00 .. PII)  as radian

*/

#include "stdafx.h"
#include <math.h>
#include "calc.h"
/*
double dot_product(struct ams_vector *a,struct ams_vector *b);


double vector_length(struct ams_vector *a);
*/
double angle(struct ams_vector *a,struct ams_vector *b)

 {
  //double acos();
  double veclnth();
  double dotprod();
  double alen;
  double blen;
  double adpb;
  double arg;
  double ang;

  alen = vector_length(a);
  blen = vector_length(b);
  adpb = dot_product(a,b);

  /* if at least other is zerovector so return 0.0 */
  if(alen == 0.0 || blen == 0.0)
   {
    ang = 0.0;
    return(ang);
   }
  else /* if not , calculate angle using arccos */
   {
    arg = adpb/(alen*blen);
    ang = acos(arg);
    return(ang);
   }
 }
    

