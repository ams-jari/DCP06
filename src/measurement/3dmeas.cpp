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
#include <dcp06/core/Logger.hpp>
#include <dcp06/init/Initialization.hpp>
#include <dcp06/measurement/3dmeas.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/core/Tool.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/core/SelectPoint.hpp>
#ifndef DCP_USE_JSON_DATABASE
#define DCP_USE_JSON_DATABASE 1
#endif
#if DCP_USE_JSON_DATABASE
#include <dcp06/database/JsonDatabase.hpp>
#endif
#include <dcp06/calculation/CalculationDist2Points.hpp>
#include <dcp06/core/Aim.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/measurement/SeparateRec.hpp>
#include <dcp06/measurement/HomePoints.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <dcp06/measurement/MeasDist.hpp>
#include <dcp06/core/AutoMatch.hpp>
#include <dcp06/application/ShaftLine.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/measurement/MidPointController.hpp>


#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

//#include <ABL_Util.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>
#include <TBL_DistAvgDlg.hpp>
#include <TBL_MeasResults.hpp>
#include <TPI_MeasConfig.hpp>
#include <UTL_StringFunctions.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>
#include <TBL_Util.hpp>


#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>

#include <GUI_Layout.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::Meas3DDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================



// ================================================================================================
// ====================================  Meas3DDialog             ==============================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Meas3DDialog::Meas3DDialog(Meas3DModel* pMeasModel):GUI::ModelHandlerC(),GUI::/*Standard*/DialogC()
			,m_pFile(0),m_pPointId(0), m_pXDsg(0), m_pYDsg(0), m_pZDsg(0),
			m_pXAct(0),m_pYAct(0),m_pZAct(0),m_pXDev(0),m_pYDev(0),m_pZDev(0),m_pDataModel(pMeasModel),
			m_pPointIdObserver(OBS_METHOD_TO_PARAM0(Meas3DDialog, OnValueChanged), this),
			m_pXObserver(OBS_METHOD_TO_PARAM0(Meas3DDialog, OnValueChanged), this),
			m_pYObserver(OBS_METHOD_TO_PARAM0(Meas3DDialog, OnValueChanged), this),
			m_pZObserver(OBS_METHOD_TO_PARAM0(Meas3DDialog, OnValueChanged), this),iInfoInd(0)

			//,m_pInfo(0)
{
	//SetTxtApplicationId(AT_DCP06);
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);
}
           
// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Meas3DDialog::~Meas3DDialog()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}

	//m_pTimer.KillTimer();
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::Meas3DDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();

	// Add fields to dialogds

	GUI::ComboLineCtrlC* emptyTextCtrl1 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	GUI::ComboLineCtrlC* emptyTextCtrl2 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	GUI::ComboLineCtrlC* emptyTextCtrl3 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	GUI::ComboLineCtrlC* emptyTextCtrl4 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	GUI::ComboLineCtrlC* emptyTextCtrl5 = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);

	emptyTextCtrl1->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	emptyTextCtrl1->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	emptyTextCtrl1->GetStringInputCtrl()->SetString(L" ");

	emptyTextCtrl2->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	emptyTextCtrl2->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	emptyTextCtrl2->GetStringInputCtrl()->SetString(L" ");

	emptyTextCtrl3->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	emptyTextCtrl3->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	emptyTextCtrl3->GetStringInputCtrl()->SetString(L" ");

	emptyTextCtrl4->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	emptyTextCtrl4->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	emptyTextCtrl4->GetStringInputCtrl()->SetString(L" ");

	emptyTextCtrl5->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	emptyTextCtrl5->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	emptyTextCtrl5->GetStringInputCtrl()->SetString(L" ");

	GUI::LayoutC* pLayout = new GUI::LayoutC(GUI::LayoutC::LT_HorizontalBox, this);
	GUI::LayoutC* pCol1 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
    GUI::LayoutC* pCol2 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
	pLayout->AddLayout(pCol1);
	pLayout->AddLayout(pCol2);
	SetLayout(pLayout);

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pFile->GetStringInputCtrl()-
	//m_pFile->GetStringInputCtrl()->GetCtrlState(GUI::BaseCtrlC::
	//m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//AddCtrl(m_pFile);
	
	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
	//m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//AddCtrl(m_pPointId);


	//SetColonPosLong( GUI::StandardDialogC::CP_10 );

	m_pXDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pXDsg->SetId(eXDsg);
	m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_DSG_TOK));
	m_pXDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short) GetModel()->m_nDecimals);
	//m_pXDsg->GetFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	//m_pXDsg->GetFloatInputCtrl()->SetRange(SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	//m_pXDsg->SetAutoColon(false);
	//m_pXDsg->SetColonPosition(9 * 11);
	//m_pXDsg->SetTextWidth(9*11);	
	m_pXDsg->SetEmptyAllowed(true);
	//m_pXDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//AddCtrl(m_pXDsg);
	

	m_pYDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pYDsg->SetId(eYDsg);
	m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_DSG_TOK));
	m_pYDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	//m_pYDsg->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pYDsg->SetEmptyAllowed(true);
	
	//m_pYDsg->SetAutoColon(false);
	//m_pYDsg->SetColonPosition(9 * 11);
	//m_pYDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_pYDsg->SetTextWidth(9*11);	
	//AddCtrl(m_pYDsg);
	

	m_pZDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZDsg->SetId(eZDsg);
	m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_DSG_TOK));
	m_pZDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pZDsg->SetEmptyAllowed(true);
	//m_pZDsg->SetAutoColon(false);
	//m_pZDsg->SetColonPosition(9 * 11);
	//m_pZDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_pZDsg->SetTextWidth(9*11);
	//AddCtrl(m_pZDsg);
	
	m_pXAct = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pXAct->SetId(eXAct);
	m_pXAct->SetText(StringC(AT_DCP06,P_DCP_X_ACT_TOK));
	m_pXAct->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXAct->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXAct->SetEmptyAllowed(true);

	m_pYAct = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYAct->SetId(eYAct);
	m_pYAct->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_TOK));
	m_pYAct->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYAct->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYAct->SetEmptyAllowed(true);
	

	m_pZAct = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZAct->SetId(eZAct);
	m_pZAct->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_TOK));
	m_pZAct->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZAct->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZAct->SetEmptyAllowed(true);

	m_pXDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pXDev->SetId(eXDev);
	m_pXDev->SetText(StringC(AT_DCP06,P_DCP_X_DEV_TOK));
	m_pXDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXDev->SetEmptyAllowed(true);
	

	m_pYDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYDev->SetId(eYDev);
	m_pYDev->SetText(StringC(AT_DCP06,P_DCP_Y_DEV_TOK));
	m_pYDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYDev->SetEmptyAllowed(true);

	m_pZDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZDev->SetId(eZDev);
	m_pZDev->SetText(StringC(AT_DCP06,P_DCP_Z_DEV_TOK));
	m_pZDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZDev->SetEmptyAllowed(true);
	
	pCol2->AddCtrl(m_pFile);
	//emptyTextCtrl1->SetText("1");
	//pCol2->AddCtrl(emptyTextCtrl1);
	
	pCol1->AddCtrl(m_pPointId);
	//emptyTextCtrl2->SetText("2");
	//pCol2->AddCtrl(emptyTextCtrl2);

	pCol1->AddCtrl(m_pXDsg);
	//emptyTextCtrl3->SetText("3");
	pCol2->AddCtrl(emptyTextCtrl3);

	pCol1->AddCtrl(m_pYDsg);
	//emptyTextCtrl4->SetText("4");
	pCol2->AddCtrl(emptyTextCtrl4);
	
	pCol1->AddCtrl(m_pZDsg);
	//emptyTextCtrl5->SetText("5");
	pCol2->AddCtrl(emptyTextCtrl5);


	pCol1->AddCtrl(m_pXAct);
	pCol2->AddCtrl(m_pXDev);

	pCol1->AddCtrl(m_pYAct);
	pCol2->AddCtrl(m_pYDev);

	pCol1->AddCtrl(m_pZAct);
	pCol2->AddCtrl(m_pZDev);


	//InsertEmptyLine();
	/*
	m_pXActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pXActDev->SetId(eXActDev);
	m_pXActDev->SetText(StringC(AT_DCP06,P_DCP_X_ACT_DEV_TOK));
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXActDev->SetEmptyAllowed(true);
	//m_pXActDev->SetAutoColon(false);
	//m_pXActDev->SetColonPosition(9 * 11);
	m_pXActDev->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	
	
	AddCtrl(m_pXActDev);

	m_pYActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYActDev->SetId(eYActDev);
	m_pYActDev->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_DEV_TOK));
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYActDev->SetEmptyAllowed(true);
	//m_pYActDev->SetAutoColon(false);
	//m_pYActDev->SetColonPosition(9 * 11);
	m_pYActDev->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	AddCtrl(m_pYActDev);

	//m_pZActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZActDev->SetId(eZActDev);
	m_pZActDev->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_DEV_TOK));
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZActDev->SetEmptyAllowed(true);
	//m_pZActDev->SetAutoColon(false);
	//m_pZActDev->SetColonPosition(9 * 11);
	m_pZActDev->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	AddCtrl(m_pZActDev);
	*/

	/*
	m_pXActDev = new GUI::MultiTextValueCtrlC();
	m_pXActDev->SetId(eXActDev);
	m_pXActDev->AddItem(1,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,0,AlignmentT::AL_LEFT,65,AlignmentT::AL_RIGHT, true);
	m_pXActDev->AddItem(2,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,50,AlignmentT::AL_LEFT,42,AlignmentT::AL_RIGHT, false);
	//m_pXActDev->SetString(1,StringC(L"100.0"));
	//m_pXActDev->SetString(2,StringC(L"-99100.0"));
	m_pXActDev->SetText(1, StringC(AT_DCP06,P_DCP_X_ACT_DEV_TOK));
	AddCtrl(m_pXActDev);

	m_pYActDev = new GUI::MultiTextValueCtrlC();
	m_pYActDev->SetId(eYActDev);
	m_pYActDev->AddItem(1,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,0,AlignmentT::AL_LEFT,65,AlignmentT::AL_RIGHT, true);
	m_pYActDev->AddItem(2,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,50,AlignmentT::AL_LEFT,42,AlignmentT::AL_RIGHT, false);
	//m_pXActDev->SetString(1,StringC(L"100.0"));
	//m_pXActDev->SetString(2,StringC(L"-99100.0"));
	m_pYActDev->SetText(1, StringC(AT_DCP06,P_DCP_Y_ACT_DEV_TOK));
	AddCtrl(m_pYActDev);

	m_pZActDev = new GUI::MultiTextValueCtrlC();
	m_pZActDev->SetId(eZActDev);
	m_pZActDev->AddItem(1,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,0,AlignmentT::AL_LEFT,65,AlignmentT::AL_RIGHT, true);
	m_pZActDev->AddItem(2,GUI::MultiTextValueCtrlC::ValueTypeT::VT_String,50,AlignmentT::AL_LEFT,42,AlignmentT::AL_RIGHT, false);
	//m_pXActDev->SetString(1,StringC(L"100.0"));
	//m_pXActDev->SetString(2,StringC(L"-99100.0"));
	m_pZActDev->SetText(1, StringC(AT_DCP06,P_DCP_Z_ACT_DEV_TOK));
	AddCtrl(m_pZActDev);
	*/

	

	m_pPointIdObserver.Attach(m_pPointId->GetSubject());
	m_pXObserver.Attach(m_pXDsg->GetSubject());
	m_pYObserver.Attach(m_pYDsg->GetSubject());
	m_pZObserver.Attach(m_pZDsg->GetSubject());

	// message....
	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);

	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	// set help
	//SetHelpTok(H_DCP_3DMEAS_TOK,0);

	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: OnTimer
