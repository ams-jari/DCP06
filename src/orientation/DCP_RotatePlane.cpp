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
#include <dcp06/init/DCP_DCP06Init.hpp>
#include <dcp06/core/DCP_DCP06Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/orientation/DCP_RotatePlane.hpp>
#include <dcp06/core/DCP_Defs.hpp>

#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06RotatePlaneDlgC::DCP06RotatePlaneDlgC(DCP::DCP06ModelC *pDCP06Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
			m_pInfo1(0),m_pInfo2(0), m_pAxis1(0),m_pAxis2(0),m_pDCP06Model(pDCP06Model)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::DCP06RotatePlaneDlgC::~DCP06RotatePlaneDlgC()
{

}

void DCP::DCP06RotatePlaneDlgC::OnInitDialog(void)
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

void DCP::DCP06RotatePlaneDlgC::OnDialogActivated()
{
	m_pAxis1->GetFloatInputCtrl()->SetFloat(0.0);
	m_pAxis2->GetFloatInputCtrl()->SetFloat(0.0);

	RefreshControls();
}

void DCP::DCP06RotatePlaneDlgC::UpdateData()
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
void DCP::DCP06RotatePlaneDlgC::RefreshControls()
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
// Description: only accept hello world Model objects
bool DCP::DCP06RotatePlaneDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06RotatePlaneModelC* pDCP06Model = dynamic_cast< DCP::DCP06RotatePlaneModelC* >( pModel );

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
DCP::DCP06RotatePlaneModelC* DCP::DCP06RotatePlaneDlgC::GetDataModel() const
{
    return (DCP::DCP06RotatePlaneModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06RotatePlaneControllerC::DCP06RotatePlaneControllerC(DCP::DCP06ModelC *pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
   // SetTitleTok( AT_DCP06, T_DCP_ROTATE_PLANE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::DCP06RotatePlaneDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
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

DCP::DCP06RotatePlaneControllerC::~DCP06RotatePlaneControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06RotatePlaneControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// CONT
void DCP::DCP06RotatePlaneControllerC::OnF1Pressed()
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
void DCP::DCP06RotatePlaneControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06RotatePlaneControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06RotatePlaneModelC::DCP06RotatePlaneModelC()
{
}
DCP::DCP06RotatePlaneModelC::~DCP06RotatePlaneModelC()
{
}