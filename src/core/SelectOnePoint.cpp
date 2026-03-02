// YKSI PISTE ACTUAL TAI DESIGN
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
#include <dcp06/core/DCP_SelectOnePoint.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

OBS_IMPLEMENT_EXECUTE(DCP::DCP06SelectOnePointDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP06SelectOnePointDlgC::DCP06SelectOnePointDlgC(DCP::DCP06ModelC *pDCP06Model):poMultiColCtrl(NULL),
	m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(DCP06SelectOnePointDlgC, OnChanged), this),
	m_pDCP06Model(pDCP06Model)
{
	//SetTxtApplicationId(AT_DCP06);

	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sActualNonSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_NONSELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
	sDesignNonSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_NONSELECTED_TOK);

	/*
	// clear buffer
	for(int i=0; i < MAX_POINTS_IN_FILE; i++)
	{
		sel_points[i].bActualDefined = false;
		sel_points[i].bDesignDefined = false;
		sel_points[i].bActualSelected = false;
		sel_points[i].bDesignSelected = false;
		sel_points[i].bPointSelected = false;
		sel_points[i].iId = 0;
	}
	*/
}
// Description: Destructor
DCP06SelectOnePointDlgC::~DCP06SelectOnePointDlgC()
{

}

void DCP06SelectOnePointDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	/* NOT CAPTIVATE
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
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NUMBER_TOK), CI_No));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_ID_TOK),CI_Point));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_ACTUAL_DESIGN_TOK),CI_ActualDesign));
	
	poMultiColCtrl->AddColSelection(CI_No,CI_Point,CI_ActualDesign);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_ACTUAL_OR_DESIGN_TOK);
	SetFunctionKey( FK1, vDef );
	
	m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());
}

void DCP06SelectOnePointDlgC::OnChanged(int unNotifyCode,int ulParam2)
{
	//if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_ACCEPTED)
	{
	}
	else if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_CHANGED)
	{

	}
}


void DCP06SelectOnePointDlgC::OnF1Pressed(void)
{
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	StringC sTemp;
	//poMultiColCtrl->GetCellStr(CI_Selected,iSelectedId,sTemp);

	
	StringC sActDes;
	poMultiColCtrl->GetCellText(CI_ActualDesign,iSelectedId,sActDes);
	
	//if(GetDCP06Model()->points[iSelectedId].bPointSelected)
	//{
		GetDCP06Model()->points[iSelectedId].bPointSelected = false;
		if(GetDCP06Model()->points[iSelectedId].bActualSelected && GetDCP06Model()->points[iSelectedId].bDesignDefined)
		{
			GetDCP06Model()->points[iSelectedId].bActualSelected = false;
			GetDCP06Model()->points[iSelectedId].bDesignSelected = true;
			//sActDes = L"a/D";
			sActDes = sActualNonSelected + L"/" + sDesignSelected;
		}
		else if(GetDCP06Model()->points[iSelectedId].bDesignSelected && GetDCP06Model()->points[iSelectedId].bActualDefined)
		{
			GetDCP06Model()->points[iSelectedId].bActualSelected = true;
			GetDCP06Model()->points[iSelectedId].bDesignSelected = false;
			sActDes = sActualSelected + L"/" + sDesignNonSelected;
			//sActDes = L"A/d";
		}
		sTemp = L" ";
		poMultiColCtrl->SetCellText(CI_ActualDesign,iSelectedId,sActDes);
		//poMultiColCtrl->SetCellStr(CI_Selected,iSelectedId,sTemp);
		//remove_point_table(iSelectedId);
	/*
	}
	else
	{
		sTemp = L"*";	
		GetDCP06Model()->points[iSelectedId].bPointSelected = true;
		GetDCP06Model()->iSelectedNo = iSelectedId;
		//poMultiColCtrl->SetCellStr(CI_Selected,iSelectedId,sTemp);
	}
	*/
}

