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
#include <dcp06/file/DCP_CircleSave.hpp>
#include <dcp06/core/DCP_DCP06Meas.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/calculation/DCP_CalcLine.hpp>
#include <dcp06/orientation/DCP_ResLine.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/calculation/DCP_CalcLineController.hpp>
#include <dcp06/file/DCP_CircleFile.hpp>

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
// OBS_IMPLEMENT_EXECUTE(DCP::DCP06UnitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::DCP06SaveCircleDlgC::DCP06SaveCircleDlgC(DCP::DCP06CircleModelC* pCircleModel, DCP06SaveCircleModelC* pDataModel):m_pId(0),
		m_pCircleModel(pCircleModel), m_pFile(0), m_pDataModel(pDataModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::DCP06SaveCircleDlgC::~DCP06SaveCircleDlgC()
{

}

void DCP::DCP06SaveCircleDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
    
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	
	// TODO VIVA
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	// InsertEmptyLine(); CAPTIVATE
	// InsertEmptyLine();CAPTIVATE

	m_pId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pId->SetId(eId);
	m_pId->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_ID_TOK));
	// m_pId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	
	AddCtrl(m_pId);
	
	//InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_FILE_TOK));
	//m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT);CAPTIVATE
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pFile);
	
	
	//SetHelpTok(H_DCP_CIRCLE_SAVE_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP06SaveCircleDlgC::OnDialogActivated()
{
	m_pDataModel->m_pFileFunc->setFile(GetDCP06Model()->sCircleFile);
	
	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06SaveCircleDlgC::RefreshControls()
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

StringC DCP::DCP06SaveCircleDlgC::get_id()
{
	StringC sTemp;

	sTemp = m_pId->GetStringInputCtrl()->GetString();
	return sTemp;
}

void DCP::DCP06SaveCircleDlgC::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::DCP06SaveCircleDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

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
DCP::DCP06ModelC* DCP::DCP06SaveCircleDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06SaveCircleControllerC::DCP06SaveCircleControllerC(DCP::DCP06CircleModelC* pCircleModel, DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pCircleModel(pCircleModel),m_pDCP06Model(pDCP06Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC(AT_DCP06, T_DCP_3DSHAFT_ALIGMENT_TOK));

	// create model
	m_pDataModel = new DCP06SaveCircleModelC(pDCP06Model);
    

    // Create a dialog
    m_pDlg = new DCP::DCP06SaveCircleDlgC(pCircleModel, m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CIRCLE_SAVE_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
 //   vDef.nAppId = AT_DCP06;
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


DCP::DCP06SaveCircleControllerC::~DCP06SaveCircleControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP06SaveCircleControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DCP06SaveCircleControllerC::OnF1Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(CIRCLE_FILE_CONTROLLER) == NULL)				
	{
		(void)AddController( CIRCLE_FILE_CONTROLLER, new DCP::DCP06CircleFileControllerC(m_pDCP06Model));
	}
	(void)GetController( CIRCLE_FILE_CONTROLLER )->SetModel( m_pDlg->GetDCP06Model());
	SetActiveController(CIRCLE_FILE_CONTROLLER, true);

	/*
	unsigned int ulCommandId = ABL::CMD_UTIL_FILE_VIEWER_ID;
	(void) ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_UTIL_FILE_VIEWER_ID);
	*/
	
}

void DCP::DCP06SaveCircleControllerC::OnF3Pressed()
{	
    if (m_pDlg == NULL)
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
			if(m_pDataModel->m_pFileFunc->save_circle_to_file(sTemp,m_pCircleModel)==1)
				 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_CIRCLE_SAVED_TOK));
		}
		else
		{
			DCP06MsgBoxC msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_SELECT_CIRCLE_FILE_TOK);
			msgbox.ShowMessageOk(sMsg);
			
		}
	}
	else
	{
			DCP06MsgBoxC msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_ENTER_CIRCLE_ID_TOK);
			msgbox.ShowMessageOk(sMsg);
	}


}
// Description: Handle change of position values
void DCP::DCP06SaveCircleControllerC::OnF6Pressed()
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


// Description: React on close of tabbed dialog
void DCP::DCP06SaveCircleControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{

}

// Description: React on close of controller
void DCP::DCP06SaveCircleControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{

	if(lCtrlID == CIRCLE_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->m_pFileFunc->setFile(m_pDlg->GetDCP06Model()->sCircleFile);
		
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



DCP::DCP06SaveCircleModelC::DCP06SaveCircleModelC(DCP06ModelC* pDCP06Model)
{
	m_pFileFunc = new CircleFileFunc(pDCP06Model);
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06SaveCircleModelC::~DCP06SaveCircleModelC()
{
	if(m_pFileFunc)
	{
		delete m_pFileFunc;
		m_pFileFunc = 0;
	}
}