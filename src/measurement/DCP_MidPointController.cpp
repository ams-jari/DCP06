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
#include "calc.h"
#include <math.h>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_DCP05Meas.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/measurement/DCP_MidPointController.hpp>
//#include <dcp06/calculation/DCP_CalcLine.hpp>
//#include <dcp06/orientation/DCP_DefineLine.hpp>
//#include <dcp06/orientation/DCP_ResLine.hpp>
//#include <dcp06/core/DCP_InputText.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_Util.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// ================================================================================================
// ====================================  DCP053DMeasControllerC ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================

DCP::DCP05MidPointControllerC::DCP05MidPointControllerC(DCP::DCP05ModelC *pDCP05Model):
	m_pDCP05Model(pDCP05Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new DCP05CommonC();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05MidPointControllerC::~DCP05MidPointControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::DCP05MidPointControllerC::OnControllerActivated(void)
{
}

void DCP::DCP05MidPointControllerC::Run(void)
{
		
		DCP::DCP05MeasModelC* pModel = new DCP05MeasModelC;
		pModel->m_iPointsCount = MAX_MID_POINTS;
		pModel->m_iMaxPoint = MAX_MID_POINTS;
		pModel->m_iMinPoint = MAX_MID_POINTS;
	
		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_MID_POINTS);
		memcpy(&pModel->point_table[0],  &m_pDCP05Model->mid_points[0], sizeof(S_POINT_BUFF) * MAX_MID_POINTS);

		//memcpy(&pModel->point_table[0],&m_pDataModel->planes[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);

	
		if(GetController(MEAS_MID_POINT_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_MID_POINT_CONTROLLER, new DCP::DCP05MeasControllerC(m_pDCP05Model) );
		}

		(void)GetController(MEAS_MID_POINT_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_MID_POINT_TOK));

		(void)GetController( MEAS_MID_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_MID_POINT_CONTROLLER, true);
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP05MidPointControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*GUI::*/ControllerC::SetModel( pModel );

	//m_pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );
	m_pCommon = new DCP05CommonC(m_pDCP05Model);
	m_pPointBuffModel = (DCP05PointBuffModelC*) pModel;
	return /*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    // return SetModel( pModel );
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP05MidPointControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

DCP::DCP05PointBuffModelC* DCP::DCP05MidPointControllerC::GetDataModel() const
{
    return (DCP::DCP05PointBuffModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP05MidPointControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05MeasModelC* pModel = (DCP::DCP05MeasModelC*) GetController( MEAS_MID_POINT_CONTROLLER )->GetModel();	
		
		double x_mid = 0.0;
		double y_mid = 0.0;
		double z_mid = 0.0;


		memcpy(&m_pDCP05Model->mid_points[0], &pModel->point_table[0],sizeof(S_POINT_BUFF) * MAX_MID_POINTS);	

		//if(m_pCommon->calc_mid_point(&pModel->point_table[0], &pModel->point_table[1],&x_mid, &y_mid, &z_mid) == 0)
		if(m_pCommon->calc_mid_point(&pModel->point_table[0], &x_mid, &y_mid, &z_mid) == 0)
		{
			GetDataModel()->m_pPointBuff[0].x = x_mid;
			GetDataModel()->m_pPointBuff[0].y = y_mid;
			GetDataModel()->m_pPointBuff[0].z = z_mid;
			GetDataModel()->m_pPointBuff[0].sta = POINT_MEASURED;
			
			//DestroyController( lCtrlID );	
			Close(EC_KEY_CONT);
		}
		else
		{
			GUI::DesktopC::Instance()->MessageShow(L"Cannot calculate mid point!");
			Close(EC_KEY_ESC);
		}
	}

	DestroyController( lCtrlID );	
}

