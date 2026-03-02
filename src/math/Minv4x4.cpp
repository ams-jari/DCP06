
/*
Matrix inversion by the adjoint method


Function:	matinv(double a[4][4],double (*b)[4][4]);
Call		matinv(&a, &b)
Input:		double a[4][4]
Output:		double (*b)[4][4]

*/
#include "stdafx.h"
short matinv4x4(double a[4][4], double (*b)[4][4])

// double a[4][4],(*b)[4][4];
{
    short i,j,i1,j1,i2,j2,k1,k2,l1,l2;
    short ij,it,ijt,test,lask;

    double d[4][4], det;


    for(i=0; i<=3; i++)
    {
        for(j=0; j<=3; j++)
        {
            lask = 0;
            d[j][i]=0.0;
            ijt=1;
            for(i1=0; i1<=1; i1++)
            {
                if(i1 != i && lask == 0)
                {
                    lask=1;
                    for(j1=0; j1<=3; j1++)
                    {
                        test=0;
                        if(j1 != j)
                        {
                            for(i2=0; i2<=3; i2++)
                            {
                                if(i2 != i1 && i2 != i)
                                {
                                    for(j2=0; j2<=3; j2++)
                                    {
                                        if(j2 != j1 && j2 != j)
                                        {
                                            if(test == 0)
                                            {
                                                k1=i2;
                                                l1=j2;
                                                test = 1;
                                            }
											else
                                            {
                                                k2=i2;
                                                l2=j2;
											}
                                        }
                                    }
                                }
                            }

                            ijt = -ijt;

                            d[j][i]=d[j][i]+(a[i1][j1]*(ijt)*
                                            (a[k1][l1]*a[k2][l2]-
                                             a[k2][l1]*a[k1][l2]));

                        }
                    }
                }
            }

            ij=(i+j)/2;
            it=-((i+j)-(2*ij));
            if(it != 0) d[j][i]=-d[j][i];
        }


    }

    det = a[0][0]*d[0][0]+a[0][1]*d[1][0]+a[0][2]*d[2][0]+a[0][3]*d[3][0];

    if(det != 0.0)
    {
        for(i=0; i<=3; i++)
            for(j=0; j<=3; j++)
                (*b)[i][j]=d[i][j]/det;
        return(1);
    }
    else
    {
        return(-1); /* error, singular matrix */
	}

}

