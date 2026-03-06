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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/application/LineSettingConf.hpp>
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

DCP::LineSettingConfDialog::LineSettingConfDialog(DCP::Model* pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
		m_pModel(pModel), m_pInfo(0), m_pHor(0), m_pVer(0)	
			//,m_pPointNo(0),m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::LineSettingConfDialog::~LineSettingConfDialog()
{

}

void DCP::LineSettingConfDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// info text control
	//m_pInfo = new GUI::TextBoxCtrlC(0,0,300,100,StringC(AT_DCP06,M_DCP_LSET_ANGLE_INFO_TOK)); CAPTIVATE
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetText(StringC(AT_DCP06,M_DCP_LSET_ANGLE_INFO_TOK));
	m_pInfo->SetId(eInfo);
	void(m_pInfo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pInfo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pInfo);

	// InsertEmptyLine(); CAPTIVATE
	//SetColonPosLong( GUI::StandardDialogC::CP_21);

	// horizontal angle 
	m_pHor = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pHor->SetId(eHor);
	m_pHor->SetText(StringC(AT_DCP06,P_DCP_LSET_HORIZONTAL_TOK));
	m_pHor->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)4);
	//m_pHor->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_pHor->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pHor);

	// vertical angle
	m_pVer = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pVer->SetId(eVer);
	m_pVer->SetText(StringC(AT_DCP06,P_DCP_LSET_VERTICAL_TOK));
	m_pVer->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)4);
	//m_pVer->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_pVer->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pVer);

	//SetHelpTok(H_DCP_LINE_SETTING_CONF_TOK,0);
}

void DCP::LineSettingConfDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::LineSettingConfDialog::UpdateData()
{
	GetDataModel()->dHor = m_pHor->GetFloatInputCtrl()->GetDouble();
	GetDataModel()->dVer = m_pVer->GetFloatInputCtrl()->GetDouble();
}

// Description: refresh all controls
void DCP::LineSettingConfDialog::RefreshControls()
{
	if(m_pHor && m_pVer)
	{
		m_pHor->GetFloatInputCtrl()->SetDouble(GetDataModel()->dHor);
		m_pVer->GetFloatInputCtrl()->SetDouble(GetDataModel()->dVer);
	}
}
// Description: only accept DCP06 Model objects
bool DCP::LineSettingConfDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::LineSettingConfModel* pDcpModel = dynamic_cast< DCP::LineSettingConfModel* >( pModel );

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
DCP::LineSettingConfModel* DCP::LineSettingConfDialog::GetDataModel() const
{
    return (DCP::LineSettingConfModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::LineSettingConfController::LineSettingConfController(DCP::Model *pModel)
    : m_pDlg( nullptr )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_LINE_SETTING_CONF_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
	 m_pDlg = new DCP::LineSettingConfDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( LINE_SETTING_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_0_DEC_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_90_DEC_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_180_DEC_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_270_DEC_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::LineSettingConfController::~LineSettingConfController()
{

}

// Description: Route model to everybody else
bool DCP::LineSettingConfController::SetModel( GUI::ModelC* pModel )
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

// 0
void DCP::LineSettingConfController::OnF1Pressed()
{
	m_pDlg->GetDataModel()->dHor = 0.0;
	m_pDlg->RefreshControls();

}

// 90
void DCP::LineSettingConfController::OnF2Pressed()
{
	m_pDlg->GetDataModel()->dHor = 90.0;
	m_pDlg->RefreshControls();
}

// 180
void DCP::LineSettingConfController::OnF3Pressed()
{
	m_pDlg->GetDataModel()->dHor = 180.0;
	m_pDlg->RefreshControls();
}

// 270
void DCP::LineSettingConfController::OnF4Pressed()
{
	m_pDlg->GetDataModel()->dHor = 270.0;
	m_pDlg->RefreshControls();
}

// CONT
void DCP::LineSettingConfController::OnF6Pressed()
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
void DCP::LineSettingConfController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::LineSettingConfController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// LineSettingModel
// ===========================================================================================
DCP::LineSettingConfModel::LineSettingConfModel()
{
	dHor = 0.0;
	dVer = 0.0;
	
}
DCP::LineSettingConfModel::~LineSettingConfModel()
{
}