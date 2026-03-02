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
DCP05SelectToolDlgC::DCP05SelectToolDlgC():poMultiColCtrl(NULL)
	
{
	//SetTxtApplicationId(AT_DCP05);
}

// Description: Destructor
DCP05SelectToolDlgC::~DCP05SelectToolDlgC()
{

}

void DCP05SelectToolDlgC::OnInitDialog(void)
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
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_POINT_NO_TOK),CI_No));

	poMultiColCtrl->AddColSelection(CI_No);
	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
}


void DCP05SelectToolDlgC::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

	DCP::DCP05SelectToolModelC* pModel = GetDataModel();
	
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP05,T_DCP_SELECT_TOOL_TOK);
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

void DCP::DCP05SelectToolDlgC::UpdateData()
{
	
	short iSelected = poMultiColCtrl->GetSelectedId();
	StringC strSelectedId;
	//poMultiColCtrl->GetCellStr(CI_Point,iSelected,strSelectedId);
	GetDataModel()->m_iSelectedId = iSelected;
	//GetDataModel()->m_strSelectedPoint = strSelectedId;
}

// Description: only accept hello world Model objects
bool DCP::DCP05SelectToolDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05SelectToolModelC* pDCP05Model = dynamic_cast< DCP::DCP05SelectToolModelC* >( pModel );

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
DCP::DCP05SelectToolModelC* DCP::DCP05SelectToolDlgC::GetDataModel() const
{
    return (DCP::DCP05SelectToolModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::DCP05SelectToolControllerC::DCP05SelectToolControllerC()
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_SELECT_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05SelectToolDlgC;  //lint !e1524 new in constructor for class 
    (void)AddDialog( SELECT_TOOL_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05SelectToolControllerC::~DCP05SelectToolControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP05SelectToolControllerC::SetModel( GUI::ModelC* pModel )
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

// OPEN
void DCP::DCP05SelectToolControllerC::OnF1Pressed()
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
void DCP::DCP05SelectToolControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05SelectToolControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// SelectToolModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP05SelectToolModelC::DCP05SelectToolModelC()
{
	m_iSelectedId = -1;
}
DCP::DCP05SelectToolModelC::~DCP05SelectToolModelC()
{
	sqrt(4.0);
}


