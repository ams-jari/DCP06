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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/application/LineFitMeas.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/measurement/SeparateRec.hpp>
#include <dcp06/measurement/HomePoints.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/ChangeFace.hpp>
#include <dcp06/core/SelectPoint.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/MeasDist.hpp>
#include <dcp06/measurement/MidPointController.hpp>
#include "calc.h"

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::LineFitMeasDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::LineFitMeasDialog::LineFitMeasDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointId(0),m_pHeight(0),m_pDistFromLine(0),m_pDistAlongLine(0),m_pLineOffset(0)/*,m_pInfo(0)*/,m_pModel(pModel),
	m_pPointIdObserver(OBS_METHOD_TO_PARAM0(LineFitMeasDialog, OnPointIdChanged), this),
	iInfoInd(0),m_pCommon(0)
{
	//SetTxtApplicationId(AT_DCP06);
	

	m_pCommon = new Common(pModel);
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);

	//memset(temp_point_table,0, sizeof(S_POINT_BUFF)*20);
	//memset(temp_point_table2,0, sizeof(S_POINT_BUFF)*20);
}


            // Description: Destructor
DCP::LineFitMeasDialog::~LineFitMeasDialog()
{
	 //m_pTimer.KillTimer();
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::LineFitMeasDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	// Add fields to dialog
	m_pPointNo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointNo->SetId(ePointNo);
	m_pPointNo->SetText(StringC(AT_DCP06,P_DCP_POINT_NO_TOK));
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pPointNo->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pPointNo);

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	// m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pPointId->SetEmptyAllowed(true);
	AddCtrl(m_pPointId);

	// InsertEmptyLine(); CAPTIVATE

	m_pHeight = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pHeight->SetId(eHeight);
	m_pHeight->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_HEIGHT_TOK));
	//m_pHeight->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pHeight->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pHeight->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//
	// m_pX->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pHeight->SetEmptyAllowed(true);
	AddCtrl(m_pHeight);

	

	m_pDistAlongLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistAlongLine->SetId(eDistAlongLine);
	m_pDistAlongLine->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_DIST_ALONG_LINE_TOK));
	//m_pDistAlongLine->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pDistAlongLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDistAlongLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pDistAlongLine->SetEmptyAllowed(true);
	AddCtrl(m_pDistAlongLine);
	
	m_pLineOffset = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLineOffset->SetId(eLineOffset);
	m_pLineOffset->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_LINE_OFFSET_TOK));
	//m_pDistAlongLine->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pLineOffset->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pLineOffset->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pLineOffset->SetEmptyAllowed(true);
	AddCtrl(m_pLineOffset);

	m_pDistFromLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistFromLine->SetId(eDistFromLine);
	m_pDistFromLine->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_DIST_FROM_LINE_TOK));
	//m_pDistFromLine->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pDistFromLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDistFromLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pY->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pDistFromLine->SetEmptyAllowed(true);
	AddCtrl(m_pDistFromLine);
	// messageline...
	// InsertEmptyLine(); CAPTIVATE
	// InsertEmptyLine(); CAPTIVATE

	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	m_pPointIdObserver.Attach(m_pPointId->GetSubject());
	

	//SetHelpTok(H_DCP_MEAS_TOK,0);
}

void DCP::LineFitMeasDialog::OnDialogActivated()
{
	// get last defined point
	//Common common(m_pModel);
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );

	GetDataModel()->m_iPointsCount = m_pCommon->get_last_defined_point(&GetDataModel()->point_table[0],GetDataModel()->m_iMaxPoint);

	if(GetDataModel()->m_iPointsCount  < GetDataModel()->m_iMinPoint)
		GetDataModel()->m_iPointsCount = GetDataModel()->m_iMinPoint;

	RefreshControls();
}

//void DCP::LineFitMeasDialog::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

void DCP::LineFitMeasDialog::UpdateData()
{
}

