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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/init/User.hpp>
#include <dcp06/core/Defs.hpp>

#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::InitializationDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::UserDialog::UserDialog():GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pUser1(0),m_pUser2(0),m_pUser3(0),m_pUser4(0),m_pUser5(0),m_pUser6(0),m_pUser7(0),
	m_pUser8(0),m_pUser9(0),m_pUser10(0)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::UserDialog::~UserDialog()
{

}

void DCP::UserDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	m_pUser1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser1->SetId(eUser1);
	m_pUser1->SetText(L"1");
	m_pUser1->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser1->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser1);

	m_pUser2 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser2->SetId(eUser2);
	m_pUser2->SetText(L"2");
	m_pUser2->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser2->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser2);

	m_pUser3 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser3->SetId(eUser3);
	m_pUser3->SetText(L"3");
	m_pUser3->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser3->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser3);

	m_pUser4 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser4->SetId(eUser4);
	m_pUser4->SetText(L"4");
	m_pUser4->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser4->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser4);

	m_pUser5 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser5->SetId(eUser5);
	m_pUser5->SetText(L"5");
	m_pUser5->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser5->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser5);

	m_pUser6 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser6->SetId(eUser6);
	m_pUser6->SetText(L"6");
	m_pUser6->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser6->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser6);

	m_pUser7 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser7->SetId(eUser7);
	m_pUser7->SetText(L"7");
	m_pUser7->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser7->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser7);

	m_pUser8 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser8->SetId(eUser8);
	m_pUser8->SetText(L"8");
	m_pUser8->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser8->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser8);

	m_pUser9 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser9->SetId(eUser9);
	m_pUser9->SetText(L"9");
	m_pUser9->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser9->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser9);

	m_pUser10 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser10->SetId(eUser10);
	m_pUser10->SetText(L"10");
	m_pUser10->GetStringInputCtrl()->SetCharsCountMax(DCP_USER_LENGTH);
	m_pUser10->GetStringInputCtrl()->SetEmptyAllowed(true);
	AddCtrl(m_pUser10);

	//SetHelpTok(H_DCP_USER_TOK,0);
}

void DCP::UserDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::UserDialog::UpdateData()
{
	GetModel()->m_sUser1	= m_pUser1->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser2	= m_pUser2->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser3	= m_pUser3->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser4	= m_pUser4->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser5	= m_pUser5->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser6	= m_pUser6->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser7	= m_pUser7->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser8	= m_pUser8->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser9	= m_pUser9->GetStringInputCtrl()->GetString();
	GetModel()->m_sUser10	= m_pUser10->GetStringInputCtrl()->GetString();

	// And selected user
	if(m_pUser1->HasFocus()) GetModel()->m_sUser = m_pUser1->GetStringInputCtrl()->GetString();
	else if(m_pUser2->HasFocus()) GetModel()->m_sUser = m_pUser2->GetStringInputCtrl()->GetString();
	else if(m_pUser3->HasFocus()) GetModel()->m_sUser = m_pUser3->GetStringInputCtrl()->GetString();
	else if(m_pUser4->HasFocus()) GetModel()->m_sUser = m_pUser4->GetStringInputCtrl()->GetString();
	else if(m_pUser5->HasFocus()) GetModel()->m_sUser = m_pUser5->GetStringInputCtrl()->GetString();
	else if(m_pUser6->HasFocus()) GetModel()->m_sUser = m_pUser6->GetStringInputCtrl()->GetString();
	else if(m_pUser7->HasFocus()) GetModel()->m_sUser = m_pUser7->GetStringInputCtrl()->GetString();
	else if(m_pUser8->HasFocus()) GetModel()->m_sUser = m_pUser8->GetStringInputCtrl()->GetString();
	else if(m_pUser9->HasFocus()) GetModel()->m_sUser = m_pUser9->GetStringInputCtrl()->GetString();
	else if(m_pUser10->HasFocus()) GetModel()->m_sUser = m_pUser10->GetStringInputCtrl()->GetString();
	else GetModel()->m_sUser = L"";	


}

// Description: refresh all controls
void DCP::UserDialog::RefreshControls()
{
	if(m_pUser1 && m_pUser2 && m_pUser3 && m_pUser4 && m_pUser5 && m_pUser6 && m_pUser7 &&
		m_pUser8 && m_pUser9 && m_pUser10)
	{
		StringC sTemp= GetModel()->m_sUser1;
		sTemp.RTrim();
		m_pUser1->GetStringInputCtrl()->SetString( sTemp);
		m_pUser2->GetStringInputCtrl()->SetString( GetModel()->m_sUser2);
		m_pUser3->GetStringInputCtrl()->SetString( GetModel()->m_sUser3);
		m_pUser4->GetStringInputCtrl()->SetString( GetModel()->m_sUser4);
		m_pUser5->GetStringInputCtrl()->SetString( GetModel()->m_sUser5);
		m_pUser6->GetStringInputCtrl()->SetString( GetModel()->m_sUser6);
		m_pUser7->GetStringInputCtrl()->SetString( GetModel()->m_sUser7);
		m_pUser8->GetStringInputCtrl()->SetString( GetModel()->m_sUser8);
		m_pUser9->GetStringInputCtrl()->SetString( GetModel()->m_sUser9);
		m_pUser10->GetStringInputCtrl()->SetString( GetModel()->m_sUser10);
	}
}
// Description: only accept hello world Model objects
bool DCP::UserDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::Model* DCP::UserDialog::GetModel() const
{
    return (DCP::Model*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::UserController::UserController()
    : m_pDlg( nullptr )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_USER_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::UserDialog;  //lint !e1524 new in constructor for class 
    (void)AddDialog( USER_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
    vDef.poOwner = this;
	  
	SetFunctionKey( FK1, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::UserController::~UserController()
{
}

// Description: Route model to everybody else
bool DCP::UserController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
}

// Description: Handle change of position values
void DCP::UserController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    // m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}



// Description: React on close of tabbed dialog
void DCP::UserController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
