// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#include "stdafx.h"
 
#include <dcp06/calculation/CalculationBestFit.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include "calc.h"
#include "core/geometry/Transformations.h"
#include "geometry/Point.h"
#include <math.h>
#include <vector>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::InitializationDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================
 

// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::CalcBestFit::CalcBestFit(BestFitModel* pBestFitModel, Model* pModel): 
	m_pBestFitModel(pBestFitModel),m_pModel(pModel)
{
	
}
// ****************************************************************************************
DCP::CalcBestFit::~CalcBestFit()
{
	
}
// ****************************************************************************************


/**************************************************************
		  check how many points are defined
***************************************************************/
short DCP::CalcBestFit::defined_pom_points(/*point_buff_ *point_OCS,*/ short *lastpoint)
{
short points_defined=0,i;
	
	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pBestFitModel->point_OCS[i].sta == 1 || m_pBestFitModel->point_OCS[i].sta == 2 /* && point_DCS[i].sta != 0 */)
			points_defined++;
		if(m_pBestFitModel->point_OCS[i].sta !=0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return points_defined;
}

/**************************************************************
		  check how many points are defined
***************************************************************/


short DCP::CalcBestFit::get_last_defined_point(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/)
{
short i,ret=0;
	Common common(m_pModel);

	for(i=MAX_BESTFIT_POINTS-1; i >= 0; i--)
	{
		if(m_pBestFitModel->point_OCS[i].sta != 0 || m_pBestFitModel->point_DCS[i].sta != 0 ||
			!common.strblank(m_pBestFitModel->point_OCS[i].point_id)) //&& point_DCS[i].sta != 0)
		{
			ret = i+1;
			break;
		}
	}
	return ret;
} 


/**************************************************************
		  check how many points are defined
***************************************************************/
short DCP::CalcBestFit::get_OCS_points_count(/*point_buff_ *point_OCS*/)
{
short i,ret=0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pBestFitModel->point_OCS[i].sta != 0 ) //&& point_DCS[i].sta != 0)
			ret++;
	}
	return ret;
}


/**************************************************************
		  check how many points are defined
***************************************************************/
short DCP::CalcBestFit::get_OCS_SCS_points_count(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/)
{
short i,ret=0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		//if(point_OCS[i].sta != 0  && point_DCS[i].sta != 0)
		if((m_pBestFitModel->point_OCS[i].sta == 1 ||m_pBestFitModel->point_OCS[i].sta == 2)  && 
			(m_pBestFitModel->point_DCS[i].sta == 1 || m_pBestFitModel->point_DCS[i].sta == 2))
			ret++;
	}
	return ret;
}

/**************************************************************
		  check how many points are defined
***************************************************************/
short DCP::CalcBestFit::get_min_points_count()
{
short ret, DOM_ROTATION;

		DOM_ROTATION = get_rotation_status();
		//ret = (get_LEVELING() == TRUE && DOM_ROTATION == FALSE  && get_HZ_PLANE_ONOFF() == TRUE ? 2: 3);
		ret = (!DOM_ROTATION  && m_pModel->dom_hz_plane ? 2: 3);
		return ret;
}


/************************************************************************
*************************************************************************/
short DCP::CalcBestFit::get_rotation_status()
{
double deg;
short ACTIVE_PLANE, rotation=false;

		ACTIVE_PLANE = m_pModel->dom_active_plane; //get_active_plane_dom();

		if(ACTIVE_PLANE == XY_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.x;//get_x_plane_rot();
			if(deg != 0.0)
			{
			  rotation = true;
			}

			deg = m_pModel->dom_rot_plane_buff.y;//get_y_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}
		}
		else if(ACTIVE_PLANE == ZX_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.x;//get_x_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

			deg = m_pModel->dom_rot_plane_buff.z; //get_z_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}
		}
		else if(ACTIVE_PLANE == YZ_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.y; //get_y_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

			deg = m_pModel->dom_rot_plane_buff.z;//get_z_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

		}
		return rotation;
}


/************************************************************************
*************************************************************************/
short DCP::CalcBestFit::direction_of_point(short plane,double a[4], double b[4], double (*unit)[4])
{
double tmp;

	dir_vector(a, b, unit);

	if(plane == XY_PLANE)
	{
	}
	else if(plane == ZX_PLANE)
	{
		tmp = (*unit)[0];

		(*unit)[0] = (*unit)[2];
		(*unit)[1] = tmp;
	}
	else if(plane == YZ_PLANE)
	{
		(*unit)[0] = (*unit)[1];
		(*unit)[1] = (*unit)[2];
	} 

	/********************************************************
	 0 <= scx <= 1 and  0 <= scy <= 1
	*********************************************************/
	if( ((*unit)[0] >= 0.0 && (*unit)[0] <= 1.0) && ((*unit)[1] >= 0.0 && (*unit)[1] <= 1.0))
	{
		(*unit)[0] = -0.75;
		(*unit)[1] =  0.75;
	}

	/********************************************************
	 -1 <= scx < 0 and  0 <= scy <= 1
	*********************************************************/
	else if( ((*unit)[0] >= -1.0 && (*unit)[0] < 0.0) && ((*unit)[1] >= 0.0 && (*unit)[1] <= 1.0) )
	{
		(*unit)[0] = -0.75;
		(*unit)[1] = -0.75;
	}

	/********************************************************
	 -1 <= scx < 0 and -1 <= scy < 0
	*********************************************************/
	else if(( (*unit)[0] >= -1.0 && (*unit)[0] < 0.0) && ((*unit)[1] >= -1.0 && (*unit)[1] < 0.0) )
	{
		(*unit)[0] =  0.75;
		(*unit)[1] = -0.75;
	}

	/********************************************************
	 0 < scx <= 1 and  -1 <= scy < 0
	*********************************************************/

	else if(((*unit)[0] > 0.0 && (*unit)[0] <= 1.0) && ((*unit)[1] >= -1.0 && (*unit)[1] < 0.0)  )
	{
		(*unit)[0] =  0.75;
		(*unit)[1] = 0.75;
	}

	if(plane == XY_PLANE)
	{
	}
	else if(plane == ZX_PLANE)
	{
		(*unit)[2] = (*unit)[0];
		(*unit)[0] = (*unit)[1];
	}
	else if(plane == YZ_PLANE)
	{
		tmp = (*unit)[1];
		(*unit)[1] = (*unit)[0];
		(*unit)[2] = tmp;
	}
	return 1;
}


