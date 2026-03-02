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
#include <dcp06/core/DCP_SelectTool.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <math.h>

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================


// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP06SelectToolDlgC::DCP06SelectToolDlgC():poMultiColCtrl(NULL)
	
{
	//SetTxtApplicationId(AT_DCP06);
}

// Description: Destructor
DCP06SelectToolDlgC::~DCP06SelectToolDlgC()
{

}

void DCP06SelectToolDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();

	//GUI::ListConfigColsC oConfigCols1;

	//USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	
	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

	// text
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NO_TOK),CI_No));

	poMultiColCtrl->AddColSelection(CI_No);
	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
}


void DCP06SelectToolDlgC::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

	DCP::DCP06SelectToolModelC* pModel = GetDataModel();
	
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_SELECT_TOOL_TOK);
	char temp[20];
	sprintf(temp,"(%d)", pModel->m_iCounts);
	sTitle += 	StringC(temp);
	SetTitle(sTitle);
	//poMultiColCtrl->AddCol(sTitle,CI_No);

	//BeginDraw(); 
	StringC sLineText;

	// first line <none>
	sLineText.Format(L"%3d %s",1,L"<None>");
	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) 1));
	USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) 1, sLineText));

	for(int i=0; i < pModel->m_iCounts; i++)
	{
		sLineText.Format(L"%3d %s",i+2,(const wchar_t*) StringC(pModel->tool_table[i].tool_id));
		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i+2));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i+2, sLineText));
	}
	
	if(pModel->m_iSelectedId > 0 && pModel->m_iSelectedId <= pModel->m_iCounts+1)
		poMultiColCtrl->SetSelectedId( pModel->m_iSelectedId);
	else
		poMultiColCtrl->SetSelectedId(1);			
	
	//EndDraw();
}

void DCP::DCP06SelectToolDlgC::UpdateData()
{
	
	short iSelected = poMultiColCtrl->GetSelectedId();
	StringC strSelectedId;
	//poMultiColCtrl->GetCellStr(CI_Point,iSelected,strSelectedId);
	GetDataModel()->m_iSelectedId = iSelected;
	//GetDataModel()->m_strSelectedPoint = strSelectedId;
}

// Description: only accept hello world Model objects
bool DCP::DCP06SelectToolDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06SelectToolModelC* pDCP06Model = dynamic_cast< DCP::DCP06SelectToolModelC* >( pModel );

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
DCP::DCP06SelectToolModelC* DCP::DCP06SelectToolDlgC::GetDataModel() const
{
    return (DCP::DCP06SelectToolModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::DCP06SelectToolControllerC::DCP06SelectToolControllerC()
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SELECT_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP06SelectToolDlgC;  //lint !e1524 new in constructor for class 
    (void)AddDialog( SELECT_TOOL_DLG, m_pDlg, true );

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

DCP::DCP06SelectToolControllerC::~DCP06SelectToolControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06SelectToolControllerC::SetModel( GUI::ModelC* pModel )
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

// OPEN
void DCP::DCP06SelectToolControllerC::OnF1Pressed()
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
void DCP::DCP06SelectToolControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06SelectToolControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// SelectToolModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06SelectToolModelC::DCP06SelectToolModelC()
{
	m_iSelectedId = -1;
}
DCP::DCP06SelectToolModelC::~DCP06SelectToolModelC()
{
	sqrt(4.0);
}


