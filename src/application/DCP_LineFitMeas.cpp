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
#include <dcp06/init/DCP_DCP05Init.hpp>
#include <dcp06/application/DCP_LineFitMeas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/measurement/DCP_MeasXYZ.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_SeparateRec.hpp>
#include <dcp06/measurement/DCP_HomePoints.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_ChangeFace.hpp>
#include <dcp06/core/DCP_SelectPoint.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/measurement/DCP_MeasDist.hpp>
#include <dcp06/measurement/DCP_MidPointController.hpp>
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
OBS_IMPLEMENT_EXECUTE(DCP::DCP05LineFitMeasDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP05LineFitMeasDlgC::DCP05LineFitMeasDlgC(DCP::DCP05ModelC *pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointId(0),m_pHeight(0),m_pDistFromLine(0),m_pDistAlongLine(0),m_pLineOffset(0)/*,m_pInfo(0)*/,m_pDCP05Model(pDCP05Model),
	m_pPointIdObserver(OBS_METHOD_TO_PARAM0(DCP05LineFitMeasDlgC, OnPointIdChanged), this),
	iInfoInd(0),m_pCommon(0)
{
	//SetTxtApplicationId(AT_DCP05);
	

	m_pCommon = new DCP05CommonC(pDCP05Model);
	strInfoText.LoadTxt(AT_DCP05,P_DCP_INFO_TOK);

	//memset(temp_point_table,0, sizeof(S_POINT_BUFF)*20);
	//memset(temp_point_table2,0, sizeof(S_POINT_BUFF)*20);
}


            // Description: Destructor
DCP::DCP05LineFitMeasDlgC::~DCP05LineFitMeasDlgC()
{
	 //m_pTimer.KillTimer();
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP05LineFitMeasDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	// Add fields to dialog
	m_pPointNo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointNo->SetId(ePointNo);
	m_pPointNo->SetText(StringC(AT_DCP05,P_DCP_POINT_NO_TOK));
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pPointNo->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pPointNo);

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP05,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	// m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pPointId->SetEmptyAllowed(true);
	AddCtrl(m_pPointId);

	// InsertEmptyLine(); CAPTIVATE

	m_pHeight = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pHeight->SetId(eHeight);
	m_pHeight->SetText(StringC(AT_DCP05,P_DCP_LINEFIT_HEIGHT_TOK));
	//m_pHeight->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pHeight->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pHeight->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//
	// m_pX->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pHeight->SetEmptyAllowed(true);
	AddCtrl(m_pHeight);

	

	m_pDistAlongLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistAlongLine->SetId(eDistAlongLine);
	m_pDistAlongLine->SetText(StringC(AT_DCP05,P_DCP_LINEFIT_DIST_ALONG_LINE_TOK));
	//m_pDistAlongLine->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pDistAlongLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDistAlongLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pDistAlongLine->SetEmptyAllowed(true);
	AddCtrl(m_pDistAlongLine);
	
	m_pLineOffset = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLineOffset->SetId(eLineOffset);
	m_pLineOffset->SetText(StringC(AT_DCP05,P_DCP_LINEFIT_LINE_OFFSET_TOK));
	//m_pDistAlongLine->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pLineOffset->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pLineOffset->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pLineOffset->SetEmptyAllowed(true);
	AddCtrl(m_pLineOffset);

	m_pDistFromLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDistFromLine->SetId(eDistFromLine);
	m_pDistFromLine->SetText(StringC(AT_DCP05,P_DCP_LINEFIT_DIST_FROM_LINE_TOK));
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

void DCP::DCP05LineFitMeasDlgC::OnDialogActivated()
{
	// get last defined point
	//DCP05CommonC common(m_pDCP05Model);
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );

	GetDataModel()->m_iPointsCount = m_pCommon->get_last_defined_point(&GetDataModel()->point_table[0],GetDataModel()->m_iMaxPoint);

	if(GetDataModel()->m_iPointsCount  < GetDataModel()->m_iMinPoint)
		GetDataModel()->m_iPointsCount = GetDataModel()->m_iMinPoint;

	RefreshControls();
}

//void DCP::DCP05LineFitMeasDlgC::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

void DCP::DCP05LineFitMeasDlgC::UpdateData()
{
}

// Description: refresh all controls
void DCP::DCP05LineFitMeasDlgC::RefreshControls()
{
	if(m_pPointNo && m_pPointId && m_pHeight && m_pDistFromLine && m_pDistAlongLine && m_pLineOffset)
	{
		if(GetDataModel()->selectedRefLine == REF_LINE_VERTICAL)
		{
			m_pHeight->SetText(StringC(AT_DCP05,P_DCP_LINEFIT_DEPTH_TOK));
		}

		StringC sTemp;
		char point_id[10];
		sTemp.Format(L"%d/%d",GetDataModel()->m_iCurrentPoint, GetDataModel()->m_iPointsCount);
		m_pPointNo->GetStringInputCtrl()->SetString(sTemp);
		
		sprintf(point_id,"%-s",GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id);
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

		int decimals = m_pDCP05Model->m_nDecimals;
		
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
			int decimals = m_pDCP05Model->m_nDecimals;
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
// Description: only accept hello world Model objects
bool DCP::DCP05LineFitMeasDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05LineFitMeasModelC* pDCP05Model = dynamic_cast< DCP::DCP05LineFitMeasModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}
// ================================================================================================

void DCP::DCP05LineFitMeasDlgC::SelectPoint(StringC strSelectedPoint,short iSelectedPointId)
{
	GetDataModel()->m_iCurrentPoint = iSelectedPointId;
	RefreshControls();

}
// ================================================================================================

void DCP::DCP05LineFitMeasDlgC::delete_point()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_TOK);

	StringC strActivePointId(L"");

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	DCP05MsgBoxC MsgBox;
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
void DCP::DCP05LineFitMeasDlgC::next_point()
{
	if(GetDataModel()->m_iCurrentPoint < GetDataModel()->m_iPointsCount)
		GetDataModel()->m_iCurrentPoint++;
	else
		 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP05,I_DCP_LAST_POINT_TOK));

	RefreshControls();
}
// ================================================================================================
void DCP::DCP05LineFitMeasDlgC::prev_point()
{
	if(GetDataModel()->m_iCurrentPoint > 1)
		GetDataModel()->m_iCurrentPoint--;

	RefreshControls();
}

