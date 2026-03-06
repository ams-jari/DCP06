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
#include <dcp06/init/Unit.hpp>
#include <dcp06/init/User.hpp>
#include <dcp06/core/Defs.hpp>

#include <ABL_Types.hpp>
#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>
#include <TPI_MeasConfig.hpp>
#include <TPI_Correction.hpp>
#include <SDI_Globals.hpp>
//#include <MAT_Geo.hpp>
#include <OBS_Observer.hpp>
#include <Onboard_Tok.hpp>

#include <TBL_Types.hpp>

#include <TBL_MeasJobInfo.hpp>

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


// Description: Constructor
DCP::InitializationDialog::InitializationDialog(bool disableFileStorage) :// m_pCombo( nullptr ), m_pCombo2(nullptr), m_pComboBox(nullptr),
	//m_pComboBoxObserver(OBS_METHOD_TO_PARAM0(InitializationDialog, OnComboBoxChanged), this),
	m_pTextTarget(0), m_pUser(0), m_pUnit(0),m_pAtmCorr(0),m_p2Face(0),m_pDesignValues(0),
	m_pOverwriteInfo(0), m_ToolInfo(0), m_pAutoIncrement(0), m_pAverageCount(0), m_pLeftRightHand(0),
	m_pAutoMatch(0), m_pAmsLog(0),m_pSaveTool(0), m_pFileStorage(0), m_pDisableFileStorage(disableFileStorage)
{ 
	//SetTxtApplicationId(AT_DCP06);

	m_sMMText.LoadTxt(AT_DCP06,V_DCP_MM_TOK);
	m_sINCHText.LoadTxt(AT_DCP06,V_DCP_INCH_TOK);
	m_sFEETText.LoadTxt(AT_DCP06,V_DCP_FEET_TOK);
}

