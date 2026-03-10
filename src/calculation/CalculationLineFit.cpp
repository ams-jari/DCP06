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
 
#include <dcp06/calculation/CalculationLineFit.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/calculation/Calculation321.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include "calc.h"
#include <math.h>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>
#include <GUI_DeskTop.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>

#include <TBL_Measurement.hpp>
#include <TBL_Util.hpp>
#include <TPI_MeasConfig.hpp>

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

DCP::CalcLineFit::CalcLineFit(Model* pModel) : m_pModel(pModel) 
{
		common = new Common(pModel);
}
// ****************************************************************************************
DCP::CalcLineFit::~CalcLineFit()
{
	if(common != 0)
	{
		delete common;
		common = 0;
	}
}

short DCP::CalcLineFit::CalcLineFitDom(Alignment321Model* align321Model, S_LINE_BUFF* line_buff, int refLine)
{
MsgBox msgbox;

	align321Model->calculated = 0;

	if(!line_buff[0].calc)
	{
		GUI::DesktopC::Instance()->MessageShow(L"Line in not calculated!");
		//msgbox.ShowMessageOk(L"Line in not calculated!");		
		return 0;
	}

	// delete old values
	delete_align321_values(align321Model);

	/*align321Model->ocsd_defined = false;
	align321Model->align321_active_line = X_LINE;
	align321Model->align321_hz_plane = false;
	
	memset(&align321Model->align321_plane_buff[0],0,sizeof(S_PLANE_BUFF));
	memset(&align321Model->align321_line_buff[0],0,sizeof(S_LINE_BUFF));	
	memset(&align321Model->align321_ref_point_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_ovalues_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_ovalues_tool_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_rot_line_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_rot_plane_buff,0,sizeof(S_POINT_BUFF));	*/

	// calculate new

	align321Model->align321_active_plane = XY_PLANE;
	align321Model->align321_active_line = X_LINE;
	align321Model->align321_hz_plane = true;

	if(!set_hz_plane1(align321Model, ACTUAL))
	{
		GUI::DesktopC::Instance()->MessageShow(L"Linefit/Dom error setting Hz..!");
		//msgbox.ShowMessageOk(L"Linefit/Dom error setting Hz..!");
		return 0;
	}
	// copy line into dom...
	memcpy(&align321Model->align321_line_buff[0],&line_buff[0],  sizeof(S_LINE_BUFF));
	memcpy(&align321Model->align321_ref_point_buff, &line_buff[0].points[0], sizeof(S_POINT_BUFF));

	// jos vertkaalisuora
	if(refLine == REF_LINE_VERTICAL)
	{
		align321Model->align321_line_buff[0].points[0].x = 0.0;
		align321Model->align321_line_buff[0].points[0].y = 0.0;
		align321Model->align321_line_buff[0].points[0].z = 0.0;
		align321Model->align321_line_buff[0].points[0].sta = 1;

		align321Model->align321_line_buff[0].points[1].x = line_buff[0].points[0].x;
		align321Model->align321_line_buff[0].points[1].y = line_buff[0].points[0].y;
		align321Model->align321_line_buff[0].points[1].z = line_buff[0].points[0].z;
		align321Model->align321_line_buff[0].points[1].sta = 1;
		
		// laske suora uudestaan
		CalcLine calcLine;
		calcLine.calc(&align321Model->align321_line_buff[0], ACTUAL);
	}

	CalcAlignment321 calc_dom(align321Model);
	if(!calc_dom.calc(false))
	{
		GUI::DesktopC::Instance()->MessageShow(L"Calculation LineFit/Dom error!");
		//msgbox.ShowMessageOk(L"Calculation LineFit/Dom error!");
	}
	else
	{
		align321Model->calculated = true;
	}


	return 1;
}

