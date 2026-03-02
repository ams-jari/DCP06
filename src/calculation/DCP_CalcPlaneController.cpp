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
#include <dcp06/calculation/DCP_CalcPlaneController.hpp>
#include <dcp06/calculation/DCP_CalcPlane.hpp>
#include <dcp06/orientation/DCP_DefinePlane.hpp>
#include <dcp06/orientation/DCP_ResPlane.hpp>
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
// ====================================  DCP063DMeasControllerC ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================

DCP::DCP06CalcPlaneControllerC::DCP06CalcPlaneControllerC(S_PLANE_BUFF* oLineBuff, short actdes, short iAskId):
	m_pPlaneBuff(oLineBuff),m_iActDes(actdes), m_iAskId(iAskId)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new DCP06CommonC();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06CalcPlaneControllerC::~DCP06CalcPlaneControllerC()
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
				pModel->m_StrText = L" ";//StringC(buffer);

				if ( NULL == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
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

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06CalcPlaneControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );
	m_pCommon = new DCP06CommonC(m_pDCP06Model);
	return/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    // return SetModel( pModel );
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP06CalcPlaneControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06CalcPlaneControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_PLANE_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06DefinePlaneModelC* pModel = (DCP::DCP06DefinePlaneModelC*) GetController( RES_PLANE_CONTROLLER )->GetModel();		
		memcpy(&m_pPlaneBuff[0],&pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
		
		if(m_iAskId)
		{
				DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_PLANE_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = 6;
				pModel->m_StrText = L" ";//StringC(buffer);

				if ( NULL == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
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
			//DestroyController( lCtrlID );	
			Close(EC_KEY_CONT);
		}
	}

	// ask line id
	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
		StringC strNewFile = pModel->m_StrText;
		char buffer[10]; buffer[0] = '\0';
		m_pCommon->convert_to_ascii(strNewFile, buffer,7); // 6->7 280508
 		sprintf(m_pPlaneBuff->id,"%-6.6s",buffer);

		//DestroyController( lCtrlID );	
		Close(EC_KEY_CONT);
	}
	DestroyController( lCtrlID );	
}

