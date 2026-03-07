// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Job Management
//
// $Workfile: DCP_Application.cpp $
//
// Summary  : 
//   
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 8.07.04 15:34 $
// $Revision: 2 $
// $Modtime: 8.07.04 15:31 $

/* $ History: $
*/
// $NoKeywords: $

#include "stdafx.h"

#include "GUI_AppFileInfo.hpp"

#include <dcp06/application/Application.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/application/ApplicationMenu.hpp>
//#include <ABL_Types.hpp>
#include <ABL_CommandFactory.hpp>
#include <ABL_LoadableApps.hpp>
#include <Onboard_Tok.hpp>

#include <CPI_Sensor.hpp>
#include <GMAT_UnitConverter.hpp>
#include <GUI_HourGlass.hpp>
//#include <GUI_Timer.hpp>
//#include <GUI_BeepInterface.hpp>
#include <GUI_Application.hpp>
#include <GUI_Desktop.hpp>
//#include <GUI_DlgLayoutDef.hpp>

#include <ABL_MsgDef.hpp>
//#include <ABL_Util.hpp>
#include <dcp06/core/MsgBox.hpp>
 
#include "dcp06/core/Defs.hpp"
#include <dcp06/core/Logger.hpp>

//lint -e714 Symbol not referenced

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Registrations  =======================================
// ================================================================================================

using namespace DCP;

// ================================================================================================
// ======================================  Static functions  ======================================
// ================================================================================================



// ================================================================================================
// ====================================  ApplicationMenuDialog =================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
ApplicationMenuDialog::ApplicationMenuDialog(Model* pModel): m_pModel(pModel)
{
	//SetTxtApplicationId( AT_DCP06 );
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
ApplicationMenuDialog::~ApplicationMenuDialog()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void ApplicationMenuDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	SetTitle(StringC(AT_DCP06,T_DCP_APPLICATION_MENU_TOK ));
	//SetHelpTok(H_DCP_APPLICATION_MENU_TOK,0);

	/*
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_LINE_SETTING_TOK), L"", nullptr,	LINE_SETTING);
	AddItem(L"", L"",StringC(AT_DCP06,L_DCP_3DROLLER_TOK),	 L"", nullptr,	FRONT_BACK_SINGLE);
	AddItem(L"", L"",StringC(AT_DCP06,L_DCP_3DSHAFT_ALIGMENT_TOK), L"", nullptr,		SHAFT_ALIGMENT);
	AddItem(L"", L"",StringC(AT_DCP06,L_DCP_SIMPLE_SCANNING_TOK), L"", nullptr,		SIMPLE_SCANNING);
	#ifdef TS16
	#else
		AddItem(L"", L"",StringC(AT_DCP06,L_DCP_ADV_SCANNING_TOK), L"", nullptr,		SCANNING);
	#endif
	*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Line_setting_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Line_setting_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_LINE_SETTING_TOK), L"", nullptr, LINE_SETTING);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"3D_roller_alignment_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"3D_roller_alignment_$SCALEFACTOR$.png"),StringC(AT_DCP06,L_DCP_3DROLLER_TOK),	 L"", nullptr, FRONT_BACK_SINGLE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"3D_shaft_alignment_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"3D_shaft_alignment_$SCALEFACTOR$.png"),StringC(AT_DCP06,L_DCP_3DSHAFT_ALIGMENT_TOK), L"", nullptr, SHAFT_ALIGMENT);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Simple_scanninng_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Simple_scanninng_$SCALEFACTOR$.png"),StringC(AT_DCP06,L_DCP_SIMPLE_SCANNING_TOK), L"", nullptr, SIMPLE_SCANNING);
	#ifdef TS16
	#else
		AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Point_Cloud_Scanning_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Point_Cloud_Scanning_$SCALEFACTOR$.png"),StringC(AT_DCP06,L_DCP_ADV_SCANNING_TOK), L"", nullptr, SCANNING);
	#endif
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Best_fit_line_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Best_fit_line_$SCALEFACTOR$.png"),StringC(AT_DCP06,L_DCP_LINE_FITTING_TOK), L"", nullptr, LINE_FITTING);

	if(m_pModel->bDemoMode)
	{	
		/* CAPTIVATE TBD
		DisableMenuItem(LINE_SETTING);
		DisableMenuItem(FRONT_BACK_SINGLE);
		DisableMenuItem(SHAFT_ALIGMENT);
		*/
		//		SetSelected(-1);
	}

    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
    vDef.poOwner = this;
	SetFunctionKey( FK1, vDef );

	// HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );

	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
// ================================================================================================
void ApplicationMenuDialog::OnSelectionDone(void)
{
	DCP06_TRACE_ENTER;
	short unId = GetSelected();
	DCP06_TRACE_POINT("selected=%d", unId);
	OnF1Pressed();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void ApplicationMenuDialog::OnF1Pressed(void)
{
	DCP06_TRACE_ENTER;
	short unId = GetSelected();
	
	/*if(m_pModel->bDemoMode)
	{
		MsgBox msgBox;
		msgBox.ShowMessageOk(StringC(AT_DCP06,M_DCP_NOT_AVAILABLE_IN_DEMO_TOK ));
		Close(EC_KEY_ESC);
	}
	else
	{*/
	
		Close(GetSelected());
	//}
	DCP06_TRACE_EXIT;
}


