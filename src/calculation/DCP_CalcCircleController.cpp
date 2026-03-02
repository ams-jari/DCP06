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
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/calculation/DCP_CalcCircleController.hpp>
#include <dcp06/calculation/DCP_CalcCircle.hpp>
#include <dcp06/orientation/DCP_DefineLine.hpp>
#include <dcp06/calculation/DCP_ResCircle.hpp>
#include <dcp06/core/DCP_InputText.hpp>

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

DCP::DCP05CalcCircleControllerC::DCP05CalcCircleControllerC(DCP05CircleModelC* pCircleModel, short iDisplay):
	m_pDataModel(pCircleModel), m_iDisplay(iDisplay)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new DCP05CommonC();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05CalcCircleControllerC::~DCP05CalcCircleControllerC()
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
void DCP::DCP05CalcCircleControllerC::OnControllerActivated(void)
{
}

void DCP::DCP05CalcCircleControllerC::Run(void)
{
	DCP05CalcCircleC circle(m_pDataModel->PLANE_TYPE, 
								&m_pDataModel->circle_points[0],
								&m_pDataModel->planes[0],
								&m_pDataModel->circle_points_in_plane[0],
								m_pDataModel->bR, m_pDCP05Model);
	
	if(circle.calc_center_of_circle())
	{
			m_pDataModel->m_iCounts = m_pDataModel->pCommon->defined_circle_points(&m_pDataModel->circle_points[0],0);
			circle.get_results(&m_pDataModel->cx, &m_pDataModel->cy,&m_pDataModel->cz,&m_pDataModel->vi, &m_pDataModel->vj,&m_pDataModel->vk,&m_pDataModel->diameter, &m_pDataModel->rms_diameter,&m_pDataModel->max_dev_in_point);
			if(m_pDataModel->show_circle_points() == 1)
			{
				//m_pDataModel->circle_points[0].sta = 1;
				//m_pDataModel->circle_points[0].calc = 1;
				//if(m_pDataModel->m_iCounts > 3)
				//{
						DCP::DCP05CircleModelC* pModel = new DCP05CircleModelC(m_pDCP05Model);
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

						//memcpy(&pModel->circle_points[0],&m_pDataModel->circle_points[0],sizeof(S_CIRCLE_BUFF));
						//memcpy(&pModel->circle_in_plane[0],&m_pDataModel->circle_points_in_plane[0],sizeof(S_CIRCLE_BUFF));
						
						if(GetController(RES_CIRCLE_CONTROLLER) == NULL)
						{
							(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::DCP05ResCircleControllerC(m_pDCP05Model, m_iDisplay) );
						}

						(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_DEV_OF_CIRCLE_TOK));

						(void)GetController( RES_CIRCLE_CONTROLLER )->SetModel(pModel);
						SetActiveController(RES_CIRCLE_CONTROLLER, true);
				//}
				//else
				//	Close(EC_KEY_CONT);
			}
			else
				Close(EC_KEY_ESC);
	}
	else
		Close(EC_KEY_ESC);
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP05CalcCircleControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );
	m_pCommon = new DCP05CommonC(m_pDCP05Model);
	return/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    // return SetModel( pModel );
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP05CalcCircleControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP05CalcCircleControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		// kopioi arvot takaisin...
		DCP::DCP05CircleModelC* pModel = (DCP::DCP05CircleModelC*) GetController( RES_CIRCLE_CONTROLLER )->GetModel();	

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


		//DestroyController( lCtrlID );	
		Close(EC_KEY_CONT);
	}
	else
	{
		//DestroyController( lCtrlID );	
		Close(EC_KEY_ESC);
	}
	DestroyController( lCtrlID );	
}