// Description: refresh all controls
void DCP::LineFitMeasDialog::RefreshControls()
{
	if(m_pPointNo && m_pPointId && m_pHeight && m_pDistFromLine && m_pDistAlongLine && m_pLineOffset)
	{
		if(GetDataModel()->selectedRefLine == REF_LINE_VERTICAL)
		{
			m_pHeight->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_DEPTH_TOK));
		}

		StringC sTemp;
		char point_id[POINT_ID_BUFF_LEN];
		sTemp.Format(L"%d/%d",GetDataModel()->m_iCurrentPoint, GetDataModel()->m_iPointsCount);
		m_pPointNo->GetStringInputCtrl()->SetString(sTemp);
		
		// Default Point ID on first display when blank
		S_POINT_BUFF& pt = GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint - 1];
		m_pCommon->strbtrim(pt.point_id);
		if (pt.point_id[0] == '\0')
		{
			char suggested[POINT_ID_BUFF_LEN];
			m_pCommon->get_suggested_next_point_id(suggested, sizeof(suggested), "P", GetDataModel()->m_iCurrentPoint);
			snprintf(pt.point_id, sizeof(pt.point_id), DCP_POINT_ID_FMT, suggested);
		}
		snprintf(point_id, sizeof(point_id), DCP_POINT_ID_FMT, pt.point_id);
		m_pCommon->strbtrim(point_id);

		sTemp = point_id ; //GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id;
		//sTemp.Format(L"%-s",temp_point_table[GetDataModel()->m_iCurrentPoint-1].point_id);
		if(sTemp.IsEmpty())
			sTemp = L" ";
		m_pPointId->GetStringInputCtrl()->SetString(sTemp);
		
		StringC sHeight = L"-";
		StringC sDist = L"-";
		StringC sDistAlongLine  = L"-";
		StringC sLineOffset  = L"-";

		int decimals = m_pModel->m_nDecimals;
		
		// HEIGHT
		if(GetDataModel()->linefit_results[GetDataModel()->m_iCurrentPoint-1].sta == 1)
		{
			sHeight.Format(L"%.*f",decimals,GetDataModel()->linefit_results[GetDataModel()->m_iCurrentPoint-1].height_diff);
			sDist.Format(L"%.*f",decimals,GetDataModel()->linefit_results[GetDataModel()->m_iCurrentPoint-1].points_distance);
			sDistAlongLine.Format(L"%.*f",decimals,GetDataModel()->linefit_results[GetDataModel()->m_iCurrentPoint-1].distance_along_line);
			sLineOffset.Format(L"%.*f",decimals,GetDataModel()->linefit_results[GetDataModel()->m_iCurrentPoint-1].line_offset);
			
		}
		m_pHeight->GetStringInputCtrl()->SetString(sHeight);
		m_pDistFromLine->GetStringInputCtrl()->SetString(sDist);
		m_pDistAlongLine->GetStringInputCtrl()->SetString(sDistAlongLine);
		m_pLineOffset->GetStringInputCtrl()->SetString(sLineOffset);


	/*	if(!GetDataModel()->disable_point_editing)
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);
		}
		else
		{
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
			m_pPointId->GetStringInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
		}

		if(GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
			int decimals = m_pModel->m_nDecimals;
			sTemp.Format(L"%.*f",decimals,GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].x);
			m_pX->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%.*f",decimals,GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].y);
			m_pY->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%.*f",decimals,GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].z);
			m_pZ->GetStringInputCtrl()->SetString(sTemp);
		}
		else
		{
			m_pX->GetStringInputCtrl()->SetString(L" ");
			m_pY->GetStringInputCtrl()->SetString(L" ");
			m_pZ->GetStringInputCtrl()->SetString(L" ");
		}*/

	}
}
// Description: only accept DCP06 Model objects
bool DCP::LineFitMeasDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::LineFitMeasModel* pDcpModel = dynamic_cast< DCP::LineFitMeasModel* >( pModel );

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

void DCP::LineFitMeasDialog::SelectPoint(StringC strSelectedPoint,short iSelectedPointId)
{
	GetDataModel()->m_iCurrentPoint = iSelectedPointId;
	RefreshControls();

}
// ================================================================================================

