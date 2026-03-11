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
#include "calc.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/init/Initialization.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/calculation/CalculationDist.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/Logger.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/SelectOnePoint.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/orientation/DefineLine.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/calculation/CalculationView.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/core/MsgBox.hpp>

#include <GUI_Types.hpp>
#include <GUI_Desktop.hpp>
#include <UTL_StringFunctions.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::CalculationDistDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================



// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::CalculationDistDialog::CalculationDistDialog(CalculationDistModel * pCalcDistModel):GUI::ModelHandlerC(),GUI::StandardDialogC()
			,m_p3DFile(0),m_pDistFile(0),m_pRefId(0),m_pTargetId(0),m_pDistId(0),m_pRefType(0),m_pTargetType(0),
			m_pDist(0),m_pNote(0),m_pDataModel(pCalcDistModel),
			m_pRefIdObserver(OBS_METHOD_TO_PARAM0(CalculationDistDialog, OnValueChanged), this),
			m_pTrgtIdObserver(OBS_METHOD_TO_PARAM0(CalculationDistDialog, OnValueChanged), this),
			m_pDistIdObserver(OBS_METHOD_TO_PARAM0(CalculationDistDialog, OnValueChanged), this),
			m_pNoteObserver(OBS_METHOD_TO_PARAM0(CalculationDistDialog, OnValueChanged), this)
{
	//SetTxtApplicationId(AT_DCP06);
}


// ================================================================================================
// Description: destructor
// ================================================================================================

DCP::CalculationDistDialog::~CalculationDistDialog()
{
}
// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::CalculationDistDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();
	// Add fields to dialog
	
	m_p3DFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_p3DFile->SetId(e3DFile);
	m_p3DFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_p3DFile->SetAutoColon(false);
	//m_p3DFile->SetColonPosition(9 * 18);
	// m_p3DFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_p3DFile);

	m_pDistFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistFile->SetId(eDistFile);
	m_pDistFile->SetText(StringC(AT_DCP06,P_DCP_DISTFILE_SK_TOK));
	m_pDistFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDistFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pDistFile->SetAutoColon(false);
	//m_pDistFile->SetColonPosition(9 * 18);
	// m_pDistFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pDistFile); 

	m_pRefType = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRefType->SetId(eRefType);
	m_pRefType->SetText(StringC(AT_DCP06,P_DCP_REFTYPE_SK_TOK));
	m_pRefType->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pRefType->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pRefType->SetAutoColon(false);
	//m_pRefType->SetColonPosition(9 * 18);
	// m_pRefType->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pRefType);

	m_pTargetType = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTargetType->SetId(eTargetType);
	m_pTargetType->SetText(StringC(AT_DCP06,P_DCP_TARGETTYPE_SK_TOK));
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pTargetType->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pTargetType->SetAutoColon(false);
	//m_pTargetType->SetColonPosition(9 * 18);
	// m_pTargetType->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pTargetType);

	m_pRefId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRefId->SetId(eRefId);
	m_pRefId->SetText(StringC(AT_DCP06,P_DCP_REF_ID_TOK));
	m_pRefId->GetStringInputCtrl()->SetCharsCountMax(6);
	m_pRefId->GetStringInputCtrl()->SetEmptyAllowed(true);
	//m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pRefId->SetAutoColon(false);
	//m_pRefId->SetColonPosition(9 * 18);
	// m_pRefId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pRefId);

	m_pTargetId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTargetId->SetId(eTargetId);
	m_pTargetId->SetText(StringC(AT_DCP06,P_DCP_TARGET_ID_TOK));
	m_pTargetId->GetStringInputCtrl()->SetCharsCountMax(6);
	m_pTargetId->GetStringInputCtrl()->SetEmptyAllowed(true);
	//m_pTargetId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pTargetId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pTargetId->SetAutoColon(false);
	//m_pTargetId->SetColonPosition(9 * 18);
	// m_pTargetId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pTargetId);

