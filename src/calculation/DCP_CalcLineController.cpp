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
#include <dcp06/calculation/DCP_CalcLineController.hpp>
#include <dcp06/calculation/DCP_CalcLine.hpp>
#include <dcp06/orientation/DCP_DefineLine.hpp>
#include <dcp06/orientation/DCP_ResLine.hpp>
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

DCP::DCP06CalcLineControllerC::DCP06CalcLineControllerC(S_LINE_BUFF* oLineBuff, short actdes, short iAskId):
	m_pLineBuff(oLineBuff),m_iActDes(actdes), m_iAskId(iAskId)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new DCP06CommonC();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06CalcLineControllerC::~DCP06CalcLineControllerC()
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
void DCP::DCP06CalcLineControllerC::OnControllerActivated(void)
{
}

void DCP::DCP06CalcLineControllerC::Run(void)
{
	DCP06CalcLineC calcline;
	if(calcline.calc(m_pLineBuff,m_iActDes))
	{
		m_pLineBuff->calc = 1;
		m_pLineBuff->sta = 1;

		if(m_pCommon->defined_points_count_in_line(&m_pLineBuff[0],0) == 2)
		{
			if(m_iAskId)
			{
				DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_LINE_ID_TOK);
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
				Close(EC_KEY_CONT);
		}
		else if(m_pCommon->defined_points_count_in_line(&m_pLineBuff[0],0) > 2)
		{
				DCP::DCP06DefineLineModelC* pModel = new DCP::DCP06DefineLineModelC;
				memcpy(&pModel->line_buff[0],&m_pLineBuff[0], sizeof(S_LINE_BUFF));
				if(GetController(RES_LINE_CONTROLLER) == NULL)
				{
					(void)AddController( RES_LINE_CONTROLLER, new DCP::DCP06ResLineControllerC(m_pDCP06Model));
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

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06CalcLineControllerC::SetModel( GUI::ModelC* pModel )
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
void DCP::DCP06CalcLineControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06CalcLineControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == RES_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06DefineLineModelC* pModel = (DCP::DCP06DefineLineModelC*) GetController( RES_LINE_CONTROLLER )->GetModel();		
		memcpy(&m_pLineBuff[0],&pModel->line_buff[0], sizeof(S_LINE_BUFF));
		
		if(m_iAskId)
		{
				DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_LINE_ID_TOK);
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
	if(lCtrlID == INPUT_TEXT_CONTROLLER) 
	{
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char buffer[10]; buffer[0] = '\0';
			m_pCommon->convert_to_ascii(strNewFile, buffer,7); // 6 -> 7 280508
			sprintf(m_pLineBuff->id,"%-6.6s",buffer);
			//DestroyController( lCtrlID );	
			Close(EC_KEY_CONT);
		}
		else
		{
			//DestroyController( lCtrlID );	
			Close(EC_KEY_ESC);

		}
	}
	DestroyController( lCtrlID );	
}

