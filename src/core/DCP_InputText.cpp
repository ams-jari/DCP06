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
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_InputTextModel.hpp>
#include <dcp06/core/DCP_Common.hpp>
//#include <dcp06/init/DCP_DCP05Init.hpp>
//#include <dcp06/core/DCP_DCP05Meas.hpp>
//#include <dcp06/core/DCP_SpecialMenu.hpp>
//#include <dcp06/core/DCP_xyz.hpp>
//#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/core/DCP_InputText.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GUI_KeyTypes.hpp>
#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP05InputTextDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP05InputTextDlgC::DCP05InputTextDlgC(DCP05ModelC* pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),m_pInfo(0),m_pDCP05Model(pDCP05Model),
	m_pText(0),m_pObserver(OBS_METHOD_TO_PARAM0(DCP05InputTextDlgC, OnValueChanged), this)
{
	//SetTxtApplicationId(AT_DCP05);

}


            // Description: Destructor
DCP::DCP05InputTextDlgC::~DCP05InputTextDlgC()
{

}

void DCP::DCP05InputTextDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	SetTitle(GetDataModel()->m_StrTitle);
	//InsertEmptyLine(); CAPTIVATE

	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	m_pInfo->SetText(GetDataModel()->m_StrInfoText);
	AddCtrl(m_pInfo);

	//InsertEmptyLine(); CAPTIVATE
	//InsertEmptyLine(); CAPTIVATE

	m_pText = new GUI::EditStringCtrlC();
	m_pText->SetId(eText);
	m_pText->SetCharsCountMax((unsigned char) GetDataModel()->m_iTextLength);
	//m_pText->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	m_pText->SetEmptyAllowed(true);
	//m_pText->SetWidth(9 *  GetDataModel()->m_iTextLength);  CAPTIVATE
	AddCtrl(m_pText);
	
	m_pObserver.Attach(m_pText->GetSubject());
}

bool DCP::DCP05InputTextDlgC::OnKeyPress  ( GUI::Key  eKey ) 
{
	if(eKey == GUI::Key_Escape)
	{
		Close(EC_KEY_ESC);
		return true;
	}
	//else if(eKey == GUI::Key_Return)
	//{	

	//	Close(EC_KEY_CONT);
	//	return true;
	//}
	else
	{
	return true;
	}
}

//void DCP::DCP05InputTextDlgC::OnDialogClosed(int  iExitCode )
//{
//	if(iExitCode == EC_KEY_ESC)
//	{
//		Close(EC_KEY_ESC);
//	}
//}

void DCP::DCP05InputTextDlgC::OnValueChanged( int unNotifyCode,  int ulParam2)
{	
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetDataModel()->iCurrentPoint;
		DCP05CommonC common(m_pDCP05Model);
		if(ulParam2 == eText)
		{
			StringC sText = m_pText->GetString();
			if(sText.IsEmpty())
				;//sprintf(m_pMeasModel->pid_ptr,"%-6-6s","");	
			else
			{
				//GetDataModel()->m_StrText = m_pText->GetString();
				Close(EC_KEY_CONT);
				//sPid = m_pPointId->GetStringInputCtrl()->GetString();
				// convert to ascii
				/*char temp[20];
				UTL::UnicodeToAscii(temp, sPid);
				common.strbtrim(temp);
				sprintf(m_pMeasModel->pid_ptr,"%-6.6s",temp);
				m_pMeasModel->save_point();
				RefreshControls();
				*/
			}
		}
	}
}

void DCP::DCP05InputTextDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP05InputTextDlgC::UpdateData()
{
	GetDataModel()->m_StrText = m_pText->GetString();
}

// Description: refresh all controls
void DCP::DCP05InputTextDlgC::RefreshControls()
{
	if(m_pText)
	{
		m_pText->SetString(GetDataModel()->m_StrText);
	}
}

// Description: only accept hello world Model objects
bool DCP::DCP05InputTextDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05InputTextModelC* pDCP05Model = dynamic_cast< DCP::DCP05InputTextModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP05InputTextModelC* DCP::DCP05InputTextDlgC::GetDataModel() const
{
    return (DCP::DCP05InputTextModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05InputTextControllerC::DCP05InputTextControllerC(DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
  //  SetTitleTok( AT_DCP05, T_DCP_InputTextDLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::DCP05InputTextDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( INPUT_TEXT_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05InputTextControllerC::~DCP05InputTextControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP05InputTextControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// CONT
void DCP::DCP05InputTextControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

	DCP::DCP05InputTextModelC* pModel = (DCP::DCP05InputTextModelC*) GetModel();		
	if(pModel->m_StrText.IsEmpty())
		return;

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}


// Description: React on close of tabbed dialog
void DCP::DCP05InputTextControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	m_pDlg->UpdateData();
	int x;
	x= 1;
}

// Description: React on close of controller
void DCP::DCP05InputTextControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

/*
OBS_IMPLEMENT_EXECUTE(DCP::InputTextC);

DCP::InputTextC::InputTextC():m_pObsInput(OBS_METHOD_TO_PARAM0(InputTextC, OnInputEnterPressed), this),m_pText(0)
{
#ifdef WIN32
#pragma warning (disable:4355)
#endif
}

DCP::InputTextC::~InputTextC()
{

}
void DCP::InputTextC::InputStr()
{
	m_pText = new GUI::EditStringCtrlC();
	//m_pText->SetPosition(20,30); CAPTIVATE
	//m_pText->SetWidth(50); CAPTIVATE
	AddControl(m_pText);
	m_pText->SetFocus();
	m_pObsInput.Attach(m_pText->GetSubject());
	iClose = 1;
	//while(iClose)
	//	;
	
}

void DCP::InputTextC::OnInputEnterPressed( int unNotifyCode,  int ulParam2)
{
	if(GUI::NC_ONEDITMODE_LEFT == unNotifyCode)
	{
		iClose = 0;
	}
}
*/