/*
	m_pRefId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRefId->SetId(eRefId);
	m_pRefId->GetTextCtrl()->SetTextTok(P_DCP_REFTYPE_SK_TOK);
	m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pRefId);

	m_pTargetId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTargetId->SetId(eTargetId);
	m_pTargetId->GetTextCtrl()->SetTextTok(P_DCP_TARGETTYPE_SK_TOK);
	m_pTargetId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pTargetId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pTargetId);
*/
	m_pDistId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistId->SetId(eDistId);
	m_pDistId->SetText(StringC(AT_DCP06,P_DCP_DISTANCE_ID_TOK));
	m_pDistId->GetStringInputCtrl()->SetCharsCountMax(6);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pDistId->SetAutoColon(false);
	//m_pDistId->SetColonPosition(9 * 18);
	// m_pDistId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pDistId);

	m_pDist = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDist->SetId(eDist);
	m_pDist->SetText(StringC(AT_DCP06,P_DCP_DIST_TOK));
	m_pDist->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDist->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pDist->SetAutoColon(false);
	//m_pDist->SetColonPosition(9 * 18);
	// m_pDist->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pDist);

	m_pNote = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNote->SetId(eNote);
	m_pNote->SetText(StringC(AT_DCP06,P_DCP_NOTE_TOK));
	m_pNote->GetStringInputCtrl()->SetCharsCountMax(8);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pNote->SetAutoColon(false);
	//m_pNote->SetColonPosition(9 * 18);
	// m_pNote->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pNote);


	m_pRefIdObserver.Attach(m_pRefId->GetSubject());
	m_pTrgtIdObserver.Attach(m_pTargetId->GetSubject());
	m_pNoteObserver.Attach(m_pNote->GetSubject());
	m_pDistIdObserver.Attach(m_pDistId->GetSubject());


	//SetHelpTok(H_DCP_CALC_DIST_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::CalculationDistDialog::OnDialogActivated()
{
	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::CalculationDistDialog::UpdateData()
{

}


// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::CalculationDistDialog::RefreshControls()
{
	if(m_p3DFile && m_pDistFile && m_pRefId && m_pTargetId && m_pDistId && 	m_pDist && m_pNote && m_pRefType && m_pTargetType)
	{
		// update 3dfile name
		m_p3DFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelected3DFile);

		// update 3dfile name
		m_pDistFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelectedDistFile);

		StringC sRefType;
		StringC sDist;

		if(m_pDataModel->active_reftype == POINT)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_POINT_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->refpoint.point_id));

		}
		else if(m_pDataModel->active_reftype == LINE)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_LINE_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->rline_buff[0].id));
			
			//m_pRefId->
		}

		else if(m_pDataModel->active_reftype == PLANE)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_PLANE_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->rplane_buff[0].id));
		}
		m_pRefType->GetStringInputCtrl()->SetString(sRefType);

		// target
		StringC sTargetType;
		sTargetType.LoadTxt(AT_DCP06,P_DCP_REF_POINT_TOK);
		m_pTargetType->GetStringInputCtrl()->SetString(sTargetType);
		m_pTargetId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->trgtpoint.point_id));

		if(m_pDataModel->iDistanceCalculated)
		{
			sDist.Format(L"%10.*f", GetModel()->m_nDecimals,m_pDataModel->dCalculatedDist);
			sprintf(m_pDataModel->cDist,"%10.*f", GetModel()->m_nDecimals,m_pDataModel->dCalculatedDist);
		}
		else
		{
			sDist = L"-";
			sprintf(m_pDataModel->cDist,"%-s","-");
		}
		m_pDist->GetStringInputCtrl()->SetString(sDist);

		m_pDistId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->cDid));
		
		//UTL::UnicodeToAscii(m_pDataModel->cRefType, sRefType);
		BSS::UTI::BSS_UTI_WCharToAscii(sRefType, m_pDataModel->cRefType);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cRefType);
		
		snprintf(m_pDataModel->cRefId, sizeof(m_pDataModel->cRefId), DCP_POINT_ID_FMT, m_pDataModel->refpoint.point_id);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cRefId);
		
		//UTL::UnicodeToAscii(m_pDataModel->cTrgtType,sTargetType);
		BSS::UTI::BSS_UTI_WCharToAscii(sTargetType, m_pDataModel->cTrgtType);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cTrgtType);

		snprintf(m_pDataModel->cTrgtId, sizeof(m_pDataModel->cTrgtId), DCP_POINT_ID_FMT, m_pDataModel->trgtpoint.point_id);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cTrgtId);
		
		StringC sTemp = m_pNote->GetStringInputCtrl()->GetString();
		//UTL::UnicodeToAscii(m_pDataModel->cNote,sTemp);
		BSS::UTI::BSS_UTI_WCharToAscii(sTemp, m_pDataModel->cNote);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cNote);

	}

}
// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::CalculationDistDialog::OnValueChanged( int unNotifyCode, int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetModel()->iCurrentPoint;
		if(ulParam2 == eRefId)
		{
			StringC sTemp;
			if(m_pRefId->GetStringInputCtrl()->IsEmpty())
				snprintf(m_pDataModel->cRefId, sizeof(m_pDataModel->cRefId), DCP_POINT_ID_FMT, "");	
			else
			{
				sTemp = m_pRefId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				snprintf(m_pDataModel->cRefId, sizeof(m_pDataModel->cRefId), DCP_POINT_ID_FMT, asciiBuffer);
				
			}
		}
		else if(ulParam2 == eTargetId)
		{
			StringC sTemp;
			if(m_pTargetId->GetStringInputCtrl()->IsEmpty())
				snprintf(m_pDataModel->cTrgtId, sizeof(m_pDataModel->cTrgtId), DCP_POINT_ID_FMT, "");	
			else
			{
				sTemp = m_pTargetId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				snprintf(m_pDataModel->cTrgtId, sizeof(m_pDataModel->cTrgtId), DCP_POINT_ID_FMT, asciiBuffer);
				
			}
		}
		else if(ulParam2 == eNote)
		{
			StringC sTemp;
			if(m_pNote->GetStringInputCtrl()->IsEmpty())
				snprintf(m_pDataModel->cRefId, sizeof(m_pDataModel->cRefId), DCP_POINT_ID_FMT, "");	
			else
			{
				sTemp = m_pNote->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				snprintf(m_pDataModel->cNote, sizeof(m_pDataModel->cNote), "%-6.6s", asciiBuffer);
			}
		}
		else if(ulParam2 == eDistId)
		{
			StringC sTemp;
			if(m_pDistId->GetStringInputCtrl()->IsEmpty())
				snprintf(m_pDataModel->cDid, sizeof(m_pDataModel->cDid), "%-6.6s", "");	
			else
			{
				sTemp = m_pDistId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				snprintf(m_pDataModel->cDid, sizeof(m_pDataModel->cDid), "%-6.6s", asciiBuffer);
			}
		}
		RefreshControls();
	}
}
// ================================================================================================
// Description: only accept DCP06 Model objects
// ================================================================================================
bool DCP::CalculationDistDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

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