// ------------------------------------------------------------------------------------------------
//
// On Initialize Dialog
//
void DCP::InitializationDialog::OnInitDialog()
{
	GUI::BaseDialogC::OnInitDialog();
	int textW = 9 * 22;

   	//GUI::SetAutoColon(false);
    //unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	// get PPM
	// float fPPM = SDI::TPSCorrectionDataImplC::GetInstance()->GetAtmoshericPPM();
    
    // Add fields to dialog
	// Target
	m_pTextTarget = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTextTarget->SetId(eTarget);
	m_pTextTarget->SetText(StringC(AT_DCP06,P_DCP_TARGET_TOK));
	void(m_pTextTarget->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pTextTarget->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	m_pTextTarget->GetStringInputCtrl()->Validate();
	m_pTextTarget->GetStringInputCtrl()->SetEmptyAllowed(true);
	m_pTextTarget->GetStringInputCtrl()->SetEmpty();
	//m_pTextTarget->SetAutoColon(false);
	//m_pTextTarget->SetColonPosition(9*18);
	
	//m_pTextTarget->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pTextTarget);
	
	// user
	m_pUser = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUser->SetId(eUser);
	m_pUser->SetText(StringC(AT_DCP06,P_DCP_USER_TOK));
	void(m_pUser->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pUser->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pUser->SetAutoColon(false);
	//m_pUser->SetColonPosition(9*18);
	// m_pUser->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pUser);

	// unit
	m_pUnit = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pUnit->SetId(eUnit);
	m_pUnit->SetText(StringC(AT_DCP06,P_DCP_UNIT_TOK));
	void(m_pUnit->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pUnit->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pUnit->SetAutoColon(false);
	//m_pUnit->SetColonPosition(9*18);
	// m_pUnit->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pUnit);

	// atm.corr
	m_pAtmCorr = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pAtmCorr->SetId(eAtmCorr);
	m_pAtmCorr->SetText(StringC(AT_DCP06,P_DCP_ATM_CORRECTION_TOK));
	void(m_pAtmCorr->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pAtmCorr->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pAtmCorr->SetAutoColon(false);
	//m_pAtmCorr->SetColonPosition(9*18);
	// m_pAtmCorr->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pAtmCorr->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pAtmCorr);
	
	//SetColonPosLong( GUI::StandardDialogC::CP_22);

	// 2Face
	m_p2Face = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_p2Face->SetId(e2Face);
	m_p2Face->SetText(StringC(AT_DCP06,P_DCP_2FACE_MEAS_TOK));
	m_p2Face->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK),			NO);
	m_p2Face->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ALL_TOK),		ALL);
	m_p2Face->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_DIST_TOK),		DIST);
	m_p2Face->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ALL_MAN_TOK), 	ALL_MANUAL);
	m_p2Face->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_DIST_MAN_TOK),	DIST_MANUAL);
	//m_p2Face->SetAutoColon(false);
	//m_p2Face->SetColonPosition(9*22);
	// m_p2Face->SetTextWidth(9*40); CAPTIVATE
	// m_p2Face->SetWidth(30); CAPTIVATE
	//m_p2Face->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	AddCtrl(m_p2Face);


	m_pFileStorage = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pFileStorage->SetId(eFileStorage);
	m_pFileStorage->SetText(StringC(AT_DCP06,P_DCP_FILE_STORAGE_TOK));
	#ifdef CS35
		m_pFileStorage->GetComboBoxInputCtrl()->AddItem(StringC(L"Local memory"),			LOCAL_MEMORY);
	#else
		m_pFileStorage->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SD_CARD_TOK),			SD_CARD);
	#endif

	m_pFileStorage->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_USB_STICK_TOK),		USB_STICK);
	//m_p2Face->SetAutoColon(false);
	//m_p2Face->SetColonPosition(9*22);
	// m_p2Face->SetTextWidth(9*40); CAPTIVATE
	// m_p2Face->SetWidth(30); CAPTIVATE
	//m_p2Face->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	if(m_pDisableFileStorage)
	{
		m_pFileStorage->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	}
	AddCtrl(m_pFileStorage);
	// 
	m_pDesignValues = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pDesignValues->SetId(eDesignValues);
	m_pDesignValues->SetText(StringC(AT_DCP06,P_DCP_DESIGN_VALUES_IN_USE_TOK));
	m_pDesignValues->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_pDesignValues->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK),	 YES);
	//m_pDesignValues->SetAutoColon(false);
	//m_pDesignValues->SetColonPosition(9*22);

	// m_pDesignValues->SetTextWidth(9*40); CAPTIVATE
	//m_pDesignValues->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	//m_pDesignValues->SetWidth(30);
	AddCtrl(m_pDesignValues);

	m_pOverwriteInfo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pOverwriteInfo->SetId(eOverWrite);
	m_pOverwriteInfo->SetText(StringC(AT_DCP06,P_DCP_OVERWRITE_INFO_TOK));
	m_pOverwriteInfo->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_pOverwriteInfo->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK), YES);
	//m_pOverwriteInfo->SetAutoColon(false);
	//m_pOverwriteInfo->SetColonPosition(9*22);
	//m_pOverwriteInfo->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	// m_pOverwriteInfo->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pOverwriteInfo);

	m_ToolInfo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_ToolInfo->SetId(eToolInfo);
	m_ToolInfo->SetText(StringC(AT_DCP06,P_DCP_TOOL_INFO_TOK));
	m_ToolInfo->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_ToolInfo->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK), YES);
	//m_ToolInfo->SetAutoColon(false);
	//m_ToolInfo->SetColonPosition(9*22);
	//m_ToolInfo->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	// m_ToolInfo->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_ToolInfo);

	m_pAutoIncrement = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pAutoIncrement->SetId(eAutoIncrement);
	m_pAutoIncrement->SetText(StringC(AT_DCP06,P_DCP_AUTO_INCREMENT_TOK));
	m_pAutoIncrement->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK),	 NO);
	m_pAutoIncrement->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK),	 YES);
	//m_pAutoIncrement->SetAutoColon(false);
	//m_pAutoIncrement->SetColonPosition(9*22);
	//m_pAutoIncrement->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	// m_pAutoIncrement->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pAutoIncrement);

	m_pAverageCount = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pAverageCount->SetId(eAverageCount);
	m_pAverageCount->SetText(StringC(AT_DCP06,P_DCP_AVERAGE_COUNT_TOK));
	m_pAverageCount->GetComboBoxInputCtrl()->AddItem(L"1",1);
	m_pAverageCount->GetComboBoxInputCtrl()->AddItem(L"2",2);
	m_pAverageCount->GetComboBoxInputCtrl()->AddItem(L"3",3);
	m_pAverageCount->GetComboBoxInputCtrl()->AddItem(L"4",4);
	m_pAverageCount->GetComboBoxInputCtrl()->AddItem(L"5",5);
	//m_pAverageCount->SetAutoColon(false);
	//m_pAverageCount->SetColonPosition(9*22);
	//m_pAverageCount->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	//m_pAverageCount->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pAverageCount);

	m_pLeftRightHand = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pLeftRightHand->SetId(eLeftRightHand);
	m_pLeftRightHand->SetText(StringC(AT_DCP06,P_DCP_LEFT_RIGHT_HAND_TOK));
	m_pLeftRightHand->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_LEFTHAND_TOK), LEFTHAND);
	m_pLeftRightHand->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_RIGHTHAND_TOK), RIGHTHAND);
	//m_pLeftRightHand->SetAutoColon(false);
	//m_pLeftRightHand->SetColonPosition(9*22);
	//m_pLeftRightHand->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	//m_pLeftRightHand->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pLeftRightHand);

	m_pAutoMatch = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pAutoMatch->SetId(eAutoMatch);
	m_pAutoMatch->SetText(StringC(AT_DCP06,P_DCP_AUTOMATCH_TOK));
	m_pAutoMatch->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_pAutoMatch->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK), YES);
	//m_pAutoMatch->SetAutoColon(false);
	//m_pAutoMatch->SetColonPosition(9*22);
	//m_pAutoMatch->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	//m_pAutoMatch->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pAutoMatch);
	
	m_pAmsLog = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pAmsLog->SetId(eAmsLog);
	m_pAmsLog->SetText(StringC(AT_DCP06,P_DCP_AMS_LOG_TOK));
	m_pAmsLog->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_pAmsLog->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK), YES);
	//m_pAmsLog->SetAutoColon(false);
	//m_pAmsLog->SetColonPosition(9*22);
	//m_pAutoMatch->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	//m_pAmsLog->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pAmsLog);

	m_pSaveTool = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pSaveTool->SetId(eSaveTool);
	m_pSaveTool->SetText(StringC(AT_DCP06,P_DCP_SAVE_TOOL_TOK));
	m_pSaveTool->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_NO_TOK), NO);
	m_pSaveTool->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_YES_TOK), YES);
	//m_pSaveTool->SetAutoColon(false);
	//m_pSaveTool->SetColonPosition(9*22);
	//m_pSaveTool->GetComboBoxInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	// m_pSaveTool->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pSaveTool);

	//SetHelpTok(H_DCP_INIT_TOK,0);

	// m_pComboBoxObserver.Attach(m_pComboBox->GetSubject());
}

