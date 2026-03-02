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
#include <dcp06/calculation/DCP_CalcMenu.hpp>
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

// Calc MENU

DCP06CalcMenuDlgC::DCP06CalcMenuDlgC(DCP06ModelC* pDCP06Model): m_pDCP06Model(pDCP06Model)
{
	//SetTxtApplicationId( AT_DCP06 );
}

// Description: Destructor
DCP06CalcMenuDlgC::~DCP06CalcMenuDlgC()
{
}

void DCP06CalcMenuDlgC::OnInitDialog(void)
{
		
	SetTitle(StringC(AT_DCP06,T_DCP_CALCULATION_MENU_TOK ));
	//SetHelpTok(H_DCP_CALCULATION_MENU_TOK,0);

	/*
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_DISTANCE_TOK),L"", NULL,	CALC_DISTANCE);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_ANGLE_TOK), L"", NULL,		CALC_ANGLE);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_CIRCLE_TOK), L"", NULL,	CALC_CIRCLE);
	*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Distance_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Distance_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_DISTANCE_TOK),L"", NULL,	CALC_DISTANCE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Angle_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Angle_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_ANGLE_TOK), L"", NULL,		CALC_ANGLE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Calculation_circle_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Calculation_circle_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_CIRCLE_TOK), L"", NULL,	CALC_CIRCLE);

	if(m_pDCP06Model->bDemoMode)
	{	
		/* CAPTIVATE TBD
		DisableMenuItem(CALC_DISTANCE);
		DisableMenuItem(CALC_ANGLE);
		DisableMenuItem(CALC_CIRCLE);
		*/
	}

	 FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    vDef.poOwner = this;
	  
	SetFunctionKey( FK1, vDef );

  // HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );

}

// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
//

void DCP06CalcMenuDlgC::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

void DCP06CalcMenuDlgC::OnF1Pressed(void)
{
	/*if(m_pDCP06Model->bDemoMode)
	{
		DCP06MsgBoxC msgBox;
		msgBox.ShowMessageOk(StringC(AT_DCP06,M_DCP_NOT_AVAILABLE_IN_DEMO_TOK ));
		Close(EC_KEY_ESC);
	}
	else
	{*/
		Close(GetSelected());
	//}
}