void DCP::LineFitMeasDialog::delete_point()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINT_TOK);

	StringC strActivePointId(L"");

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	MsgBox MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->m_iPointsCount--;
		for(int i = GetDataModel()->m_iCurrentPoint-1; i < GetDataModel()->m_iMaxPoint; i++)
		{
			memcpy(&GetDataModel()->point_table[i], &GetDataModel()->point_table[i+1], sizeof(S_POINT_BUFF));	
			memcpy(&GetDataModel()->linefit_results[i], &GetDataModel()->linefit_results[i+1], sizeof(S_LINE_FITTING_RESULTS));	
		}
		memset(&GetDataModel()->point_table[GetDataModel()->m_iMaxPoint-1],0,sizeof(S_POINT_BUFF));
		memset(&GetDataModel()->linefit_results[GetDataModel()->m_iMaxPoint-1],0,sizeof(S_LINE_FITTING_RESULTS));
	}

	if(GetDataModel()->m_iPointsCount < GetDataModel()->m_iMinPoint)
		GetDataModel()->m_iPointsCount = GetDataModel()->m_iMinPoint;

	if(GetDataModel()->m_iCurrentPoint < 1) 
		GetDataModel()->m_iCurrentPoint = 1;

	if(GetDataModel()->m_iCurrentPoint > GetDataModel()->m_iPointsCount) 
		GetDataModel()->m_iCurrentPoint = GetDataModel()->m_iPointsCount;

	RefreshControls();
}
// ================================================================================================
void DCP::LineFitMeasDialog::next_point()
{
	if(GetDataModel()->m_iCurrentPoint < GetDataModel()->m_iPointsCount)
		GetDataModel()->m_iCurrentPoint++;
	else
		 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_LAST_POINT_TOK));

	RefreshControls();
}
// ================================================================================================
void DCP::LineFitMeasDialog::prev_point()
{
	if(GetDataModel()->m_iCurrentPoint > 1)
		GetDataModel()->m_iCurrentPoint--;

	RefreshControls();
}

// ================================================================================================

void DCP::LineFitMeasDialog::update_meas_values(double x, double y, double z, short /*DCP_POINT_STATUS*/ status)
{
	//Common common(m_pModel);

	if(m_pCommon->check_distance(x,y,z,GetDataModel()->point_table,GetDataModel()->m_iPointsCount, GetDataModel()->m_iCurrentPoint))
	{
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].x = x;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].y = y;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].z = z;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].cds = m_pModel->active_coodinate_system;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].sta = status;

		RefreshControls();
	}
}
// ================================================================================================
void DCP::LineFitMeasDialog::add_point()
{
	if(GetDataModel()->m_iPointsCount < GetDataModel()->m_iMaxPoint)
	{
		// 271011
		char point_id_buf[POINT_ID_BUFF_LEN];
		snprintf(point_id_buf, sizeof(point_id_buf), DCP_POINT_ID_FMT, GetDataModel()->point_table[GetDataModel()->m_iPointsCount-1].point_id);

		if(!m_pCommon->strblank(point_id_buf))
		{
			m_pCommon->inc_id(point_id_buf);
		}
		else
		{
			if(!m_pCommon->strblank(GetDataModel()->default_pid))
			{
				snprintf(point_id_buf, sizeof(point_id_buf), "%s%d", GetDataModel()->default_pid, GetDataModel()->m_iPointsCount+1);
			}
			else
			{
				// Phase D: Fallback when last point and default_pid both blank
				m_pCommon->get_suggested_next_point_id(point_id_buf, sizeof(point_id_buf), "P", GetDataModel()->m_iPointsCount + 1);
			}
		}

		GetDataModel()->m_iPointsCount++;
		GetDataModel()->m_iCurrentPoint = GetDataModel()->m_iPointsCount;
		snprintf(GetDataModel()->point_table[GetDataModel()->m_iPointsCount - 1].point_id, sizeof(GetDataModel()->point_table[0].point_id), DCP_POINT_ID_FMT, point_id_buf);
		RefreshControls();
	}
	else 
	{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_ADD_POINT_TOK);
		MsgBox MsgBox;
		MsgBox.ShowMessageOk(strMsg);
	}
}
// ================================================================================================
void DCP::LineFitMeasDialog::OnPointIdChanged( int unNotifyCode, int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		StringC sPoint;
		sPoint = m_pPointId->GetStringInputCtrl()->GetString();
		char cPoint[POINT_ID_BUFF_LEN];
		//UTL::UnicodeToAscii(cPoint,DCP_POINT_ID_LENGTH +1,sPoint);// +1 280508
		BSS::UTI::BSS_UTI_WCharToAscii(sPoint, cPoint, DCP_POINT_ID_LENGTH + 1);

		m_pCommon->strbtrim(cPoint);
		snprintf(GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id, POINT_ID_BUFF_LEN, DCP_POINT_ID_FMT, cPoint);
		RefreshControls();
	}
}



// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::LineFitMeasController::LineFitMeasController(Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel), m_pCommon(0),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_LINE_FITTING_POINTS_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::LineFitMeasDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( MEAS_DLG, m_pDlg, true );

	calcLineFit = new CalcLineFit(pModel);

	m_pCommon = new Common(pModel);
    // Set the function key
	isATR = pModel->isATR;
   show_function_keys();


	//poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
    
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

} //lint !e818 Pointer parameter could be declared as pointing to const