// Description: only accept DCP06 Model objects
bool DCP::InitializationDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

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

// Description: refresh all controls
void DCP::InitializationDialog::RefreshControls()
{
	if (m_pTextTarget && m_pUser && m_pUnit && m_pAtmCorr && m_p2Face && m_pDesignValues && 
		m_pOverwriteInfo && m_ToolInfo && m_pAutoIncrement &&   m_pAverageCount && 
		m_pLeftRightHand && m_pAutoMatch && m_pAmsLog && m_pSaveTool && m_pFileStorage)
	{
		TPI::MeasConfigC oMeasConfig;
		oMeasConfig.Get();
		float fRef = oMeasConfig.GetReflectorConstant();
		
		TPI::CorrectionC oCorrectionData;
		oCorrectionData.Get();
		float fPPM = oCorrectionData.GetAtmoshericPPM();

		// TARGET	
		char temp[100];
		sprintf(temp,"%+.1f",fRef*1000.0);
		m_pTextTarget->GetStringInputCtrl()->SetString(temp);

		// USER	
		if(!GetModel()->m_sUser.IsEmpty())
			m_pUser->GetStringInputCtrl()->SetString(GetModel()->m_sUser);
		else
			m_pUser->GetStringInputCtrl()->SetString(L" ");
	
		// UNIT
		StringC sTemp;
		StringC sUnit;
		StringC sDec;

		if(GetModel()->m_nUnit == MM)
			sUnit = m_sMMText;

		else if(GetModel()->m_nUnit == FEET)
			sUnit = m_sFEETText;

		else
			sUnit = m_sINCHText;

		short iDec = GetModel()->m_nDecimals;
		
		if(iDec == 1)
			sDec = "0.1";

		else if(iDec == 2)
			sDec = "0.01";

		else if(iDec == 3)
			sDec = "0.001";

		else if(iDec == 4)
			sDec = "0.0001";

		sTemp = sDec;
		sTemp += " ";
		sTemp += sUnit;
		m_pUnit->GetStringInputCtrl()->SetString(sTemp);

		// PPM
		sTemp.Format(L"%5.4f",fPPM);
		m_pAtmCorr->GetStringInputCtrl()->SetString(sTemp);

		m_p2Face->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_n2FaceMeas);
		m_pDesignValues->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nDesignValues);
		m_pOverwriteInfo->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nOverWriteInfo);
		m_ToolInfo->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nToolInfo);
		m_pAutoIncrement->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nAutoIncrement);
		m_pAverageCount->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nAverageCount);
		m_pLeftRightHand->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nLeftRightHand);
		m_pAutoMatch->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nAutoMatch);
		m_pAmsLog->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nAmsLog);
		m_pSaveTool->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->m_nSaveTool);

