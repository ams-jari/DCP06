
#include "stdafx.h"
#include "calc.h"
#include <math.h>

/*                                                                          */
/* cartsph                                                            */
/*                                                                          */
/* transform cartesian coordinates to sphere variables                       */
/*                                                                          */
/* input:      p0[4]    cartesian coordinate values x,y,z,0                 */
/* output:     disty    distance (mm)                                       */
/*             thetax   rotation angle about x-axis (radians)               */
/*             thetaz   rotation angle about z-axis (radians)               */
/* return:              normal=1, error=-1                                  */
/*                                                                          */
/*                                                                          */


/***********************************************************************
	FUNCTION   :short cartsph(double p0[4], double *disty, double *thetax, double *thetaz);
	INPUT      :
	OUTPUT     :
************************************************************************/

short cartsph(double p0[4], double *disty, double *thetax,double *thetaz)
{

	double distxy,thetaz1;

	double pii;

	pii=3.141592654;

	/* s = û(xý+yý+zý) */

	*disty = sqrt(p0[0]*p0[0] + p0[1]*p0[1] + p0[2]*p0[2]);

	if(*disty == 0.0)
		return -1;

	/* dxy = û(xý+yý) */

	 distxy =  sqrt(p0[0]*p0[0]+p0[1]*p0[1]);

	 if(distxy == 0.0)
		return -1;

	  /* éy = arcsin((-z)/s) huom ! kulma kasvaa vastapaivaan */

	 *thetax = asin((p0[2])/(*disty));

		/* éz1 = arcsin(y/û(xý+yý)) */

		thetaz1 = asin(p0[1]/distxy);

	   /* éz = éz1 - éz2 */

		/****************
		   x >= 0
		****************/

		if(p0[0] >= 0)
		{
		   *thetaz = thetaz1;
		}
		else
		{
			/****************
				 y >= 0
			****************/

			if(p0[1] >= 0)
			{
			   *thetaz = (pii - thetaz1);
			}

			/****************
				y < 0
			*****************/

			else
			{
			   *thetaz = -(pii + thetaz1);
			}
	   }

	   *thetaz = 2 * pii - (pii + *thetaz);

	  return(1);
}




/***********************************************************************
	FUNCTION   :short cartsph(double p0[4], double *disty, double *thetax, double *thetaz);
	INPUT      :
	OUTPUT     :
************************************************************************/

short cartsph_left(double p0[4], double *disty, double *thetax,double *thetaz)
{

	double distxy,thetaz1;

	double pii;

	pii=3.141592654;

	/* s = û(xý+yý+zý) */

	*disty = sqrt(p0[0]*p0[0] + p0[1]*p0[1] + p0[2]*p0[2]);

	if(*disty == 0.0)
		return -1;

	/* dxy = û(xý+yý) */

	 distxy =  sqrt(p0[0]*p0[0]+p0[1]*p0[1]);

	 if(distxy == 0.0)
		return -1;

	  /* éy = arcsin((-z)/s) huom ! kulma kasvaa vastapaivaan */

	 *thetax = asin((p0[2])/(*disty));

		/* éz1 = arcsin(y/û(xý+yý)) */

		thetaz1 = asin(p0[1]/distxy);

	   /* éz = éz1 - éz2 */

		/****************
		   x < 0
		****************/

		if(p0[0] <= 0)
		{
		   *thetaz = pii - thetaz1;
		}
		else
		{
			/****************
				 y >= 0
			****************/

			if(p0[1] >= 0)
			{
			   *thetaz = thetaz1;
			}

			/****************
				y < 0
			*****************/

			else
			{
			   *thetaz = 2 *pii + thetaz1;
			}
	   }

	  return(1);
}





