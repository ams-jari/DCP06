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
#include <dcp06/calculation/CalculationMenu.hpp>
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

CalculationMenuDialog::CalculationMenuDialog(Model* pModel): m_pModel(pModel)
{
	//SetTxtApplicationId( AT_DCP06 );
}

// Description: Destructor
CalculationMenuDialog::~CalculationMenuDialog()
{
}

void CalculationMenuDialog::OnInitDialog(void)
{
		
	SetTitle(StringC(AT_DCP06,T_DCP_CALCULATION_MENU_TOK ));
	//SetHelpTok(H_DCP_CALCULATION_MENU_TOK,0);

	/*
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_DISTANCE_TOK),L"", nullptr,	CALC_DISTANCE);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_ANGLE_TOK), L"", nullptr,		CALC_ANGLE);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_CIRCLE_TOK), L"", nullptr,	CALC_CIRCLE);
	*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Distance_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Distance_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_DISTANCE_TOK),L"", nullptr,	CALC_DISTANCE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Angle_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Angle_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_ANGLE_TOK), L"", nullptr,		CALC_ANGLE);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Calculation_circle_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Calculation_circle_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_CIRCLE_TOK), L"", nullptr,	CALC_CIRCLE);

	if(m_pModel->bDemoMode)
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

void CalculationMenuDialog::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

void CalculationMenuDialog::OnF1Pressed(void)
{
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
}

