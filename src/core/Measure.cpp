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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/core/Measure.hpp>
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
OBS_IMPLEMENT_EXECUTE(DCP::DCP06MeasDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06MeasDlgC::DCP06MeasDlgC(DCP::DCP06ModelC *pDCP06Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0)/*,m_pInfo(0)*/,m_pDCP06Model(pDCP06Model),
	m_pPointIdObserver(OBS_METHOD_TO_PARAM0(DCP06MeasDlgC, OnPointIdChanged), this),
	iInfoInd(0),m_pCommon(0)
{
	//SetTxtApplicationId(AT_DCP06);
	

	m_pCommon = new DCP06CommonC(pDCP06Model);
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);

	//memset(temp_point_table,0, sizeof(S_POINT_BUFF)*20);
	//memset(temp_point_table2,0, sizeof(S_POINT_BUFF)*20);
}


            // Description: Destructor
DCP::DCP06MeasDlgC::~DCP06MeasDlgC()
{
	 //m_pTimer.KillTimer();
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP06MeasDlgC::OnInitDialog(void)
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

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP06,P_DCP_X_TOK));
	m_pX->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//
	// m_pX->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE

	m_pX->SetEmptyAllowed(true);
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP06,P_DCP_Y_TOK));
	m_pY->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pY->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pY->SetEmptyAllowed(true);
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP06,P_DCP_Z_TOK));
	m_pZ->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pZ->SetEmptyAllowed(true);
	AddCtrl(m_pZ);
	
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

void DCP::DCP06MeasDlgC::OnDialogActivated()
{
	// get last defined point
	DCP06CommonC common(m_pDCP06Model);
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );

	GetDataModel()->m_iPointsCount = common.get_last_defined_point(&GetDataModel()->point_table[0],GetDataModel()->m_iMaxPoint);

	if(GetDataModel()->m_iPointsCount  < GetDataModel()->m_iMinPoint)
		GetDataModel()->m_iPointsCount = GetDataModel()->m_iMinPoint;

	RefreshControls();
}

//void DCP::DCP06MeasDlgC::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

void DCP::DCP06MeasDlgC::UpdateData()
{
}

// Description: refresh all controls
void DCP::DCP06MeasDlgC::RefreshControls()
{
	if(m_pPointNo && m_pPointId && m_pX && m_pY && m_pZ)
	{
		
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

		if(!GetDataModel()->disable_point_editing)
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
			int decimals = m_pDCP06Model->m_nDecimals;
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
		}

	}
}
// Description: only accept hello world Model objects
bool DCP::DCP06MeasDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06MeasModelC* pDCP06Model = dynamic_cast< DCP::DCP06MeasModelC* >( pModel );

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

void DCP::DCP06MeasDlgC::SelectPoint(StringC strSelectedPoint,short iSelectedPointId)
{
	GetDataModel()->m_iCurrentPoint = iSelectedPointId;
	RefreshControls();

}
// ================================================================================================

void DCP::DCP06MeasDlgC::delete_point()
{

	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_POINTS_TOK);

	StringC strActivePointId(GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id);

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	DCP06MsgBoxC MsgBox;
	short ret = MsgBox.DeletePointsMeas(strMsg);

	//if(MsgBox.ShowMessageYesNo(strMsg))
	if(ret == 2) // current point
	{
		GetDataModel()->m_iPointsCount--;
		for(int i = GetDataModel()->m_iCurrentPoint-1; i < GetDataModel()->m_iMaxPoint; i++)
		{
			memcpy(&GetDataModel()->point_table[i], &GetDataModel()->point_table[i+1], sizeof(S_POINT_BUFF));	
		}
		memset(&GetDataModel()->point_table[GetDataModel()->m_iMaxPoint-1],0,sizeof(S_POINT_BUFF));
	}
	else if(ret == 1) // all points
	{
		GetDataModel()->m_iCurrentPoint = 1;
		memset(&GetDataModel()->point_table[0],0,sizeof(S_POINT_BUFF)*GetDataModel()->m_iMaxPoint);
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
void DCP::DCP06MeasDlgC::next_point()
{
	if(GetDataModel()->m_iCurrentPoint < GetDataModel()->m_iPointsCount)
		GetDataModel()->m_iCurrentPoint++;
	else
		 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_DCP_LAST_POINT_TOK));

	RefreshControls();
}
// ================================================================================================
void DCP::DCP06MeasDlgC::prev_point()
{
	if(GetDataModel()->m_iCurrentPoint > 1)
		GetDataModel()->m_iCurrentPoint--;

	RefreshControls();
}

// ================================================================================================