/************************************************************************
	Calculate POM and ChangeStation

	Check how many points are defined
	Check if rotation has made in DOM
*************************************************************************/

short DCP::CalcBestFit::calc_transform(/*point_buff_ *point_OCS,		// Object points
					point_buff_ *point_DCS,			// Sensor points
					point_buff_ *point_RES,			// Residuals
				   	double (*ocs_matrix)[4][4],		
				   	double (*ocs_inv_matrix)[4][4],
				   	short pom_chst,					// 0=Pom 1 = Chst
				   	double *rms_x, 
				   	double *rms_y, 
				   	double *rms_z*/) 	
{

short i, points_defined=0, count, DOM_ROTATION,ret;
double mea[MAX_BESTFIT_POINTS*3],nom[MAX_BESTFIT_POINTS*3];
double x[4], xu[4];
double tempx,tempy,tempz;
short sta_OCS,sta_DCS;
	
		MsgBox msgbox;

		/**************************************************************
		  check how many points are defined
		***************************************************************/
		for(i=0; i < MAX_BESTFIT_POINTS; i++)
		{
			sta_OCS = m_pBestFitModel->point_OCS[i].sta;
			sta_DCS = m_pBestFitModel->point_DCS[i].sta;

			if((sta_OCS ==1 || sta_OCS == 2) && (sta_DCS ==1 || sta_DCS == 2))
			// if(point_OCS[i].sta != 0 && point_DCS[i].sta != 0)
				points_defined++;
		}

		DOM_ROTATION = get_rotation_status();

		/**************************************************************
		  2 points levelled
		***************************************************************/
		if(points_defined == 2 /*&& get_LEVELING() == TRUE*/ && DOM_ROTATION == false
			&& m_pModel->dom_hz_plane /*get_HZ_PLANE_ONOFF() == TRUE*/ && m_pBestFitModel->pom_chst != 0)
		{
			calculate_2points_levelled(/*point_OCS, point_DCS, ocs_matrix,pom_chst*/) ;
		}

		/**************************************************************
		  at least 2 point must be defined
		***************************************************************/

		else if(points_defined < 2 &&
				//get_LEVELING() == TRUE &&
				DOM_ROTATION == false &&
				m_pModel->dom_hz_plane/*get_HZ_PLANE_ONOFF() == TRUE*/ && m_pBestFitModel->pom_chst == 1)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_2_POINTS_TOK);
			msgbox.ShowMessageOk(msg);
			//msgbox(TXT_NIL_TOKEN,M_IN_MIN_2_POINTS_TOK,MB_OK);
			return false;
		}

		/**************************************************************
			3 points , levelled
		***************************************************************/

		else if(points_defined == 3 &&/* get_LEVELING() == TRUE &&*/ DOM_ROTATION == false
			&& m_pModel->dom_hz_plane/*get_HZ_PLANE_ONOFF() == TRUE*/ && m_pBestFitModel->pom_chst != 0) 
		{
//			PrintLn("3 points, levelled");
			calculate_3points_levelled(/*point_OCS, point_DCS, ocs_matrix,pom_chst*/);
		}

		/**************************************************************
			3 points , not levelled
		***************************************************************/

		else if (points_defined == 3 &&
				(/*get_LEVELING() == false ||*/
				 DOM_ROTATION/* == true*/ ||
				 m_pModel->dom_hz_plane == false /*get_HZ_PLANE_ONOFF() == FALSE*/ ||
				 m_pBestFitModel->pom_chst == 0))
		{
//			PrintLn("3 points, not levelled");
			calculate_3points_not_levelled(/*point_OCS, point_DCS, ocs_matrix,pom_chst*/);
		}

		/**************************************************************
		  at least 2 point must be defined
		***************************************************************/
		else if(points_defined < 3 && (
				//get_LEVELING() == FALSE ||
				DOM_ROTATION/* == true*/ ||
				m_pModel->dom_hz_plane == false /*get_HZ_PLANE_ONOFF() == FALSE*/ ||
				m_pBestFitModel->pom_chst == 0))
		{

			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_3_POINTS_TOK);
			msgbox.ShowMessageOk(msg);
			//msgbox(TXT_NIL_TOKEN,M_IN_MIN_3_POINTS_TOK,MB_OK);
			return false;
		}

		/**************************************************************
		   > 3 points
		***************************************************************/

		else
		{
			/**********************************************************
				Alloc memory for table
			***********************************************************/
		
			if(nom != nullptr && mea != nullptr)
			{
				count = 0;

				for(i=0; i < MAX_BESTFIT_POINTS; i++)
				{
					sta_OCS = m_pBestFitModel->point_OCS[i].sta;
					sta_DCS = m_pBestFitModel->point_DCS[i].sta;

					if((sta_OCS ==1 || sta_OCS == 2) && (sta_DCS ==1 || sta_DCS == 2))

					// if(point_OCS[i].sta != 0 && point_DCS[i].sta != 0)
					{
						mea[count*3+0] = m_pBestFitModel->point_DCS[i].x;
						mea[count*3+1] = m_pBestFitModel->point_DCS[i].y;
						mea[count*3+2] = m_pBestFitModel->point_DCS[i].z;

						nom[count*3+0] = m_pBestFitModel->point_OCS[i].x;
						nom[count*3+1] = m_pBestFitModel->point_OCS[i].y;
						nom[count*3+2] = m_pBestFitModel->point_OCS[i].z;

						count++;
					}
				}

				/****************
					Calculate - DCP9 solveRigidTransform (Kabsch)
				****************/
				std::vector<DCP9::Geometry::Point> sourcePts, targetPts;
				for(int idx=0; idx<points_defined; idx++) {
					sourcePts.push_back(DCP9::Geometry::Point(mea[idx*3+0], mea[idx*3+1], mea[idx*3+2]));
					targetPts.push_back(DCP9::Geometry::Point(nom[idx*3+0], nom[idx*3+1], nom[idx*3+2]));
				}
				DCP9::Core::Geometry::RigidTransformResult transformResult = DCP9::Core::Geometry::solveRigidTransform(sourcePts, targetPts, true);
				ret = transformResult.isValid ? 1 : -1;
				if(ret == 1)
				{
					const Eigen::Matrix3d& R = transformResult.rotation;
					const Eigen::Vector3d& t = transformResult.translation;
					m_pBestFitModel->matrix[0][0]=R(0,0); m_pBestFitModel->matrix[0][1]=R(0,1); m_pBestFitModel->matrix[0][2]=R(0,2); m_pBestFitModel->matrix[0][3]=t(0);
					m_pBestFitModel->matrix[1][0]=R(1,0); m_pBestFitModel->matrix[1][1]=R(1,1); m_pBestFitModel->matrix[1][2]=R(1,2); m_pBestFitModel->matrix[1][3]=t(1);
					m_pBestFitModel->matrix[2][0]=R(2,0); m_pBestFitModel->matrix[2][1]=R(2,1); m_pBestFitModel->matrix[2][2]=R(2,2); m_pBestFitModel->matrix[2][3]=t(2);
					m_pBestFitModel->matrix[3][0]=0; m_pBestFitModel->matrix[3][1]=0; m_pBestFitModel->matrix[3][2]=0; m_pBestFitModel->matrix[3][3]=1;
					m_pBestFitModel->ocs_defined = true;
				}
				else if(ret == -1)
				{
					m_pBestFitModel->ocs_defined = false;	
					//set_ocs_defined(FALSE, pom_chst);
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					//msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				}
				else
				{
					m_pBestFitModel->ocs_defined = false;	
					//set_ocs_defined(FALSE,pom_chst);
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_ABORTED_TOK);
					msgbox.ShowMessageOk(msg);

					//msgbox(TXT_NIL_TOKEN, M_ABORTED_TOK,MB_OK);
				}
			}
			else
			{
				m_pBestFitModel->ocs_defined = false;	
				//set_ocs_defined(FALSE,pom_chst);
			}
		}

		if(m_pBestFitModel->ocs_defined/*get_ocs_defined(pom_chst) == TRUE*/)
		{
			//show_matrix4x4(ocsp_matrix);
			/*******************************
				calculate_residuals & RMS
			********************************/
			count = 0;

			tempx = 0.0;
			tempy = 0.0;
			tempz = 0.0;
			/*
			*rms_x = 0.0;
			*rms_y = 0.0;
			*rms_z = 0.0;
			*/
			for(i=0; i < MAX_BESTFIT_POINTS; i++)
			{
				sta_OCS = m_pBestFitModel->point_OCS[i].sta;
				sta_DCS = m_pBestFitModel->point_DCS[i].sta;

				if((sta_OCS ==1 || sta_OCS == 2) && (sta_DCS ==1 || sta_DCS == 2))

				//if(point_OCS[i].sta != 0 && point_DCS[i].sta != 0)
				{
					count++;

					x[0] = m_pBestFitModel->point_DCS[i].x;
					x[1] = m_pBestFitModel->point_DCS[i].y;
					x[2] = m_pBestFitModel->point_DCS[i].z;
					x[3] = 1;

					ptrans(x, &m_pBestFitModel->matrix[0], &xu);

					m_pBestFitModel->point_RES[i].x = xu[0] - m_pBestFitModel->point_OCS[i].x;
					m_pBestFitModel->point_RES[i].y = xu[1] - m_pBestFitModel->point_OCS[i].y;
					m_pBestFitModel->point_RES[i].z = xu[2] - m_pBestFitModel->point_OCS[i].z;
					m_pBestFitModel->point_RES[i].sta = POINT_DESIGN;

					tempx = tempx +(m_pBestFitModel->point_RES[i].x * m_pBestFitModel->point_RES[i].x);
					tempy = tempy +(m_pBestFitModel->point_RES[i].y * m_pBestFitModel->point_RES[i].y);
					tempz = tempz +(m_pBestFitModel->point_RES[i].z * m_pBestFitModel->point_RES[i].z);
				}
				//else if((sta_OCS == 3 || sta_OCS == 4) && (sta_DCS ==3 || sta_DCS == 4))

				// Modified 14/6/99
				else if( ((sta_OCS == 3 || sta_OCS == 4) && (sta_DCS ==3 || sta_DCS == 4)) ||
						 ((sta_OCS == 3 || sta_OCS == 4) && (sta_DCS == 1 || sta_DCS == 2)) ||
						 (( sta_OCS == 1 || sta_OCS == 2) && (sta_DCS == 3 || sta_DCS == 4)))	 			
								
				{
					m_pBestFitModel->point_RES[i].sta = POINT_DESIGN_REJECTED;
				}
				else
				{
					m_pBestFitModel->point_RES[i].sta = POINT_NOT_DEFINED;
				}
			}

			m_pBestFitModel->rms_x = sqrt(tempx / count);
			m_pBestFitModel->rms_y = sqrt(tempy / count);
			m_pBestFitModel->rms_z = sqrt(tempz / count);
			/*
			PrintLn("Rmsx:%*.2f",*rms_x);
			PrintLn("Rmsy:%*.2f",*rms_y);
			PrintLn("Rmsz:%*.2f",*rms_z);
			*/
			matinv4x4(&m_pBestFitModel->matrix[0]/*[0]*/, &m_pBestFitModel->inv_matrix);

			if(m_pBestFitModel->pom_chst == 0) // POM
			{
				/*
				set_ocsp_matrix((*ocs_matrix)[0][0], (*ocs_matrix)[1][0],(*ocs_matrix)[2][0],(*ocs_matrix)[3][0],
								(*ocs_matrix)[0][1], (*ocs_matrix)[1][1],(*ocs_matrix)[2][1],(*ocs_matrix)[3][1],
								(*ocs_matrix)[0][2], (*ocs_matrix)[1][2],(*ocs_matrix)[2][2],(*ocs_matrix)[3][2],
								(*ocs_matrix)[0][3], (*ocs_matrix)[1][3],(*ocs_matrix)[2][3],(*ocs_matrix)[3][3]);
				
			
			
				set_ocsp_inv_matrix(
							(*ocs_inv_matrix)[0][0], (*ocs_inv_matrix)[1][0],(*ocs_inv_matrix)[2][0],(*ocs_inv_matrix)[3][0],
							(*ocs_inv_matrix)[0][1], (*ocs_inv_matrix)[1][1],(*ocs_inv_matrix)[2][1],(*ocs_inv_matrix)[3][1],
							(*ocs_inv_matrix)[0][2], (*ocs_inv_matrix)[1][2],(*ocs_inv_matrix)[2][2],(*ocs_inv_matrix)[3][2],
							(*ocs_inv_matrix)[0][3], (*ocs_inv_matrix)[1][3],(*ocs_inv_matrix)[2][3],(*ocs_inv_matrix)[3][3]);
				*/	

			}
			else			  // CHST
			{
				/*
				set_ocsc_matrix((*ocs_matrix)[0][0], (*ocs_matrix)[1][0],(*ocs_matrix)[2][0],(*ocs_matrix)[3][0],
								(*ocs_matrix)[0][1], (*ocs_matrix)[1][1],(*ocs_matrix)[2][1],(*ocs_matrix)[3][1],
								(*ocs_matrix)[0][2], (*ocs_matrix)[1][2],(*ocs_matrix)[2][2],(*ocs_matrix)[3][2],
								(*ocs_matrix)[0][3], (*ocs_matrix)[1][3],(*ocs_matrix)[2][3],(*ocs_matrix)[3][3]);
						
				set_ocsc_inv_matrix(
							(*ocs_inv_matrix)[0][0], (*ocs_inv_matrix)[1][0],(*ocs_inv_matrix)[2][0],(*ocs_inv_matrix)[3][0],
							(*ocs_inv_matrix)[0][1], (*ocs_inv_matrix)[1][1],(*ocs_inv_matrix)[2][1],(*ocs_inv_matrix)[3][1],
							(*ocs_inv_matrix)[0][2], (*ocs_inv_matrix)[1][2],(*ocs_inv_matrix)[2][2],(*ocs_inv_matrix)[3][2],
							(*ocs_inv_matrix)[0][3], (*ocs_inv_matrix)[1][3],(*ocs_inv_matrix)[2][3],(*ocs_inv_matrix)[3][3]);
				*/
			}
		}

