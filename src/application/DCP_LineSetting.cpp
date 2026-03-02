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
#include <dcp06/init/DCP_DCP06Init.hpp>
#include <dcp06/core/DCP_DCP06Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/application/DCP_LineSetting.hpp>
#include <dcp06/application/DCP_LineSettingConf.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include "calc.h"
#include <math.h>

#include <GUI_Types.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06LineSettingDlgC::DCP06LineSettingDlgC(DCP::DCP06LSetModelC* pDCP06LSetModel):
		GUI::ModelHandlerC(),GUI::StandardDialogC(),m_pHor(0),m_pVer(0),
		m_pDataModel(pDCP06LSetModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::DCP06LineSettingDlgC::~DCP06LineSettingDlgC()
{
	//m_pTimer.KillTimer();
}

void DCP::DCP06LineSettingDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	//InsertEmptyLine();CAPTIVATE
	//InsertEmptyLine();CAPTIVATE

	//SetColonPosLong( GUI::StandardDialogC::CP_21);

	m_pHor = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pHor->SetId(eHor);
	m_pHor->SetText(StringC(AT_DCP06,P_DCP_LSET_HORIZONTAL_TOK));
	m_pHor->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pHor->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pHor->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)4);
	//m_pHor->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_pHor->SetTextWidth(9 * 40); CAPTIVATE
	//m_pHor->SetAutoColon(false);
	//m_pHor->SetColonPosition(9 *20);
	AddCtrl(m_pHor);

	m_pVer = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pVer->SetId(eVer);
	m_pVer->SetText(StringC(AT_DCP06,P_DCP_LSET_VERTICAL_TOK));
	m_pVer->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)4);
	m_pVer->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pVer->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pVer->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pVer->SetTextWidth(9 * 40); CAPTIVATE
	//m_pVer->SetAutoColon(false);
	//m_pVer->SetColonPosition(9 * 20);
	AddCtrl(m_pVer);

	//SetHelpTok(H_DCP_LINE_SETTING_TOK,0);

	//m_pTimer.SetTimer( 100/*2000*/ / GUI::TimerC::iMS_PER_TICK , 100 / GUI::TimerC::iMS_PER_TICK );
}

//void DCP::DCP06LineSettingDlgC::OnTimer(void)
//{
//	/*
//	unsigned short unMeasHandle =
//		TPI::SensorC::Instance()->GetMeasHandleLast();
//
//	TPI::MeasDataC oMeasData;
//	TPI::SensorC::Instance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
//	
//	m_pDataModel->ang_p = radtodeg(oMeasData.GetHorizontalAngle());
//	m_pDataModel->ver_temp = radtodeg(oMeasData.GetVerticalAngle());
//	*/
//	RefreshControls();
//}

double DCP::DCP06LineSettingDlgC::get_hor_angle()
{
	return m_pHor->GetFloatInputCtrl()->GetDouble();
}

double DCP::DCP06LineSettingDlgC::get_ver_angle()
{
	return m_pVer->GetFloatInputCtrl()->GetDouble();
}

void DCP::DCP06LineSettingDlgC::OnDialogActivated()
{
	// Calc angle to xy-plane
	m_pDataModel->ang1 = acos(m_pDataModel->line[0].uz / 1.00) * (180/PII);
	m_pDataModel->ang2 = 90-m_pDataModel->ang1;	

	// Calc the length of projection in XY-plane
	m_pDataModel->pit  = cos(m_pDataModel->ang2*(PII / 180));
	m_pDataModel->ang = acos(m_pDataModel->line[0].ux / m_pDataModel->pit) *(180/PII);

	if(m_pDataModel->line[0].uy < 0.0)
		m_pDataModel->ang = 360.0 - m_pDataModel->ang;

	if(m_pDataModel->left_cds == LEFTHAND)
	{
		//DCP06MsgBoxC msgbox;
		//msgbox.ShowMessageOk(L"LeftHand");
		m_pDataModel->ang_h = m_pDataModel->ang;
	}
	else
	{
		m_pDataModel->ang_h = 360.0 - m_pDataModel->ang;
		
		if(m_pDataModel->ang_h <= 180.0)
			m_pDataModel->ang_h = 180.0 + m_pDataModel->ang_h;
		else
			m_pDataModel->ang_h = m_pDataModel->ang_h - 180.0;
	}

	RefreshControls();
}

void DCP::DCP06LineSettingDlgC::UpdateData()
{
}