void DCP::CalcLineFit::delete_align321_values(Alignment321Model* align321Model)
{
	// delete old values
	align321Model->ocsd_defined = false;
	align321Model->align321_active_plane = XY_PLANE;
	align321Model->align321_active_line = X_LINE;
	align321Model->align321_hz_plane = true;
	
	memset(&align321Model->align321_plane_buff[0],0,sizeof(S_PLANE_BUFF));
	memset(&align321Model->align321_line_buff[0],0,sizeof(S_LINE_BUFF));	
	memset(&align321Model->align321_ref_point_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_ovalues_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_ovalues_tool_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_rot_line_buff,0,sizeof(S_POINT_BUFF));	
	memset(&align321Model->align321_rot_plane_buff,0,sizeof(S_POINT_BUFF));	
}

// ****************************************************************************************
short DCP::CalcLineFit::CalcAllPoints(S_LINE_BUFF* line_buff,
								 S_POINT_BUFF* points,	
								 S_LINE_FITTING_RESULTS* results,
 								 double manualHeight,
     							double shiftValue,
								double rotateAngle,
								int selectedHeight,
								int selectedShift,
								int selectedRotate,Alignment321Model* align321Model,
								S_LINE_BUFF* line_ocs,
								S_POINT_BUFF* points_in_line,
								int selectedRefLine)
{
	// delete results
	memset(&results[0], 0, sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);
	memset(&line_ocs[0],0, sizeof(S_LINE_BUFF));
	memset(&points_in_line[0], 0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);

	
	for(int i = 0; i < MAX_LINEFIT_POINTS; i++)
	{
		if(points[i].sta == 1)
		{
			CalcPoint(i, line_buff,points,results,manualHeight,shiftValue,rotateAngle,selectedHeight,selectedShift,selectedRotate,align321Model,line_ocs, points_in_line,selectedRefLine);
		}
	}
	return 1;
}

