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
#include <dcp06/core/DCP_DCP05Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_HiddenPointConf.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/measurement/DCP_MeasXYZ.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/measurement/DCP_MeasDist.hpp>
#include "calc.h"

#include <GUI_DeskTop.hpp>
#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP05InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP05HiddenPointDlgC::DCP05HiddenPointDlgC(DCP::DCP05ModelC *pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointDist(0),m_pX(0),m_pY(0),m_pZ(0),m_pDCP05Model(pDCP05Model),m_iCurrentPoint(1),
	/*m_pInfo(0),*/iInfoInd(0)
{
	//SetTxtApplicationId(AT_DCP05);
	
	strInfoText.LoadTxt(AT_DCP05,P_DCP_INFO_TOK);

	memset(&measured_points[0],0,sizeof(S_POINT_BUFF)*MAX_POINTS_IN_HIDDENPOINT_BAR);

	m_pDCP05Model->active_tool = 0;
}


            // Description: Destructor
DCP::DCP05HiddenPointDlgC::~DCP05HiddenPointDlgC()
{
	//m_pTimer.KillTimer();

	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP05HiddenPointDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	m_pPointNo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointNo->SetId(ePointNo);
	m_pPointNo->SetText(StringC(AT_DCP05,P_DCP_POINT_NO_TOK));
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pPointNo);

	m_pPointDist = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pPointDist->SetId(ePointDist);
	m_pPointDist->SetText(StringC(AT_DCP05,P_DCP_POINT_DIST_TOK));
	m_pPointDist->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointDist->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pPointDist->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pDCP05Model->m_nDecimals);
	m_pPointDist->SetEmptyAllowed(true);
	AddCtrl(m_pPointDist);

	//InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP05,P_DCP_X_TOK));
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pX->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pDCP05Model->m_nDecimals);
	m_pX->SetEmptyAllowed(true);
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP05,P_DCP_Y_TOK));
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pDCP05Model->m_nDecimals);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pY->SetEmptyAllowed(true);
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP05,P_DCP_Z_TOK));
	m_pZ->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pDCP05Model->m_nDecimals);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZ->SetEmptyAllowed(true);
	AddCtrl(m_pZ);
	
	//InsertEmptyLine(); CAPTIVATE
	//InsertEmptyLine(); CAPTIVATE

	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	//SetHelpTok(H_DCP_HIDDENPOINT_TOK,0);
}

//void DCP::DCP05HiddenPointDlgC::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}
void DCP::DCP05HiddenPointDlgC::OnDialogActivated()
{
	m_pCommon = new DCP05CommonC(m_pDCP05Model);
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	
	RefreshControls();
}

void DCP::DCP05HiddenPointDlgC::UpdateData()
{

	//GetDCP05Model()->m_sUser1	= m_pUser1->GetStringInputCtrl()->GetString();
}

// Description: refresh all controls
void DCP::DCP05HiddenPointDlgC::RefreshControls()
{
	if(m_pPointNo && m_pPointDist && m_pX && m_pY && m_pZ)
	{
		StringC sTemp;

		sTemp.Format(L"%d/%d",m_iCurrentPoint, MAX_POINTS_IN_HIDDENPOINT_BAR);
		m_pPointNo->GetStringInputCtrl()->SetString(sTemp);

		if(m_pDCP05Model->hidden_point_bar[m_iCurrentPoint-1] != 0.0)
			m_pPointDist->GetFloatInputCtrl()->SetDouble(m_pDCP05Model->hidden_point_bar[m_iCurrentPoint-1]);
		else
			m_pPointDist->GetFloatInputCtrl()->SetEmpty();


		if(measured_points[m_iCurrentPoint-1].sta != POINT_NOT_DEFINED)
		{
			m_pX->GetFloatInputCtrl()->SetDouble(measured_points[m_iCurrentPoint-1].x);
			m_pY->GetFloatInputCtrl()->SetDouble(measured_points[m_iCurrentPoint-1].y);
			m_pZ->GetFloatInputCtrl()->SetDouble(measured_points[m_iCurrentPoint-1].z);
		}
		else
		{
			m_pX->GetFloatInputCtrl()->SetEmpty();
			m_pY->GetFloatInputCtrl()->SetEmpty();
			m_pZ->GetFloatInputCtrl()->SetEmpty();
		}
	}
}