//		ACTIVE_PNO = 1;

		// show_values(1,TRUE);

		if(m_pBestFitModel->ocs_defined/*get_ocs_defined(pom_chst) == TRUE*/)
		{
//				sprintf(msgTxt,"%-s \n\033V %-s %6.1f \n\033V %-s %6.1f \n\033V %-s %6.1f",
//						   "RMS of Calculation", RMS_X_TEXT, rms_x, RMS_Y_TEXT,rms_y, RMS_Z_TEXT,rms_z);
//				show_message(msgTxt); //ms removed

				
				//set_ocs_defined(TRUE,pom_chst);
		}
/*
		if(SHOW_INFO == 1)
		{
		   show_matrix4x4(ocs_matrix);
		}
*/
		return (m_pBestFitModel->ocs_defined/*get_ocs_defined(pom_chst)*/);

}


/************************************************************************
	3 POINTS, LEVELLED
*************************************************************************/
short DCP::CalcBestFit::calculate_3points_levelled(/*point_buff_ *point_OCS,	// Object points
								point_buff_ *point_DCS,		// Sensor
								double (*ocs_matrix)[4][4],
							    short pom_chst*/)				// 0=Pom 1 = Chst)
{
short i, count, first, second, third;
double mea1[4], mea2[4], mea3[4];
double nom1[4], nom2[4], nom3[4];
double temp11[4][4], temp21[4][4], temp21_inv[4][4];
double temp31[4][4];
double temp12[4][4], temp22[4][4], temp22_inv[4][4];
double temp32[4][4];
double unit[4];
double ocsp11[4][4], ocsp12[4][4], ocsp22[4][4];
double length;
short active_plane,sta_OCS,sta_DCS;
//double dcs_matrix[4][4];
				
			MsgBox msgbox;
//			PrintLn("3 points, levelled");
			active_plane = m_pModel->dom_active_plane;//get_active_plane_dom();

			/***********************
				Find defined points
			************************/
			count = 0;

			for(i=0; i < MAX_BESTFIT_POINTS; i++)
			{
				sta_OCS = m_pBestFitModel->point_OCS[i].sta;
				sta_DCS = m_pBestFitModel->point_DCS[i].sta;	
				
				if( (sta_OCS== 1 || sta_OCS== 2) && (sta_DCS == 1 || sta_DCS== 2))
				{
					count++;

					if(count ==1)
					{
//						PrintLn("1. no=%d",i);
						first = i;
					}

					else if(count ==2)
					{
//						PrintLn("2. no=%d",i);
						second = i;
					}

					else if(count ==3)
					{
//						PrintLn("3. no=%d",i);
						third = i;
						break;
					}
				}
			}

			/**************************************
			   1. Calculate
				use 1 and 2 measured points
			**************************************/
			mea1[0] = m_pBestFitModel->point_DCS[first].x;
			mea1[1] = m_pBestFitModel->point_DCS[first].y;
			mea1[2] = m_pBestFitModel->point_DCS[first].z;

			nom1[0]	= m_pBestFitModel->point_OCS[first].x;
			nom1[1] = m_pBestFitModel->point_OCS[first].y;
			nom1[2] = m_pBestFitModel->point_OCS[first].z;

			mea2[0] = m_pBestFitModel->point_DCS[second].x;
			mea2[1] = m_pBestFitModel->point_DCS[second].y;
			mea2[2] = m_pBestFitModel->point_DCS[second].z;

			nom2[0]	= m_pBestFitModel->point_OCS[second].x;
			nom2[1] = m_pBestFitModel->point_OCS[second].y;
			nom2[2] = m_pBestFitModel->point_OCS[second].z;

			/*********************************************************
				 SCS
			**********************************************************/
			length = sqrt( ( (mea2[0]- mea1[0])*(mea2[0]- mea1[0]) ) +
						   ( (mea2[1]- mea1[1])*(mea2[1]- mea1[1]) ) +
						   ( (mea2[2]- mea1[2])*(mea2[2]- mea1[2]) ) );

			if(length ==0.0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				//msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				
				//set_ocs_defined(FALSE,pom_chst);
				return false;
			}

			direction_of_point(XY_PLANE, mea1, mea2, &unit);
			/*

			if(active_plane == XY_PLANE)
			{
			*/
				mea3[0] = mea1[0] + length * unit[0];
				mea3[1] = mea1[1] + length * unit[1];
				mea3[2] = mea1[2];

				mea2[2] = mea1[2];  //p2z = p1z
			/*
			}
			else if(active_plane == ZX_PLANE)
			{

				mea3[0] = mea1[0] + length * unit[0]; //p1x + L*nscy
				mea3[1] = mea1[1];
				mea3[2] = mea1[2] + length * unit[2];

				mea2[1] = mea1[1];  //p2y = p1y

			}
			else if(active_plane == YZ_PLANE)
			{

				mea3[0] = mea1[0]; 	//p3x=p1x
				mea3[1] = mea1[1] + length * unit[1];
				mea3[2] = mea1[2] + length * unit[2];

				mea2[0] = mea1[0];  //p2x = p1x
			}

			*/

			/*********************************************************
				 OCS
			**********************************************************/
			length = sqrt( ( (nom2[0]- nom1[0])*(nom2[0]- nom1[0]) ) +
						   ( (nom2[1]- nom1[1])*(nom2[1]- nom1[1]) ) +
						   ( (nom2[2]- nom1[2])*(nom2[2]- nom1[2])) );

			if(length ==0.0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				return false;
				/*
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				return FALSE;
				*/
			}

			direction_of_point(active_plane, nom1, nom2,&unit);

			if(active_plane == XY_PLANE)
			{
				nom3[0] = nom1[0] + length * unit[0];
				nom3[1] = nom1[1] + length * unit[1];
				nom3[2] = nom1[2];

				nom2[2] = nom1[2];  //p2z = p1z
			}
			else if(active_plane == ZX_PLANE)
			{
				nom3[0] = nom1[0] + length * unit[0]; 	//p1x + L*nscy
				nom3[1] = nom1[1];
				nom3[2] = nom1[2] + length * unit[2];

				nom2[1] = nom1[1];  					//p2y = p1y
			}
			else if(active_plane == YZ_PLANE)
			{
				nom3[0] = nom1[0]; 						//p3x=p1x
				nom3[1] = nom1[1] + length * unit[1];
				nom3[2] = nom1[2] + length * unit[2];

				nom2[0] = nom1[0];  					//p2x = p1x
			}

			htransm(mea1, mea2, mea3, 'y', &temp11);
			htransm(nom1, nom2, nom3, 'y', &temp21);

			//get_dcs_table(&dcs_matrix);

			if(matinv4x4(temp21, &temp21_inv) != -1)
			{
				matmul4x4(m_pModel->dcs_matrix, temp11,&temp31); 		/* dcs_matrix is unitmatrix */
				matmul4x4(temp31, temp21_inv, &ocsp11);
			}

			/***************************************
			   Cannot calculate
			***************************************/
			else
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				return false;
				/*
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				return FALSE;
				*/
			}
