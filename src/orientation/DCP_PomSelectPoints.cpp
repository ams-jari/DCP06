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
#include <dcp06/orientation/DCP_PomSelectPoints.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_SeparateRec.hpp>
#include <dcp06/measurement/DCP_HomePoints.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_SelectPoint.hpp>

#include <dcp06/file/DCP_SelectFile.hpp>
#include <dcp06/file/DCP_AdfFileFunc.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_SelectMultiPoints.hpp>
#include <GUI_Types.hpp>
#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>
#include <UTL_StringFunctions.hpp>
#include <GUI_DeskTop.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP05PomSelectPointsDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP05PomSelectPointsDlgC::DCP05PomSelectPointsDlgC(DCP::DCP05ModelC* pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pDCP05Model(pDCP05Model),
	m_pPointIdObserver(OBS_METHOD_TO_PARAM0(DCP05PomSelectPointsDlgC, OnPointIdChanged), this),
	m_pXObserver(OBS_METHOD_TO_PARAM0(DCP05PomSelectPointsDlgC, OnPointIdChanged), this),
	m_pYObserver(OBS_METHOD_TO_PARAM0(DCP05PomSelectPointsDlgC, OnPointIdChanged), this),
	m_pZObserver(OBS_METHOD_TO_PARAM0(DCP05PomSelectPointsDlgC, OnPointIdChanged), this),
	m_pCommon(0)
{
	//SetTxtApplicationId(AT_DCP05);
	m_pCommon = new DCP05CommonC(pDCP05Model);
}


            // Description: Destructor
DCP::DCP05PomSelectPointsDlgC::~DCP05PomSelectPointsDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP05PomSelectPointsDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	if(GetDataModel()->iDisplay == CHST_DLG)
		SetTitle(StringC(AT_DCP05,T_DCP_CHANGE_STATION_POS1_POINTS_TOK));

	// Add fields to dialog
	m_pPointNo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointNo->SetId(ePointNo);
	m_pPointNo->SetText(StringC(AT_DCP05,P_DCP_POINT_NO_TOK));
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pPointNo->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pPointNo);

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP05,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
//	m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE

	AddCtrl(m_pPointId);

	//InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP05,P_DCP_X_TOK));
	//m_pX->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pX->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP05Model->m_nDecimals);
	m_pX->SetEmptyAllowed(true);
	//m_pX->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP05,P_DCP_Y_TOK));
	//m_pY->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pY->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP05Model->m_nDecimals);
	m_pY->SetEmptyAllowed(true);
	//m_pY->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP05,P_DCP_Z_TOK));
	//m_pZ->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pZ->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP05Model->m_nDecimals);
	m_pZ->SetEmptyAllowed(true);
	//m_pZ->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pZ);

	m_pPointIdObserver.Attach(m_pPointId->GetSubject());
	m_pXObserver.Attach(m_pX->GetSubject());
	m_pYObserver.Attach(m_pY->GetSubject());
	m_pZObserver.Attach(m_pZ->GetSubject());

	// help
	//SetHelpTok(H_DCP_POM_POINTS_TOK,0);
}

void DCP::DCP05PomSelectPointsDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP05PomSelectPointsDlgC::UpdateData()
{

}

void DCP::DCP05PomSelectPointsDlgC::SetFocusToPoint()
{
	if(!GetDataModel()->disable_point_editing)
		m_pPointId->SetFocus();
	else
		m_pX->SetFocus();
}

