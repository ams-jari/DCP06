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

#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_Defs.hpp>


#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06MsgBoxC::DCP06MsgBoxC()
{
	
}
// ****************************************************************************************
DCP::DCP06MsgBoxC::~DCP06MsgBoxC()
{
	
}
// ****************************************************************************************
bool DCP::DCP06MsgBoxC::ShowMessageYesNo(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information, 0);    
		pDlg->SetMessageText( strMessage );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Yes|GUI::MessageDialogC::MBR_No ));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Yes)
		{
			return true;
		}
		return false;
}		

// ****************************************************************************************
bool DCP::DCP06MsgBoxC::ShowMessageOkAbort(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( strMessage );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort ));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Ok)
		{
			return true;
		}
		return false;
}		

// ****************************************************************************************
void DCP::DCP06MsgBoxC::ShowMessageOk(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( strMessage );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok ));
		(void)pDlg->Execute();
}

// ****************************************************************************************
short DCP::DCP06MsgBoxC::ShowMessageOkAbortRetry(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( strMessage );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Ok)
		{
			return 1;
		}
		else if(eRes == GUI::MessageDialogC::MBR_Abort)
			return -1;
		return 0;
}

// ****************************************************************************************
short DCP::DCP06MsgBoxC::DeleteChstMessage(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0); 
		
		pDlg->SetMessageText( strMessage );

		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = pDlg;
		
		vDef.strLable	= StringC(AT_DCP06,K_DCP_ALL_TOK);
		pDlg->SetFunctionKey( FK2, vDef );

		vDef.strLable	= StringC(AT_DCP06,K_DCP_STATION_TOK);
		pDlg->SetFunctionKey( FK4, vDef );

		vDef.strLable	=StringC(AT_DCP06, K_DCP_ABORT_TOK);
		pDlg->SetFunctionKey( FK6, vDef );

		//pDlg->ActivateFunctionKeys();
		
		//pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		
		//GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		FunctionKeyT eRes = pDlg->GetResultAsFK();
		if(eRes == FK2) // ALL
		{
			return 1;
		}
		else if(eRes == FK4)
		{
			return 2;
		}
		else
			return -1;
	}

short DCP::DCP06MsgBoxC::DeletePointsMeas(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0); 
		
		pDlg->SetMessageText( strMessage );

		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = pDlg;
		
		vDef.strLable	= StringC(AT_DCP06,K_DCP_ALL_TOK);
		pDlg->SetFunctionKey( FK2, vDef );

		vDef.strLable	= StringC(AT_DCP06,K_DCP_CURRENT_POINT_TOK);
		pDlg->SetFunctionKey( FK4, vDef );

		vDef.strLable	=StringC(AT_DCP06, K_DCP_ABORT_TOK);
		pDlg->SetFunctionKey( FK6, vDef );

		//pDlg->ActivateFunctionKeys();
		
		//pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		
		//GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		FunctionKeyT eRes = pDlg->GetResultAsFK();
		if(eRes == FK2) // ALL
		{
			return 1;
		}
		else if(eRes == FK4)
		{
			return 2;
		}
		else
			return -1;
}