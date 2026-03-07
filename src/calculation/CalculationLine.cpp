// ================================================================================================
// CalculationLine.cpp - Line calculation core and controller
// Consolidated from calcline.cpp and CalcLineController.cpp
// ================================================================================================

#include "stdafx.h"
#include "calc.h"
#include <math.h>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/orientation/DefineLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/core/InputText.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ========== CalcLine ==========
DCP::CalcLine::CalcLine() {}
DCP::CalcLine::~CalcLine() {}

short DCP::CalcLine::calc(S_LINE_BUFF* line, short actdes)
{
short points_defined,i, ret1,lastpoint;
struct ams_vector a,b,a_des,b_des;
struct line c, c_des;
double directionX, directionY, directionZ;
short   count;
bool	designValuesValid,ret=true;
double p_mat[MAX_POINTS_IN_LINE*3];
double x_tot = 0.0, y_tot = 0.0, z_tot = 0.0;

	MsgBox msgbox;

	line->des_calc = 0;
	line->des_sta =LINE_NOT_DEFINED;
	line->calc = 0;
	line->sta = LINE_NOT_DEFINED;

	points_defined = defined_points_count_in_line(line, &lastpoint);

	if(points_defined < 2)
	{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_2_POINTS_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
	}

	else if(points_defined == 2)
	{
		count = 0;
		designValuesValid=true;

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
							designValuesValid = false;
					}
				}
				if(count == 2)
				{
					b.x = line[0].points[i].x;
					b.y = line[0].points[i].y;
					b.z = line[0].points[i].z;

					if(actdes == BOTH && designValuesValid == true)
					{
						if(line[0].points[i].dsta == 1 || line[0].points[i].dsta == 2)
						{
							b_des.x = line[0].points[i].xdes;
							b_des.y = line[0].points[i].ydes;
							b_des.z = line[0].points[i].zdes;
							designValuesValid=true;
						}
						else
							designValuesValid = false;
					}
					break;
				}
			}
		}

		equation_of_line(&a, &b, &c);

		line[0].px = c.px;
		line[0].py = c.py;
		line[0].pz = c.pz;

		directionX = c.ux;
		directionY = c.uy;
		directionZ = c.uz;

		line[0].ux = c.ux;
		line[0].uy = c.uy;
		line[0].uz = c.uz;
		line[0].calc = 1;
		line[0].sta = LINE_DEFINED;

		if(actdes == BOTH && designValuesValid == true)
		{

			equation_of_line(&a_des, &b_des, &c_des);

			line[0].des_px = c_des.px;
			line[0].des_py = c_des.py;
			line[0].des_pz = c_des.pz;

			line[0].des_ux = c_des.ux;
			line[0].des_uy = c_des.uy;
			line[0].des_uz = c_des.uz;
			line[0].des_calc = 1;
			line[0].des_sta = LINE_DEFINED;
		}
	}
	else
	{
		designValuesValid=true;

		count = 0;

		for(i=0; i < MAX_POINTS_IN_LINE; i++)
		{
			if(line->points[i].sta == 1 || line->points[i].sta == 2)
			{
				p_mat[count*3 +0] = line[0].points[i].x;
				p_mat[count*3 +1] = line[0].points[i].y;
				p_mat[count*3 +2] = line[0].points[i].z;

				if(actdes == BOTH && designValuesValid != false)
				{
					if(line[0].points[i].dsta == 0)
					{
						designValuesValid = false;
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
		if((ret1=FitLine(p_mat, points_defined, &directionX, &directionY, &directionZ))== -1)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
		}
		else if(ret1 == -2)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_ABORTED_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;	
		}
		else
		{
			line[0].ux = directionX;
			line[0].uy = directionY;
			line[0].uz = directionZ;

			line[0].px = x_tot;
			line[0].py = y_tot;
			line[0].pz = z_tot;

			line[0].calc = 1;
			line[0].sta = LINE_DEFINED;
		}

		// CALCULATION IN DESIGN VALUES
		if(ret == true && actdes == BOTH && designValuesValid == true)
		{

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

				if(FitLine(p_mat, points_defined, &directionX, &directionY, &directionZ)== -1)
				{
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_CALC_TOK);
					msgbox.ShowMessageOk(msg);
					ret = false;
				}
				else
				{
					line[0].des_ux = directionX;
					line[0].des_uy = directionY;
					line[0].des_uz = directionZ;

					line[0].des_px = x_tot;
					line[0].des_py = y_tot;
					line[0].des_pz = z_tot;

					line[0].des_calc = 1;
					line[0].des_sta = LINE_DEFINED;
				}
		}
	}

	return ret;
}