#ifdef CS35
		m_pFileStorage->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->FILE_STORAGE1 == CPI::deviceLocalMemory ? LOCAL_MEMORY: USB_STICK );
#else
		m_pFileStorage->GetComboBoxInputCtrl()->SetSelectedId(GetModel()->FILE_STORAGE1 == CPI::deviceSdCard ? SD_CARD: USB_STICK );
#endif

	}
}

// Description: update the road model with the new values
void DCP::InitializationDialog::UpdateData()
{
	//m_pTextTarget(0), 
	GetModel()->m_sUser = m_pUser->GetStringInputCtrl()->GetString();
    //m_pUnit(0),
	//m_pAtmCorr(0),
	GetModel()->m_n2FaceMeas		= m_p2Face->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nDesignValues	= m_pDesignValues->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nOverWriteInfo	= m_pOverwriteInfo->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nToolInfo		= m_ToolInfo->GetComboBoxInputCtrl()->GetSelectedId();
    GetModel()->m_nAutoIncrement	= m_pAutoIncrement->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nAverageCount	= m_pAverageCount->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nLeftRightHand	= m_pLeftRightHand->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nAutoMatch		= m_pAutoMatch->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nAmsLog			= m_pAmsLog->GetComboBoxInputCtrl()->GetSelectedId();
	GetModel()->m_nSaveTool		= m_pSaveTool->GetComboBoxInputCtrl()->GetSelectedId();

#ifdef CS35
	GetModel()->FILE_STORAGE1		= m_pFileStorage->GetComboBoxInputCtrl()->GetSelectedId() == LOCAL_MEMORY ? CPI::deviceLocalMemory : CPI::deviceUSB;
#else
	GetModel()->FILE_STORAGE1		= m_pFileStorage->GetComboBoxInputCtrl()->GetSelectedId() == SD_CARD ? CPI::deviceSdCard : CPI::deviceUSB;
#endif

	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
	GetModel()->poConfigController->StoreConfigData();

}

