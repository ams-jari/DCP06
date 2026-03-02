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
// $NoKeywor ds: $

#include "stdafx.h"
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/init/DCP_DCP05Unit.hpp>
#include <dcp06/core/DCP_Defs.hpp>
//#include <DCP_Text.hpp>


#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP05UnitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::DCP05UnitDlgC::DCP05UnitDlgC():m_pComboBoxObserver(OBS_METHOD_TO_PARAM0(DCP05UnitDlgC, OnComboBoxChanged), this),
	m_pUnit(0), m_pDecimals(0)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP05);
}


            // Description: Destructor
DCP::DCP05UnitDlgC::~DCP05UnitDlgC()
{

}

void DCP::DCP05UnitDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
		

	// Add fields to dialog
	m_pUnit = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pUnit->SetId(eUnit);
	m_pUnit->SetText(StringC(AT_DCP05,P_DCP_UNIT_TOK));
	m_pUnit->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP05,V_DCP_MM_TOK),MM);
	m_pUnit->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP05,V_DCP_INCH_TOK), INCH);
	m_pUnit->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP05,V_DCP_FEET_TOK), FEET);
	//m_pUnit->SetAutoColon(false);
	//m_pUnit->SetColonPosition(9* 15);
	AddCtrl(m_pUnit);

	
	m_pDecimals = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pDecimals->SetId(eDecimals);
	m_pDecimals->SetText(StringC(AT_DCP05,P_DCP_DECIMALS_TOK));
	if(GetDCP05Model()->m_nUnit == MM)
	{
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"1",1);
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"2",2);
	}
	else if(GetDCP05Model()->m_nUnit == INCH)
	{
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"2",1);
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"3",2);

	}
	else
	{
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"3",1);
		m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"4",2);
	}
	
	//m_pDecimals->SetAutoColon(false);
	//m_pDecimals->SetColonPosition(9* 15);
	AddCtrl(m_pDecimals);

	m_pComboBoxObserver.Attach(m_pUnit->GetSubject());

	//SetHelpTok(H_DCP_UNIT_TOK,0);
	
}


void DCP::DCP05UnitDlgC::OnDialogActivated()
{
	RefreshControls();
}
void DCP::DCP05UnitDlgC::OnComboBoxChanged( int unNotifyCode,  int ulParam2)
{
	if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	{
		
		int ret = m_pUnit->GetComboBoxInputCtrl()->GetSelectedId();
		int ret2;
		// mm
		if(ret == MM)
		{
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(2);
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(1);

			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"1",1);
			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"2",2);

		}
		// inch
		else if( ret == INCH)
		{
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(2);
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(1);

			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"2",1);
			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"3",2);
		}
		// feet
		else
		{
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(1);
			ret2 = m_pDecimals->GetComboBoxInputCtrl()->RemoveItem(2);

			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"3",1);
			m_pDecimals->GetComboBoxInputCtrl()->AddItem(L"4",2);
		}

	}
	if(unNotifyCode== GUI::NC_ONEDITMODE_LEFT)
	{
	}
}

// Description: refresh all controls
void DCP::DCP05UnitDlgC::RefreshControls()
{	
	if(m_pUnit) m_pUnit->GetComboBoxInputCtrl()->SetSelectedId(GetDCP05Model()->m_nUnit);
	if(m_pDecimals)
	{	
		if(GetDCP05Model()->m_nUnit == MM)
			m_pDecimals->GetComboBoxInputCtrl()->SetSelectedId(GetDCP05Model()->m_nDecimals);
		else if(GetDCP05Model()->m_nUnit == INCH)
		{
			m_pDecimals->GetComboBoxInputCtrl()->SetSelectedId(GetDCP05Model()->m_nDecimals-1);
		}
		else
		{
			m_pDecimals->GetComboBoxInputCtrl()->SetSelectedId(GetDCP05Model()->m_nDecimals-2);
		}
		
	}
}

void DCP::DCP05UnitDlgC::UpdateData()
{
	GetDCP05Model()->m_nUnit		= m_pUnit->GetComboBoxInputCtrl()->GetSelectedId();

	StringC sDec = m_pDecimals->GetComboBoxInputCtrl()->GetSelectedStr();
	char cDec[10];
	//UTL::UnicodeToAscii(cDec, sDec);
	BSS::UTI::BSS_UTI_WCharToAscii( sDec, cDec );
	
	GetDCP05Model()->m_nDecimals	= atoi(cDec);
	//GetDCP05Model()->m_nDecimals	= m_pDecimals->GetComboBoxInputCtrl()->GetSelectedId();
	
}

DCP::DCP05ModelC* DCP::DCP05UnitDlgC::GetDCP05Model() const
{
    return (DCP::DCP05ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// Description: only accept hello world Model objects
bool DCP::DCP05UnitDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

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
// ====================================  DCP05ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UnitControllerC
// 
DCP::DCP05UnitControllerC::DCP05UnitControllerC()
    : m_pDlg( NULL )
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_UNIT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05UnitDlgC;  //lint !e1524 new in constructor for class 
    (void)AddDialog( UNIT_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
    vDef.poOwner = this;
	  
	SetFunctionKey( FK1, vDef );


	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05UnitControllerC::~DCP05UnitControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP05UnitControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

// Description: Handle change of position values
void DCP::DCP05UnitControllerC::OnF1Pressed()
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
void DCP::DCP05UnitControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}