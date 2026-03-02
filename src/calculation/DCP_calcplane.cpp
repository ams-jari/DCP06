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
#include <dcp06/calculation/DCP_CalcPlane.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include "calc.h"
#include <math.h>


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

DCP::DCP05CalcPlaneC::DCP05CalcPlaneC()
{

}
DCP::DCP05CalcPlaneC::~DCP05CalcPlaneC()
{

}
// *********************************************************************
//
// *********************************************************************
short DCP::DCP05CalcPlaneC::calc(S_PLANE_BUFF *plane, short actdes)
{
short points_defined,i, ret=true;
struct ams_vector a,b,c,a_des,b_des,c_des;
struct plane d,d_des;
short count,ff,des_ok;
double p_mat[MAX_POINTS_IN_PLANE*3];
double para,parb,parc;
double /*e[3],*/ n[3];
double dist;
double x_tot=0.0, y_tot=0.0, z_tot=0.0;

	DCP05MsgBoxC msgbox;

	plane[0].calc = 0;
	plane[0].sta = PLANE_NOT_DEFINED;
	plane[0].des_calc = 0;
	plane[0].des_sta = PLANE_NOT_DEFINED;

	points_defined = defined_points_count_in_plane(plane, NULL);

	if(points_defined < 3)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP05,M_DCP_IN_MIN_3_POINTS_TOK);
		msgbox.ShowMessageOk(msg);
		ret = false;
	}

	else if(points_defined == 3)
	{
		count = 0;

		/* select all 3 defined points from table */

		des_ok = true;

		for(i=0; i < MAX_POINTS_IN_PLANE; i++)
		{
			if(plane[0].points[i].sta == POINT_DESIGN || plane[0].points[i].sta == POINT_MEASURED)
			{
				count++;

				if(count == 1)
				{
					a.x = plane[0].points[i].x;
					a.y = plane[0].points[i].y;
					a.z = plane[0].points[i].z;

					if(actdes == BOTH)
					{
						if(plane->points[i].dsta == POINT_DESIGN || plane->points[i].dsta == POINT_MEASURED)
						{
							a_des.x = plane[0].points[i].xdes;
							a_des.y = plane[0].points[i].ydes;
							a_des.z = plane[0].points[i].zdes;
						}
						else
							des_ok = false;
					}
				}

				else if(count == 2)
				{
					b.x = plane[0].points[i].x;
					b.y = plane[0].points[i].y;
					b.z = plane[0].points[i].z;
					if(actdes == BOTH && des_ok)// == true)
					{
						if(plane->points[i].dsta == POINT_DESIGN || plane->points[i].dsta == POINT_MEASURED)
						{
							b_des.x = plane[0].points[i].xdes;
							b_des.y = plane[0].points[i].ydes;
							b_des.z = plane[0].points[i].zdes;
						}
						else
							des_ok = false;
					}
				}

				else if(count == 3)
				{
					c.x = plane[0].points[i].x;
					c.y = plane[0].points[i].y;
					c.z = plane[0].points[i].z;

					if(actdes == BOTH && des_ok)// == true)
					{
						if(plane->points[i].dsta == POINT_DESIGN || plane->points[i].dsta == POINT_MEASURED)
						{
							c_des.x = plane[0].points[i].xdes;
							c_des.y = plane[0].points[i].ydes;
							c_des.z = plane[0].points[i].zdes;
						}
						else
							des_ok = false;
					}

					break;
				}
			}
		}

		equation_of_plane(&a, &b, &c, &d);

		if(d.nx == 0.0 && d.ny == 0.0 && d.nz == 0.0)
		{
			//sprintf(msgstr,"%-s",CANNOT_CALCULATE_TEXT);
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
		}

		else
		{
			plane[0].px = d.px; plane[0].py = d.py; plane[0].pz = d.pz;
			plane[0].nx = d.nx; plane[0].ny = d.ny; plane[0].nz = d.nz;

			para = d.nx; parb = d.ny; parc = d.nz;
			plane->calc = 1;
			plane->sta = PLANE_DEFINED;
		}

		if(actdes == BOTH && des_ok)// == true)
		{
			equation_of_plane(&a_des, &b_des, &c_des, &d_des);

			if(d_des.nx == 0.0 && d_des.ny == 0.0 && d_des.nz == 0.0)
			{
			}
			else
			{
				plane[0].des_px = d_des.px; plane[0].des_py = d_des.py; plane[0].des_pz = d_des.pz;
				plane[0].des_nx = d_des.nx; plane[0].des_ny = d_des.ny; plane[0].des_nz = d_des.nz;
				plane[0].des_calc = 1;
				plane[0].des_sta = PLANE_DEFINED;
			}
		}

	}
	else
	{
//		show_txtmsg("Calc plane > 3 pnts");
//		sprintf(msgTxt,"%-s, %-s",CALCULATING_TEXT,PRESS_ESC_TO_ABORT);
//		show_info(msgTxt);
//		p_mat = (double *) calloc(points_defined,sizeof(double) * 3);

		des_ok = true;

		if(p_mat != NULL)
		{
			count = 0;

			// ***************************************************
			// select all defined and accepted points from table 
			// ***************************************************

			for(i=0; i < MAX_POINTS_IN_PLANE; i++)
			{
				if(plane[0].points[i].sta == 1 ||plane[0].points[i].sta == 2)
				{
					if(plane[0].points[i].dsta ==0)
						des_ok = false;

					if(count == 0)
					{
						a.x = plane[0].points[i].x;
						a.y = plane[0].points[i].y;
						a.z = plane[0].points[i].z;
					}

					else if(count == 1)
					{
						b.x = plane[0].points[i].x;
						b.y = plane[0].points[i].y;
						b.z = plane[0].points[i].z;
					}

					else if(count == 2)
					{
						c.x = plane[0].points[i].x;
						c.y = plane[0].points[i].y;
						c.z = plane[0].points[i].z;
					}

					*(p_mat + count*3 +0) = plane[0].points[i].x;
					*(p_mat + count*3 +1) = plane[0].points[i].y;
					*(p_mat + count*3 +2) = plane[0].points[i].z;

					x_tot = x_tot + plane[0].points[i].x;
					y_tot = y_tot + plane[0].points[i].y;
					z_tot = z_tot + plane[0].points[i].z;


					count++;
				}

				if(count == points_defined)
				{
					x_tot = x_tot / count;
					y_tot = y_tot / count;
					z_tot = z_tot / count;

					break;
				}
			}

			//keyclr();

			if((ff=FitPlane(p_mat, points_defined, &para, &parb, &parc))== -1)
			{
				//sprintf(msgstr,"%-s",CANNOT_CALCULATE_TEXT);
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else if(ff == -2)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_ABORTED_TOK);
				msgbox.ShowMessageOk(msg);

				ret = false;
			}
			else
			{
				n[0] = para;
				n[1] = parb;
				n[2] = parc; //-1.0;

				// to unit vector

				dist = sqrt(n[0] *n[0] + n[1]*n[1] + n[2] *n[2]);

				if(dist != 0.0)
				{

					plane[0].px = x_tot; // *(p_mat + 0);
					plane[0].py = y_tot; // *(p_mat + 1);
					plane[0].pz = z_tot; // *(p_mat + 2);

					plane[0].nx = n[0];
					plane[0].ny = n[1];
					plane[0].nz = n[2];
					plane[0].calc = 1;
					plane[0].sta = PLANE_DEFINED;
				}
				else
				{
					StringC msg;
					msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					ret = false;
				}
			}

			// if(p_mat != NULL) free(p_mat);
			
		}
		else
		{
			ret = false;
		}

		if(actdes == BOTH && des_ok)// == true)
		{
//		sprintf(msgTxt,"%-s, %-s",CALCULATING_TEXT,PRESS_ESC_TO_ABORT);
//		show_info(msgTxt);

	//	p_mat = (double *) calloc(points_defined, sizeof(double) * 3);

		des_ok = true;

		if(p_mat != NULL)
		{
			count = 0;

			/* select all defined points from table */
			for(i=0; i < MAX_POINTS_IN_PLANE; i++)
			{
				if(plane[0].points[i].sta == POINT_DESIGN || plane[0].points[i].sta == POINT_MEASURED)
				{
					if(count == 0)
					{
						a.x = plane[0].points[i].xdes;
						a.y = plane[0].points[i].ydes;
						a.z = plane[0].points[i].zdes;
					}

					else if(count == 1)
					{
						b.x = plane[0].points[i].xdes;
						b.y = plane[0].points[i].ydes;
						b.z = plane[0].points[i].zdes;
					}

					else if(count == 2)
					{
						c.x = plane[0].points[i].xdes;
						c.y = plane[0].points[i].ydes;
						c.z = plane[0].points[i].zdes;
					}

					*(p_mat + count*3 +0) = plane[0].points[i].xdes;
					*(p_mat + count*3 +1) = plane[0].points[i].ydes;
					*(p_mat + count*3 +2) = plane[0].points[i].zdes;

					x_tot = x_tot + plane[0].points[i].xdes;
					y_tot = y_tot + plane[0].points[i].ydes;
					z_tot = z_tot + plane[0].points[i].zdes;


					count++;
				}

				if(count == points_defined)
				{
					x_tot = x_tot / count;
					y_tot = y_tot / count;
					z_tot = z_tot / count;

					break;
				}
			}
			if((ff=FitPlane(p_mat, points_defined, &para, &parb, &parc))== -1)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else if(ff == -2)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_ABORTED_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else
			{
				n[0] = para;
				n[1] = parb;
				n[2] = parc; //-1.0;

				// to unit vector

				dist = sqrt(n[0] *n[0] + n[1]*n[1] + n[2] *n[2]);

				if(dist != 0.0)
				{

					plane[0].des_px = x_tot; // *(p_mat + 0);
					plane[0].des_py = y_tot; // *(p_mat + 1);
					plane[0].des_pz = z_tot; // *(p_mat + 2);

					plane[0].des_nx = n[0];
					plane[0].des_ny = n[1];
					plane[0].des_nz = n[2];
					plane[0].des_calc = 1;
					plane[0].des_sta = PLANE_DEFINED;
				}
				else
				{
					StringC msg;
					msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					ret = false;
				}
			}

			//if(p_mat != NULL) free(p_mat);
		}
		else
		{
		}
	}
	}
	if(ret != false)
	{
/*		if(SHOW_INFO == 1)
		{
			sprintf(msgTxt,"A=%5.5f B=%5.5f C=%5.5f\npoint in plane\nx=%-1f y=%.1f z=%.1f",para,parb,parc,
											plane[0].px, plane[0].py, plane[0].pz);
			show_message(msgTxt);
		}
*/
	}
	return ret;
}

// *********************************************************************
//
// *********************************************************************
short DCP::DCP05CalcPlaneC::defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint)
{
	short count=0,i,sta;

	if(lastpoint != NULL)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_PLANE; i++)
	{
		sta = plane[0].points[i].sta;

		if(sta == 1 || sta == 2)
			count++;
		
		if(sta != 0)
		{
			if(lastpoint != NULL)
				*lastpoint = i+1;
		}
	}
	return count;
}


// *********************************************************************
// Laskee monta pistettä oetetaan laskentaan	
// *********************************************************************