// ================================================================================================
//void DCP::Meas3DDialog::OnTimer(void)
//{
//
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	//m_pInfo->SetText(strInfoText + sMsg);
//
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::Meas3DDialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
	m_pCommon = new Common(GetModel());
#if DCP_USE_JSON_DATABASE
	DCP::Database::JsonDatabase* jdb = GetModel() && GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase()) : 0;
	bool jobOpen = jdb && jdb->isJobLoaded() && !GetModel()->m_currentJobId.empty();
	if (jobOpen)
	{
		int nPts = jdb->getJobPointsCount();
		GetModel()->m_currentPointIndex = (nPts > 0) ? 1 : 0;
	}
	else
#endif
		m_pDataModel->m_pFileFunc->setFile(GetModel()->ADFFileName);
	RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::Meas3DDialog::UpdateData()
{
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::Meas3DDialog::RefreshControls()
{
	if(m_pFile && m_pPointId && m_pXDsg && m_pYDsg && m_pZDsg && m_pXAct &&
		m_pYAct && m_pZAct && m_pXDev &&
		m_pYDev && m_pZDev) 
	{
		m_pCommon->empty_xyz_buffers(m_pDataModel->bXdif,m_pDataModel->bYdif,m_pDataModel->bZdif,9);
		sprintf(m_pDataModel->bFid,"%-8.8s","");
		m_pDataModel->bPid[0] = '\0';

#if DCP_USE_JSON_DATABASE
		DCP::Database::JsonDatabase* jdb = GetModel() && GetModel()->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase()) : 0;
		m_pDataModel->m_bJobOpen = (jdb && jdb->isJobLoaded() && !GetModel()->m_currentJobId.empty());
		if (m_pDataModel->m_bJobOpen)
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);
			m_pFile->GetStringInputCtrl()->SetString(StringC(GetModel()->m_currentJobId.c_str()));
			m_pDataModel->DSP_MODE = SINGLE;
			int idx = GetModel()->m_currentPointIndex;
			if (idx > 0 && jdb->getPointByIndex(idx, true,
				m_pDataModel->bPid,
				m_pDataModel->bXmea, m_pDataModel->bXdes,
				m_pDataModel->bYmea, m_pDataModel->bYdes,
				m_pDataModel->bZmea, m_pDataModel->bZdes,
				m_pDataModel->bNote))
			{
				m_pCommon->strbtrim(m_pDataModel->bPid);
			}
			else
			{
				m_pDataModel->bPid[0] = '\0';
				m_pDataModel->bXmea[0] = m_pDataModel->bXdes[0] = '\0';
				m_pDataModel->bYmea[0] = m_pDataModel->bYdes[0] = '\0';
				m_pDataModel->bZmea[0] = m_pDataModel->bZdes[0] = '\0';
			}
		}
		else
#endif
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);
			m_pFile->GetStringInputCtrl()->SetString(StringC(m_pDataModel->m_pFileFunc->getFileName()));
			if(m_pDataModel->m_pFileFunc->point_type == 0 )
				m_pDataModel->DSP_MODE = SINGLE;
		}
		else
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
			m_pFile->GetStringInputCtrl()->SetString(L"");
		}
		m_pDataModel->set_xyz_mea_ptr();
		m_pDataModel->set_xyz_des_ptr();

		// *************************
		// SINGLE
		// *************************
		
		if(m_pDataModel->DSP_MODE != FRONTBACK)
		{
			sprintf(m_pDataModel->bPid,"%-s",m_pCommon->strbtrim(m_pDataModel->pid_ptr));

			m_pCommon->get_dist_len(m_pDataModel->xmea_ptr,m_pDataModel->xdes_ptr, m_pDataModel->bXdif,11);
			m_pCommon->get_dist_len(m_pDataModel->ymea_ptr,m_pDataModel->ydes_ptr, m_pDataModel->bYdif,11);
			m_pCommon->get_dist_len(m_pDataModel->zmea_ptr,m_pDataModel->zdes_ptr, m_pDataModel->bZdif,11);
			
			if(!m_pCommon->strblank(m_pDataModel->xdes_ptr))
			{	
				m_pDataModel->Xdes = atof(m_pDataModel->xdes_ptr);
				m_pXDsg->GetFloatInputCtrl()->SetDouble(atof(m_pDataModel->xdes_ptr));
			}
			else
			{
				m_pXDsg->GetFloatInputCtrl()->SetEmpty();
			}

			// Y dsg
			if(!m_pCommon->strblank(m_pDataModel->ydes_ptr))
			{
				m_pDataModel->Ydes = atof(m_pDataModel->ydes_ptr);
				m_pYDsg->GetFloatInputCtrl()->SetDouble(atof(m_pDataModel->ydes_ptr));
			}
			else
			{
				m_pYDsg->GetFloatInputCtrl()->SetEmpty();
			} 
			
			if(!m_pCommon->strblank(m_pDataModel->zdes_ptr))
			{
				m_pDataModel->Zdes = atof(m_pDataModel->zdes_ptr);
				m_pZDsg->GetFloatInputCtrl()->SetDouble(atof(m_pDataModel->zdes_ptr));
			}
			else
			{
				m_pZDsg->GetFloatInputCtrl()->SetEmpty();
			}

			m_pXAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->xmea_ptr));
			m_pYAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->ymea_ptr));
			m_pZAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->zmea_ptr));

			m_pXDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bXdif));
			m_pYDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bYdif));
			m_pZDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bZdif));
		}
		
		m_pPointId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bPid));
	}
}

// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::Meas3DDialog::OnValueChanged(int unNotifyCode,  int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetModel()->iCurrentPoint;
		Common common(GetModel());
		if(ulParam2 == ePointId)
		{
			StringC sPid;
			if(m_pPointId->GetStringInputCtrl()->IsEmpty())
				sprintf(m_pDataModel->pid_ptr,"%-6.6s","");	
			else
			{
				sPid = m_pPointId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char temp[20];
				//UTL::UnicodeToAscii(temp, sPid);
				BSS::UTI::BSS_UTI_WCharToAscii(sPid, temp);
				common.strbtrim(temp);
				sprintf(m_pDataModel->pid_ptr,"%-6.6s",temp);
				m_pDataModel->save_point();
				RefreshControls();
			}
		}
		else
		{
			
			
			if (ulParam2 == eXDsg)
			{
				if(!m_pXDsg->GetFloatInputCtrl()->IsEmpty())
					sprintf(m_pDataModel->xdes_ptr,"%9.*f",GetModel()->m_nDecimals,m_pXDsg->GetFloatInputCtrl()->GetDouble());
				else
					sprintf(m_pDataModel->xdes_ptr,"%9.9s"," ");
				m_pDataModel->save_point();
				RefreshControls();
			}
			else if (ulParam2 == eYDsg)
			{
				if(!m_pYDsg->GetFloatInputCtrl()->IsEmpty())
					sprintf(m_pDataModel->ydes_ptr,"%9.*f",GetModel()->m_nDecimals,m_pYDsg->GetFloatInputCtrl()->GetDouble());
				else
					sprintf(m_pDataModel->ydes_ptr,"%9.9s"," ");
				m_pDataModel->save_point();
				RefreshControls();

			}
			else if(ulParam2 == eZDsg)
			{
				if(!m_pZDsg->GetFloatInputCtrl()->IsEmpty())
					sprintf(m_pDataModel->zdes_ptr,"%9.*f",GetModel()->m_nDecimals,m_pZDsg->GetFloatInputCtrl()->GetDouble());
				else
					sprintf(m_pDataModel->zdes_ptr,"%9.9s"," ");

				m_pDataModel->save_point();
				RefreshControls();
			}
		}
	}
}


// ================================================================================================
// Description: SetModel
// ================================================================================================
bool DCP::Meas3DDialog::SetModel( GUI::ModelC* pModel )
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
// Description: GetModel
// ================================================================================================
DCP::Model* DCP::Meas3DDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}



// ================================================================================================
// ====================================  Meas3DController ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Meas3DController::Meas3DController(bool bShaft, DCP::Model* pModel )
    : m_pDlg( nullptr ),m_bPointMenu(false),m_bShaft(bShaft),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	
	if(!bShaft)
		SetTitle( StringC(AT_DCP06, T_DCP_3DMEAS_TOK) );
	else
		SetTitle( StringC(AT_DCP06, T_DCP_3DSHAFT_ALIGMENT_TOK ));

	msgbox  = new MsgBox;
		
	// create model
	m_pDataModel = new Meas3DModel(pModel);
    
	// Create a dialog
    m_pDlg = new DCP::Meas3DDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DMEAS_DLG, m_pDlg, true );

	
    // Set the function key
	//show_function_keys();

} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Meas3DController::~Meas3DController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
	if(msgbox)
	{
		delete msgbox;
		msgbox = 0;
	}
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::Meas3DController::OnControllerActivated(void)
{
	show_function_keys();
	m_pDataModel->set_dcp05_model(m_pDlg->GetModel());
	m_pCommon = new Common(m_pDlg->GetModel());
}