void DCP::DCP06MeasDlgC::update_meas_values(double x, double y, double z, short /*DCP_POINT_STATUS*/ status)
{
	DCP06CommonC common(m_pDCP06Model);

	if(common.check_distance(x,y,z,GetDataModel()->point_table,GetDataModel()->m_iPointsCount, GetDataModel()->m_iCurrentPoint))
	{
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].x = x;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].y = y;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].z = z;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].cds = m_pDCP06Model->active_coodinate_system;
		GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].sta = status;

		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP06MeasDlgC::add_point()
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
		strMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_ADD_POINT_TOK);
		DCP06MsgBoxC MsgBox;
		MsgBox.ShowMessageOk(strMsg);
	}
}
// ================================================================================================
void DCP::DCP06MeasDlgC::OnPointIdChanged( int unNotifyCode, int ulParam2)
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
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06MeasControllerC::DCP06MeasControllerC(DCP::DCP06ModelC *pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model), m_pCommon(0),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_MEAS_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP06MeasDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( MEAS_DLG, m_pDlg, true );

	m_pCommon = new DCP06CommonC(pDCP06Model);
    // Set the function key
	isATR = pDCP06Model->isATR;
   show_function_keys();


	//poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
    
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

} //lint !e818 Pointer parameter could be declared as pointing to const


void DCP::DCP06MeasControllerC::show_function_keys()
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
		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06,K_DCP_ALL_TOK);
		SetFunctionKey( FK1, vDef );

		if(isATR)
		{
			vDef.strLable = StringC(AT_DCP06,K_DCP_DIST_TOK);
			SetFunctionKey( FK2, vDef );
		}


		vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_ADD_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
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

DCP::DCP06MeasControllerC::~DCP06MeasControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// Description: Hello World model
DCP::DCP06MeasModelC* DCP::DCP06MeasDlgC::GetDataModel() const
{
    return (DCP::DCP06MeasModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// Description: Route model to everybody else
bool DCP::DCP06MeasControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
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

// Description: Handle change of position values
void DCP::DCP06MeasControllerC::OnF1Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEAS_DLG);
		show_function_keys();
	}

	if(m_pDCP06Model->m_nOverWriteInfo )
	{
		if(m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
				StringC msg;
				DCP06MsgBoxC msgbox;
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

		DCP::DCP06MeasXYZModelC* pModel = new DCP06MeasXYZModelC;
		
		sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);
		m_pCommon->strbtrim(pModel->sPointId);
		
		if(GetController(MEAS_XYZ_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::DCP06MeasXYZControllerC(m_pDCP06Model));
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
void DCP::DCP06MeasControllerC::OnF2Pressed()
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

		DCP::DCP06MeasDistModelC* pModel = new DCP06MeasDistModelC;

		if(GetController(MEAS_DIST_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_DIST_CONTROLLER, new DCP::DCP06MeasDistControllerC(m_pDCP06Model));
		}
		(void)GetController( MEAS_DIST_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_DIST_CONTROLLER, true);
	}
} 
// NEXT POINT
void DCP::DCP06MeasControllerC::OnF3Pressed()
{
	m_pDlg->next_point();
}
// previous point
void DCP::DCP06MeasControllerC::OnF4Pressed()
{
	m_pDlg->prev_point();
}

// Add point
void DCP::DCP06MeasControllerC::OnF5Pressed()
{
	m_pDlg->add_point();
}
// Description: Handle change of position values
void DCP::DCP06MeasControllerC::OnF6Pressed()
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
void DCP::DCP06MeasControllerC::OnSHF1Pressed()
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
void DCP::DCP06MeasControllerC::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}

// INIT
void DCP::DCP06MeasControllerC::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP06InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP06Model);
	SetActiveController(INIT_CONTROLLER, true);
}

void DCP::DCP06MeasControllerC::OnSHF4Pressed()
{
	/*
	// SPECIAL MENU
	DCP::DCP06SpecialMenuDlgC* poSpecialMenuDlg = new DCP::DCP06SpecialMenuDlgC();
	AddDialog(SPECIAL_MENU,poSpecialMenuDlg); 	
	SetActiveDialog(SPECIAL_MENU, true);
	*/
	if(GetController(SPECIAL_MENU_CONTROLLER) == NULL)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::DCP06SpecialMenuControllerC(m_pDCP06Model) );
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel( m_pDCP06Model);
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);

}
// PID
void DCP::DCP06MeasControllerC::OnSHF5Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		DCP::DCP06SelectPointModelC* pModel = new DCP06SelectPointModelC;
		
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
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP06SelectPointControllerC );
		}
		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
}
void DCP::DCP06MeasControllerC::OnSHF6Pressed()
{
	
	//DCP06ChangeFaceC changeFace(CHANGE_FACE_CONTROLLER);
	
	// I<>II
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER, poChangeFace);
		SetActiveController(CHANGE_FACE_CONTROLLER, true);
	
}

