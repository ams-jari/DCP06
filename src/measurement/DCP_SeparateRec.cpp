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
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_SeparateRec.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/measurement/DCP_MeasDist.hpp>
#include <dcp06/core/DCP_Tool.hpp>



#include <GUI_Types.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>

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

DCP::DCP05SeparateRecDlgC::DCP05SeparateRecDlgC(DCP::DCP05ModelC *pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pDCP05Model(pDCP05Model), angle_ok(false), dist_ok(false), dDist(0.0), dHor(0.0), 
	dVer(0.0),m_pCommon(0)/*,m_pInfo(0)*/,iInfoInd(0)
{
	//SetTxtApplicationId(AT_DCP05);
	strInfoText.LoadTxt(AT_DCP05,P_DCP_INFO_TOK);

	m_pDCP05Model->active_tool = 0;
}


            // Description: Destructor
DCP::DCP05SeparateRecDlgC::~DCP05SeparateRecDlgC()
{
   	 //m_pTimer.KillTimer();

	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP05SeparateRecDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	m_pCommon = new DCP05CommonC(m_pDCP05Model);

	// Add fields to dialog
	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP05,P_DCP_POINT_ID_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pPointId);

	//InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP05,P_DCP_X_TOK));
	m_pX->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);

	m_pX->SetEmptyAllowed(true);
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP05,P_DCP_Y_TOK));
	m_pY->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pY->SetEmptyAllowed(true);
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP05,P_DCP_Z_TOK));
	m_pZ->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZ->SetEmptyAllowed(true);
	AddCtrl(m_pZ);
	
	//InsertEmptyLine();CAPTIVATE
	//InsertEmptyLine();CAPTIVATE
	//InsertEmptyLine();CAPTIVATE

	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	//SetHelpTok(H_DCP_SEPARATE_RECORDING_TOK,0);
}

void DCP::DCP05SeparateRecDlgC::OnDialogActivated()
{
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );	
	//StartCycle( 2000 / GUI::TimerC::iMS_PER_TICK);
	RefreshControls();
	
}

//void DCP::DCP05SeparateRecDlgC::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}
void DCP::DCP05SeparateRecDlgC::UpdateData()
{

}

// Description: refresh all controls
void DCP::DCP05SeparateRecDlgC::RefreshControls()
{
	if(m_pPointId &&  m_pX && m_pY && m_pZ)
	{
		m_pPointId->GetStringInputCtrl()->SetString(StringC(GetDataModel()->m_pPointBuff[0].point_id));
		
		if(dist_ok && angle_ok)
		{
			m_pCommon->to_xyz(dDist, dHor, dVer, &dX, &dY, &dZ, 0, 0, 0, 0);
			GetDataModel()->m_pPointBuff[0].xsta = 1;
			GetDataModel()->m_pPointBuff[0].ysta = 1;
			GetDataModel()->m_pPointBuff[0].zsta = 1;
			GetDataModel()->m_pPointBuff[0].x = dX;
			GetDataModel()->m_pPointBuff[0].y = dY;
			GetDataModel()->m_pPointBuff[0].z = dZ;
			
			char temp[100];
			sprintf(temp,"%9.*f",m_pDCP05Model->m_nDecimals,GetDataModel()->m_pPointBuff[0].x); 
			m_pX->GetStringInputCtrl()->SetString(temp);

			sprintf(temp,"%9.*f",m_pDCP05Model->m_nDecimals,GetDataModel()->m_pPointBuff[0].y); 
			m_pY->GetStringInputCtrl()->SetString(temp);

			sprintf(temp,"%9.*f",m_pDCP05Model->m_nDecimals,GetDataModel()->m_pPointBuff[0].z); 
			m_pZ->GetStringInputCtrl()->SetString(temp);
		}
	}
}
// Description: only accept hello world Model objects
bool DCP::DCP05SeparateRecDlgC::SetModel( GUI::ModelC* pModel )
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
DCP::DCP05PointBuffModelC* DCP::DCP05SeparateRecDlgC::GetDataModel() const
{
    return (DCP::DCP05PointBuffModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP05SeparateRecDlgC::PointDelete()
{
	DCP05MsgBoxC msgbox;
	StringC msg;
	StringC strActivePoint(L"");

	msg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)strActivePoint);
	if(msgbox.ShowMessageYesNo(msg))
	{
		
	}
}

// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05SeparateRecControllerC::DCP05SeparateRecControllerC(DCP::DCP05ModelC *pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model),m_pCommon(0), poVideoDlg(0), m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_SEPARATE_RECORDING_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	m_pCommon = new DCP05CommonC(pDCP05Model);

    // Create a dialog
    m_pDlg = new DCP::DCP05SeparateRecDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SEPARATE_RECORDING_DLG, m_pDlg, true );

    // Set the function key
	
	isATR = pDCP05Model->isATR;
    show_function_keys();

    


} //lint !e818 Pointer parameter could be declared as pointing to const


void DCP::DCP05SeparateRecControllerC::show_function_keys()
{

	ResetFunctionKeys();

	if(m_bCamera)
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_DIST_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_RECORDING_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );
		
		GUI::DesktopC::Instance()->UpdateFunctionKeys();

	}
	else
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_DIST_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_RECORDING_TOK);
		SetFunctionKey( FK3, vDef );

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

		vDef.strLable = StringC(AT_DCP05,K_DCP_TOOL_TOK);
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

DCP::DCP05SeparateRecControllerC::~DCP05SeparateRecControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP05SeparateRecControllerC::SetModel( GUI::ModelC* pModel )
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
void DCP::DCP05SeparateRecControllerC::OnF2Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(SEPARATE_RECORDING_DLG);
		show_function_keys();
	}

	if(m_pCommon->check_edm_mode())
	{

		//DisableFunctionKey(FK1);
		DisableFunctionKey(FK2);
		DisableFunctionKey(FK3);
		//DisableFunctionKey(FK4);
		//DisableFunctionKey(FK5);
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
// Description: Handle change of position values
void DCP::DCP05SeparateRecControllerC::OnF3Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(SEPARATE_RECORDING_DLG);
		show_function_keys();
	}
		unsigned short unMeasHandle =
		TPI::SensorC::Instance()->GetMeasHandleLast();

		TPI::MeasDataC oMeasData;
		TPI::SensorC::Instance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
	
		double dHor = oMeasData.GetHorizontalAngle();
		double dVer = oMeasData.GetVerticalAngle();

		m_pDlg->dHor = dHor;
		m_pDlg->dVer = dVer;
		m_pDlg->angle_ok = true;

		m_pDlg->RefreshControls();


	/*
    if (m_pDCP05MeasDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDCP05MeasDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
	*/
}
// Description: Handle change of position values
void DCP::DCP05SeparateRecControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(!m_bCamera)
	{
	 // Update model
		// Set it to hello world dialog
		m_pDlg->UpdateData();

		// Remove the following statement if you don't want an exit
		// to the main menu
		(void)Close(EC_KEY_CONT);
	}
	else
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(SEPARATE_RECORDING_DLG);
		show_function_keys();
	}
   
}
void DCP::DCP05SeparateRecControllerC::OnSHF1Pressed()
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
// DEL
void DCP::DCP05SeparateRecControllerC::OnSHF2Pressed()
{
	m_pDlg->PointDelete();
}

// INIT
void DCP::DCP05SeparateRecControllerC::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP05InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP05Model);
	SetActiveController(INIT_CONTROLLER, true);
}

// TOOL
void DCP::DCP05SeparateRecControllerC::OnSHF4Pressed()
{
	/*
	// SPECIAL MENU
	DCP::DCP05SpecialMenuDlgC* poSpecialMenuDlg = new DCP::DCP05SpecialMenuDlgC();
	AddDialog(SPECIAL_MENU,poSpecialMenuDlg); 	
	SetActiveDialog(SPECIAL_MENU, true);
	*/
	if(GetController(TOOL_CONTROLLER) == NULL)
	{
		(void)AddController( TOOL_CONTROLLER, new DCP::DCP05ToolControllerC());
	}
	(void)GetController( TOOL_CONTROLLER )->SetModel(m_pDCP05Model);
	SetActiveController(TOOL_CONTROLLER, true);

}


// Description: React on close of tabbed dialog
void DCP::DCP05SeparateRecControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05SeparateRecControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
		// MEASURE XYZ
	if(lCtrlID == MEAS_DIST_CONTROLLER)
	{
		//EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		//EnableFunctionKey(FK4);
		//EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{
			DCP::DCP05MeasDistModelC* pModel = (DCP::DCP05MeasDistModelC*) GetController( MEAS_DIST_CONTROLLER )->GetModel();		
			m_pDlg->dDist = pModel->m_dD;
			m_pDlg->dist_ok = true;		

			 GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP05,I_DCP_DIST_MEASURED_TOK));
		}
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}