// Description: only accept hello world Model objects
bool DCP::DCP05HiddenPointDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05PointBuffModelC* pDCP05Model = dynamic_cast< DCP::DCP05PointBuffModelC* >( pModel );

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
DCP::DCP05PointBuffModelC* DCP::DCP05HiddenPointDlgC::GetDataModel() const
{
    return (DCP::DCP05PointBuffModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP05HiddenPointDlgC::PointNext()
{
	if(m_iCurrentPoint == 1)
		m_iCurrentPoint = MAX_POINTS_IN_HIDDENPOINT_BAR;
	else if(m_iCurrentPoint < MAX_POINTS_IN_HIDDENPOINT_BAR)
		m_iCurrentPoint++;	


	RefreshControls();
}
// ================================================================================================
void DCP::DCP05HiddenPointDlgC::PointPrev()
{
	if(m_iCurrentPoint > 1)
		m_iCurrentPoint--;
	RefreshControls();
}

// ================================================================================================
void DCP::DCP05HiddenPointDlgC::PointDelete()
{
	DCP05MsgBoxC msgbox;
	StringC msg;
	StringC strActivePoint(L"");

	msg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)strActivePoint);
	if(msgbox.ShowMessageYesNo(msg))
	{
		memset(&measured_points[m_iCurrentPoint-1],0,sizeof(S_POINT_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP05HiddenPointDlgC::update_meas_values(double x, double y, double z, short /*/*DCP_POINT_STATUS*/ status)
{
	DCP05CommonC common(m_pDCP05Model);
	if(common.check_distance(x,y,z,measured_points,MAX_POINTS_IN_HIDDENPOINT_BAR, m_iCurrentPoint))
	{
		measured_points[m_iCurrentPoint-1].x = x;
		measured_points[m_iCurrentPoint-1].y = y;
		measured_points[m_iCurrentPoint-1].z = z;
		measured_points[m_iCurrentPoint-1].sta = status;
		RefreshControls();
	}
}
// ================================================================================================
short DCP::DCP05HiddenPointDlgC::calc_hidden_point(void)
{
short points_defined;
short i,count;
struct ams_vector a,b;
struct line c;
double p_mat[MAX_POINTS_IN_HIDDENPOINT_BAR*3];
double para, parb, parc;
double xtemp, ytemp, ztemp,tmp;

	DCP05MsgBoxC msgbox;

	/* how many points are defined */
	points_defined = 0;

	for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
	{
		if(measured_points[i].sta > 0)
			points_defined++;
	}

	if(points_defined < 2)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP05,M_DCP_IN_MIN_2_POINTS_TOK);
		msgbox.ShowMessageOk(msg);
		hiddenpoint[0].sta = POINT_NOT_DEFINED; // not calculated
		return false;
	}

	else  if(points_defined == 2)
	{
		count = 0;

		for(i=0;i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
		{
			if(measured_points[i].sta != 0)
			{
				count++;

				if(count == 1)
				{
					a.x = measured_points[i].x;
					a.y = measured_points[i].y;
					a.z = measured_points[i].z;
				}
				if(count == 2)
				{
					b.x = measured_points[i].x;
					b.y = measured_points[i].y;
					b.z = measured_points[i].z;
					break;
				}
			}
		}

		equation_of_line(&a, &b, &c);

		para = c.ux; parb = c.uy; parc = c.uz;

		//sprintf(Temp,"A=%.5f B=%.5f C=%.5f",para,parb,parc);
		//show_info(Temp);

		/**********************************
			calculate hidden point
		**********************************/

		xtemp = ytemp = ztemp = 0.0;

		for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
		{
			if(measured_points[i].sta != 0)
			{

				tmp = 	m_pDCP05Model->hidden_point_bar[i];
				xtemp = xtemp +  (measured_points[i].x - (tmp* para));
				ytemp = ytemp +  (measured_points[i].y - (tmp* parb));
				ztemp = ztemp +  (measured_points[i].z - (tmp* parc));
			}
		}
		hiddenpoint[0].x = xtemp / points_defined;
		hiddenpoint[0].y = ytemp / points_defined;
		hiddenpoint[0].z = ztemp / points_defined;
		hiddenpoint[0].sta = POINT_MEASURED;
	}
	else
	{
			count = 0;
			for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
			{
				if(measured_points[i].sta != 0)
				{
					p_mat[count*3 +0] = measured_points[i].x;
					p_mat[count*3 +1] = measured_points[i].y;
					p_mat[count*3 +2] = measured_points[i].z;

					count++;
				}
				if(count == points_defined)
					break;
			}

			//show_matrixn("p_mat",p_mat, points_defined, 3);

			if(FitLine(p_mat, points_defined, &para, &parb, &parc)== -1)
			{
				// show_info(CANNOT_CALCULATE_TEXT);

				hiddenpoint[0].sta = POINT_NOT_DEFINED; /* no calc */
				return false;
			}
			else
			{
				//sprintf(Temp,"A=%.5f B=%.5f C=%.5f",para,parb,parc);
				//show_info(Temp);

				/**********************************
				calculate hidden point
				**********************************/

				xtemp = ytemp = ztemp = 0.0;

				for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
				{
					if(measured_points[i].sta != 0)
					{
						tmp = m_pDCP05Model->hidden_point_bar[i];
						xtemp = xtemp +  measured_points[i].x - (tmp * para);
						ytemp = ytemp +  measured_points[i].y - (tmp * parb);
						ztemp = ztemp +  measured_points[i].z - (tmp * parc);
					}
				}
				hiddenpoint[0].x = xtemp / points_defined;
				hiddenpoint[0].y = ytemp / points_defined;
				hiddenpoint[0].z = ztemp / points_defined;
				hiddenpoint[0].sta = POINT_MEASURED;
			}
	}
	StringC msgTitle;
	StringC sX,sY,sZ;
	sX.LoadTxt(AT_DCP05,P_DCP_X_TOK);
	sY.LoadTxt(AT_DCP05,P_DCP_Y_TOK);
	sZ.LoadTxt(AT_DCP05,P_DCP_Z_TOK);
	
	char tempX[15];
	sprintf(tempX,"%9.*f", m_pDCP05Model->m_nDecimals,hiddenpoint[0].x); 

	char tempY[15];
	sprintf(tempY,"%9.*f", m_pDCP05Model->m_nDecimals,hiddenpoint[0].y); 

	char tempZ[15];
	sprintf(tempZ,"%9.*f", m_pDCP05Model->m_nDecimals,hiddenpoint[0].z); 

	msgTitle.LoadTxt(AT_DCP05,L_DCP_HIDDEN_POINT_TOK);
	msgTitle += L"\n";
	msgTitle += sX;
	msgTitle += StringC(tempX);
	msgTitle += L"\n";
    msgTitle += sY;
	msgTitle += StringC(tempY);
	msgTitle += L"\n";
    msgTitle += sZ;
	msgTitle += StringC(tempZ);
	msgTitle += L"\n";
    
	StringC msg;	
	if(msgbox.ShowMessageOkAbort(msgTitle, msg))
	{
		GetDataModel()->m_pPointBuff[0].x = hiddenpoint[0].x;
		GetDataModel()->m_pPointBuff[0].y = hiddenpoint[0].y;
		GetDataModel()->m_pPointBuff[0].z = hiddenpoint[0].z;
		GetDataModel()->m_pPointBuff[0].sta = POINT_MEASURED;
		return true;
	}
	else
	{
		return false;
	}
}

short  DCP::DCP05HiddenPointDlgC::get_current_point()
{
	return m_iCurrentPoint;
}


// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05HiddenPointControllerC::DCP05HiddenPointControllerC(DCP::DCP05ModelC *pDCP05Model)
    : m_pDlg( NULL ), m_pDCP05Model(pDCP05Model),m_pCommon(0),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_HIDDENPOINT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05HiddenPointDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( HIDDENPOINT_DLG, m_pDlg, true );


	m_pCommon = new DCP05CommonC(m_pDCP05Model);

	isATR = pDCP05Model->isATR;
    show_function_keys();
} //lint !e818 Pointer parameter could be declared as pointing to const

void DCP::DCP05HiddenPointControllerC::show_function_keys()
{
	// Set the function key
	
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

		vDef.strLable = StringC(AT_DCP05,K_DCP_CALC_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );
		
		// SHIFT
		/// CAPTIVATE
		//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
		//{
			vDef.strLable = StringC(AT_DCP05,K_DCP_CAMERA_TOK);
			SetFunctionKey( SHFK1, vDef );
		//}
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_CLEAR_TOK);
		SetFunctionKey( SHFK2, vDef );
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_CONFIG_TOK);
		SetFunctionKey( SHFK3, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_INIT_TOK);
		SetFunctionKey( SHFK4, vDef );

			// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP05;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::DCP05HiddenPointControllerC::~DCP05HiddenPointControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP05HiddenPointControllerC::SetModel( GUI::ModelC* pModel )
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

// ALL
void DCP::DCP05HiddenPointControllerC::OnF1Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(HIDDENPOINT_DLG);
		show_function_keys();
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

		sprintf(pModel->sPointId,"HP%d",m_pDlg->get_current_point());
		m_pCommon->strbtrim(pModel->sPointId);
		
		if(GetController(MEAS_XYZ_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::DCP05MeasXYZControllerC(m_pDCP05Model));
		}
		(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_XYZ_CONTROLLER, true);
	}
}

