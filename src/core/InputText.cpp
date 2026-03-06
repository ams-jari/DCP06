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
#include <dcp06/core/Model.hpp>
#include <dcp06/core/InputTextModel.hpp>
#include <dcp06/core/Common.hpp>
//#include <dcp06/init/Initialization.hpp>
//#include <dcp06/core/Measure.hpp>
//#include <dcp06/core/SpecialMenu.hpp>
//#include <dcp06/core/xyz.hpp>
//#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_KeyTypes.hpp>
#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP06InputTextDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06InputTextDlgC::DCP06InputTextDlgC(DCP06ModelC* pDCP06Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),m_pInfo(0),m_pDCP06Model(pDCP06Model),
	m_pText(0),m_pObserver(OBS_METHOD_TO_PARAM0(DCP06InputTextDlgC, OnValueChanged), this)
{
	//SetTxtApplicationId(AT_DCP06);

}


            // Description: Destructor
DCP::DCP06InputTextDlgC::~DCP06InputTextDlgC()
{

}

void DCP::DCP06InputTextDlgC::OnInitDialog(void)
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

bool DCP::DCP06InputTextDlgC::OnKeyPress  ( GUI::Key  eKey ) 
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

//void DCP::DCP06InputTextDlgC::OnDialogClosed(int  iExitCode )
//{
//	if(iExitCode == EC_KEY_ESC)
//	{
//		Close(EC_KEY_ESC);
//	}
//}

void DCP::DCP06InputTextDlgC::OnValueChanged( int unNotifyCode,  int ulParam2)
{	
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetDataModel()->iCurrentPoint;
		DCP06CommonC common(m_pDCP06Model);
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

void DCP::DCP06InputTextDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP06InputTextDlgC::UpdateData()
{
	GetDataModel()->m_StrText = m_pText->GetString();
}

// Description: refresh all controls
void DCP::DCP06InputTextDlgC::RefreshControls()
{
	if(m_pText)
	{
		m_pText->SetString(GetDataModel()->m_StrText);
	}
}

// Description: only accept hello world Model objects
bool DCP::DCP06InputTextDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06InputTextModelC* pDCP06Model = dynamic_cast< DCP::DCP06InputTextModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP06InputTextModelC* DCP::DCP06InputTextDlgC::GetDataModel() const
{
    return (DCP::DCP06InputTextModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06InputTextControllerC::DCP06InputTextControllerC(DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
  //  SetTitleTok( AT_DCP06, T_DCP_InputTextDLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::DCP06InputTextDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( INPUT_TEXT_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06InputTextControllerC::~DCP06InputTextControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06InputTextControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// CONT
void DCP::DCP06InputTextControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

	DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetModel();		
	if(pModel->m_StrText.IsEmpty())
		return;

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}


// Description: React on close of tabbed dialog
void DCP::DCP06InputTextControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	m_pDlg->UpdateData();
	int x;
	x= 1;
}

// Description: React on close of controller
void DCP::DCP06InputTextControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
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