/*
			if(SHOW_INFO)
			{
				show_matrix4x4(ocsp11);
			}
*/
			/**************************************
			   2. Calculate
				use 1 and 3 measured points
			**************************************/

			mea1[0] = m_pBestFitModel->point_DCS[first].x;
			mea1[1] = m_pBestFitModel->point_DCS[first].y;
			mea1[2] = m_pBestFitModel->point_DCS[first].z;

			nom1[0]	= m_pBestFitModel->point_OCS[first].x;
			nom1[1] = m_pBestFitModel->point_OCS[first].y;
			nom1[2] = m_pBestFitModel->point_OCS[first].z;

			mea2[0] = m_pBestFitModel->point_DCS[third].x;
			mea2[1] = m_pBestFitModel->point_DCS[third].y;
			mea2[2] = m_pBestFitModel->point_DCS[third].z;

			nom2[0]	= m_pBestFitModel->point_OCS[third].x;
			nom2[1] = m_pBestFitModel->point_OCS[third].y;
			nom2[2] = m_pBestFitModel->point_OCS[third].z;

			/*********************************************************
				 SCS
			**********************************************************/

			length = sqrt( ( (mea2[0]- mea1[0])*(mea2[0]- mea1[0]) ) +
						   ( (mea2[1]- mea1[1])*(mea2[1]- mea1[1]) ) +
						   ( (mea2[2]- mea1[2])*(mea2[2]- mea1[2]) ) );

			if(length ==0.0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				return false;
				/*
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				return FALSE;
				*/
			}

			direction_of_point(XY_PLANE, mea1, mea2, &unit);

			/*
			if(active_plane == XY_PLANE)
			{

			*/
				mea3[0] = mea1[0] + length * unit[0];
				mea3[1] = mea1[1] + length * unit[1];
				mea3[2] = mea1[2];

				mea2[2] = mea1[2];  					//p2z = p1z
			/*
			}
			else if(active_plane == ZX_PLANE)
			{

				mea3[0] = mea1[0] + length * unit[0]; 	//p1x + L*nscy
				mea3[1] = mea1[1];
				mea3[2] = mea1[2] + length * unit[2];

				mea2[1] = mea1[1];  					//p2y = p1y


			}
			else if(active_plane == YZ_PLANE)
			{

				mea3[0] = mea1[0]; 						//p3x=p1x
				mea3[1] = mea1[1] + length * unit[1];
				mea3[2] = mea1[2] + length * unit[2];

				mea2[0] = mea1[0];  					//p2x = p1x
			}
			  */

			/*********************************************************
				 OCS
			**********************************************************/
			length = sqrt( ( (nom2[0]- nom1[0])*(nom2[0]- nom1[0]) ) +
						   ( (nom2[1]- nom1[1])*(nom2[1]- nom1[1]) ) +
						   ( (nom2[2]- nom1[2])*(nom2[2]- nom1[2])) );

			if(length ==0.0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				return false;
				/*	
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				return FALSE;
				*/
			}

			direction_of_point(active_plane, nom1, nom2, &unit);

			if(active_plane == XY_PLANE)
			{
				nom3[0] = nom1[0] + length * unit[0];
				nom3[1] = nom1[1] + length * unit[1];
				nom3[2] = nom1[2];

				nom2[2] = nom1[2];  					//p2z = p1z
			}
			else if(active_plane == ZX_PLANE)
			{
				nom3[0] = nom1[0] + length * unit[0]; 	//p1x + L*nscy
				nom3[1] = nom1[1];
				nom3[2] = nom1[2] + length * unit[2];

				nom2[1] = nom1[1];  					//p2y = p1y
			}
			else if(active_plane == YZ_PLANE)
			{
				nom3[0] = nom1[0]; 						//p3x=p1x
				nom3[1] = nom1[1] + length * unit[1];
				nom3[2] = nom1[2] + length * unit[2];

				nom2[0] = nom1[0];  					//p2x = p1x
			}

			htransm(mea1, mea2, mea3, 'y', &temp12);
			htransm(nom1, nom2, nom3, 'y', &temp22);

			//get_dcs_table(&dcs_matrix);

			if(matinv4x4(temp22, &temp22_inv) != -1)
			{
				matmul4x4(m_pModel->dcs_matrix, temp12,&temp32); 		/* dcs_matrix is unitmatrix */
				matmul4x4(temp32, temp22_inv, &ocsp12);
/*
				if(SHOW_INFO)
				{
					show_matrix4x4(ocsp12);
				}
*/
				matsum_n(&ocsp11[0][0],4,4, &ocsp12[0][0], &ocsp22[0][0]);

				m_pBestFitModel->matrix[0][0] = ocsp22[0][0] / 2;
				m_pBestFitModel->matrix[0][1] = ocsp22[0][1] / 2;
				m_pBestFitModel->matrix[0][2] = ocsp22[0][2] / 2;
				m_pBestFitModel->matrix[0][3] = ocsp22[0][3] / 2;

				m_pBestFitModel->matrix[1][0] = ocsp22[1][0] / 2;
				m_pBestFitModel->matrix[1][1] = ocsp22[1][1] / 2;
				m_pBestFitModel->matrix[1][2] = ocsp22[1][2] / 2;
				m_pBestFitModel->matrix[1][3] = ocsp22[1][3] / 2;

				m_pBestFitModel->matrix[2][0] = ocsp22[2][0] / 2;
				m_pBestFitModel->matrix[2][1] = ocsp22[2][1] / 2;
				m_pBestFitModel->matrix[2][2] = ocsp22[2][2] / 2;
				m_pBestFitModel->matrix[2][3] = ocsp22[2][3] / 2;

				m_pBestFitModel->matrix[3][0] = ocsp22[3][0] / 2;
				m_pBestFitModel->matrix[3][1] = ocsp22[3][1] / 2;
				m_pBestFitModel->matrix[3][2] = ocsp22[3][2] / 2;
				m_pBestFitModel->matrix[3][3] = ocsp22[3][3] / 2;
				m_pBestFitModel->ocs_defined = true;
				//set_ocs_defined(TRUE,pom_chst);
			}

			/***************************************
			   Cannot calculate
			***************************************/
			else
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				m_pBestFitModel->ocs_defined = false;
				/*
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				*/
			}
			return m_pBestFitModel->ocs_defined; //get_ocs_defined(pom_chst);
}

