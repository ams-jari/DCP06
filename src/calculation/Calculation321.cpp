// ================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright 2002 by Leica Geosystems AG, Heerbrugg
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

#include <dcp06/calculation/Calculation321.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include "calc.h"

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
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06CalcDomC::DCP06CalcDomC(DCP06DomModelC* pDCP06DomModel)://,DCP06ModelC* pDCP06Model): 
	m_pDCP06DomModel(pDCP06DomModel)//,m_pDCP06Model(pDCP06Model)
{
	
}
// ****************************************************************************************
DCP::DCP06CalcDomC::~DCP06CalcDomC()
{
	
}
// ****************************************************************************************
bool DCP::DCP06CalcDomC::calc(bool show_message)
{
double nz[4]; 		// normal of xy-plane
double ny[4];		// normal of xz-plane
double nx[4];		// normal of yz-plane

double n_line[4];  	// normal of line

double temp1[4];
double tempmat2[4][4], tempmat3[4][4];

double deg, rad;
double point1[4], point2[4], point3[4];
double ovalues[4];
//double ocsd_table[4][4], ocsd_inv_table[4][4];
S_PLANE_BUFF *pPlane;

StringC msg;

	DCP06MsgBoxC msgbox;
	
	if(m_pDCP06DomModel->dom_hz_plane)
		pPlane = &m_pDCP06DomModel->dom_hz_plane_buff[0];
	else
		pPlane = &m_pDCP06DomModel->dom_plane_buff[0];

	if(pPlane[0].calc == 0)
	{	
		msg.LoadTxt(AT_DCP05, M_DCP_DEFINE_REF_PLANE_TOK);
		msgbox.ShowMessageOk(msg);
		return false;
	}

	if(m_pDCP06DomModel->dom_line_buff[0].calc == 0)
	{
		msg.LoadTxt(AT_DCP05, M_DCP_DEFINE_REF_LINE_TOK);
		msgbox.ShowMessageOk(msg);
		return false;
	}

	if(m_pDCP06DomModel->dom_ref_point_buff.sta == 0)
	{
		msg.LoadTxt(AT_DCP05, M_DCP_DEFINE_REF_POINT_TOK);
		msgbox.ShowMessageOk(msg);
		return false;
	}


	if(m_pDCP06DomModel->dom_ovalues_buff.sta == 0)
	{
		m_pDCP06DomModel->dom_ovalues_buff.x = 0.0;
		m_pDCP06DomModel->dom_ovalues_buff.y = 0.0;
		m_pDCP06DomModel->dom_ovalues_buff.z = 0.0;
	}

	n_line[0] = m_pDCP06DomModel->dom_line_buff[0].ux;
	n_line[1] = m_pDCP06DomModel->dom_line_buff[0].uy;
	n_line[2] = m_pDCP06DomModel->dom_line_buff[0].uz;
	n_line[3] = 1;


	/* ***********************************
		XY-PLANE
	************************************ */


	if(m_pDCP06DomModel->dom_active_plane == XY_PLANE)
	{
		// normal of xy-plane

		nz[0] = pPlane[0].nx;
		nz[1] = pPlane[0].ny;
		nz[2] = pPlane[0].nz;
		nz[3] = 1.0;

		// normal of x-line
		if(m_pDCP06DomModel->dom_active_line == X_LINE)
		{
			// ny
			vec_product(nz, n_line, &temp1);
			unit_vector(temp1, &ny);

			// nx
			vec_product(ny, nz, &temp1);
			unit_vector(temp1, &nx);

		}
		else if(m_pDCP06DomModel->dom_active_line == Y_LINE)
		{
			// nx
			vec_product(n_line, nz, &temp1);
			unit_vector(temp1, &nx);

			// ny
			vec_product(nz, nx, &temp1);
			unit_vector(temp1, &ny);
		}

	}

	/************************************
		XZ-PLANE
	*************************************/
	else if(m_pDCP06DomModel->dom_active_plane == ZX_PLANE)
	{
		// normal of xz-plane

		ny[0] = pPlane[0].nx;
		ny[1] = pPlane[0].ny;
		ny[2] = pPlane[0].nz;
		ny[3] = 1.0;


		if(m_pDCP06DomModel->dom_active_line == X_LINE)
		{

			// nz
			vec_product(n_line, ny, &temp1);
			unit_vector(temp1, &nz);

			// nx
			vec_product(ny, nz, &temp1);
			unit_vector(temp1, &nx);

		}
		else if(m_pDCP06DomModel->dom_active_line == Z_LINE)
		{
			// nx
			vec_product(ny, n_line, &temp1);
			unit_vector(temp1, &nx);

			// nz
			vec_product(nx, ny, &temp1);
			unit_vector(temp1, &nz);
		}
	}

	/************************************
		YZ-PLANE
	*************************************/
	else if(m_pDCP06DomModel->dom_active_plane == YZ_PLANE)
	{
		// normal of yz-plane

		nx[0] = pPlane[0].nx;
		nx[1] = pPlane[0].ny;
		nx[2] = pPlane[0].nz;
		nx[3] = 1.0;


		if(m_pDCP06DomModel->dom_active_line == Y_LINE)
		{

			// nz
			vec_product(nx, n_line, &temp1);
			unit_vector(temp1, &nz);

			// ny
			vec_product(nz, nx, &temp1);
			unit_vector(temp1, &ny);

		}
		else if(m_pDCP06DomModel->dom_active_line == Z_LINE)
		{
			// ny
			vec_product(n_line, nx, &temp1);
			unit_vector(temp1, &ny);

			// nz
			vec_product(nx, ny, &temp1);
			unit_vector(temp1, &nz);
		}
	}
		/*
		common.set_ocsd_matrix(
				nx[0], nx[1],nx[2], 0.0, 
				ny[0], ny[1],ny[2], 0.0, 
				nz[0], nz[1],nz[2], 0.0, 
				rpoint_buff.x,rpoint_buff.y,rpoint_buff.z, 1.0);
		*/
 		/*  Removed 1.4.1998 */
		m_pDCP06DomModel->matrix[0][0] = nx[0];
		m_pDCP06DomModel->matrix[1][0] = nx[1];
		m_pDCP06DomModel->matrix[2][0] = nx[2];
		m_pDCP06DomModel->matrix[3][0] = 0.0;


		m_pDCP06DomModel->matrix[0][1] = ny[0];
		m_pDCP06DomModel->matrix[1][1] = ny[1];
		m_pDCP06DomModel->matrix[2][1] = ny[2];
		m_pDCP06DomModel->matrix[3][1] = 0.0;

		m_pDCP06DomModel->matrix[0][2] = nz[0];
		m_pDCP06DomModel->matrix[1][2] = nz[1];
		m_pDCP06DomModel->matrix[2][2] = nz[2];
		m_pDCP06DomModel->matrix[3][2] = 0.0;

		m_pDCP06DomModel->matrix[0][3] = m_pDCP06DomModel->dom_ref_point_buff.x;
		m_pDCP06DomModel->matrix[1][3] = m_pDCP06DomModel->dom_ref_point_buff.y;
		m_pDCP06DomModel->matrix[2][3] = m_pDCP06DomModel->dom_ref_point_buff.z;
		m_pDCP06DomModel->matrix[3][3] = 1.0;
		

		//get_ocsd_table(&ocsd_table);
		//get_ocsd_table(&ocsd_inv_table);

		if(m_pDCP06DomModel->dom_active_plane == XY_PLANE)
		{
			deg = m_pDCP06DomModel->dom_rot_plane_buff.x;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
			
				rotate_about_x_axis(m_pDCP06DomModel->matrix /*ocsd_table*/, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);//ocsd_table[0][0]);
				
			}

			deg = m_pDCP06DomModel->dom_rot_plane_buff.y;
			if(deg != 0.0)
			{
				
				rad = degtorad(deg);
				rotate_about_y_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
				
			}

			deg = m_pDCP06DomModel->dom_rot_line_buff.z;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				 rotate_about_z_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);

			}
		}

		else if(m_pDCP06DomModel->dom_active_plane == ZX_PLANE)
		{

			deg = m_pDCP06DomModel->dom_rot_plane_buff.x;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_x_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);

			}

			deg = m_pDCP06DomModel->dom_rot_plane_buff.z;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_z_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
			}

			deg = m_pDCP06DomModel->dom_rot_line_buff.y;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_y_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
			}
		}
		else if(m_pDCP06DomModel->dom_active_plane == YZ_PLANE)
		{

			deg = m_pDCP06DomModel->dom_rot_plane_buff.y;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_y_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
			}

			deg = m_pDCP06DomModel->dom_rot_plane_buff.z;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_z_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
			}

			deg = m_pDCP06DomModel->dom_rot_line_buff.x;
			if(deg != 0.0)
			{
				rad = degtorad(deg);
				rotate_about_x_axis(m_pDCP06DomModel->matrix, rad, &tempmat2);
				matcopy_n(&tempmat2[0][0], 4,4, &m_pDCP06DomModel->matrix[0][0]);
			}
		}

		ovalues[0] = m_pDCP06DomModel->dom_ovalues_buff.x + m_pDCP06DomModel->dom_ovalues_tool_buff.x;
		ovalues[1] = m_pDCP06DomModel->dom_ovalues_buff.y + m_pDCP06DomModel->dom_ovalues_tool_buff.y;
		ovalues[2] = m_pDCP06DomModel->dom_ovalues_buff.z + m_pDCP06DomModel->dom_ovalues_tool_buff.z;
		ovalues[3] = 1.0;
		
		//PrintLn("ofs values");
		//PrintLn("%8.2f%8.2f %8.2f",ovalues[0],ovalues[1],ovalues[2]);

		/*
		ovalues[0] = ovalues_buff.x;
		ovalues[1] = ovalues_buff.y;
		ovalues[2] = ovalues_buff.z;
		ovalues[3] =1.0;
		*/

		matcopy_n(ovalues, 1,4, point1);

		matcopy_n(ovalues, 1,4, point2);
		point2[0] = point2[0] + 1000;

		matcopy_n(ovalues, 1,4, point3);
		point3[1] = point3[1] + 1000;

		htransm(point1, point2, point3, 'y', &tempmat2);

		if(matinv4x4(tempmat2, &tempmat3) == -1)
		{
			msg.LoadTxt(AT_DCP05, M_DCP_CANNOT_CALC_TOK);
			msgbox.ShowMessageOk(msg);
			//m_pDCP06Model->ocsd_defined=false;
			return false;
		}

		matcopy_n(&m_pDCP06DomModel->matrix[0][0],4,4,&tempmat2[0][0]);
		matmul4x4(tempmat2, tempmat3, &m_pDCP06DomModel->matrix); // HOX pit�� asettaa my�s ocsd_matrix
		
		/*
		set_ocsd_matrix(ocsd_table[0][0], ocsd_table[1][0],ocsd_table[2][0],ocsd_table[3][0],
						ocsd_table[0][1], ocsd_table[1][1],ocsd_table[2][1],ocsd_table[3][1],
						ocsd_table[0][2], ocsd_table[1][2],ocsd_table[2][2],ocsd_table[3][2],
						ocsd_table[0][3], ocsd_table[1][3],ocsd_table[2][3],ocsd_table[3][3]);
				
		m_pDCP06Model->ocsd_defined=true;
		*/

		//show_matrix4x4(ocsd_matrix);
		
		matinv4x4(m_pDCP06DomModel->matrix, &m_pDCP06DomModel->inv_matrix);	    // HOX pit�� asettaa my�s ocsd_inv_matrix
		
		/*
		set_ocsd_inv_matrix( ocsd_inv_table[0][0], ocsd_inv_table[1][0],ocsd_inv_table[2][0],ocsd_inv_table[3][0],
									ocsd_inv_table[0][1], ocsd_inv_table[1][1],ocsd_inv_table[2][1],ocsd_inv_table[3][1],
									ocsd_inv_table[0][2], ocsd_inv_table[1][2],ocsd_inv_table[2][2],ocsd_inv_table[3][2],
								   ocsd_inv_table[0][3], ocsd_inv_table[1][3],ocsd_inv_table[2][3],ocsd_inv_table[3][3]);
		*/
		
		if(show_message) {
			msg.LoadTxt(AT_DCP05, M_DCP_CALCULATION_OK_TOK);
			msgbox.ShowMessageOk(msg);
		}
		
		return true;

}