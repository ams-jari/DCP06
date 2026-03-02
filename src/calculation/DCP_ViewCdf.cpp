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
#include <dcp06/calculation/DCP_ViewCdf.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/calculation/DCP_EditCalcDist.hpp>
#include <dcp06/core/DCP_Common.hpp>

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

//OBS_IMPLEMENT_EXECUTE(DCP::DCP06ViewCdfDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP06ViewCdfDlgC::DCP06ViewCdfDlgC(DCP::CdfFileFunc *pFileFunc,DCP06ModelC* pDCP06Model):poMultiColCtrl(NULL),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(DCP06ViewCdfDlgC, OnChanged), this),
	m_pFileFunc(pFileFunc),m_iSelectedCount(0)
{
	//SetTxtApplicationId(AT_DCP06);


	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sActualNonSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_NONSELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
	sDesignNonSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_NONSELECTED_TOK);

	m_strMaxPointSelected.LoadTxt(AT_DCP06,L_DCP_MAX_POINTS_SELECTED_TOK); 

	m_pCommon = new DCP06CommonC(pDCP06Model);
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP06ViewCdfDlgC::~DCP06ViewCdfDlgC()
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
void DCP06ViewCdfDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	//GUI::ListConfigColsC oConfigCols1;
	/*
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_DistId));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Dist));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Note));
	*/

	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NUMBER_TOK),CI_No));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_DISTANCE_ID_TOK),CI_DistId));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_DIST_TOK),CI_Dist));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_NOTE_TOK),CI_Note));

	poMultiColCtrl->AddColSelection(CI_No,CI_DistId,CI_Dist,CI_Note);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
	
	// infotext
	/*
	StringC sTemp1;
	sTemp1.LoadTxt(AT_DCP06,P_DCP_DISTANCE_FILE_TOK);
	sTemp1 += L":";
	sTemp1 =  m_pDataModel->sSelectedDistFile;

	USER_APP_VERIFY(poMultiColCtrl->AddColStr(sTemp1,CI_Point));

	// set column config active
	USER_APP_VERIFY(poMultiColCtrl->SetConfig(0));
	AddTable(poMultiColCtrl);
	*/
	// help
	//SetHelpTok(H_DCP_CALC_DIST_VIEW_TOK, 0);

	//m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());
}

// ================================================================================================
// Description: OnChanged
// ================================================================================================
/*
void DCP06ViewCdfDlgC::OnChanged(int unNotifyCode, int ulParam2)
{
	//if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_ACCEPTED)
	{
	}
	else if(unNotifyCode == GUI::NC_ONLISTMULTICOL_SELECTION_CHANGED)
	{

	}
}
*/
// ================================================================================================
// Description: RefreshControls
// ================================================================================================
void DCP06ViewCdfDlgC::RefreshControls()
{
	if(poMultiColCtrl)
	{
		//BeginDraw(); 
		char temp[20];
		StringC sNo;
		StringC sDistId;
		StringC sDist;
		StringC sNote;

		poMultiColCtrl->DestroyAll();

		for(short i=0; i < m_pFileFunc->getPointsCount(); i++)
		{
			sprintf(temp,"%d",i+1);
			sNo = StringC(temp);

			m_pFileFunc->calcdist_pnt(i+1);

			sDistId = StringC(m_pFileFunc->id);
			sDist = StringC(m_pFileFunc->dist);
			sNote = StringC(m_pFileFunc->note);

			USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, sNo));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_DistId, (short) i, sDistId));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Dist, (short) i, sDist));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Note, (short) i, sNote));


			//sTemp.Format(L"%-3d %-6.6s %s%s",i+1,(const wchar_t*)StringC(sPoint), (const wchar_t*)StringC(sActualDesign),(const wchar_t*)(StringC(sSelected)));
			//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Point, (short) i, sLine));
		}
		//EndDraw();
	}
}


// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP06ViewCdfDlgC::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();


	

	StringC sSelected = L" ";
	//StringC sActualDesign = L"a/D";
	StringC sActualDesign = sActualNonSelected + L"/" + sDesignSelected;
	StringC sPoint;
	StringC sPointNo;
	StringC sLine;

	//BeginDraw(); 
	
	for(short i=0; i < m_pFileFunc->getPointsCount(); i++)
	{
			
		
		//sTemp.Format(L"%-3d %-6.6s %s%s",i+1,(const wchar_t*)StringC(sPoint), (const wchar_t*)StringC(sActualDesign),(const wchar_t*)(StringC(sSelected)));
		//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Point, (short) i, sLine));
	}
	//EndDraw();

}
// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::DCP06ViewCdfDlgC::UpdateData()
{
	//nothing to update
}


// ================================================================================================
// Description: DeletePoint
// ================================================================================================
bool DCP::DCP06ViewCdfDlgC::DeletePoint()
{
	bool ret = false;
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	if(m_pFileFunc->delete_point_from_cdf(iSelectedId))
	{
		RefreshControls();
		ret = true;
	}
	return ret;
}

