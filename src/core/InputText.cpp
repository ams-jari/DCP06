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
OBS_IMPLEMENT_EXECUTE(DCP::InputTextDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::InputTextDialog::InputTextDialog(Model* pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),m_pInfo(0),m_pModel(pModel),
	m_pText(0),m_pObserver(OBS_METHOD_TO_PARAM0(InputTextDialog, OnValueChanged), this)
{
	//SetTxtApplicationId(AT_DCP06);

}


            // Description: Destructor
DCP::InputTextDialog::~InputTextDialog()
{

}

void DCP::InputTextDialog::OnInitDialog(void)
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

bool DCP::InputTextDialog::OnKeyPress  ( GUI::Key  eKey ) 
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

//void DCP::InputTextDialog::OnDialogClosed(int  iExitCode )
//{
//	if(iExitCode == EC_KEY_ESC)
//	{
//		Close(EC_KEY_ESC);
//	}
//}

void DCP::InputTextDialog::OnValueChanged( int unNotifyCode,  int ulParam2)
{	
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetDataModel()->iCurrentPoint;
		Common common(m_pModel);
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
				snprintf(m_pMeasModel->pid_ptr, sizeof(m_pMeasModel->pid_ptr), DCP_POINT_ID_FMT, temp);
				m_pMeasModel->save_point();
				RefreshControls();
				*/
			}
		}
	}
}

void DCP::InputTextDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::InputTextDialog::UpdateData()
{
	GetDataModel()->m_StrText = m_pText->GetString();
}

// Description: refresh all controls
void DCP::InputTextDialog::RefreshControls()
{
	if(m_pText)
	{
		m_pText->SetString(GetDataModel()->m_StrText);
	}
}

// Description: only accept DCP06 Model objects
bool DCP::InputTextDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::InputTextModel* pDcpModel = dynamic_cast< DCP::InputTextModel* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: DCP06 model
DCP::InputTextModel* DCP::InputTextDialog::GetDataModel() const
{
    return (DCP::InputTextModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::InputTextController::InputTextController(Model* pModel)
    : m_pDlg( nullptr )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
  //  SetTitleTok( AT_DCP06, T_DCP_InputTextDLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::InputTextDialog(pModel);  //lint !e1524 new in constructor for class 
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

DCP::InputTextController::~InputTextController()
{

}

// Description: Route model to everybody else
bool DCP::InputTextController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// CONT
void DCP::InputTextController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

	DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetModel();		
	if(pModel->m_StrText.IsEmpty())
		return;

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}


// Description: React on close of tabbed dialog
void DCP::InputTextController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	m_pDlg->UpdateData();
	int x;
	x= 1;
}

// Description: React on close of controller
void DCP::InputTextController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
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