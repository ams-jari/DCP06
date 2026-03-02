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
#include <dcp06/calculation/DCP_CalcLine.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include "calc.h"

#include   <math.h>


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

DCP::DCP05CalcLineC::DCP05CalcLineC()
{

}
DCP::DCP05CalcLineC::~DCP05CalcLineC()
{

}
// *********************************************************************
//
// *********************************************************************
short DCP::DCP05CalcLineC::calc(S_LINE_BUFF *line, short actdes)
{
short points_defined,i, ret1,lastpoint;
struct ams_vector a,b,a_des,b_des;
struct line c, c_des;
double para,parb,parc;
short   count;
bool	des_ok,ret=true;
double p_mat[MAX_POINTS_IN_LINE*3];
double x_tot = 0.0, y_tot = 0.0, z_tot = 0.0;


	DCP05MsgBoxC msgbox;

	line->des_calc = 0;
	line->des_sta =LINE_NOT_DEFINED;
	line->calc = 0;
	line->sta = LINE_NOT_DEFINED;

	points_defined = defined_points_count_in_line(line, &lastpoint);

	if(points_defined < 2)
	{
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_IN_MIN_2_POINTS_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
	}

	else if(points_defined == 2)
	{
		count = 0;
		des_ok=true;

		for(i=0;i < MAX_POINTS_IN_LINE; i++)
		{
			if(line[0].points[i].sta == 1 || line[0].points[i].sta == 2)
			{
				count++;

				if(count == 1)
				{
					a.x = line[0].points[i].x;
					a.y = line[0].points[i].y;
					a.z = line[0].points[i].z;

					if(actdes == BOTH)
					{
						if(line[0].points[i].dsta == 1 ||line[0].points[i].dsta == 2)
						{
							a_des.x = line[0].points[i].xdes;
							a_des.y = line[0].points[i].ydes;
							a_des.z = line[0].points[i].zdes;
						}
						else
							des_ok = false;
					}
				}
				if(count == 2)
				{
					b.x = line[0].points[i].x;
					b.y = line[0].points[i].y;
					b.z = line[0].points[i].z;

					if(actdes == BOTH && des_ok == true)
					{
						if(line[0].points[i].dsta == 1 || line[0].points[i].dsta == 2)
						{
							b_des.x = line[0].points[i].xdes;
							b_des.y = line[0].points[i].ydes;
							b_des.z = line[0].points[i].zdes;
							des_ok=true;
						}
						else
							des_ok = false;
					}
					break;
				}
			}
		}

		equation_of_line(&a, &b, &c);

		line[0].px = c.px;
		line[0].py = c.py;
		line[0].pz = c.pz;

		para = c.ux;
		parb = c.uy;
		parc = c.uz;

		line[0].ux = c.ux;
		line[0].uy = c.uy;
		line[0].uz = c.uz;
		line[0].calc = 1;
		line[0].sta = LINE_DEFINED;

		if(actdes == BOTH && des_ok == true)
		{

			equation_of_line(&a_des, &b_des, &c_des);

			line[0].des_px = c_des.px;
			line[0].des_py = c_des.py;
			line[0].des_pz = c_des.pz;

			/*
			para = c.ux;
			parb = c.uy;
			parc = c.uz;
			*/

			line[0].des_ux = c_des.ux;
			line[0].des_uy = c_des.uy;
			line[0].des_uz = c_des.uz;
			line[0].des_calc = 1;
			line[0].des_sta = LINE_DEFINED;
		}
	}
	else
	{
		des_ok=true;


#ifdef diagnostics
			PrintLn("Line > 2 points (actual)");
#endif
			count = 0;

			for(i=0; i < MAX_POINTS_IN_LINE; i++)
			{
				if(line->points[i].sta == 1 || line->points[i].sta == 2)
				{
					p_mat[count*3 +0] = line[0].points[i].x;
					p_mat[count*3 +1] = line[0].points[i].y;
					p_mat[count*3 +2] = line[0].points[i].z;

					if(actdes == BOTH && des_ok != false)
					{
						if(line[0].points[i].dsta == 0)
						{
							des_ok = false;
						}
					}

					x_tot = x_tot + line[0].points[i].x;
					y_tot = y_tot + line[0].points[i].y;
					z_tot = z_tot + line[0].points[i].z;
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
			if((ret1=FitLine(p_mat, points_defined, &para, &parb, &parc))== -1)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else if(ret1 == -2)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_ABORTED_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;	
			}
			else
			{
				line[0].ux = para;
				line[0].uy = parb;
				line[0].uz = parc;

				line[0].px = x_tot; //p_mat[0];
				line[0].py = y_tot; //p_mat[1];
				line[0].pz = z_tot; //p_mat[2];

				line[0].calc = 1;
				line[0].sta = LINE_DEFINED;
			}

		// CALCULATION IN DESIGN VALUES
		if(ret == true && actdes == BOTH && des_ok == true)
		{

			//show_info(CALCULATING_TEXT);

				count = 0;

				for(i=0; i < MAX_POINTS_IN_LINE; i++)
				{
					if(line[0].points[i].sta == 1 || line[0].points[i].sta == 2)
					{
						p_mat[count*3 +0] = line[0].points[i].xdes;
						p_mat[count*3 +1] = line[0].points[i].ydes;
						p_mat[count*3 +2] = line[0].points[i].zdes;

						x_tot = x_tot + line[0].points[i].xdes;
						y_tot = y_tot + line[0].points[i].ydes;
						z_tot = z_tot + line[0].points[i].zdes;
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

				if(FitLine(p_mat, points_defined, &para, &parb, &parc)== -1)
				{
					StringC msg;
					msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					ret = false;
				}
				else
				{
					line[0].des_ux = para;
					line[0].des_uy = parb;
					line[0].des_uz = parc;

					line[0].des_px = x_tot; //p_mat[0];
					line[0].des_py = y_tot; //p_mat[1];
					line[0].des_pz = z_tot; //p_mat[2];

					line[0].des_calc = 1;
					line[0].des_sta = LINE_DEFINED;
				}
		}
	}

	if(ret != false)
	{
		/*
		if(SHOW_INFO == 1)
		{
			sprintf(msgTxt,"A=%.5f B=%.5f C=%.5f\nPoint in line\nx=%.1f y=%.1f z=%.1f",para,parb,parc,
							line->px, line->py,line->pz);
			show_message(msgTxt);
		}
		*/
	}

	return ret;
}

// *********************************************************************
//
// *********************************************************************
short DCP::DCP05CalcLineC::points_count_in_line(S_LINE_BUFF *line)
{
short count=0,i;

	for(i=0; i< MAX_POINTS_IN_LINE; i++)
	{
		if(line[0].points[i].sta != 0)
			count++;
	}
	return count;
}


// *********************************************************************
// Laskee monta pistettä oetetaan laskentaan	
// *********************************************************************
short DCP::DCP05CalcLineC::defined_points_count_in_line(S_LINE_BUFF *line,short *lastpoint)
{
short count=0,i,sta;

	if(lastpoint != NULL)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_LINE; i++)
	{
		sta = line[0].points[i].sta;
		
		if(sta == 1 || sta == 2)
		{
			count++;
		}

		if(sta != 0)
		{
			if(lastpoint != NULL)
				*lastpoint = i+1;
		}
	}
	return count;
}
