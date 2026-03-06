// ================================================================================================
// CalculationPlane.cpp - Plane calculation core and controller
// Consolidated from calcplane.cpp and CalcPlaneController.cpp
// ================================================================================================

#include "stdafx.h"
#include "calc.h"
#include <math.h>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/orientation/DefinePlane.hpp>
#include <dcp06/orientation/ResPlane.hpp>
#include <dcp06/core/InputText.hpp>

#include "core/geometry/PlaneFitting.h"
#include "geometry/Point.h"
#include "geometry/Plane.h"
#include <vector>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ========== DCP06CalcPlaneC ==========
DCP::DCP06CalcPlaneC::DCP06CalcPlaneC() {}
DCP::DCP06CalcPlaneC::~DCP06CalcPlaneC() {}

short DCP::DCP06CalcPlaneC::calc(S_PLANE_BUFF* plane, short actdes)
{
short points_defined,i, ret=true;
struct ams_vector a,b,c,a_des,b_des,c_des;
struct plane d,d_des;
short count,ff,des_ok;
double p_mat[MAX_POINTS_IN_PLANE*3];
double para,parb,parc;
double n[3];
double dist;
double x_tot=0.0, y_tot=0.0, z_tot=0.0;

	DCP06MsgBoxC msgbox;
	(void)d; (void)d_des;

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
					if(actdes == BOTH && des_ok)
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

					if(actdes == BOTH && des_ok)
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

		DCP9::Geometry::Point p1(a.x, a.y, a.z), p2(b.x, b.y, b.z), p3(c.x, c.y, c.z);
		DCP9::Geometry::Plane dcp9Plane(p1, p2, p3);
		const std::vector<double>& normal = dcp9Plane.normal();
		if(normal[0] == 0.0 && normal[1] == 0.0 && normal[2] == 0.0)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
		}
		else
		{
			plane[0].px = dcp9Plane.point().x(); plane[0].py = dcp9Plane.point().y(); plane[0].pz = dcp9Plane.point().z();
			plane[0].nx = normal[0]; plane[0].ny = normal[1]; plane[0].nz = normal[2];
			para = normal[0]; parb = normal[1]; parc = normal[2];
			plane->calc = 1;
			plane->sta = PLANE_DEFINED;
		}
		if(actdes == BOTH && des_ok)
		{
			DCP9::Geometry::Point pd1(a_des.x, a_des.y, a_des.z), pd2(b_des.x, b_des.y, b_des.z), pd3(c_des.x, c_des.y, c_des.z);
			DCP9::Geometry::Plane dcp9PlaneDes(pd1, pd2, pd3);
			const std::vector<double>& normalDes = dcp9PlaneDes.normal();
			if(normalDes[0] != 0.0 || normalDes[1] != 0.0 || normalDes[2] != 0.0)
			{
				plane[0].des_px = dcp9PlaneDes.point().x(); plane[0].des_py = dcp9PlaneDes.point().y(); plane[0].des_pz = dcp9PlaneDes.point().z();
				plane[0].des_nx = normalDes[0]; plane[0].des_ny = normalDes[1]; plane[0].des_nz = normalDes[2];
				plane[0].des_calc = 1;
				plane[0].des_sta = PLANE_DEFINED;
			}
		}
	}
	else
	{
		des_ok = true;

		if(p_mat != NULL)
		{
			count = 0;

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

			std::vector<DCP9::Geometry::Point> pts;
			for(int idx=0; idx<points_defined; idx++)
				pts.push_back(DCP9::Geometry::Point(p_mat[idx*3+0], p_mat[idx*3+1], p_mat[idx*3+2]));
			DCP9::Core::Geometry::PlaneFitResult planeResult = DCP9::Core::Geometry::bestFitPlane(pts);
			ff = planeResult.isValid ? 1 : -1;
			if(ff == -1)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else
			{
				const std::vector<double>& normal = planeResult.plane.normal();
				n[0] = para = normal[0];
				n[1] = parb = normal[1];
				n[2] = parc = normal[2];
				dist = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
				if(dist != 0.0)
				{
					plane[0].px = planeResult.plane.point().x();
					plane[0].py = planeResult.plane.point().y();
					plane[0].pz = planeResult.plane.point().z();
					plane[0].nx = n[0]; plane[0].ny = n[1]; plane[0].nz = n[2];
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
		}
		else
		{
			ret = false;
		}

		if(actdes == BOTH && des_ok)
		{
		des_ok = true;

		if(p_mat != NULL)
		{
			count = 0;

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
			std::vector<DCP9::Geometry::Point> ptsDes;
			for(int idx=0; idx<points_defined; idx++)
				ptsDes.push_back(DCP9::Geometry::Point(p_mat[idx*3+0], p_mat[idx*3+1], p_mat[idx*3+2]));
			DCP9::Core::Geometry::PlaneFitResult planeResultDes = DCP9::Core::Geometry::bestFitPlane(ptsDes);
			ff = planeResultDes.isValid ? 1 : -1;
			if(ff == -1)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_CALC_TOK);
				msgbox.ShowMessageOk(msg);
				ret = false;
			}
			else
			{
				const std::vector<double>& normalDes = planeResultDes.plane.normal();
				n[0] = para = normalDes[0];
				n[1] = parb = normalDes[1];
				n[2] = parc = normalDes[2];
				dist = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
				if(dist != 0.0)
				{
					plane[0].des_px = planeResultDes.plane.point().x();
					plane[0].des_py = planeResultDes.plane.point().y();
					plane[0].des_pz = planeResultDes.plane.point().z();
					plane[0].des_nx = n[0]; plane[0].des_ny = n[1]; plane[0].des_nz = n[2];
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
		}
		else
		{
		}
	}
	}
	return ret;
}

short DCP::DCP06CalcPlaneC::defined_points_count_in_plane(S_PLANE_BUFF* plane,short* lastpoint)
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

// ========== DCP06CalcPlaneControllerC ==========
DCP::DCP06CalcPlaneControllerC::DCP06CalcPlaneControllerC(S_PLANE_BUFF* oLineBuff, short actdes, short iAskId):
	m_pPlaneBuff(oLineBuff),m_iActDes(actdes), m_iAskId(iAskId), m_pCommon(0), m_pDCP06Model(0)
{
}

DCP::DCP06CalcPlaneControllerC::~DCP06CalcPlaneControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP06CalcPlaneControllerC::OnControllerActivated(void)
{
}

void DCP::DCP06CalcPlaneControllerC::Run(void)
{
	DCP06CalcPlaneC calcplane;
	if(calcplane.calc(m_pPlaneBuff,m_iActDes))
	{
		m_pPlaneBuff->calc = 1;
		m_pPlaneBuff->sta = 1;

		if(m_pCommon->defined_points_count_in_plane(&m_pPlaneBuff[0],0) == 3)
		{
			if(m_iAskId)
			{
				DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_PLANE_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = 6;
				pModel->m_StrText = L" ";

				if ( NULL == pModel)
				{
					USER_APP_VERIFY( false );
					return;
				}

				if(GetController(INPUT_TEXT_CONTROLLER) == NULL)
				{
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP06InputTextControllerC( m_pDCP06Model));
				}

				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);
			}
			else
				Close(EC_KEY_CONT);
		}
		
		else if(m_pCommon->defined_points_count_in_plane(&m_pPlaneBuff[0],0) > 2)
		{
				DCP::DCP06DefinePlaneModelC* pModel = new DCP::DCP06DefinePlaneModelC;
				memcpy(&pModel->plane_buff[0],&m_pPlaneBuff[0], sizeof(S_PLANE_BUFF));
				if(GetController(RES_PLANE_CONTROLLER) == NULL)
				{
					(void)AddController( RES_PLANE_CONTROLLER, new DCP::DCP06ResPlaneControllerC(m_pDCP06Model));
				}
	
				(void)GetController(RES_PLANE_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK));
	
				(void)GetController( RES_PLANE_CONTROLLER )->SetModel(pModel);
				SetActiveController(RES_PLANE_CONTROLLER, true);
		}
	}
	else
		Close(EC_KEY_ESC);
}

