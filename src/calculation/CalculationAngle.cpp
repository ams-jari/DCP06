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
#include "calc.h"
#include <dcp06/core/Model.hpp>

#include <dcp06/calculation/CalculationAngle.hpp>
#include <dcp06/file/AngleFile.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/SelectOnePoint.hpp>

#include <dcp06/calculation/CalculationView.hpp>

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
OBS_IMPLEMENT_EXECUTE(DCP::CalculationAngleDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================



// ================================================================================================
// ====================================  CalculationAngleDialog  ======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CalculationAngleDialog::CalculationAngleDialog(CalculationAngleModel * pCalcAngleModel):GUI::ModelHandlerC(),GUI::StandardDialogC()
			,m_p3DFile(0),m_pAngleFile(0),m_pRefId(0),m_pTargetId(0),m_pAngleId(0),
			m_pAngle(0),m_pNote(0),m_pDataModel(pCalcAngleModel),m_pRefType(0),m_pTargetType(0),
			m_pRefIdObserver(OBS_METHOD_TO_PARAM0(CalculationAngleDialog, OnValueChanged), this),
			m_pTrgtIdObserver(OBS_METHOD_TO_PARAM0(CalculationAngleDialog, OnValueChanged), this),
			m_pAngleIdObserver(OBS_METHOD_TO_PARAM0(CalculationAngleDialog, OnValueChanged), this),
			m_pNoteObserver(OBS_METHOD_TO_PARAM0(CalculationAngleDialog, OnValueChanged), this)

{
	//SetTxtApplicationId(AT_DCP06);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalculationAngleDialog::~CalculationAngleDialog()
{

}
// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::CalculationAngleDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	m_p3DFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_p3DFile->SetId(e3DFile);
	m_p3DFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_p3DFile->SetAutoColon(false);
	//m_p3DFile->SetColonPosition(9 * 18);
	// m_p3DFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_p3DFile);

	m_pAngleFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pAngleFile->SetId(eAngleFile);
	m_pAngleFile->SetText(StringC(AT_DCP06,P_DCP_ANGLEFILE_SK_TOK));
	m_pAngleFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pAngleFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pAngleFile->SetAutoColon(false);
	//m_pAngleFile->SetColonPosition(9 * 18);
	// m_pAngleFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);CAPTIVATE
	AddCtrl(m_pAngleFile);
	
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
	//m_pTargetType->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
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
	//m_pRefId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
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

	m_pAngleId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pAngleId->SetId(eAngleId);
	m_pAngleId->SetText(StringC(AT_DCP06,P_DCP_ANGLE_ID_TOK));
	m_pAngleId->GetStringInputCtrl()->SetCharsCountMax(6);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pAngleId->SetAutoColon(false);
	//m_pAngleId->SetColonPosition(9 * 18);
	//m_pAngleId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pAngleId);

	m_pAngle = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pAngle->SetId(eAngle);
	m_pAngle->SetText(StringC(AT_DCP06,P_DCP_ANGLE_TOK));
	m_pAngle->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pAngle->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pAngle->SetAutoColon(false);
	//m_pAngle->SetColonPosition(9 * 18);
	//m_pAngle->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);CAPTIVATE
	AddCtrl(m_pAngle);

	m_pNote = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNote->SetId(eNote);
	m_pNote->SetText(StringC(AT_DCP06,P_DCP_NOTE_TOK));
	m_pNote->GetStringInputCtrl()->SetCharsCountMax(8);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pDistId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pNote->SetAutoColon(false);
	//m_pNote->SetColonPosition(9 * 18);
	//m_pNote->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pNote);

	m_pRefIdObserver.Attach(m_pRefId->GetSubject());
	m_pTrgtIdObserver.Attach(m_pTargetId->GetSubject());
	m_pNoteObserver.Attach(m_pNote->GetSubject());
	m_pAngleIdObserver.Attach(m_pAngleId->GetSubject());

	//SetHelpTok(H_DCP_CALC_ANGLE_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::CalculationAngleDialog::OnDialogActivated()
{
	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::CalculationAngleDialog::UpdateData()
{

}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::CalculationAngleDialog::RefreshControls()
{
	if(m_p3DFile &&  m_pAngleFile && m_pRefId && m_pTargetId && m_pAngleId && m_pAngle && 
		m_pNote && m_pRefType && m_pTargetType)
	{
		// update 3dfile name
		
		m_p3DFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelected3DFile);

		// update 3dfile name
		m_pAngleFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelectedAngleFile);

		StringC sRefType;
		StringC sTargetType;
		StringC sAngle;

		if(m_pDataModel->active_reftype == POINT)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_POINT_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->refpoint.point_id));

		}
		else if(m_pDataModel->active_reftype == LINE)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_LINE_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->rline_buff[0].id));

		}

		else if(m_pDataModel->active_reftype == PLANE)
		{
			sRefType.LoadTxt(AT_DCP06,P_DCP_REF_PLANE_TOK);
			m_pRefId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->rplane_buff[0].id));

		}
		m_pRefType->GetStringInputCtrl()->SetString(sRefType);

		
		
		// target
		if(m_pDataModel->active_reftype == LINE)
		{
			if(m_pDataModel->active_targettype == PLANE)
			{
				return;
			}
		}
		else if(m_pDataModel->active_reftype== PLANE)
		{
			if(m_pDataModel->active_targettype !=PLANE)
			{
				return;
			}
		}
		if(m_pDataModel->active_targettype == POINT)
		{
			sTargetType.LoadTxt(AT_DCP06,P_DCP_REF_POINT_TOK);
			m_pTargetId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->trgtpoint.point_id));
		}
		else if(m_pDataModel->active_targettype == LINE)
		{
				sTargetType.LoadTxt(AT_DCP06,P_DCP_REF_LINE_TOK);
				m_pTargetId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->trgt_line_buff[0].id));

		}

		else if(m_pDataModel->active_targettype == PLANE)
		{
			sTargetType.LoadTxt(AT_DCP06,P_DCP_REF_PLANE_TOK);
			m_pTargetId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->trgt_plane_buff[0].id));
		}
		m_pTargetType->GetStringInputCtrl()->SetString(sTargetType);

		if(m_pDataModel->iAngleCalculated)
		{
			sAngle.Format(L"%10.*f", 6/*GetModel()->m_nDecimals*/,m_pDataModel->dCalculatedAngle);
			sprintf(m_pDataModel->cAngle,"%10.*f", 6/*GetModel()->m_nDecimals*/,m_pDataModel->dCalculatedAngle);
		}
		else
		{
			sAngle = L"-";
			sprintf(m_pDataModel->cAngle,"%-s","-");
		}
		m_pAngle->GetStringInputCtrl()->SetString(sAngle);

		m_pAngleId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->cAid));

		//UTL::UnicodeToAscii(m_pDataModel->cRefType, sRefType);
		BSS::UTI::BSS_UTI_WCharToAscii(sRefType, m_pDataModel->cRefType);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cRefType);
		
		sprintf(m_pDataModel->cRefId,"%-6.6s",m_pDataModel->refpoint.point_id);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cRefId);
		
		//UTL::UnicodeToAscii(m_pDataModel->cTrgtType,sTargetType);
		BSS::UTI::BSS_UTI_WCharToAscii(sTargetType, m_pDataModel->cTrgtType);
		m_pDataModel->pCommon->strbtrim(m_pDataModel->cTrgtType);

		sprintf(m_pDataModel->cTrgtId,"%-6.6s",m_pDataModel->trgtpoint.point_id);
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
void DCP::CalculationAngleDialog::OnValueChanged(int unNotifyCode, int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetModel()->iCurrentPoint;
		if(ulParam2 == eRefId)
		{
			StringC sTemp;
			if(m_pRefId->GetStringInputCtrl()->IsEmpty())
				sprintf(m_pDataModel->cRefId,"%-6.6s","");	
			else
			{
				sTemp = m_pRefId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				sprintf(m_pDataModel->cRefId,"%-6.6s",asciiBuffer);
				
			}
		}
		else if(ulParam2 == eTargetId)
		{
			StringC sTemp;
			if(m_pTargetId->GetStringInputCtrl()->IsEmpty())
				sprintf(m_pDataModel->cRefId,"%-6.6s","");	
			else
			{
				sTemp = m_pTargetId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				sprintf(m_pDataModel->cTrgtId,"%-6.6s",asciiBuffer);
				
			}
		}
		else if(ulParam2 == eNote)
		{
			StringC sTemp;
			if(m_pNote->GetStringInputCtrl()->IsEmpty())
				sprintf(m_pDataModel->cNote,"%-6.6s","");	
			else
			{
				sTemp = m_pNote->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				sprintf(m_pDataModel->cNote,"%-6.6s",asciiBuffer);
			}
		}
		else if(ulParam2 == eAngleId)
		{
			StringC sTemp;
			if(m_pAngleId->GetStringInputCtrl()->IsEmpty())
				sprintf(m_pDataModel->cAid,"%-6.6s","");	
			else
			{
				sTemp = m_pAngleId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char asciiBuffer[STRING_BUFFER_SMALL];
				BSS::UTI::BSS_UTI_WCharToAscii(sTemp, asciiBuffer);
				m_pDataModel->pCommon->strbtrim(asciiBuffer);
				sprintf(m_pDataModel->cAid,"%-6.6s",asciiBuffer);
			}
		}
		RefreshControls();
	}
}
// ================================================================================================
// Description:  SetModel
// ================================================================================================
bool DCP::CalculationAngleDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pModel = dynamic_cast< DCP::Model* >( pModel );

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
// Description:  GetModel
// ================================================================================================
DCP::Model* DCP::CalculationAngleDialog::GetModel() const
{
    return (DCP::Model*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}



// ================================================================================================
// ====================================  CalculationAngleController=================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CalculationAngleController::CalculationAngleController(Model* pModel)
    : m_pDlg( nullptr ), m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_CALC_ANGLE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model 
	m_pDataModel = new CalculationAngleModel(pModel);
    
	// Create a dialog
    m_pDlg = new DCP::CalculationAngleDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CALC_ANGLE_DLG, m_pDlg, true );

	// Set the function key
	m_pDataModel->dspMode = CalculationAngleModel::eNormal;

    // Set the function key
	change_function_keys();
	
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalculationAngleController::~CalculationAngleController()
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
bool DCP::CalculationAngleController::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: F1
// ================================================================================================
void DCP::CalculationAngleController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

  
	if(m_pDataModel->dspMode == CalculationAngleModel::eNormal)
	{
		// SELECT FILE
		DCP::SelectFileModel* pModel = new SelectFileModel;
		if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle,m_pModel ) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);


	}
	else if (m_pDataModel->dspMode == CalculationAngleModel::eRef)
	{
		// POINT SELECTED
		/*
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;
		change_function_keys();
		m_pDataModel->active_reftype = POINT;
		m_pDlg->RefreshControls();
		*/

		m_pDataModel->dspMode = CalculationAngleModel::eNormal;
		change_function_keys();
		m_pDataModel->active_reftype = POINT;

	
	}
	else
	{
		m_pDataModel->active_targettype = POINT;
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;

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




		m_pDlg->RefreshControls();

	}
	change_function_keys();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F2