/************************************************************************
	2 POINTS, LEVELLED
*************************************************************************/
short DCP::CalcBestFit::calculate_2points_levelled(/*point_buff_ *point_OCS,		// Object points
								point_buff_ *point_DCS,			// Sensor
							   double (*ocs_matrix)[4][4],		
							   short pom_chst*/)					// 0=Pom 1 = Chst))
{
short i, count, first, second;
double mea1[4], mea2[4], mea3[4];
double nom1[4], nom2[4], nom3[4];
double temp1[4][4], temp2[4][4], temp2_inv[4][4];
double temp3[4][4];
double length;
short active_plane,sta_OCS,sta_DCS;
double unit[4];
//double dcs_matrix[4][4];

			MsgBox msgbox;
			active_plane = m_pModel->dom_active_plane;//get_active_plane_dom();

			/************************
			 find last defined point
			*************************/
			count = 0;

			for(i=0; i < MAX_BESTFIT_POINTS; i++)
			{
					
				sta_OCS = m_pBestFitModel->point_OCS[i].sta;
				sta_DCS =m_pBestFitModel->point_DCS[i].sta;

				if((sta_OCS ==1 || sta_OCS == 2) && (sta_DCS ==1 || sta_DCS == 2))
				{
					count++;

					if(count == 1)
						first = i;

					if(count == 2)
					{
						second = i;
						break;
					}
				}
			}


				/************************
					set the 3.points values
				*************************/
				mea1[0] = m_pBestFitModel->point_DCS[first].x;
				mea2[0] = m_pBestFitModel->point_DCS[second].x;

				mea1[1] = m_pBestFitModel->point_DCS[first].y;
				mea2[1] = m_pBestFitModel->point_DCS[second].y;

				mea1[2] = m_pBestFitModel->point_DCS[first].z;
				mea2[2] = m_pBestFitModel->point_DCS[second].z;

				length = sqrt( ( (mea2[0]- mea1[0])*(mea2[0]- mea1[0]) ) +
							   ( (mea2[1]- mea1[1])*(mea2[1]- mea1[1]) ) +
							   ( (mea2[2]- mea1[2])*(mea2[2]- mea1[2]) ) );

				if(length ==0.0)
				{
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					m_pBestFitModel->ocs_defined = false;
					return false;
					/*
					msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
					set_ocs_defined(FALSE,pom_chst);
					return FALSE;
					*/
				}

				direction_of_point(XY_PLANE, mea1, mea2,&unit);

				/*
				if(active_plane == XY_PLANE)
				{

				*/
					mea3[0] = mea1[0] + length * unit[0];
					mea3[1] = mea1[1] + length * unit[1];
					mea3[2] = mea1[2];

					mea2[2] = mea1[2];  //p2z = p1z
				/*
				}
				else if(active_plane == ZX_PLANE)
				{

					mea3[0] = mea1[0] + length * unit[0]; //p1x + L*nscy
					mea3[1] = mea1[1];
					mea3[2] = mea1[2] + length * unit[2];

					mea2[1] = mea1[1];  //p2y = p1y


				}
				else if(active_plane == YZ_PLANE)
				{

					mea3[0] = mea1[0]; 	//p3x=p1x
					mea3[1] = mea1[1] + length * unit[1];
					mea3[2] = mea1[2] + length * unit[2];

					mea2[0] = mea1[0];  //p2x = p1x
				}

				*/
				/*****************************
					 OCS
				******************************/
				nom1[0] = m_pBestFitModel->point_OCS[first].x;
				nom2[0] = m_pBestFitModel->point_OCS[second].x;

				nom1[1] = m_pBestFitModel->point_OCS[first].y;
				nom2[1] = m_pBestFitModel->point_OCS[second].y;

				nom1[2] = m_pBestFitModel->point_OCS[first].z;
				nom2[2] = m_pBestFitModel->point_OCS[second].z;

				length = sqrt( ( (nom2[0]- nom1[0])*(nom2[0]- nom1[0]) ) +
							   ( (nom2[1]- nom1[1])*(nom2[1]- nom1[1]) ) +
							   ( (nom2[2]- nom1[2])*(nom2[2]- nom1[2])) );

				if(length ==0.0)
				{
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					m_pBestFitModel->ocs_defined = false;
					return false;
					//msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
					//set_ocs_defined(FALSE,pom_chst);
					//return FALSE;
				}

				direction_of_point(active_plane, nom1, nom2, &unit);

				/***************************************

					Set 3. points values
				****************************************/
				if(active_plane == XY_PLANE)
				{
					nom3[0] = nom1[0] + length * unit[0]; 	// p3x = p1x + length * nscx
					nom3[1] = nom1[1] + length * unit[1]; 	// p3y = p1y + length * nscy
					nom3[2] = nom1[2];                    	// p3z = p1z

					nom2[2] = nom1[2];  				  	// p2z = p1z
				}
				else if(active_plane == ZX_PLANE)
				{
					nom3[0] = nom1[0] + length * unit[0]; 	// p3x = p1x + length * nscy
					nom3[1] = nom1[1];                    	// p3y = p1y
					nom3[2] = nom1[2] + length * unit[2]; 	// p3z = p1z + length * nscz

					nom2[1] = nom1[1];  				  	// p2y = p1y
				}
				else if(active_plane == YZ_PLANE)
				{
					nom3[0] = nom1[0]; 						// p3x = p1x
					nom3[1] = nom1[1] + length * unit[1];   // p3y = p1y + length * nscy
					nom3[2] = nom1[2] + length * unit[2];   // p3z = p1z + length * nscz

					nom2[0] = nom1[0];  					// p2x = p1x
				}

				/**************************************
					Calculate
				**************************************/
				htransm(mea1, mea2, mea3, 'y', &temp1);
				htransm(nom1, nom2, nom3, 'y', &temp2);

				//get_dcs_table(&dcs_matrix);

				if(matinv4x4(temp2, &temp2_inv) != -1)
				{
					matmul4x4(m_pModel->dcs_matrix, temp1,&temp3); 		/* dcs_matrix is unitmatrix */
					matmul4x4(temp3, temp2_inv, &m_pBestFitModel->matrix);
					m_pBestFitModel->ocs_defined = true;
					//set_ocs_defined(TRUE,pom_chst);
				}

			/***************************************
			   Cannot calculate
			***************************************/
			else
			{
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					m_pBestFitModel->ocs_defined = false;
				/*

				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);
				set_ocs_defined(FALSE,pom_chst);
				*/
			}
		return (m_pBestFitModel->ocs_defined);
}