// Description: refresh all controls
void DCP::DCP05PomSelectPointsDlgC::RefreshControls()
{
		
	if(m_pPointNo && m_pPointId && m_pX && m_pY && m_pZ )
	{
		//GetDataModel()->m_iPointsCount = m_pCommon->get_OCS_points_count(&GetDataModel()->points[0], MAX_POM_POINTS);
		if(GetDataModel()->iCurrentPoint < 1) GetDataModel()->iCurrentPoint  = 1;
		if(GetDataModel()->iCurrentPoint < 1) GetDataModel()->iCurrentPoint  = GetDataModel()->iMaxPoint;
		if(GetDataModel()->m_iPointsCount < GetDataModel()->iMinPoint) GetDataModel()->m_iPointsCount = GetDataModel()->iMinPoint;
		StringC sTemp;
		sTemp.Format(L"%d/%d",GetDataModel()->iCurrentPoint, GetDataModel()->m_iPointsCount);
		m_pPointNo->GetStringInputCtrl()->SetString(sTemp);

		sTemp = GetDataModel()->points[GetDataModel()->iCurrentPoint-1].point_id;
		sTemp.RTrim();
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

		if(GetDataModel()->points[GetDataModel()->iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
			m_pX->GetFloatInputCtrl()->SetDouble(GetDataModel()->points[GetDataModel()->iCurrentPoint-1].x);
			m_pY->GetFloatInputCtrl()->SetDouble(GetDataModel()->points[GetDataModel()->iCurrentPoint-1].y);
			m_pZ->GetFloatInputCtrl()->SetDouble(GetDataModel()->points[GetDataModel()->iCurrentPoint-1].z);

		}
		else
		{
			m_pX->GetFloatInputCtrl()->SetEmpty();
			m_pY->GetFloatInputCtrl()->SetEmpty();
			m_pZ->GetFloatInputCtrl()->SetEmpty();

		}
	}
}

void DCP::DCP05PomSelectPointsDlgC::OnPointIdChanged( int unNotifyCode,  int ulParam2)

 
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		short iCurrentPno = GetDataModel()->iCurrentPoint;
		//DCP05CommonC common;
		if(ulParam2 == ePointId)
		{
			StringC sPid;
			if(m_pPointId->GetStringInputCtrl()->IsEmpty())
				sprintf(GetDataModel()->points[iCurrentPno-1].point_id,"%-6.6s","");	
			else
			{
				sPid = m_pPointId->GetStringInputCtrl()->GetString();
				// convert to ascii
				char temp[20];
				//UTL::UnicodeToAscii(temp, sPid);
				BSS::UTI::BSS_UTI_WCharToAscii( sPid, temp );
				m_pCommon->strbtrim(temp);
				sprintf(GetDataModel()->points[iCurrentPno-1].point_id,"%-6.6s",temp);

			}
				
		}
		else
		{
			if (ulParam2 == eX)
			{
				if(!m_pX->GetFloatInputCtrl()->IsEmpty())
					GetDataModel()->points[iCurrentPno-1].x = m_pX->GetFloatInputCtrl()->GetDouble();
			}
			else if (ulParam2 == eY)
			{
				if(!m_pY->GetFloatInputCtrl()->IsEmpty())
					GetDataModel()->points[iCurrentPno-1].y = m_pY->GetFloatInputCtrl()->GetDouble();

			}
			else if(ulParam2 == eZ)
			{
				if(!m_pZ->GetFloatInputCtrl()->IsEmpty())
					GetDataModel()->points[iCurrentPno-1].z = m_pZ->GetFloatInputCtrl()->GetDouble();

			}
			if(!m_pX->GetFloatInputCtrl()->IsEmpty() && !m_pY->GetFloatInputCtrl()->IsEmpty() && !m_pZ->GetFloatInputCtrl()->IsEmpty())
			{
				GetDataModel()->points[iCurrentPno-1].sta = POINT_DESIGN;
				RefreshControls();
			}
		}
		/*
		StringC sPoint;
		sPoint = m_pPointId->GetStringInputCtrl()->GetString();
		char cPoint[10];
		UTL::UnicodeToAscii(cPoint,DCP_POINT_ID_LENGTH,sPoint);
		sprintf(GetDataModel()->point_table[GetDataModel()->m_iCurrentPoint-1].point_id,"%-6s",cPoint);
		RefreshControls();
		*/
	}
}