// ================================================================================================
// Description: F2
// ================================================================================================
void DCP::DCP05HiddenPointControllerC::OnF2Pressed()
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
// NEXT
void DCP::DCP05HiddenPointControllerC::OnF3Pressed()
{
	m_pDlg->PointNext();
}

// PREV
void DCP::DCP05HiddenPointControllerC::OnF4Pressed()
{
	m_pDlg->PointPrev();
}

// Description: Handle change of position values
void DCP::DCP05HiddenPointControllerC::OnF5Pressed()
{
	if(m_pDlg->calc_hidden_point())
		Close(EC_KEY_CONT);
}
// Description: Handle change of position values
void DCP::DCP05HiddenPointControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }


	if(!m_bCamera)
	{
		 m_pDlg->UpdateData();

		 if(m_pDlg->GetDataModel()->m_pPointBuff[0].sta != 0)
			(void)Close(EC_KEY_CONT);
		else
			(void)Close(EC_KEY_ESC);
	}
	else
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(HIDDENPOINT_DLG);
		show_function_keys();
	}

    // Update model
    // Set it to hello world dialog
   
}
void DCP::DCP05HiddenPointControllerC::OnSHF1Pressed()
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
// Delete
void DCP::DCP05HiddenPointControllerC::OnSHF2Pressed()
{
	m_pDlg->PointDelete();
}

