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
#include <dcp06/core/xyz.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Tool.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
//#include <GUI_Timer.hpp>

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

DCP::XYZDialog::XYZDialog(DCP::Model *pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pModel(pModel)/*,m_pInfo(0)*/,iInfoInd(0)
{
	//SetTxtApplicationId(AT_DCP06);
	strInfoText.LoadTxt(AT_DCP06,P_DCP_INFO_TOK);
	m_pModel->active_tool = 0;

}


            // Description: Destructor
DCP::XYZDialog::~XYZDialog()
{
	//KillTimer();


	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::XYZDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();

	//InsertEmptyLine(); CAPTIVATE

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pPointId->SetEmptyAllowed(true);
	AddCtrl(m_pPointId);

	//InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP06,P_DCP_X_TOK));
	m_pX->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pX->SetEmptyAllowed(true);
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP06,P_DCP_Y_TOK));
	m_pY->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pY->SetEmptyAllowed(true);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP06,P_DCP_Z_TOK));
	m_pZ->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pZ->SetEmptyAllowed(true);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pZ);

	//InsertEmptyLine();CAPTIVATE
	//InsertEmptyLine();CAPTIVATE

	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	//SetHelpTok(H_DCP_X_OR_Y_OR_Z_TOK,0);
}

//void DCP::XYZDialog::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

void DCP::XYZDialog::OnDialogActivated()
{
	m_pCommon = new Common(m_pModel);
	
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	RefreshControls();
}

void DCP::XYZDialog::UpdateData()
{

}

// Description: refresh all controls
void DCP::XYZDialog::RefreshControls()
{
	if(m_pPointId  && m_pX && m_pY && m_pZ)
	{	
		char temp[20];
		m_pPointId->GetStringInputCtrl()->SetString(GetDataModel()->m_pPointBuff[0].point_id);
		
		//X
		if(GetDataModel()->m_pPointBuff[0].xsta)
		{
			sprintf(temp,"%9.*f",m_pModel->m_nDecimals,GetDataModel()->m_pPointBuff[0].x); 
			m_pX->GetStringInputCtrl()->SetString(temp);
		}
		else
			m_pX->GetStringInputCtrl()->SetString(L" ");

		// Y
		m_pY->GetStringInputCtrl()->SetString(L" ");
		if(GetDataModel()->m_pPointBuff[0].ysta)
		{
			sprintf(temp,"%9.*f",m_pModel->m_nDecimals,GetDataModel()->m_pPointBuff[0].y); 
			m_pY->GetStringInputCtrl()->SetString(temp);
		}
		else
			m_pY->GetStringInputCtrl()->SetString(L" ");

		// Z
		if(GetDataModel()->m_pPointBuff[0].zsta)
		{
			sprintf(temp,"%9.*f",m_pModel->m_nDecimals,GetDataModel()->m_pPointBuff[0].z); 
			m_pZ->GetStringInputCtrl()->SetString(temp);
		}
		else
			m_pZ->GetStringInputCtrl()->SetString(L" ");

	}
}
// Description: only accept DCP06 Model objects
bool DCP::XYZDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::PointBuffModel* pDcpModel = dynamic_cast< DCP::PointBuffModel* >( pModel );

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

// Description: DCP06 model
DCP::PointBuffModel* DCP::XYZDialog::GetDataModel() const
{
    return (DCP::PointBuffModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


void DCP::XYZDialog::delete_point()
{
	MsgBox msgbox;
	StringC msg;
	StringC strActivePoint(L"");

	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)strActivePoint);
	if(msgbox.ShowMessageYesNo(msg))
	{
		GetDataModel()->m_pPointBuff[0].sta = 0;
		GetDataModel()->m_pPointBuff[0].x = 0.0;
		GetDataModel()->m_pPointBuff[0].xsta = 0;
		GetDataModel()->m_pPointBuff[0].y = 0.0;
		GetDataModel()->m_pPointBuff[0].ysta = 0;
		GetDataModel()->m_pPointBuff[0].z = 0.0;
		GetDataModel()->m_pPointBuff[0].zsta = 0;
	}

}

// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::XYZController::XYZController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel),m_pCommon(0), poVideoDlg(0),m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_XORYORZYZ_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::XYZDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( XORYORZ_DLG, m_pDlg, true );

	m_pCommon = new Common(pModel);

	isATR = pModel->isATR;
    show_function_keys();

	
} //lint !e818 Pointer parameter could be declared as pointing to const