// Description: only accept hello world Model objects
bool DCP::DCP05PomSelectPointsDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05PomSelectPointsModelC* pDCP05Model = dynamic_cast< DCP::DCP05PomSelectPointsModelC* >( pModel );

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

// Description: Hello World model
DCP::DCP05PomSelectPointsModelC* DCP::DCP05PomSelectPointsDlgC::GetDataModel() const
{
    return (DCP::DCP05PomSelectPointsModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP05PomSelectPointsDlgC::PointNext()
{
	DCP05MsgBoxC MsgBox;
	MsgBox.ShowMessageOk(L"Next point");
}

// ================================================================================================
void DCP::DCP05PomSelectPointsDlgC::PointPrev()
{
	DCP05MsgBoxC MsgBox;
	MsgBox.ShowMessageOk(L"Prev point");
}

// ================================================================================================
void DCP::DCP05PomSelectPointsDlgC::SelectPoint(short iId)
{
	DCP05MsgBoxC MsgBox;
	MsgBox.ShowMessageOk(L"Prev point");
}

// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05PomSelectPointsControllerC::DCP05PomSelectPointsControllerC(DCP::DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_POM_POINTS_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// Create a dialog
    m_pDlg = new DCP::DCP05PomSelectPointsDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( POM_POINT_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_PICK_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_NEXT_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_PREV_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_ID_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_ADD_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05PomSelectPointsControllerC::~DCP05PomSelectPointsControllerC()
{
}
// Description: Route model to everybody else
bool DCP::DCP05PomSelectPointsControllerC::SetModel( GUI::ModelC* pModel )
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

// PICK
void DCP::DCP05PomSelectPointsControllerC::OnF1Pressed()
{
	DCP05CommonC common(m_pDCP05Model);
	DCP05MsgBoxC msgbox;
	short last=0;
	short pcount = m_pDlg->GetDataModel()->iMaxPoint;

	if(common.get_OCS_points_count(&m_pDlg->GetDataModel()->points[0],m_pDlg->GetDataModel()->iMaxPoint)>0)
	{
		StringC msg;
		short iCur = m_pDlg->GetDataModel()->iCurrentPoint;
		StringC strActivePoint =m_pDlg->GetDataModel()->points[iCur].point_id;
	
		msg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINTS_TOK);
		msg.Format(msg,L"");
		if(msgbox.ShowMessageYesNo(msg))
		{
			memset(&m_pDlg->GetDataModel()->points[0],0,sizeof(S_POINT_BUFF) * m_pDlg->GetDataModel()->iMaxPoint); 
			memset(&m_pDlg->GetDataModel()->points1[0],0,sizeof(S_POINT_BUFF) * m_pDlg->GetDataModel()->iMaxPoint); 
			last = 0;
		}
		else
		{	
			last = common.get_last_defined_point(&m_pDlg->GetDataModel()->points[0],&m_pDlg->GetDataModel()->points1[0],
					m_pDlg->GetDataModel()->iMaxPoint);
			
			pcount = m_pDlg->GetDataModel()->iMaxPoint-last;
		}
	}

		// SELECT FILE		
		DCP::DCP05SelectFileModelC* pModel = new DCP05SelectFileModelC;
		if(GetController(SELECT_FILE_CONTROLLER) == NULL)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP05SelectFileControllerC(ADF_BF_STA/*ONLY_ADF*/, sTitle,m_pDCP05Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);

}
// NEXT
void DCP::DCP05PomSelectPointsControllerC::OnF2Pressed()
{

	if(m_pDlg->GetDataModel()->iCurrentPoint < m_pDlg->GetDataModel()->iMaxPoint &&
		m_pDlg->GetDataModel()->iCurrentPoint < m_pDlg->GetDataModel()->m_iPointsCount)
		m_pDlg->GetDataModel()->iCurrentPoint++;
	else
	{
	 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP05,I_DCP_LAST_POINT_TOK));
	}
	
	m_pDlg->RefreshControls();
	m_pDlg->SetFocusToPoint();

}

