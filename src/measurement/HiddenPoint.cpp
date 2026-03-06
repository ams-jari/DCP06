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
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/measurement/HiddenPointConf.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/MeasDist.hpp>
#include "calc.h"
#include "core/geometry/LineFitting.h"
#include "geometry/Point.h"
#include "geometry/Line.h"
#include <vector>

#include <GUI_DeskTop.hpp>
#include <GUI_Types.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::InitializationDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::HiddenPointDialog::HiddenPointDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointNo(0),m_pPointDist(0),m_pX(0),m_pY(0),m_pZ(0),m_pModel(pModel),m_iCurrentPoint(1),
	/*m_pInfo(0),*/iInfoInd(0)
{
	//SetTxtApplicationId(AT_DCP06);
	
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);

	memset(&measured_points[0],0,sizeof(S_POINT_BUFF)*MAX_POINTS_IN_HIDDENPOINT_BAR);

	m_pModel->active_tool = 0;
}


            // Description: Destructor
DCP::HiddenPointDialog::~HiddenPointDialog()
{
	//m_pTimer.KillTimer();

	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::HiddenPointDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	m_pPointNo = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointNo->SetId(ePointNo);
	m_pPointNo->SetText(StringC(AT_DCP06,P_DCP_POINT_NO_TOK));
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointNo->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pPointNo);

	m_pPointDist = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pPointDist->SetId(ePointDist);
	m_pPointDist->SetText(StringC(AT_DCP06,P_DCP_POINT_DIST_TOK));
	m_pPointDist->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointDist->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pPointDist->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pModel->m_nDecimals);
	m_pPointDist->SetEmptyAllowed(true);
	AddCtrl(m_pPointDist);

	//InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP06,P_DCP_X_TOK));
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pX->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pModel->m_nDecimals);
	m_pX->SetEmptyAllowed(true);
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP06,P_DCP_Y_TOK));
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pModel->m_nDecimals);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pY->SetEmptyAllowed(true);
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP06,P_DCP_Z_TOK));
	m_pZ->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char)m_pModel->m_nDecimals);
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

//void DCP::HiddenPointDialog::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}
void DCP::HiddenPointDialog::OnDialogActivated()
{
	m_pCommon = new Common(m_pModel);
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	
	RefreshControls();
}

void DCP::HiddenPointDialog::UpdateData()
{

	//GetModel()->m_sUser1	= m_pUser1->GetStringInputCtrl()->GetString();
}