void DCP::XYZController::show_function_keys()
{

	ResetFunctionKeys();

    // Set the function key
	if(m_bCamera)
	{
		FKDef vDef;
		vDef.poOwner = this;
		
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06, K_DCP_ALL_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_X_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_Y_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_Z_TOK);
		SetFunctionKey( FK5, vDef );

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

		vDef.strLable = StringC(AT_DCP06,K_DCP_X_TOK);
		SetFunctionKey( FK2, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_Y_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_Z_TOK);
		SetFunctionKey( FK4, vDef );
		
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

		vDef.strLable = StringC(AT_DCP06,K_DCP_TOOL_TOK);
		SetFunctionKey( SHFK4, vDef );

		// Hide SHF6(Quit Key)
		// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP06;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::XYZController::~XYZController()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::XYZController::SetModel( GUI::ModelC* pModel )
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

// ALL
void DCP::XYZController::OnF1Pressed()
{
	Common common(m_pModel);

	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(XORYORZ_DLG);
		show_function_keys();
	}

	if(m_pModel->m_nOverWriteInfo)
	{
		if(m_pDlg->GetDataModel()->m_pPointBuff[0].xsta != 0 ||
			m_pDlg->GetDataModel()->m_pPointBuff[0].ysta != 0 ||
			m_pDlg->GetDataModel()->m_pPointBuff[0].zsta != 0 )
		{
			if(!common.overwrite_point(m_pDlg->GetDataModel()->m_pPointBuff[0].point_id))
			{
				return;
			}
		
		}

	}

	if(m_pCommon->check_edm_mode())
	{
		DisableFunctionKey(FK1);
		DisableFunctionKey(FK2);
		DisableFunctionKey(FK3);
		DisableFunctionKey(FK4);
		//DisableFunctionKey(FK5);
		DisableFunctionKey(FK6);

		DCP::MeasXYZModel* pModel = new MeasXYZModel;

		sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->m_pPointBuff[0].point_id);
		m_pCommon->strbtrim(pModel->sPointId);


		if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::MeasXYZController(m_pModel));
		}
		(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_XYZ_CONTROLLER, true);
	}
}

// X
void DCP::XYZController::OnF2Pressed()
{
	Common common(m_pModel);

	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(XORYORZ_DLG);
		show_function_keys();
	}

	if(m_pModel->m_nOverWriteInfo)
	{
		if(m_pDlg->GetDataModel()->m_pPointBuff[0].xsta != 0)
		{
			if(!common.overwrite_point(m_pDlg->GetDataModel()->m_pPointBuff[0].point_id))
			{
				return;
			}
		
		}
	}

	DisableFunctionKey(FK1);
	DisableFunctionKey(FK2);
	DisableFunctionKey(FK3);
	DisableFunctionKey(FK4);
	//DisableFunctionKey(FK5);
	DisableFunctionKey(FK6);

	DCP::MeasXYZModel* pModel = new MeasXYZModel;
	
	sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->m_pPointBuff[0].point_id);
	m_pCommon->strbtrim(pModel->sPointId);

	if(GetController(MEAS_X_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_X_CONTROLLER, new DCP::MeasXYZController(m_pModel));
	}
	(void)GetController( MEAS_X_CONTROLLER )->SetModel( pModel);
	SetActiveController(MEAS_X_CONTROLLER, true);

}
// Y
void DCP::XYZController::OnF3Pressed()
{
	Common common(m_pModel);

	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(XORYORZ_DLG);
		show_function_keys();
	}

	if(m_pModel->m_nOverWriteInfo)
	{
		if(m_pDlg->GetDataModel()->m_pPointBuff[0].ysta != 0 )
		{
			if(!common.overwrite_point(m_pDlg->GetDataModel()->m_pPointBuff[0].point_id))
			{
				return;
			}
		
		}
	}
	
	DisableFunctionKey(FK1);
	DisableFunctionKey(FK2);
	DisableFunctionKey(FK3);
	DisableFunctionKey(FK4);
	//DisableFunctionKey(FK5);
	DisableFunctionKey(FK6);

	DCP::MeasXYZModel* pModel = new MeasXYZModel;
	
	sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->m_pPointBuff[0].point_id);
	m_pCommon->strbtrim(pModel->sPointId);

	if(GetController(MEAS_Y_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_Y_CONTROLLER, new DCP::MeasXYZController(m_pModel));
	}
	(void)GetController( MEAS_Y_CONTROLLER )->SetModel( pModel);
	SetActiveController(MEAS_Y_CONTROLLER, true);

}

// Z
void DCP::XYZController::OnF4Pressed()
{
	Common common(m_pModel);

	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(XORYORZ_DLG);
		show_function_keys();
	}

	if(m_pModel->m_nOverWriteInfo)
	{
		if(m_pDlg->GetDataModel()->m_pPointBuff[0].zsta != 0 )
		{
			if(!common.overwrite_point(m_pDlg->GetDataModel()->m_pPointBuff[0].point_id))
			{
				return;
			}
		
		}
	}

	DisableFunctionKey(FK1);
	DisableFunctionKey(FK2);
	DisableFunctionKey(FK3);
	DisableFunctionKey(FK4);
	//DisableFunctionKey(FK5);
	DisableFunctionKey(FK6);


	DCP::MeasXYZModel* pModel = new MeasXYZModel;
	sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->m_pPointBuff[0].point_id);
	m_pCommon->strbtrim(pModel->sPointId);

	if(GetController(MEAS_Z_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_Z_CONTROLLER, new DCP::MeasXYZController(m_pModel));
	}
	(void)GetController( MEAS_Z_CONTROLLER )->SetModel( pModel);
	SetActiveController(MEAS_Z_CONTROLLER, true);

}