/************************************************************************
	3 POINTS, NOT LEVELLED
*************************************************************************/
short DCP::CalcBestFit::calculate_3points_not_levelled(/*point_buff_ *point_OCS,	// Object points
									point_buff_ *point_DCS,	// Sensor
								   double (*ocs_matrix)[4][4],		
								   short pom_chst*/)	// 0=Pom 1 = Chst))
{
short i,  count,sta_OCS,sta_DCS;
double mea1[4], mea2[4], mea3[4];
double nom1[4], nom2[4], nom3[4];
double temp1[4][4], temp2[4][4], temp2_inv[4][4];
double temp3[4][4];
//double dcs_matrix[4][4];

			/***********************
				Find defined points
			************************/
		//	PrintLn("3 points not levelled ");
			count = 0;

			for(i=0; i < MAX_BESTFIT_POINTS; i++)
			{
				sta_OCS =m_pBestFitModel->point_OCS[i].sta;
				sta_DCS =m_pBestFitModel->point_DCS[i].sta;

				if((sta_OCS ==1 || sta_OCS == 2) && (sta_DCS ==1 || sta_DCS == 2))

				//if(point_OCS[i].sta != 0 && point_DCS[i].sta != 0)
				{
					count++;

					if(count ==1)
					{
						mea1[0] = m_pBestFitModel->point_DCS[i].x;
						mea1[1] = m_pBestFitModel->point_DCS[i].y;
						mea1[2] = m_pBestFitModel->point_DCS[i].z;

						nom1[0]	= m_pBestFitModel->point_OCS[i].x;
						nom1[1] = m_pBestFitModel->point_OCS[i].y;
						nom1[2] = m_pBestFitModel->point_OCS[i].z;
					}

					else if(count ==2)
					{
						mea2[0] = m_pBestFitModel->point_DCS[i].x;
						mea2[1] = m_pBestFitModel->point_DCS[i].y;
						mea2[2] = m_pBestFitModel->point_DCS[i].z;

						nom2[0]	= m_pBestFitModel->point_OCS[i].x;
						nom2[1] = m_pBestFitModel->point_OCS[i].y;
						nom2[2] = m_pBestFitModel->point_OCS[i].z;
					}

					else if(count ==3)
					{
						mea3[0] = m_pBestFitModel->point_DCS[i].x;
						mea3[1] = m_pBestFitModel->point_DCS[i].y;
						mea3[2] = m_pBestFitModel->point_DCS[i].z;

						nom3[0]	= m_pBestFitModel->point_OCS[i].x;
						nom3[1] = m_pBestFitModel->point_OCS[i].y;
						nom3[2] = m_pBestFitModel->point_OCS[i].z;
						break;
					}
				}
			}

			/**************************************
				Calculate
			**************************************/
			htransm(mea1, mea2, mea3, 'y', &temp1);
			htransm(nom1, nom2, nom3, 'y', &temp2);

			if(matinv4x4(temp2, &temp2_inv) != -1)
			{
				//get_dcs_table(&dcs_matrix);

				matmul4x4(m_pModel->dcs_matrix, temp1,&temp3); 		/* dcs_matrix is unitmatrix */
				matmul4x4(temp3, temp2_inv, &m_pBestFitModel->matrix);
				m_pBestFitModel->ocs_defined = true;
				//set_ocs_defined(TRUE,pom_chst);
			}

			/***************************************
			   Cannot calculate
			***************************************/
			else
			{

				m_pBestFitModel->ocs_defined = false;
				/*
				msgbox(TXT_NIL_TOKEN, M_CANNOT_CALC_TOK,MB_OK);	
				set_ocs_defined(FALSE,pom_chst);
				*/
			}
			return m_pBestFitModel->ocs_defined; //(get_ocs_defined(pom_chst));
}



