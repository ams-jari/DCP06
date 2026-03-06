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
#include <dcp06/core/SelectMultiPoints.hpp>
#include <dcp06/core/Defs.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
#include <stdio.h>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

OBS_IMPLEMENT_EXECUTE(DCP::SelectMultiPointsDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
SelectMultiPointsDialog::SelectMultiPointsDialog(DCP::Model *pModel):poMultiColCtrl(nullptr),
	m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(SelectMultiPointsDialog, OnChanged), this),
	m_pModel(pModel),m_iSelectedCount(0)
{
	//SetTxtApplicationId(AT_DCP06);


	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sActualNonSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_NONSELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
	sDesignNonSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_NONSELECTED_TOK);

	m_strMaxPointSelected.LoadTxt(AT_DCP06,L_DCP_MAX_POINTS_SELECTED_TOK); 

	m_pCommon = new Common(pModel);
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
SelectMultiPointsDialog::~SelectMultiPointsDialog()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void SelectMultiPointsDialog::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	/*
	GUI::ListConfigColsC oConfigCols1;

	//USER_APP_VERIFY(oConfigCols1.AddCol(CW_Point, GUI::ListConfigColsC::AL_LEFT, CI_Point));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Point));
	*/

	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

	// infotext
	StringC sTemp1;
	sTemp1 = GetDataModel()->sInfo;
	USER_APP_VERIFY(poMultiColCtrl->AddCol(sTemp1,CI_Point)); // VIVA ONKO OIKEIN

	poMultiColCtrl->AddColSelection(CI_Point);

	// set column config active
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_SEL_MULTI_TOK);
	SetFunctionKey( FK1, vDef );

	// help
	//SetHelpTok(GetDataModel()->helpToken, 0);

	m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());
}

// ================================================================================================
// Description: OnChanged
// ================================================================================================
void SelectMultiPointsDialog::OnChanged( int unNotifyCode,  int ulParam2)
{
	//if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_ACCEPTED)
	{
	}
	else if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_CHANGED)
	{

	}
}
// ================================================================================================
// Description: RefreshControls
// ================================================================================================
void SelectMultiPointsDialog::RefreshControls()
{

}

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void SelectMultiPointsDialog::OnF1Pressed(void)
{
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	StringC sTemp;
	


	poMultiColCtrl->GetCellText(CI_Point,iSelectedId,sTemp);
	StringC sPointNo= sTemp.Mid(0,3);
	StringC sPoint	= sTemp.Mid(4,6);
	StringC sActDes = sTemp.Mid(11,3);
	StringC sSelected;
	StringC sLine;	

	if(GetDataModel()->sel_points[iSelectedId].bPointSelected)
	{
		GetDataModel()->sel_points[iSelectedId].bPointSelected = false;
		if(GetDataModel()->sel_points[iSelectedId].bActualSelected && GetDataModel()->sel_points[iSelectedId].bDesignDefined)
		{
			GetDataModel()->sel_points[iSelectedId].bActualSelected = false;
			GetDataModel()->sel_points[iSelectedId].bDesignSelected = true;
			//sActDes = L"a/D";
			sActDes = sActualNonSelected + L"/" + sDesignSelected;
		}
		else if(GetDataModel()->sel_points[iSelectedId].bDesignSelected && GetDataModel()->sel_points[iSelectedId].bActualDefined)
		{
			GetDataModel()->sel_points[iSelectedId].bActualSelected = true;
			GetDataModel()->sel_points[iSelectedId].bDesignSelected = false;
			sActDes = sActualSelected + L"/" + sDesignNonSelected;
			//sActDes = L"A/d";
		}
		sTemp = L" ";
		sSelected = sTemp;
		sLine = sPointNo + L" " + sPoint + L" " + sActDes + sSelected;

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) iSelectedId, sLine));
		remove_point_table(iSelectedId+1);
	}
	else
	{
		if(m_iSelectedCount >= 	GetDataModel()->m_iMaxPoint)
		{	
			GUI::DesktopC::Instance()->MessageShow(m_strMaxPointSelected);
		}
		else
		{
			sTemp = L"*";	
			sSelected = sTemp;
			GetDataModel()->sel_points[iSelectedId].bPointSelected = true;
			//poMultiColCtrl->SetCellStr(CI_Selected,iSelectedId,sTemp);
			sLine = sPointNo + L" " + sPoint + L" " + sActDes + sSelected;

			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) iSelectedId, sLine));
	
			add_point_table(iSelectedId+1, GetDataModel()->sel_points[iSelectedId].bActualSelected ? true:false);	
		}
	}


}    
// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void SelectMultiPointsDialog::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

	StringC sSelected = L" ";
	//StringC sActualDesign = L"a/D";
	StringC sActualDesign = sActualNonSelected + L"/" + sDesignSelected;
	StringC sPoint;
	StringC sPointNo;
	StringC sLine;

	short iDef =  GetDataModel()->m_iDef;
	//BeginDraw(); 
	char temp[20];
	
	for(short i=0; i < GetDataModel()->m_iPointsCount; i++)
	{
			
		 if(iDef == BOTH)
		 {
			GetDataModel()->sel_points[i].bDesignSelected = (GetDataModel()->sel_points[i].bDesignDefined) ? true : false;
			GetDataModel()->sel_points[i].bActualSelected = (GetDataModel()->sel_points[i].bActualDefined) ? true : false;
		 }
		 else if(GetDataModel()->sel_points[i].bActualDefined == 1 && iDef == ACTUAL)
		 {
			GetDataModel()->sel_points[i].bDesignSelected = false;
			GetDataModel()->sel_points[i].bActualSelected = true;
		 }
		 else
		 {
			if(GetDataModel()->sel_points[i].bDesignDefined)
				GetDataModel()->sel_points[i].bDesignSelected = true;
		}
				
		 if(GetDataModel()->sel_points[i].bActualSelected && GetDataModel()->sel_points[i].bActualDefined)
			sActualDesign  = sActualSelected;
		 else if(!GetDataModel()->sel_points[i].bActualSelected && GetDataModel()->sel_points[i].bActualDefined)
			sActualDesign = sActualNonSelected;
		 else
			sActualDesign = L"-";

		sActualDesign += L"/";

		 if(GetDataModel()->sel_points[i].bDesignSelected && GetDataModel()->sel_points[i].bDesignDefined)
			sActualDesign  += sDesignSelected;
		 else if(!GetDataModel()->sel_points[i].bDesignSelected && GetDataModel()->sel_points[i].bDesignDefined)
			sActualDesign += sDesignNonSelected;
		 else
			sActualDesign += L"-";	

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		//wchar_t cTemp[80];

		sPointNo.Format(L"%-3d", i+1);
		sprintf(temp,"%-6.6s",m_pCommon->strbtrim(GetDataModel()->sel_points[i].point_id));
		sPoint = StringC(temp);
		sLine = sPointNo + StringC(L" ") + sPoint + StringC(L" ") + sActualDesign + sSelected;
		
		//sTemp.Format(L"%-3d %-6.6s %s%s",i+1,(const wchar_t*)StringC(sPoint), (const wchar_t*)StringC(sActualDesign),(const wchar_t*)(StringC(sSelected)));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i, sLine));
	}
	//EndDraw();

}
// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::SelectMultiPointsDialog::UpdateData()
{
	//nothing to update
}

