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
#include <dcp06/core/GUI.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP06DlgC);


// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// Description: Constructor
DCP::DCP06DlgC::DCP06DlgC() : m_pCombo( NULL ), m_pCombo2(NULL), m_pComboBox(NULL),
	m_pComboBoxObserver(OBS_METHOD_TO_PARAM0(DCP06DlgC, OnComboBoxChanged), this)
{
}

// ------------------------------------------------------------------------------------------------
//
// On Initialize Dialog
//
void DCP::DCP06DlgC::OnInitDialog()
{

//	SetTxtApplicationId(10111);
   
  //  unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

    // Add fields to dialog
    //----------------------------- Empty row ------------------------------
   // (void)InsertEmptyLine( unHeight );
    //------------------------------ Comboline -----------------------------
    m_pCombo = new GUI::ComboLineCtrlC( GUI::ComboLineCtrlC::IC_Angle );
    m_pCombo->SetId( eCombo );

    // Note: All statements setting or changing text strings should use
    //       '...->SetTextTok' with text tokens as parameters rather than
    //       text strings !
    (void)m_pCombo->SetText( L"Combo" );

    m_pCombo->GetAngleInputCtrl()->SetEmptyAllowed( true );
    void(m_pCombo->GetAngleInputCtrl()->SetDecimalPlaces( 4 ) );
    void(m_pCombo->GetAngleInputCtrl()->SetRangeStd( 0.0, 400.0 ) );
    void(m_pCombo->SetCtrlState( GUI::BaseCtrlC::CS_ReadWrite ) );
    AddCtrl( m_pCombo );


	   //------------------------------ Comboline -----------------------------
	m_pCombo2 = new GUI::ComboLineCtrlC( GUI::ComboLineCtrlC::IC_Distance );
    m_pCombo2->SetId( eCombo2 );

    // Note: All statements setting or changing text strings should use
    //       '...->SetTextTok' with text tokens as parameters rather than
    //       text strings !
    (void)m_pCombo2->SetText( L"ComboDist" );

	m_pCombo2->GetDistanceInputCtrl()->SetEmptyAllowed( true );
    void(m_pCombo2->GetDistanceInputCtrl()->SetDecimalPlaces( 4 ) );
    void(m_pCombo2->GetDistanceInputCtrl()->SetRangeStd( 0.0, 400.0 ) );
    void(m_pCombo2->SetCtrlState( GUI::BaseCtrlC::CS_ReadWrite ) );
    AddCtrl( m_pCombo2 );

	m_pComboBox =  new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pComboBox->SetId(eComboBox);
	m_pComboBox->SetText(StringC(AT_DCP06,P_DCP_COMBO_TOK));
	m_pComboBox->GetComboBoxInputCtrl()->AddItem(L"One");
	m_pComboBox->GetComboBoxInputCtrl()->AddItem(L"two");
	m_pComboBox->GetComboBoxInputCtrl()->AddItem(L"Third");
	AddCtrl(m_pComboBox);

	m_pComboBoxObserver.Attach(m_pComboBox->GetSubject());
}

// Description: only accept hello world Model objects
bool DCP::DCP06DlgC::SetModel( GUI::ModelC* pModel )
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

// Description: refresh all controls
void DCP::DCP06DlgC::RefreshControls()
{
	if(m_pComboBox)
	{
		m_pComboBox->GetComboBoxInputCtrl()->SetSelectedId (GetDCP06Model()->m_nOption);	
	}
}

// Description: update the road model with the new values
void DCP::DCP06DlgC::UpdateData()
{
	GetDCP06Model()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}

// Description: Hello World model
DCP::DCP06ModelC* DCP::DCP06DlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}



void DCP::DCP06DlgC::OnComboBoxChanged(int unNotifyCode, int ulParam2)
{
	if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	{


	}
	if(unNotifyCode== GUI::NC_ONEDITMODE_LEFT)
	{
	}


}


