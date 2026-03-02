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
#include <dcp06/orientation/DCP_MeasV.hpp>
#include <dcp06/orientation/DCP_OffsV.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_SeparateRec.hpp>
#include <dcp06/measurement/DCP_HomePoints.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/measurement/DCP_MeasXYZ.hpp>
#include <dcp06/measurement/DCP_MeasDist.hpp>
#include <dcp06/measurement/DCP_MidPointController.hpp>
#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>
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

DCP::DCP05MeasVDlgC::DCP05MeasVDlgC(DCP::DCP05ModelC* pDCP05Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pDCP05Model(pDCP05Model), iInfoInd(0)
{
	//SetTxtApplicationId(AT_DCP05);
	m_pCommon = new DCP05CommonC(pDCP05Model);
	strInfoText.LoadTxt(AT_DCP05,P_DCP_INFO_TOK);

	m_pDCP05Model->active_tool = 0;
}


            // Description: Destructor
DCP::DCP05MeasVDlgC::~DCP05MeasVDlgC()
{
	 //m_pTimer.KillTimer();
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP05MeasVDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	//InsertEmptyLine(); CAPTIVATE

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP05,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	//m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pPointId->SetEmptyAllowed(true);
	AddCtrl(m_pPointId);

	// InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP05,P_DCP_X_TOK));
	//m_pX->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pX->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pX->SetEmptyAllowed(true);
	// m_pX->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP05,P_DCP_Y_TOK));
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pY->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pY->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pY->SetEmptyAllowed(true);
	// m_pY->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP05,P_DCP_Z_TOK));
	//m_pZ->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZ->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pZ->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	m_pZ->SetEmptyAllowed(true);
	AddCtrl(m_pZ);

	//InsertEmptyLine(); CAPTIVATE
	// InsertEmptyLine(); CAPTIVATE

	// message....
	/*
	m_pInfo = new GUI::TextCtrlC();
	m_pInfo->SetId(eInfo);
	//m_pInfo->SetColor(GUI::PCI_NTEXT,GUI::BROWN);
	AddCtrl(m_pInfo);
	*/
	//SetHelpTok(H_DCP_MEASV_TOK,0);
	
}

void DCP::DCP05MeasVDlgC::OnDialogActivated()
{
	//m_pTimer.SetTimer( 2000 / GUI::TimerC::iMS_PER_TICK , 2000 / GUI::TimerC::iMS_PER_TICK );
	
	RefreshControls();
}

//void DCP::DCP05MeasVDlgC::OnTimer(void)
//{
//	StringC sMsg = m_pCommon->get_info_text(iInfoInd);
//	//m_pInfo->SetText(strInfoText + sMsg);
//	GUI::DesktopC::Instance()->MessageShow(strInfoText + sMsg,true);
//}

void DCP::DCP05MeasVDlgC::UpdateData()
{
	if(!m_pX->GetFloatInputCtrl()->IsEmpty() && !m_pY->GetFloatInputCtrl()->IsEmpty() &&
		!m_pZ->GetFloatInputCtrl()->IsEmpty())
	{
		GetDataModel()->ref_point_buff.sta	= POINT_MEASURED;
		GetDataModel()->ref_point_buff.x		= m_pX->GetFloatInputCtrl()->GetDouble();
		GetDataModel()->ref_point_buff.y		= m_pY->GetFloatInputCtrl()->GetDouble();
        GetDataModel()->ref_point_buff.z		= m_pZ->GetFloatInputCtrl()->GetDouble();
		
	}
}

// Description: refresh all controls
void DCP::DCP05MeasVDlgC::RefreshControls()
{
	if(m_pPointId && m_pX && m_pY && m_pZ)
	{
		if(GetDataModel()->ref_point_buff.sta != POINT_NOT_DEFINED)
		{
			m_pX->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.x);
			m_pY->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.y);
			m_pZ->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.z);

		}
		else
		{
			m_pX->GetFloatInputCtrl()->SetEmpty();
			m_pY->GetFloatInputCtrl()->SetEmpty();
			m_pZ->GetFloatInputCtrl()->SetEmpty();
		}
		m_pCommon->strbtrim(GetDataModel()->ref_point_buff.point_id);
		m_pPointId->GetStringInputCtrl()->SetString(StringC(GetDataModel()->ref_point_buff.point_id));
	}
}