// Config
void DCP::DCP05HiddenPointControllerC::OnSHF3Pressed()
{
	// create model
	DCP::DCP05HiddenPointBarConfModelC* pModel = new DCP05HiddenPointBarConfModelC;
	
	// set current values
	memcpy(pModel->hidden_point_bar,m_pDCP05Model->hidden_point_bar, sizeof(double) *MAX_POINTS_IN_HIDDENPOINT_BAR);

	if(GetController(HIDDENPOINT_CONF_CONTROLLER) == NULL)
	{
		(void)AddController( HIDDENPOINT_CONF_CONTROLLER, new DCP::DCP05HiddenPointConfControllerC(m_pDCP05Model));
	}
	(void)GetController( HIDDENPOINT_CONF_CONTROLLER )->SetModel(pModel);
	SetActiveController(HIDDENPOINT_CONF_CONTROLLER, true);

}

void DCP::DCP05HiddenPointControllerC::OnSHF4Pressed()
{
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP05InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP05Model);
	SetActiveController(INIT_CONTROLLER, true);
}


// Description: React on close of tabbed dialog
void DCP::DCP05HiddenPointControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(HIDDENPOINT_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}


// Description: React on close of controller
void DCP::DCP05HiddenPointControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
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
			// get values
			DCP::DCP05MeasXYZModelC* pModel = (DCP::DCP05MeasXYZModelC*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;
			m_pDlg->update_meas_values(x,y,z, POINT_MEASURED);
		}
	}

	// HIDDEN POINT BAR CONF
	if(lCtrlID == HIDDENPOINT_CONF_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		// get values
		DCP::DCP05HiddenPointBarConfModelC* pModel = (DCP::DCP05HiddenPointBarConfModelC*) GetController( HIDDENPOINT_CONF_CONTROLLER )->GetModel();		
		memcpy(m_pDCP05Model->hidden_point_bar, pModel->hidden_point_bar, sizeof(double) *HIDDENPOINT_CONF_CONTROLLER);
	}


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


DCP::DCP05HiddenPointDlgBaseC::DCP05HiddenPointDlgBaseC(int iCtrlId,DCP::DCP05ModelC* pDCP50Model ,DCP::DCP05PointBuffModelC* pDCPPointBuffModel)
{
	if(GetController(iCtrlId) == NULL)
	{
		(void)AddController( iCtrlId, new DCP::DCP05HiddenPointControllerC(pDCP50Model));
	}
	(void)GetController( iCtrlId )->SetModel(pDCPPointBuffModel/* m_pDCP05MeasDlg->GetDCP05Model()*/);
	SetActiveController(iCtrlId, true);
}