/******************************************************************************\
*    DOM.CPP                                                                   *
*                                                                              *
*                                                                              *
* COPYRIGHT   : A.M.S.Ltd, 1998.                                               *
*                                                                              *
* PROJECT     : PMP05-TPS		                                       *
*                                                                              *
* DESCRIPTION : Dom		                                               *
*                                                                              *
* INSTRUCTIONS:                                                                *
*                                                                              *
* FIRST CODED : 25.3.1998 Poj		                                       *
*                                                                              *
*                                                                              *
* REVISIONS   :                                                                *
*   DATE        NAME        CHANGE                                             *
*   yyyy-mm-dd  ii          description                                        *
*                                                                              *
\******************************************************************************/

/******************************************************************************\
*                                                                              *
*   Includes for the Run Time Library                                          *
*                                                                              *
\******************************************************************************/
#include "stdafx.h"
#include "calc.h"
#include <math.h>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/calculation/CalculationCircleCore.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/calculation/ResCircle.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;


/******************************************************************************\
*                                                                              *
*   Includes for the Beta II API                                               *
*                                                                              *
\******************************************************************************/

// #define diagnostics     TRUE
/*
#include     <beta.hpp>
#include  <mmi_exp.hpp>
#include  <bap_exp.hpp>
#include  <sup_exp.hpp>
#include  <tmc_exp.hpp>
#include  <wir_exp.hpp>
#include  <aus_exp.hpp>
#include  <aut_exp.hpp>
#include  <geomath.hpp>
#ifdef diagnostics
	#include  <diagnstc.hpp>
#endif
// #include <log_file.hpp>
#include <fil_exp.hpp>
*/
/******************************************************************************\
*                                                                              *
*   Includes for this application                                              *
*                                                                              *
\******************************************************************************/