// Description: only accept hello world Model objects
bool DCP::DCP05MeasVDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05OffsVModelC* pDCP05Model = dynamic_cast< DCP::DCP05OffsVModelC* >( pModel );

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
DCP::DCP05OffsVModelC* DCP::DCP05MeasVDlgC::GetDataModel() const
{
    return (DCP::DCP05OffsVModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

void DCP::DCP05MeasVDlgC::set_values(double x, double y, double z)
{
	m_pX->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.x);
	m_pY->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.y);
	m_pZ->GetFloatInputCtrl()->SetDouble(GetDataModel()->ref_point_buff.z);
	RefreshControls();

}

void DCP::DCP05MeasVDlgC::delete_point()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_TOK);

	StringC strActivePointId(L"");

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	DCP05MsgBoxC MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		memset(&GetDataModel()->ref_point_buff,0,sizeof(S_POINT_BUFF));
		RefreshControls();
	}
}

// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UserControllerC
// 
DCP::DCP05MeasVControllerC::DCP05MeasVControllerC(DCP::DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model),m_pCommon(0),poVideoDlg(0), m_bCamera(false)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_ORIE_DOM_POINT_MEASV_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05MeasVDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( MEASV_DLG, m_pDlg, true );

	m_pCommon = new DCP05CommonC(pDCP05Model);

	isATR = pDCP05Model->isATR;
    show_function_keys();

   

} //lint !e818 Pointer parameter could be declared as pointing to const

void DCP::DCP05MeasVControllerC::show_function_keys()
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

		vDef.strLable = StringC(AT_DCP05,K_DCP_OFFSV_TOK);
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

		vDef.strLable = StringC(AT_DCP05,K_DCP_CHANGE_FACE_TOK);
		SetFunctionKey( SHFK6, vDef );
	}
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

