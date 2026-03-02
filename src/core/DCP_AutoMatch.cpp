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
#include <dcp06/core/DCP_AutoMatch.hpp>
#include <dcp06/core/DCP_Defs.hpp>


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

DCP::DCP05AutoMatchDlgC::DCP05AutoMatchDlgC(DCP::DCP05ModelC* pDCP05Model):
		GUI::ModelHandlerC(),GUI::StandardDialogC(),m_pLine1(0),m_pLine2(0),m_pX(0), m_pY(0), m_pZ(0),
		m_pDCP05Model(pDCP05Model)
{
	//SetTxtApplicationId(AT_DCP05);
}


            // Description: Destructor
DCP::DCP05AutoMatchDlgC::~DCP05AutoMatchDlgC()
{
}

void DCP::DCP05AutoMatchDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	//InsertEmptyLine(); CAPTIVATE

	
	StringC sActual;
	sActual.LoadTxt(AT_DCP05,TT_DCP_ACTUAL_TOK);

	StringC sDesign;
	sDesign.LoadTxt(AT_DCP05,TT_DCP_DESIGN_TOK);
	
	StringC sDev;
	sDev.LoadTxt(AT_DCP05, TT_DCP_DEV_TOK);
	
	m_pLine1 = new GUI::TextCtrlC();
	m_pLine1->SetId(eLine1);
	//m_pLine1->SetTextStr(sNearestPoint);
	AddCtrl(m_pLine1);

	// InsertEmptyLine(); CAPTIVATE

	StringC sHeader;
	sHeader.Format(L"%2s%9s %9s %s", (const wchar_t*) L" ", (const wchar_t*) sActual, (const wchar_t*) sDesign,(const wchar_t*) sDev);
	
	m_pLine2 = new GUI::TextCtrlC();
	m_pLine2->SetId(eLine2);
	m_pLine2->SetText(sHeader);
	AddCtrl(m_pLine2);

	m_pX = new GUI::TextCtrlC();
	m_pX->SetId(eX);
	AddCtrl(m_pX);

	m_pY = new GUI::TextCtrlC();
	m_pY->SetId(eY);
	AddCtrl(m_pY);

	m_pZ = new GUI::TextCtrlC();
	m_pZ->SetId(eZ);
	AddCtrl(m_pZ);

	//SetHelpTok(H_DCP_AUTOMATCH_TOK,0);
}



void DCP::DCP05AutoMatchDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP05AutoMatchDlgC::UpdateData()
{
}

// Description: refresh all controls
void DCP::DCP05AutoMatchDlgC::RefreshControls()
{
	if(m_pLine1 && m_pLine2 && m_pX && m_pY && m_pZ)
	{	
		StringC sNearestPoint;
		sNearestPoint.LoadTxt(AT_DCP05, L_DCP_NEAREST_POINT_TOK);
		sNearestPoint += L" ";
		sNearestPoint += GetDataModel()->pointid;

		StringC sX, sY, sZ;
		sX.LoadTxt(AT_DCP05, P_DCP_X_TOK);
		sY.LoadTxt(AT_DCP05, P_DCP_Y_TOK);
		sZ.LoadTxt(AT_DCP05, P_DCP_Z_TOK);

		short iDecimals = m_pDCP05Model->m_nDecimals;

		StringC sXline, sYline, sZline;

		sXline.Format(L"%1.1s:%9.*f %9.*f %9.*f", (const wchar_t*) sX, iDecimals, GetDataModel()->xmea,iDecimals,GetDataModel()->xdsg ,iDecimals, GetDataModel()->xmea - GetDataModel()->xdsg);
		sYline.Format(L"%1.1s:%9.*f %9.*f %9.*f", (const wchar_t*) sY, iDecimals, GetDataModel()->ymea,iDecimals,GetDataModel()->ydsg ,iDecimals, GetDataModel()->ymea - GetDataModel()->ydsg);
		sZline.Format(L"%1.1s:%9.*f %9.*f %9.*f", (const wchar_t*) sZ, iDecimals, GetDataModel()->zmea,iDecimals,GetDataModel()->zdsg ,iDecimals, GetDataModel()->zmea - GetDataModel()->zdsg);
	
		m_pLine1->SetText(sNearestPoint);
		m_pX->SetText(sXline);
		m_pY->SetText(sYline);
		m_pZ->SetText(sZline);
	}
}
// Description: only accept hello world Model objects
bool DCP::DCP05AutoMatchDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05AutoMatchModelC* pDCP05AutoMatchModel = dynamic_cast< DCP::DCP05AutoMatchModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP05AutoMatchModel != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05AutoMatchModel ))
    {
		// kopioidaan tarvittavat arvot
        RefreshControls();
		return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP05AutoMatchModelC* DCP::DCP05AutoMatchDlgC::GetDataModel() const
{
    return (DCP::DCP05AutoMatchModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05AutoMatchControllerC::DCP05AutoMatchControllerC(DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_AUTOMATCH_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05AutoMatchDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( AUTOMATCH_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_ABORT_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK5, vDef );
		
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP05AutoMatchControllerC::~DCP05AutoMatchControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP05AutoMatchControllerC::SetModel( GUI::ModelC* pModel )
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

// SET
void DCP::DCP05AutoMatchControllerC::OnF1Pressed()
{
   if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
   (void)Close(EC_KEY_ESC);
}

// CONT
void DCP::DCP05AutoMatchControllerC::OnF5Pressed()
{
   if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    // m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);

}


// Description: React on close of tabbed dialog
void DCP::DCP05AutoMatchControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05AutoMatchControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// DCP05LineSettingModelC
// ===========================================================================================
DCP::DCP05AutoMatchModelC::DCP05AutoMatchModelC()
{
			xmea = 0.0;
			ymea = 0.0;
			zmea = 0.0;
			xdsg = 0.0; 
			ydsg = 0.0;
			zdsg = 0.0;
			pointid = L"";
	
}
DCP::DCP05AutoMatchModelC::~DCP05AutoMatchModelC()
{
}