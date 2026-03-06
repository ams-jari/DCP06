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
#include <dcp06/orientation/RotatePlane.hpp>
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

DCP::RotatePlaneDialog::RotatePlaneDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
			m_pInfo1(0),m_pInfo2(0), m_pAxis1(0),m_pAxis2(0),m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::RotatePlaneDialog::~RotatePlaneDialog()
{

}

void DCP::RotatePlaneDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	SetTitle(GetDataModel()->sTitle);	
	// Add fields to dialog
	
	//m_pInfo1 = new GUI::TextBoxCtrlC(0,0,300,100,StringC(AT_DCP06,M_DCP_ROTATE_PLANE_INFO1_TOK)); CAPTIVATE
	m_pInfo1 = new GUI::TextCtrlC();
	m_pInfo1->SetText(StringC(AT_DCP06,M_DCP_ROTATE_PLANE_INFO1_TOK));
	m_pInfo1->SetId(eInfo1);
	void(m_pInfo1->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pInfo1->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pInfo1);
	
	
	m_pAxis1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pAxis1->SetId(eAxis1);
	m_pAxis1->SetText(StringC(AT_DCP06,P_DCP_X_AXIS_TOK));
	m_pAxis1->GetFloatInputCtrl()->SetDecimalPlaces(6);
	m_pAxis1->GetFloatInputCtrl()->SetRange(-359.999999,359.999999);
	//m_pAxis1->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pAxis1);

	m_pAxis2 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pAxis2->SetId(eAxis2);
	m_pAxis2->SetText(StringC(AT_DCP06,P_DCP_Y_AXIS_TOK));
	m_pAxis2->GetFloatInputCtrl()->SetRange(-359.999999,359.999999);
	m_pAxis2->GetFloatInputCtrl()->SetDecimalPlaces(6);
	//m_pAxis2->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pAxis2);
	
	//InsertEmptyLine(); CAPTIVATE

	//m_pInfo2 = new GUI::TextBoxCtrlC(0,0,300,100,StringC(AT_DCP06,M_DCP_ROTATE_PLANE_INFO2_TOK)); CAPTIVATE
	m_pInfo2 = new GUI::TextCtrlC();
	m_pInfo2->SetText(StringC(AT_DCP06,M_DCP_ROTATE_PLANE_INFO2_TOK));
	m_pInfo2->SetId(eInfo2);
	void(m_pInfo2->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pInfo2->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pInfo2);
	
	//SetHelpTok(H_DCP_ROTATE_PLANE_TOK,0);
}

void DCP::RotatePlaneDialog::OnDialogActivated()
{
	m_pAxis1->GetFloatInputCtrl()->SetFloat(0.0);
	m_pAxis2->GetFloatInputCtrl()->SetFloat(0.0);

	RefreshControls();
}

void DCP::RotatePlaneDialog::UpdateData()
{
	if(GetDataModel()->plane_type == XY_PLANE)
	{
		GetDataModel()->point_buff.x = m_pAxis1->GetFloatInputCtrl()->GetDouble();
		GetDataModel()->point_buff.y = m_pAxis2->GetFloatInputCtrl()->GetDouble();

	}
	else if(GetDataModel()->plane_type == ZX_PLANE)
	{
		GetDataModel()->point_buff.x = m_pAxis1->GetFloatInputCtrl()->GetDouble();
		GetDataModel()->point_buff.z = m_pAxis2->GetFloatInputCtrl()->GetDouble();
	}
	else
	{
		GetDataModel()->point_buff.y = m_pAxis1->GetFloatInputCtrl()->GetDouble();
		GetDataModel()->point_buff.z = m_pAxis2->GetFloatInputCtrl()->GetDouble();
	}
}

// Description: refresh all controls
void DCP::RotatePlaneDialog::RefreshControls()
{
	if(m_pAxis1 && m_pAxis2)
	{
		if(GetDataModel()->plane_type == XY_PLANE)
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.x);
			m_pAxis2->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.y);

		}
		else if(GetDataModel()->plane_type == ZX_PLANE)
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.x);
			m_pAxis2->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.z);
		}
		else
		{
			m_pAxis1->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.y);
			m_pAxis2->GetFloatInputCtrl()->SetDouble(GetDataModel()->point_buff.z);
		}		
	}
	
}
// Description: only accept DCP06 Model objects
bool DCP::RotatePlaneDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::RotatePlaneModel* pDcpModel = dynamic_cast< DCP::RotatePlaneModel* >( pModel );

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
DCP::RotatePlaneModel* DCP::RotatePlaneDialog::GetDataModel() const
{
    return (DCP::RotatePlaneModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::RotatePlaneController::RotatePlaneController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
   // SetTitleTok( AT_DCP06, T_DCP_ROTATE_PLANE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::RotatePlaneDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( ROTATE_PLANE_DLG, m_pDlg, true );

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

DCP::RotatePlaneController::~RotatePlaneController()
{

}

// Description: Route model to everybody else
bool DCP::RotatePlaneController::SetModel( GUI::ModelC* pModel )
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
void DCP::RotatePlaneController::OnF1Pressed()
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
void DCP::RotatePlaneController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::RotatePlaneController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::RotatePlaneModel::RotatePlaneModel()
{
}
DCP::RotatePlaneModel::~RotatePlaneModel()
{
}