// ================================================================================================

void DCP::DCP05LineFitMeasDlgC::update_meas_values(double x, double y, double z, short /*DCP_POINT_STATUS*/ status)
{
	//DCP05CommonC common(m_pDCP05Model);

	if(m_pCommon->check_distance(x,y,z,GetDataModel()->point_table,GetDataModel()->m_iPointsCount, GetDataModel()->m_iCurrentPoint))
	{
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].x = x;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].y = y;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].z = z;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].cds = m_pDCP05Model->active_coodinate_system;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].sta = status;

		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP05LineFitMeasDlgC::add_point()
{
	if(GetDataModel()->m_iPointsCount < GetDataModel()->m_iMaxPoint)
	{
		// 271011
		char temp[10];
		sprintf(temp,"%-s",GetDataModel()->point_table[GetDataModel()->m_iPointsCount-1].point_id);

		if(!m_pCommon->strblank(temp))
		{
			m_pCommon->inc_id(temp);
		}
		else
		{
			if(!m_pCommon->strblank(GetDataModel()->default_pid))
			{
				sprintf(temp,"%s%d",GetDataModel()->default_pid,GetDataModel()->m_iPointsCount+1);
			}
		}

		GetDataModel()->m_iPointsCount++;
		GetDataModel()->m_iCurrentPoint = GetDataModel()->m_iPointsCount;
		RefreshControls();
	}
	else 
	{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP05,M_DCP_CANNOT_ADD_POINT_TOK);
		DCP05MsgBoxC MsgBox;
		MsgBox.ShowMessageOk(strMsg);
	}
}
// ================================================================================================
void DCP::DCP05LineFitMeasDlgC::OnPointIdChanged( int unNotifyCode, int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		StringC sPoint;
		sPoint = m_pPointId->GetStringInputCtrl()->GetString();
		char cPoint[10];
		//UTL::UnicodeToAscii(cPoint,DCP_POINT_ID_LENGTH +1,sPoint);// +1 280508
		BSS::UTI::BSS_UTI_WCharToAscii(sPoint, cPoint,DCP_POINT_ID_LENGTH +1);

		m_pCommon->strbtrim(cPoint);
		sprintf(GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id,"%s",cPoint);
		RefreshControls();
	}
}



// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05LineFitMeasControllerC::DCP05LineFitMeasControllerC(DCP05ModelC *pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model), m_pCommon(0),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_LINE_FITTING_POINTS_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05LineFitMeasDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( MEAS_DLG, m_pDlg, true );

	calcLineFit = new DCP05CalcLineFitC(pDCP05Model);

	m_pCommon = new DCP05CommonC(pDCP05Model);
    // Set the function key
	isATR = pDCP05Model->isATR;
   show_function_keys();


	//poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
    
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

} //lint !e818 Pointer parameter could be declared as pointing to const


void DCP::DCP05LineFitMeasControllerC::show_function_keys()
{
	ResetFunctionKeys();
	
	if(m_bCamera)
	{
		FKDef vDef;
		vDef.poOwner = this;
		
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP05, K_DCP_ALL_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP05, K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );

		FKDef vDef1;
			//vDef1.nAppId = AT_DCP05;
			vDef1.poOwner = this;
			vDef1.strLable = L" ";;
			SetFunctionKey( SHFK1, vDef1 );
		SetFunctionKey( SHFK6, vDef1 );
		
		GUI::DesktopC::Instance()->UpdateFunctionKeys();
	}
	else 
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP05,K_DCP_ALL_TOK);
		SetFunctionKey( FK1, vDef );

		if(isATR)
		{
			vDef.strLable = StringC(AT_DCP05,K_DCP_DIST_TOK);
			SetFunctionKey( FK2, vDef );
		}


		vDef.strLable = StringC(AT_DCP05,K_DCP_NEXT_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_PREV_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_ADD_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );
		
		// SHIFT
		// CAPTIVATE
		//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
		//{
			vDef.strLable = StringC(AT_DCP05,K_DCP_CAMERA_TOK);
			SetFunctionKey( SHFK1, vDef );
		//}
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
		SetFunctionKey( SHFK2, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_INIT_TOK);
		SetFunctionKey( SHFK3, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_SPECIAL_TOK);
		SetFunctionKey( SHFK4, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_ID_TOK);
		SetFunctionKey( SHFK5, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CHANGE_FACE_TOK);
		SetFunctionKey( SHFK6, vDef );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::DCP05LineFitMeasControllerC::~DCP05LineFitMeasControllerC()
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
// Description: Hello World model
DCP::DCP05LineFitMeasModelC* DCP::DCP05LineFitMeasDlgC::GetDataModel() const
{
    return (DCP::DCP05LineFitMeasModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// Description: Route model to everybody else
bool DCP::DCP05LineFitMeasControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// Description: Handle change of position values
void DCP::DCP05LineFitMeasControllerC::OnF1Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEAS_DLG);
		show_function_keys();
	}

	if(m_pDCP05Model->m_nOverWriteInfo )
	{
		if(m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
				StringC msg;
				DCP05MsgBoxC msgbox;
				msg.LoadTxt(AT_DCP05, M_DCP_OVERWRITE_POINT_TOK);
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


		DCP::DCP05MeasXYZModelC* pModel = new DCP05MeasXYZModelC;
		
		sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);
		m_pCommon->strbtrim(pModel->sPointId);
		
		if(GetController(MEAS_XYZ_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::DCP05MeasXYZControllerC(m_pDCP05Model));
		}
		(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_XYZ_CONTROLLER, true);
	}	
	// ExecuteAll();
	/*
    if (m_pDlg == NULL)
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
// Description: F2
// ================================================================================================
void DCP::DCP05LineFitMeasControllerC::OnF2Pressed()
{
	//// DIST
	if(m_pCommon->check_edm_mode())
	{
		DisableFunctionKey(FK1);
		DisableFunctionKey(FK2);
		DisableFunctionKey(FK3);
		DisableFunctionKey(FK4);
		DisableFunctionKey(FK5);
		DisableFunctionKey(FK6);

		DCP::DCP05MeasDistModelC* pModel = new DCP05MeasDistModelC;

		if(GetController(MEAS_DIST_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_DIST_CONTROLLER, new DCP::DCP05MeasDistControllerC(m_pDCP05Model));
		}
		(void)GetController( MEAS_DIST_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_DIST_CONTROLLER, true);
	}
} 
// NEXT POINT
void DCP::DCP05LineFitMeasControllerC::OnF3Pressed()
{
	m_pDlg->next_point();
}
// previous point
void DCP::DCP05LineFitMeasControllerC::OnF4Pressed()
{
	m_pDlg->prev_point();
}

// Add point
void DCP::DCP05LineFitMeasControllerC::OnF5Pressed()
{
	m_pDlg->add_point();
}
// Description: Handle change of position values
void DCP::DCP05LineFitMeasControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(!m_bCamera)
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
    // Update model
    // Set it to hello world dialog
   // m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    //(void)Close(EC_KEY_CONT);
}
void DCP::DCP05LineFitMeasControllerC::OnSHF1Pressed()
{
	/* CAPTIVATE
	if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
	{

		if(poVideoDlg == 0)
		{
			poVideoDlg = new ABL::VideoDialogC( AT_DCP05);
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
void DCP::DCP05LineFitMeasControllerC::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}

// INIT
void DCP::DCP05LineFitMeasControllerC::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP05InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP05Model);
	SetActiveController(INIT_CONTROLLER, true);
}

void DCP::DCP05LineFitMeasControllerC::OnSHF4Pressed()
{
	/*
	// SPECIAL MENU
	DCP::DCP05SpecialMenuDlgC* poSpecialMenuDlg = new DCP::DCP05SpecialMenuDlgC();
	AddDialog(SPECIAL_MENU,poSpecialMenuDlg); 	
	SetActiveDialog(SPECIAL_MENU, true);
	*/
	if(GetController(SPECIAL_MENU_CONTROLLER) == NULL)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::DCP05SpecialMenuControllerC(m_pDCP05Model, DISABLE_HIDDEN_POINT | DISABLE_HOME_POINTS | DISABLE_X_OR_Y_OR_Z |DISABLE_SEPARATE_REC) );
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel( m_pDCP05Model);
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);

}
// PID
void DCP::DCP05LineFitMeasControllerC::OnSHF5Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		DCP::DCP05SelectPointModelC* pModel = new DCP05SelectPointModelC;
		
		// create point list
		for(int i = 0; i < m_pDlg->GetDataModel()->m_iPointsCount;i++)
		{
			pModel->points[i].no = i+1;
			sprintf(pModel->points[i].point_id,"%-6.6s", m_pDlg->GetDataModel()->point_table[i].point_id);
			sprintf(pModel->points[i].point_status,"%c", m_pDlg->GetDataModel()->point_table[i].sta != POINT_NOT_DEFINED ? '+': '-');
		}
		pModel->m_iCounts = m_pDlg->GetDataModel()->m_iPointsCount;

		if(GetController(SELECT_POINT_CONTROLLER) == NULL)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP05SelectPointControllerC );
		}
		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
}
void DCP::DCP05LineFitMeasControllerC::OnSHF6Pressed()
{
	
	//DCP05ChangeFaceC changeFace(CHANGE_FACE_CONTROLLER);
	
	// I<>II
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER, poChangeFace);
		SetActiveController(CHANGE_FACE_CONTROLLER, true);
	
}