// Description: refresh all controls
void DCP::HiddenPointDialog::RefreshControls()
{
	if(m_pPointNo && m_pPointDist && m_pX && m_pY && m_pZ)
	{
		StringC sTemp;

		sTemp.Format(L"%d/%d",m_iCurrentPoint, MAX_POINTS_IN_HIDDENPOINT_BAR);
		m_pPointNo->GetStringInputCtrl()->SetString(sTemp);

		if(m_pModel->hidden_point_bar[m_iCurrentPoint-1] != 0.0)
			m_pPointDist->GetFloatInputCtrl()->SetDouble(m_pModel->hidden_point_bar[m_iCurrentPoint-1]);
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
bool DCP::HiddenPointDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::PointBuffModel* pModel = dynamic_cast< DCP::PointBuffModel* >( pModel );

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

// Description: Hello World model
DCP::PointBuffModel* DCP::HiddenPointDialog::GetDataModel() const
{
    return (DCP::PointBuffModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::HiddenPointDialog::PointNext()
{
	if(m_iCurrentPoint == 1)
		m_iCurrentPoint = MAX_POINTS_IN_HIDDENPOINT_BAR;
	else if(m_iCurrentPoint < MAX_POINTS_IN_HIDDENPOINT_BAR)
		m_iCurrentPoint++;	


	RefreshControls();
}
// ================================================================================================
void DCP::HiddenPointDialog::PointPrev()
{
	if(m_iCurrentPoint > 1)
		m_iCurrentPoint--;
	RefreshControls();
}

// ================================================================================================
void DCP::HiddenPointDialog::PointDelete()
{
	MsgBox msgbox;
	StringC msg;
	StringC strActivePoint(L"");

	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)strActivePoint);
	if(msgbox.ShowMessageYesNo(msg))
	{
		memset(&measured_points[m_iCurrentPoint-1],0,sizeof(S_POINT_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::HiddenPointDialog::update_meas_values(double x, double y, double z, short /*/*DCP_POINT_STATUS*/ status)
{
	Common common(m_pModel);
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
short DCP::HiddenPointDialog::calc_hidden_point(void)
{
short points_defined;
short i,count;
double directionX, directionY, directionZ;
double sumX, sumY, sumZ, distanceOffset;

	MsgBox msgbox;

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
		msg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_2_POINTS_TOK);
		msgbox.ShowMessageOk(msg);
		hiddenpoint[0].sta = POINT_NOT_DEFINED; // not calculated
		return false;
	}

	else  if(points_defined == 2)
	{
		DCP9::Geometry::Point p1, p2;
		count = 0;
		for(i=0;i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
		{
			if(measured_points[i].sta != 0)
			{
				count++;
				if(count == 1)
					p1 = DCP9::Geometry::Point(measured_points[i].x, measured_points[i].y, measured_points[i].z);
				if(count == 2)
				{
					p2 = DCP9::Geometry::Point(measured_points[i].x, measured_points[i].y, measured_points[i].z);
					break;
				}
			}
		}
		DCP9::Geometry::Line line(p1, p2);
		std::vector<double> dir = line.direction();
		directionX = dir[0]; directionY = dir[1]; directionZ = dir[2];

		/**********************************
			calculate hidden point
		**********************************/

		sumX = sumY = sumZ = 0.0;

		for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
		{
			if(measured_points[i].sta != 0)
			{

				distanceOffset = 	m_pModel->hidden_point_bar[i];
				sumX = sumX +  (measured_points[i].x - (distanceOffset* directionX));
				sumY = sumY +  (measured_points[i].y - (distanceOffset* directionY));
				sumZ = sumZ +  (measured_points[i].z - (distanceOffset* directionZ));
			}
		}
		hiddenpoint[0].x = sumX / points_defined;
		hiddenpoint[0].y = sumY / points_defined;
		hiddenpoint[0].z = sumZ / points_defined;
		hiddenpoint[0].sta = POINT_MEASURED;
	}
	else
	{
			std::vector<DCP9::Geometry::Point> pts;
			for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
			{
				if(measured_points[i].sta != 0)
					pts.push_back(DCP9::Geometry::Point(measured_points[i].x, measured_points[i].y, measured_points[i].z));
				if((short)pts.size() == points_defined)
					break;
			}
			DCP9::Core::Geometry::LineFitResult result = DCP9::Core::Geometry::bestFitLine(pts);
			if(!result.isValid)
			{
				hiddenpoint[0].sta = POINT_NOT_DEFINED;
				return false;
			}
			directionX = result.direction[0];
			directionY = result.direction[1];
			directionZ = result.direction[2];
			{
				/**********************************
				calculate hidden point
				**********************************/

				sumX = sumY = sumZ = 0.0;

				for(i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR; i++)
				{
					if(measured_points[i].sta != 0)
					{
						distanceOffset = m_pModel->hidden_point_bar[i];
						sumX = sumX +  measured_points[i].x - (distanceOffset * directionX);
						sumY = sumY +  measured_points[i].y - (distanceOffset * directionY);
						sumZ = sumZ +  measured_points[i].z - (distanceOffset * directionZ);
					}
				}
				hiddenpoint[0].x = sumX / points_defined;
				hiddenpoint[0].y = sumY / points_defined;
				hiddenpoint[0].z = sumZ / points_defined;
				hiddenpoint[0].sta = POINT_MEASURED;
			}
	}
	StringC msgTitle;
	StringC sX,sY,sZ;
	sX.LoadTxt(AT_DCP06,P_DCP_X_TOK);
	sY.LoadTxt(AT_DCP06,P_DCP_Y_TOK);
	sZ.LoadTxt(AT_DCP06,P_DCP_Z_TOK);
	
	char tempX[15];
	sprintf(tempX,"%9.*f", m_pModel->m_nDecimals,hiddenpoint[0].x); 

	char tempY[15];
	sprintf(tempY,"%9.*f", m_pModel->m_nDecimals,hiddenpoint[0].y); 

	char tempZ[15];
	sprintf(tempZ,"%9.*f", m_pModel->m_nDecimals,hiddenpoint[0].z); 

	msgTitle.LoadTxt(AT_DCP06,L_DCP_HIDDEN_POINT_TOK);
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

short  DCP::HiddenPointDialog::get_current_point()
{
	return m_iCurrentPoint;
}


// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::HiddenPointController::HiddenPointController(DCP::Model *pModel)
    : m_pDlg( nullptr ), m_pModel(pModel),m_pCommon(0),poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_HIDDENPOINT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::HiddenPointDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( HIDDENPOINT_DLG, m_pDlg, true );


	m_pCommon = new Common(m_pModel);

	isATR = pModel->isATR;
    show_function_keys();
} //lint !e818 Pointer parameter could be declared as pointing to const

void DCP::HiddenPointController::show_function_keys()
{
	// Set the function key
	
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

		vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );
		
		// SHIFT
		/// CAPTIVATE
		//if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
		//{
			vDef.strLable = StringC(AT_DCP06,K_DCP_CAMERA_TOK);
			SetFunctionKey( SHFK1, vDef );
		//}
		
		vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
		SetFunctionKey( SHFK2, vDef );
		
		vDef.strLable = StringC(AT_DCP06,K_DCP_CONFIG_TOK);
		SetFunctionKey( SHFK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_INIT_TOK);
		SetFunctionKey( SHFK4, vDef );

			// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP06;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::HiddenPointController::~HiddenPointController()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::HiddenPointController::SetModel( GUI::ModelC* pModel )
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

// ALL
void DCP::HiddenPointController::OnF1Pressed()
{
	if (m_pDlg == nullptr)
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

		DCP::MeasXYZModel* pModel = new MeasXYZModel;

		sprintf(pModel->sPointId,"HP%d",m_pDlg->get_current_point());
		m_pCommon->strbtrim(pModel->sPointId);
		
		if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::MeasXYZController(m_pModel));
		}
		(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_XYZ_CONTROLLER, true);
	}
}

// ================================================================================================
// Description: F2
// ================================================================================================
void DCP::HiddenPointController::OnF2Pressed()
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

		DCP::MeasDistModel* pModel = new MeasDistModel;

		if(GetController(MEAS_DIST_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_DIST_CONTROLLER, new DCP::MeasDistController(m_pModel));
		}
		(void)GetController( MEAS_DIST_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_DIST_CONTROLLER, true);
	}
} 
// NEXT
void DCP::HiddenPointController::OnF3Pressed()
{
	m_pDlg->PointNext();
}

// PREV
void DCP::HiddenPointController::OnF4Pressed()
{
	m_pDlg->PointPrev();
}

// Description: Handle change of position values
void DCP::HiddenPointController::OnF5Pressed()
{
	if(m_pDlg->calc_hidden_point())
		Close(EC_KEY_CONT);
}
// Description: Handle change of position values
void DCP::HiddenPointController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
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
void DCP::HiddenPointController::OnSHF1Pressed()
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
// Delete
void DCP::HiddenPointController::OnSHF2Pressed()
{
	m_pDlg->PointDelete();
}

// Config
void DCP::HiddenPointController::OnSHF3Pressed()
{
	// create model
	DCP::HiddenPointBarConfModel* pModel = new HiddenPointBarConfModel;
	
	// set current values
	memcpy(pModel->hidden_point_bar,m_pModel->hidden_point_bar, sizeof(double) *MAX_POINTS_IN_HIDDENPOINT_BAR);

	if(GetController(HIDDENPOINT_CONF_CONTROLLER) == nullptr)
	{
		(void)AddController( HIDDENPOINT_CONF_CONTROLLER, new DCP::HiddenPointConfController(m_pModel));
	}
	(void)GetController( HIDDENPOINT_CONF_CONTROLLER )->SetModel(pModel);
	SetActiveController(HIDDENPOINT_CONF_CONTROLLER, true);

}

void DCP::HiddenPointController::OnSHF4Pressed()
{
	if(GetController(INIT_CONTROLLER) == nullptr)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::InitializationController );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pModel);
	SetActiveController(INIT_CONTROLLER, true);
}


// Description: React on close of tabbed dialog
void DCP::HiddenPointController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(HIDDENPOINT_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}


// Description: React on close of controller
void DCP::HiddenPointController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
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
			// get values
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
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
		DCP::HiddenPointBarConfModel* pModel = (DCP::HiddenPointBarConfModel*) GetController( HIDDENPOINT_CONF_CONTROLLER )->GetModel();		
		memcpy(m_pModel->hidden_point_bar, pModel->hidden_point_bar, sizeof(double) *HIDDENPOINT_CONF_CONTROLLER);
	}


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


DCP::HiddenPointDialogBase::HiddenPointDialogBase(int iCtrlId,DCP::Model* pModel ,DCP::PointBuffModel* pDCPPointBuffModel)
{
	if(GetController(iCtrlId) == nullptr)
	{
		(void)AddController( iCtrlId, new DCP::HiddenPointController(pModel));
	}
	(void)GetController( iCtrlId )->SetModel(pDCPPointBuffModel/* m_pMeasureDlg->GetModel()*/);
	SetActiveController(iCtrlId, true);
}