// ****************************************************************************************
short DCP::CalcLineFit::CalcPoint(short index, 
								  S_LINE_BUFF* line_buff,
								 S_POINT_BUFF* points,	
								 S_LINE_FITTING_RESULTS* results,
 								 double manualHeight,
     							double shiftValue,
								double rotateAngle,
								int selectedHeight,
								int selectedShift,
								int selectedRotate,
								Alignment321Model* align321Model,
								S_LINE_BUFF* line_ocs,
								S_POINT_BUFF* points_in_line,
								int selectedRefLine)
{

	if( points[index].sta == 1 && align321Model->calculated) 
	{

		double p_ocs[4];

		// convert measured point to ocs
		convert_point_to_ocs(points, index, align321Model, &p_ocs);
		points[index].xdes = p_ocs[0];
		points[index].ydes = p_ocs[1];
		points[index].zdes = p_ocs[2];



		
		// copy line
		S_LINE_BUFF line[1];
		S_LINE_BUFF line_ocs_hz[1];

		memcpy(&line[0],  &line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&line_ocs[0],  &line_buff[0], sizeof(S_LINE_BUFF));
		
		// jos suoraa ei ole laskettu
		if(line[0].calc != 1)
			return 0;
	
		//// line last point
		int lastPoint = common->get_max_defined_point_line(line);

		// konvertoi suoran scs pisteet ocs:ksi	
		double p_temp[4];
		double p_in1[4];
		for(int i = 0; i < MAX_POINTS_IN_LINE; i++)
		{
			p_in1[0] = line_ocs[0].points[i].x;
			p_in1[1] = line_ocs[0].points[i].y;
			p_in1[2] = line_ocs[0].points[i].z;

			convert_point_to_ocs(p_in1, align321Model, &p_temp);
			
			line_ocs[0].points[i].x = p_temp[0];
			line_ocs[0].points[i].y = p_temp[1];
			line_ocs[0].points[i].z = p_temp[2];

			// hz
			//p_in1[0] = line_ocs[0].points[i].x;
			//p_in1[1] = line_ocs[0].points[i].y;
			//p_in1[2] = 0.0;//line_ocs[0].points[i].z;

			//convert_point_to_ocs(p_in1, align321Model, &p_temp);
			//
			//line_ocs_hz[0].points[i].x = p_temp[0];
			//line_ocs_hz[0].points[i].y = p_temp[1];
			//line_ocs_hz[0].points[i].z = 0.0;//p_temp[2];

		}

		// move this later
		if(selectedRefLine == REF_LINE_HORIZONTAL)
		{
			// siirret��nk� linjaa oikealle y+
			if(selectedShift == SHIFT_RIGHT)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].y -= shiftValue;
				}
			}
			// siirret��nk� linjaa vasemmalle y-
			else if(selectedShift == SHIFT_LEFT)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].y += shiftValue;
				}
			}
			// siirret��nk� linjaa yl�s z+
			else if(selectedShift == SHIFT_UP)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].z += shiftValue;
				}
			}
			// siirret��nk� linjaa alas z-
			else if(selectedShift == SHIFT_DOWN)
			{
				for(int i=0; i < lastPoint; i++)
					line_ocs[0].points[i].z -= shiftValue;
			}
		}
		else
		{
			// siirret��nk� linjaa oikealle y+
			if(selectedShift == SHIFT_RIGHT)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].y -= shiftValue;
				}
			}
			// siirret��nk� linjaa vasemmalle y-
			else if(selectedShift == SHIFT_LEFT)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].y += shiftValue;
				}
			}
			// siirret��nk� linjaa yl�s z+
			else if(selectedShift == SHIFT_FORWARD)
			{
				for(int i=0; i < lastPoint; i++)
				{
					line_ocs[0].points[i].x += shiftValue;
				}
			}
			// siirret��nk� linjaa alas z-
			else if(selectedShift == SHIFT_BACKWARD)
			{
				for(int i=0; i < lastPoint; i++)
					line_ocs[0].points[i].x -= shiftValue;
			}
		}
		// selected rotate
		
		double rot_angle = degtorad(-rotateAngle);

		if(selectedRefLine == REF_LINE_HORIZONTAL)
		{
			// muunna suoran pisteen around to z -axis
			if(selectedRotate == ROTATE_HORIZONTAL)
			{
				double x_orig = line_ocs[0].points[0].x;
				double y_orig = line_ocs[0].points[0].y;
				double z_orig = line_ocs[0].points[0].z;
				
				for(int i=0; i < lastPoint; i++)
				{
					double x_temp = line_ocs[0].points[i].x;
					double y_temp = line_ocs[0].points[i].y;
					double z_temp = line_ocs[0].points[i].z;
			
					double x_new = x_orig + (x_temp-x_orig) *cos(rot_angle) - (y_temp-y_orig) * sin(rot_angle);
					double y_new = y_orig +(x_temp-x_orig) *sin(rot_angle) + (y_temp-y_orig) * cos(rot_angle);
					double z_new = z_temp;// + z_orig;

					line_ocs[0].points[i].x = x_new;
					line_ocs[0].points[i].y = y_new;
					line_ocs[0].points[i].z = z_new;
				}
			}
			// muunna pisteet y akselin ymp�ri
			else if(selectedRotate == ROTATE_VERTICAL)
			{
					double x_orig = line_ocs[0].points[0].x;
					double y_orig = line_ocs[0].points[0].y;
					double z_orig = line_ocs[0].points[0].z;

					for(int i=0; i < lastPoint; i++)
					{
					
						double x_temp = line_ocs[0].points[i].x;
						double y_temp = line_ocs[0].points[i].y;
						double z_temp = line_ocs[0].points[i].z;
						
						//testi 4.10.17
						//double x_new = z_orig + (z_temp-z_orig) *sin(rot_angle) + (x_temp-x_orig) * cos(rot_angle);
						double x_new = x_orig + (z_temp-z_orig) *sin(rot_angle) + (x_temp-x_orig) * cos(rot_angle);
						double y_new = y_temp;// + y_orig;
						double z_new = z_orig + (z_temp-z_orig) *cos(rot_angle) - (x_temp-x_orig) * sin(rot_angle);

						line_ocs[0].points[i].x = x_new;
						line_ocs[0].points[i].y = y_new;
						line_ocs[0].points[i].z = z_new;
					}
			}
		}
		// vertical line
		else
		{
			rot_angle = -rot_angle;
			// muunna suoran pisteen around to x -axis
			if(selectedRotate == ROTATE_VERTICAL_LEFT_RIGHT)
			{
				double x_orig = line_ocs[0].points[0].x;
				double y_orig = line_ocs[0].points[0].y;
				double z_orig = line_ocs[0].points[0].z;
				
				for(int i=0; i < lastPoint; i++)
				{
					double x_temp = line_ocs[0].points[i].x;
					double y_temp = line_ocs[0].points[i].y;
					double z_temp = line_ocs[0].points[i].z;
			
					double x_new = x_temp;// + x_orig;
					double y_new = y_orig + (y_temp-y_orig) *cos(rot_angle) - (z_temp-z_orig) * sin(rot_angle);
					double z_new = z_orig +(y_temp-y_orig) *sin(rot_angle) + (z_temp - z_orig) * cos(rot_angle);

					line_ocs[0].points[i].x = x_new;
					line_ocs[0].points[i].y = y_new;
					line_ocs[0].points[i].z = z_new;
				}
			}
			// muunna pisteet y akselin ymp�ri
			else if(selectedRotate == ROTATE_VERTICAL_DEPTH)
			{
					double x_orig = line_ocs[0].points[0].x;
					double y_orig = line_ocs[0].points[0].y;
					double z_orig = line_ocs[0].points[0].z;

					for(int i=0; i < lastPoint; i++)
					{
					
						double x_temp = line_ocs[0].points[i].x;
						double y_temp = line_ocs[0].points[i].y;
						double z_temp = line_ocs[0].points[i].z;
				
						double x_new = x_orig + (z_temp-z_orig) *sin(rot_angle) + (x_temp-x_orig) * cos(rot_angle);
						double y_new = y_temp;// + y_orig;
						double z_new = z_orig + (z_temp-z_orig) *cos(rot_angle) - (x_temp-x_orig) * sin(rot_angle);

						line_ocs[0].points[i].x = x_new;
						line_ocs[0].points[i].y = y_new;
						line_ocs[0].points[i].z = z_new;
					}
			}
		}

		

		// konvertoidaan suoran pisteen hz tasolle tai YZ tasolle
		memcpy(&line_ocs_hz[0],  &line_ocs[0], sizeof(S_LINE_BUFF));

		if(selectedRefLine == REF_LINE_HORIZONTAL)
		{
			for(int i=0; i < lastPoint; i++)
			{
				line_ocs_hz[0].points[i].z = 0.0;
			}
		}
		else
		{
			for(int i=0; i < lastPoint; i++)
			{
				line_ocs_hz[0].points[i].x = 0.0;
			}
		}
	

		// ja laske line
		CalcLine calcLine;
		if(!calcLine.calc(&line_ocs[0], ACTUAL))
			return 0;
		
		calcLine.calc(&line_ocs_hz[0], ACTUAL);

		double z = p_ocs[2];//points[index].z;
		double x = p_ocs[0];
		double zDiff = 0.0;

		
		// distance from line to point ALKUPER�INEN LINE
		struct ams_vector m;
		struct ams_vector dest_point;
		struct ams_vector dest_point_hz;
		double dist_hz = 0.0;

		struct line wline;
		double dist;

		m.x = p_ocs[0];//points[index].x;
		m.y = p_ocs[1];//points[index].y;
		m.z = p_ocs[2];//points[index].z;

		wline.px = line_ocs[0].px;
		wline.py = line_ocs[0].py;
		wline.pz = line_ocs[0].pz;
		wline.ux = line_ocs[0].ux;
		wline.uy = line_ocs[0].uy;
		wline.uz = line_ocs[0].uz;

		// mitatun pisteen et�isyys suorasta, palauttaa lasketun pisteen suoralle ja et�isyyden
		dist = calc_point_dist_from_line2(&m, &wline, &dest_point);
		points_in_line[index].x = dest_point.x;
		points_in_line[index].y = dest_point.y;
		points_in_line[index].z = dest_point.z;

		// laske pisteen et�isyys suorasta hz-tasossa
		if(selectedRefLine == REF_LINE_HORIZONTAL)
		{
			m.x = p_ocs[0];//points[index].x;
			m.y = p_ocs[1];//points[index].y;
			m.z = 0.0;//p_ocs[2];//points[index].z;
		}
		else
		{
			m.x = 0.0;//p_ocs[0];//points[index].x;
			m.y = p_ocs[1];//points[index].y;
			m.z = p_ocs[2];//points[index].z;
		}

		wline.px = line_ocs_hz[0].px;
		wline.py = line_ocs_hz[0].py;
		wline.pz = line_ocs_hz[0].pz;
		wline.ux = line_ocs_hz[0].ux;
		wline.uy = line_ocs_hz[0].uy;
		wline.uz = line_ocs_hz[0].uz;
		dist_hz = calc_point_dist_from_line2(&m, &wline, &dest_point_hz);

		// test convert point to scs
		

		// konvertoi dest_point to ocs to check y coordinate
		//double p_in[4];
		//double p_ocs_line[4];

		// convert calculated point to ocs
		/*p_in[0] = dest_point.x;
		p_in[1] = dest_point.y;
		p_in[2] = dest_point.z;*/

		//convert_point_to_ocs(p_in, align321Model, &p_ocs_line);

		// lasketaan shiftattu linepisteeen et�isyys mitatuun pisteeseen
		//ams_vector dest_point_ocs;
		//ams_vector mea_point_ocs;
		
		//dest_point_ocs.x = p_ocs_line[0];
		//dest_point_ocs.y = p_ocs_line[1];
		//dest_point_ocs.z = p_ocs_line[2];
		
	/*	mea_point_ocs.x = p_ocs[0];
		mea_point_ocs.y = p_ocs[1];
		mea_point_ocs.z = p_ocs[2];
		
		dist = calc_point_dist_from_point(&m, &dest_point);*/

		results[index].points_distance = dist;

		double y_offset_from_ref = dest_point.y - p_ocs[1];
		
		if(selectedRefLine == REF_LINE_HORIZONTAL)
		{
			m.x = p_ocs[0];
			m.y = p_ocs[1];
			m.z = 0.00;//dest_point.z;
		}
		else
		{
			m.x = 0.0;//p_ocs[0];
			m.y = p_ocs[1];
			m.z = p_ocs[2];//0.00;//dest_point.z;
		}
		struct ams_vector dest_point1;
		dest_point1.x = dest_point_hz.x;
		dest_point1.y = dest_point_hz.y;
		dest_point1.z = dest_point_hz.z;
		double offset= calc_point_dist_from_point(&m, &dest_point1);
		
		//results[index].line_offset = dest_point.y - p_ocs[1];//p_ocs[1] - dest_point.y;//p_ocs_line[1];
		results[index].line_offset = y_offset_from_ref < 0.0 ? -offset : offset;

		// ja et�isyys 1. pisteest�
		double dist1;
		m.x = line_ocs[0].points[0].x;
		m.y = line_ocs[0].points[0].y;
		m.z = line_ocs[0].points[0].z;

		dist1 = calc_point_dist_from_point(&m, &dest_point);
		results[index].distance_along_line = dist1;

		if(selectedHeight == FIRST_POINT)
		{
			if(selectedRefLine == REF_LINE_HORIZONTAL)
			{
				zDiff = z - line_ocs[0].points[0].z;
			}
			else
			{
				zDiff = x - line_ocs[0].points[0].x;
			}
		}
		else if(selectedHeight == LAST_POINT)
		{
			if(selectedRefLine == REF_LINE_HORIZONTAL)
			{
				zDiff = z - line_ocs[0].points[lastPoint-1].z;
			}
			else
			{
				zDiff = x - line_ocs[0].points[lastPoint-1].x;
			}
		}

		else if(selectedHeight == INTERPOLATION)
		{
			if(selectedRefLine == REF_LINE_HORIZONTAL)
			{
				zDiff = z - dest_point.z;
			}
			else
			{
				zDiff = x - dest_point.x;
			}
		}
		else if(selectedHeight == MANUALLY_ENTERED)
		{
			if(selectedRefLine == REF_LINE_HORIZONTAL)
			{
				zDiff = z - manualHeight;
			}
			else
			{
				zDiff = x - manualHeight;
			}
		}

		results[index].height_diff = zDiff;
			results[index].sta = 1;

		

		
	}

	return 1;
}

