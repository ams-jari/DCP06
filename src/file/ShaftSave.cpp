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
#include <dcp06/file/ShaftSave.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/file/ShaftFile.hpp>

#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>

#include <GUI_DeskTop.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
// OBS_IMPLEMENT_EXECUTE(DCP::UnitDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::SaveShaftDialog::SaveShaftDialog(DCP::ShaftModel* pShaftModel, SaveShaftModel* pDataModel):m_pId(0),
		m_pShaftModel(pShaftModel), m_pFile(0), m_pDataModel(pDataModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::SaveShaftDialog::~SaveShaftDialog()
{

}

void DCP::SaveShaftDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
    
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	//InsertEmptyLine(); CAPTIVATE
	//InsertEmptyLine(); CAPTIVATE

	m_pId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pId->SetId(eId);
	m_pId->SetText(StringC(AT_DCP06,P_DCP_SHAFT_ID_TOK));
	//m_pId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	
	AddCtrl(m_pId);
	
	//InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_FILE_TOK));
	//m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pFile);
	
	
	//SetHelpTok(H_DCP_SHAFT_SAVE_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::SaveShaftDialog::OnDialogActivated()
{
	m_pDataModel->m_pFileFunc->setFile(GetModel()->sShaftFile);
	
	RefreshControls();
}

// Description: refresh all controls
void DCP::SaveShaftDialog::RefreshControls()
{	
	if(m_pId && m_pFile)	
	{
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(StringC(m_pDataModel->m_pFileFunc->getFileName()));
		}
		else
			m_pFile->GetStringInputCtrl()->SetString(L"");
	}
}

StringC DCP::SaveShaftDialog::get_id()
{
	StringC sTemp;

	sTemp = m_pId->GetStringInputCtrl()->GetString();
	return sTemp;
}

void DCP::SaveShaftDialog::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::SaveShaftDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::SaveShaftDialog::GetModel() const
{
    return (DCP::Model*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::SaveShaftController::SaveShaftController(DCP::ShaftModel* pShaftModel, Model* pModel)
    : m_pDlg( nullptr ),m_pShaftModel(pShaftModel),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_3DSHAFT_ALIGMENT_TOK));

	// create model
	m_pDataModel = new SaveShaftModel(pModel);
    

    // Create a dialog
    m_pDlg = new DCP::SaveShaftDialog(pShaftModel, m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SHAFT_SAVE_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
    //vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_FILE_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_SAVE_TOK);
	SetFunctionKey( FK3, vDef );


    vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
    SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::SaveShaftController::~SaveShaftController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// Description: Route model to everybody else
bool DCP::SaveShaftController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pModel = dynamic_cast< DCP::Model* >( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::SaveShaftController::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(SHAFT_FILE_CONTROLLER) == nullptr)				
	{
		(void)AddController( SHAFT_FILE_CONTROLLER, new DCP::ShaftFileController(m_pDlg->GetModel()));
	}
	(void)GetController( SHAFT_FILE_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(SHAFT_FILE_CONTROLLER, true);

	/*
	unsigned int ulCommandId = ABL::CMD_UTIL_FILE_VIEWER_ID;
	(void) ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_UTIL_FILE_VIEWER_ID);
	*/
	
}

void DCP::SaveShaftController::OnF3Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	StringC sTemp;
	sTemp = m_pDlg->get_id();
	if(!sTemp.IsEmpty())
	{
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			if(m_pDataModel->m_pFileFunc->save_shaft_to_file(sTemp,m_pShaftModel)==1)
				 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06, I_DCP_SHAFT_SAVED_TOK ));
		}
		else
		{
			MsgBox msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_SELECT_SHAFT_FILE_TOK);
			msgbox.ShowMessageOk(sMsg);
			
		}
	}
	else
	{
			MsgBox msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_ENTER_SHAFT_ID_TOK);
			msgbox.ShowMessageOk(sMsg);
	}


}
// Description: Handle change of position values
void DCP::SaveShaftController::OnF6Pressed()
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
void DCP::SaveShaftController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{

}

// Description: React on close of controller
void DCP::SaveShaftController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{

	if(lCtrlID == SHAFT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->m_pFileFunc->setFile(m_pDlg->GetModel()->sShaftFile);
		
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



DCP::SaveShaftModel::SaveShaftModel(Model* pModel)
{
	m_pFileFunc = new ShaftFileFunc(pModel);
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::SaveShaftModel::~SaveShaftModel()
{
	if(m_pFileFunc)
	{
		delete m_pFileFunc;
		m_pFileFunc = 0;
	}
}