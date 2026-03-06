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
#include <dcp06/orientation/RotateLine.hpp>
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

DCP::RotateLineDialog::RotateLineDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
			m_pInfo1(0),m_pInfo2(0), m_pAxis1(0),m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::RotateLineDialog::~RotateLineDialog()
{

}

void DCP::RotateLineDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	SetTitle(GetDataModel()->sTitle);	

	// Add fields to dialog
	//m_pInfo1 = new GUI::TextBoxCtrlC(0,0,300,100,StringC(AT_DCP06,M_DCP_ROTATE_LINE_INFO1_TOK)); CAPTIVATE
	m_pInfo1 = new GUI::TextCtrlC();
	m_pInfo1->SetText(StringC(AT_DCP06,M_DCP_ROTATE_LINE_INFO1_TOK));
	m_pInfo1->SetId(eInfo1);
	void(m_pInfo1->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pInfo1->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pInfo1);
	
	
	m_pAxis1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pAxis1->SetId(eAxis1);

	if(GetDataModel()->plane_type == XY_PLANE)
		m_pAxis1->SetText(StringC(AT_DCP06,P_DCP_Z_AXIS_TOK));

	else if(GetDataModel()->plane_type == YZ_PLANE)
		m_pAxis1->SetText(StringC(AT_DCP06,P_DCP_X_AXIS_TOK));

	else if(GetDataModel()->plane_type == ZX_PLANE)
		m_pAxis1->SetText(StringC(AT_DCP06,P_DCP_Y_AXIS_TOK));

	m_pAxis1->GetFloatInputCtrl()->SetDecimalPlaces(6);
	m_pAxis1->GetFloatInputCtrl()->SetRange(-359.999999,359.999999);
	//m_pAxis1->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pAxis1);

	//InsertEmptyLine(); CAPTIVATE

	//m_pInfo2 = new GUI::TextBoxCtrlC(0,0,300,100,StringC(AT_DCP06,M_DCP_ROTATE_LINE_INFO2_TOK)); CAPTIVATE
	m_pInfo2 = new GUI::TextCtrlC();
	m_pInfo2->SetText(StringC(AT_DCP06,M_DCP_ROTATE_LINE_INFO2_TOK));
	m_pInfo2->SetId(eInfo2);
	void(m_pInfo2->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pInfo2->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pInfo2);
	
	//SetHelpTok(H_DCP_ROTATE_LINE_TOK,0);
}	

void DCP::RotateLineDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::RotateLineDialog::UpdateData()
{
	if(GetDataModel()->plane_type == XY_PLANE)
	{
		GetDataModel()->point_buff.z = m_pAxis1->GetFloatInputCtrl()->GetDouble();
	}
	else if(GetDataModel()->plane_type == ZX_PLANE)
	{
		GetDataModel()->point_buff.y = m_pAxis1->GetFloatInputCtrl()->GetDouble();
	}
	else
	{
		GetDataModel()->point_buff.x = m_pAxis1->GetFloatInputCtrl()->GetDouble();
	}
}

// Description: refresh all controls
void DCP::RotateLineDialog::RefreshControls()
{
	if(m_pAxis1)
	{
		if(GetDataModel()->plane_type == XY_PLANE)
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.z);
		}
		else if(GetDataModel()->plane_type == ZX_PLANE)
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.y);
		}
		else
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.x);
		}		
	}
}

// Description: only accept DCP06 Model objects
bool DCP::RotateLineDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::RotateLineModel* pDcpModel = dynamic_cast< DCP::RotateLineModel* >( pModel );

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
DCP::RotateLineModel* DCP::RotateLineDialog::GetDataModel() const
{
    return (DCP::RotateLineModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::RotateLineController::RotateLineController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    // SetTitleTok( AT_DCP06, T_DCP_DOM_ROTATE_LINE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::RotateLineDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( ROTATE_LINE_DLG, m_pDlg, true );

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

DCP::RotateLineController::~RotateLineController()
{

}

// Description: Route model to everybody else
bool DCP::RotateLineController::SetModel( GUI::ModelC* pModel )
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
void DCP::RotateLineController::OnF1Pressed()
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
void DCP::RotateLineController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::RotateLineController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::RotateLineModel::RotateLineModel()
{
}
DCP::RotateLineModel::~RotateLineModel()
{
}