void DCP::LineFitMeasController::show_function_keys()
{
	ResetFunctionKeys();
	
	if(m_bCamera)
	{
		FKDef vDef;
		vDef.poOwner = this;
		
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06, K_DCP_MEAS_TOK);
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
		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06,K_DCP_MEAS_TOK);
		SetFunctionKey( FK1, vDef );

		// Phase E: Remove DIST; F2=NEXT, F3=PREV, F4=ADD, F5=CONT
		vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_ADD_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = L" ";
		SetFunctionKey( FK6, vDef );

		
		// SHIFT
		// CAPTIVATE
		//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
		//{
			vDef.strLable = StringC(AT_DCP06,K_DCP_CAMERA_TOK);
			SetFunctionKey( SHFK1, vDef );
		//}
		
		vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
		SetFunctionKey( SHFK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_INIT_TOK);
		SetFunctionKey( SHFK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_SPECIAL_TOK);
		SetFunctionKey( SHFK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
		SetFunctionKey( SHFK5, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CHANGE_FACE_TOK);
		SetFunctionKey( SHFK6, vDef );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::LineFitMeasController::~LineFitMeasController()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}

	if(calcLineFit)
	{
		delete calcLineFit;
		calcLineFit = 0;
	}
}
// Description: DCP06 model
DCP::LineFitMeasModel* DCP::LineFitMeasDialog::GetDataModel() const
{
    return (DCP::LineFitMeasModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// Description: Route model to everybody else
bool DCP::LineFitMeasController::SetModel( GUI::ModelC* pModel )
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

// Description: Handle change of position values
void DCP::LineFitMeasController::OnF1Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEAS_DLG);
		show_function_keys();
	}

	if(m_pModel->m_nOverWriteInfo )
	{
		if(m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
				StringC msg;
				MsgBox msgbox;
				msg.LoadTxt(AT_DCP06, M_DCP_OVERWRITE_POINT_TOK);
				msg.Format(msg,(const wchar_t*)StringC(m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id));
				if(!msgbox.ShowMessageYesNo(msg))
				//if(msgbox1(TXT_NIL_TOKEN, M_OVERWRITE_POINT_TOK,(void *) pid_ptr, MB_YESNO) == FALSE)
				{
					m_pDlg->RefreshControls();
					return;
				}
				m_pDlg->RefreshControls();
		}
	}
	if(m_pCommon->check_edm_mode())
	{

		DisableFunctionKey(FK1);
		DisableFunctionKey(FK2);
		DisableFunctionKey(FK3);
		DisableFunctionKey(FK4);
		DisableFunctionKey(FK5);
		DisableFunctionKey(FK6);


		DCP::MeasXYZModel* pModel = new MeasXYZModel;
		
		snprintf(pModel->sPointId, sizeof(pModel->sPointId), DCP_POINT_ID_FMT, m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);
		m_pCommon->strbtrim(pModel->sPointId);
		
		if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::MeasXYZController(m_pModel));
		}
		(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_XYZ_CONTROLLER, true);
	}	
	// ExecuteAll();
	/*
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
	*/
}

// ================================================================================================
// Description: F2 - NEXT POINT (Phase E: DIST removed)
// ================================================================================================
void DCP::LineFitMeasController::OnF2Pressed()
{
	m_pDlg->next_point();
} 
// PREV POINT
void DCP::LineFitMeasController::OnF3Pressed()
{
	m_pDlg->prev_point();
}
// ADD POINT
void DCP::LineFitMeasController::OnF4Pressed()
{
	m_pDlg->add_point();
}