// Description: React on close of tabbed dialog
void DCP::DCP06MeasControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(MEAS_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// Description: React on close of controller
void DCP::DCP06MeasControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// SELECT POINT
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectPointModelC* pModel = (DCP::DCP06SelectPointModelC*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		StringC strSelectedPoint = pModel->m_strSelectedPoint;
		short iSelectedPointId = pModel->m_iSelectedId;
		m_pDlg->SelectPoint(strSelectedPoint,iSelectedPointId);
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
			DCP::DCP06MeasDistModelC* pModel = (DCP::DCP06MeasDistModelC*) GetController( MEAS_DIST_CONTROLLER )->GetModel();
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
			DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;
			m_pDlg->update_meas_values(x,y,z, POINT_MEASURED);
			
			if(m_pDCP06Model->m_nAutoIncrement)
			{
				if(m_pDlg->GetDataModel()->m_iCurrentPoint < m_pDlg->GetDataModel()->m_iPointsCount)
					m_pDlg->GetDataModel()->m_iCurrentPoint++;
			}
		}
	}
	else if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{/*
		DCP06MsgBoxC MsgBox;
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
			// create model
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;

			if(GetController(HIDDENPOINT_CONTROLLER) == NULL)
			{
				(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::DCP06HiddenPointControllerC(m_pDCP06Model));
			}
			(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel(pModel/* m_pDlg->GetDataModel()*/);
			SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		// **********************************************************************************************************
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			// create model
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;
			
			sprintf(pModel->m_pPointBuff[0].point_id,"%s", m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].point_id);


			if(GetController(XYZ_CONTROLLER) == NULL)
			{
				(void)AddController( XYZ_CONTROLLER, new DCP::DCP06XYZControllerC( m_pDCP06Model));
			}
			(void)GetController( XYZ_CONTROLLER )->SetModel( pModel);
			SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			// create model
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;

			if(GetController(CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::DCP06CircleControllerC(m_pDCP06Model));
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel );
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == SEPARATE_REC)
		{
			// create model
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;

			if(GetController(SEPARATE_RECORDING_CONTROLLER) == NULL)
			{
				(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::DCP06SeparateRecControllerC(m_pDCP06Model));
			}
			(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel(pModel);
			SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			// create model
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;

			if(GetController(HOME_POINTS_CONTROLLER) == NULL)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::DCP06HomePointsControllerC(m_pDCP06Model) );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel(m_pDCP06Model/* m_pDlg->GetDataModel()*/);
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}
		else if(lExitCode == MID_POINT)
		{
			DCP::DCP06PointBuffModelC* pModel = new DCP06PointBuffModelC;

			sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", "");

			//pModel->m_pPointBuff[0].xsta = pMeasModel->
			if(GetController(MID_POINT_CONTROLLER) == NULL)
			{
				(void)AddController( MID_POINT_CONTROLLER, new DCP::DCP06MidPointControllerC(m_pDCP06Model));
			}
			(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
			SetActiveController(MID_POINT_CONTROLLER, true);
		}
	}

	// HIDDENPOINT
	if(lCtrlID == HIDDENPOINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06PointBuffModelC* pModel = (DCP::DCP06PointBuffModelC*) GetController( HIDDENPOINT_CONTROLLER )->GetModel();		

		m_pDlg->update_meas_values(pModel->m_pPointBuff[0].x,pModel->m_pPointBuff[0].y,pModel->m_pPointBuff[0].z, POINT_MEASURED);
	}

	// X or Y or Z
	if(lCtrlID == XYZ_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06PointBuffModelC* pModel = (DCP::DCP06PointBuffModelC*) GetController( XYZ_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].xsta)
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;

		if(pModel->m_pPointBuff[0].ysta)
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;

		if(pModel->m_pPointBuff[0].zsta)
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;

		//m_pDlg->update_meas_values(pModel->m_pPointBuff[0].x,pModel->m_pPointBuff[0].y,pModel->m_pPointBuff[0].z, POINT_MEASURED);
	}
	
	// CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06PointBuffModelC* pModel = (DCP::DCP06PointBuffModelC*) GetController( CIRCLE_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta = 1;
		}
	}

	// SEPARATE
	if(lCtrlID == SEPARATE_RECORDING_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		// FIXED 01042011
		DCP::DCP06PointBuffModelC* pModel = (DCP::DCP06PointBuffModelC*) GetController( SEPARATE_RECORDING_CONTROLLER )->GetModel();		
		
		//if(pModel->m_pPointBuff[0].sta != 0)
		if(pModel->m_pPointBuff[0].xsta !=0 && pModel->m_pPointBuff[0].ysta !=0 && pModel->m_pPointBuff[0].zsta !=0)
		{
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta = 1;
		}
	}
	if(lCtrlID == MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06PointBuffModelC* pModel = (DCP::DCP06PointBuffModelC*) GetController( MID_POINT_CONTROLLER )->GetModel();	
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->point_table[m_pDlg->GetDataModel()->m_iCurrentPoint-1].sta = 1;
		}
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

void DCP::DCP06MeasControllerC::OnControllerActivated(void)
{
//	ActivateMeasurement();
}
void DCP::DCP06MeasControllerC::OnControllerClosed(int lExitCode)
{
//	DeactivateMeasurement(lExitCode);
}

// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06MeasModelC::DCP06MeasModelC()
{
	m_iMinPoint = 0;
	m_iMaxPoint = 0;
	m_iPointsCount = 0;
	m_iCurrentPoint = 1;
	default_pid[0] = '\0';
	memset(&point_table[0],0,sizeof(S_POINT_BUFF)* 20);
	memset(&point_table2[0],0,sizeof(S_POINT_BUFF)* 20);

	disable_point_editing = false;
}
DCP::DCP06MeasModelC::~DCP06MeasModelC()
{
}