short DCP::CalcLineFit::convert_point_to_ocs(double p_in[4], Alignment321Model* align321Model, double (*p_out)[4])
{
	double x[4];
	double xu[4];

	x[0] = p_in[0];
	x[1] = p_in[1];
	x[2] = p_in[2];
	x[3] = 1;

		ptrans(x, &align321Model->matrix[0], &xu);

		(*p_out)[0] = xu[0];
		(*p_out)[1] = xu[1];
		(*p_out)[2] = xu[2];

		return 1;
}

short DCP::CalcLineFit::convert_point_to_scs(double p_in[4], Alignment321Model* align321Model, double (*p_out)[4])
{
	double x[4];
	double xu[4];

	x[0] = p_in[0];
	x[1] = p_in[1];
	x[2] = p_in[2];
	x[3] = 1;

		ptrans(x, &align321Model->inv_matrix[0], &xu);

		(*p_out)[0] = xu[0];
		(*p_out)[1] = xu[1];
		(*p_out)[2] = xu[2];

		return 1;
}

//double v[4],double a[4][4], double (*w)[4])
short DCP::CalcLineFit::convert_point_to_ocs(S_POINT_BUFF* points, short index, Alignment321Model* align321Model, double (*p_out)[4])
{

	double x[4];
	double xu[4];

		x[0] = points[index].x;
		x[1] = points[index].y;
		x[2] = points[index].z;
		x[3] = 1;

		ptrans(x, &align321Model->matrix[0], &xu);

		points[index].xdes = xu[0];
		points[index].ydes = xu[1];
		points[index].zdes = xu[2];

		(*p_out)[0] = xu[0];
		(*p_out)[1] = xu[1];
		(*p_out)[2] = xu[2];

		return 1;
		
}
// ================================================================================================
short DCP::CalcLineFit::set_hz_plane1(Alignment321Model* align321Model,short actualdesign)
{
short dist_count,ret;

ret = false;
	
	//Common common(m_pModel);
	MsgBox msgbox;

	dist_count = common->points_count_in_plane(&align321Model->align321_hz_plane_buff[0]);
	
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	//bool bComp = oMeasConfig.GetCompensatorMode();
	bool bComp = oMeasConfig.IsCompensatorEnabled();

	//if(get_compensator_onoff() == FALSE)
	if(!bComp)
	{
		//oMeasConfig.SetCompensatorMode(true);
		oMeasConfig.SetCompensatorEnabled(true);
		oMeasConfig.Activate();
	}

	bool ret1 =TBL::CheckCompensator();
	if(!ret1)
	{
		//msgbox.ShowMessageOk(L"Not levelled!");	
		return false;
	}

	if(set_horizontal_plane(align321Model/*&GetDataModel()->hz_plane_buff[0], display, plane_type*/))
	{
		if(calc_plane(&align321Model->align321_hz_plane_buff[0],actualdesign))// == true)
		{
			align321Model->align321_hz_plane_buff[0].calc = 1;
			align321Model->align321_hz_plane_buff[0].sta = PLANE_DEFINED; 

			dist_count = common->points_count_in_plane(&align321Model->align321_hz_plane_buff[0]);

			snprintf(align321Model->align321_hz_plane_buff[0].points[0].point_id, sizeof(align321Model->align321_hz_plane_buff[0].points[0].point_id), DCP_POINT_ID_FMT, "rp-p1");
			snprintf(align321Model->align321_hz_plane_buff[0].points[1].point_id, sizeof(align321Model->align321_hz_plane_buff[0].points[1].point_id), DCP_POINT_ID_FMT, "rp-p2");
			snprintf(align321Model->align321_hz_plane_buff[0].points[2].point_id, sizeof(align321Model->align321_hz_plane_buff[0].points[2].point_id), DCP_POINT_ID_FMT, "rp-p3");
			align321Model->align321_hz_plane = true;
			ret = true;
		}
		else
		{
			//if(display == A321_DLG)
				align321Model->align321_hz_plane = false;
			
			//else if(display == DLG_DOMUSER)
			//	GetModel()->hz_plane = false;
		}
	 }
	 else
	 {
		// msgbox(TXT_NIL_TOKEN, M_ERROR_IN_SET_HZ_TOK,MB_OK);
	 }

	return ret;
}

