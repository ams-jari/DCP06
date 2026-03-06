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
#include <dcp06/core/AddTool.hpp>
#include <dcp06/core/Defs.hpp>

#include <GUI_Types.hpp>
#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>
#include <TPI_MeasConfig.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
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


// ================================================================================================
// ====================================  AddToolDialog   =======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::AddToolDialog::AddToolDialog(DCP::Model * pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
					m_pToolId(0), m_pTargetType(0), m_pConstant(0),m_pToolX(0), m_pToolY(0),
					m_pToolZ(0),m_pModel(pModel)


{
	//SetTxtApplicationId(AT_DCP06);
}	


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::AddToolDialog::~AddToolDialog()
{

}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::AddToolDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog

	m_pToolId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolId->SetId(eToolId);
	m_pToolId->SetText(StringC(AT_DCP06,P_DCP_TOOL_ID_TOK));
	m_pToolId->GetStringInputCtrl()->SetCharsCountMax(DCP_TOOL_ID_LENGTH);
	//m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pToolId->SetAutoColon(false);
	//m_pToolId->SetColonPosition(9*20);
	AddCtrl(m_pToolId);

	m_pTargetType = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTargetType->SetId(eTargetType);
	m_pTargetType->SetText(StringC(AT_DCP06,P_DCP_TARGET_TOK));
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pTargetType->SetEmptyAllowed(true);
	//m_pTargetType->SetAutoColon(false);
	//m_pTargetType->SetColonPosition(9*20);
	AddCtrl(m_pTargetType);

	m_pConstant = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pConstant->SetId(eConstant);
	m_pConstant->SetText(StringC(AT_DCP06,P_DCP_CONSTANT_TOK));
	//m_pConstant->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pConstant->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pConstant->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pConstant->SetEmptyAllowed(true);
	//m_pConstant->SetAutoColon(false);
	m_pConstant->GetFloatInputCtrl()->SetDecimalPlaces(1);
	//m_pConstant->SetColonPosition(9*20);
	
	AddCtrl(m_pConstant);

	m_pToolX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolX->SetId(eToolX);
	m_pToolX->SetText(StringC(AT_DCP06,P_DCP_TOOL_X_TOK));
	m_pToolX->GetFloatInputCtrl()->SetDecimalPlaces(m_pModel->m_nDecimals);
	m_pToolX->GetFloatInputCtrl()->SetRange(TOOL_VALUE_MIN,TOOL_VALUE_MAX);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pToolX->SetEmptyAllowed(true);
	//m_pToolX->SetAutoColon(false);
	//m_pToolX->SetColonPosition(9*20);
	
	AddCtrl(m_pToolX);

	m_pToolY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolY->SetId(eToolY);
	m_pToolY->SetText(StringC(AT_DCP06,P_DCP_TOOL_Y_TOK));
	m_pToolY->GetFloatInputCtrl()->SetDecimalPlaces(m_pModel->m_nDecimals);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pToolY->SetEmptyAllowed(true);
	//m_pToolY->SetAutoColon(false);
	//m_pToolY->SetColonPosition(9*20);
	m_pToolY->GetFloatInputCtrl()->SetRange(TOOL_VALUE_MIN,TOOL_VALUE_MAX);
	AddCtrl(m_pToolY);

	m_pToolZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolZ->SetId(eToolZ);
	m_pToolZ->SetText(StringC(AT_DCP06,P_DCP_TOOL_Z_TOK));
	m_pToolZ->GetFloatInputCtrl()->SetDecimalPlaces(m_pModel->m_nDecimals);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pToolZ->SetEmptyAllowed(true);
	//m_pToolZ->SetAutoColon(false);
	//m_pToolZ->SetColonPosition(9*20);
	m_pToolZ->GetFloatInputCtrl()->SetRange(TOOL_VALUE_MIN,TOOL_VALUE_MAX);
	AddCtrl(m_pToolZ);

	//SetHelpTok(H_DCP_ADD_TOOL_TOK,0);
}
// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::AddToolDialog::UpdateData()
{	
	GetDataModel()->tool.constant = m_pConstant->GetFloatInputCtrl()->GetDouble();
	GetDataModel()->tool.x = m_pToolX->GetFloatInputCtrl()->GetDouble();
	GetDataModel()->tool.y = m_pToolY->GetFloatInputCtrl()->GetDouble();
	GetDataModel()->tool.z = m_pToolZ->GetFloatInputCtrl()->GetDouble();

	char szTemp[20+1];
	//UTL::UnicodeToAscii(szTemp, m_pToolId->GetStringInputCtrl()->GetString());
	BSS::UTI::BSS_UTI_WCharToAscii(m_pToolId->GetStringInputCtrl()->GetString(), szTemp);

	sprintf(GetDataModel()->tool.tool_id,"%-10.10s",(char *) szTemp);

}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::AddToolDialog::OnDialogActivated()
{
	RefreshControls();
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::AddToolDialog::RefreshControls()
{
	if( m_pToolId && m_pTargetType && m_pConstant  && m_pToolX && m_pToolY &&  m_pToolZ)
	{
		m_pTargetType->GetStringInputCtrl()->SetString(L" ");


		TPI::MeasConfigC oMeasConfig;
		oMeasConfig.Get();
		float dConst = oMeasConfig.GetReflectorConstant();
		dConst = (float) (dConst * 1000.0);
		m_pConstant->GetFloatInputCtrl()->SetFloat(dConst);
		// TARGET	
		//char temp[100];
		//sprintf_s(temp,20,"%+.1f",fRef*1000.0);
		//m_pConstant->GetStringInputCtrl()->SetString(temp);
	}
}

// ================================================================================================
// Description: SetModel
// ================================================================================================
bool DCP::AddToolDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::AddToolModel* pDcpModel = dynamic_cast< DCP::AddToolModel* >( pModel );

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

// ================================================================================================
// Description: 
// ================================================================================================
DCP::AddToolModel* DCP::AddToolDialog::GetDataModel() const
{
    return (DCP::AddToolModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  AddToolController===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// 
OBS_IMPLEMENT_EXECUTE(DCP::AddToolController);

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::AddToolController::AddToolController(Model *pModel)
    : m_pDlg( nullptr ), m_pModel(pModel),m_pOnApplicationClosedObserver(OBS_METHOD_TO_PARAM0(AddToolController, OnApplicationClosed), this)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_ADD_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::AddToolDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( ADD_TOOL_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_TARGET_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	m_pOnApplicationClosedObserver.Attach(GUI::AppMgntC::Instance()->GetAppCloseSubject());


} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::AddToolController::~AddToolController()
{

}

// ================================================================================================
// Description: SetModel
// ================================================================================================
bool DCP::AddToolController::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: F1
// ================================================================================================
void DCP::AddToolController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	unsigned int ulCommandId = ABL::CMD_MGMT_REFLECTORS_ID;
	ABL::CommandFactoryI* poCommandFactory = 
		ABL::CommandFactoryContainerC::Instance()->GetFactory((unsigned int) ulCommandId);
	
	if(nullptr != poCommandFactory)
	{
		if(poCommandFactory->CanExecute())
			poCommandFactory->Execute();
		
	}
}

// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::AddToolController::OnF6Pressed()
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

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::AddToolController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::AddToolController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: OnApplicationClosed
// ================================================================================================
void DCP::AddToolController::OnApplicationClosed(int unNewApp,  int ulAppOwner)
{	
	// update prism constant value
	//if(unNewApp == 	ABL::CMD_MGMT_REFLECTORS_ID)
		m_pDlg->RefreshControls();
}


// ================================================================================================
// ====================================  AddToolModel =======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::AddToolModel::AddToolModel()
{
	
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::AddToolModel::~AddToolModel()
{
}