DCP::DCP05MeasVControllerC::~DCP05MeasVControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP05MeasVControllerC::SetModel( GUI::ModelC* pModel )
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
void DCP::DCP05MeasVControllerC::OnF1Pressed()
{
	if(m_bCamera)
	{
		m_bCamera = false;
		poVideoDlg->Close();
		SetActiveDialog(MEASV_DLG);
		show_function_keys();
	}
	
	if(m_pDCP05Model->m_nOverWriteInfo )
	{
		if(m_pDlg->GetDataModel()->ref_point_buff.sta  != POINT_NOT_DEFINED)
		{
				StringC msg;
				DCP05MsgBoxC msgbox;
				msg.LoadTxt(AT_DCP05, M_DCP_OVERWRITE_POINT_TOK);
				msg.Format(msg,(const wchar_t*)StringC(m_pDlg->GetDataModel()->ref_point_buff.point_id));
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
			/*isableFunctionKey(FK3);
			DisableFunctionKey(FK4);
			*/
			DisableFunctionKey(FK5);
			DisableFunctionKey(FK6);

		DCP::DCP05MeasXYZModelC* pModel = new DCP05MeasXYZModelC;

		sprintf(pModel->sPointId,"%6.6s",m_pDlg->GetDataModel()->ref_point_buff.point_id);
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
void DCP::DCP05MeasVControllerC::OnF2Pressed()
{
		// DIST
	if(m_pCommon->check_edm_mode())
	{

		DisableFunctionKey(FK1);
		DisableFunctionKey(FK2);
		/*isableFunctionKey(FK3);
		DisableFunctionKey(FK4);
		*/
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
// OFFSV
void DCP::DCP05MeasVControllerC::OnF5Pressed()
{
	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP05OffsVModelC* pModel = new DCP::DCP05OffsVModelC();
	memcpy(&pModel->ovalues_buff,&m_pDlg->GetDataModel()->ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDlg->GetDataModel()->ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDlg->GetDataModel()->ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = m_pDlg->GetDataModel()->display;

	if(GetController(OFFSV_CONTROLLER) == NULL)
	{
		(void)AddController( OFFSV_CONTROLLER, new DCP::DCP05OffsvControllerC(m_pDCP05Model,pModel->display ) );
	}
	(void)GetController( OFFSV_CONTROLLER )->SetModel( pModel);
	SetActiveController(OFFSV_CONTROLLER, true);
	
}
// Description: Handle change of position values
void DCP::DCP05MeasVControllerC::OnF6Pressed()
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
		SetActiveDialog(MEASV_DLG);
		show_function_keys();
	}
   
}
void DCP::DCP05MeasVControllerC::OnSHF1Pressed()
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
void DCP::DCP05MeasVControllerC::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}

// INIT
void DCP::DCP05MeasVControllerC::OnSHF3Pressed()
{
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP05InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP05Model);
	SetActiveController(INIT_CONTROLLER, true);
}

void DCP::DCP05MeasVControllerC::OnSHF4Pressed()
{
	if(GetController(SPECIAL_MENU_CONTROLLER) == NULL)
	{
		(void)AddController( SPECIAL_MENU_CONTROLLER, new DCP::DCP05SpecialMenuControllerC(m_pDCP05Model) );
	}
	(void)GetController( SPECIAL_MENU_CONTROLLER )->SetModel(m_pDCP05Model/* m_pDlg->GetDataModel()*/);
	SetActiveController(SPECIAL_MENU_CONTROLLER, true);

}

// CHANGE FACE
void DCP::DCP05MeasVControllerC::OnSHF6Pressed()
{
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER, poChangeFace);
		SetActiveController(CHANGE_FACE_CONTROLLER, true);
}

// Description: React on close of tabbed dialog
void DCP::DCP05MeasVControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == CAMERA_DLG)
	{
		SetActiveDialog(MEASV_DLG);
		m_bCamera = false;
		show_function_keys();
	}
}