void DCP06SelectOnePointDlgC::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();
	/*
	unsigned short unCount  = 0;
	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, L"Filename1"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date, (short) unCount, L"DateFilename1"));

	unCount  = 1;
	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, L"Filename2"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date, (short) unCount, L"DateFilename2"));
	*/

	// 

	StringC sSelected = L" ";
	//StringC sActualDesign = L"a/D";
	StringC sActualDesign = sActualNonSelected + L"/" + sDesignSelected;
	StringC sPoint;
	short iDef =  GetDCP06Model()->m_iDef;
	//BeginDraw();  NO CAPTIVATE
	char temp[20],temp_no[10];
	
	for(short i=0; i < GetDCP06Model()->m_iPointsCount; i++)
	{
		sprintf(temp,"%-s",GetDCP06Model()->points[i].point_id);
		sPoint = temp;
		/*	
		 if(iDef == BOTH)
		 {
			GetDCP06Model()->points[i].bDesignSelected = (GetDCP06Model()->points[i].bDesignDefined) ? true : false;
			GetDCP06Model()->points[i].bActualSelected = (GetDCP06Model()->points[i].bActualDefined) ? true : false;
		 }
		 else if(GetDCP06Model()->points[i].bActualDefined == 1 && iDef == ACTUAL)
		 {
			GetDCP06Model()->points[i].bDesignSelected = false;
			GetDCP06Model()->points[i].bActualSelected = true;
					
			//	sprintf(Temp, "%-6.6s %c/%c ", pid,actual_selected[0] ,(des==1) ? design_nonselected[0] : '-');
		 }
		 else
		 {
			if(GetDCP06Model()->points[i].bDesignDefined)
				GetDCP06Model()->points[i].bDesignSelected = true;
			//if(!mea)
			//		pList[iCount-1].bActualSelected = false;

				 // 	sprintf(Temp, "%-6.6s %c/%c ", pid,(mea == 0) ? '-':actual_nonselected[0] , (des==0) ? '-': design_selected[0] );
		}
		 */
		/*
		sel_points[i].bActualDefined = true;
		sel_points[i].bDesignDefined = true;
		sel_points[i].bActualSelected = false;
		sel_points[i].bDesignSelected = true;
		sel_points[i].bPointSelected = false;
		sel_points[i].iId = 0;
		*/
		/*
		 if(GetDCP06Model()->points[i].bActualSelected && GetDCP06Model()->points[i].bActualDefined)
			sActualDesign  = sActualSelected;
		 else if(!GetDCP06Model()->points[i].bActualSelected && GetDCP06Model()->points[i].bActualDefined)
			sActualDesign = sActualNonSelected;
		 else
			sActualDesign = L"-";

		sActualDesign += L"/";

		 if(GetDCP06Model()->points[i].bDesignSelected && GetDCP06Model()->points[i].bDesignDefined)
			sActualDesign  += sDesignSelected;
		 else if(!GetDCP06Model()->points[i].bDesignSelected && GetDCP06Model()->points[i].bDesignDefined)
			sActualDesign = sDesignNonSelected;
		 else
			sActualDesign += L"-";	
		*/		
		 if(GetDCP06Model()->points[i].bActualSelected)
			sActualDesign  = sActualSelected;
		 else if(GetDCP06Model()->points[i].bActualDefined) 
		 {
			  sActualDesign = sActualNonSelected;
		 }	 
		else
			sActualDesign = L"-";
 
	 	 sActualDesign += L"/";

		 if(GetDCP06Model()->points[i].bDesignSelected)
			 sActualDesign  += sDesignSelected;
		 else if(GetDCP06Model()->points[i].bDesignDefined)
		 {
				if(GetDCP06Model()->points[i].bActualSelected == true)
					sActualDesign += sDesignNonSelected;
				else
					sActualDesign  += sDesignSelected;
		 }
		 else
				sActualDesign += L"-";	

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		sprintf(temp_no,"%-d",i+1);
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, StringC(temp_no)));

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i, sPoint));

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_ActualDesign,(short) i, sActualDesign)); 
		//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Selected,(short) i, sSelected)); 
	}
	//EndDraw(); NO CAPTIVATE

}