bool DCP::DCP06ViewCdfDlgC::GetSelectedData(StringC& sDistId, StringC& sRefId, StringC& sNote)
{
	bool ret = false;
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	
	if(iSelectedId != -1)
	{
		m_pFileFunc->calcdist_pnt(iSelectedId+1);	
		
		UTL::Trim(m_pFileFunc->id);	
		sDistId = StringC(m_pFileFunc->id);
		UTL::Trim(m_pFileFunc->ref);
		sRefId = StringC(m_pFileFunc->ref);
		UTL::Trim(m_pFileFunc->note);
		sNote = StringC(m_pFileFunc->note);
		
		ret = true;
	}
	
	return ret;
}

// ================================================================================================
// Description: only accept DCP06ViewCdfModelC objects
// ================================================================================================
bool DCP::DCP06ViewCdfDlgC::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: GetDataModel
// ================================================================================================
DCP::DCP06ModelC* DCP::DCP06ViewCdfDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  DCP06ViewCdfControllerC=========================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06ViewCdfControllerC::DCP06ViewCdfControllerC(DCP::CdfFileFunc *pFileFunc, DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pFileFunc(pFileFunc),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    //OnControllerActivatedSetTitleTok( AT_DCP06, T_DCP_3D_FILE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

	//SetTitleTok( AT_DCP06, T_DCP_CALC_DIST_VIEW_TOK);

	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_CALC_DIST_VIEW_TOK);
	sTitle += L" (";
	sTitle += StringC(m_pFileFunc->getFileName());
	sTitle += L")";
	SetTitle(sTitle);

	// Create a dialog
    m_pDlg = new DCP::DCP06ViewCdfDlgC(m_pFileFunc, m_pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( VIEWCDF_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_EDIT_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06ViewCdfControllerC::~DCP06ViewCdfControllerC()
{

}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06ViewCdfControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	// set title here bacause it depending on caller...
	DCP::DCP06ModelC* pMod = dynamic_cast< DCP::DCP06ModelC* >( pModel );
	//SetTitleStr(pMod->sTitle);
		
	// Set it to hello world dialog
     return m_pDlg->SetModel( pModel ); 
}

// ================================================================================================
// Description: F1 / EDIT
// ================================================================================================
void DCP::DCP06ViewCdfControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	StringC sDistId, sRefId, sNote;
	
	if(m_pDlg->GetSelectedData(sDistId,sRefId,sNote) == false)
		return;

	DCP::DCP06EditCalcDistModelC* pModel = new DCP06EditCalcDistModelC;

	pModel->sDistId = sDistId;
	pModel->sRefId =  sRefId;
	pModel->sNote =	  sNote;	

	if(GetController(VIEWCDF_EDIT_CONTROLLER) == NULL)
	{
		(void)AddController( VIEWCDF_EDIT_CONTROLLER, new DCP::DCP06EditCalcDistControllerC(m_pDlg->GetDCP06Model() ));
	}
	(void)GetController( VIEWCDF_EDIT_CONTROLLER )->SetModel(pModel);
	SetActiveController(VIEWCDF_EDIT_CONTROLLER, true);


    // Update model
    // Set it to dialog
    //m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    //(void)Close(EC_KEY_CONT);
}
// ================================================================================================
// Description: SHF2 / DELETE
// ================================================================================================
void DCP::DCP06ViewCdfControllerC::OnSHF5Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	m_pDlg->DeletePoint();

    // Update model
    // Set it to dialog
    //m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    //(void)Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: F6 /CONT
// ================================================================================================
void DCP::DCP06ViewCdfControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
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
void DCP::DCP06ViewCdfControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06ViewCdfControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{

	if(lCtrlID == VIEWCDF_EDIT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06EditCalcDistModelC* pModel = (DCP::DCP06EditCalcDistModelC*) GetController( VIEWCDF_EDIT_CONTROLLER )->GetModel();	
		
		DCP06CommonC pCommon(m_pDCP06Model);
		char temp[100];
		
		

		//UTL::UnicodeToAscii(temp, 6, pModel->sDistId);
		BSS::UTI::BSS_UTI_WCharToAscii(pModel->sDistId, temp,6);
		pCommon.strbtrim(temp);
		sprintf(m_pFileFunc->id,"%-s", temp);

		//UTL::UnicodeToAscii(temp, 6, pModel->sRefId);
		BSS::UTI::BSS_UTI_WCharToAscii(pModel->sRefId, temp,6);
		pCommon.strbtrim(temp);
		sprintf(m_pFileFunc->ref,"%-s", temp);

		//UTL::UnicodeToAscii(temp, 8, pModel->sNote);
		BSS::UTI::BSS_UTI_WCharToAscii(pModel->sNote, temp,6);
		pCommon.strbtrim(temp);
		sprintf(m_pFileFunc->note,"%-s", temp);
		
		m_pFileFunc->cdf_save_pnt();
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}
