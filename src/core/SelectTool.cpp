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
#include <dcp06/core/SelectTool.hpp>
#include <dcp06/core/Defs.hpp>
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
SelectToolDialog::SelectToolDialog():poMultiColCtrl(nullptr)
	
{
	//SetTxtApplicationId(AT_DCP06);
}

// Description: Destructor
SelectToolDialog::~SelectToolDialog()
{

}

void SelectToolDialog::OnInitDialog(void)
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


void SelectToolDialog::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

	DCP::SelectToolModel* pModel = GetDataModel();
	
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

void DCP::SelectToolDialog::UpdateData()
{
	
	short iSelected = poMultiColCtrl->GetSelectedId();
	StringC strSelectedId;
	//poMultiColCtrl->GetCellStr(CI_Point,iSelected,strSelectedId);
	GetDataModel()->m_iSelectedId = iSelected;
	//GetDataModel()->m_strSelectedPoint = strSelectedId;
}

// Description: only accept hello world Model objects
bool DCP::SelectToolDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::SelectToolModel* pModel = dynamic_cast< DCP::SelectToolModel* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::SelectToolModel* DCP::SelectToolDialog::GetDataModel() const
{
    return (DCP::SelectToolModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::SelectToolController::SelectToolController()
    : m_pDlg( nullptr )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SELECT_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::SelectToolDialog;  //lint !e1524 new in constructor for class 
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

DCP::SelectToolController::~SelectToolController()
{

}

// Description: Route model to everybody else
bool DCP::SelectToolController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::Model* pModel = dynamic_cast< DCP::Model* >( pModel );

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

// OPEN
void DCP::SelectToolController::OnF1Pressed()
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
void DCP::SelectToolController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::SelectToolController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// SelectToolModel
// ===========================================================================================

// Instantiate template classes
DCP::SelectToolModel::SelectToolModel()
{
	m_iSelectedId = -1;
}
DCP::SelectToolModel::~SelectToolModel()
{
	sqrt(4.0);
}


