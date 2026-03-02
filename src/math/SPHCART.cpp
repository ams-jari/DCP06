/*
Muunnos pallokoordinaatistosta karteesiseen koordinaatistoon

Function:	sphcart(struct sphvect *a,struct ams_vector *b)
Call:		sphcart(&a, &b)
Input:		struct sphvect s
Output:		struct ams_vector b
*/



//#define diagnostics     TRUE
/*
#include     <beta.hpp>
#include  <mmi_exp.hpp>
#include  <bap_exp.hpp>

#define diagnostics     TRUE
#ifdef diagnostics
	#include <diagnstc.hpp>
#endif
*/
#include "stdafx.h"
#include <math.h>
#include "calc.h"

void sphcart(struct sphvect *a, struct ams_vector *b)
{
  //double sin();
  //double cos();
  double xyproj;
#ifdef diagnostics
  PrintLn("a->r=%.5f a->theta=%.5f a->fii=%.5f",a->r,a->theta,a->fii);
#endif	
  xyproj = a->r*cos(a->fii);

  b->x = xyproj*cos(a->theta);
  b->y = xyproj*sin(a->theta);
  b->z = a->r*sin(a->fii);
#ifdef diagnostics	
  PrintLn("b->x=%.5f b->y=%.5f b->z=%.5f",b->x,b->y,b->z);
#endif
}
