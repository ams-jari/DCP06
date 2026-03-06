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
#include "calc.h"
#include <math.h>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/measurement/MidPointController.hpp>
//#include <dcp06/calculation/calcline.hpp>
//#include <dcp06/orientation/DefineLine.hpp>
//#include <dcp06/orientation/ResLine.hpp>
//#include <dcp06/core/InputText.hpp>

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
// ====================================  Meas3DController ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================

DCP::MidPointController::MidPointController(DCP::Model *pModel):
	m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new Common();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::MidPointController::~MidPointController()
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
void DCP::MidPointController::OnControllerActivated(void)
{
}

void DCP::MidPointController::Run(void)
{
		
		DCP::MeasureModel* pModel = new MeasureModel;
		pModel->m_iPointsCount = MAX_MID_POINTS;
		pModel->m_iMaxPoint = MAX_MID_POINTS;
		pModel->m_iMinPoint = MAX_MID_POINTS;
	
		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_MID_POINTS);
		memcpy(&pModel->point_table[0],  &m_pModel->mid_points[0], sizeof(S_POINT_BUFF) * MAX_MID_POINTS);

		//memcpy(&pModel->point_table[0],&m_pDataModel->planes[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);

	
		if(GetController(MEAS_MID_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_MID_POINT_CONTROLLER, new DCP::MeasureController(m_pModel) );
		}

		(void)GetController(MEAS_MID_POINT_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_MID_POINT_TOK));

		(void)GetController( MEAS_MID_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_MID_POINT_CONTROLLER, true);
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::MidPointController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*GUI::*/ControllerC::SetModel( pModel );

	//m_pModel = dynamic_cast< DCP::Model* >( pModel );
	m_pCommon = new Common(m_pModel);
	m_pPointBuffModel = (PointBuffModel*) pModel;
	return /*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    // return SetModel( pModel );
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::MidPointController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

DCP::PointBuffModel* DCP::MidPointController::GetDataModel() const
{
    return (DCP::PointBuffModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::MidPointController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_MID_POINT_CONTROLLER )->GetModel();	
		
		double x_mid = 0.0;
		double y_mid = 0.0;
		double z_mid = 0.0;


		memcpy(&m_pModel->mid_points[0], &pModel->point_table[0],sizeof(S_POINT_BUFF) * MAX_MID_POINTS);	

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