// ================================================================================================
// ====================================  CalcCircle  ========================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CalcCircle::CalcCircle(short plane_type,
										S_CIRCLE_BUFF *circle_points, 
										 S_PLANE_BUFF* planes,
										S_CIRCLE_BUFF *circle_points_in_plane,
										double dToolRadius,Model* pModel ): 
				m_iPlaneType(plane_type),
				m_pCircle_points(circle_points),
				m_pCircle_points_in_plane(circle_points_in_plane),
				m_pPlanes(planes),
				m_dToolRadius(dToolRadius)
{
	m_pMsgBox = new MsgBox;
	m_pCommon = new Common(pModel);
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalcCircle::~CalcCircle()
{
	if(m_pMsgBox)
	{
		delete m_pMsgBox;
		m_pMsgBox = 0;
	}

	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// ================================================================================================
// Description: calc_center_of_circle
// ================================================================================================
short DCP::CalcCircle::calc_center_of_circle(/*S_CIRCLE_BUFF *circle_points, S_CIRCLE_BUFF *circle_points_in_plane*/)
{
//short ret;
double cx_,cy_,cz_;
double mea1[4];
double point1[4];
short i,count,ret1,count1;
StringC sMsg;
CalcPlane temp_calc_plane;
S_PLANE_BUFF temp_plane_buffer[1];

	if(m_iPlaneType == 0)
	{
		sMsg.LoadTxt(AT_DCP06, M_DCP_DEFINE_CIRCLE_PLANE_TOK);
		m_pMsgBox->ShowMessageOk(sMsg);
		return false;
	}
	cz_= 0;

	count= m_pCommon->defined_circle_points(&m_pCircle_points[0],0);

	if(count < 3)
	{	
		sMsg.LoadTxt(AT_DCP06, M_DCP_CIRCLE_WITH_AT_LEAST_PNTS_TOK);
		m_pMsgBox->ShowMessageOk(sMsg);
		return false;
	}

	//(void) MMI_DrawBusyField(ON);

	count1 = m_pCommon->get_max_defined_point_circle(&m_pCircle_points[0]);

	for(i=0;i<MAX_POINTS_IN_CIRCLE;i++)
	{
		m_pCircle_points_in_plane[0].points[i].sta = 0;
		m_pCircle_points_in_plane[0].points[i].x = 0.0;
		m_pCircle_points_in_plane[0].points[i].y = 0.0;
		m_pCircle_points_in_plane[0].points[i].z = 0.0;
		m_pCircle_points_in_plane[0].points[i].diameter = 0.0;
		sprintf(m_pCircle_points_in_plane[0].points[i].point_id,"%-6.6s", "");
	}

	ret1 = convert_points_to_plane(&m_pCircle_points[0], &m_pCircle_points_in_plane[0],count1); // APUTAULUKKOON

    
    memset(&temp_plane_buffer[0],0,sizeof(S_PLANE_BUFF));
    // Points from circle buffer to plane buffer 
	for(i=0;i<MAX_POINTS_IN_PLANE;i++)
	{
		temp_plane_buffer[0].points[i].x= m_pCircle_points[0].points[i].x;
		temp_plane_buffer[0].points[i].y= m_pCircle_points[0].points[i].y;
		temp_plane_buffer[0].points[i].z= m_pCircle_points[0].points[i].z;
		temp_plane_buffer[0].points[i].sta= m_pCircle_points[0].points[i].sta;
		temp_plane_buffer[0].points[i].xdes= m_pCircle_points[0].points[i].xdes;
		temp_plane_buffer[0].points[i].ydes= m_pCircle_points[0].points[i].ydes;
		temp_plane_buffer[0].points[i].zdes= m_pCircle_points[0].points[i].zdes;
		temp_plane_buffer[0].points[i].dsta= m_pCircle_points[0].points[i].dsta;
		strcpy(temp_plane_buffer[0].points[i].point_id, m_pCircle_points[0].points[i].point_id);
     }
	// Calc temporary plane for getting normal
	temp_calc_plane.calc(&temp_plane_buffer[0], ACTUAL);

	if(ret1 == 0)
	{
		//(void) MMI_DrawBusyField(OFF);
		return 0;
	}

	//(void) MMI_DrawBusyField(ON);
	if(count <= 3)
		ret1 = calc_circle(m_pCircle_points_in_plane,count1,&cx_,&cy_,&diameter);
	else
		ret1 = pns_ymp(m_pCircle_points_in_plane,count1,&cx_,&cy_,&diameter);

	// *******************
	// Copy diameters
	// *******************
	for(i=0;i < MAX_POINTS_IN_CIRCLE;i++)
	{
		m_pCircle_points[0].points[i].diameter = m_pCircle_points_in_plane[0].points[i].diameter;			
	}

	if(ret1 == 1)
	{
		// diameter =  diameter + (bR * 2.0);
		diameter =  diameter - m_dToolRadius; // =RADIUS !!!!!!!

		if(m_iPlaneType == XY_PLANE)
		{
			cx = cx_;
			cy = cy_;
			cz = m_pCircle_points[0].points[0].z;

			// Set vector
			vi = 0;
		    vj = 0;
		    vk = 1;

		}
		else if(m_iPlaneType == YZ_PLANE)
		{
			cx = m_pCircle_points[0].points[0].x;
			cy = cy_;
			cz = cx_;

			// Set vector
			vi = 1;
		    vj = 0;
		    vk = 0;
		}
		else if(m_iPlaneType== ZX_PLANE)
		{
			cx = cx_;
			cy = m_pCircle_points[0].points[0].y;
			cz = cy_;

			// Set vector
			vi = 0;
		    vj = 1;
		    vk = 0;
		}
		else if(m_iPlaneType == MEAS_PLANE)
		{
			mea1[0] = cx_;
			mea1[1] = cy_;
			mea1[2] = cz_;

			ptrans(mea1,ccs_inv_matrix, &point1);

			cx = point1[0];
			cy = point1[1];
			cz = point1[2];

			// Set vector
			vi = temp_plane_buffer[0].nx;
		    vj = temp_plane_buffer[0].ny;
		    vk = temp_plane_buffer[0].nz;
		}
		else if(m_iPlaneType == CIRCLE_POINTS_PLANE)
		{
			mea1[0] = cx_;
			mea1[1] = cy_;
			mea1[2] = cz_;

			ptrans(mea1,ccs_inv_matrix, &point1);

			cx = point1[0];
			cy = point1[1];
			cz = point1[2];

			// Set vector
			vi = temp_plane_buffer[0].nx;
		    vj = temp_plane_buffer[0].ny;
		    vk = temp_plane_buffer[0].nz;
		}
		m_pCircle_points[0].cx = cx;
		m_pCircle_points[0].cy = cy;
		m_pCircle_points[0].cz = cz;
	    m_pCircle_points[0].vi_ = vi;
		m_pCircle_points[0].vj_ = vj;
		m_pCircle_points[0].vk_ = vk;
		m_pCircle_points[0].diameter = diameter;	
		m_pCircle_points[0].calc= 1;
		m_pCircle_points[0].sta= 1;
	}
	else
	{
		if(ret1 == -3) // Samalla suoralla // ret ->ret1 171000
		{	
			sMsg.LoadTxt(AT_DCP06,M_DCP_POINTS_ARE_IN_LINE_TOK);
			m_pMsgBox->ShowMessageOk(sMsg);
		}
		else if(ret1 == -1) // K��nteismatriisi ei onninstunut   ret ->ret1 171000
		{
			sMsg.LoadTxt(AT_DCP06, M_DCP_CANNOT_CALC_TOK);
			m_pMsgBox->ShowMessageOk(sMsg);
		}
		m_pCircle_points[0].calc= 0;
		m_pCircle_points[0].sta= 0;

		//(void) MMI_DrawBusyField(OFF);

		return 0;
	}
	//(void) MMI_DrawBusyField(OFF);

	return 1;
}


// ================================================================================================
// Description: calc_matrix
// ================================================================================================
short DCP::CalcCircle::calc_matrix(S_POINT_BUFF *points)
{
short i,ret,count,cc;
double mea1[4],mea2[4],mea3[4], uni[4];
double point1[3],point2[3],point3[3];
struct ams_vector c,m;
struct plane wplane;
double dist;

	// ******************************
	// Added 11.2.1999
	// ******************************

	memset(&temp_plane_table[0],0,sizeof(S_PLANE_BUFF));
	
	count = 0;
	for(i=0;i<MAX_POINTS_IN_PLANE;i++)
	{
		temp_plane_table[0].points[i].x= points[i].x;
		temp_plane_table[0].points[i].y= points[i].y;
		temp_plane_table[0].points[i].z= points[i].z;
		temp_plane_table[0].points[i].sta= points[i].sta;
		temp_plane_table[0].points[i].xdes= points[i].xdes;
		temp_plane_table[0].points[i].ydes= points[i].ydes;
		temp_plane_table[0].points[i].zdes= points[i].zdes;
		temp_plane_table[0].points[i].dsta= points[i].dsta;
		strcpy(temp_plane_table[0].points[i].point_id, points[i].point_id);

		if(points[i].sta == 1 || points[i].sta == 2)
		{
			count++;
		}
	}
		
	if(count > 3)
	{
		// ********************************************************
		// Lasketaan taso
		// ********************************************************
		CalcPlane calc_plane;
		if(calc_plane.calc(&temp_plane_table[0], ACTUAL))
		{
			//PrintLn("Tason normaali");
			//PrintLn("x:%.5f Y:%.5f Z=%.5f", temp_plane_table[0].nx,temp_plane_table[0].ny,temp_plane_table[0].nz);
			
			//PrintLn("Tason tunnettu piste");
			//PrintLn("x:%.5f Y:%.5f Z=%.5f", temp_plane_table[0].px,temp_plane_table[0].py,temp_plane_table[0].pz);

			cc = 0;			

			for(i=0;i<MAX_POINTS_IN_PLANE;i++)
			{
				if(temp_plane_table[0].points[i].sta == 1 ||temp_plane_table[0].points[i].sta == 2)
				{	
					// ***************************************************
					// Pisteen et�isyys tasosta
					// ***************************************************
					m.x = temp_plane_table[0].points[i].x;
					m.y = temp_plane_table[0].points[i].y;
					m.z = temp_plane_table[0].points[i].z;
				
					wplane.px = temp_plane_table[0].px;
					wplane.py = temp_plane_table[0].py;
					wplane.pz = temp_plane_table[0].pz;
				
					wplane.nx = temp_plane_table[0].nx;
					wplane.ny = temp_plane_table[0].ny;
					wplane.nz = temp_plane_table[0].nz;
					// JMA: tutki t�m�
				
					dist = calc_point_dist_from_plane(&m,&wplane);
					// PrintLn("Dist=%.5f",dist); 	
				
					// *********************************
					// Tason normaali yksikk�vektoriksi 
					// *********************************

					mea1[0] = temp_plane_table[0].nx;
					mea1[1] = temp_plane_table[0].ny;
					mea1[2] = temp_plane_table[0].nz;
				
					unit_vector(mea1,&uni);
							
					// PrintLn("uni x = %.5f",uni[0]);  //X
					// PrintLn("uni y = %.5f",uni[1]);  //Y
					// PrintLn("uni z = %.5f",uni[2]);  //Z
				
					c.x = dist * uni[0];
					c.y = dist * uni[1];
					c.z = dist * uni[2];
										
					// *********************************
					//	Lasketaan 3 ensinm�isen pisteen
					//  koordinaatit lasketulle tasolle
					// *********************************
					if(cc==0)
					{			
						point1[0] = points[0].x - c.x;
						point1[1] = points[0].y - c.y;
						point1[2] = points[0].z - c.z;
						/*
						PrintLn("1x=%.5f",point1[0]);
						PrintLn("1y=%.5f",point1[1]);
						PrintLn("1z=%.5f",point1[2]);
						*/

					}
					else if(cc==1)
					{			
						point2[0] = points[1].x - c.x;
						point2[1] = points[1].y - c.y;
						point2[2] = points[1].z - c.z;
						/*
						PrintLn("2x=%.5f",point2[0]);
						PrintLn("2y=%.5f",point2[1]);
						PrintLn("2z=%.5f",point2[2]);
						*/
					}

					else if(cc==2)
					{			
						point3[0] = points[2].x - c.x;
						point3[1] = points[2].y - c.y;
						point3[2] = points[2].z - c.z;
						/*
						PrintLn("3x=%.5f",point3[0]);
						PrintLn("3y=%.5f",point3[1]);
						PrintLn("3z=%.5f",point3[2]);
						*/
						break;	
					}
					cc++;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		point1[0] = points[0].x;
		point1[1] = points[0].y;
		point1[2] = points[0].z;

		point2[0] = points[1].x;
		point2[1] = points[1].y;
		point2[2] = points[1].z;

		point3[0] = points[2].x;
		point3[1] = points[2].y;
		point3[2] = points[2].z;
	}


	// ******************************
	// End added
	// ******************************

	mea1[0] = point1[0];
	mea1[1] = point1[1];
	mea1[2] = point1[2];

	mea2[0] = point2[0];
	mea2[1] = point2[1];
	mea2[2] = point2[2];

	mea3[0] = point3[0];
	mea3[1] = point3[1];
	mea3[2] = point3[2];

	htransm(mea1,mea2,mea3,'y',&ccs_matrix);
	if(matinv4x4(ccs_matrix, &ccs_inv_matrix) != -1)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	return ret;
}

// ================================================================================================
// Description: convert_points_to_plane
// ================================================================================================
short DCP::CalcCircle::convert_points_to_plane(S_CIRCLE_BUFF *circle_points, S_CIRCLE_BUFF *circle_points_in_plane, short count)
{
short i;
double mea1[4],point1[4];
StringC sMsg;
short count1;

	// ******************************************
	// CIRCLEPOINTS TO BUFF
	// ******************************************

	for(i=0;i<count;i++)
	{
		circle_points_in_plane[0].points[i].x = circle_points[0].points[i].x;
		circle_points_in_plane[0].points[i].y = circle_points[0].points[i].y;
		circle_points_in_plane[0].points[i].z = circle_points[0].points[i].z;
		circle_points_in_plane[0].points[i].sta = circle_points[0].points[i].sta;
		sprintf(circle_points_in_plane[0].points[i].point_id,"%-6.6s",circle_points[0].points[i].point_id);
	}

	if(m_iPlaneType == XY_PLANE)
	{
		
	}
	else if(m_iPlaneType== YZ_PLANE)
	{
		// mea1[0] = mea1[2];
		// mea2[0] = mea2[2];
		// mea3[0] = mea3[2];

		for(i=0;i<count;i++)
		{
			circle_points_in_plane[0].points[i].x = circle_points[0].points[i].z; // Z -> X
		}
	}
	else if(m_iPlaneType== ZX_PLANE)
	{
		// mea1[1] = mea1[2];
		// mea2[1] = mea2[2];
		// mea3[1] = mea3[2];

		for(i=0;i<count;i++)
		{
			circle_points_in_plane[0].points[i].y = circle_points[0].points[i].z; // Z -> Y
		}

	}
	else if(m_iPlaneType == MEAS_PLANE)
	{
		count1 = 0;
		for(i=0;i< MAX_POINTS_IN_PLANE /*3*/; i++)
		{
			if(m_pPlanes[0].points[i].sta != 0)
				count1++;
		}
		if(count1 < 3)
		{
			sMsg.LoadTxt(AT_DCP06, M_DCP_CPLANE_WITH_3_PNTS_TOK);
			m_pMsgBox->ShowMessageOk(sMsg);
			return 0;
		}
		if(calc_matrix(&m_pPlanes[0].points[0]) == 1)
		{
			for(i=0;i<count;i++)
			{
				mea1[0] = m_pCircle_points[0].points[i].x;
				mea1[1] = m_pCircle_points[0].points[i].y;
				mea1[2] = m_pCircle_points[0].points[i].z;
			
				ptrans(mea1, ccs_matrix, &point1);
			
				m_pCircle_points_in_plane[0].points[i].x = point1[0];
				m_pCircle_points_in_plane[0].points[i].y = point1[1];
				m_pCircle_points_in_plane[0].points[i].z = point1[2];
				
				// ptrans(mea2, ccs_matrix, &point2);
				// ptrans(mea3, ccs_matrix, &point3);
			}
		}
		else
		{	
			return 0;	
		}
	}
	else if(m_iPlaneType == CIRCLE_POINTS_PLANE)
	{
		if(calc_matrix(&m_pCircle_points[0].points[0]) == 1)
		{
			for(i=0;i<count;i++)
			{
				mea1[0] = m_pCircle_points[0].points[i].x;
				mea1[1] = m_pCircle_points[0].points[i].y;
				mea1[2] = m_pCircle_points[0].points[i].z;
			
				ptrans(mea1, ccs_matrix, &point1);
			
				m_pCircle_points_in_plane[0].points[i].x = point1[0];
				m_pCircle_points_in_plane[0].points[i].y = point1[1];
				m_pCircle_points_in_plane[0].points[i].z = point1[2];
				
				// ptrans(mea2, ccs_matrix, &point2);
				// ptrans(mea3, ccs_matrix, &point3);
			}

			// ptrans(mea1, ccs_matrix, &point1);
			// ptrans(mea2, ccs_matrix, &point2);
			// ptrans(mea3, ccs_matrix, &point3);
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

// ================================================================================================
// Description: get_results
// ================================================================================================
void DCP::CalcCircle::get_results(double* dCx, double* dCy, double* dCz, double* dVi, double* dVj, double* dVk, double* dDiameter, double* rms, short* iPno)
{
	
	*dCx = cx;
	*dCy = cy;
	*dCz = cz;
    *dVi = vi;
	*dVj = vj;
	*dVk = vk;
	*dDiameter = diameter;
	
	get_max_dist_and_rms_circle(&m_pCircle_points[0],iPno,rms);

}

// ================================================================================================
// Description: get_max_dist_and_rms_circle
// ================================================================================================
double DCP::CalcCircle::get_max_dist_and_rms_circle(S_CIRCLE_BUFF *circle_, short *pno, double *rms_diameter)
{

short i, count=0;

double max=0.0, dist, dist2=0.0, dist3;



	for(i=0; i < MAX_POINTS_IN_CIRCLE; i++)
	{

		if(circle_[0].points[i].sta == 1 || circle_[0].points[i].sta == 2)
		{
			count++;

			/* Calculate distance */

			dist = circle_[0].diameter - circle_[0].points[i].diameter;
			// dist_rad = (circle_[0].diameter / 2.0) - (circle_[0].points[i].diameter / 2.0);

			dist3 = fabs(dist);
			// dist3_rad = fabs(dist_rad);

			dist2 = dist2 + (dist3 * dist3);
			// dist2_rad = dist2_rad + (dist3_rad * dist3_rad);
			

			if(count == 1)
			{
				*pno = i+1;
				max = dist;

			}
			if(dist3 > fabs(max))
			{
				max = dist;
				*pno = i+1;
			}
		}
	}
	
	// ******************
	// RMS 
	// ******************
	
	if(count > 0)
	{
		*rms_diameter = sqrt(dist2 / count);
		// *rms_radius = sqrt(dist2_rad / count);
	}
	else
	{
		*rms_diameter = 99.9;
		// *rms_radius = 99.9;
	}
	return max;
}

// ================================================================================================
// Description: calc_circle
// ================================================================================================
short DCP::CalcCircle::calc_circle(S_CIRCLE_BUFF *points, int count, double *cxtod,double *cytod, double *diameter)
{
	short i,a,j,k,ret;
	double cx,cy,sx=0,sy=0,x1,x2,x3,y1,y2,y3,radius,srad=0.0,dist;
	struct ams_vector cxyz,mxyz;
	a=0;
		sx = 0.0;
		sy = 0.0;
		srad = 0.0;

		for (i = 0; i < count; i++)
		{
			for (j = i+1; j < count; j++)
				{
					for (k = j+1; k < count; k++)
					{
						if( (points[0].points[i].sta == 1 || points[0].points[i].sta == 2) && 
							(points[0].points[j].sta == 1 || points[0].points[j].sta == 2) && 
							(points[0].points[k].sta == 1 || points[0].points[k].sta == 2))
						{
							x1 = points[0].points[i].x;
							y1 = points[0].points[i].y;
							x2 = points[0].points[j].x;
							y2 = points[0].points[j].y;
							x3 = points[0].points[k].x;
							y3 = points[0].points[k].y;
							ret = center_of_circle(x1,y1,x2,y2,x3,y3,&radius,&cx,&cy);

							sx += cx;
							sy += cy;
							srad += radius;
							a++;
						}
					}
				}
		}

		*cxtod = sx/a;
		*cytod = sy/a;
		// *diameter = 2*srad/a;
		*diameter = srad/a;	

		for( i=0; i < count; i++)
		{
			if(points[0].points[i].sta != 0)
			{
				mxyz.x = points[0].points[i].x;
				mxyz.y = points[0].points[i].y;
				mxyz.z = 1;
				cxyz.x = *cxtod;
				cxyz.y = *cytod;
				cxyz.z = 1;
				dist = calc_point_dist_from_point(&mxyz,&cxyz);
				points[0].points[i].diameter = dist-m_dToolRadius; // 2* DIST NOW IS RADIUS
			}
		}

		return ret;
}


//double DCP::CalcCircle::pns_ymp(double x[], double y[], double& x0,double& y0,int ndata)
short DCP::CalcCircle::pns_ymp(S_CIRCLE_BUFF *points, int count, double *cxtod,double *cytod, double *diameter)
{
	double AtA[3][3];
	//Matrix AtA(3,3);
	double invAtA[3][3];
	//Matrix invAtA(3,3);
	//double determi;
	double Atb[3] = {0.0, 0.0, 0.0};
	double r = 0;
	double tmppar3 = 0;
	double x0=0.0,y0=0.0;
	struct ams_vector cxyz,mxyz;
	double dist = 0.0;
	short i;

	reset_matrix (&AtA[0][0], 3, 3);
	reset_matrix (&invAtA[0][0], 3, 3);

	// T�ytet��n matriisit	
	/* ORIG
	for (int i=0;i<ndata;i++)
	{		
		AtA(1,1) += 4*x[i]*x[i];
		AtA(1,2) += 4*x[i]*y[i];
		AtA(1,3) += 2*x[i];
		AtA(2,2) += 4*y[i]*y[i];
		AtA(2,3) += 2*y[i];		
		Atb[0] += 2*x[i]*(x[i]*x[i]+y[i]*y[i]);
		Atb[1] += 2*y[i]*(x[i]*x[i]+y[i]*y[i]);
		Atb[2] += x[i]*x[i]+y[i]*y[i];
	}

	AtA(2,1) = AtA(1,2);
	AtA(3,1) = AtA(1,3);
	AtA(3,2) = AtA(2,3);
	AtA(3,3) = ndata;

	// K��nteismatriisi
	determi = inv(AtA, invAtA);

	// Ratkaistaan ympyr�n parametrit
	for (i=1;i<4;i++)
	{		
		x0 += invAtA(1,i)*Atb[i-1];
		y0 += invAtA(2,i)*Atb[i-1];
		tmppar3 += invAtA(3,i)*Atb[i-1];
	}

	r = sqrt(x0*x0+y0*y0+tmppar3);
	*/
	
		short lask = 0;
		double x = 0.0, y = 0.0;

		for (i = 0; i < count; i++)
		{
			if( points[0].points[i].sta == 1 || points[0].points[i].sta == 2)
			{
					x = points[0].points[i].x;
					y = points[0].points[i].y;
			
					AtA[0][0] += 4*x*x;
					AtA[0][1] += 4*x*y;
					AtA[0][2] += 2*x;
					AtA[1][1] += 4*y*y;
					AtA[1][2] += 2*y;		
					Atb[0] += 2*x * (x*x+y*y);
					Atb[1] += 2*y * (x*x+y*y);
					Atb[2] += x*x + y*y;

					lask++;
			}		
		}
		AtA[1][0] = AtA[0][1];
		AtA[2][0] = AtA[0][2];
		AtA[2][1] = AtA[1][2];
		AtA[2][2] = lask;
		
	// K��nteismatriisi
	//determi = inv(AtA, invAtA);
	matinv_n (&AtA[0][0], 3,3, &invAtA[0][0]);

	// Ratkaistaan ympyr�n parametrit
	for (i=1;i<4;i++)
	{		
		x0 += invAtA[0][i-1]*Atb[i-1];
		y0 += invAtA[1][i-1]*Atb[i-1];
		tmppar3 += invAtA[2][i-1]*Atb[i-1];
	}
	r = sqrt(x0*x0+y0*y0+tmppar3);
	
		*cxtod = x0;
		*cytod = y0;
		*diameter = r;

		for( i=0; i < count; i++)
		{
			if(points[0].points[i].sta != 0)
			{
				mxyz.x = points[0].points[i].x;
				mxyz.y = points[0].points[i].y;
				mxyz.z = 1;
				cxyz.x = *cxtod;
				cxyz.y = *cytod;
				cxyz.z = 1;
				dist = calc_point_dist_from_point(&mxyz,&cxyz);
				points[0].points[i].diameter = dist-m_dToolRadius; // 2* DIST NOW IS RADIUS
			}
		}
	//return r;
	return 1;
}

// ========== CalcCircleontrollerC ==========
DCP::CalcCircleontrollerC::CalcCircleontrollerC(CircleModel* pCircleModel, short iDisplay):
	m_pDataModel(pCircleModel), m_iDisplay(iDisplay), m_pCommon(0), m_pModel(0)
{
}

DCP::CalcCircleontrollerC::~CalcCircleontrollerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::CalcCircleontrollerC::OnControllerActivated(void)
{
}

void DCP::CalcCircleontrollerC::Run(void)
{
	CalcCircle circle(m_pDataModel->PLANE_TYPE, 
								&m_pDataModel->circle_points[0],
								&m_pDataModel->planes[0],
								&m_pDataModel->circle_points_in_plane[0],
								m_pDataModel->bR, m_pModel);
	
	if(circle.calc_center_of_circle())
	{
			m_pDataModel->m_iCounts = m_pDataModel->pCommon->defined_circle_points(&m_pDataModel->circle_points[0],0);
			circle.get_results(&m_pDataModel->cx, &m_pDataModel->cy,&m_pDataModel->cz,&m_pDataModel->vi, &m_pDataModel->vj,&m_pDataModel->vk,&m_pDataModel->diameter, &m_pDataModel->rms_diameter,&m_pDataModel->max_dev_in_point);
			if(m_pDataModel->show_circle_points() == 1)
			{
						DCP::CircleModel* pModel = new CircleModel(m_pModel);
						pModel->bR = m_pDataModel->bR;
						pModel->cx = m_pDataModel->cx;
						pModel->cy = m_pDataModel->cy;
						pModel->cz = m_pDataModel->cz;

					    pModel->vi = m_pDataModel->vi;
						pModel->vj = m_pDataModel->vj;
						pModel->vk = m_pDataModel->vk;	
						
						pModel->diameter = m_pDataModel->diameter;	
						pModel->PLANE_TYPE = m_pDataModel->PLANE_TYPE;
						pModel->rms_diameter = m_pDataModel->rms_diameter;
						pModel->m_iCounts = m_pDataModel->m_iCounts;
						pModel->max_dev_in_point = m_pDataModel->max_dev_in_point;
						memcpy(&pModel->temp_plane_table[0],&m_pDataModel->temp_plane_table[0],sizeof(S_PLANE_BUFF));
						memcpy(&pModel->planes[0],&m_pDataModel->planes[0],sizeof(S_PLANE_BUFF ));
						memcpy(&pModel->circle_points[0],&m_pDataModel->circle_points[0],sizeof(S_CIRCLE_BUFF ));
						memcpy(&pModel->circle_points_in_plane[0],&m_pDataModel->circle_points_in_plane[0],sizeof(S_CIRCLE_BUFF ));
						
						if(GetController(RES_CIRCLE_CONTROLLER) == nullptr)
						{
							(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::ResCircleController(m_pModel, m_iDisplay) );
						}

						(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DEV_OF_CIRCLE_TOK));

						(void)GetController( RES_CIRCLE_CONTROLLER )->SetModel(pModel);
						SetActiveController(RES_CIRCLE_CONTROLLER, true);
			}
			else
				Close(EC_KEY_ESC);
	}
	else
		Close(EC_KEY_ESC);
}

bool DCP::CalcCircleontrollerC::SetModel( GUI::ModelC* pModel )
{
	m_pModel = dynamic_cast< DCP::Model* >( pModel );
	m_pCommon = new Common(m_pModel);
	return ControllerC::SetModel( pModel );
}

void DCP::CalcCircleontrollerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	(void)lDlgID;
	(void)lExitCode;
}

void DCP::CalcCircleontrollerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::CircleModel* pModel = (DCP::CircleModel*) GetController( RES_CIRCLE_CONTROLLER )->GetModel();	

		m_pDataModel->bR = pModel->bR;
		m_pDataModel->cx = pModel->cx;
		m_pDataModel->cy = pModel->cy;
		m_pDataModel->cz = pModel->cz;
		m_pDataModel->diameter = pModel->diameter;
		m_pDataModel->PLANE_TYPE = pModel->PLANE_TYPE;
		m_pDataModel->rms_diameter = pModel->rms_diameter;
		m_pDataModel->m_iCounts = pModel->m_iCounts;
		m_pDataModel->max_dev_in_point = pModel->max_dev_in_point;
		memcpy(&m_pDataModel->temp_plane_table[0],&pModel->temp_plane_table[0],sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->planes[0],&pModel->planes[0],sizeof(S_PLANE_BUFF ));
		memcpy(&m_pDataModel->circle_points[0],&pModel->circle_points[0],sizeof(S_CIRCLE_BUFF ));
		memcpy(&m_pDataModel->circle_points_in_plane[0],&pModel->circle_points_in_plane[0],sizeof(S_CIRCLE_BUFF ));

		Close(EC_KEY_CONT);
	}
	else
	{
		Close(EC_KEY_ESC);
	}
	DestroyController( lCtrlID );	
}