// Description: React on close of tabbed dialog
void DCP::DCP05LineFitMeasControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(MEAS_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// Description: React on close of controller
void DCP::DCP05LineFitMeasControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// SELECT POINT
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectPointModelC* pModel = (DCP::DCP05SelectPointModelC*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
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
			DCP::DCP05MeasDistModelC* pModel = (DCP::DCP05MeasDistModelC*) GetController( MEAS_DIST_CONTROLLER )->GetModel();
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
			DCP::DCP05MeasXYZModelC* pModel = (DCP::DCP05MeasXYZModelC*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;
			m_pDlg->update_meas_values(x,y,z, POINT_MEASURED);
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].x = x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].y = y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].z = z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint - 1].sta = 1;

			calculate_linefit(m_pDlg->GetDataModel()->m_iCurrentPoint);
			/*if(m_pDCP05Model->m_nAutoIncrement)
			{
				if(m_pDlg->GetDataModel()->m_iCurrentPoint < m_pDlg->GetDataModel()->m_iPointsCount)
					m_pDlg->GetDataModel()->m_iCurrentPoint++;
			}*/
		}
	}
	else if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{/*
		DCP05MsgBoxC MsgBox;
		char temp[100];
		sprintf(temp,"%s (%d)", "Exit code", lExitCode);
		MsgBox.ShowMessageOk(StringC(temp));
		*/
	}
	// SPECIAL MENU
	// **********************************************************************************************************
	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		// **********************************************************************************************************
		if(lExitCode == HIDDEN_POINT)
		{
			DCP05MsgBoxC msgbox;
			msgbox.ShowMessageOk(L"Cannot use the hiddenpoint");

			//// create model
			//DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			//if(GetController(HIDDENPOINT_CONTROLLER) == NULL)
			//{
			//	(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::DCP05HiddenPointControllerC(m_pDCP05Model));
			//}
			//(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel(pModel/* m_pDlg->GetDataModel()*/);
			//SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		// **********************************************************************************************************
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			DCP05MsgBoxC msgbox;
			msgbox.ShowMessageOk(L"Cannot use the hiddenpoint");

			//// create model
			//DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;
			//
			//sprintf(pModel->m_pPointBuff[0].point_id,"%s", m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);


			//if(GetController(XYZ_CONTROLLER) == NULL)
			//{
			//	(void)AddController( XYZ_CONTROLLER, new DCP::DCP05XYZControllerC( m_pDCP05Model));
			//}
			//(void)GetController( XYZ_CONTROLLER )->SetModel( pModel);
			//SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			if(GetController(CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::DCP05CircleControllerC(m_pDCP05Model));
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel );
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == MID_POINT)
		{
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", "");
			
			if(GetController(MID_POINT_CONTROLLER) == NULL)
			{
				(void)AddController( MID_POINT_CONTROLLER, new DCP::DCP05MidPointControllerC(m_pDCP05Model));
			}
			(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
			SetActiveController(MID_POINT_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			if(GetController(HOME_POINTS_CONTROLLER) == NULL)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::DCP05HomePointsControllerC(m_pDCP05Model) );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel(m_pDCP05Model/* m_pDlg->GetDataModel()*/);
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}
	}

	
	// CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( CIRCLE_CONTROLLER )->GetModel();		
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
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( MID_POINT_CONTROLLER )->GetModel();		
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

void DCP::DCP05LineFitMeasControllerC::calculate_linefit(short iCurrPoint)
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
		m_pDlg->GetDataModel()->domModel,
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

void DCP::DCP05LineFitMeasControllerC::OnControllerActivated(void)
{
//	ActivateMeasurement();
}
void DCP::DCP05LineFitMeasControllerC::OnControllerClosed(int lExitCode)
{
//	DeactivateMeasurement(lExitCode);
}

// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP05LineFitMeasModelC::DCP05LineFitMeasModelC()
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
DCP::DCP05LineFitMeasModelC::~DCP05LineFitMeasModelC()
{
}