// ================================================================================================
// Description: show_function_keys
// ================================================================================================
void DCP::Meas3DController::show_function_keys()
{
	ResetFunctionKeys();

	if(m_bCamera)
	{
		FKDef vDef;
		vDef.poOwner = this;
		
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06, K_DCP_ALL_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );

		FKDef vDef1;
			//vDef1.nAppId = AT_DCP06;
			vDef1.poOwner = this;
			vDef1.strLable = L" ";;
			SetFunctionKey( SHFK1, vDef1 );
		SetFunctionKey( SHFK6, vDef1 );
		
		GUI::DesktopC::Instance()->UpdateFunctionKeys();
	}
	else
	{
		if(m_bPointMenu)
		{
			// change function keys
			FKDef vDef;
			//vDef.nAppId = AT_DCP06;
			vDef.poOwner = this;
			
			vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
			SetFunctionKey( FK1, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_ADD_TOK);
			SetFunctionKey( FK2, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
			SetFunctionKey( FK3, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_SWAP_TOK);
			SetFunctionKey( FK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_3DFILE_TOK);
			SetFunctionKey( FK5, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
			SetFunctionKey( SHFK2, vDef );

			// Hide quit
			FKDef vDef1;
			//vDef1.nAppId = AT_DCP06;
			vDef1.poOwner = this;
			vDef1.strLable = L" ";;
			SetFunctionKey( SHFK6, vDef1 );


			// hide other
			/*
			HideFunctionKey(SHFK3);
			HideFunctionKey(SHFK4);
			HideFunctionKey(SHFK5);
			HideFunctionKey(SHFK6);
			*/
			GUI::DesktopC::Instance()->UpdateFunctionKeys();

			//m_bPointMenu = true;
		}
		else
		{
			FKDef vDef;
			//vDef.nAppId = AT_DCP06;
			vDef.poOwner = this;
			vDef.strLable = StringC(AT_DCP06,K_DCP_ALL_TOK);
			SetFunctionKey( FK1, vDef );

			if(m_pDlg->GetModel()->isATR)
			{
				vDef.strLable = StringC(AT_DCP06,K_DCP_DIST_TOK);
				SetFunctionKey( FK2, vDef );
			}

			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
			SetFunctionKey( FK3, vDef );
			
			if(!m_bShaft)
				vDef.strLable = StringC(AT_DCP06,K_DCP_AIM_TOK);
			else
				vDef.strLable = StringC(AT_DCP06,K_DCP_SHAFT_TOK);
			SetFunctionKey( FK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
			SetFunctionKey( FK5, vDef );
		
			vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
			SetFunctionKey( FK6, vDef );
			
			// SHIFT
			/// CAPTIVATE 
			//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
			{
				vDef.strLable = StringC(AT_DCP06,K_DCP_CAMERA_TOK);
				SetFunctionKey( SHFK1, vDef );
			}
			
			vDef.strLable = StringC(AT_DCP06,K_DCP_INIT_TOK);
			SetFunctionKey( SHFK2, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_SPECIAL_TOK);
			SetFunctionKey( SHFK3, vDef );
			
			vDef.strLable = StringC(AT_DCP06,K_DCP_TOOL_TOK);
			SetFunctionKey( SHFK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
			SetFunctionKey( SHFK5, vDef );


			//if(m_pDlg->GetModel()->bDemoMode == true)
			//{
 				//DisableFunctionKey(SHFK4);
				//DisableFunctionKey(SHFK5);
			//}

			vDef.strLable = StringC(AT_DCP06,K_DCP_CHANGE_FACE_TOK);
			SetFunctionKey( SHFK6, vDef );
			GUI::DesktopC::Instance()->UpdateFunctionKeys();
			//m_bPointMenu = false;
		}
	}
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::Meas3DController::SetModel( GUI::ModelC* pModel )
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
// Description: F1
// ================================================================================================
void DCP::Meas3DController::OnF1Pressed()
{
	if(m_bPointMenu)
	{
		// PreviousPoint
#if DCP_USE_JSON_DATABASE
		if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
		{
			if (m_pDlg->GetModel()->m_currentPointIndex > 1)
			{
				m_pDlg->GetModel()->m_currentPointIndex--;
				m_pDataModel->DSP_MODE = SINGLE;
			}
			m_pDlg->RefreshControls();
		}
		else
#endif
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pDataModel->m_pFileFunc->form_prev_pnt();
			m_pDataModel->DSP_MODE = SINGLE;
			m_pDlg->RefreshControls();
		}
		else
		{
			msg.LoadTxt(AT_DCP06,  M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgbox->ShowMessageOk(msg);
		}
		m_bPointMenu = false;	
		show_function_keys();
	}
	else
	{
		if(m_bCamera)
		{
			m_bCamera = false;
			poVideoDlg->Close();
			SetActiveDialog(_3DMEAS_DLG);
			show_function_keys();
		}


		m_pDataModel->set_xyz_mea_ptr();

		if(m_pDlg->GetModel()->m_nOverWriteInfo && 
			!m_pDlg->GetModel()->m_nAutoMatch)
		{
			if(!m_pCommon->strblank(m_pDataModel->xmea_ptr) || 
				!m_pCommon->strblank(m_pDataModel->ymea_ptr) || 
				!m_pCommon->strblank(m_pDataModel->zmea_ptr))
			{
				msg.LoadTxt(AT_DCP06, M_DCP_OVERWRITE_POINT_TOK);
				msg.Format(msg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
				if(!msgbox->ShowMessageYesNo(msg))
				//if(msgbox1(TXT_NIL_TOKEN, M_OVERWRITE_POINT_TOK,(void *) pid_ptr, MB_YESNO) == FALSE)
				{
					m_pDlg->RefreshControls();
					return;
				}
				m_pDlg->RefreshControls();
			}
		}
		// ALL
		if(m_pCommon->check_edm_mode())
		{
			DisableFunctionKey(FK1);
			DisableFunctionKey(FK2);
			DisableFunctionKey(FK3);
			DisableFunctionKey(FK4);
			DisableFunctionKey(FK5);
			DisableFunctionKey(FK6);

			DCP::MeasXYZModel* pModel = new MeasXYZModel;
			pModel->tooli = 1;
			sprintf(pModel->sPointId,"%6.6s",m_pDataModel->pid_ptr);
			m_pCommon->strbtrim(pModel->sPointId);

			if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
			{
				(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::MeasXYZController(m_pDlg->GetModel()));
			}
			(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
			SetActiveController(MEAS_XYZ_CONTROLLER, true);
		}
	}
	//ExecuteAll();
}


// ================================================================================================
// Description: F2
// ================================================================================================
void DCP::Meas3DController::OnF2Pressed()
{
	if(m_bPointMenu)
	{	

		// add point

		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		char buffer[POINT_ID_BUFF_LEN]; buffer[0] = '\0';

#if DCP_USE_JSON_DATABASE
		if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
		{
			DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
				dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
			if (jdb && jdb->isJobLoaded())
			{
				int n = jdb->getJobPointsCount();
				sprintf(buffer, "P%d", n + 1);
				DCP::InputTextModel* pModel = new InputTextModel;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = static_cast<int>(DCP_POINT_ID_LENGTH);
				pModel->m_StrText = StringC(buffer);
				if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pDlg->GetModel()));
				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);
				m_bPointMenu = false;
				show_function_keys();
			}
			else
			{
				msg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
				msgbox->ShowMessageOk(msg);
			}
		}
		else
#endif
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pDataModel->m_pFileFunc->get_next_point_id(buffer);

			//if(get_AUTO_MATCH() == FALSE)
			if(!m_pDlg->GetModel()->m_nAutoMatch)
			{
					/*
					add_point(FormFile_,buff);
					{
						DSP_MODE = SINGLE;
					}
					*/
				DCP::InputTextModel* pModel = new InputTextModel;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
				pModel->m_StrTitle = GetTitle();
				pModel->m_iTextLength = static_cast<int>(DCP_POINT_ID_LENGTH);
				pModel->m_StrText = StringC(buffer);

				if ( nullptr == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
				{
					USER_APP_VERIFY( false );
					return;
				}

				if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
				{
					(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pDlg->GetModel() ));
				}

				(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
				SetActiveController(INPUT_TEXT_CONTROLLER, true);

				m_bPointMenu = false;	
				show_function_keys();

			}
			else
			{
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_ADD_POINT_TOK);
				msgbox->ShowMessageOk(msg);
				//(void ) msgbox(TXT_NIL_TOKEN, M_CANNOT_ADDPNT_TOK,MB_OK);
			}
		}
		else
		{
			msg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgbox->ShowMessageOk(msg);
			//(void ) msgbox(TXT_NIL_TOKEN, M_3DFILE_ISNOT_OPEN_TOK,MB_OK);
		}
	}
	else
	{
		if(m_pCommon->check_edm_mode())
		{
			DisableFunctionKey(FK1);
			DisableFunctionKey(FK2);
			DisableFunctionKey(FK3);
			DisableFunctionKey(FK4);
			DisableFunctionKey(FK5);
			DisableFunctionKey(FK6);

			// DIST
			DCP::MeasDistModel* pModel = new MeasDistModel;

			if(GetController(MEAS_DIST_CONTROLLER) == nullptr)
			{
				(void)AddController( MEAS_DIST_CONTROLLER, new DCP::MeasDistController(m_pDlg->GetModel()));
			}
			(void)GetController( MEAS_DIST_CONTROLLER )->SetModel( pModel);
			SetActiveController(MEAS_DIST_CONTROLLER, true);
		}
	}

} 

// ================================================================================================
// Description: F3
// ================================================================================================
void DCP::Meas3DController::OnF3Pressed()
{
	unsigned short unMeasHandle =
		TPI::SensorC::Instance()->GetMeasHandleLast();
	//CPI::SensorC::GetInstance()->GetMeasHandleLast();

	TPI::MeasDataC oMeasData;
	TPI::SensorC::Instance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
	//CPI::SensorC::GetInstance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
	double x = oMeasData.GetHorizontalAngle();
	double y = oMeasData.GetVerticalAngle();

	if(m_bPointMenu) 
	{ 
#if DCP_USE_JSON_DATABASE
		if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
		{
			DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
				dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
			int nPts = jdb ? jdb->getJobPointsCount() : 0;
			if (m_pDlg->GetModel()->m_currentPointIndex < nPts)
			{
				m_pDlg->GetModel()->m_currentPointIndex++;
				m_pDataModel->DSP_MODE = SINGLE;
				m_pDlg->RefreshControls();
			}
			else
				GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_LAST_POINT_TOK));
		}
		else
#endif
		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			if(m_pDataModel->m_pFileFunc->form_next_pnt())
			{
				m_pDataModel->DSP_MODE = SINGLE;
			}
			else
			{
				 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_LAST_POINT_TOK));
	
			}
			m_pDlg->RefreshControls();
		}
		else
		{
			msg.LoadTxt(AT_DCP06,  M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgbox->ShowMessageOk(msg);
		}

		//m_pDlg->PointNext();
		m_bPointMenu = false;	
		show_function_keys();
	}
	else
	{
		m_bPointMenu = true;
		show_function_keys();
	}
}

// ================================================================================================
// Description: F4
// ================================================================================================
void DCP::Meas3DController::OnF4Pressed()
{

	if(m_bPointMenu)
	{
		// swap
						//if(read_allow_edit())
						if(1)
						{
							m_pDataModel->set_xyz_mea_ptr();
							m_pDataModel->set_xyz_des_ptr();
							char temp_x[XYZ_VALUE_BUFF_LEN]; 
							sprintf(temp_x,"%9.9s",m_pDataModel->xmea_ptr);
							sprintf(m_pDataModel->xmea_ptr,"%9.9s",m_pDataModel->xdes_ptr);
							sprintf(m_pDataModel->xdes_ptr,"%9.9s",temp_x);

							sprintf(temp_x,"%9.9s",m_pDataModel->ymea_ptr);
							sprintf(m_pDataModel->ymea_ptr,"%9.9s",m_pDataModel->ydes_ptr);
							sprintf(m_pDataModel->ydes_ptr,"%9.9s",temp_x);

							sprintf(temp_x,"%9.9s",m_pDataModel->zmea_ptr);
							sprintf(m_pDataModel->zmea_ptr,"%9.9s",m_pDataModel->zdes_ptr);
							sprintf(m_pDataModel->zdes_ptr,"%9.9s",temp_x);
							m_pDataModel->save_point();
							m_pDlg->RefreshControls();
							// form_save_pnt(FormFile_);
						}
						else
						{
							//msgbox(TXT_NIL_TOKEN, M_EDIT_NOT_ALLOWED_TOK,MB_OK);
						}

		m_bPointMenu = false;	
		show_function_keys();
	}
	else
	{	
		// AIM
		if(!m_bShaft)
		{
			m_pDataModel->set_xyz_des_ptr();
			if(!m_pDlg->GetModel()->m_nDesignValues)	
				return;

			if(m_pCommon->strblank(m_pDataModel->xdes_ptr) ||m_pCommon->strblank(m_pDataModel->ydes_ptr) ||m_pCommon->strblank(m_pDataModel->zdes_ptr))
				return;
			
			//set_aim(atof(m_pDataModel->xdes_ptr),atof(m_pDataModel->ydes_ptr),atof(m_pDataModel->zdes_ptr), m_pDlg->GetModel()->active_coodinate_system);		
			if(GetController(AIM_CONTROLLER) == nullptr)
			{
				(void)AddController( AIM_CONTROLLER, new DCP::AimController(	atof(m_pDataModel->xdes_ptr),
																					atof(m_pDataModel->ydes_ptr),
																					atof(m_pDataModel->zdes_ptr),
																					m_pDlg->GetModel()->active_coodinate_system) );
			}

			(void)GetController( AIM_CONTROLLER )->SetModel(m_pDlg->GetModel());
			SetActiveController(AIM_CONTROLLER, true);
		}

		// SHAFT
		else
		{
			char pid[10];
			sprintf(pid,"%-s",m_pDataModel->pid_ptr);
			m_pCommon->strbtrim(pid);
			short iLen = (short) strlen(pid);
			if(iLen > 0)
			{
				if(pid[iLen-1] == 'd' || pid[iLen-1] == 'D')
				{
					StringC sMsg;
					sMsg.LoadTxt(AT_DCP06, M_DCP_CANNOT_SHAFT_POINT_TOK);
					msgbox->ShowMessageOk(sMsg);
					return;
				}
			}
			ShaftLineModel *pModel = new ShaftLineModel();
			sprintf(pModel->pid,"%6.6s", m_pDataModel->pid_ptr);

			//set_aim(atof(m_pDataModel->xdes_ptr),atof(m_pDataModel->ydes_ptr),atof(m_pDataModel->zdes_ptr), m_pDlg->GetModel()->active_coodinate_system);		
			if(GetController(SHAFT_ALIGMENT_LINE_CONTROLLER) == nullptr)
			{
				(void)AddController( SHAFT_ALIGMENT_LINE_CONTROLLER, new DCP::ShaftLineController(m_pDlg->GetModel()));
			}

			(void)GetController( SHAFT_ALIGMENT_LINE_CONTROLLER )->SetModel(pModel);
			SetActiveController(SHAFT_ALIGMENT_LINE_CONTROLLER, true);

		}
	}
}