// PREV
void DCP::DCP05PomSelectPointsControllerC::OnF3Pressed()
{
	if(m_pDlg->GetDataModel()->iCurrentPoint > 1)
	m_pDlg->GetDataModel()->iCurrentPoint--;
	
	m_pDlg->RefreshControls();
	m_pDlg->SetFocusToPoint();

}

// PID
void DCP::DCP05PomSelectPointsControllerC::OnF4Pressed()
{
	
	if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		DCP::DCP05SelectPointModelC* pModel = new DCP05SelectPointModelC;
		pModel->m_iCounts = m_pDlg->GetDataModel()->m_iPointsCount;
		pModel->m_iSelectedId = m_pDlg->GetDataModel()->iCurrentPoint;
		for(int i=0; i < m_pDlg->GetDataModel()->m_iPointsCount; i++)
		{
			pModel->points[i].no = i+1;
			sprintf(pModel->points[i].point_id,"%s", m_pDlg->GetDataModel()->points[i].point_id);
			
			if(m_pDlg->GetDataModel()->points[i].sta)
				pModel->points[i].point_status[0] = '+';
			else
				pModel->points[i].point_status[0] = '-';
		}

		if(GetController(SELECT_POINT_CONTROLLER) == NULL)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP05SelectPointControllerC );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
	
	/*
	unsigned int ulControllerId = ABL::CTL_ID_ASCII_INPUT;
	GUI::ControllerC* poController = 
		ABL::ControllerFactoryContainerC::Instance()->CreateController((unsigned short) ulControllerId); 
	AddController((int)ulControllerId,poController); 
	(void)SetActiveController( ulControllerId, true );
	*/
}

