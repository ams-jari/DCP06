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
#include <dcp06/core/SelectPoint.hpp>
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
SelectPointDialog::SelectPointDialog():poMultiColCtrl(nullptr)
	
{
	//SetTxtApplicationId(AT_DCP06);

	
	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
}

// Description: Destructor
SelectPointDialog::~SelectPointDialog()
{

}

void SelectPointDialog::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();

	/*
	GUI::ListConfigColsC oConfigCols1;

	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Point));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_ActualDesign));
	*/
	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);


	// text
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NO_TOK),CI_No));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_ID_TOK),CI_Point));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_STATUS_TOK),CI_ActualDesign));

	poMultiColCtrl->AddColSelection(CI_No, CI_Point, CI_ActualDesign);
	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
}


void SelectPointDialog::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

	DCP::SelectPointModel* pModel = GetDataModel();
	
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_SELECT_POINT_TOK);
	char temp[20];
	sprintf(temp,"(%d)", pModel->m_iCounts);
	sTitle += 	StringC(temp);
	SetTitle(sTitle);

	//BeginDraw(); 
	StringC sPointNo;
	for(int i=0; i < pModel->m_iCounts; i++)
	{
		sprintf(temp,"%d",i+1);
		sPointNo = temp;
		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i + 1));

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i + 1, temp));

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i + 1, pModel->points[i].point_id));

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_ActualDesign,(short) i + 1, pModel->points[i].point_status));
	}
	
	if(pModel->m_iSelectedId > 0 && pModel->m_iSelectedId <= pModel->m_iCounts)
		poMultiColCtrl->SetSelectedId( pModel->m_iSelectedId);
	else
		poMultiColCtrl->SetSelectedId(1);			
	
	//EndDraw();
	

	// 
	/*
	StringC sSelected = L" ";
	StringC sActualDesign = sActualSelected + L"/" + sDesignSelected;
	StringC sPoint;
		for(short i=0; i < MAX_POINTS_IN_FILE; i++)
	{
		sprintf(temp,"point%d",i+1);
		sPoint = temp;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Point, (short) i, sPoint));

		USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_ActualDesign,(short) i, sActualDesign)); 
	}
	EndDraw();
	*/

}

void DCP::SelectPointDialog::UpdateData()
{
	
	short iSelected = poMultiColCtrl->GetSelectedId();
	StringC strSelectedId;
	poMultiColCtrl->GetCellText(CI_Point,iSelected,strSelectedId);
	GetDataModel()->m_iSelectedId = iSelected;
	GetDataModel()->m_strSelectedPoint = strSelectedId;
}

// Description: only accept DCP06 Model objects
bool DCP::SelectPointDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::SelectPointModel* pDcpModel = dynamic_cast< DCP::SelectPointModel* >( pModel );

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
DCP::SelectPointModel* DCP::SelectPointDialog::GetDataModel() const
{
    return (DCP::SelectPointModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::SelectPointController::SelectPointController()
    : m_pDlg( nullptr )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SELECT_POINT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::SelectPointDialog;  //lint !e1524 new in constructor for class 
    (void)AddDialog( SELECT_POINT_DLG, m_pDlg, true );

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

DCP::SelectPointController::~SelectPointController()
{

}

// Description: Route model to everybody else
bool DCP::SelectPointController::SetModel( GUI::ModelC* pModel )
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

// OPEN
void DCP::SelectPointController::OnF1Pressed()
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
void DCP::SelectPointController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::SelectPointController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::SelectPointModel::SelectPointModel()
{
	m_iSelectedId = -1;
	m_strSelectedPoint = L"";
	memset(&points[0],0, sizeof(S_SELECT_POINT) * MAX_SELECT_POINTS);
}
DCP::SelectPointModel::~SelectPointModel()
{
	sqrt(4.0);
}