// ================================================================================================
// Description: only accept DCP06 Model objects
// ================================================================================================
DCP::Model* DCP::CalculationDistDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  CalculationDistController  ===================================
// ================================================================================================

// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::CalculationDistController::CalculationDistController(Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_CALC_DIST_TOK /*C_DCP_APPLICATION_NAME_TOK */));


	// create model 
	m_pDataModel = new CalculationDistModel(m_pModel);
    // Create a dialog
    m_pDlg = new DCP::CalculationDistDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CALC_DIST_DLG, m_pDlg, true );

    // Set the function key
	m_pDataModel->dspMode = CalculationDistModel::eNormal;

	change_function_keys();

} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalculationDistController::~CalculationDistController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::CalculationDistController::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void DCP::CalculationDistController::OnF1Pressed()
{
	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if (m_pDataModel->dspMode == CalculationDistModel::eNormal)
	{
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (!jdb || !jdb->isJobLoaded() || m_pModel->m_currentJobId.empty())
		{
			MsgBox msgBox;
			StringC msg;
			msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgBox.ShowMessageOk(msg);
			return;
		}
		m_pDataModel->sSelected3DFile = StringC(m_pModel->m_currentJobId.c_str());
		m_pDataModel->iPointCount3dfile = jdb->getPointListAsSelectPoints(&m_pDataModel->point_list[0], MAX_POINTS_IN_FILE, ACTUAL);
		m_pDataModel->dspMode = CalculationDistModel::ePointSelected;
		change_function_keys();
		m_pDataModel->active_reftype = POINT;
		SelectOnePointModel* pSelModel = new SelectOnePointModel;
		pSelModel->m_iCurrentPoint = 1;
		pSelModel->m_iDef = ACTUAL;
		pSelModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
		memcpy(&pSelModel->points[0], &m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		if (GetController(SELECT_REF_POINT_CONTROLLER) == nullptr)
			(void)AddController(SELECT_REF_POINT_CONTROLLER, new DCP::SelectOnePointController(m_pDlg->GetModel()));
		(void)GetController(SELECT_REF_POINT_CONTROLLER)->SetModel(pSelModel);
		SetActiveController(SELECT_REF_POINT_CONTROLLER, true);
	}
	else
	{
		// POINT SELECTED
		SelectOnePointModel* pModel = new SelectOnePointModel;
		pModel->m_iCurrentPoint = 1;
		pModel->m_iDef = ACTUAL;
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
		memcpy(&pModel->points[0], &m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS)* MAX_POINTS_IN_FILE);

		if(GetController(SELECT_REF_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_REF_POINT_CONTROLLER, new DCP::SelectOnePointController(m_pDlg->GetModel()) );
		}
		(void)GetController( SELECT_REF_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_REF_POINT_CONTROLLER, true);



		m_pDataModel->dspMode = CalculationDistModel::eNormal;
		change_function_keys();
		m_pDataModel->active_reftype = POINT;
		m_pDlg->RefreshControls();
	}
	/*
    // Update model
    // Set it to hello world dialog
    m_pCalcDistDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
	*/
}

