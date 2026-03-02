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
#include <dcp06/core/DCP_SpecialMenu.hpp>
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

// SPECIAL MENU

DCP05SpecialMenuDlgC::DCP05SpecialMenuDlgC(DCP05ModelC* pDCP05Model,int showItems):m_pDCP05Model(pDCP05Model),m_pShowItems(showItems)
{
	//SetTxtApplicationId( AT_DCP05 );
}

// Description: Destructor
DCP05SpecialMenuDlgC::~DCP05SpecialMenuDlgC()
{
}

void DCP05SpecialMenuDlgC::OnInitDialog(void)
{
		
	SetTitle(StringC(AT_DCP05,T_DCP_SPECIAL_MENU_TOK ));
	//SetHelpTok(H_DCP_SPECIAL_MENU_TOK,0);
	/*
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_HIDDEN_POINT_TOK),L"", NULL,HIDDEN_POINT);
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_X_OR_Y_OR_Z_TOK),L"", NULL	,X_OR_Y_OR_Z);
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_CIRCLE_TOK),L"", NULL, CIRCLE);
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_SEPARATE_REC_TOK),L"", NULL,SEPARATE_REC);
	AddItem(L"", L"", StringC(AT_DCP05,L_DCP_HOME_POINTS_TOK),L"", NULL,HOME_POINTS);
	*/
	if(!(m_pShowItems & DISABLE_HIDDEN_POINT))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Hidden_point_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Hidden_point_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_HIDDEN_POINT_TOK),L"", NULL,HIDDEN_POINT);

	if(!(m_pShowItems & DISABLE_X_OR_Y_OR_Z))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"X_or_Y_or_Z_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"X_or_Y_or_Z_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_X_OR_Y_OR_Z_TOK),L"", NULL	,X_OR_Y_OR_Z);

	if(!(m_pShowItems & DISABLE_CIRCLE))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Circle_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Circle_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_CIRCLE_TOK),L"", NULL, CIRCLE);

	if(!(m_pShowItems & DISABLE_SEPARATE_REC))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Separate_recording_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Separate_recording_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_SEPARATE_REC_TOK),L"", NULL,SEPARATE_REC);

	if(!(m_pShowItems & DISABLE_HOME_POINTS))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"Home_Points_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"Home_Points_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_HOME_POINTS_TOK),L"", NULL,HOME_POINTS);
	
	if(!(m_pShowItems & DISABLE_MID_POINT))
		AddItem(GUI::GetAppResourceUrl(AT_DCP05,"mid_point_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP05,"mid_point_$SCALEFACTOR$.png"), StringC(AT_DCP05,L_DCP_MID_POINT_TOK),L"", NULL,MID_POINT);
	
	if(m_pDCP05Model->bDemoMode)
	{	
		/* CAPTIVATE TBD
		DisableMenuItem(HIDDEN_POINT);
		DisableMenuItem(X_OR_Y_OR_Z);
		DisableMenuItem(CIRCLE);
		DisableMenuItem(SEPARATE_REC);
		DisableMenuItem(HOME_POINTS);
		*/
	}

}

// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
//

void DCP05SpecialMenuDlgC::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

void DCP05SpecialMenuDlgC::OnF1Pressed(void)
{
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


DCP::DCP05SpecialMenuControllerC::DCP05SpecialMenuControllerC(DCP05ModelC* pDCP05Model,int showItems) 
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_SPECIAL_MENU_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP05SpecialMenuDlgC(m_pDCP05Model, showItems);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SPECIAL_MENU, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
    vDef.poOwner = this;
	  
	SetFunctionKey( FK1, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05SpecialMenuControllerC::~DCP05SpecialMenuControllerC()
{
}

// Description: Handle change of position values
void DCP::DCP05SpecialMenuControllerC::OnF1Pressed()
{
    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}



// Description: React on close of tabbed dialog
void DCP::DCP05SpecialMenuControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
