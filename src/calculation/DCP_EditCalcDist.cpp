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
#include <dcp06/calculation/DCP_EditCalcDist.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_Tool.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_Types.hpp>

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


// USER DIALOG

DCP::DCP05EditCalcDistDlgC::DCP05EditCalcDistDlgC(DCP::DCP05ModelC *pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pDistId(0),m_pRefId(0),m_pNote(0),m_pDCP05Model(pDCP05Model)
{
	//SetTxtApplicationId(AT_DCP05);
}


            // Description: Destructor
DCP::DCP05EditCalcDistDlgC::~DCP05EditCalcDistDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP05EditCalcDistDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	//InsertEmptyLine(); CAPTIVATE
	//InsertEmptyLine(); CAPTIVATE

	m_pDistId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistId->SetId(eDistId);
	m_pDistId->SetText(StringC(AT_DCP05,P_DCP_DISTANCE_ID_TOK));
	m_pDistId->GetStringInputCtrl()->SetCharsCountMax(6);
	m_pDistId->SetEmptyAllowed(true);
	AddCtrl(m_pDistId);

	m_pRefId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRefId->SetId(eRefId);
	m_pRefId->SetText(StringC(AT_DCP05,P_DCP_REF_ID_TOK));
	m_pRefId->GetStringInputCtrl()->SetCharsCountMax(6);
	m_pRefId->SetEmptyAllowed(true);
	AddCtrl(m_pRefId);

	m_pNote = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNote->SetId(eNote);
	m_pNote->SetText(StringC(AT_DCP05,P_DCP_NOTE_TOK));
	m_pNote->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pNote->SetEmptyAllowed(true);
	AddCtrl(m_pNote);

	//SetHelpTok(H_DCP_CALC_DIST_EDIT_TOK,0);
}


void DCP::DCP05EditCalcDistDlgC::OnDialogActivated()
{
	m_pCommon = new DCP05CommonC(m_pDCP05Model);
	RefreshControls();
}

void DCP::DCP05EditCalcDistDlgC::UpdateData()
{
	GetDataModel()->sDistId = m_pDistId->GetStringInputCtrl()->GetString();
	GetDataModel()->sRefId = m_pRefId->GetStringInputCtrl()->GetString();
	GetDataModel()->sNote = m_pNote->GetStringInputCtrl()->GetString();
	
}

// Description: refresh all controls
void DCP::DCP05EditCalcDistDlgC::RefreshControls()
{
	if(m_pDistId  && m_pRefId && m_pNote)
	{	
		m_pDistId->GetStringInputCtrl()->SetString(GetDataModel()->sDistId);
		m_pRefId->GetStringInputCtrl()->SetString(GetDataModel()->sRefId);
		m_pNote->GetStringInputCtrl()->SetString(GetDataModel()->sNote);
		
	}
}
// Description: only accept hello world Model objects
bool DCP::DCP05EditCalcDistDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05EditCalcDistModelC* pDCP05Model = dynamic_cast< DCP::DCP05EditCalcDistModelC* >( pModel );

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

// Description: Hello World model
DCP::DCP05EditCalcDistModelC* DCP::DCP05EditCalcDistDlgC::GetDataModel() const
{
    return (DCP::DCP05EditCalcDistModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}



// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05EditCalcDistControllerC::DCP05EditCalcDistControllerC(DCP::DCP05ModelC *pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model),m_pCommon(0)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_CALC_DIST_EDIT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05EditCalcDistDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( XORYORZ_DLG, m_pDlg, true );

	m_pCommon = new DCP05CommonC(m_pDCP05Model);

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	// Hide SHF6(Quit Key)
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );
	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05EditCalcDistControllerC::~DCP05EditCalcDistControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP05EditCalcDistControllerC::SetModel( GUI::ModelC* pModel )
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

// CONT
void DCP::DCP05EditCalcDistControllerC::OnF1Pressed()
{
    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}




// Description: React on close of tabbed dialog
void DCP::DCP05EditCalcDistControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05EditCalcDistControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}