short DCP::CalcLine::points_count_in_line(S_LINE_BUFF* line)
{
short count=0,i;

	for(i=0; i< MAX_POINTS_IN_LINE; i++)
	{
		if(line[0].points[i].sta != 0)
			count++;
	}
	return count;
}

short DCP::CalcLine::defined_points_count_in_line(S_LINE_BUFF* line,short* lastpoint)
{
short count=0,i,sta;

	if(lastpoint != nullptr)
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
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return count;
}

// ========== CalcLineController ==========
DCP::CalcLineController::CalcLineController(S_LINE_BUFF* oLineBuff, short actdes, short iAskId):
	m_pLineBuff(oLineBuff),m_iActDes(actdes), m_iAskId(iAskId), m_pCommon(0), m_pModel(0)
{
}

DCP::CalcLineController::~CalcLineController()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::CalcLineController::OnControllerActivated(void)
{
}

void DCP::CalcLineController::Run(void)
{
	CalcLine calcline;
	if(calcline.calc(m_pLineBuff,m_iActDes))
	{
		m_pLineBuff->calc = 1;
		m_pLineBuff->sta = 1;

		if(m_pCommon->defined_points_count_in_line(&m_pLineBuff[0],0) == 2)
		{
			if(m_iAskId)
			{
				DCP::InputTextModel* pModel = new InputTextModel;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_LINE_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = 6;
				pModel->m_StrText = L" ";

				if ( nullptr == pModel)
				{
					USER_APP_VERIFY( false );
					return;
				}

				if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
				{
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pModel ));
				}

				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);
			}
			else
				Close(EC_KEY_CONT);
		}
		else if(m_pCommon->defined_points_count_in_line(&m_pLineBuff[0],0) > 2)
		{
				DCP::DefineLineModel* pModel = new DCP::DefineLineModel;
				memcpy(&pModel->line_buff[0],&m_pLineBuff[0], sizeof(S_LINE_BUFF));
				if(GetController(RES_LINE_CONTROLLER) == nullptr)
				{
					(void)AddController( RES_LINE_CONTROLLER, new DCP::ResLineController(m_pModel));
				}
	
				(void)GetController(RES_LINE_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK));
	
				(void)GetController( RES_LINE_CONTROLLER )->SetModel(pModel);
				SetActiveController(RES_LINE_CONTROLLER, true);
		}
	}
	else
	{
		Close(EC_KEY_ESC);
	}
}

bool DCP::CalcLineController::SetModel( GUI::ModelC* pModel )
{
	m_pModel = dynamic_cast< DCP::Model* >( pModel );
	m_pCommon = new Common(m_pModel);
	return ControllerC::SetModel( pModel );
}

void DCP::CalcLineController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	(void)lDlgID;
	(void)lExitCode;
}

void DCP::CalcLineController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DefineLineModel* pModel = (DCP::DefineLineModel*) GetController( RES_LINE_CONTROLLER )->GetModel();		
		memcpy(&m_pLineBuff[0],&pModel->line_buff[0], sizeof(S_LINE_BUFF));
		
		if(m_iAskId)
		{
				DCP::InputTextModel* pModel = new InputTextModel;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_LINE_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = 6;
				pModel->m_StrText = L" ";

				if ( nullptr == pModel)
				{
					USER_APP_VERIFY( false );
					return;
				}

				if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
				{
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pModel ));
				}

				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);


		}
		else
		{
			Close(EC_KEY_CONT);
		}
	}

	if(lCtrlID == INPUT_TEXT_CONTROLLER) 
	{
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char buffer[10]; buffer[0] = '\0';
			m_pCommon->convert_to_ascii(strNewFile, buffer,7);
			sprintf(m_pLineBuff->id,"%-6.6s",buffer);
			Close(EC_KEY_CONT);
		}
		else
		{
			Close(EC_KEY_ESC);

		}
	}
	DestroyController( lCtrlID );	
}