void DCP::DCP06SelectOnePointDlgC::UpdateData()
{
	StringC sSelected;
	short iSelected = poMultiColCtrl->GetSelectedId()+1;
	//poMultiColCtrl->GetCellStr(CI_Selected,iSelected, sSelected);
	//GetDCP06Model()->ADFFileName = sSelected;
	GetDCP06Model()->iSelectedNo = iSelected;

		
}
// Description: only accept hello world Model objects
bool DCP::DCP06SelectOnePointDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06SelectOnePointModelC* pDCP06Model = dynamic_cast< DCP::DCP06SelectOnePointModelC* >( pModel );

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
DCP::DCP06SelectOnePointModelC* DCP::DCP06SelectOnePointDlgC::GetDCP06Model() const
{
    return (DCP::DCP06SelectOnePointModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}




// ******************************************************************************

DCP::DCP06SelectOnePointControllerC::DCP06SelectOnePointControllerC(DCP::DCP06ModelC *pDCP06Model)
    : m_pDCP06SelectOnePointDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_3DFILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
     m_pDCP06SelectOnePointDlg = new DCP::DCP06SelectOnePointDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SELECT_ONE_POINT_DLG, m_pDCP06SelectOnePointDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_ACTUAL_OR_DESIGN_TOK);
	SetFunctionKey( FK1, vDef );

	/*
	vDef.nLable = K_DCP_NEW_TOK;
	SetFunctionKey( FK2, vDef );

	vDef.nLable = K_DCP_COPY_TOK;
	SetFunctionKey( FK3, vDef );

	vDef.nLable = K_DCP_SWAP_TOK;
	SetFunctionKey( FK4, vDef );

	vDef.nLable = K_DCP_CLOSE_TOK;
	SetFunctionKey( FK5, vDef );
	*/
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";
	SetFunctionKey( SHFK6, vDef1 );

	/*
	// SHIFT
	vDef.nLable = K_DCP_DEL_TOK;
	SetFunctionKey( SHFK2, vDef );
	
	vDef.nLable = K_DCP_SAVE_TOK;
	SetFunctionKey( SHFK3, vDef );

	vDef.nLable = K_DCP_VIEW_TOK;
	SetFunctionKey( SHFK5, vDef );
	*/
} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP06SelectOnePointControllerC::~DCP06SelectOnePointControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06SelectOnePointControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDCP06SelectOnePointDlg->SetModel( pModel );
	
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
void DCP::DCP06SelectOnePointControllerC::OnF1Pressed()
{

}

// NEW
void DCP::DCP06SelectOnePointControllerC::OnF2Pressed()
{
}

// CLOSE
void DCP::DCP06SelectOnePointControllerC::OnF5Pressed()
{

}

// CONT
void DCP::DCP06SelectOnePointControllerC::OnF6Pressed()
{
    if (m_pDCP06SelectOnePointDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDCP06SelectOnePointDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}
// DEL
void DCP::DCP06SelectOnePointControllerC::OnSHF2Pressed()
{
}

// VIEW
void DCP::DCP06SelectOnePointControllerC::OnSHF5Pressed()
{
	/*
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP06InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP06MeasDlg->GetDCP06Model());
	SetActiveController(INIT_CONTROLLER, true);
	*/
}

// Description: React on close of tabbed dialog
void DCP::DCP06SelectOnePointControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06SelectOnePointControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDCP06SelectOnePointDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// Instantiate template classes
DCP::DCP06SelectOnePointModelC::DCP06SelectOnePointModelC()
{
	//memset(&sel_points[0],0,sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
	//memset(nro_table,0,sizeof(short) * MAX_POINTS_IN_FILE*2);
}


DCP::DCP06SelectOnePointModelC::~DCP06SelectOnePointModelC()
{
}