short DCP::CalcLineFit::calc_plane(S_PLANE_BUFF *plane, short actdes)
{
	CalcPlane calcplane;

	return calcplane.calc(plane,actdes);

}

// ================================================================================================
short DCP::CalcLineFit::set_horizontal_plane(Alignment321Model* align321Model/*plane_buff_ *planes, short DISPLAY, short PLANE_TYPE*/)
{
short points;
short ret;
	
	ret = false;
	points  = 0;	

	//Common common(m_pModel);//(/*m_pModel*/);

	points = common->points_count_in_plane(&align321Model->align321_hz_plane_buff[0]);

		if(points > 0)
		{
			memset(&align321Model->align321_hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
			//delete_all_plane(MAX_POINTS_IN_PLANE,&planes[0]);	
			points = 0;
		}
		
		if(points == 0)
		{
			if(align321Model->align321_active_plane == XY_PLANE)
			{
				align321Model->align321_hz_plane_buff[0].points[0].x = 0.0;
				align321Model->align321_hz_plane_buff[0].points[0].y = 0.0;
				align321Model->align321_hz_plane_buff[0].points[0].z = 0.0;
				align321Model->align321_hz_plane_buff[0].points[0].sta = POINT_DESIGN;


				align321Model->align321_hz_plane_buff[0].points[1].x = 1.0;
				align321Model->align321_hz_plane_buff[0].points[1].y = 0.0;
				align321Model->align321_hz_plane_buff[0].points[1].z = 0.0;
				align321Model->align321_hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				align321Model->align321_hz_plane_buff[0].points[2].x = 0.0;
				align321Model->align321_hz_plane_buff[0].points[2].y = 1.0;
				align321Model->align321_hz_plane_buff[0].points[2].z = 0.0;
				align321Model->align321_hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
			//else if(GetDataModel()->active_plane == ZX_PLANE)
			//{
			//	//PrintLn("Setting %s", "ZX");

			//	GetDataModel()->hz_plane_buff[0].points[0].x = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].y = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

			//	GetDataModel()->hz_plane_buff[0].points[1].x = 1.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].y = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

			//	GetDataModel()->hz_plane_buff[0].points[2].x = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].y = 1.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

			//	ret = true;
			//}
			//else if(GetDataModel()->active_plane == YZ_PLANE)
			//{
			//	//PrintLn("Setting %s", "YZ");

			//	GetDataModel()->hz_plane_buff[0].points[0].x = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].y = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

			//	GetDataModel()->hz_plane_buff[0].points[1].x = 1.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].y = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

			//	GetDataModel()->hz_plane_buff[0].points[2].x = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].y = 1.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].z = 0.0;
			//	GetDataModel()->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

			//	ret = true;
			//}
		}
		
		else
		{
			ret = false;
		}
		


	return ret;
}


