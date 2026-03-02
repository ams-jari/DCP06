/*
Calculate homogenous matrix coordinate system from three points

Function:	htransm(double p0[4],double p1[4], double p2[4], short iyz, double (*hm)[4][4])
Call	:	htransm(&po, &p1, &p2, iyz, &hm)
Input:	:	double p0[4]
		double p1[4]
		double p2[4]
		short iyz

Output:		double (*hm)[4][4]

*/
#include "stdafx.h"
#include <math.h>
#include "calc.h"


short htransm(double p0[4],double p1[4],double p2[4],	short iyz,double (*hm)[4][4])
{
    double v0[4],v1[4],v2[4],apu[4];

    if(iyz == 'z' || iyz == 'Z')
    {

	dir_vector(p0,p1,&v0);    /* x direction vector v0 */

	dir_vector(p0,p2,&v2);    /* z direction vector v2 */

	vec_product(v2,v0,&apu);    /* y direction vector  v1  */
	unit_vector(apu,&v1);

	vec_product(v0,v1,&apu);    /* orthogonalize vector v2 */
	unit_vector(apu,&v2);

	}

	else
	{

	dir_vector(p0,p1,&v0);    /* x direction vector v0 */

	dir_vector(p0,p2,&v1);    /* y direction vector v1 */

	vec_product(v0,v1,&apu);    /* z direction vector  v2  */
	unit_vector(apu,&v2);

	vec_product(v2,v0,&apu);    /* orthogonalize vector v1 */
	unit_vector(apu,&v1);

	}

    (*hm)[0][0]=v0[0]; (*hm)[0][1]=v1[0]; (*hm)[0][2]=v2[0]; (*hm)[0][3]=p0[0];
    (*hm)[1][0]=v0[1]; (*hm)[1][1]=v1[1]; (*hm)[1][2]=v2[1]; (*hm)[1][3]=p0[1];
    (*hm)[2][0]=v0[2]; (*hm)[2][1]=v1[2]; (*hm)[2][2]=v2[2]; (*hm)[2][3]=p0[2];
    (*hm)[3][0]=0.0  ; (*hm)[3][1]=0.0  ; (*hm)[3][2]=0.0  ; (*hm)[3][3]=1.0  ;

	return 1;
}