// Phase E: F5 = CONT
void DCP::LineFitMeasController::OnF5Pressed()
{
	if (m_pDlg == nullptr)
	{
		USER_APP_VERIFY(false);
		return;
	}
	if (!m_bCamera)
	{
		m_pDlg->UpdateData();
		(void)Close(EC_KEY_CONT);
	}
	else
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEAS_DLG);
		show_function_keys();
	}
}
// Phase E: F6 unused (was CONT, now on F5)
void DCP::LineFitMeasController::OnF6Pressed()
{
	if (m_pDlg == nullptr)
	{
		USER_APP_VERIFY(false);
		return;
	}
	if (m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEAS_DLG);
		show_function_keys();
	}
}
void DCP::LineFitMeasController::OnSHF1Pressed()
{
	/* CAPTIVATE
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
// Description: Handle change of position values
void DCP::LineFitMeasController::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}

// INIT
void DCP::LineFitMeasController::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == nullptr)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::InitializationController );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pModel);
	SetActiveController(INIT_CONTROLLER, true);
}

void DCP::LineFitMeasController::OnSHF4Pressed()
{
	/*
	// SPECIAL MENU
	DCP::SpecialMenuDialog* poSpecialMenuDlg = new DCP::SpecialMenuDialog();
	AddDialog(SPECIAL_MENU,poSpecialMenuDlg); 	
	SetActiveDialog(SPECIAL_MENU, true);
	*/
	if(GetController(SPECIAL_MENU_CONTROLLER) == nullptr)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::SpecialMenuController(m_pModel, DISABLE_HIDDEN_POINT | DISABLE_HOME_POINTS | DISABLE_X_OR_Y_OR_Z |DISABLE_SEPARATE_REC) );
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel( m_pModel);
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);

}
// PID
void DCP::LineFitMeasController::OnSHF5Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		DCP::SelectPointModel* pModel = new SelectPointModel;
		
		// create point list
		for(int i = 0; i < m_pDlg->GetDataModel()->m_iPointsCount;i++)
		{
			pModel->points[i].no = i+1;
			snprintf(pModel->points[i].point_id, sizeof(pModel->points[i].point_id), DCP_POINT_ID_FMT, m_pDlg->GetDataModel()->point_table[i].point_id);
			sprintf(pModel->points[i].point_status,"%c", m_pDlg->GetDataModel()->point_table[i].sta != POINT_NOT_DEFINED ? '+': '-');
		}
		pModel->m_iCounts = m_pDlg->GetDataModel()->m_iPointsCount;

		if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
		}
		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
}
void DCP::LineFitMeasController::OnSHF6Pressed()
{
	
	//ChangeFace changeFace(CHANGE_FACE_CONTROLLER);
	
	// I<>II
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER, poChangeFace);
		SetActiveController(CHANGE_FACE_CONTROLLER, true);
	
}

// Description: React on close of tabbed dialog
void DCP::LineFitMeasController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(MEAS_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// Description: React on close of controller
void DCP::LineFitMeasController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// SELECT POINT
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectPointModel* pModel = (DCP::SelectPointModel*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		StringC strSelectedPoint = pModel->m_strSelectedPoint;
		short iSelectedPointId = pModel->m_iSelectedId;
		m_pDlg->SelectPoint(strSelectedPoint,iSelectedPointId);
	}
	
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
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;
			m_pDlg->update_meas_values(x,y,z, POINT_MEASURED);
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].x = x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].y = y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].z = z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].sta = 1;

			calculate_linefit(m_pDlg->GetDataModel()->m_iCurrentPoint);
			/*if(m_pModel->m_nAutoIncrement)
			{
				if(m_pDlg->GetDataModel()->m_iCurrentPoint < m_pDlg->GetDataModel()->m_iPointsCount)
					m_pDlg->GetDataModel()->m_iCurrentPoint++;
			}*/
		}
	}
	else if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{/*
		MsgBox MsgBox;
		char exit_msg_buf[100];
		sprintf(exit_msg_buf,"%s (%d)", "Exit code", lExitCode);
		MsgBox.ShowMessageOk(StringC(exit_msg_buf));
		*/
	}
	// SPECIAL MENU
	// **********************************************************************************************************
	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		// **********************************************************************************************************
		if(lExitCode == HIDDEN_POINT)
		{
			MsgBox msgbox;
			msgbox.ShowMessageOk(L"Cannot use the hiddenpoint");

			//// create model
			//DCP::PointBuffModel* pModel = new PointBuffModel;

			//if(GetController(HIDDENPOINT_CONTROLLER) == nullptr)
			//{
			//	(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::HiddenPointController(m_pModel));
			//}
			//(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel(pModel/* m_pDlg->GetDataModel()*/);
			//SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		// **********************************************************************************************************
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			MsgBox msgbox;
			msgbox.ShowMessageOk(L"Cannot use the hiddenpoint");

			//// create model
			//DCP::PointBuffModel* pModel = new PointBuffModel;
			//
			//sprintf(pModel->m_pPointBuff[0].point_id,"%s", m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);


			//if(GetController(XYZ_CONTROLLER) == nullptr)
			//{
			//	(void)AddController( XYZ_CONTROLLER, new DCP::XYZController( m_pModel));
			//}
			//(void)GetController( XYZ_CONTROLLER )->SetModel( pModel);
			//SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			// create model
			DCP::PointBuffModel* pModel = new PointBuffModel;

			if(GetController(CIRCLE_CONTROLLER) == nullptr)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::CircleController(m_pModel));
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel );
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == MID_POINT)
		{
			DCP::PointBuffModel* pModel = new PointBuffModel;

			snprintf(pModel->m_pPointBuff[0].point_id, sizeof(pModel->m_pPointBuff[0].point_id), DCP_POINT_ID_FMT, "");
			
			if(GetController(MID_POINT_CONTROLLER) == nullptr)
			{
				(void)AddController( MID_POINT_CONTROLLER, new DCP::MidPointController(m_pModel));
			}
			(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
			SetActiveController(MID_POINT_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			// create model
			DCP::PointBuffModel* pModel = new PointBuffModel;

			if(GetController(HOME_POINTS_CONTROLLER) == nullptr)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::HomePointsController(m_pModel) );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel(m_pModel/* m_pDlg->GetDataModel()*/);
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}
	}

	
	// CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( CIRCLE_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta = 1;

			//m_pDlg->update_meas_values(x,y,z, POINT_MEASURED);
		m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].x =  pModel->m_pPointBuff[0].x;
		m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].y =  pModel->m_pPointBuff[0].y;
		m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].z =  pModel->m_pPointBuff[0].z;
		m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].sta = 1;

		calculate_linefit(m_pDlg->GetDataModel()->m_iCurrentPoint);
			/*m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].x = x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].y = y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].z = z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].sta = 1;

		calculate_linefit(m_pDlg->GetDataModel()->m_iCurrentPoint);*/
		}
	}
	if(lCtrlID == MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( MID_POINT_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta = 1;
			
			calculate_linefit(m_pDlg->GetDataModel()->m_iCurrentPoint);
		}
	}

	
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

