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
#include <dcp06/calculation/DCP_ViewAgf.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/calculation/DCP_EditCalcAngle.hpp>
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
DCP06ViewAgfDlgC::DCP06ViewAgfDlgC(DCP::AgfFileFunc *pFileFunc,DCP06ModelC* pDCP06Model):poMultiColCtrl(NULL),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(DCP06ViewCdfDlgC, OnChanged), this),
	m_pFileFunc(pFileFunc),m_iSelectedCount(0)
{
	//SetTxtApplicationId(AT_DCP06);

	m_pCommon = new DCP06CommonC(pDCP06Model);
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP06ViewAgfDlgC::~DCP06ViewAgfDlgC()
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
void DCP06ViewAgfDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	/*
	GUI::ListConfigColsC oConfigCols1;

	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_AngleId));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Angle));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Note));
	*/
	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NUMBER_TOK),CI_No));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_ANGLE_ID_TOK),CI_AngleId));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_ANGLE_TOK),CI_Angle));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_NOTE_TOK),CI_Note));

	poMultiColCtrl->AddColSelection(CI_No,CI_AngleId,CI_Angle,CI_Note);

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
	//SetHelpTok(H_DCP_CALC_ANGLE_VIEW_TOK, 0);

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
void DCP06ViewAgfDlgC::RefreshControls()
{
	if(poMultiColCtrl)
	{
		//BeginDraw(); 
		char temp[20];
		StringC sNo;
		StringC sAngleId;
		StringC sAngle;
		StringC sNote;

		poMultiColCtrl->DestroyAll();

		for(short i=0; i < m_pFileFunc->getPointsCount(); i++)
		{
			sprintf(temp,"%d",i+1);
			sNo = StringC(temp);

			m_pFileFunc->calcdist_pnt(i+1);

			sAngleId = StringC(m_pFileFunc->id);
			sAngle = StringC(m_pFileFunc->dist);
			sNote = StringC(m_pFileFunc->note);

			USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, sNo));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_AngleId, (short) i, sAngleId));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Angle, (short) i, sAngle));
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
void DCP06ViewAgfDlgC::OnDialogActivated()
{
	GUI::TableDialogC::OnDialogActivated();

}
// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::DCP06ViewAgfDlgC::UpdateData()
{
	//nothing to update
}


// ================================================================================================
// Description: DeletePoint
// ================================================================================================
bool DCP::DCP06ViewAgfDlgC::DeletePoint()
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

bool DCP::DCP06ViewAgfDlgC::GetSelectedData(StringC& sDistId, StringC& sRefId, StringC& sTrgtId, StringC& sNote)
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

		UTL::Trim(m_pFileFunc->target);
		sTrgtId = StringC(m_pFileFunc->target);

		UTL::Trim(m_pFileFunc->note);
		sNote = StringC(m_pFileFunc->note);
		
		ret = true;
	}
	
	return ret;
}

// ================================================================================================
// Description: only accept DCP06ViewAgfModelC objects
// ================================================================================================
bool DCP::DCP06ViewAgfDlgC::SetModel( GUI::ModelC* pModel )
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
DCP::DCP06ModelC* DCP::DCP06ViewAgfDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  DCP06ViewAgfControllerC=========================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06ViewAgfControllerC::DCP06ViewAgfControllerC(DCP::AgfFileFunc *pFileFunc,DCP06ModelC* pDCP06Model )
    : m_pDlg( NULL ),m_pFileFunc(pFileFunc), m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    //OnControllerActivatedSetTitleTok( AT_DCP06, T_DCP_3D_FILE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);

	//SetTitleTok( AT_DCP06, T_DCP_CALC_DIST_VIEW_TOK);

	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_CALC_ANGLE_VIEW_TOK);
	sTitle += L" (";
	sTitle += StringC(m_pFileFunc->getFileName());
	sTitle += L")";
	SetTitle(sTitle);

	// Create a dialog
    m_pDlg = new DCP::DCP06ViewAgfDlgC(m_pFileFunc, pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( VIEWAGF_DLG, m_pDlg, true );

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

DCP::DCP06ViewAgfControllerC::~DCP06ViewAgfControllerC()
{

}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06ViewAgfControllerC::SetModel( GUI::ModelC* pModel )
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
void DCP::DCP06ViewAgfControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	StringC sDistId, sRefId, sNote, sTrgtId;
	
	if(m_pDlg->GetSelectedData(sDistId,sRefId, sTrgtId, sNote) == false)
		return;

	DCP::DCP06EditCalcAngleModelC* pModel = new DCP06EditCalcAngleModelC;

	pModel->sDistId = sDistId;
	pModel->sRefId =  sRefId;
	pModel->sTrgtId = sTrgtId;
	pModel->sNote =	  sNote;	

	if(GetController(VIEWAGF_EDIT_CONTROLLER) == NULL)
	{
		(void)AddController( VIEWAGF_EDIT_CONTROLLER, new DCP::DCP06EditCalcAngleControllerC(m_pDlg->GetDCP06Model() ));
	}
	(void)GetController( VIEWAGF_EDIT_CONTROLLER )->SetModel(pModel);
	SetActiveController(VIEWAGF_EDIT_CONTROLLER, true);


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
void DCP::DCP06ViewAgfControllerC::OnSHF5Pressed()
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
void DCP::DCP06ViewAgfControllerC::OnF6Pressed()
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
void DCP::DCP06ViewAgfControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06ViewAgfControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{

	if(lCtrlID == VIEWAGF_EDIT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06EditCalcAngleModelC* pModel = (DCP::DCP06EditCalcAngleModelC*) GetController( VIEWAGF_EDIT_CONTROLLER )->GetModel();	
		
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

		//UTL::UnicodeToAscii(temp, 6, pModel->sTrgtId);
		BSS::UTI::BSS_UTI_WCharToAscii(pModel->sTrgtId, temp,6);
		pCommon.strbtrim(temp);
		sprintf(m_pFileFunc->target,"%-s", temp);

		//UTL::UnicodeToAscii(temp, 8, pModel->sNote);
		BSS::UTI::BSS_UTI_WCharToAscii(pModel->sNote, temp,6);
		pCommon.strbtrim(temp);
		sprintf(m_pFileFunc->note,"%-s", temp);
		
		m_pFileFunc->cdf_save_pnt();
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}