// ADD
void DCP::DCP05PomSelectPointsControllerC::OnF5Pressed()
{
	// CAPTIVATE TO BE CHEKED
	if(m_pDlg->GetDataModel()->m_iPointsCount < m_pDlg->GetDataModel()->iMaxPoint)
	{	
		DCP05CommonC* m_pCommon = new DCP05CommonC(m_pDCP05Model);

		// 271011
		char temp[10];
		sprintf(temp,"%-s",m_pDlg->GetDataModel()->points[m_pDlg->GetDataModel()->m_iPointsCount-1].point_id);

		if(!m_pCommon->strblank(temp))
		{
			m_pCommon->inc_id(temp);
		}
		else
		{
			if(!m_pCommon->strblank(m_pDlg->GetDataModel()->default_pid))
			{
				sprintf(temp,"%s%d",m_pDlg->GetDataModel()->default_pid,m_pDlg->GetDataModel()->m_iPointsCount+1);
			}
		}
		delete m_pCommon;
		m_pCommon = 0;
		m_pDlg->GetDataModel()->m_iPointsCount++;
		m_pDlg->GetDataModel()->iCurrentPoint = m_pDlg->GetDataModel()->m_iPointsCount;

		sprintf(m_pDlg->GetDataModel()->points[m_pDlg->GetDataModel()->m_iPointsCount-1].point_id,"%-s",temp);

		//m_pDlg->GetDataModel()->m_iPointsCount++;
		//m_pDlg->GetDataModel()->iCurrentPoint = m_pDlg->GetDataModel()->m_iPointsCount;
	}
	else
	{
		DCP05MsgBoxC msgbox;
		StringC msg;
		msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_ADD_POINT_TOK);
		msgbox.ShowMessageOk(msg);
	}
	m_pDlg->RefreshControls();
	m_pDlg->SetFocusToPoint();
}
// Description: Handle change of position values
void DCP::DCP05PomSelectPointsControllerC::OnF6Pressed()
{
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
}
// Description: Handle change of position values
void DCP::DCP05PomSelectPointsControllerC::OnSHF2Pressed()
{
	DCP05MsgBoxC msgbox;
	StringC msg;
	short iCur = m_pDlg->GetDataModel()->iCurrentPoint;
	StringC strActivePoint =m_pDlg->GetDataModel()->points[iCur].point_id;
	
	msg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)strActivePoint);
	if(msgbox.ShowMessageYesNo(msg))
	{
		int MINPOINTS =  m_pDlg->GetDataModel()->iMinPoint;
		int MAXPOINTS =  m_pDlg->GetDataModel()->iMaxPoint;

		for(int i=iCur-1; i < m_pDlg->GetDataModel()->iMaxPoint-1; i++)
		{
			memcpy(&m_pDlg->GetDataModel()->points[i],&m_pDlg->GetDataModel()->points[i+1],sizeof(S_POINT_BUFF));
			memcpy(&m_pDlg->GetDataModel()->points1[i],&m_pDlg->GetDataModel()->points1[i+1],sizeof(S_POINT_BUFF));
			/*
			strcpy(points[i].point_id,points[i+1].point_id);
			points[i].x     =points[i+1].x;
			points[i].y     =points[i+1].y;
			points[i].z     =points[i+1].z;
			points[i].xdes  =points[i+1].xdes;
			points[i].ydes  =points[i+1].ydes;
			points[i].zdes  =points[i+1].zdes;
			points[i].sta   =points[i+1].sta;
			points[i].dsta  =points[i+1].dsta;
			
			strcpy(points2[i].point_id,points2[i+1].point_id);
			points2[i].x    =points2[i+1].x;
			points2[i].y    =points2[i+1].y;
			points2[i].z    =points2[i+1].z;
			points2[i].xdes =points2[i+1].xdes;
			points2[i].ydes =points2[i+1].ydes;
			points2[i].zdes =points2[i+1].zdes;
			points2[i].sta  =points2[i+1].sta;
			points2[i].dsta =points2[i+1].dsta;
			*/
		}
		DCP05CommonC common(m_pDCP05Model);
		common.delete_point(&m_pDlg->GetDataModel()->points[m_pDlg->GetDataModel()->iMaxPoint-1]);
		common.delete_point(&m_pDlg->GetDataModel()->points1[m_pDlg->GetDataModel()->iMaxPoint-1]);		
		m_pDlg->GetDataModel()->m_iPointsCount--;
		
		if(m_pDlg->GetDataModel()->m_iPointsCount < MINPOINTS) 
				m_pDlg->GetDataModel()->m_iPointsCount = MINPOINTS;
		if(m_pDlg->GetDataModel()->iCurrentPoint > m_pDlg->GetDataModel()->m_iPointsCount) 
				m_pDlg->GetDataModel()->iCurrentPoint = m_pDlg->GetDataModel()->m_iPointsCount;
		
		if(m_pDlg->GetDataModel()->iCurrentPoint < 1 )m_pDlg->GetDataModel()->iCurrentPoint = 1;
		
		
		/*
							delete_point(&points[MAXPOINTS-1]);
							points[MAXPOINTS-1].x = UNDEFVAL;
							points[MAXPOINTS-1].y = UNDEFVAL;
							points[MAXPOINTS-1].z = UNDEFVAL;

							delete_point(&points2[MAXPOINTS-1]);
							points2[MAXPOINTS-1].x = UNDEFVAL;
							points2[MAXPOINTS-1].y = UNDEFVAL;
							points2[MAXPOINTS-1].z = UNDEFVAL;

							COUNTS--;

							if(COUNTS < MINPOINTS) COUNTS = MINPOINTS;
							if(ACTIVE_PNO > COUNTS) ACTIVE_PNO = COUNTS;
							if(ACTIVE_PNO < 1 ) ACTIVE_PNO = 1;
			*/
		m_pDlg->RefreshControls();			
	}
}

void DCP::DCP05PomSelectPointsControllerC::save_current_point()
{
	
}

