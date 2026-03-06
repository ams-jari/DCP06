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
#include <dcp06/measurement/HiddenPointConf.hpp>
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

DCP::HiddenPointConfDialog::HiddenPointConfDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pDist1(0),m_pDist2(0),m_pDist3(0),m_pDist4(0),m_pDist5(0),m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::HiddenPointConfDialog::~HiddenPointConfDialog()
{

}

void DCP::HiddenPointConfDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	m_pDist1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pDist1->SetId(eDist1);
	m_pDist1->SetText(StringC(AT_DCP06,P_DCP_DIST1_TOK));
	m_pDist1->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pDist1->SetEmptyAllowed(true);
	AddCtrl(m_pDist1);

	m_pDist2 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pDist2->SetId(eDist2);
	m_pDist2->SetText(StringC(AT_DCP06,P_DCP_DIST2_TOK));
	m_pDist2->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pDist2->SetEmptyAllowed(true);
	AddCtrl(m_pDist2);

	m_pDist3 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pDist3->SetId(eDist3);
	m_pDist3->SetText(StringC(AT_DCP06,P_DCP_DIST3_TOK));
	m_pDist3->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pDist3->SetEmptyAllowed(true);
	AddCtrl(m_pDist3);

	m_pDist4 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pDist4->SetId(eDist4);
	m_pDist4->SetText(StringC(AT_DCP06,P_DCP_DIST4_TOK));
	m_pDist4->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pDist4->SetEmptyAllowed(true);
	AddCtrl(m_pDist4);

	m_pDist5 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pDist5->SetId(eDist5);
	m_pDist5->SetText(StringC(AT_DCP06,P_DCP_DIST5_TOK));
	m_pDist5->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pDist5->SetEmptyAllowed(true);
	AddCtrl(m_pDist5);
	
	//SetHelpTok(H_DCP_HIDDENPOINT_CONF_TOK,0);
}

void DCP::HiddenPointConfDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::HiddenPointConfDialog::UpdateData()
{
	// dist1
	if(m_pDist1->GetFloatInputCtrl()->IsEmpty())
		GetModel()->hidden_point_bar[0] = 0.0;
	else
		GetModel()->hidden_point_bar[0] = m_pDist1->GetFloatInputCtrl()->GetDouble();

	// dist2
	if(m_pDist2->GetFloatInputCtrl()->IsEmpty())
		GetModel()->hidden_point_bar[1] = 0.0;
	else
		GetModel()->hidden_point_bar[1] = m_pDist2->GetFloatInputCtrl()->GetDouble();

	// dist3
	if(m_pDist3->GetFloatInputCtrl()->IsEmpty())
		GetModel()->hidden_point_bar[2] = 0.0;
	else
		GetModel()->hidden_point_bar[2] = m_pDist3->GetFloatInputCtrl()->GetDouble();

	// dist4
	if(m_pDist4->GetFloatInputCtrl()->IsEmpty())
		GetModel()->hidden_point_bar[3] = 0.0;
	else
		GetModel()->hidden_point_bar[3] = m_pDist4->GetFloatInputCtrl()->GetDouble();

	// dist5
	if(m_pDist5->GetFloatInputCtrl()->IsEmpty())
		GetModel()->hidden_point_bar[4] = 0.0;
	else
		GetModel()->hidden_point_bar[4] = m_pDist5->GetFloatInputCtrl()->GetDouble();

}

// Description: refresh all controls
void DCP::HiddenPointConfDialog::RefreshControls()
{
	if(m_pDist1 && m_pDist2 && m_pDist3 && m_pDist4 && m_pDist5)
	{	
		// dist 1
		if(GetModel()->hidden_point_bar[0] != 0.0)
			m_pDist1->GetFloatInputCtrl()->SetDouble(GetModel()->hidden_point_bar[0]);
		else
			m_pDist1->GetFloatInputCtrl()->SetEmpty();
		
		// dist2
		if(GetModel()->hidden_point_bar[1] != 0.0)
			m_pDist2->GetFloatInputCtrl()->SetDouble(GetModel()->hidden_point_bar[1]);
		else
			m_pDist2->GetFloatInputCtrl()->SetEmpty();
		
		// dist3
		if(GetModel()->hidden_point_bar[2] != 0.0)
			m_pDist3->GetFloatInputCtrl()->SetDouble(GetModel()->hidden_point_bar[2]);
		else
			m_pDist3->GetFloatInputCtrl()->SetEmpty();
		
		// dist4
		if(GetModel()->hidden_point_bar[3] != 0.0)
			m_pDist4->GetFloatInputCtrl()->SetDouble(GetModel()->hidden_point_bar[3]);
		else
			m_pDist4->GetFloatInputCtrl()->SetEmpty();

		// dist5
		if(GetModel()->hidden_point_bar[4] != 0.0)
			m_pDist5->GetFloatInputCtrl()->SetDouble(GetModel()->hidden_point_bar[4]);
		else
			m_pDist5->GetFloatInputCtrl()->SetEmpty();

	}
}
// Description: only accept hello world Model objects
bool DCP::HiddenPointConfDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::HiddenPointBarConfModel* pModel = dynamic_cast< DCP::HiddenPointBarConfModel* >( pModel );

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
DCP::HiddenPointBarConfModel* DCP::HiddenPointConfDialog::GetModel() const
{
    return (DCP::HiddenPointBarConfModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::HiddenPointConfController::HiddenPointConfController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_HIDDENPOINT_CONF_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::HiddenPointConfDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( HIDDENPOINT_CONF_DLG, m_pDlg, true );

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

DCP::HiddenPointConfController::~HiddenPointConfController()
{

}
// Description: Route model to everybody else
bool DCP::HiddenPointConfController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::Model* pModel = dynamic_cast< DCP::Model* >( pModel );

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

// Description: Handle change of position values
void DCP::HiddenPointConfController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}



// Description: React on close of tabbed dialog
void DCP::HiddenPointConfController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}


// ================================================================================================
// ====================================  HiddenPointBarConfModel  ===================================
// ================================================================================================

DCP::HiddenPointBarConfModel::HiddenPointBarConfModel()
{
	memset(&hidden_point_bar[0],0, sizeof(double) *MAX_POINTS_IN_HIDDENPOINT_BAR);
}

DCP::HiddenPointBarConfModel::~HiddenPointBarConfModel()
{

}