// ================================================================================================
void DCP::CalculationAngleController::OnF2Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

  
	if(m_pDataModel->dspMode == CalculationAngleModel::eNormal)
	{
		// SELECT ANGLE FILE
		if(GetController(ANGLE_FILE_CONTROLLER) == nullptr)
		{
			(void)AddController( ANGLE_FILE_CONTROLLER, new DCP::AngleFileController(m_pModel ));
		}
		(void)GetController( ANGLE_FILE_CONTROLLER )->SetModel( m_pDlg->GetModel());
		SetActiveController(ANGLE_FILE_CONTROLLER, true);

	}
	else if (m_pDataModel->dspMode == CalculationAngleModel::eRef)
	{
		// REF LINE
		m_pDataModel->active_reftype = LINE;
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;
	
		// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_LINE;
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

		if(GetController(SELECT_MULTIPOINTS_REF_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_REF_LINE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

	(void)GetController( SELECT_MULTIPOINTS_REF_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_REF_LINE_CONTROLLER, true);

	
	}
	else
	{	
		// TARGET LINE

		m_pDataModel->active_targettype = LINE;
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;

		// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_LINE;
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

		if(GetController(SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

	(void)GetController( SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER, true);

		
	}
	change_function_keys();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F3
// ================================================================================================
void DCP::CalculationAngleController::OnF3Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

  
	if(m_pDataModel->dspMode == CalculationAngleModel::eNormal)
	{
		m_pDataModel->dspMode = CalculationAngleModel::eRef;

	}
	else if (m_pDataModel->dspMode == CalculationAngleModel::eRef)
	{
		// REF PLANE
		m_pDataModel->active_reftype = PLANE;
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;

		// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_PLANE;
		pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
	
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

		if(GetController(SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

		(void)GetController( SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER, true);


	}
	else
	{	
		// TARGET PLANE

		m_pDataModel->active_targettype = PLANE;
		m_pDataModel->dspMode = CalculationAngleModel::eNormal;

				// SELECT MULTIPOINTS
		DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_PLANE;
		pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
	
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

		if(GetController(SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER, new DCP::SelectMultiPointsController(m_pDlg->GetModel()) );
		}

		(void)GetController( SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER, true);

		
	}
	change_function_keys();
	m_pDlg->RefreshControls();

}

// ================================================================================================
// Description: F4
// ================================================================================================
void DCP::CalculationAngleController::OnF4Pressed()
{
	    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
  
	m_pDataModel->dspMode = CalculationAngleModel::eTarget;
	
	change_function_keys();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F5
// ================================================================================================
void DCP::CalculationAngleController::OnF5Pressed()
{
	if(m_pDataModel->dspMode == CalculationAngleModel::eNormal)
	{
		// save results
		if(m_pDataModel->pAgfFileFunc->IsOpen())
		{
			m_pDataModel->save();
		}
		else
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_ANGLEFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
	}
	else
	{

	}
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F5
// ================================================================================================
void DCP::CalculationAngleController::OnF6Pressed()
{
	 if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: SHF2
// ================================================================================================
void DCP::CalculationAngleController::OnSHF2Pressed()
{
	m_pDataModel->clear_buffers();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnSHF5Pressed / VIEW
// ================================================================================================
void DCP::CalculationAngleController::OnSHF5Pressed()
{
	if(!m_pDataModel->pAgfFileFunc->IsOpen())
		return;

	if(GetController(VIEWAGF_CONTROLLER) == nullptr)
	{
		(void)AddController( VIEWAGF_CONTROLLER, new DCP::ViewAgfController(m_pDataModel->pAgfFileFunc, m_pDlg->GetModel()) );
	}
	(void)GetController( VIEWAGF_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(VIEWAGF_CONTROLLER, true);

}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::CalculationAngleController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::CalculationAngleController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		m_pDataModel->sSelected3DFile = pModel->m_strSelectedFile;
		m_pDataModel->pAdfFileFunc->setFile(m_pDataModel->sSelected3DFile);

		// and get point list
		m_pDataModel->iPointCount3dfile = m_pDataModel->pAdfFileFunc->GetPointList(&m_pDataModel->point_list[0], MAX_POINTS_IN_FILE, ACTUAL);
	}
	
	if(lCtrlID == ANGLE_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->sSelectedAngleFile = m_pDlg->GetModel()->sCalcAngleFile;	
		m_pDataModel->pAgfFileFunc->setFile(m_pDataModel->sSelectedAngleFile);
	}


	// TARGET POINT
	if(lCtrlID == SELECT_TARGET_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectOnePointModel* pModel = (DCP::SelectOnePointModel*) GetController( SELECT_TARGET_POINT_CONTROLLER )->GetModel();		
		
		m_pDataModel->pAdfFileFunc->form_pnt(pModel->iSelectedNo);

		m_pDataModel->pCommon->strbtrim(m_pDataModel->pAdfFileFunc->pointid_front);
		sprintf(m_pDataModel->trgtpoint.point_id,"%6s",m_pDataModel->pAdfFileFunc->pointid_front);

		// design or design
		if(pModel->points[pModel->iSelectedNo-1].bActualSelected)
		{
			m_pDataModel->trgtpoint.x = atof(m_pDataModel->pAdfFileFunc->xmea_front);
			m_pDataModel->trgtpoint.y = atof(m_pDataModel->pAdfFileFunc->ymea_front);
			m_pDataModel->trgtpoint.z = atof(m_pDataModel->pAdfFileFunc->zmea_front);

			if( !m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->xmea_front) && 
				!m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->ymea_front) && 
				!m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->zmea_front))
			{
				m_pDataModel->trgtpoint.sta = 1;
			}
			else
			{
				m_pDataModel->trgtpoint.sta = 0;
			}
		}
		else
		{
			m_pDataModel->trgtpoint.x = atof(m_pDataModel->pAdfFileFunc->xdes_front);
			m_pDataModel->trgtpoint.y = atof(m_pDataModel->pAdfFileFunc->ydes_front);
			m_pDataModel->trgtpoint.z = atof(m_pDataModel->pAdfFileFunc->zdes_front);

			if( !m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->xdes_front) && 
				!m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->ydes_front) && 
				!m_pDataModel->pCommon->strblank(m_pDataModel->pAdfFileFunc->zdes_front))
			{
				m_pDataModel->trgtpoint.sta = 1;
			}
			else
			{
				m_pDataModel->trgtpoint.sta = 0;
			}
		}
		m_pDataModel->all_defined();
	}

	// REF LINE
	if(lCtrlID == SELECT_MULTIPOINTS_REF_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*) GetController( SELECT_MULTIPOINTS_REF_LINE_CONTROLLER )->GetModel();		
		Common common(m_pModel);
		
		//delete old values
		memset(&m_pDataModel->rline_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[15], bYmea1[15], bZmea1[15];
		char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_LINE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if(iPno != 0)
			{
				m_pDataModel->pAdfFileFunc->form_pnt1((int) iPno,pid1, nullptr, bXmea1, bXdes1, nullptr, bYmea1, bYdes1, nullptr, bZmea1, bZdes1, nullptr);
				
				if(pModel->nro_table[i][1] == DESIGN)
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
	// TARGET LINE
	if(lCtrlID == SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*) GetController( SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER )->GetModel();		
		Common common(m_pDlg->GetModel());
		
		//delete old values
		memset(&m_pDataModel->trgt_line_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[15], bYmea1[15], bZmea1[15];
		char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_LINE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if(iPno != 0)
			{
				m_pDataModel->pAdfFileFunc->form_pnt1((int) iPno,pid1, nullptr, bXmea1, bXdes1, nullptr, bYmea1, bYdes1, nullptr, bZmea1, bZdes1, nullptr);
				
				if(pModel->nro_table[i][1] == DESIGN)
				{
					des = 1;
					m_pDataModel->trgt_line_buff[0].points[iCount].x = atof(bXdes1);
					m_pDataModel->trgt_line_buff[0].points[iCount].y = atof(bYdes1);
					m_pDataModel->trgt_line_buff[0].points[iCount].z = atof(bZdes1);
					
					if(!m_pDataModel->pCommon->strblank(bXdes1) && !m_pDataModel->pCommon->strblank(bYdes1) &&
						!m_pDataModel->pCommon->strblank(bZdes1) )

						m_pDataModel->trgt_line_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->trgt_line_buff[0].points[iCount].sta = 0;

				}
				else
				{
					act = 1;
					m_pDataModel->trgt_line_buff[0].points[iCount].x = atof(bXmea1);
					m_pDataModel->trgt_line_buff[0].points[iCount].y = atof(bYmea1);
					m_pDataModel->trgt_line_buff[0].points[iCount].z = atof(bZmea1);
					
					if(!m_pDataModel->pCommon->strblank(bXmea1) && !m_pDataModel->pCommon->strblank(bYmea1) &&
						!m_pDataModel->pCommon->strblank(bZmea1) )

						m_pDataModel->trgt_line_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->trgt_line_buff[0].points[iCount].sta = 0;

				}
				iCount++;
			}
			m_pDataModel->REF_ACTDES = (des ==1) ? DESIGN : ACTUAL;
		}
	
		if(GetController(CALC_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_LINE_CONTROLLER, new DCP::CalcLineController(&m_pDataModel->trgt_line_buff[0],ACTUAL, 1) );
		}

		(void)GetController( CALC_LINE_CONTROLLER )->SetModel(m_pDlg->GetModel());
		SetActiveController(CALC_LINE_CONTROLLER, true);
	}
	
	// REF PLANE
	if(lCtrlID == SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*) GetController( SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER )->GetModel();		
		Common common(m_pDlg->GetModel());
		
		//delete old values
		memset(&m_pDataModel->rplane_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[15], bYmea1[15], bZmea1[15];
		char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if(iPno != 0)
			{
				m_pDataModel->pAdfFileFunc->form_pnt1((int) iPno,pid1, nullptr, bXmea1, bXdes1, nullptr, bYmea1, bYdes1, nullptr, bZmea1, bZdes1, nullptr);
				
				if(pModel->nro_table[i][1] == DESIGN)
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
	
	// TARGET PLANE
	if(lCtrlID == SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*) GetController( SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER )->GetModel();		
		Common common(m_pDlg->GetModel());
		
		//delete old values
		memset(&m_pDataModel->rplane_buff[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[15], bYmea1[15], bZmea1[15];
		char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if(iPno != 0)
			{
				m_pDataModel->pAdfFileFunc->form_pnt1((int) iPno,pid1, nullptr, bXmea1, bXdes1, nullptr, bYmea1, bYdes1, nullptr, bZmea1, bZdes1, nullptr);
				
				if(pModel->nro_table[i][1] == DESIGN)
				{
					des = 1;
					m_pDataModel->trgt_plane_buff[0].points[iCount].x = atof(bXdes1);
					m_pDataModel->trgt_plane_buff[0].points[iCount].y = atof(bYdes1);
					m_pDataModel->trgt_plane_buff[0].points[iCount].z = atof(bZdes1);
					
					if(!m_pDataModel->pCommon->strblank(bXdes1) && !m_pDataModel->pCommon->strblank(bYdes1) &&
						!m_pDataModel->pCommon->strblank(bZdes1) )

						m_pDataModel->trgt_plane_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->trgt_plane_buff[0].points[iCount].sta = 0;

				}
				else
				{
					act = 1;
					m_pDataModel->trgt_plane_buff[0].points[iCount].x = atof(bXmea1);
					m_pDataModel->trgt_plane_buff[0].points[iCount].y = atof(bYmea1);
					m_pDataModel->trgt_plane_buff[0].points[iCount].z = atof(bZmea1);
					
					if(!m_pDataModel->pCommon->strblank(bXmea1) && !m_pDataModel->pCommon->strblank(bYmea1) &&
						!m_pDataModel->pCommon->strblank(bZmea1) )

						m_pDataModel->trgt_plane_buff[0].points[iCount].sta = 1;
					else	
						m_pDataModel->trgt_plane_buff[0].points[iCount].sta = 0;

				}
				iCount++;
			}
			m_pDataModel->REF_ACTDES = (des ==1) ? DESIGN : ACTUAL;
		}
	
		if(GetController(CALC_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_PLANE_CONTROLLER, new DCP::CalcPlaneontrollerC(&m_pDataModel->trgt_plane_buff[0],ACTUAL, 1) );
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
void DCP::CalculationAngleController::change_function_keys()
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

		vDef.strLable = StringC(AT_DCP06,K_DCP_ANGLE_FILE_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_REF_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_TARGET_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_SAVE_TOK);
		SetFunctionKey( FK5, vDef );
	
		// SHIFT
		vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
		SetFunctionKey( SHFK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
		SetFunctionKey( SHFK5, vDef );

	}
	else if(m_pDataModel->dspMode == 1) // REF
	{
		//vDef.nLable = K_DCP_POINT_TOK;
		//SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_PLANE_TOK);
		SetFunctionKey( FK3, vDef );

	}
	else if(m_pDataModel->dspMode == 2) // 
	{
		if(m_pDataModel->active_reftype == LINE)
		{
			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
			SetFunctionKey( FK1, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
			SetFunctionKey( FK2, vDef );
		}
		else
		{
			vDef.strLable = StringC(AT_DCP06,K_DCP_PLANE_TOK);
			SetFunctionKey( FK3, vDef );
		}

	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

// ================================================================================================
// ====================================  CalculationAngleModel  ====================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CalculationAngleModel::CalculationAngleModel(DCP::Model* pModel):pAdfFileFunc(0),pCommon(0),pMsgBox(0)
{
	active_reftype=LINE;
	active_targettype=LINE;	
	dspMode = eNormal;

	sSelected3DFile = L" ";
	pAdfFileFunc = new AdfFileFunc(pModel);
	pAgfFileFunc = new AgfFileFunc(pModel);
	pAdfFileFunc->always_single = 1;
	pCommon = new Common(pModel);
	pMsgBox = new MsgBox;

	sActualSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_SELECTED_TOK);
	sActualNonSelected.LoadTxt(AT_DCP06,P_DCP_ACTUAL_NONSELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_SELECTED_TOK);
	sDesignNonSelected.LoadTxt(AT_DCP06,P_DCP_DESIGN_NONSELECTED_TOK);
	
	clear_buffers();

}
// ================================================================================================
// Description: Destructor
// ================================================================================================
void DCP::CalculationAngleModel::clear_buffers()
{
	memset(&rplane_buff[0],	0,	sizeof(S_PLANE_BUFF));
	memset(&rline_buff[0],	0,	sizeof(S_LINE_BUFF));
	memset(&refpoint,		0,	sizeof(S_POINT_BUFF));
	memset(&trgtpoint,		0,	sizeof(S_POINT_BUFF));
	memset(&trgt_line_buff[0],0,	sizeof(S_LINE_BUFF ));
	memset(&trgt_plane_buff[0],0,	sizeof(S_PLANE_BUFF));

	dCalculatedAngle = 0.0;
	iAngleCalculated = 0;
	REF_ACTDES = 0;

	cAid[0] = '\0';
	cNote[0] = '\0';
	cAngle[0] = '\0';
	cRefId[0] = '\0';
	cRefType[0] = '\0';
	cTrgtId[0] = '\0';
	cTrgtType[0] = '\0';
}
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalculationAngleModel::~CalculationAngleModel()
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
		if(pAgfFileFunc)
		{
			delete pAgfFileFunc;
			pAgfFileFunc = 0;
		}
}

// ================================================================================================
// Description: destructor
// ================================================================================================
short DCP::CalculationAngleModel::all_defined()
{
	if(is_active_reference_defined() == 1 && is_active_target_defined() == 1)
	{
		calc_angle();
		return 1;
	}
	else
	{
		dCalculatedAngle = 0.0;
		iAngleCalculated = 0;
	}
	return 0;
}

// ************************************************************************
//
// ************************************************************************
short DCP::CalculationAngleModel::is_active_reference_defined()
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
short DCP::CalculationAngleModel::is_active_target_defined()
{
short ret=0;

	if(active_targettype == POINT)
	{
		if(trgtpoint.sta == 1 ||
	   	trgtpoint.dsta == 1)
		{
		  ret = 1;
		}
	}
	else if(active_targettype == LINE)
	{
		if(trgt_line_buff[0].calc == 1 ||
		   trgt_line_buff[0].des_calc == 1)
		{
			ret = 1;
		}
	
	}
	else if(active_targettype == PLANE)
	{
		if(trgt_plane_buff[0].calc == 1 ||
			trgt_plane_buff[0].des_calc == 1)
		{
			ret = 1;
		}
	
	}
	return ret;
}


/************************************************************************
*************************************************************************/
short DCP::CalculationAngleModel::calc_angle_line_point()
{
struct ams_vector line1,line2;
double ang;
int pcounts,i;

	if(rline_buff[0].calc == 0)
	{
	  //show_error(MSG_DEFINE_REFERENCE_LINE);
	  return 0;
	}

	if(trgtpoint.sta == 0)
	{
	  //show_error(MSG_DEFINE_TARGET_POINT);
	  return 0;
	}

	/*******************************************
		Calculate how many points are in line
		Only 2 points are acceptable
	********************************************/

	pcounts = 0;

	for(i=0; i < MAX_POINTS_IN_LINE; i++)
	{
		 if(rline_buff[0].points[i].sta == 1 ||rline_buff[0].points[i].sta == 2) 
			pcounts++;
	}
	if(pcounts > 2)
	{
		//show_error(LINE_MUST_BE_DEFINED_WITH_2POINTS);
		return 0;
	}

	line1.x = 	trgtpoint.x -
				rline_buff[0].px;

	line1.y = 	trgtpoint.y -
				rline_buff[0].py;

	line1.z = 	trgtpoint.z -
				rline_buff[0].pz;

	line2.x =	rline_buff[0].ux;
	line2.y =	rline_buff[0].uy;
	line2.z =	rline_buff[0].uz;

	ang = angle(&line1,&line2);
	dCalculatedAngle = radtodeg(ang);
	iAngleCalculated = 1;
	//sprintf(bAngle,"%10.3f",ang);
	//FldUpd(ANGLE_TAG,bAngle);

	return 1;
}


/************************************************************************
*************************************************************************/
short DCP::CalculationAngleModel::calc_angle_line_line()
{
struct ams_vector line1,line2;
double ang;

	if(rline_buff[0].calc == 0)
	{
	  //show_error(MSG_DEFINE_REFERENCE_LINE);
	  return 0;
	}

	if(trgt_line_buff[0].calc == 0)
	{
	  //show_error(MSG_DEFINE_TARGET_LINE);
	  return 0;
	}

	line1.x = 	trgt_line_buff[0].ux;
	line1.y = 	trgt_line_buff[0].uy;
	line1.z = 	trgt_line_buff[0].uz;

	line2.x =	rline_buff[0].ux;
	line2.y =	rline_buff[0].uy;
	line2.z =	rline_buff[0].uz;

	ang = angle(&line1,&line2);
	dCalculatedAngle = radtodeg(ang);
	iAngleCalculated = 1;
	//sprintf(bAngle,"%10.3f",ang);
	//FldUpd(ANGLE_TAG,bAngle);

	return 1;

}

/************************************************************************
*************************************************************************/
short DCP::CalculationAngleModel::calc_angle_plane_plane()
{
struct ams_vector line1,line2;
double ang;

	if(rplane_buff[0].calc == 0)
	{
	  return 0;
	}

	if(trgt_plane_buff[0].calc == 0)
	{
	  return 0;
	}

	line1.x = 	trgt_plane_buff[0].nx;
	line1.y = 	trgt_plane_buff[0].ny;
	line1.z = 	trgt_plane_buff[0].nz;

	line2.x =	rplane_buff[0].nx;
	line2.y =	rplane_buff[0].ny;
	line2.z =	rplane_buff[0].nz;

	ang = angle(&line1,&line2);
	dCalculatedAngle = radtodeg(ang);

	iAngleCalculated = 1;

	//sprintf(bAngle,"%10.3f",ang); // wrong : was sprintf(bAngle,...");
	//FldUpd(ANGLE_TAG,bAngle);

	return 1;
}


// ************************************************************************
//
// ************************************************************************
short DCP::CalculationAngleModel::calc_angle()
{
short ret = 0;
//double dist;
//struct ams_vector m;
//struct ams_vector wpoint;
//struct line   wline;
//struct plane  wplane;
	/*
	sprintf(bAid,"%-6.6s" ," ");
	sprintf(bAngle,"%-10.10s" ," ");
	sprintf(bNote,"%-8.8s" ," ");
	strbtrim(bNote);
	strbtrim(bAid);
	
	MMI_UpdateField(DialogId, 13, bAid, TRUE);
	MMI_UpdateField(DialogId, 14, bAngle, TRUE);
	MMI_UpdateField(DialogId, 15, bNote, TRUE);
	*/
	if(active_reftype == LINE)
	{
		switch(active_targettype) 
		{
			case POINT:
						ret = calc_angle_line_point();
						break;
			case LINE:
						ret = calc_angle_line_line();
						break;
		}
	}
	else if(active_reftype == PLANE)
	{
		switch(active_targettype)
		{
			case PLANE:
						ret = calc_angle_plane_plane();
						break;
		}
	}

	if(ret==1)
	{
		pAgfFileFunc->get_next_id(cAid);
		pCommon->strbtrim(cAid);
		
		//get_next_distid(&CalcAngleFile_,bAid);
		//strbtrim(bAid);
		//MMI_UpdateField(DialogId, 13, bAid, TRUE);	
	}
	//MMI_UpdateField(DialogId, 14, bAngle, TRUE);
	return ret;
}


// *******************************************************
//
// *******************************************************
void DCP::CalculationAngleModel::save()
{
//char strDist[20];
char strRefType[20], strTrgtType[20];
char strRef[15],strTrgt[15];

	if(is_active_target_defined() == 1 && is_active_reference_defined() == 1)
	{	
		/*
		if(!strblank(bAngle))
			sprintf(strDist,"%-10.10s", bAngle);
		else
			sprintf(strDist,"%-10.10s", " ");
		*/
		switch(active_reftype)
		{
			case POINT: 
					sprintf(strRef,"%-s", refpoint.point_id);
					sprintf(strRefType,"%-8s/%c",cRefType,(REF_ACTDES==ACTUAL) ? sActualSelected[0]/*'A'*/:sDesignSelected[0] /*'D'*/);
					break;

			case LINE:
					sprintf(strRef,"%-s", rline_buff[0].id);
					sprintf(strRefType,"%-8s/%c",cRefType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] /*'A'*/:sDesignSelected[0] /*'D'*/);
					break;

			case PLANE:
					sprintf(strRef,"%-s", rplane_buff[0].id);
					sprintf(strRefType,"%-8s/%c",cRefType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] /*'A'*/:sDesignSelected[0] /*'D'*/);
					break;
		}

		switch(active_targettype)
		{
			case POINT: 
					sprintf(strTrgt,"%-s", trgtpoint.point_id);
					sprintf(strTrgtType,"%-8s/%c",cTrgtType,(REF_ACTDES==ACTUAL) ? sActualSelected[0]/*'A'*/:sDesignSelected[0] /*'D'*/);
					break;

			case LINE:
					sprintf(strTrgt,"%-s", trgt_line_buff[0].id);
					sprintf(strTrgtType,"%-8s/%c",cTrgtType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] /*'A'*/:sDesignSelected[0] /*'D'*/);
					break;

			case PLANE:
					sprintf(strTrgt,"%-s", trgt_plane_buff[0].id);
					sprintf(strTrgtType,"%-8s/%c",cTrgtType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] /*'A'*/:sDesignSelected[0] /*'D'*/);
					break;
		}
		
		//sprintf(strRefType,"%-8s/%c",cRefType,(REF_ACTDES==ACTUAL) ? sActualSelected[0] :sDesignSelected[0]);
		// sprintf(strTrgt,"%-s",trgtpoint.point_id);
		// sprintf(strTrgtType,"%-8s/%c",bTargetType,(TRG_ACTDES==ACTUAL) ?actual_selected[0] /*'A'*/:design_selected[0] /*'D'*/);
		// PrintLn("bDid=%-s",bDid);
		if(pAgfFileFunc->save_calcangle_to_file(cAngle, strRef,strRefType,strTrgt,strTrgtType, cAid,cNote,1))
			 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_ANGLE_SAVED_TOK));
		/*
		if(CalcAngleFile_.open == RC_OK)
		{
			sprintf(bCdf,"%-8.8s(%d)",get_fname(CalcAngleFile_.name),CalcAngleFile_.points);
			MMI_UpdateField(DialogId, 3, bCdf, TRUE);
		}
		*/
	}
}