// Description: React on close of tabbed dialog
void DCP::DCP05PomSelectPointsControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05PomSelectPointsControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectPointModelC* pModel = (DCP::DCP05SelectPointModelC*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		//StringC strSelectedPoint = pModel->m_strSelectedPoint;
		m_pDlg->GetDataModel()->iCurrentPoint = pModel->m_iSelectedId;
		
	}

	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectFileModelC* pModel = (DCP::DCP05SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		//m_pDCP05PomDlg->SelectFile(strSelectedFile);
		// select points from file
		//char filename_temp[20];
		//UTL::UnicodeToAscii(filename_temp, strSelectedFile);

		// create
		AdfFileFunc adf(m_pDCP05Model);
		adf.always_single = 1;
		DCP05CommonC common(m_pDCP05Model);
		short def;

		if(m_pDlg->GetDataModel()->iDisplay == POM_DLG)
			def = DESIGN;
		else
			def = ACTUAL;

		if(adf.setFile(strSelectedFile)== true)
		{
			//if(iCount)
			{
				// SELECT MULTIPOINTS
				DCP::DCP05SelectMultiPointsModelC* pModel = new DCP05SelectMultiPointsModelC;
				//S_SELECT_POINTS pTempList[MAX_SELECT_POINTS];
				//memset(&pTempList[0], 0,sizeof(S_SELECT_POINTS) *MAX_SELECT_POINTS);
				int iCount = adf.GetPointList(&pModel->sel_points[0],MAX_SELECT_POINTS, def);
				pModel->m_iPointsCount = iCount;
				pModel->m_iDef = def;
				pModel->sSelectedFile = strSelectedFile;
				pModel->m_iMinPoint = m_pDlg->GetDataModel()->iMinPoint;
				pModel->m_iMaxPoint = MAX_POM_POINTS;
				
				pModel->helpToken = H_DCP_SEL_MULTI_A_OR_D_TOK;
				
				// info  
				StringC sTemp;
				sTemp.LoadTxt(AT_DCP05, T_DCP_SELECT_POINTS_TOK);
				wchar_t cTemp[80];
				swprintf(cTemp,L"%s (%d-%d) %s",(const wchar_t*)StringC(sTemp),pModel->m_iMinPoint,pModel->m_iMaxPoint,(const wchar_t*)StringC(pModel->sSelectedFile));
				pModel->sInfo = StringC(cTemp);
			
				// set title
				pModel->sTitle = GetTitle();

				if(GetController(SELECT_MULTIPOINTS_CONTROLLER) == NULL)
				{
					(void)AddController( SELECT_MULTIPOINTS_CONTROLLER, new DCP::DCP05SelectMultiPointsControllerC(m_pDCP05Model) );
				}

				//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_PLANE_MEAS_TOK);

				(void)GetController( SELECT_MULTIPOINTS_CONTROLLER )->SetModel(pModel);
				SetActiveController(SELECT_MULTIPOINTS_CONTROLLER, true);
					
					/*
					*/
				}	
		} 
		else
		{
			DCP05MsgBoxC msgbox;
			msgbox.ShowMessageOk(L"Set file error!");
		}
	}  
	  
	if(lCtrlID == SELECT_MULTIPOINTS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectMultiPointsModelC* pModel = (DCP::DCP05SelectMultiPointsModelC*) GetController( SELECT_MULTIPOINTS_CONTROLLER )->GetModel();		
		AdfFileFunc adf(m_pDCP05Model);
		adf.always_single = 1;
		DCP05CommonC common(m_pDCP05Model);
		
		if(adf.setFile(pModel->sSelectedFile))
		{
			short  last = common.get_last_defined_point(&m_pDlg->GetDataModel()->points[0],&m_pDlg->GetDataModel()->points1[0],
					m_pDlg->GetDataModel()->iMaxPoint);
			short cc = last;
			short /*DCP::DCP_POINT_STATUS*/  sdes,smea;
			// copy selected points inti 
			for(short i=0;i<pModel->m_iMaxPoint/*pcount*/ /*MAXPOINTS*/;i++)
			{
				char bXmea[15], bYmea[15], bZmea[15];
				char bXdes[15], bYdes[15], bZdes[15],pid[7];//,fname[13];
				
				if(pModel->nro_table[i][0] != 0)
				{
					cc++; 
					//form_select_pnt1(&fs,(int) nro_table[i][0], pid, NULL, bXmea, bXdes, NULL, bYmea, bYdes, NULL, bZmea, bZdes, NULL);
					adf.form_pnt1((int) pModel->nro_table[i][0], pid, NULL, bXmea, bXdes, NULL, bYmea, bYdes, NULL, bZmea, bZdes, NULL);
					sprintf(m_pDlg->GetDataModel()->points[cc-1].point_id,"%-6.6s",pid);
					m_pDlg->GetDataModel()->points[cc-1].x = (pModel->nro_table[i][1] == DESIGN)? atof(bXdes) : atof(bXmea);
					m_pDlg->GetDataModel()->points[cc-1].y = (pModel->nro_table[i][1] == DESIGN)? atof(bYdes) : atof(bYmea);
					m_pDlg->GetDataModel()->points[cc-1].z = (pModel->nro_table[i][1] == DESIGN)? atof(bZdes) : atof(bZmea);

					sdes= (!common.strblank(bXdes) && !common.strblank(bYdes) && !common.strblank(bZdes)) ? POINT_DESIGN: POINT_NOT_DEFINED;
					smea= (!common.strblank(bXmea) && !common.strblank(bYmea) && !common.strblank(bZmea)) ? POINT_DESIGN: POINT_NOT_DEFINED;

					m_pDlg->GetDataModel()->points[cc-1].sta=(pModel->nro_table[i][1] == DESIGN)? sdes :smea;
				}
			} 
			m_pDlg->GetDataModel()->m_iPointsCount = cc;
		}
	}

	/*
	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		if(lExitCode == HIDDEN_POINT)
		{
			if(GetController(HIDDENPOINT_CONTROLLER) == NULL)
			{
				(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::DCP05HiddenPointControllerC );
			}
			(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel( m_pDCP05MeasDlg->GetDCP05Model());
			SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			if(GetController(XYZ_CONTROLLER) == NULL)
			{
				(void)AddController( XYZ_CONTROLLER, new DCP::DCP05XYZControllerC );
			}
			(void)GetController( XYZ_CONTROLLER )->SetModel( m_pDCP05MeasDlg->GetDCP05Model());
			SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			if(GetController(CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::DCP05CircleControllerC );
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel( m_pDCP05MeasDlg->GetDCP05Model());
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == SEPARATE_REC)
		{
			if(GetController(SEPARATE_RECORDING_CONTROLLER) == NULL)
			{
				(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::DCP05SeparateRecControllerC );
			}
			(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel( m_pDCP05MeasDlg->GetDCP05Model());
			SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			if(GetController(HOME_POINTS_CONTROLLER) == NULL)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::DCP05HomePointsControllerC );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel( m_pDCP05MeasDlg->GetDCP05Model());
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}


	}
	*/
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// POMModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP05PomSelectPointsModelC::DCP05PomSelectPointsModelC()
{
	memset(&points[0],0, sizeof(S_POINT_BUFF)* MAX_SELECT_POINTS);
	memset(&points1[0],0, sizeof(S_POINT_BUFF)* MAX_SELECT_POINTS);
	iMaxPoint = 20;
	iMinPoint = 2;
			//(short iDisplay;
	iCurrentPoint =1;
	m_iPointsCount =3;
	default_pid[0] = '\0';
	disable_point_editing = false;
}


DCP::DCP05PomSelectPointsModelC::~DCP05PomSelectPointsModelC()
{
}