// Description: React on close of controller
void DCP::DCP05MeasVControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_DIST_CONTROLLER)
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		/*EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);*/
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
	if(lCtrlID == MEAS_XYZ_CONTROLLER )
	{
		EnableFunctionKey(FK1);
		EnableFunctionKey(FK2);
		/*EnableFunctionKey(FK3);
		EnableFunctionKey(FK4);*/
		EnableFunctionKey(FK5);
		EnableFunctionKey(FK6);

		if(lExitCode == EC_KEY_CONT)
		{
			DCP::DCP05MeasXYZModelC* pModel = (DCP::DCP05MeasXYZModelC*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
			m_pDlg->GetDataModel()->ref_point_buff.x =pModel->m_dX;
			m_pDlg->GetDataModel()->ref_point_buff.y =pModel->m_dY;
			m_pDlg->GetDataModel()->ref_point_buff.z =pModel->m_dZ;
			m_pDlg->GetDataModel()->ref_point_buff.sta = POINT_MEASURED;
		}
	}

	if(lCtrlID == OFFSV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05OffsVModelC* pModel = (DCP::DCP05OffsVModelC*) GetController( OFFSV_CONTROLLER )->GetModel();		
		memcpy(&m_pDlg->GetDataModel()->ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDataModel()->ovalues_tool_buff, &pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDataModel()->ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));


		m_pDlg->UpdateData();
		(void)Close(EC_KEY_CONT);
	}
	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		if(lExitCode == HIDDEN_POINT)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;
			sprintf(pModel->m_pPointBuff[0].point_id,"%s", m_pDlg->GetDataModel()->ref_point_buff.point_id); 
			if(GetController(HIDDENPOINT_CONTROLLER) == NULL)
			{
				(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::DCP05HiddenPointControllerC(m_pDCP05Model));
			}
			(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel(pModel );
			SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;
			sprintf(pModel->m_pPointBuff[0].point_id,"%s", m_pDlg->GetDataModel()->ref_point_buff.point_id); 
			if(GetController(XYZ_CONTROLLER) == NULL)
			{
				(void)AddController( XYZ_CONTROLLER, new DCP::DCP05XYZControllerC(m_pDCP05Model));
			}
			(void)GetController( XYZ_CONTROLLER )->SetModel( pModel);
			SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			if(GetController(CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::DCP05CircleControllerC( m_pDCP05Model) );
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel);
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == SEPARATE_REC)
		{
			// create model
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			if(GetController(SEPARATE_RECORDING_CONTROLLER) == NULL)
			{
				(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::DCP05SeparateRecControllerC(m_pDCP05Model) );
			}
			(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel(pModel );
			SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			if(GetController(HOME_POINTS_CONTROLLER) == NULL)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::DCP05HomePointsControllerC(m_pDCP05Model) );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel( m_pDCP05Model/*m_pDlg->GetDataModel()*/);
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}
		else if(lExitCode == MID_POINT)
		{
			DCP::DCP05PointBuffModelC* pModel = new DCP05PointBuffModelC;

			sprintf(pModel->m_pPointBuff[0].point_id,"%6.6s", "");//m_pDataModel->pid_ptr);
			
			if(GetController(MID_POINT_CONTROLLER) == NULL)
			{
				(void)AddController( MID_POINT_CONTROLLER, new DCP::DCP05MidPointControllerC(m_pDCP05Model));
			}
			(void)GetController( MID_POINT_CONTROLLER )->SetModel( pModel);
			SetActiveController(MID_POINT_CONTROLLER, true);
		}


	}
		// HIDDENPOINT
	if(lCtrlID == HIDDENPOINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( HIDDENPOINT_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->ref_point_buff.x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->ref_point_buff.y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->ref_point_buff.z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->ref_point_buff.sta = 1;
		}
	}

	// X or Y or Z
	if(lCtrlID == XYZ_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( XYZ_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].xsta)
			m_pDlg->GetDataModel()->ref_point_buff.x = pModel->m_pPointBuff[0].x;

		if(pModel->m_pPointBuff[0].ysta)
			m_pDlg->GetDataModel()->ref_point_buff.y = pModel->m_pPointBuff[0].y;

		if(pModel->m_pPointBuff[0].zsta)
			m_pDlg->GetDataModel()->ref_point_buff.z = pModel->m_pPointBuff[0].z;

		//m_pDlg->update_meas_values(pModel->m_pPointBuff[0].x,pModel->m_pPointBuff[0].y,pModel->m_pPointBuff[0].z, POINT_MEASURED);
	}
	
	// CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( CIRCLE_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->ref_point_buff.x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->ref_point_buff.y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->ref_point_buff.z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->ref_point_buff.sta = 1;
		}
	}

	if(lCtrlID == MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( MID_POINT_CONTROLLER )->GetModel();		
		if(pModel->m_pPointBuff[0].sta != 0)
		{
			m_pDlg->GetDataModel()->ref_point_buff.x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->ref_point_buff.y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->ref_point_buff.z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->ref_point_buff.sta = 1;
		}
	}

	// SEPARATE
	if(lCtrlID == SEPARATE_RECORDING_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		// FIXED 01042011
		DCP::DCP05PointBuffModelC* pModel = (DCP::DCP05PointBuffModelC*) GetController( SEPARATE_RECORDING_CONTROLLER )->GetModel();		
		
		//if(pModel->m_pPointBuff[0].sta != 0)
		if(pModel->m_pPointBuff[0].xsta !=0 && pModel->m_pPointBuff[0].ysta !=0 && pModel->m_pPointBuff[0].zsta !=0)
		{
			m_pDlg->GetDataModel()->ref_point_buff.x = pModel->m_pPointBuff[0].x;
			m_pDlg->GetDataModel()->ref_point_buff.y = pModel->m_pPointBuff[0].y;
			m_pDlg->GetDataModel()->ref_point_buff.z = pModel->m_pPointBuff[0].z;
			m_pDlg->GetDataModel()->ref_point_buff.sta = 1;
		}
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}