// ================================================================================================
// Description: only accept SelectMultiPointsModel objects
// ================================================================================================
bool DCP::SelectMultiPointsDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::SelectMultiPointsModel* pModel = dynamic_cast< DCP::SelectMultiPointsModel* >( pModel );

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

// ================================================================================================
// Description: GetDataModel
// ================================================================================================
DCP::SelectMultiPointsModel* DCP::SelectMultiPointsDialog::GetDataModel() const
{
    return (DCP::SelectMultiPointsModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// Description: remove selected point from table
// ================================================================================================
void DCP::SelectMultiPointsDialog::remove_point_table(short sel)
{
short i,j;

	for(i=0; i < GetDataModel()->m_iPointsCount;i++)
	{
		if(GetDataModel()->nro_table[i][0] == sel)
		{
			for(j=i;j < GetDataModel()->m_iPointsCount-1;j++)
			{
				GetDataModel()->nro_table[i][0] = GetDataModel()->nro_table[j+1][0];
				GetDataModel()->nro_table[i][1] = GetDataModel()->nro_table[j+1][1];
			}

			GetDataModel()->nro_table[GetDataModel()->m_iPointsCount-1][0] = 0;
			GetDataModel()->nro_table[GetDataModel()->m_iPointsCount-1][1] = 0;
		}
		
	}
	m_iSelectedCount--;
}

// ================================================================================================
// Description: adds point number into table
// ================================================================================================
void DCP::SelectMultiPointsDialog::add_point_table(short sel, bool bActual)
{
short i;
	
	for(i=0; i<GetDataModel()->m_iPointsCount;i++)
	{
		if(GetDataModel()->nro_table[i][0] == 0)
		{
			GetDataModel()->nro_table[i][0] = sel;
			
			if(!bActual)
				GetDataModel()->nro_table[i][1] = DESIGN;
			else	
				GetDataModel()->nro_table[i][1] = ACTUAL;
			break;
		}
	}
	m_iSelectedCount++;
}
// ================================================================================================
// ====================================  SelectMultiPointsController=========================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::SelectMultiPointsController::SelectMultiPointsController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    //OnControllerActivatedSetTitleTok( AT_DCP06, T_DCP_3D_FILE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

    // Create a dialog
    m_pDlg = new DCP::SelectMultiPointsDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SELECT_MULTIPOINTS_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_SEL_MULTI_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::SelectMultiPointsController::~SelectMultiPointsController()
{

}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::SelectMultiPointsController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	// set title here bacause it depending on caller...
	DCP::SelectMultiPointsModel* pMod = dynamic_cast< DCP::SelectMultiPointsModel* >( pModel );
	SetTitle(pMod->sTitle);
		
	// Set it to hello world dialog
     return m_pDlg->SetModel( pModel ); 
}

// ================================================================================================
// Description: F6 /CONT
// ================================================================================================
void DCP::SelectMultiPointsController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: React on close of active dialog
// ================================================================================================
void DCP::SelectMultiPointsController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::SelectMultiPointsController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// ====================================  SelectMultiPointsModel==============================
// ================================================================================================

// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::SelectMultiPointsModel::SelectMultiPointsModel()
{
	memset(&sel_points[0],0,sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
	memset(nro_table,0,sizeof(short) * MAX_POINTS_IN_FILE*2);

	m_iMinPoint =1;
	m_iMaxPoint =1;
	m_iPointsCount = 1;
	m_iCurrentPoint = 1;
	m_iDef = BOTH;
	sSelectedFile = L"";
	sTitle = L"";
	sInfo = L"";

}

// ================================================================================================
// Description: destructor
// ================================================================================================
DCP::SelectMultiPointsModel::~SelectMultiPointsModel()
{
}