void DCP::LineFitMeasController::calculate_linefit(short iCurrPoint)
{

	calcLineFit->CalcPoint(iCurrPoint -1, &m_pDlg->GetDataModel()->pLline_buff[0],
		&m_pDlg->GetDataModel()->point_table[0],
		m_pDlg->GetDataModel()->linefit_results,
		m_pDlg->GetDataModel()->manualHeight,
		m_pDlg->GetDataModel()->shiftValue,
		m_pDlg->GetDataModel()->rotateAngle,
		m_pDlg->GetDataModel()->selectedHeight,
		m_pDlg->GetDataModel()->selectedShift,
		m_pDlg->GetDataModel()->selectedRotate,
		m_pDlg->GetDataModel()->align321Model,
		&m_pDlg->GetDataModel()->pLline_buff_ocs[0],
		&m_pDlg->GetDataModel()->points_in_line[0],
		m_pDlg->GetDataModel()->selectedRefLine);


	//if( m_pDlg->GetDataModel()->point_table[iCurrPoint-1].sta == 1) 
	//{
	//	double z = m_pDlg->GetDataModel()->point_table[iCurrPoint-1].z;
	//	double zDiff = 0.0;

	//	if(m_pDlg->GetDataModel()->selectedHeight == FIRST_POINT)
	//	{
	//		zDiff = z - m_pDlg->GetDataModel()->pLline_buff[0].points[0].z;
	//		m_pDlg->GetDataModel()->linefit_results[iCurrPoint - 1].height_diff = zDiff;
	//		m_pDlg->GetDataModel()->linefit_results[iCurrPoint - 1].sta = 1;
	//	}
	//}
}

void DCP::LineFitMeasController::OnControllerActivated(void)
{
//	ActivateMeasurement();
}
void DCP::LineFitMeasController::OnControllerClosed(int lExitCode)
{
//	DeactivateMeasurement(lExitCode);
}

// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::LineFitMeasModel::LineFitMeasModel()
{
	m_iMinPoint = 0;
	m_iMaxPoint = 0;
	m_iPointsCount = 0;
	m_iCurrentPoint = 1;
	default_pid[0] = '\0';
	memset(&point_table[0],0,sizeof(S_POINT_BUFF)* MAX_LINEFIT_POINTS);
	memset(&points_in_line[0],0,sizeof(S_POINT_BUFF)* MAX_LINEFIT_POINTS);

	memset(&linefit_results[0], 0, sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);

	//memset(&point_table2[0],0,sizeof(S_POINT_BUFF)* 20);

	disable_point_editing = false;
}
DCP::LineFitMeasModel::~LineFitMeasModel()
{
}