// Z with camera
void DCP::XYZController::OnF5Pressed()
{
	if(m_bCamera)
	{
		Common common(m_pModel);

		if (m_pDlg == nullptr)
		{
			USER_APP_VERIFY( false );
			return;
		}
		
		if(m_bCamera)
		{
			m_bCamera = false;
			poVideoDlg->Close();
			SetActiveDialog(XORYORZ_DLG);
			show_function_keys();
		}

		if(m_pModel->m_nOverWriteInfo)
		{
			if(m_pDlg->GetDataModel()->m_pPointBuff[0].zsta != 0 )
			{
				if(!common.overwrite_point(m_pDlg->GetDataModel()->m_pPointBuff[0].point_id))
				{
					return;
				}
			
			}
		}
		DCP::MeasXYZModel* pModel = new MeasXYZModel;
		sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->m_pPointBuff[0].point_id);
		m_pCommon->strbtrim(pModel->sPointId);

		if(GetController(MEAS_Z_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_Z_CONTROLLER, new DCP::MeasXYZController(m_pModel));
		}
		(void)GetController( MEAS_Z_CONTROLLER )->SetModel( pModel);
		SetActiveController(MEAS_Z_CONTROLLER, true);
	}
}

// CONT
void DCP::XYZController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
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
		SetActiveDialog(XORYORZ_DLG);
		show_function_keys();
	}

   
}
void DCP::XYZController::OnSHF1Pressed()
{
	/* CAPTIVATE
	if( ABL::VideoDialogC::IsCameraAvailable(CFA::CT_OVC) )
	{

		if(
		poVideoDlg == 0)
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
// DEL
void DCP::XYZController::OnSHF2Pressed()
{
	
	m_pDlg->delete_point();
	m_pDlg->RefreshControls();
}

// INIT
void DCP::XYZController::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == nullptr)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::InitializationController );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pModel);
	SetActiveController(INIT_CONTROLLER, true);
}

// TOOL
void DCP::XYZController::OnSHF4Pressed()
{
	if(GetController(TOOL_CONTROLLER) == nullptr)
	{
		(void)AddController( TOOL_CONTROLLER, new DCP::ToolController );
	}
	(void)GetController( TOOL_CONTROLLER )->SetModel( m_pModel);
	SetActiveController(TOOL_CONTROLLER, true);
}


// Description: React on close of tabbed dialog
void DCP::XYZController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(XORYORZ_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// Description: React on close of controller
void DCP::XYZController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_XYZ_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		//EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{	
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;

			m_pDlg->GetDataModel()->m_pPointBuff[0].x = x;
			m_pDlg->GetDataModel()->m_pPointBuff[0].y = y;
			m_pDlg->GetDataModel()->m_pPointBuff[0].z = z;
			m_pDlg->GetDataModel()->m_pPointBuff[0].xsta = 1;
			m_pDlg->GetDataModel()->m_pPointBuff[0].ysta = 1;
			m_pDlg->GetDataModel()->m_pPointBuff[0].zsta = 1;
		}
	}

	if(lCtrlID == MEAS_X_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		//EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{	
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_X_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;

			m_pDlg->GetDataModel()->m_pPointBuff[0].x = x;
			m_pDlg->GetDataModel()->m_pPointBuff[0].xsta = 1;
		}
	}

	if(lCtrlID == MEAS_Y_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		//EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{	
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_Y_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;

			m_pDlg->GetDataModel()->m_pPointBuff[0].y = y;
			m_pDlg->GetDataModel()->m_pPointBuff[0].ysta = 1;
		}
	}

	if(lCtrlID == MEAS_Z_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);
		//EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{	
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_Z_CONTROLLER )->GetModel();		
			double x = pModel->m_dX;
			double y = pModel->m_dY;
			double z = pModel->m_dZ;

			m_pDlg->GetDataModel()->m_pPointBuff[0].z = z;
			m_pDlg->GetDataModel()->m_pPointBuff[0].zsta = 1;
		}
	}

	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		if(lExitCode == HIDDEN_POINT)
		{

		}
		else if(lExitCode == X_OR_Y_OR_Z)
		{

		}
		else if(lExitCode == CIRCLE)
		{

		}
		else if(lExitCode == SEPARATE_REC)
		{

		}
		else if(lExitCode == HOME_POINTS)
		{

		}


	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}