// ================================================================================================
// Description: F5
// ================================================================================================
void DCP::Meas3DController::OnF5Pressed()
{
	if(m_bPointMenu)
	{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		if(m_pDataModel->m_pFileFunc->IsOpen() && m_pDataModel->file_updated == 1)
			m_pDataModel->m_pFileFunc->add_header_to_adf();

		if(GetController(FILE_CONTROLLER) == nullptr)
		{
			(void)AddController( FILE_CONTROLLER, new DCP::FileController(m_pDlg->GetModel()) );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( FILE_CONTROLLER )->SetModel(m_pDlg->GetModel());
		SetActiveController(FILE_CONTROLLER, true);

		m_bPointMenu = false;	
		show_function_keys();

	}
	else
	{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		if(m_pDataModel->m_bJobOpen || m_pDataModel->m_pFileFunc->IsOpen())
			ShowSelectPointDlg();
		else
		{
			msg.LoadTxt(AT_DCP06,  M_DCP_3DFILE_ISNOT_OPEN_TOK);
			msgbox->ShowMessageOk(msg);
		}
	}
}

// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::Meas3DController::OnF6Pressed()
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
	if(!m_bCamera)
	   (void)Close(EC_KEY_CONT);
	else
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(_3DMEAS_DLG);
		show_function_keys();
	}
}
// ================================================================================================
// Description: SHF2
// ================================================================================================
void DCP::Meas3DController::OnSHF1Pressed()
{
	
	/* CAPTIVATE TBD 
	if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
    {
		if(poVideoDlg == 0)
		{
			poVideoDlg = new ABL::VideoDialogC( AT_DCP06);
			CFA::CameraT cameraType = poVideoDlg->GetCameraType();
			
			poVideoDlg->SetId(CAMERA_DLG);
			poVideoDlg->SetCameraType(CFA::CT_OVC);
		}
		   // (void)AddPage(poVideoDlg, StringC(L"Video"));
		(void)AddDialog( CAMERA_DLG, poVideoDlg, true );

		SetActiveDialog(CAMERA_DLG, true);
		m_bCamera = true;
		show_function_keys();
    }
	*/
		if(poVideoDlg == 0)
		{
			poVideoDlg = new GSV::GeospatialViewDialogC();//GSV::GeospatialViewCtrlC();
			APP_ASSERT_RET_VOID(poVideoDlg);
			poVideoDlg->AddView(GSV::GSV_Camera);
			poVideoDlg->SetId(CAMERA_DLG);
		}
		(void)AddDialog( CAMERA_DLG, poVideoDlg, true );
		SetActiveDialog(CAMERA_DLG, true);
		m_bCamera = true;
		show_function_keys();
}
// ================================================================================================
// Description: SHF2
// ================================================================================================
void DCP::Meas3DController::OnSHF2Pressed()
{
	if(m_bPointMenu)
	{
		// CLEAR	
		if(!m_pDataModel->m_pFileFunc->IsOpen())
		{
			if(m_pDataModel->DSP_MODE == SINGLE)
			{
				m_pCommon->empty_xyz_buffers(m_pDataModel->bXmea, m_pDataModel->bYmea,m_pDataModel->bZmea,9);
				sprintf(m_pDataModel->note_ptr,"%-6.6s","");

			}
		}
		else
		{
			if(m_pDataModel->DSP_MODE == SINGLE)
			{
				m_pCommon->empty_xyz_buffers(m_pDataModel->m_pFileFunc->xmea_front,
					m_pDataModel->m_pFileFunc->ymea_front,
					m_pDataModel->m_pFileFunc->zmea_front,9);
				sprintf(m_pDataModel->m_pFileFunc->note_front,"%-6.6s","");
				//memset(m_pDataModel->m_pFileFunc->note_front,0,7);
			}
			m_pDataModel->save_point();
			
							
		}
		m_pDlg->RefreshControls();
		m_bPointMenu = false;	
		show_function_keys();
	}
	else
	{
		// INIT
		if(GetController(INIT_CONTROLLER) == nullptr)
		{
			(void)AddController( INIT_CONTROLLER, new DCP::InitializationController );
		}
		(void)GetController( INIT_CONTROLLER )->SetModel( m_pDlg->GetModel());
		SetActiveController(INIT_CONTROLLER, true);
	}
}

// ================================================================================================
// Description: SHF3
// ================================================================================================
void DCP::Meas3DController::OnSHF3Pressed()
{
	if(GetController(SPECIAL_MENU_CONTROLLER) == nullptr)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::SpecialMenuController (m_pDlg->GetModel()));
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);
}


// ================================================================================================
// Description: SHF4
// ================================================================================================
void DCP::Meas3DController::OnSHF4Pressed()
{
	ShowToolDlg();
	
}

// ================================================================================================
// Description: SHF5
// ================================================================================================
void DCP::Meas3DController::OnSHF5Pressed()
{
	
	CalculationDist2PointsModel* pModel = new CalculationDist2PointsModel;
	int iCount = m_pDataModel->m_pFileFunc->GetPointList(&pModel->points[0],MAX_SELECT_POINTS, BOTH);
	pModel->sFile = StringC(m_pDataModel->m_pFileFunc->getFileName());
	pModel->m_iCounts = iCount;
	pModel->pAdfFile = m_pDataModel->m_pFileFunc;

	// title for calc2points display
	StringC sTemp;
	sTemp.LoadTxt(AT_DCP06, T_CALC_DISTANCE_2_POINTS_TOK);
	pModel->sTitle = GetTitle();
	pModel->sTitle += sTemp;

	if(GetController(CALC_2_POINTS_CONTROLLER) == nullptr)
	{
		(void)AddController( CALC_2_POINTS_CONTROLLER, new DCP::CalculationDist2PointsController(m_pDlg->GetModel()));
	}
	(void)GetController( CALC_2_POINTS_CONTROLLER )->SetModel(pModel);
	SetActiveController(CALC_2_POINTS_CONTROLLER, true);
}