// Description: refresh all controls
void DCP::DCP06LineSettingDlgC::RefreshControls()
{
	if(m_pHor && m_pVer)
	{
		unsigned short unMeasHandle =
		TPI::SensorC::Instance()->GetMeasHandleLast();

		TPI::MeasDataC oMeasData;
		TPI::SensorC::Instance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
	
		m_pDataModel->ang_p = radtodeg(oMeasData.GetHorizontalAngle());
		m_pDataModel->ver_temp = radtodeg(oMeasData.GetVerticalAngle());
		
		if(m_pDataModel->ang_p < m_pDataModel->ang_h)
			m_pDataModel->ang_p = m_pDataModel->ang_p + 360.0; 
					
		m_pDataModel->kulma = m_pDataModel->ang_p - m_pDataModel->ang_h;  
					
		if(m_pDataModel->left_cds == RIGHTHAND)
		{
			m_pDataModel->kulma = 360 - m_pDataModel->kulma; 		   
		}
		m_pHor->GetFloatInputCtrl()->SetDouble(m_pDataModel->kulma);
		m_pVer->GetFloatInputCtrl()->SetDouble(m_pDataModel->ver_temp);
	}
}
// Description: only accept hello world Model objects
bool DCP::DCP06LineSettingDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
		// kopioidaan tarvittavat arvot
		m_pDataModel->left_cds = pDCP06Model->m_nLeftRightHand;
		memcpy(&m_pDataModel->line_dom[0],&pDCP06Model->dom_line_buff[0],sizeof(S_LINE_BUFF));
		m_pDataModel->line = m_pDataModel->line_dom;

        RefreshControls();
		return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP06ModelC* DCP::DCP06LineSettingDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06LineSettingControllerC::DCP06LineSettingControllerC()
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle( StringC(AT_DCP06, T_DCP_LINE_SETTING_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	m_pDataModel = new DCP06LSetModelC;

    // Create a dialog
    m_pDlg = new DCP::DCP06LineSettingDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( LINE_SETTING_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_SET_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	//vDef.nLable = K_DCP_DEL_TOK;
	//SetFunctionKey( SHFK2, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );
	/*
	vDef1.strLable = L"Test";;
	SetFunctionKey( FK3, vDef1 );
	*/
} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP06LineSettingControllerC::~DCP06LineSettingControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06LineSettingControllerC::SetModel( GUI::ModelC* pModel )
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

// SET
void DCP::DCP06LineSettingControllerC::OnF1Pressed()
{
	DCP06LSetConfModelC* pDCP06LSetConfModel = new DCP06LSetConfModelC;
	

	pDCP06LSetConfModel->dHor = m_pDlg->get_hor_angle();
	pDCP06LSetConfModel->dVer = m_pDlg->get_ver_angle();

	if(GetController(LINE_SETTING_CONF_CONTROLLER) == NULL)
	{
		(void)AddController( LINE_SETTING_CONF_CONTROLLER, new DCP::DCP06LineSettingConfControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController( LINE_SETTING_CONF_CONTROLLER )->SetModel(pDCP06LSetConfModel);
	SetActiveController(LINE_SETTING_CONF_CONTROLLER, true);

}

// TEST
/*
void DCP::DCP06LineSettingControllerC::OnF3Pressed()
{
   if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	   unsigned short unMeasHandle =
		TPI::SensorC::Instance()->GetMeasHandleLast();

		TPI::MeasDataC oMeasData;
		TPI::SensorC::Instance()->GetMeasurementRunning( unMeasHandle ,oMeasData);
	
		double dHor = oMeasData.GetHorizontalAngle();
		double dVer = oMeasData.GetVerticalAngle();

		char temp[200];
		sprintf(temp,"H:%.4f, V:%.4f", dHor, dVer);

		DCP06MsgBoxC msgBox;
		msgBox.ShowMessageOk(StringC(temp));

}
*/
// CONT
void DCP::DCP06LineSettingControllerC::OnF6Pressed()
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


// Description: React on close of tabbed dialog
void DCP::DCP06LineSettingControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06LineSettingControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == LINE_SETTING_CONF_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DCP06LSetConfModelC* pModel = (DCP::DCP06LSetConfModelC*) GetController( LINE_SETTING_CONF_CONTROLLER )->GetModel();		
		
		double dHor = pModel->dHor;
		double dVer = pModel->dVer;
		
		if(m_pDataModel->left_cds == RIGHTHAND)
			dHor = 360.0 - dHor;

		m_pDataModel->theta1 = m_pDataModel->ang_h + dHor;
		if(m_pDataModel->theta1 > 360.0) m_pDataModel->theta1 = m_pDataModel->theta1 - 360.0;
		m_pDataModel->fii1 = dVer;

		m_pDataModel->theta1 = degtorad(m_pDataModel->theta1);
		m_pDataModel->fii1 = degtorad(m_pDataModel->fii1);

		TBL::PositTelescopeControllerC* poPositTelescope = new TBL::PositTelescopeControllerC();
		poPositTelescope->EnableMessageBox(true);
		poPositTelescope->SetDirection(m_pDataModel->theta1,m_pDataModel->fii1);
		GUI::ControllerC::AddController(301, poPositTelescope);
		SetActiveController(301, true);
	

	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ===========================================================================================
// DCP05LineSettingModelC
// ===========================================================================================
DCP::DCP06LSetModelC::DCP06LSetModelC()
{
	dHor = 0.0;
	dVer = 0.0;
	
}
DCP::DCP06LSetModelC::~DCP06LSetModelC()
{
}