// Description: DCP06 model
DCP::Model* DCP::InitializationDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


//void DCP::InitializationDialog::OnComboBoxChanged(int unNotifyCode, int ulParam2)
//{
//	if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
//	{
//
//
//	}
//	if(unNotifyCode== GUI::NC_ONEDITMODE_LEFT)
//	{
//	}
//}

OBS_IMPLEMENT_EXECUTE(DCP::InitializationController);
//-------------------------------------------------------------------------------------------------
// InitializationController
// 
DCP::InitializationController::InitializationController(bool disableFileStorage)
    : m_pDlg( nullptr ),m_pOnApplicationClosedObserver(OBS_METHOD_TO_PARAM0(InitializationController, OnApplicationClosed), this)
{
	int ii = GetTxtApplicationId(); 
	SetTxtApplicationId(AT_DCP06);
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_INIT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::InitializationDialog(disableFileStorage);  //lint !e1524 new in constructor for class 
    (void)AddDialog( INIT_DLG, m_pDlg, true );
	
    // Set the function key

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	vDef.poOwner = this;
	SetFunctionKey(FK1, vDef);

	vDef.strLable = StringC(AT_DCP06,K_DCP_TARGET_TOK);
	vDef.poOwner = this;
	SetFunctionKey(FK2, vDef);

	vDef.strLable = StringC(AT_DCP06,K_DCP_USER_TOK);
	vDef.poOwner = this;
	SetFunctionKey(FK3, vDef);

	vDef.strLable = StringC(AT_DCP06,K_DCP_UNIT_TOK);
	vDef.poOwner = this;
	SetFunctionKey(FK4, vDef);
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_PPM_TOK);
	vDef.poOwner = this;
	SetFunctionKey(FK5, vDef);

	// DISABLE SHFK6	
	FKDef vDef1;
	vDef1.strLable = L" ";
    vDef1.poOwner = this;
	SetFunctionKey( SHFK6, vDef1 );


	m_pOnApplicationClosedObserver.Attach(GUI::AppMgntC::Instance()->GetAppCloseSubject());

} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::InitializationController::~InitializationController()
{

}

// Description: Route model to everybody else
bool DCP::InitializationController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

// Description: Handle change of position values
void DCP::InitializationController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
      // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

// TARGET
void DCP::InitializationController::OnF2Pressed()
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

// Description: Handle change of position values
void DCP::InitializationController::OnF3Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(INIT_USER_CONTROLLER) == nullptr)
	{
		(void)AddController( INIT_USER_CONTROLLER, new DCP::UserController );
	}
	(void)GetController( INIT_USER_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(INIT_USER_CONTROLLER, true);

}

void DCP::InitializationController::OnF4Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(INIT_UNIT_CONTROLLER) == nullptr)
	{
		(void)AddController( INIT_UNIT_CONTROLLER, new DCP::UnitController );
	}
	(void)GetController( INIT_UNIT_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(INIT_UNIT_CONTROLLER, true);
}


// PPM
void DCP::InitializationController::OnF5Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
	}
	ABL::CTL_ID_ASCII_INPUT;
	unsigned int ulControllerId = ABL::CTL_ID_PPM_REFRACT_CORR;
	GUI::ControllerC* poController = 
		ABL::ControllerFactoryContainerC::Instance()->CreateController((unsigned short) ulControllerId); 
	AddController((int)ulControllerId,poController); 
	(void)SetActiveController( ulControllerId, true );
}

// Description: React on close of tabbed dialog
void DCP::InitializationController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::InitializationController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == ABL::CTL_ID_PPM_REFRACT_CORR)
	{
				
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

void DCP::InitializationController::OnApplicationClosed( int unNewApp,  int ulAppOwner)
{
	if(unNewApp == 	ABL::CMD_MGMT_REFLECTORS_ID)
		m_pDlg->RefreshControls();
}