bool DCP::DCP06CalcPlaneControllerC::SetModel( GUI::ModelC* pModel )
{
	m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );
	m_pCommon = new DCP06CommonC(m_pDCP06Model);
	return ControllerC::SetModel( pModel );
}

void DCP::DCP06CalcPlaneControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	(void)lDlgID;
	(void)lExitCode;
}

void DCP::DCP06CalcPlaneControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_PLANE_CONTROLLER)
	{
		DCP::DCP06DefinePlaneModelC* pModel = (DCP::DCP06DefinePlaneModelC*) GetController( RES_PLANE_CONTROLLER )->GetModel();		
		memcpy(&m_pPlaneBuff[0],&pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
		
		if(m_iAskId)
		{
				DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_PLANE_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = 6;
				pModel->m_StrText = L" ";

				if ( NULL == pModel)
				{
					USER_APP_VERIFY( false );
					return;
				}

				if(GetController(INPUT_TEXT_CONTROLLER) == NULL)
				{
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP06InputTextControllerC(m_pDCP06Model ));
				}

				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);


		}
		else
		{
			Close(EC_KEY_CONT);
		}
	}

	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
		StringC strNewFile = pModel->m_StrText;
		char buffer[10]; buffer[0] = '\0';
		m_pCommon->convert_to_ascii(strNewFile, buffer,7);
 		sprintf(m_pPlaneBuff->id,"%-6.6s",buffer);

		Close(EC_KEY_CONT);
	}
	DestroyController( lCtrlID );	
}
