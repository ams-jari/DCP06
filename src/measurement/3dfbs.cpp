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
#include <dcp06/measurement/3dfbs.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/core/Tool.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/core/SelectPoint.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <dcp06/core/Aim.hpp>
#include <dcp06/calculation/CalculationDist2Points.hpp>
#include <dcp06/measurement/SeparateRec.hpp>
#include <dcp06/measurement/HomePoints.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/core/AutoMatch.hpp>
#include <dcp06/measurement/MidPointController.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <UTL_StringFunctions.hpp>
#include <OBS_Observer.hpp>
//#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>

#include <GUI_Layout.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::Fbs3DDialog);
// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// ================================================================================================
// ====================================  Fbs3DDialog              ==============================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Fbs3DDialog::Fbs3DDialog(Fbs3DMeasModel* pDataModel):GUI::ModelHandlerC(),GUI::DialogC(),
			m_pDataModel(pDataModel),
			m_pFile(0),m_pPointId(0), m_pXDsg(0), m_pYDsg(0), m_pZDsg(0),
			m_pXAct(0),m_pYAct(0),m_pZAct(0),m_pXDev(0),m_pYDev(0),m_pZDev(0),
			m_pPointIdObserver(OBS_METHOD_TO_PARAM0(Fbs3DDialog, OnValueChanged), this),
			m_pXObserver(OBS_METHOD_TO_PARAM0(Fbs3DDialog, OnValueChanged), this),
			m_pYObserver(OBS_METHOD_TO_PARAM0(Fbs3DDialog, OnValueChanged), this),
			m_pZObserver(OBS_METHOD_TO_PARAM0(Fbs3DDialog, OnValueChanged), this),iInfoInd(0)
			//,m_pInfo(0)
{
     // VIVA REMOVED
	 //	SetTxtApplicationId(AT_DCP06);
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);

	// load title texts
	sFront.LoadTxt(AT_DCP06,T_DCP_3DROLLER_FRONT_TOK);
	sBack.LoadTxt(AT_DCP06,T_DCP_3DROLLER_BACK_TOK);
	sFrontBack.LoadTxt(AT_DCP06,T_DCP_3DROLLER_FRONT_BACK_TOK);
	sSingle.LoadTxt(AT_DCP06,T_DCP_3DROLLER_SINGLE_TOK);
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Fbs3DDialog::~Fbs3DDialog()
{
	//m_pTimer.KillTimer();
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::Fbs3DDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();

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

	// Add fields to dialogds
	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);

	// VIVA REMOVED
   // m_pFile->GetTextCtrl()->SetTextTok(P_DCP_3DFILE_SK_TOK);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));

	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//AddCtrl(m_pFile);
	
	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);

	// VIVA REMOVED
    //	m_pPointId->GetTextCtrl()->SetTextTok(P_DCP_POINT_ID_SK_TOK);
    m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_SK_TOK));


	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
	//m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); // CAPTIVATE
	//AddCtrl(m_pPointId);

	//SetColonPosLong( GUI::StandardDialogC::CP_10 );

	m_pXDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pXDsg->SetId(eXDsg);
	
	// VIVA REMOVED
	//m_pXDsg->GetTextCtrl()->SetTextTok(P_DCP_X_DSG_TOK);
	m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_DSG_TOK));

	m_pXDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short) GetModel()->m_nDecimals);
	//m_pXDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); // CAPTIVATE
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);

	// VIVA REMOVED
	// m_pXDsg->SetAutoColon(false);

	// VIVA REMOVED
	// m_pXDsg->SetColonPosition(9 * 11);
	m_pXDsg->SetEmptyAllowed(true);
	//AddCtrl(m_pXDsg);

	m_pYDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pYDsg->SetId(eYDsg);
	
	// VIVA REMOVED
	// m_pYDsg->GetTextCtrl()->SetTextTok(P_DCP_Y_DSG_TOK);
	m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_DSG_TOK));
	
	m_pYDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	//m_pYDsg->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pYDsg->SetEmptyAllowed(true);

	// VIVA REMOVED
	//m_pYDsg->SetAutoColon(false);
	//m_pYDsg->SetColonPosition(9 * 11);
	//m_pYDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//AddCtrl(m_pYDsg);

	m_pZDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZDsg->SetId(eZDsg);

	// VIVA REMOVED
	// m_pZDsg->GetTextCtrl()->SetTextTok(P_DCP_Z_DSG_TOK);
	m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_DSG_TOK));

	m_pZDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pZDsg->SetEmptyAllowed(true);

	// VIVA REMOVED
	// m_pZDsg->SetAutoColon(false);
	// m_pZDsg->SetColonPosition(9 * 11);
	//m_pZDsg->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
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

	// VIVA REMOVED
	// m_pXActDev->GetTextCtrl()->SetTextTok(P_DCP_X_ACT_DEV_TOK);
	m_pXActDev->SetText(StringC(AT_DCP06,P_DCP_X_ACT_DEV_TOK));

	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXActDev->SetEmptyAllowed(true);
	
	// VIVA REMOVED
	// m_pXActDev->SetAutoColon(false);
	// m_pXActDev->SetColonPosition(9 * 11);
	m_pXActDev->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	AddCtrl(m_pXActDev);

	m_pYActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYActDev->SetId(eYActDev);
	
	// VIVA REMOVED
	//m_pYActDev->GetTextCtrl()->SetTextTok(P_DCP_Y_ACT_DEV_TOK);
	m_pYActDev->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_DEV_TOK));

	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYActDev->SetEmptyAllowed(true);

	// VIVA REMOVED
	// m_pYActDev->SetAutoColon(false);
	// m_pYActDev->SetColonPosition(9 * 11);
	m_pYActDev->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT);
	AddCtrl(m_pYActDev);

	m_pZActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZActDev->SetId(eZActDev);
	
	// VIVA REMOVED
	//m_pZActDev->GetTextCtrl()->SetTextTok(P_DCP_Z_ACT_DEV_TOK);
	m_pZActDev->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_DEV_TOK));

	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZActDev->SetEmptyAllowed(true);

	// VIVA REMOVED
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

	/*
	// message....
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	// set help
	// VIVA REMOVED
	//SetHelpTok(H_DCP_3DROLLER_TOK,0);
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: OnTimer
// ================================================================================================
//void DCP::Fbs3DDialog::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}
// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::Fbs3DDialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
	m_pDataModel->pFileFunc->setFile(GetModel()->ADFFileName);
	m_pCommon = new Common(GetModel());
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	RefreshControls();
	DCP06_TRACE_EXIT;
}
// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::Fbs3DDialog::UpdateData()
{

}
// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::Fbs3DDialog::OnValueChanged(int unNotifyCode,  int ulParam2)
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
				snprintf(m_pDataModel->pid_ptr, sizeof(m_pDataModel->pid_ptr), DCP_POINT_ID_FMT, "");	
			else
			{
				sPid = m_pPointId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char point_id_buf[20];
				//UTL::UnicodeToAscii(point_id_buf, sPid);
				BSS::UTI::BSS_UTI_WCharToAscii(sPid, point_id_buf);
				common.strbtrim(point_id_buf);
				snprintf(m_pDataModel->pid_ptr, sizeof(m_pDataModel->pid_ptr), DCP_POINT_ID_FMT, point_id_buf);
				m_pDataModel->save_point();
				RefreshControls();
			}
		}
		else
		{
			m_pDataModel->set_xyz_mea_ptr();
			m_pDataModel->set_xyz_des_ptr();

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
// Description: refresh all controls
// ================================================================================================
void DCP::Fbs3DDialog::RefreshControls()
{
	if(m_pFile && m_pPointId && m_pXDsg && m_pYDsg&& m_pXAct &&
		m_pYAct && m_pZAct && m_pXDev &&
		m_pYDev && m_pZDev) 
	{

		// SHOW VALUES
		m_pCommon->empty_xyz_buffers(m_pDataModel->bXdif,m_pDataModel->bYdif,m_pDataModel->bZdif,9);
		sprintf(m_pDataModel->bFid,"%-8.8s","");
		snprintf(m_pDataModel->bPid, sizeof(m_pDataModel->bPid), DCP_POINT_ID_FMT, "");
		
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);

			if(m_pDataModel->pFileFunc->point_type == 0) 
				m_pDataModel->DSP_MODE = SINGLE;
			
			else if(m_pDataModel->pFileFunc->point_type == 1)
			{
				if(m_pDataModel->DSP_MODE != FRONTBACK)
				{
					if(m_pDataModel->pFileFunc->cpoint == 1) // FRONT
						m_pDataModel->DSP_MODE = FRONT;
					else
						m_pDataModel->DSP_MODE = BACK;
				}
			}
			
			// SHOW CORRECT TITLE
			if(m_pDataModel->DSP_MODE == FRONT)
			{
				// VIVA REMOVED
				//SetTitleStr(sFront);
				SetTitle(sFront);

			}
			else if(m_pDataModel->DSP_MODE == BACK)
			{
				// VIVA REMOVED
				//SetTitleStr(sBack);
				SetTitle(sBack);
			}
			else if(m_pDataModel->DSP_MODE == FRONTBACK)
			{

				// VIVA REMOVED
				// SetTitleStr(sFrontBack);
				SetTitle(sFrontBack);
			}
			else
			{
				// VIVA REMOVED
				// SetTitleStr(sSingle);
				SetTitle(sSingle);		
			}

		}
		else
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);

			// SHOW CORRECT TITLE
			if(m_pDataModel->DSP_MODE == FRONT)
			{
				// VIVA REMOVED
				//SetTitleStr(sFront);
				SetTitle(sFront);
			}
			else if(m_pDataModel->DSP_MODE == BACK)
			{
				// VIVA REMOVED
				//SetTitleStr(sBack);
				SetTitle(sBack);
			}
			else if(m_pDataModel->DSP_MODE == FRONTBACK)
			{
				// VIVA REMOVED
				// SetTitleStr(sFrontBack);
				SetTitle(sFrontBack);
			}
			else
			{
				// VIVA REMOVED
				// SetTitleStr(sSingle);
				SetTitle(sSingle);		
			}

		}
		m_pDataModel->set_xyz_des_ptr();
		m_pDataModel->set_xyz_mea_ptr();
		
		if(m_pDataModel->DSP_MODE != FRONTBACK)
		{
			// set correct promptext
			// VIVA REMOVED
			// m_pXDsg->GetTextCtrl()->SetTextTok(P_DCP_X_DSG_TOK);
			m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_DSG_TOK));
			
			// VIVA REMOVED
			//m_pXDsg->SetAutoColon(false);
			//m_pXDsg->SetColonPosition(9 * 11);

			// VIVA REMOVED
			//m_pYDsg->GetTextCtrl()->SetTextTok(P_DCP_Y_DSG_TOK);
			m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_DSG_TOK));
			
			// VIVA REMOVED
			//m_pYDsg->SetAutoColon(false);
			//m_pYDsg->SetColonPosition(9 * 11);

			// VIVA REMOVED
			//m_pZDsg->GetTextCtrl()->SetTextTok(P_DCP_Z_DSG_TOK);
			m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_DSG_TOK));
			
			// VIVA REMOVED
			// m_pZDsg->SetAutoColon(false);
			// m_pZDsg->SetColonPosition(9 * 11);
			
			// VIVA REMOVED
			//m_pXActDev->GetTextCtrl()->SetTextTok(P_DCP_X_ACT_DEV_TOK); 
			//m_pXActDev->SetText(1,StringC(AT_DCP06,P_DCP_X_ACT_DEV_TOK)); 
			m_pXAct->SetText(StringC(AT_DCP06,P_DCP_X_ACT_TOK));
			m_pXDev->SetText(StringC(AT_DCP06,P_DCP_X_DEV_TOK));
			
			// VIVA REMOVED
			// m_pXActDev->SetAutoColon(false);
			// m_pXActDev->SetColonPosition(9 * 11);

			// VIVA REMOVED
			//m_pYActDev->GetTextCtrl()->SetTextTok(P_DCP_Y_ACT_DEV_TOK);
			//m_pYActDev->SetText(1,StringC(AT_DCP06,P_DCP_Y_ACT_DEV_TOK));
			m_pYAct->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_TOK));
			m_pYDev->SetText(StringC(AT_DCP06,P_DCP_Y_DEV_TOK));


			// VIVA REMOVED
			// m_pYActDev->SetAutoColon(false);
			// m_pYActDev->SetColonPosition(9 * 11);

			// VIVA REMOVED
			//m_pZActDev->GetTextCtrl()->SetTextTok(P_DCP_Z_ACT_DEV_TOK); 
			//m_pZActDev->SetText(1,StringC(AT_DCP06,P_DCP_Z_ACT_DEV_TOK)); 
			m_pZAct->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_TOK)); 
			m_pZDev->SetText(StringC(AT_DCP06,P_DCP_Z_DEV_TOK));

			// VIVA REMOVED
			// m_pZActDev->SetAutoColon(false);
			// m_pZActDev->SetColonPosition(9 * 11);

			snprintf(m_pDataModel->bPid, sizeof(m_pDataModel->bPid), DCP_POINT_ID_FMT, m_pCommon->strbtrim(m_pDataModel->pid_ptr));

			m_pCommon->get_dist_len(m_pDataModel->xmea_ptr,m_pDataModel->xdes_ptr, m_pDataModel->bXdif,11);
			m_pCommon->get_dist_len(m_pDataModel->ymea_ptr,m_pDataModel->ydes_ptr, m_pDataModel->bYdif,11);
			m_pCommon->get_dist_len(m_pDataModel->zmea_ptr,m_pDataModel->zdes_ptr, m_pDataModel->bZdif,11);
			
			// Xdsg
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
			
			// Zdsg
			if(!m_pCommon->strblank(m_pDataModel->zdes_ptr))
			{
				m_pDataModel->Zdes = atof(m_pDataModel->zdes_ptr);
				m_pZDsg->GetFloatInputCtrl()->SetDouble(atof(m_pDataModel->zdes_ptr));
			}
			else
			{
				m_pZDsg->GetFloatInputCtrl()->SetEmpty();
			}
						
			//char temp[100];
			// Xdiff
			//sprintf(temp,"%9.9s %9.9s", m_pDataModel->bXdif ,m_pDataModel->xmea_ptr);
			//m_pXActDev->GetStringInputCtrl()->SetString(StringC(temp));
			
			//m_pXActDev->SetString(1,StringC(m_pDataModel->bXdif));
			//m_pXActDev->SetString(2,StringC(m_pDataModel->xmea_ptr));

			m_pXDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bXdif));
			m_pXAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->xmea_ptr));

			
			// Ydiff
			//sprintf(temp,"%9.9s %9.9s", m_pDataModel->bYdif,m_pDataModel->ymea_ptr);
			//m_pYActDev->GetStringInputCtrl()->SetString(StringC(temp));
			//m_pYActDev->SetString(1,StringC(m_pDataModel->bYdif));
			//m_pYActDev->SetString(2,StringC(m_pDataModel->ymea_ptr));
			m_pYDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bYdif));
			m_pYAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->ymea_ptr));



			// Zdiff
			//sprintf(temp,"%9.9s %9.9s", m_pDataModel->bZdif, m_pDataModel->zmea_ptr);
			//m_pZActDev->GetStringInputCtrl()->SetString(StringC(temp));
			
			//m_pZActDev->SetString(1,StringC(m_pDataModel->bZdif));
			//m_pZActDev->SetString(2,StringC(m_pDataModel->zmea_ptr));
			m_pZDev->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bZdif));
			m_pZAct->GetStringInputCtrl()->SetString(StringC(m_pDataModel->zmea_ptr));

		}
		// FRONTBACK
		else
		{
			char *x_ptr, *y_ptr, *z_ptr;
			if(m_pDataModel->FRONTBACK_ACTUAL == ACTUAL)
			{
				x_ptr = m_pDataModel->xmea_b_ptr;
				y_ptr = m_pDataModel->ymea_b_ptr;
				z_ptr = m_pDataModel->zmea_b_ptr;
			}
			else
			{
				x_ptr = m_pDataModel->xdes_b_ptr;
				y_ptr = m_pDataModel->ydes_b_ptr;
				z_ptr = m_pDataModel->zdes_b_ptr;
			}
	
						
			
			// ACTUAL
			if(m_pDataModel->FRONTBACK_ACTUAL == ACTUAL)
			{
				// VIVA REMOVED
				//m_pXActDev->GetTextCtrl()->SetTextTok(P_DCP_X_ACT_FRONT_BACK_TOK); 
				//m_pXActDev->SetText(1,StringC(AT_DCP06,P_DCP_X_ACT_FRONT_BACK_TOK)); 
				m_pXDev->SetText(StringC(AT_DCP06,P_DCP_X_ACT_FRONT_BACK_TOK)); 


				// VIVA REMOVED
				//m_pXActDev->SetAutoColon(false);
				// m_pXActDev->SetColonPosition(9 * 14);

				//m_pYActDev->GetTextCtrl()->SetTextTok(P_DCP_Y_ACT_FRONT_BACK_TOK); 
				//m_pYActDev->SetText(1,StringC(AT_DCP06,P_DCP_Y_ACT_FRONT_BACK_TOK)); 
				m_pYDev->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_FRONT_BACK_TOK)); 
				
				// VIVA REMOVED
				//m_pYActDev->SetAutoColon(false);
				//m_pYActDev->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pZActDev->GetTextCtrl()->SetTextTok(P_DCP_Z_ACT_FRONT_BACK_TOK); 
				//m_pZActDev->SetText(1,StringC(AT_DCP06,P_DCP_Z_ACT_FRONT_BACK_TOK)); 
				m_pZDev->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_FRONT_BACK_TOK)); 

				
				// VIVA REMOVED
				//m_pZActDev->SetAutoColon(false);
				//m_pZActDev->SetColonPosition(9 * 14);
		
				m_pCommon->get_dist_len(m_pDataModel->xmea_f_ptr,m_pDataModel->xmea_b_ptr, m_pDataModel->bXdif,11);
				m_pCommon->get_dist_len(m_pDataModel->ymea_f_ptr,m_pDataModel->ymea_b_ptr, m_pDataModel->bYdif,11);
				m_pCommon->get_dist_len(m_pDataModel->zmea_f_ptr,m_pDataModel->zmea_b_ptr, m_pDataModel->bZdif,11);


				// VIVA REMOVED
				//m_pXDsg->GetTextCtrl()->SetTextTok(P_DCP_X_ACT_BACK_TOK);
				m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_ACT_BACK_TOK));

				// VIVA REMOVED
				//m_pXDsg->SetAutoColon(false);
				//m_pXDsg->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pYDsg->GetTextCtrl()->SetTextTok(P_DCP_Y_ACT_BACK_TOK);
				m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_BACK_TOK));

				// VIVA REMOVED
				//m_pYDsg->SetAutoColon(false);
				//m_pYDsg->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pZDsg->GetTextCtrl()->SetTextTok(P_DCP_Z_ACT_BACK_TOK);
				m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_BACK_TOK));
				// VIVA REMOVED
				//m_pZDsg->SetAutoColon(false);
				//m_pZDsg->SetColonPosition(9 * 14);
			}
			// DESIGN
			else
			{
				// VIVA REMOVED
				//m_pXActDev->GetTextCtrl()->SetTextTok(P_DCP_X_DESIGN_FRONT_BACK_TOK); 
				//m_pXActDev->SetText(1,StringC(AT_DCP06,P_DCP_X_DESIGN_FRONT_BACK_TOK)); 
				m_pXDev->SetText(StringC(AT_DCP06,P_DCP_X_DESIGN_FRONT_BACK_TOK)); 

				// VIVA REMOVED
				//m_pXActDev->SetAutoColon(false);
				//m_pXActDev->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pYActDev->GetTextCtrl()->SetTextTok(P_DCP_Y_DESIGN_FRONT_BACK_TOK); 
				//m_pYActDev->SetText(1,StringC(AT_DCP06,P_DCP_Y_DESIGN_FRONT_BACK_TOK)); 
				m_pYDev->SetText(StringC(AT_DCP06,P_DCP_Y_DESIGN_FRONT_BACK_TOK)); 
				
				// VIVA REMOVED
				//m_pYActDev->SetAutoColon(false);
				//m_pYActDev->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pZActDev->GetTextCtrl()->SetTextTok(P_DCP_Z_DESIGN_FRONT_BACK_TOK); 
				//m_pZActDev->SetText(1,StringC(AT_DCP06,P_DCP_Z_DESIGN_FRONT_BACK_TOK)); 
				m_pZDev->SetText(StringC(AT_DCP06,P_DCP_Z_DESIGN_FRONT_BACK_TOK)); 
				
				// VIVA REMOVED
				//m_pZActDev->SetAutoColon(false);
				//m_pZActDev->SetColonPosition(9 * 14);

				m_pCommon->get_dist_len(m_pDataModel->xdes_f_ptr,m_pDataModel->xdes_b_ptr, m_pDataModel->bXdif,11);
				m_pCommon->get_dist_len(m_pDataModel->ydes_f_ptr,m_pDataModel->ydes_b_ptr, m_pDataModel->bYdif,11);
				m_pCommon->get_dist_len(m_pDataModel->zdes_f_ptr,m_pDataModel->zdes_b_ptr, m_pDataModel->bZdif,11);


				// VIVA REMOVED
				//m_pXDsg->GetTextCtrl()->SetTextTok(P_DCP_X_DESIGN_BACK_TOK);
				m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_DESIGN_BACK_TOK));
				
				// VIVA REMOVED
				//m_pXDsg->SetAutoColon(false);
				//m_pXDsg->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pYDsg->GetTextCtrl()->SetTextTok(P_DCP_Y_DESIGN_BACK_TOK);
				m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_DESIGN_BACK_TOK));
				
				// VIVA REMOVED
				//m_pYDsg->SetAutoColon(false);
				//m_pYDsg->SetColonPosition(9 * 14);

				// VIVA REMOVED
				//m_pZDsg->GetTextCtrl()->SetTextTok(P_DCP_Z_DESIGN_BACK_TOK);
				m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_DESIGN_BACK_TOK));
				
				// VIVA REMOVED
				//m_pZDsg->SetAutoColon(false);
				//m_pZDsg->SetColonPosition(9 * 14);
			}

								
			char diff_str[100];
			// Xdiff
			sprintf(diff_str,"%10.10s", m_pDataModel->bXdif);
			//m_pXActDev->GetStringInputCtrl()->SetString(StringC(diff_str));

			//m_pXActDev->SetString(1,StringC(L""));
			//m_pXActDev->SetString(2,StringC(diff_str));
			m_pXDev->GetStringInputCtrl()->SetString(StringC(L" "));
			m_pXAct->GetStringInputCtrl()->SetString(StringC(diff_str));


			// Ydiff
			sprintf(diff_str,"%10.10s", m_pDataModel->bYdif);
			//m_pYActDev->GetStringInputCtrl()->SetString(StringC(diff_str));
			//m_pYActDev->SetString(1,StringC(L""));
			//m_pYActDev->SetString(2,StringC(diff_str));

			m_pYDev->GetStringInputCtrl()->SetString(StringC(L" "));
			m_pYAct->GetStringInputCtrl()->SetString(StringC(diff_str));


			// Zdiff
			sprintf(diff_str,"%10.10s", m_pDataModel->bZdif);
			//m_pZActDev->GetStringInputCtrl()->SetString(StringC(diff_str));
			//m_pZActDev->SetString(1,StringC(L""));
			//m_pZActDev->SetString(2,StringC(diff_str));

			m_pZDev->GetStringInputCtrl()->SetString(StringC(L" "));
			m_pZAct->GetStringInputCtrl()->SetString(StringC(diff_str));


			// Xdsg
			if(!m_pCommon->strblank(x_ptr))
			{	
				m_pDataModel->Xdes = atof(x_ptr);
				m_pXDsg->GetFloatInputCtrl()->SetDouble(atof(x_ptr));
			}
			else
			{
				m_pXDsg->GetFloatInputCtrl()->SetEmpty();
			}

			// Y dsg
			if(!m_pCommon->strblank(y_ptr))
			{
				m_pDataModel->Ydes = atof(y_ptr);
				m_pYDsg->GetFloatInputCtrl()->SetDouble(atof(y_ptr));
			}
			else
			{
				m_pYDsg->GetFloatInputCtrl()->SetEmpty();
			} 
			
			// Zdsg
			if(!m_pCommon->strblank(z_ptr))
			{
				m_pDataModel->Zdes = atof(z_ptr);
				m_pZDsg->GetFloatInputCtrl()->SetDouble(atof(z_ptr));
			}
			else
			{
				m_pZDsg->GetFloatInputCtrl()->SetEmpty();
			}
			snprintf(m_pDataModel->bPid, sizeof(m_pDataModel->bPid), DCP_POINT_ID_FMT, m_pCommon->strbtrim(m_pDataModel->pid_ptr));
		}
		// Point id
		m_pPointId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->bPid));
		m_pFile->GetStringInputCtrl()->SetString(StringC(m_pDataModel->pFileFunc->getFileName()));
	}
}

// ================================================================================================
// Description: only accept DCP06 Model objects
// ================================================================================================
bool DCP::Fbs3DDialog::SetModel( GUI::ModelC* pModel )
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
// Description: only accept xxx Model objects
// ================================================================================================
DCP::Model* DCP::Fbs3DDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Fbs3DController::Fbs3DController(Model* pModel)
    : m_pDlg( nullptr ),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    
	// VIVA REMOVED
	//SetTitleTok( AT_DCP06, T_DCP_3DROLLER_TOK /*C_DCP_APPLICATION_NAME_TOK */);
	SetTitle( StringC(AT_DCP06, T_DCP_3DROLLER_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	m_pDataModel = new Fbs3DMeasModel(pModel);

    // Create a dialog
    m_pDlg = new DCP::Fbs3DDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DROLLER_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	// VIVA REMOVED
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	// VIVA REMOVED
	//vDef.nLable = K_DCP_ALL_TOK;
	vDef.strLable = StringC(AT_DCP06,K_DCP_ALL_TOK);

	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_AIM_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_DISPLAY_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_INIT_TOK);
	SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_SPECIAL_TOK);
	SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_TOOL_TOK);
	SetFunctionKey( SHFK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
	SetFunctionKey( SHFK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CHANGE_FACE_TOK);
	SetFunctionKey( SHFK6, vDef );

} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Fbs3DController::~Fbs3DController()
{
}
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::Fbs3DController::SetModel( GUI::ModelC* pModel )
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
// Description: OnControllerActivated
// ================================================================================================
void DCP::Fbs3DController::OnControllerActivated(void)
{
	show_function_keys();

}
// ================================================================================================
// Description: show_function_keys
// ================================================================================================
void DCP::Fbs3DController::show_function_keys()
{
	ResetFunctionKeys();

			// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP06;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );


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
			if(m_pDataModel->iPointKeys == 0 && m_pDataModel->DSP_MODE != FRONTBACK)
			{
				
			FKDef vDef;
			//vDef.nAppId = AT_DCP06;
			vDef.poOwner = this;
			vDef.strLable = StringC(AT_DCP06,K_DCP_ALL_TOK);
			SetFunctionKey( FK1, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_AIM_TOK);
			SetFunctionKey( FK2, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
			SetFunctionKey( FK3, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_DISPLAY_TOK);
			SetFunctionKey( FK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
			SetFunctionKey( FK5, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
			SetFunctionKey( FK6, vDef );
			
			// CAPTIVATE SHIFT
			//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
			//{
				vDef.strLable = StringC(AT_DCP06,K_DCP_CAMERA_TOK);
				SetFunctionKey( SHFK1, vDef );
			//}
		
			vDef.strLable = StringC(AT_DCP06,K_DCP_INIT_TOK);
			SetFunctionKey( SHFK2, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_SPECIAL_TOK);
			SetFunctionKey( SHFK3, vDef );
			
			vDef.strLable = StringC(AT_DCP06,K_DCP_TOOL_TOK);
			SetFunctionKey( SHFK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
			SetFunctionKey( SHFK5, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_CHANGE_FACE_TOK);
			SetFunctionKey( SHFK6, vDef );
		}
		else if(m_pDataModel->iPointKeys == 1 && m_pDataModel->DSP_MODE != FRONTBACK)
		{
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
			
			// SHIFT
			vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
			SetFunctionKey( SHFK2, vDef );

					// Hide quit
			FKDef vDef1;
			//vDef1.nAppId = AT_DCP06;
			vDef1.poOwner = this;
			vDef1.strLable = L" ";;
			SetFunctionKey( SHFK6, vDef1 );

		}
		// front - back
		else
		{
			FKDef vDef;
			//vDef.nAppId = AT_DCP06;
			vDef.poOwner = this;
			vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
			SetFunctionKey( FK1, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_ACTUAL_OR_DESIGN_TOK);
			SetFunctionKey( FK2, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
			SetFunctionKey( FK3, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_DISPLAY_TOK);
			SetFunctionKey( FK4, vDef );

			vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
			SetFunctionKey( FK5, vDef );
		}
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
			//m_bPointMenu = true;
}
// *********************************************************************
//
// *********************************************************************
void DCP::Fbs3DController::change_dsp()
{
//int f,b;
	
	if(m_pDataModel->pFileFunc->IsOpen())
	{
		if(m_pDataModel->pFileFunc->point_type == 1) // F/B
		{
			if(m_pDataModel->DSP_MODE == FRONT)
				m_pDataModel->DSP_MODE = FRONTBACK;
			
			else if(m_pDataModel->DSP_MODE == BACK)
			{
				m_pDataModel->DSP_MODE = FRONT;
				m_pDataModel->pFileFunc->cpoint = 1;
			}
			else if(m_pDataModel->DSP_MODE == FRONTBACK)
			{
				m_pDataModel->DSP_MODE = BACK;
				m_pDataModel->pFileFunc->cpoint = 2;
			}
		}
		else
			m_pDataModel->DSP_MODE = SINGLE;

	}
	else
	{
		if(m_pDataModel->DSP_MODE == SINGLE)
			m_pDataModel->DSP_MODE = BACK;

		else if(m_pDataModel->DSP_MODE == BACK)
			m_pDataModel->DSP_MODE = FRONT;

		else if(m_pDataModel->DSP_MODE == FRONT)
		{
			m_pDataModel->DSP_MODE = FRONTBACK;
			
			//show_front_back();
		}
		else if(m_pDataModel->DSP_MODE == FRONTBACK)
		{
			m_pDataModel->DSP_MODE = SINGLE;
		}
	}
}

// ================================================================================================
// Description: ConfirmClose
// ================================================================================================
bool DCP::Fbs3DController::ConfirmClose(bool bEsc)
{
	if(bEsc && m_pDataModel->iPointKeys == 1)
	{
		m_pDataModel->iPointKeys = 0;
		show_function_keys();
		m_pDlg->RefreshControls();
		return false;
	}
	else 
	{
		if(m_pDataModel->DSP_MODE == FRONTBACK)
		{
			change_dsp();
			show_function_keys();
			m_pDlg->RefreshControls();
			return false;
		}	
	}
	return true;
}
// ================================================================================================
// Description: F1 / ALL
// ================================================================================================
void DCP::Fbs3DController::OnF1Pressed()
{
	if(m_pDataModel->iPointKeys == 0 && m_pDataModel->DSP_MODE != FRONTBACK)
	{
		// MEASURE ALL
			// Previous point
		/*
		if(!m_pDataModel->pFileFunc->IsOpen())
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		*/
		if(m_bCamera)
		{
			m_bCamera = false;
			poVideoDlg->Close();
			SetActiveDialog(_3DROLLER_DLG);
			show_function_keys();
		}

		m_pDataModel->set_xyz_mea_ptr();

		if(m_pDlg->GetModel()->m_nOverWriteInfo && 
			!m_pDlg->GetModel()->m_nAutoMatch)
		{
			if(!m_pDataModel->pCommon->strblank(m_pDataModel->xmea_ptr) || 
				!m_pDataModel->pCommon->strblank(m_pDataModel->ymea_ptr) || 
				!m_pDataModel->pCommon->strblank(m_pDataModel->zmea_ptr))
			{
				sMsg.LoadTxt(AT_DCP06, M_DCP_OVERWRITE_POINT_TOK);
				sMsg.Format(sMsg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
				if(!m_pDataModel->pMsgBox->ShowMessageYesNo(sMsg))
				{
					m_pDlg->RefreshControls();
					return;
				}
				m_pDlg->RefreshControls();
			}
		}
		// ALL
		if(m_pDataModel->pCommon->check_edm_mode())
		{
			DisableFunctionKey(FK1);
			DisableFunctionKey(FK2);
			DisableFunctionKey(FK3);
			DisableFunctionKey(FK4);
			DisableFunctionKey(FK5);
			DisableFunctionKey(FK6);

			DCP::MeasXYZModel* pModel = new MeasXYZModel;
			pModel->tooli = 1;
			
			snprintf(pModel->sPointId, sizeof(pModel->sPointId), DCP_POINT_ID_FMT, m_pDataModel->pid_ptr);
			
			if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
			{
				(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::MeasXYZController(m_pDlg->GetModel()));
			}
			(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
			SetActiveController(MEAS_XYZ_CONTROLLER, true);
		}
	}
	else if((m_pDataModel->iPointKeys == 1 && m_pDataModel->DSP_MODE != FRONTBACK) || m_pDataModel->DSP_MODE == FRONTBACK)
	{
		// Previous point
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			m_pDataModel->pFileFunc->form_prev_pnt();
			
			if(m_pDataModel->pFileFunc->point_type == 1) // F/B
			{
				if(m_pDataModel->pFileFunc->cpoint == 1) // FRONT
					m_pDataModel->DSP_MODE = FRONT;
				else
					m_pDataModel->DSP_MODE = BACK;

				m_pDataModel->pFileFunc->cpoint = 2;
				m_pDataModel->DSP_MODE = BACK;
			}
			else
				m_pDataModel->DSP_MODE = SINGLE;
		}
		else
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}

		m_pDataModel->iPointKeys = 0;
		show_function_keys();	
	}
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F2 / DIST
// ================================================================================================
void DCP::Fbs3DController::OnF2Pressed()
{
	if (m_pDlg == nullptr)
    {
	    USER_APP_VERIFY( false );
		return;
	}
	// AIM
	if(m_pDataModel->iPointKeys == 0 && m_pDataModel->DSP_MODE != FRONTBACK)
	{
		m_pDataModel->set_xyz_des_ptr();
	
		if(!m_pDlg->GetModel()->m_nDesignValues)	
			return;

		if(m_pDataModel->pCommon->strblank(m_pDataModel->xdes_ptr) ||
			m_pDataModel->pCommon->strblank(m_pDataModel->ydes_ptr) ||
			m_pDataModel->pCommon->strblank(m_pDataModel->zdes_ptr))
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

	// Ask point id
	else if(m_pDataModel->iPointKeys == 1 && m_pDataModel->DSP_MODE != FRONTBACK)
	{
		char buffer[20]; buffer[0] = '\0';

		if(m_pDataModel->pFileFunc->IsOpen())
		{
			m_pDataModel->pFileFunc->get_next_point_id(buffer);

			//if(get_AUTO_MATCH() == FALSE)
			if(!m_pDlg->GetModel()->m_nAutoMatch)
			{
				DCP::InputTextModel* pModel = new InputTextModel;
				pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
				
				// VIVA REMOVED
				//pModel->m_StrTitle = GetTitleStr();
				pModel->m_StrTitle = GetTitle();
				
				pModel->m_iTextLength = 6;
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

				m_pDataModel->iPointKeys = 0;	
				show_function_keys();

			}
			else
			{
				sMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_ADD_POINT_TOK);
				m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
				//(void ) msgbox(TXT_NIL_TOKEN, M_CANNOT_ADDPNT_TOK,MB_OK);
			}
		}
		else
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		m_pDataModel->iPointKeys = 0;
		show_function_keys();	

		m_pDlg->RefreshControls();

	}
	else
	{
		if(m_pDataModel->FRONTBACK_ACTUAL == ACTUAL)
			m_pDataModel->FRONTBACK_ACTUAL = DESIGN;
		else
			m_pDataModel->FRONTBACK_ACTUAL = ACTUAL;

		m_pDataModel->iPointKeys = 0;
		show_function_keys();	

		m_pDlg->RefreshControls();
	}

}

// ================================================================================================
// Description: F3 / POINT
// ================================================================================================
void DCP::Fbs3DController::OnF3Pressed()
{
	if(m_pDataModel->iPointKeys == 0 && m_pDataModel->DSP_MODE != FRONTBACK)
	{
		// point
		m_pDataModel->iPointKeys = 1;
		show_function_keys();	
	}
	else if((m_pDataModel->iPointKeys == 1 && m_pDataModel->DSP_MODE != FRONTBACK) || m_pDataModel->DSP_MODE == FRONTBACK )
	{
		// next point
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			if(m_pDataModel->pFileFunc->form_next_pnt())
			{
				
				if(m_pDataModel->pFileFunc->point_type == 1) // F/B
				{
					if(m_pDataModel->pFileFunc->cpoint == 1) // FRONT
						m_pDataModel->DSP_MODE = FRONT;
					else
						m_pDataModel->DSP_MODE = BACK;

					m_pDataModel->pFileFunc->cpoint = 2;
					m_pDataModel->DSP_MODE = BACK;
				}
				else
					m_pDataModel->DSP_MODE = SINGLE;
			}
			else
				// VIVA REMOVED
				//GUI::DesktopC::Instance()->MessageShow(I_DCP_LAST_POINT_TOK, AT_DCP06);
				GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_LAST_POINT_TOK));
		}
		else
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
	
		m_pDataModel->iPointKeys = 0;
		show_function_keys();	
	}

	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F4 / AIM
// ================================================================================================
void DCP::Fbs3DController::OnF4Pressed()
{
	if(m_pDataModel->iPointKeys == 0)
	{
		if(m_pDataModel->DSP_MODE == SINGLE && m_pDataModel->pFileFunc->IsOpen())
		{
			// do not changed display
		}
		else
		{	
			if(m_pDataModel->DSP_MODE == FRONTBACK)
				m_pDataModel->FRONTBACK_ACTUAL = ACTUAL;

			m_pDataModel->set_xyz_des_ptr();
			m_pDataModel->set_xyz_mea_ptr();
			change_dsp();
			m_pDlg->RefreshControls();
		}
	}
	else
	{
		// swap
		//if(read_allow_edit())
		{
			char temp_x[15];

			m_pDataModel->set_xyz_mea_ptr();
			m_pDataModel->set_xyz_des_ptr();

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
		}
		/*
		else
		{
			//msgbox(TXT_NIL_TOKEN, M_EDIT_NOT_ALLOWED_TOK,MB_OK);
		}	
		*/
		m_pDataModel->iPointKeys = 0;
		

	}
	show_function_keys();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F5 / PID
// ================================================================================================
void DCP::Fbs3DController::OnF5Pressed()
{
	// point id
	if(m_pDataModel->iPointKeys == 0)
	{	
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			DCP::SelectPointModel* pModel = new SelectPointModel;
			int iCount = m_pDataModel->pFileFunc->GetPointList(&pModel->points[0],MAX_SELECT_POINTS);
			pModel->m_iCounts = iCount;
			pModel->m_iSelectedId = m_pDataModel->pFileFunc->active_point_front;
			
			if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
			{
				(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
			}

			//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

			(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
			SetActiveController(SELECT_POINT_CONTROLLER, true);
		 }
		else
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_3DFILE_ISNOT_OPEN_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}

		m_pDataModel->iPointKeys = 0;	
		show_function_keys();

	}
	// 3d file
	else
	{
		// check if want update header...
		if(m_pDataModel->pFileFunc->IsOpen() && m_pDataModel->file_updated == 1)
		{
			m_pDataModel->pFileFunc->add_header_to_adf();
			m_pDataModel->file_updated = 0;
		}

		// call file dialog
		if(GetController(FILE_CONTROLLER) == nullptr)
		{
			(void)AddController( FILE_CONTROLLER, new DCP::FileController(m_pDlg->GetModel()) );
		}

		(void)GetController( FILE_CONTROLLER )->SetModel(m_pDlg->GetModel());
		SetActiveController(FILE_CONTROLLER, true);

		m_pDataModel->iPointKeys = 0;	
		show_function_keys();
		
	}
}

// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::Fbs3DController::OnF6Pressed()
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
		SetActiveDialog(_3DROLLER_DLG);
		show_function_keys();
	}
}
// ================================================================================================
// Description: SHF1
// ================================================================================================
void DCP::Fbs3DController::OnSHF1Pressed()
{
	/*
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
void DCP::Fbs3DController::OnSHF2Pressed()
{
	if(m_pDataModel->iPointKeys != 0)
	{
		// clear
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			if(m_pDataModel->DSP_MODE == SINGLE)
			{
				m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->pFileFunc->xmea_front,
									m_pDataModel->pFileFunc->ymea_front,
									m_pDataModel->pFileFunc->zmea_front,9);
				sprintf(m_pDataModel->pFileFunc->note_front,"%-6.6s", " ");
			}
			else if(m_pDataModel->DSP_MODE == FRONT)
			{
				m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->pFileFunc->xmea_front,
									m_pDataModel->pFileFunc->ymea_front,
									m_pDataModel->pFileFunc->zmea_front,9);
				sprintf(m_pDataModel->pFileFunc->note_front,"%-6.6s", " ");	
			}

			else if(m_pDataModel->DSP_MODE == BACK)
			{
				m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->pFileFunc->xmea_back,
									m_pDataModel->pFileFunc->ymea_back,
									m_pDataModel->pFileFunc->zmea_back,9);
				sprintf(m_pDataModel->pFileFunc->note_back,"%-6.6s", " ");	
			}
			else if(m_pDataModel->DSP_MODE == FRONTBACK)
			{
				m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->pFileFunc->xmea_front,
							m_pDataModel->pFileFunc->ymea_front,
							m_pDataModel->pFileFunc->zmea_front,9);
				sprintf(m_pDataModel->pFileFunc->note_front,"%-6.6s", " ");	
			}
			m_pDataModel->save_point();
			
		}
		else
		{
			// CLEAR
				if(m_pDataModel->DSP_MODE == SINGLE)
				{
					m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->bXmea, m_pDataModel->bYmea,m_pDataModel->bZmea,9);
					sprintf(m_pDataModel->bNote,"%-6.6s"," ");
				}

				else if(m_pDataModel->DSP_MODE == FRONT)
				{
					m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->bXmea_f, m_pDataModel->bYmea_f,m_pDataModel->bZmea_f,9);
					sprintf(m_pDataModel->bNote,"%-6.6s"," ");
				}

				else if(m_pDataModel->DSP_MODE == BACK)
				{
					m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->bXmea_b, m_pDataModel->bYmea_b,m_pDataModel->bZmea_b,9);
					sprintf(m_pDataModel->bNote,"%-6.6s"," ");
				}

				else if(m_pDataModel->DSP_MODE == FRONTBACK)
				{
					m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->bXmea_b, m_pDataModel->bYmea_b,m_pDataModel->bZmea_b,9);
					m_pDataModel->pCommon->empty_xyz_buffers(m_pDataModel->bXmea_f, m_pDataModel->bYmea_f,m_pDataModel->bZmea_f,9);
					sprintf(m_pDataModel->bNote,"%-6.6s"," ");
				}		
		}

	}
	else
	{
		if(GetController(INIT_CONTROLLER) == nullptr)
		{
			(void)AddController( INIT_CONTROLLER, new DCP::InitializationController );
		}
		(void)GetController( INIT_CONTROLLER )->SetModel( m_pDlg->GetModel());
		SetActiveController(INIT_CONTROLLER, true);
	}

	m_pDlg->RefreshControls();
	m_pDataModel->iPointKeys = 0;	
	show_function_keys();

}

// ================================================================================================
// Description: SHF3 / SPECIAL MENU
// ================================================================================================
void DCP::Fbs3DController::OnSHF3Pressed()
{
	if(GetController(SPECIAL_MENU_CONTROLLER) == nullptr)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::SpecialMenuController( m_pDlg->GetModel()));
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);

}

// ================================================================================================
// Description: SHF4
// ================================================================================================
void DCP::Fbs3DController::OnSHF4Pressed()
{
	if(GetController(TOOL_CONTROLLER) == nullptr)
	{
		(void)AddController( TOOL_CONTROLLER, new DCP::ToolController());
	}
	(void)GetController( TOOL_CONTROLLER )->SetModel( m_pDlg->GetModel());
	SetActiveController(TOOL_CONTROLLER, true);

}

// ================================================================================================
// Description: SHF5 / CALC
// ================================================================================================
void DCP::Fbs3DController::OnSHF5Pressed()
{
	CalculationDist2PointsModel* pModel = new CalculationDist2PointsModel;
	int iCount = m_pDataModel->pFileFunc->GetPointList(&pModel->points[0],MAX_SELECT_POINTS, BOTH);
	pModel->sFile = StringC(m_pDataModel->pFileFunc->getFileName());
	pModel->m_iCounts = iCount;
	pModel->pAdfFile = m_pDataModel->pFileFunc;

	// title for calc2points display
	StringC sTemp;
	sTemp.LoadTxt(AT_DCP06, T_CALC_DISTANCE_2_POINTS_TOK);

	// VIVA REMOVED
	//pModel->sTitle = GetTitleStr();
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
// Description: SHF6 / CHANGE FACE
// ================================================================================================
void DCP::Fbs3DController::OnSHF6Pressed()
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
void DCP::Fbs3DController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(_3DROLLER_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::Fbs3DController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{	
	// ************************
	// File
	// ************************
	if(lCtrlID == FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->pFileFunc->always_single = 0;
		m_pDataModel->pFileFunc->setFile(m_pDlg->GetModel()->ADFFileName);
		m_pDataModel->pFileFunc->form_pnt((int) m_pDataModel->pFileFunc->active_point_front);
	}
	// ************************
	// Add point
	// ************************
	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char buffer[10]; buffer[0] = '\0';
			m_pDataModel->pCommon->convert_to_ascii(strNewFile, buffer, DCP_POINT_ID_LENGTH + 1);

			m_pDataModel->pFileFunc->add_point(buffer);

			if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
			{
				if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
					m_pDataModel->DSP_MODE = FRONT;
				else
					m_pDataModel->DSP_MODE = BACK;
			}
			else
			{
				m_pDataModel->DSP_MODE = SINGLE;
			}
	}
	// ************************
	// select point
	// ************************
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectPointModel* pModel = (DCP::SelectPointModel*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		StringC strSelectedPoint = pModel->m_strSelectedPoint;
		short iSelectedPointId = pModel->m_iSelectedId;
		m_pDataModel->pFileFunc->form_pnt(pModel->m_iSelectedId);
		
		if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
		{
			if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
				m_pDataModel->DSP_MODE = FRONT;
			else
				m_pDataModel->DSP_MODE = BACK;
		}
		else
		{
			m_pDataModel->DSP_MODE = SINGLE;
		}
	}

	// ************************
	// MEASURE XYZ	
	// ************************
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
			int pno = m_pDataModel->pFileFunc->find_point(m_pDataModel->x_temp, m_pDataModel->y_temp, m_pDataModel->z_temp);

			if( pno > 0)
			{
					m_pDataModel->pFileFunc->form_pnt((int) pno);
					
					// added 280110
					if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
						{
							if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
								m_pDataModel->DSP_MODE = FRONT;
							else
								m_pDataModel->DSP_MODE = BACK;
						}
						else
						{
							m_pDataModel->DSP_MODE = SINGLE;
						}

					// added 280110
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

					// added 280110
					if(GetController(AUTOMATCH_CONTROLLER) == nullptr)
					{
						(void)AddController( AUTOMATCH_CONTROLLER, new DCP::AutoMatchController(m_pDlg->GetModel()));
					}
					(void)GetController( AUTOMATCH_CONTROLLER )->SetModel(pModel);
					SetActiveController(AUTOMATCH_CONTROLLER, true);
				/*
				m_pDataModel->pFileFunc->form_pnt(pno);
				m_pDataModel->set_xyz_mea_ptr();
					
				if(m_pDlg->GetModel()->m_nOverWriteInfo)
				{
					if( !m_pDataModel->pCommon->strblank(m_pDataModel->xmea_ptr) || 
						!m_pDataModel->pCommon->strblank(m_pDataModel->ymea_ptr) || 
						!m_pDataModel->pCommon->strblank(m_pDataModel->zmea_ptr))
					{
						sMsg.LoadTxt(AT_DCP06,M_DCP_OVERWRITE_POINT_TOK);
						sMsg.Format(sMsg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
						if(!m_pDataModel->pMsgBox->ShowMessageYesNo(sMsg))
						{
							m_pDlg->RefreshControls();
							return;
						}
						m_pDlg->RefreshControls();
					}
				}
							
				m_pDataModel->pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
				m_pDataModel->save_point();
				*/
			}	
				//MMI_ChangeDlgCaption(DialogId, PROG_CAPTION_TOK, T_3DMEAS_TOK);
		}
		else
		{
			m_pDataModel->pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
			m_pDataModel->save_point();
		}

		if(m_pDataModel->DSP_MODE == FRONT)
		{
			short f,b;
			if(m_pDataModel->pFileFunc->IsOpen())
			{
				b = !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->xmea_back) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->ymea_back) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->zmea_back);
				f = !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->xmea_front) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->ymea_front) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->zmea_front);
			}
			else
			{
				b = !m_pDataModel->pCommon->strblank(m_pDataModel->bXmea_b) && !m_pDataModel->pCommon->strblank(m_pDataModel->bYmea_b) && !m_pDataModel->pCommon->strblank(m_pDataModel->bZmea_b);
				f = !m_pDataModel->pCommon->strblank(m_pDataModel->bXmea_f) && !m_pDataModel->pCommon->strblank(m_pDataModel->bYmea_f) && !m_pDataModel->pCommon->strblank(m_pDataModel->bZmea_f);
			}

			if(b && f)
			{
				m_pDataModel->DSP_MODE = FRONTBACK;
				show_function_keys();
			}
		}


		// Auto Increment codes
		/*
		if(m_pDataModel->pFileFunc->IsOpen())
		{
			if(m_pDlg->GetModel()->m_nAutoIncrement)
			{
				short ret = m_pDataModel->pFileFunc->form_next_pnt();
				if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
				{
					if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
						m_pDataModel->DSP_MODE = FRONT;
					else
						m_pDataModel->DSP_MODE = BACK;

					m_pDataModel->pFileFunc->cpoint = 2;
					m_pDataModel->DSP_MODE = BACK;
				}
				else
				{
					m_pDataModel->DSP_MODE = SINGLE;
				}

				if(ret == 0) // Ei seuraavaa pistett�, lis�t��n
				{
					char buff[20]; buff[0] = '\0';
					m_pDataModel->pFileFunc->add_point(buff);								
					if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
					{
						if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
							m_pDataModel->DSP_MODE = FRONT;
						else
							m_pDataModel->DSP_MODE = BACK;
					}
					else
					{
						m_pDataModel->DSP_MODE = SINGLE;
					}
				}
			}
		}*/
		}
	}
	if(lCtrlID == AUTOMATCH_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::AutoMatchModel* pModel = (DCP::AutoMatchModel*) GetController( AUTOMATCH_CONTROLLER )->GetModel();		
			short pno = pModel->pno;
			
			m_pDataModel->pFileFunc->form_pnt(pno);
			//m_pDataModel->DSP_MODE = SINGLE;
			m_pDataModel->set_xyz_mea_ptr();
					
			if(m_pDlg->GetModel()->m_nOverWriteInfo)
					//if(get_show_overwrite_info() == TRUE)
			{
				if( !m_pDataModel->pCommon->strblank(m_pDataModel->xmea_ptr) || 
					!m_pDataModel->pCommon->strblank(m_pDataModel->ymea_ptr) || 
					!m_pDataModel->pCommon->strblank(m_pDataModel->zmea_ptr))
					{
						sMsg.LoadTxt(AT_DCP06,M_DCP_OVERWRITE_POINT_TOK);
						sMsg.Format(sMsg,(const wchar_t*)StringC(m_pDataModel->pid_ptr));
						if(!m_pDataModel->pMsgBox->ShowMessageYesNo(sMsg))
							//if(msgbox1(TXT_NIL_TOKEN, M_OVERWRITE_POINT_TOK,(void *) pid_ptr, MB_YESNO) == FALSE)
						{
			
						}
						else
						{
							m_pDataModel->pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
							m_pDataModel->save_point();
						}
					}
					else
					{
							m_pDataModel->pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
							m_pDataModel->save_point();
					}

			}
			else
			{
				m_pDataModel->pCommon->copy_xyz_to_buffer(&pModel->xmea, &pModel->ymea, &pModel->zmea, m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
				m_pDataModel->save_point();
			}
			if(m_pDataModel->DSP_MODE == FRONT)
			{
				short f,b;
				if(m_pDataModel->pFileFunc->IsOpen())
				{
					b = !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->xmea_back) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->ymea_back) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->zmea_back);
					f = !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->xmea_front) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->ymea_front) && !m_pDataModel->pCommon->strblank(m_pDataModel->pFileFunc->zmea_front);
				}
				else
				{
					b = !m_pDataModel->pCommon->strblank(m_pDataModel->bXmea_b) && !m_pDataModel->pCommon->strblank(m_pDataModel->bYmea_b) && !m_pDataModel->pCommon->strblank(m_pDataModel->bZmea_b);
					f = !m_pDataModel->pCommon->strblank(m_pDataModel->bXmea_f) && !m_pDataModel->pCommon->strblank(m_pDataModel->bYmea_f) && !m_pDataModel->pCommon->strblank(m_pDataModel->bZmea_f);
				}

				if(b && f)
				{
					m_pDataModel->DSP_MODE = FRONTBACK;
				}
			}


			// Auto Increment codes
			if(m_pDataModel->pFileFunc->IsOpen())
			{
				if(m_pDlg->GetModel()->m_nAutoIncrement)
				{
					short ret = m_pDataModel->pFileFunc->form_next_pnt();
					if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
					{
						if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
							m_pDataModel->DSP_MODE = FRONT;
						else
							m_pDataModel->DSP_MODE = BACK;

						m_pDataModel->pFileFunc->cpoint = 2;
						m_pDataModel->DSP_MODE = BACK;
					}
					else
					{
						m_pDataModel->DSP_MODE = SINGLE;
					}

					if(ret == 0) // Ei seuraavaa pistett�, lis�t��n
					{
						char buff[20]; buff[0] = '\0';
						m_pDataModel->pFileFunc->add_point(buff);								
						if(m_pDataModel->pFileFunc->point_type == 1)     //FRONTBACK
						{
							if(m_pDataModel->pFileFunc->cpoint == 1)      //FRONT
								m_pDataModel->DSP_MODE = FRONT;
							else
								m_pDataModel->DSP_MODE = BACK;
						}
						else
						{
							m_pDataModel->DSP_MODE = SINGLE;
						}
					}
				}

			}
	}

	// ************************
	// SPECIAL MENU
	// ************************
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
	// ************************
	// CIRCLE
	// ************************

	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( CIRCLE_CONTROLLER )->GetModel();		
		m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		m_pDataModel->y_temp = pModel->m_pPointBuff[0].y;
		m_pDataModel->z_temp= pModel->m_pPointBuff[0].z;
			
		m_pDataModel->pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
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
		
		m_pDataModel->pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
	
		m_pDataModel->save_point();
	}
	// ************************
	// SEPARATE
	// ************************
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

	// ************************
	//hiddenpoint HIDDENPOINT_CONTROLLER
	// ************************
	if(lCtrlID == HIDDENPOINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( HIDDENPOINT_CONTROLLER )->GetModel();		
		m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		m_pDataModel->y_temp = pModel->m_pPointBuff[0].y;
		m_pDataModel->z_temp= pModel->m_pPointBuff[0].z;
			
		m_pDataModel->pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		m_pDataModel->save_point();
	}

	// ************************
	// XorYorZ XYZ_CONTROLLER
	// ************************
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
	
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: ShowHiddenPointDlg
// ================================================================================================
void DCP::Fbs3DController::ShowHiddenPointDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;
	snprintf(pModel->m_pPointBuff[0].point_id, sizeof(pModel->m_pPointBuff[0].point_id), DCP_POINT_ID_FMT, m_pDataModel->pid_ptr);

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
void DCP::Fbs3DController::ShowXorYorZDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;
	snprintf(pModel->m_pPointBuff[0].point_id, sizeof(pModel->m_pPointBuff[0].point_id), DCP_POINT_ID_FMT, m_pDataModel->pid_ptr);
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
void DCP::Fbs3DController::ShowCircleDlg()
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
void DCP::Fbs3DController::ShowSeparateRecDlg()
{
	DCP::PointBuffModel* pModel = new PointBuffModel;

	snprintf(pModel->m_pPointBuff[0].point_id, sizeof(pModel->m_pPointBuff[0].point_id), DCP_POINT_ID_FMT, m_pDataModel->pid_ptr);

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
void DCP::Fbs3DController::ShowHomePointsDlg()
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
void DCP::Fbs3DController::ShowMidPointDlg()
{
		DCP::PointBuffModel* pModel = new PointBuffModel;

		snprintf(pModel->m_pPointBuff[0].point_id, sizeof(pModel->m_pPointBuff[0].point_id), DCP_POINT_ID_FMT, m_pDataModel->pid_ptr);

		//pModel->m_pPointBuff[0].xsta = pMeasModel->
		if(GetController(MID_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( MID_POINT_CONTROLLER, new DCP::MidPointController(m_pDlg->GetModel()));
		}
		(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
		SetActiveController(MID_POINT_CONTROLLER, true);
}
// ================================================================================================
// ====================================  Meas3DModel           ==============================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Fbs3DMeasModel::Fbs3DMeasModel(DCP::Model* pModel)//:m_pModel(pModel))
{
	pFileFunc = new AdfFileFunc(pModel);
	pCommon = new Common(pModel);
	pMsgBox = new MsgBox;
	pFileFunc->always_single = 0;
	DSP_MODE = SINGLE;
	FRONTBACK_ACTUAL = ACTUAL;
	iPointKeys = 0;
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
	
	bXmea_f[ 0 ] = '\0'; 
	bYmea_f[ 0 ] = '\0'; 
	bZmea_f[ 0 ] = '\0'; 
	bXdes_f[ 0 ] = '\0'; 
	bYdes_f[ 0 ] = '\0'; 
	bZdes_f[ 0 ] = '\0'; 
	bXmea_b[ 0 ] = '\0'; 
	bYmea_b[ 0 ] = '\0'; 
	bZmea_b[ 0 ] = '\0'; 
	bXdes_b[ 0 ] = '\0'; 
	bYdes_b[ 0 ] = '\0'; 
	bZdes_b[ 0 ] = '\0'; 


}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Fbs3DMeasModel::~Fbs3DMeasModel()
{
	if(pFileFunc)
	{
		if(pFileFunc->IsOpen() && file_updated == 1)
			pFileFunc->add_header_to_adf();

		delete pFileFunc;
		pFileFunc = 0;
	}
	if(pMsgBox)
	{
		delete pMsgBox;
		pMsgBox = 0;
	}
	if(pCommon)
	{
		delete pCommon;
		pCommon = 0;
	}
}

// *********************************************************************
//
// *********************************************************************
void DCP::Fbs3DMeasModel::set_xyz_mea_ptr()
{
	if(pFileFunc->IsOpen())
	{
		switch(DSP_MODE)
		{
			case FRONT:
			case SINGLE:

						xmea_ptr = &pFileFunc->xmea_front[0];
						ymea_ptr = &pFileFunc->ymea_front[0];
						zmea_ptr = &pFileFunc->zmea_front[0];
						pid_ptr  = &pFileFunc->pointid_front[0];
						note_ptr = &pFileFunc->note_front[0];

						//sprintf(msgstr,"%-s\n\r x:%-s\n\r y:%-s\n\r z:%-s","ptr->front file",
						//				FormFile_.xmea_front,
						//				FormFile_.ymea_front,
						//				FormFile_.zmea_front	);
						//msgbox("",msgstr,MB_OK);

						break;
			//case FRONTBACK:
			case BACK:
						//sprintf(msgstr,"%-s","ptr->back file");
						//msgbox("",msgstr,MB_OK);

						xmea_ptr = pFileFunc->xmea_back;
						ymea_ptr = pFileFunc->ymea_back;
						zmea_ptr = pFileFunc->zmea_back;
						pid_ptr  = pFileFunc->pointid_back;
						note_ptr = pFileFunc->note_back;
						break;

			case FRONTBACK:
						xmea_b_ptr = pFileFunc->xmea_back;
						ymea_b_ptr = pFileFunc->ymea_back;
						zmea_b_ptr = pFileFunc->zmea_back;
						xmea_f_ptr = pFileFunc->xmea_front;
						ymea_f_ptr = pFileFunc->ymea_front;
						zmea_f_ptr = pFileFunc->zmea_front;
						pid_ptr  = pFileFunc->pointid_back;
						note_ptr = pFileFunc->note_back;
						break;
			default:
						//sprintf(msgstr,"%-s","ptr->xmea");
						//msgbox("",msgstr,MB_OK);
						pid_ptr  = pCommon->strbtrim( bPid);
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
						//sprintf(msgstr,"%-s","ptr->single");
						//msgbox("",msgstr,MB_OK);
						pid_ptr  = pCommon->strbtrim(bPid);
						xmea_ptr = bXmea;
						ymea_ptr = bYmea;
						zmea_ptr = bZmea;
						note_ptr = bNote;	
						break;

			case FRONT:
						//sprintf(msgstr,"%-s","ptr->front");
						//msgbox("",msgstr,MB_OK);
						pid_ptr  = pCommon->strbtrim(bPid);
						xmea_ptr = bXmea_f;
						ymea_ptr = bYmea_f;
						zmea_ptr = bZmea_f;
						note_ptr = bNote;
						break;

			//case FRONTBACK:
			case BACK:
						//	sprintf(msgstr,"%-s","ptr->back");
					   //	msgbox("",msgstr,MB_OK);
						pid_ptr  = pCommon->strbtrim(bPid);
						xmea_ptr = bXmea_b;
						ymea_ptr = bYmea_b;
						zmea_ptr = bZmea_b;
						note_ptr = bNote;	
						break;

			case FRONTBACK:
						xmea_b_ptr = bXmea_b;
						ymea_b_ptr = bYmea_b;
						zmea_b_ptr = bZmea_b;
						xmea_f_ptr = bXmea_f;
						ymea_f_ptr = bYmea_f;
						zmea_f_ptr = bZmea_f;
						pid_ptr  = pCommon->strbtrim(bPid);
						note_ptr = bNote;

			default:
					  //	sprintf(msgstr,"%-s","ptr->xmea");
					  //	msgbox("",msgstr,MB_OK);
						pid_ptr  = pCommon->strbtrim(bPid);
						xmea_ptr = bXmea;
						ymea_ptr = bYmea;
						zmea_ptr = bZmea;
						note_ptr = bNote;
						break;
		}
	}
}

// *********************************************************************
//
// *********************************************************************
void DCP::Fbs3DMeasModel::set_xyz_des_ptr()
{
	if(pFileFunc->IsOpen())
	{
		switch(DSP_MODE)
		{
			case FRONT:
			case SINGLE:
						xdes_ptr = pFileFunc->xdes_front;
						ydes_ptr = pFileFunc->ydes_front;
						zdes_ptr = pFileFunc->zdes_front;
						break;
			//case FRONTBACK:
			case BACK:
						xdes_ptr = pFileFunc->xdes_back;
						ydes_ptr = pFileFunc->ydes_back;
						zdes_ptr = pFileFunc->zdes_back;
						break;

			case FRONTBACK:
						xdes_b_ptr = pFileFunc->xdes_back;
						ydes_b_ptr = pFileFunc->ydes_back;
						zdes_b_ptr = pFileFunc->zdes_back;
						xdes_f_ptr = pFileFunc->xdes_front;
						ydes_f_ptr = pFileFunc->ydes_front;
						zdes_f_ptr = pFileFunc->zdes_front;
						break;
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
			case FRONT:
						xdes_ptr = bXdes_f;
						ydes_ptr = bYdes_f;
						zdes_ptr = bZdes_f;
						break;

			//case FRONTBACK:
			case BACK:
						xdes_ptr = bXdes_b;
						ydes_ptr = bYdes_b;
						zdes_ptr = bZdes_b;
						break;

			case FRONTBACK:
						xdes_b_ptr = bXdes_b;
						ydes_b_ptr = bYdes_b;
						zdes_b_ptr = bZdes_b;
						xdes_f_ptr = bXdes_f;
						ydes_f_ptr = bYdes_f;
						zdes_f_ptr = bZdes_f;
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
void DCP::Fbs3DMeasModel::save_point()
{
	if(pFileFunc->IsOpen())
	{
		short iTemp = pFileFunc->cpoint;
		pFileFunc->form_save_pnt();
		file_updated = 1;
		pFileFunc->cpoint = iTemp;

	}
}