// ================================================================================================
// Description: OnF2Pressed
// ================================================================================================
void DCP::CalculationDistController::OnF2Pressed()
{
	if(m_pDataModel->dspMode == CalculationDistModel::eNormal)
	{
		// SELECT caldisrtfile
		//DCP::DistFileModel* pModel = new DistFileModel;
			if(GetController(DIST_FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( DIST_FILE_CONTROLLER, new DCP::DistFileController(m_pModel) );
			}
			(void)GetController( DIST_FILE_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(DIST_FILE_CONTROLLER, true);
	
	}
	else
	{
		// line SELECTED
		m_pDataModel->dspMode = CalculationDistModel::eNormal;
		change_function_keys();
		m_pDataModel->active_reftype = LINE;
		m_pDlg->RefreshControls();
		
		// define line
	
		// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_DISTANCE;
		pModel->m_iMaxPoint = MAX_POINTS_IN_LINE;
	
		// set info text....
		StringC sInfo;
		sInfo.LoadTxt(AT_DCP06, T_DCP_SELECT_POINTS_TOK);
		sInfo += L"  ";
		sInfo += pModel->sSelectedFile;
		pModel->sInfo = sInfo;

		// set title
		pModel->sTitle = GetTitle();
	
		// set help token
		pModel->helpToken = H_DCP_SEL_MULTI_A_OR_D_TOK;

		if(GetController(SELECT_MULTIPOINTS_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_LINE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

	(void)GetController( SELECT_MULTIPOINTS_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_LINE_CONTROLLER, true);
	
		
	}

}

// ================================================================================================
// Description: OnF3Pressed
// ================================================================================================
void DCP::CalculationDistController::OnF3Pressed()
{
	if(m_pDataModel->dspMode ==CalculationDistModel::eNormal)
	{	
		// plane selected
		m_pDataModel->dspMode =CalculationDistModel::eRef;
	}
	else
	{
		m_pDataModel->active_reftype = PLANE;
		m_pDataModel->dspMode = CalculationDistModel::eNormal;

		// define plane
	
		// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_DISTANCE;
		pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
		pModel->m_iDef = ACTUAL;
	
		// set info text....
		StringC sInfo;
		sInfo.LoadTxt(AT_DCP06, T_DCP_SELECT_POINTS_TOK);
		sInfo += L"  ";
		sInfo += pModel->sSelectedFile;
		pModel->sInfo = sInfo;

		// set title
		pModel->sTitle = GetTitle();
	
		// set help token
		pModel->helpToken = H_DCP_SEL_MULTI_A_OR_D_TOK;

		if(GetController(SELECT_MULTIPOINTS_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_PLANE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

	(void)GetController( SELECT_MULTIPOINTS_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_PLANE_CONTROLLER, true);

	}

	change_function_keys();
	m_pDlg->RefreshControls();

}

// ================================================================================================
// Description: OnF4Pressed
// ================================================================================================
void DCP::CalculationDistController::OnF4Pressed()
{
	// target point
	// POINT SELECTED
	SelectOnePointModel* pModel = new SelectOnePointModel;
	pModel->m_iCurrentPoint = 1;
	pModel->m_iDef = ACTUAL;
	pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	memcpy(&pModel->points[0], &m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS)* MAX_POINTS_IN_FILE);

	if(GetController(SELECT_TARGET_POINT_CONTROLLER) == nullptr)
	{
		(void)AddController( SELECT_TARGET_POINT_CONTROLLER, new DCP::SelectOnePointController(m_pDlg->GetModel()) );
	}
	(void)GetController( SELECT_TARGET_POINT_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_TARGET_POINT_CONTROLLER, true);


}

// ================================================================================================
// Description: OnF5Pressed / SAVE
// ================================================================================================
void DCP::CalculationDistController::OnF5Pressed()
{
		
	if(m_pDataModel->pCdfFileFunc->IsOpen())
	{
		m_pDataModel->save();
	}
	else
	{
		StringC sMsg;
		sMsg.LoadTxt(AT_DCP06,M_DCP_DISTFILE_ISNOT_OPEN_TOK);
		m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
	}
}
// ================================================================================================
// Description: OnSHF5Pressed / VIEW
// ================================================================================================
void DCP::CalculationDistController::OnSHF5Pressed()
{
	if(!m_pDataModel->pCdfFileFunc->IsOpen())
		return;

	if(GetController(VIEWCDF_CONTROLLER) == nullptr)
	{
		(void)AddController( VIEWCDF_CONTROLLER, new DCP::ViewCdfController(m_pDataModel->pCdfFileFunc, m_pModel) );
	}
	(void)GetController( VIEWCDF_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(VIEWCDF_CONTROLLER, true);

}

// ================================================================================================
// Description: OnF6Pressed
// ================================================================================================
void DCP::CalculationDistController::OnF6Pressed()
{
	if(m_pDataModel->dspMode == CalculationDistModel::eNormal)
	{
		(void)Close(EC_KEY_CONT);
	}
	else
	{
		m_pDataModel->dspMode = CalculationDistModel::eNormal;
		change_function_keys();
	}
}

// ================================================================================================
// Description: OnSHF2Pressed
// ================================================================================================
void DCP::CalculationDistController::OnSHF2Pressed()
{
	m_pDataModel->clear_buffers();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::CalculationDistController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================

void DCP::CalculationDistController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == DIST_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->sSelectedDistFile = m_pDlg->GetModel()->sCalcDistFile;	
		m_pDataModel->pCdfFileFunc->setFile(m_pDataModel->sSelectedDistFile);
	}
	// REF POINT
	if (lCtrlID == SELECT_REF_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectOnePointModel* pModel = (DCP::SelectOnePointModel*)GetController(SELECT_REF_POINT_CONTROLLER)->GetModel();
		bool useActual = (pModel->points[pModel->iSelectedNo-1].bActualSelected) != 0;
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded())
		{
			char pid[POINT_ID_BUFF_LEN], bXmea[DCP_COORD_STR_BUFF_LEN], bXdes[DCP_COORD_STR_BUFF_LEN];
			char bYmea[DCP_COORD_STR_BUFF_LEN], bYdes[DCP_COORD_STR_BUFF_LEN];
			char bZmea[DCP_COORD_STR_BUFF_LEN], bZdes[DCP_COORD_STR_BUFF_LEN];
			if (jdb->getPointByIndex(pModel->iSelectedNo, useActual, pid, bXmea, bXdes, bYmea, bYdes, bZmea, bZdes, 0))
			{
				snprintf(m_pDataModel->refpoint.point_id, sizeof(m_pDataModel->refpoint.point_id), DCP_POINT_ID_FMT, pid);
				m_pDataModel->refpoint.x = atof(useActual ? bXmea : bXdes);
				m_pDataModel->refpoint.y = atof(useActual ? bYmea : bYdes);
				m_pDataModel->refpoint.z = atof(useActual ? bZmea : bZdes);
				m_pDataModel->refpoint.sta = (!m_pDataModel->pCommon->strblank(useActual ? bXmea : bXdes) &&
					!m_pDataModel->pCommon->strblank(useActual ? bYmea : bYdes) &&
					!m_pDataModel->pCommon->strblank(useActual ? bZmea : bZdes)) ? 1 : 0;
			}
		}
		m_pDataModel->all_defined();
	}

	// TARGET POINT
	if (lCtrlID == SELECT_TARGET_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectOnePointModel* pModel = (DCP::SelectOnePointModel*)GetController(SELECT_TARGET_POINT_CONTROLLER)->GetModel();
		bool useActual = (pModel->points[pModel->iSelectedNo-1].bActualSelected) != 0;
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded())
		{
			char pid[POINT_ID_BUFF_LEN], bXmea[DCP_COORD_STR_BUFF_LEN], bXdes[DCP_COORD_STR_BUFF_LEN];
			char bYmea[DCP_COORD_STR_BUFF_LEN], bYdes[DCP_COORD_STR_BUFF_LEN];
			char bZmea[DCP_COORD_STR_BUFF_LEN], bZdes[DCP_COORD_STR_BUFF_LEN];
			if (jdb->getPointByIndex(pModel->iSelectedNo, useActual, pid, bXmea, bXdes, bYmea, bYdes, bZmea, bZdes, 0))
			{
				snprintf(m_pDataModel->trgtpoint.point_id, sizeof(m_pDataModel->trgtpoint.point_id), DCP_POINT_ID_FMT, pid);
				m_pDataModel->trgtpoint.x = atof(useActual ? bXmea : bXdes);
				m_pDataModel->trgtpoint.y = atof(useActual ? bYmea : bYdes);
				m_pDataModel->trgtpoint.z = atof(useActual ? bZmea : bZdes);
				m_pDataModel->trgtpoint.sta = (!m_pDataModel->pCommon->strblank(useActual ? bXmea : bXdes) &&
					!m_pDataModel->pCommon->strblank(useActual ? bYmea : bYdes) &&
					!m_pDataModel->pCommon->strblank(useActual ? bZmea : bZdes)) ? 1 : 0;
			}
		}
		m_pDataModel->all_defined();
	}
	
	if (lCtrlID == SELECT_MULTIPOINTS_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*)GetController(SELECT_MULTIPOINTS_LINE_CONTROLLER)->GetModel();
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (!jdb || !jdb->isJobLoaded())
		{
			MsgBox msgBox;
			StringC msg;
			msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgBox.ShowMessageOk(msg);
			return;
		}
		Common common(m_pModel);
		memset(&m_pDataModel->rline_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[DCP_COORD_STR_BUFF_LEN], bYmea1[DCP_COORD_STR_BUFF_LEN], bZmea1[DCP_COORD_STR_BUFF_LEN];
		char bXdes1[DCP_COORD_STR_BUFF_LEN], bYdes1[DCP_COORD_STR_BUFF_LEN], bZdes1[DCP_COORD_STR_BUFF_LEN];
		char pid1[POINT_ID_BUFF_LEN];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_LINE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if (iPno != 0)
			{
				bool useDesign = (pModel->nro_table[i][1] == DESIGN);
				if (!jdb->getPointByIndex((int)iPno, !useDesign, pid1, bXmea1, bXdes1, bYmea1, bYdes1, bZmea1, bZdes1, 0))
					continue;
				if (pModel->nro_table[i][1] == DESIGN)
				{
					des = 1;
					m_pDataModel->rline_buff[0].points[iCount].x = atof(bXdes1);
					m_pDataModel->rline_buff[0].points[iCount].y = atof(bYdes1);
					m_pDataModel->rline_buff[0].points[iCount].z = atof(bZdes1);
					
					if(!m_pDataModel->pCommon->strblank(bXdes1) && !m_pDataModel->pCommon->strblank(bYdes1) &&
						!m_pDataModel->pCommon->strblank(bZdes1) )

						m_pDataModel->rline_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->rline_buff[0].points[iCount].sta = 0;

				}
				else
				{
					act = 1;
					m_pDataModel->rline_buff[0].points[iCount].x = atof(bXmea1);
					m_pDataModel->rline_buff[0].points[iCount].y = atof(bYmea1);
					m_pDataModel->rline_buff[0].points[iCount].z = atof(bZmea1);
					
					if(!m_pDataModel->pCommon->strblank(bXmea1) && !m_pDataModel->pCommon->strblank(bYmea1) &&
						!m_pDataModel->pCommon->strblank(bZmea1) )

						m_pDataModel->rline_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->rline_buff[0].points[iCount].sta = 0;

				}
				iCount++;
			}
			m_pDataModel->REF_ACTDES = (des ==1) ? DESIGN : ACTUAL;
		}
	
		if(GetController(CALC_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_LINE_CONTROLLER, new DCP::CalcLineController(&m_pDataModel->rline_buff[0],ACTUAL, 1) );
		}

		(void)GetController( CALC_LINE_CONTROLLER )->SetModel(m_pDlg->GetModel());
		SetActiveController(CALC_LINE_CONTROLLER, true);
	}

	if (lCtrlID == SELECT_MULTIPOINTS_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*)GetController(SELECT_MULTIPOINTS_PLANE_CONTROLLER)->GetModel();
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (!jdb || !jdb->isJobLoaded())
		{
			MsgBox msgBox;
			StringC msg;
			msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgBox.ShowMessageOk(msg);
			return;
		}
		Common common(m_pModel);
		memset(&m_pDataModel->rplane_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[DCP_COORD_STR_BUFF_LEN], bYmea1[DCP_COORD_STR_BUFF_LEN], bZmea1[DCP_COORD_STR_BUFF_LEN];
		char bXdes1[DCP_COORD_STR_BUFF_LEN], bYdes1[DCP_COORD_STR_BUFF_LEN], bZdes1[DCP_COORD_STR_BUFF_LEN];
		char pid1[POINT_ID_BUFF_LEN];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if (iPno != 0)
			{
				bool useDesign = (pModel->nro_table[i][1] == DESIGN);
				if (!jdb->getPointByIndex((int)iPno, !useDesign, pid1, bXmea1, bXdes1, bYmea1, bYdes1, bZmea1, bZdes1, 0))
					continue;
				if (pModel->nro_table[i][1] == DESIGN)
				{
					des = 1;
					m_pDataModel->rplane_buff[0].points[iCount].x = atof(bXdes1);
					m_pDataModel->rplane_buff[0].points[iCount].y = atof(bYdes1);
					m_pDataModel->rplane_buff[0].points[iCount].z = atof(bZdes1);
					
					if(!m_pDataModel->pCommon->strblank(bXdes1) && !m_pDataModel->pCommon->strblank(bYdes1) &&
						!m_pDataModel->pCommon->strblank(bZdes1) )

						m_pDataModel->rplane_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->rplane_buff[0].points[iCount].sta = 0;

				}
				else
				{
					act = 1;
					m_pDataModel->rplane_buff[0].points[iCount].x = atof(bXmea1);
					m_pDataModel->rplane_buff[0].points[iCount].y = atof(bYmea1);
					m_pDataModel->rplane_buff[0].points[iCount].z = atof(bZmea1);
					
					if(!m_pDataModel->pCommon->strblank(bXmea1) && !m_pDataModel->pCommon->strblank(bYmea1) &&
						!m_pDataModel->pCommon->strblank(bZmea1) )

						m_pDataModel->rplane_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->rplane_buff[0].points[iCount].sta = 0;

				}
				iCount++;
			}
			m_pDataModel->REF_ACTDES = (des ==1) ? DESIGN : ACTUAL;
		}
	
		if(GetController(CALC_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_PLANE_CONTROLLER, new DCP::CalcPlaneontrollerC(&m_pDataModel->rplane_buff[0],ACTUAL, 1) );
		}

		(void)GetController( CALC_PLANE_CONTROLLER )->SetModel(m_pDlg->GetModel());
		SetActiveController(CALC_PLANE_CONTROLLER, true);
	}
	if(lCtrlID == CALC_LINE_CONTROLLER/* && lExitCode == EC_KEY_CONT*/)
	{
		m_pDataModel->all_defined();
	}
	if(lCtrlID == CALC_PLANE_CONTROLLER/* && lExitCode == EC_KEY_CONT*/)
	{
		m_pDataModel->all_defined();
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}
// ================================================================================================
// Description: change_function_keys
// ================================================================================================
void DCP::CalculationDistController::change_function_keys()
{
	// 
	ResetFunctionKeys();
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	 FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	if(m_pDataModel->dspMode == 0) // NORMAL
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06,K_DCP_3DFILE_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_DIST_FILE_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_REF_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_TARGET_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_SAVE_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );

		// SHIFT
		vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
		SetFunctionKey( SHFK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
		SetFunctionKey( SHFK5, vDef );
	}
	else if(m_pDataModel->dspMode == 1) // REF
	{
		vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_PLANE_TOK);
		SetFunctionKey( FK3, vDef );

	}
	else if(m_pDataModel->dspMode == 2) // 
	{

	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

// ================================================================================================
// ====================================  CalculationDistModel=======================================
// ================================================================================================


// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::CalculationDistModel::CalculationDistModel(DCP::Model* pModel): pAdfFileFunc(0),pCommon(0),pMsgBox(0)
{
	active_reftype = POINT;
	dspMode = eNormal;
	sSelected3DFile = L"";
	sSelectedDistFile = L"";
	pAdfFileFunc = new AdfFileFunc(pModel);
	pAdfFileFunc->always_single = 1;
	pCommon = new Common(pModel);
	pCdfFileFunc = new CdfFileFunc(pModel);
	pMsgBox = new MsgBox;
	memset(&point_list[0],0, sizeof(S_SELECT_POINTS)* MAX_POINTS_IN_FILE);

	clear_buffers();

	dCalculatedDist = 0.0;
	iDistanceCalculated = 0;

	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sActualNonSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_NONSELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
	sDesignNonSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_NONSELECTED_TOK);
}
// ================================================================================================
// Description: destructor
// ================================================================================================

void DCP::CalculationDistModel::clear_buffers()
{
	memset(&refpoint, 0, sizeof(S_POINT_BUFF));
	memset(&trgtpoint,0, sizeof(S_POINT_BUFF));
	memset(&rline_buff[0],0, sizeof(S_LINE_BUFF));
	memset(&rplane_buff[0], 0, sizeof(S_PLANE_BUFF));
	cDid[0] = '\0';
	cNote[0] = '\0';
	cDist[0] = '\0';
	cRefId[0] = '\0';
	cRefType[0] = '\0';
	cTrgtId[0] = '\0';
	cTrgtType[0] = '\0';
	dCalculatedDist = 0.0;
	iDistanceCalculated = 0;
	REF_ACTDES = 0;
}
// ================================================================================================
// Description: destructor
// ================================================================================================
DCP::CalculationDistModel::~CalculationDistModel()
{
		if(pAdfFileFunc)
		{
			delete pAdfFileFunc;
			pAdfFileFunc = 0;
		}

		if(pCommon)
		{
			delete pCommon;
			pCommon = 0;
		}
		if(pMsgBox)
		{
			delete pMsgBox;
			pMsgBox = 0;
		}
		if(pCdfFileFunc)
		{
			delete pCdfFileFunc;
			pCdfFileFunc = 0;
		}
}
// ================================================================================================
// Description: destructor
// ================================================================================================
short DCP::CalculationDistModel::all_defined()
{
	if(is_active_reference_defined() == 1 && is_active_target_defined() == 1)
	{
		calc_distance();
		return 1;
	}
	else
	{
		dCalculatedDist = 0.0;
		iDistanceCalculated = 0;
	}
	return 0;
}

// ================================================================================================
// Description: destructor
// ================================================================================================
short DCP::CalculationDistModel::is_active_reference_defined()
{
short ret=0;

	// ***************************************
	// ***************************************
	if(active_reftype == POINT)
	{
		if(refpoint.sta > 0 ||
		   refpoint.dsta > 0)
		{
			ret = 1;
		}
	}
	// ***************************************
	// ***************************************
	else if(active_reftype == LINE)
	{
		if(rline_buff[0].calc == 1 ||
		   rline_buff[0].des_calc == 1)
		{
			ret = 1;
		}
	}

	// ***************************************
	// ***************************************
	else if(active_reftype == PLANE)
	{
		if(rplane_buff[0].calc == 1 ||
			rplane_buff[0].des_calc == 1)
		{
			ret = 1;
		}
	}

	return ret;
}

// ************************************************************************
//
// ************************************************************************
short DCP::CalculationDistModel::is_active_target_defined()
{
short ret=1;

	if(trgtpoint.sta == 0 &&
	   trgtpoint.dsta == 0)
	{
	  ret = 0;
	}
	return ret;
}

// ************************************************************************
//
// ************************************************************************
short DCP::CalculationDistModel::calc_distance()
{
short ret = 0;
//double dist;
struct ams_vector m;
struct ams_vector wpoint;
struct line   wline;
struct plane  wplane;

	iDistanceCalculated = 0;

	if(trgtpoint.sta == 0 &&
	   trgtpoint.dsta == 0	)
	{
		StringC sMsg;
		sMsg.LoadTxt(AT_DCP06,M_DCP_DEFINE_TARGET_POINT_TOK);
		pMsgBox->ShowMessageOk(sMsg);
		return 1;
	}
	
	// ***************************************
	// ***************************************
	if(active_reftype == POINT)
	{
		if(refpoint.sta > 0 &&
		   trgtpoint.sta != 0)
		{
			/********************************
				 Actual Values
			********************************/
			m.x = trgtpoint.x;
			m.y = trgtpoint.y;
			m.z = trgtpoint.z;

			wpoint.x = refpoint.x;
			wpoint.y = refpoint.y;
			wpoint.z = refpoint.z;

			/* Calculate distance */
			dCalculatedDist = calc_point_dist_from_point(&m, &wpoint);
			iDistanceCalculated = 1;
			//sprintf(bDist,"%10.*f", get_DECIMALS(),dist);
			ret = 1;
		}
		else
		{
		}
	}

	// ***************************************
	// ***************************************
	else if(active_reftype == LINE)
	{
		if(rline_buff[0].calc == 1 &&
		   trgtpoint.sta != 0)
		{
			m.x = trgtpoint.x;
			m.y = trgtpoint.y;
			m.z = trgtpoint.z;

			wline.px = rline_buff[0].px;
			wline.py = rline_buff[0].py;
			wline.pz = rline_buff[0].pz;

			wline.ux = rline_buff[0].ux;
			wline.uy = rline_buff[0].uy;
			wline.uz = rline_buff[0].uz;

			/* Calculate distance */
			dCalculatedDist = calc_point_dist_from_line(&m, &wline);
			iDistanceCalculated = 1;
			//sprintf(bDist,"%10.*f", get_DECIMALS(),dist);
			ret = 1;
		}
		else
		{
		}
	}

	// ***************************************
	// ***************************************

	else if(active_reftype == PLANE)
	{
		if(rplane_buff[0].calc == 1 &&
		   trgtpoint.sta != 0)
		{
			m.x = trgtpoint.x;
			m.y = trgtpoint.y;
			m.z = trgtpoint.z;

			wplane.px = rplane_buff[0].px;
			wplane.py = rplane_buff[0].py;
			wplane.pz = rplane_buff[0].pz;

			wplane.nx = rplane_buff[0].nx;
			wplane.ny = rplane_buff[0].ny;
			wplane.nz = rplane_buff[0].nz;

			/* Calculate distance */
			dCalculatedDist = calc_point_dist_from_plane(&m, &wplane);
			iDistanceCalculated = 1;
			//sprintf(bDist,"%10.*f", get_DECIMALS(),dist);
			ret = 1;
		}
		else
		{	
		}
	}
	else
	{
	}
	if(ret==1)
	{
		pCdfFileFunc->get_next_id(cDid);
		pCommon->strbtrim(cDid);
		//strbtrim(bDid);
		//MMI_UpdateField(DialogId, 13, bDid, TRUE);	
	}
	//MMI_UpdateField(DialogId, 14, bDist, TRUE);
	return ret;
}

// *******************************************************
//
// *******************************************************
void DCP::CalculationDistModel::save()
{
//char strDist[20];
char strRefType[20], strTrgtType[20];
//char strRef[15],strTrgt[15];
//char strDistId[15];
//char strNote[15];

	if(is_active_target_defined() == 1 && is_active_reference_defined() == 1)
	{
	/*	StringC sTemp;
		sTemp = m_pDlg->m_pDist->GetStringInputCtrl()->GetString();
		UTL::UnicodeToAscii(temp, strDist);
		pCommon.strbtrim(strDist);
		
		sTemp = m_pDlg->m_pDistId->GetStringInputCtrl()->GetString();
		UTL::UnicodeToAscii(temp, strDistId);
		pCommon->strbtrim(strDistId);
		
		sTemp = m_pDlg->m_pNote->GetStringInputCtrl()->GetString();
		UTL::UnicodeToAscii(temp, strNote);
		pCommon->strbtrim(strNote);
	*/
		/*
		if(!pCommon->strblank(bDist))
			sprintf(strDist,"%-10.10s", bDist);
		else
			sprintf(strDist,"%-10.10s", " ");
		*/
		/*
		switch(active_reftype)
		{
			case POINT: 
					sprintf(strRef,"%-s", refpoint.point_id);
					sprintf(strRefType,"%-8s/%c",bRefType,(REF_ACTDES==ACTUAL) ? actual_selected[0]:design_selected[0] );
					break;

			case LINE:
					sprintf(strRef,"%-s", rline_buff[0].id);
					sprintf(strRefType,"%-8s/%c",bRefType,(REF_ACTDES==ACTUAL) ? actual_selected[0] :design_selected[0]);
					break;

			case PLANE:
					sprintf(strRef,"%-s", rplane_buff[0].id);
					sprintf(strRefType,"%-8s/%c",bRefType,(REF_ACTDES==ACTUAL) ? actual_selected[0] :design_selected[0]);
					break;
		}
		*/
		sprintf(strRefType,"%-8s/%c",cRefType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] :sDesignSelected[0]);

		//sprintf(strTrgt,"%-s",trgtpoint.point_id);
		sprintf(strTrgtType,"%-8s/%c",cTrgtType,(TRG_ACTDES==ACTUAL) ?sActualSelected[0] /*'A'*/:sDesignSelected[0] /*'D'*/);
		//PrintLn("bDid=%-s",bDid);
		if(pCdfFileFunc->save_calcdist_to_file(cDist, cRefId,strRefType,cTrgtId,strTrgtType, cDid,cNote,1))
			 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_DISTANCE_SAVED_TOK));
		/*
		if(CalcDistFile_.open == RC_OK)
		{
			sprintf(bCdf,"%-8.8s(%d)",get_fname(CalcDistFile_.name),CalcDistFile_.points);
			MMI_UpdateField(DialogId, 3, bCdf, TRUE);
		}*/
	}
}