// ================================================================================================
// Description: SHF6
// ================================================================================================
void DCP::Meas3DController::OnSHF6Pressed()
{
		// I<>II
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(302, poChangeFace);
		SetActiveController(302, true);

}
// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::Meas3DController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(_3DMEAS_DLG);
		m_bCamera = false;
		show_function_keys();
	}
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::Meas3DController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// File
	if(lCtrlID == FILE_CONTROLLER) // && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->file_updated = 0;	
		if(lExitCode == EC_KEY_CONT)
			m_pDataModel->m_pFileFunc->setFile(m_pDlg->GetModel()->ADFFileName);
	}

	// MEASURE DIST
	if(lCtrlID == MEAS_DIST_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{
			DCP::MeasDistModel* pModel = (DCP::MeasDistModel*) GetController( MEAS_DIST_CONTROLLER )->GetModel();
			int x;
			x=1;
		}
	}
	// MEASURE XYZ
	if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);
		
		if(lExitCode == EC_KEY_CONT)
		{
		DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
		m_pDataModel->x_temp = pModel->m_dX;
		m_pDataModel->y_temp = pModel->m_dY;
		m_pDataModel->z_temp= pModel->m_dZ;

		if(m_pDlg->GetModel()->m_nAutoMatch)
		{
			int pno = m_pDataModel->m_pFileFunc->find_point(m_pDataModel->x_temp, m_pDataModel->y_temp, m_pDataModel->z_temp);

			if( pno > 0)
			{
					m_pDataModel->m_pFileFunc->form_pnt((int) pno);
					m_pDataModel->set_xyz_des_ptr();
					m_pDataModel->set_xyz_mea_ptr();

					// show dialog
					AutoMatchModel* pModel = new AutoMatchModel;
					pModel->pno = pno;
					pModel->xdsg = atof(m_pDataModel->xdes_ptr);
					// 280110
					//pModel->xmea = atof(m_pDataModel->xmea_ptr);
					pModel->xmea = m_pDataModel->x_temp;

					pModel->ydsg = atof(m_pDataModel->ydes_ptr);
					// 280110
					//pModel->ymea = atof(m_pDataModel->ymea_ptr);
					pModel->ymea = m_pDataModel->y_temp;

					pModel->zdsg = atof(m_pDataModel->zdes_ptr);
					// 280110
					//pModel->zmea = atof(m_pDataModel->zmea_ptr);
					pModel->zmea = m_pDataModel->z_temp;

					pModel->pointid = StringC(m_pDataModel->pid_ptr);

					if(GetController(AUTOMATCH_CONTROLLER) == nullptr)
					{
						(void)AddController( AUTOMATCH_CONTROLLER, new DCP::AutoMatchController(m_pDlg->GetModel()));
					}
					(void)GetController( AUTOMATCH_CONTROLLER )->SetModel(pModel);
					SetActiveController(AUTOMATCH_CONTROLLER, true);

					/*	
					m_pDataModel->m_pFileFunc->form_pnt(pno);
					m_pDataModel->DSP_MODE = SINGLE;
					m_pDataModel->set_xyz_mea_ptr();
					
					if(m_pDlg->GetModel()->m_nOverWriteInfo)
					//if(get_show_overwrite_info() == TRUE)
					{
						if( !m_pCommon->strblank(m_pDataModel->xmea_ptr) || 
							!m_pCommon->strblank(m_pDataModel->ymea_ptr) || 
							!m_pCommon->strblank(m_pDataModel->zmea_ptr))
						{
							msg.LoadTxt(AT_DCP06,M_DCP_OVERWRITE_POINT_TOK);
							msg.Format(msg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
							if(!msgbox->ShowMessageYesNo(msg))
							//if(msgbox1(TXT_NIL_TOKEN, M_OVERWRITE_POINT_TOK,(void *) pid_ptr, MB_YESNO) == FALSE)
							{
								m_pDlg->RefreshControls();
								return;
							}
							m_pDlg->RefreshControls();
						}
					}
							
					m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
					m_pDataModel->save_point();
					*/
				}	
				//MMI_ChangeDlgCaption(DialogId, PROG_CAPTION_TOK, T_3DMEAS_TOK);
			}
			else
			{
				m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
				m_pDataModel->save_point();

				// Tool values to file
				if(m_pDataModel->m_pFileFunc->IsOpen())
				{
					AmsToolLog tool_log(m_pDataModel->m_pFileFunc->getFileName(),m_pDataModel->pid_ptr,m_pCommon,m_pDlg->GetModel());
				}
			}
				
			// Auto Increment codes
#if DCP_USE_JSON_DATABASE
			if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel() && m_pDlg->GetModel()->m_nAutoIncrement)
			{
				DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
					dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
				if (jdb && jdb->isJobLoaded())
				{
					int nPts = jdb->getJobPointsCount();
					if (m_pDlg->GetModel()->m_currentPointIndex < nPts)
						m_pDlg->GetModel()->m_currentPointIndex++;
					else
					{
						char buf[32]; sprintf(buf, "P%d", nPts + 1);
						jdb->addPoint(buf, DCP::Database::PointData());
						m_pDlg->GetModel()->m_currentPointIndex = nPts + 1;
					}
					m_pDataModel->DSP_MODE = SINGLE;
					m_pDlg->RefreshControls();
				}
			}
			else
