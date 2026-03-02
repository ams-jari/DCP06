// ================================================================================================
//
// Project  : Pluto/Venus
//
// Component: Job Management
//
// $Workfile: DCP_Application.cpp $
//
// Summary  : 
//   
// ------------------------------------------------------------------------------------------------
//
// Copyright 2002 by Leica Geosystems AG, Heerbrugg
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

#include <dcp06/application/DCP_Application.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/application/DCP_ApplicationMenu.hpp>
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
#include <dcp06/core/DCP_MsgBox.hpp>
 
#include "dcp06/core/DCP_Defs.hpp"

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
// ====================================  DCP05ApplicationMenuDlgC =================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP05ApplicationMenuDlgC::DCP05ApplicationMenuDlgC(DCP05ModelC* pDCP05Model): m_pDCP05Model(pDCP05Model)
{
	//SetTxtApplicationId( AT_DCP05 );
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP05ApplicationMenuDlgC::~DCP05ApplicationMenuDlgC()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP05ApplicationMenuDlgC::OnInitDialog(void)
{
		
	SetTitle(StringC(AT_DCP05,T_DCP_APPLICATION_MENU_TOK ));
	//SetHelpTok(H_DCP_APPLICATION_MENU_TOK,0);

	/*
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_LINE_SETTING_TOK), L"", NULL,	LINE_SETTING);
	AddItem(L"", L"",StringC(AT_DCP05,L_DCP_3DROLLER_TOK),	 L"", NULL,	FRONT_BACK_SINGLE);
	AddItem(L"", L"",StringC(AT_DCP05,L_DCP_3DSHAFT_ALIGMENT_TOK), L"", NULL,		SHAFT_ALIGMENT);
	AddItem(L"", L"",StringC(AT_DCP05,L_DCP_SIMPLE_SCANNING_TOK), L"", NULL,		SIMPLE_SCANNING);
	#ifdef TS16
	#else
		AddItem(L"", L"",StringC(AT_DCP05,L_DCP_ADV_SCANNING_TOK), L"", NULL,		SCANNING);
	#endif
	*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Line_setting_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Line_setting_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_LINE_SETTING_TOK), L"", NULL, LINE_SETTING);
	AddItem(GUI::GetAppResourceUrl(AT_DCP05,"3D_roller_alignment_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"3D_roller_alignment_$SCALEFACTOR$.png"),StringC(AT_DCP05,L_DCP_3DROLLER_TOK),	 L"", NULL, FRONT_BACK_SINGLE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP05,"3D_shaft_alignment_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"3D_shaft_alignment_$SCALEFACTOR$.png"),StringC(AT_DCP05,L_DCP_3DSHAFT_ALIGMENT_TOK), L"", NULL, SHAFT_ALIGMENT);
	AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Simple_scanninng_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Simple_scanninng_$SCALEFACTOR$.png"),StringC(AT_DCP05,L_DCP_SIMPLE_SCANNING_TOK), L"", NULL, SIMPLE_SCANNING);
	#ifdef TS16
	#else
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Point_Cloud_Scanning_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Point_Cloud_Scanning_$SCALEFACTOR$.png"),StringC(AT_DCP05,L_DCP_ADV_SCANNING_TOK), L"", NULL, SCANNING);
	#endif
	AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Best_fit_line_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Best_fit_line_$SCALEFACTOR$.png"),StringC(AT_DCP05,L_DCP_LINE_FITTING_TOK), L"", NULL, LINE_FITTING);

	if(m_pDCP05Model->bDemoMode)
	{	
		/* CAPTIVATE TBD
		DisableMenuItem(LINE_SETTING);
		DisableMenuItem(FRONT_BACK_SINGLE);
		DisableMenuItem(SHAFT_ALIGMENT);
		*/
		//SetSelected(-1);
	}

    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
    vDef.poOwner = this;
	SetFunctionKey( FK1, vDef );

	// HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP05;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );


}

// ================================================================================================
// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
// ================================================================================================
void DCP05ApplicationMenuDlgC::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void DCP05ApplicationMenuDlgC::OnF1Pressed(void)
{

	short unId = GetSelected();
	
	/*if(m_pDCP05Model->bDemoMode)
	{
		DCP05MsgBoxC msgBox;
		msgBox.ShowMessageOk(StringC(AT_DCP05,M_DCP_NOT_AVAILABLE_IN_DEMO_TOK ));
		Close(EC_KEY_ESC);
	}
	else
	{*/
	
		Close(GetSelected());
	//}
}


