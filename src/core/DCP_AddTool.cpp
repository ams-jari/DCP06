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
#include <dcp06/init/DCP_DCP05Init.hpp>
#include <dcp06/core/DCP_DCP05Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/core/DCP_AddTool.hpp>
#include <dcp06/core/DCP_Defs.hpp>

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
//OBS_IMPLEMENT_EXECUTE(DCP::DCP05InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// ================================================================================================
// ====================================  DCP05AddToolDlgC   =======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP05AddToolDlgC::DCP05AddToolDlgC(DCP::DCP05ModelC * pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
					m_pToolId(0), m_pTargetType(0), m_pConstant(0),m_pToolX(0), m_pToolY(0),
					m_pToolZ(0),m_pDCP05Model(pDCP05Model)


{
	//SetTxtApplicationId(AT_DCP05);
}	


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05AddToolDlgC::~DCP05AddToolDlgC()
{

}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::DCP05AddToolDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog

	m_pToolId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolId->SetId(eToolId);
	m_pToolId->SetText(StringC(AT_DCP05,P_DCP_TOOL_ID_TOK));
	m_pToolId->GetStringInputCtrl()->SetCharsCountMax(DCP_TOOL_ID_LENGTH);
	//m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pToolId->SetAutoColon(false);
	//m_pToolId->SetColonPosition(9*20);
	AddCtrl(m_pToolId);

	m_pTargetType = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTargetType->SetId(eTargetType);
	m_pTargetType->SetText(StringC(AT_DCP05,P_DCP_TARGET_TOK));
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pTargetType->SetEmptyAllowed(true);
	//m_pTargetType->SetAutoColon(false);
	//m_pTargetType->SetColonPosition(9*20);
	AddCtrl(m_pTargetType);

	m_pConstant = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pConstant->SetId(eConstant);
	m_pConstant->SetText(StringC(AT_DCP05,P_DCP_CONSTANT_TOK));
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
	m_pToolX->SetText(StringC(AT_DCP05,P_DCP_TOOL_X_TOK));
	m_pToolX->GetFloatInputCtrl()->SetDecimalPlaces(m_pDCP05Model->m_nDecimals);
	m_pToolX->GetFloatInputCtrl()->SetRange(TOOL_VALUE_MIN,TOOL_VALUE_MAX);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pToolX->SetEmptyAllowed(true);
	//m_pToolX->SetAutoColon(false);
	//m_pToolX->SetColonPosition(9*20);
	
	AddCtrl(m_pToolX);

	m_pToolY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolY->SetId(eToolY);
	m_pToolY->SetText(StringC(AT_DCP05,P_DCP_TOOL_Y_TOK));
	m_pToolY->GetFloatInputCtrl()->SetDecimalPlaces(m_pDCP05Model->m_nDecimals);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pToolY->SetEmptyAllowed(true);
	//m_pToolY->SetAutoColon(false);
	//m_pToolY->SetColonPosition(9*20);
	m_pToolY->GetFloatInputCtrl()->SetRange(TOOL_VALUE_MIN,TOOL_VALUE_MAX);
	AddCtrl(m_pToolY);

	m_pToolZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolZ->SetId(eToolZ);
	m_pToolZ->SetText(StringC(AT_DCP05,P_DCP_TOOL_Z_TOK));
	m_pToolZ->GetFloatInputCtrl()->SetDecimalPlaces(m_pDCP05Model->m_nDecimals);
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
void DCP::DCP05AddToolDlgC::UpdateData()
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
void DCP::DCP05AddToolDlgC::OnDialogActivated()
{
	RefreshControls();
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::DCP05AddToolDlgC::RefreshControls()
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
bool DCP::DCP05AddToolDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05AddToolModelC* pDCP05Model = dynamic_cast< DCP::DCP05AddToolModelC* >( pModel );

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

// ================================================================================================
// Description: 
// ================================================================================================
DCP::DCP05AddToolModelC* DCP::DCP05AddToolDlgC::GetDataModel() const
{
    return (DCP::DCP05AddToolModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP05AddToolControllerC===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// 
OBS_IMPLEMENT_EXECUTE(DCP::DCP05AddToolControllerC);

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP05AddToolControllerC::DCP05AddToolControllerC(DCP05ModelC *pDCP05Model)
    : m_pDlg( NULL ), m_pDCP05Model(pDCP05Model),m_pOnApplicationClosedObserver(OBS_METHOD_TO_PARAM0(DCP05AddToolControllerC, OnApplicationClosed), this)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_ADD_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05AddToolDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( ADD_TOOL_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_TARGET_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	m_pOnApplicationClosedObserver.Attach(GUI::AppMgntC::Instance()->GetAppCloseSubject());


} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05AddToolControllerC::~DCP05AddToolControllerC()
{

}

// ================================================================================================
// Description: SetModel
// ================================================================================================
bool DCP::DCP05AddToolControllerC::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: F1
// ================================================================================================
void DCP::DCP05AddToolControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	unsigned int ulCommandId = ABL::CMD_MGMT_REFLECTORS_ID;
	ABL::CommandFactoryI* poCommandFactory = 
		ABL::CommandFactoryContainerC::Instance()->GetFactory((unsigned int) ulCommandId);
	
	if(NULL != poCommandFactory)
	{
		if(poCommandFactory->CanExecute())
			poCommandFactory->Execute();
		
	}
}

// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::DCP05AddToolControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
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
void DCP::DCP05AddToolControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP05AddToolControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: OnApplicationClosed
// ================================================================================================
void DCP::DCP05AddToolControllerC::OnApplicationClosed(int unNewApp,  int ulAppOwner)
{	
	// update prism constant value
	//if(unNewApp == 	ABL::CMD_MGMT_REFLECTORS_ID)
		m_pDlg->RefreshControls();
}


// ================================================================================================
// ====================================  DCP05AddToolModelC =======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP05AddToolModelC::DCP05AddToolModelC()
{
	
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05AddToolModelC::~DCP05AddToolModelC()
{
}