#endif
			if(m_pDataModel->m_pFileFunc->IsOpen())
			{
				if(m_pDlg->GetModel()->m_nAutoIncrement)
				{
					short ret = m_pDataModel->m_pFileFunc->form_next_pnt();
					m_pDataModel->DSP_MODE = SINGLE;
					if(ret == 0)
					{
						char buff[20]; buff[0] = '\0';
						m_pDataModel->m_pFileFunc->add_point(buff);								
					}
				}
			}
		}	
		//m_pDCP053DMeasDlg->update_meas_values(x,y,z, POINT_MEASURED);
	}
	else if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{	/*
		MsgBox MsgBox;
		char temp[100];
		sprintf(temp,"%s (%d)", "Exit code", lExitCode);
		MsgBox.ShowMessageOk(StringC(temp));
		*/
	}

	if(lCtrlID == AUTOMATCH_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::AutoMatchModel* pModel = (DCP::AutoMatchModel*) GetController( AUTOMATCH_CONTROLLER )->GetModel();		
			short pno = pModel->pno;
			
			m_pDataModel->m_pFileFunc->form_pnt(pno);
			m_pDataModel->DSP_MODE = SINGLE;
			m_pDataModel->set_xyz_mea_ptr();
					
			if(m_pDlg->GetModel()->m_nOverWriteInfo)
					//if(get_show_overwrite_info() == TRUE)
			{
				if( !m_pCommon->strblank(m_pDataModel->xmea_ptr) || 
					!m_pCommon->strblank(m_pDataModel->ymea_ptr) || 
					!m_pCommon->strblank(m_pDataModel->zmea_ptr))
					{
						msg.LoadTxt(AT_DCP06,M_DCP_OVERWRITE_POINT_TOK);
						msg.Format(msg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
						if(!msgbox->ShowMessageYesNo(msg))
							//if(msgbox1(TXT_NIL_TOKEN, M_OVERWRITE_POINT_TOK,(void *) pid_ptr, MB_YESNO) == FALSE)
						{
			
						}
						else
						{
							m_pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
							m_pDataModel->save_point();

							// Tool values to file
							if(m_pDataModel->m_pFileFunc->IsOpen())
							{
								AmsToolLog tool_log(m_pDataModel->m_pFileFunc->getFileName(),m_pDataModel->pid_ptr,m_pCommon,m_pDlg->GetModel());
							}

						}
					}
					else
					{
							m_pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
							m_pDataModel->save_point();
							// Tool values to file
							if(m_pDataModel->m_pFileFunc->IsOpen())
							{
								AmsToolLog tool_log(m_pDataModel->m_pFileFunc->getFileName(),m_pDataModel->pid_ptr,m_pCommon,m_pDlg->GetModel());
							}
					}

			}
			else
			{
				m_pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
				m_pDataModel->save_point();

				// Tool values to file
				if(m_pDataModel->m_pFileFunc->IsOpen())
				{
					AmsToolLog tool_log(m_pDataModel->m_pFileFunc->getFileName(),m_pDataModel->pid_ptr,m_pCommon,m_pDlg->GetModel());
				}

			}
		}


	// select point
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectPointModel* pModel = (DCP::SelectPointModel*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
#if DCP_USE_JSON_DATABASE
		if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
			m_pDlg->GetModel()->m_currentPointIndex = pModel->m_iSelectedId;
		else
#endif
		m_pDataModel->m_pFileFunc->form_pnt(pModel->m_iSelectedId);
		m_pDataModel->DSP_MODE = SINGLE;
		m_pDlg->RefreshControls();
	}
	// SPECIAL MENU
	if(lCtrlID == SPECIAL_MENU_CONTROLLER && lExitCode != EC_KEY_ESC )
	{	
			if(lExitCode == HIDDEN_POINT)
				ShowHiddenPointDlg();

			else if(lExitCode == X_OR_Y_OR_Z)
				ShowXorYorZDlg();

			else if(lExitCode == CIRCLE)
				ShowCircleDlg();
 
			else if(lExitCode == SEPARATE_REC)
				ShowSeparateRecDlg();

			else if(lExitCode == HOME_POINTS)
				ShowHomePointsDlg();
			else if(lExitCode == MID_POINT)
				ShowMidPointDlg();
		}
	// Add point
	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char buffer[POINT_ID_BUFF_LEN]; buffer[0] = '\0';
			m_pCommon->convert_to_ascii(strNewFile, buffer, DCP_POINT_ID_LENGTH + 1);
			m_pCommon->strbtrim(buffer);
			std::string pointId(buffer);
#if DCP_USE_JSON_DATABASE
			if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
			{
				DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
					dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
				if (jdb && jdb->isJobLoaded() && !pointId.empty())
				{
					DCP::Database::PointData data;
					jdb->addPoint(pointId, data);
					int n = jdb->getJobPointsCount();
					m_pDlg->GetModel()->m_currentPointIndex = n;
					m_pDlg->RefreshControls();
				}
			}
			else
#endif
			m_pDataModel->m_pFileFunc->add_point(buffer);
	}

	// CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( CIRCLE_CONTROLLER )->GetModel();		
		m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		m_pDataModel->y_temp = pModel->m_pPointBuff[0].y;
		m_pDataModel->z_temp= pModel->m_pPointBuff[0].z;
			
		m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		// also diamter
		sprintf(m_pDataModel->note_ptr,"%6.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].diameter);
		m_pDataModel->save_point();
	}

	// SEPARATE
	if(lCtrlID == SEPARATE_RECORDING_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( SEPARATE_RECORDING_CONTROLLER )->GetModel();		
		
		if(pModel->m_pPointBuff[0].xsta)
			sprintf(m_pDataModel->xmea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].x);

		if(pModel->m_pPointBuff[0].ysta)
			sprintf(m_pDataModel->ymea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].y);
		
		if(pModel->m_pPointBuff[0].zsta)
			sprintf(m_pDataModel->zmea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].z);

		// also diamter
		sprintf(m_pDataModel->note_ptr,"%6.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].diameter);
		m_pDataModel->save_point();
	}
	//hiddenpoint HIDDENPOINT_CONTROLLER
	if(lCtrlID == HIDDENPOINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( HIDDENPOINT_CONTROLLER )->GetModel();		
		m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		m_pDataModel->y_temp = pModel->m_pPointBuff[0].y;
		m_pDataModel->z_temp= pModel->m_pPointBuff[0].z;
			
		m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		m_pDataModel->save_point();
	}

	// XorYorZ XYZ_CONTROLLER
	if(lCtrlID == XYZ_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( XYZ_CONTROLLER )->GetModel();		
		
		if(pModel->m_pPointBuff[0].xsta)
			sprintf(m_pDataModel->xmea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].x);

		if(pModel->m_pPointBuff[0].ysta)
			sprintf(m_pDataModel->ymea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].y);
		
		if(pModel->m_pPointBuff[0].zsta)
			sprintf(m_pDataModel->zmea_ptr,"%9.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].z);

		// also diamter
		sprintf(m_pDataModel->note_ptr,"%6.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].diameter);
		m_pDataModel->save_point();
	}

	if(lCtrlID == MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( MID_POINT_CONTROLLER )->GetModel();	
		m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		m_pDataModel->y_temp =  pModel->m_pPointBuff[0].y;
		m_pDataModel->z_temp=   pModel->m_pPointBuff[0].z;
		
		m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
	
		m_pDataModel->save_point();
	}


	if(lCtrlID == SHAFT_ALIGMENT_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		char pid[10];
		
		ShaftLineModel *pModel = (DCP::ShaftLineModel*) GetController( SHAFT_ALIGMENT_LINE_CONTROLLER )->GetModel();	
		sprintf(pid,"%-s", pModel->pid);
		m_pCommon->strbtrim(pid);

		m_pDataModel->x_temp = pModel->cx;
		m_pDataModel->y_temp = pModel->cy;
		m_pDataModel->z_temp= pModel->cz;
			
		m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		// also diamter
		sprintf(m_pDataModel->note_ptr,"%6.*f",m_pDlg->GetModel()->m_nDecimals,pModel->diameter);
		m_pDataModel->save_point();


		// find 'd' point to save shaft
		short iLen = (short) strlen(pid);
		if(iLen <= 5) 
		{ 
			pid[iLen] = 'D';
			pid[iLen+1] = '\0';
		}

		if(m_pDataModel->m_pFileFunc->seek_pid(pid) == 0)
		m_pDataModel->m_pFileFunc->add_point(pid);

	
		m_pDataModel->x_temp = pModel->x;
		m_pDataModel->y_temp = pModel->y;
		m_pDataModel->z_temp= pModel->z;
			
		m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		if(pModel->active_line == X_LINE)
		{
			sprintf(m_pDataModel->xmea_ptr,"%9.9s"," ");	
		}
		else if(pModel->active_line == Y_LINE)
		{
			sprintf(m_pDataModel->ymea_ptr,"%9.9s"," ");
		}
		else if(pModel->active_line == Z_LINE)
		{
			sprintf(m_pDataModel->zmea_ptr,"%9.9s"," ");
		}
		
		sprintf(m_pDataModel->note_ptr,"%-6.6s","mm/m");
		
		m_pDataModel->save_point();
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: ConfirmClose
// ================================================================================================
bool DCP::Meas3DController::ConfirmClose(bool bEsc)
{
	if(bEsc && m_bPointMenu)
	{
		m_bPointMenu = false;
		show_function_keys();
		return false;
	}
	else
	{
		return true;
	}
}

// ================================================================================================
// Description: ShowSelectPointDlg
// ================================================================================================
void DCP::Meas3DController::ShowSelectPointDlg()
{
#if DCP_USE_JSON_DATABASE
	if (m_pDataModel->m_bJobOpen && m_pDlg->GetModel())
	{
		DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded())
		{
			DCP::SelectPointModel* pModel = new SelectPointModel;
			int iCount = jdb->getPointListAsSelectPoint(&pModel->points[0], MAX_SELECT_POINTS);
			pModel->m_iCounts = iCount;
			pModel->m_iSelectedId = m_pDlg->GetModel()->m_currentPointIndex;
			if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
				(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
			(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
			SetActiveController(SELECT_POINT_CONTROLLER, true);
		}
		return;
	}
#endif
	if(m_pDataModel->m_pFileFunc->IsOpen())
	{
		DCP::SelectPointModel* pModel = new SelectPointModel;
		int iCount = m_pDataModel->m_pFileFunc->GetPointList(&pModel->points[0],MAX_SELECT_POINTS);
		pModel->m_iCounts = iCount;
		pModel->m_iSelectedId = m_pDataModel->m_pFileFunc->active_point_front;
		if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
	}
}

// ================================================================================================
// Description: ShowHiddenPointDlg
// ================================================================================================
void DCP::Meas3DController::ShowHiddenPointDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;
	sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", m_pDataModel->pid_ptr);

	if(GetController(HIDDENPOINT_CONTROLLER) == nullptr)
	{
		(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::HiddenPointController(m_pDlg->GetModel()));
	}
	(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel( pModel);
	SetActiveController(HIDDENPOINT_CONTROLLER, true);
}

// ================================================================================================
// Description: ShowXorYorZDlg
// ================================================================================================
void DCP::Meas3DController::ShowXorYorZDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;
	sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", m_pDataModel->pid_ptr);
	if(GetController(XYZ_CONTROLLER) == nullptr)
	{
		(void)AddController( XYZ_CONTROLLER, new DCP::XYZController(m_pDlg->GetModel()) );
	}
	(void)GetController( XYZ_CONTROLLER )->SetModel( pModel);
	SetActiveController(XYZ_CONTROLLER, true);
}

// ================================================================================================
// Description: ShowCircleDlg
// ================================================================================================
void DCP::Meas3DController::ShowCircleDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;
	if(GetController(CIRCLE_CONTROLLER) == nullptr)
	{
		(void)AddController( CIRCLE_CONTROLLER, new DCP::CircleController(m_pDlg->GetModel()));
	}
	(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel);
	SetActiveController(CIRCLE_CONTROLLER, true);
}

// ================================================================================================
// Description: ShowSeparateRecDlg
// ================================================================================================
void DCP::Meas3DController::ShowSeparateRecDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;

	sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", m_pDataModel->pid_ptr);

	//pModel->m_pPointBuff[0].xsta = pMeasModel->
	if(GetController(SEPARATE_RECORDING_CONTROLLER) == nullptr)
	{
		(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::SeparateRecController(m_pDlg->GetModel()));
	}
	(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel( pModel);
	SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
}

// ================================================================================================
// Description: ShowHomePointsDlg
// ================================================================================================
void DCP::Meas3DController::ShowHomePointsDlg()
{
	if(GetController(HOME_POINTS_CONTROLLER) == nullptr)
	{
		(void)AddController( HOME_POINTS_CONTROLLER, new DCP::HomePointsController(m_pDlg->GetModel()) );
	}
	(void)GetController( HOME_POINTS_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(HOME_POINTS_CONTROLLER, true); 
}

// ================================================================================================
// Description: ShowHomePointsDlg
// ================================================================================================
void DCP::Meas3DController::ShowMidPointDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;

	sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", m_pDataModel->pid_ptr);

	//pModel->m_pPointBuff[0].xsta = pMeasModel->
	if(GetController(MID_POINT_CONTROLLER) == nullptr)
	{
		(void)AddController( MID_POINT_CONTROLLER, new DCP::MidPointController(m_pDlg->GetModel()));
	}
	(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
	SetActiveController(MID_POINT_CONTROLLER, true);

		//DCP::MeasureModel* pModel = new MeasureModel;
		//pModel->m_iPointsCount = 2;
		//pModel->m_iMaxPoint = 2;
		//pModel->m_iMinPoint = 2;
	
		//memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * 2);
	
		//if(GetController(MID_POINT_CONTROLLER) == nullptr)
		//{
		//	(void)AddController( MID_POINT_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
		//}

		//(void)GetController(MID_POINT_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_MID_POINT_TOK));

		//(void)GetController( MID_POINT_CONTROLLER )->SetModel(pModel);
		//SetActiveController(MID_POINT_CONTROLLER, true);
}

// ================================================================================================
// Description: ShowToolDlg
// ================================================================================================
void DCP::Meas3DController::ShowToolDlg()
{
	if(GetController(TOOL_CONTROLLER) == nullptr)
	{
		(void)AddController( TOOL_CONTROLLER, new DCP::ToolController());
	}
	(void)GetController( TOOL_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(TOOL_CONTROLLER, true);
}


// ================================================================================================
// ====================================  Meas3DModel           ==============================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Meas3DModel::Meas3DModel(DCP::Model* pModel):m_pModel(pModel),m_pCommon(0),m_bJobOpen(false)
{
	m_pFileFunc = new AdfFileFunc(m_pModel);
	m_pFileFunc->always_single = 1;
	m_pCommon = new Common(m_pModel);

	DSP_MODE = SINGLE;
	Xdes = 0.0;
	Ydes = 0.0;
	Zdes = 0.0;
	set_xyz_mea_ptr();
	set_xyz_des_ptr();
	file_updated = 0;
	bFid[ 0 ] = '\0'; 
	bPid[ 0 ] = '\0'; 
	bXmea[ 0 ] = '\0'; 
	bYmea[ 0 ] = '\0'; 
	bZmea[ 0 ] = '\0'; 

	bXdes[ 0 ] = '\0'; 
	bYdes[ 0 ] = '\0'; 
	bZdes[ 0 ] = '\0'; 

	bXdif[ 0 ] = '\0'; 
	bYdif[ 0 ] = '\0'; 
	bZdif[ 0 ] = '\0'; 

	bInfo1[ 0 ] = '\0'; 
	bInfo2[ 0 ] = '\0'; 
	bNote[ 0 ] = '\0'; 
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Meas3DModel::~Meas3DModel()
{
	if(m_pFileFunc)
	{	
		if(m_pFileFunc->IsOpen() && file_updated == 1)
			m_pFileFunc->add_header_to_adf();
		
		delete m_pFileFunc;
		m_pFileFunc = 0;
	}
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// ================================================================================================
// Description: set_xyz_mea_ptr
// ================================================================================================
void DCP::Meas3DModel::set_xyz_mea_ptr()
{
	if (m_bJobOpen)
	{
		pid_ptr  = bPid;
		xmea_ptr = bXmea;
		ymea_ptr = bYmea;
		zmea_ptr = bZmea;
		note_ptr = bNote;
		return;
	}
	if(m_pFileFunc->IsOpen())
	{
		switch(DSP_MODE)
		{


			case FRONT:
			case SINGLE:

						xmea_ptr = &m_pFileFunc->xmea_front[0];
						ymea_ptr = &m_pFileFunc->ymea_front[0];
						zmea_ptr = &m_pFileFunc->zmea_front[0];
						pid_ptr  = &m_pFileFunc->pointid_front[0];
						note_ptr = &m_pFileFunc->note_front[0]; 
						break;
			default:
						pid_ptr  = bPid;
						xmea_ptr = bXmea;
						ymea_ptr = bYmea;
						zmea_ptr = bZmea;
						note_ptr = bNote;
						break;
		}
	}
	else
	{
		switch(DSP_MODE)
		{

			case SINGLE:
						pid_ptr  = bPid;
						xmea_ptr = bXmea;
						ymea_ptr = bYmea;
						zmea_ptr = bZmea;
						note_ptr = bNote;	
						break;
			default:
						pid_ptr  = bPid;
						xmea_ptr = bXmea;
						ymea_ptr = bYmea;
						zmea_ptr = bZmea;
						note_ptr = bNote;		
						break;
		}
	}
}



// ================================================================================================
// Description: set_xyz_des_ptr
// ================================================================================================
void DCP::Meas3DModel::set_xyz_des_ptr()
{
	if (m_bJobOpen)
	{
		xdes_ptr = bXdes;
		ydes_ptr = bYdes;
		zdes_ptr = bZdes;
		return;
	}
	if(m_pFileFunc->IsOpen())
	{
		switch(DSP_MODE)
		{
			case FRONT:
			case SINGLE:
						xdes_ptr = m_pFileFunc->xdes_front;
						ydes_ptr = m_pFileFunc->ydes_front;
						zdes_ptr = m_pFileFunc->zdes_front;
						break;
			/*
			case FRONTBACK:
			case BACK:
						xdes_ptr = FormFile_->xdes_back;
						ydes_ptr = FormFile_->ydes_back;
						zdes_ptr = FormFile_->zdes_back;
						break;
			*/
			default:
						xdes_ptr = bXdes;
						ydes_ptr = bYdes;
						zdes_ptr = bZdes;
						break;
		}
	}
	else
	{
		switch(DSP_MODE)
		{

			case SINGLE:
						xdes_ptr = bXdes;
						ydes_ptr = bYdes;
						zdes_ptr = bZdes;
						break;
			default:
						xdes_ptr = bXdes;
						ydes_ptr = bYdes;
						zdes_ptr = bZdes;
						break;
		}
	}
}

// ================================================================================================
// Description: save_point
// ================================================================================================
void DCP::Meas3DModel::save_point()
{
#if DCP_USE_JSON_DATABASE
	if (m_bJobOpen && m_pModel)
	{
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded())
		{
			char pidBuf[POINT_ID_BUFF_LEN];
			m_pCommon->strbtrim(pid_ptr);
			sprintf(pidBuf, "%.64s", pid_ptr);
			m_pCommon->strbtrim(pidBuf);
			std::string pointId(pidBuf);
			if (pointId.empty()) return;
			DCP::Database::PointData data;
			bool exists = jdb->getPoint(pointId, data);
			data.x_mea = m_pCommon->strblank(xmea_ptr) ? 0.0 : atof(xmea_ptr);
			data.y_mea = m_pCommon->strblank(ymea_ptr) ? 0.0 : atof(ymea_ptr);
			data.z_mea = m_pCommon->strblank(zmea_ptr) ? 0.0 : atof(zmea_ptr);
			data.x_dsg = m_pCommon->strblank(xdes_ptr) ? std::numeric_limits<double>::quiet_NaN() : atof(xdes_ptr);
			data.y_dsg = m_pCommon->strblank(ydes_ptr) ? std::numeric_limits<double>::quiet_NaN() : atof(ydes_ptr);
			data.z_dsg = m_pCommon->strblank(zdes_ptr) ? std::numeric_limits<double>::quiet_NaN() : atof(zdes_ptr);
			if (note_ptr && !m_pCommon->strblank(note_ptr)) { char n[16]; sprintf(n, "%.14s", note_ptr); m_pCommon->strbtrim(n); data.note = n; }
			if (exists) jdb->updatePoint(pointId, data);
			else jdb->addPoint(pointId, data);
			file_updated = 1;
		}
		return;
	}
#endif
	if(m_pFileFunc->IsOpen())
	{
		m_pFileFunc->form_save_pnt();
		file_updated = 1;
	}
}


void DCP::Meas3DModel::set_dcp05_model(Model *pModel)
{
	m_pModel = pModel;
}