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
#include "calc.h"
#include <math.h>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Aim.hpp>
#include <dcp06/core/DCP_Tool.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_Util.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>
#include <TBL_DistAvgDlg.hpp>
#include <TBL_MeasResults.hpp>
#include <TPI_MeasConfig.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_MeasConfig.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// ================================================================================================
// ====================================  DCP063DMeasControllerC ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================

DCP::DCP06AimControllerC::DCP06AimControllerC(double x, double y, double z, short cds):m_dX(x), m_dY(y), m_dZ(z),
	lockin_done(0),m_iCds(cds)
{
		point1[0] = 0.0;
		point1[1] = 0.0;
		point1[2] = 0.0;
		point1[3] = 0;
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
	//m_pCommon = new DCP06CommonC();
} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06AimControllerC::~DCP06AimControllerC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP06AimControllerC::set_values(double x, double y, double z, short cds)
{
	m_dX = x;
	m_dY = y;
	m_dZ = z;
	m_iCds = cds;
}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::DCP06AimControllerC::OnControllerActivated(void)
{
	// AIM
		/*TBL::PositTelescopeControllerC* poPositTelescope = new TBL::PositTelescopeControllerC();
		poPositTelescope->EnableMessageBox(true);
		poPositTelescope->SetDirection(3.0,1.0);
		GUI::ControllerC::AddController(301, poPositTelescope);
		SetActiveController(301, true);
		*/

}

void DCP::DCP06AimControllerC::Run(void)
{

		point1[0] = m_dX;
		point1[1] = m_dY;
		point1[2] = m_dZ;
		point1[3] = 1;

		// test
		DCP06MsgBoxC msgbox;
		char temp[100];
		sprintf(temp,"X:%.2f Y:%.2f Z%.2f",m_dX, m_dY, m_dZ);
		//msgbox.ShowMessageOk(StringC(temp));
		
		if(m_pDCP06Model->active_tool > 0)
		{
			if(m_pDCP06Model->m_nToolInfo)
			{
				//DCP06MsgBoxC msgBox;
				//msgBox.ShowMessageOk(L"Aim with tool");
				if(GetController(TOOL_CONTROLLER) == NULL)
				{
					(void)AddController( TOOL_CONTROLLER, new DCP::DCP06ToolControllerC());
				}
				(void)GetController( TOOL_CONTROLLER )->SetModel( m_pDCP06Model);
				SetActiveController(TOOL_CONTROLLER, true);
			}
			else
			{
				point1[0] = point1[0] + m_pDCP06Model->active_tool_x;
				point1[1] = point1[1] + m_pDCP06Model->active_tool_y;
				point1[2] = point1[2] + m_pDCP06Model->active_tool_z;
				
				if(!set_aim())
					Close(EC_KEY_CONT);
			}
		} 
		else
		{
			short rr = set_aim();
			//if(!set_aim())
			if(!rr)
				Close(EC_KEY_CONT);
		}

	
}


// ================================================================================================
// Description: set_aim
// ================================================================================================
short DCP::DCP06AimControllerC::set_aim()
{
	
		// Object coordinate system, POM
		if (m_iCds/*m_pDCP06Model->active_coodinate_system*/ == OCSP)
		{
			ptrans(point1, &m_pDCP06Model->ocsp_inv_matrix[0], &point2);
		}
		
		// Object coordinate system, CHST 
		else if (m_iCds/*m_pDCP06Model->active_coodinate_system*/ == OCSC)
		{
			ptrans(point1, &m_pDCP06Model->ocsc_inv_matrix[0], &point2);
		}

		// Object coordinate system, DOM 
		else if (m_iCds/*m_pDCP06Model->active_coodinate_system*/ == OCSD)
		{
			ptrans(point1, &m_pDCP06Model->ocsd_inv_matrix[0], &point2);
		}

		// Object coordinate system, USER DEFINED 
		else if (m_iCds/*m_pDCP06Model->active_coodinate_system*/ == OCSU)
		{
			ptrans(point1, &m_pDCP06Model->ocsu_inv_matrix[0], &point2);
		}
		
		// Device coordinate system 
		else
		{
			ptrans(point1, &m_pDCP06Model->dcs_inv_matrix[0], &point2);
		}



		/*******************************************************
			convert cartesian coordinates to sphere

		********************************************************/
		if(m_pDCP06Model->m_nLeftRightHand == RIGHTHAND)
		{
			if(cartsph(point2, &dist1, &fii1, &theta1) == -1)

				return 0;	
		}
		else
		{
			if(cartsph_left(point2, &dist1, &fii1, &theta1) == -1)

				return 0;	
		}

		/*************************
			radians to dec
		**************************/
		// remove 030798
		// theta1 = radtodeg(theta1);
		
		 fii1 = radtodeg(fii1);
	     fii1 = 90 - fii1;
		 fii1 = degtorad(fii1);	

		//sprintf(thetastr,"%6.4f\0", (theta1*400)/360);
		//sprintf(fiistr,"%6.4f\0", (fii1*400)/360);

		// ***********************
		// Added 150499		
		// ***********************
	
		lockin_done = 0;
		if(m_pDCP06Model->isATR)
		{
			// TSEKKAA
			/*
			ON_OFF_TYPE onoff=OFF;
			AUT_GetLockStatus(onoff);
			if(onoff)
			{
				//show_txtmsg("LOCK");
				AUT_LockOut();
				//AUT_LockIn();
				lockin_done = TRUE;
			}
			else
			{
				//show_txtmsg("No LOCK");
			}*/
		}
		
		/*
		if(lockin_done)
		{
			//show_txtmsg("NOMSG");
			Result = BAP_PosTelescope(	BAP_POSIT, BAP_POS_NOMSG, theta1,fii1,5,5);
		}
		else
		{
			//show_txtmsg("MSG");
			Result = BAP_PosTelescope(	BAP_POSIT, BAP_POS_DLG, theta1,fii1,5,5);
		}
		*/		
		// AIM
		
		// test
		char temp[100];
		sprintf(temp,"H:%.4f V:%.4f", theta1, fii1);
		DCP06MsgBoxC msgbox;
		//msgbox.ShowMessageOk(StringC(temp));

		TBL::PositTelescopeControllerC* poPositTelescope = new TBL::PositTelescopeControllerC();
		poPositTelescope->EnableMessageBox(true);
		if(GetController(DO_AIM_CONTROLLER) == NULL)
		{
			/*GUI::ControllerC::*/AddController(DO_AIM_CONTROLLER, poPositTelescope);
		}
		poPositTelescope->SetDirection(theta1,fii1);
		SetActiveController(DO_AIM_CONTROLLER, true);
		
		return 1;
		/*
		if (Result != RC_OK)
		{

		//	( void )BAP_Message (Result,  ReturnKey);
		}
		*/
		/*else 
		{
			if(lockin_done)
			{
				measure_distance();
			}
		*/	
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06AimControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );
	m_pCommon = new DCP06CommonC(m_pDCP06Model);
	return/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    // return SetModel( pModel );
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP06AimControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	/*
	if(m_bPointMenu && lExitCode == 2)
		SetActiveDialog(_3DMEAS_DLG);
	*/

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06AimControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// 
	if(lCtrlID == DO_AIM_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		//DestroyController( lCtrlID );	
		
		// TARVIIKO
		/*
		if(lockin_done)
		{
			measure_distance();
		}
		*/
		Close(EC_KEY_CONT);
	}
	// 
	else if(lCtrlID == TOOL_CONTROLLER )//&& lExitCode == EC_KEY_CONT)
	{
		if(m_pDCP06Model->active_tool > 0 &&  lExitCode == EC_KEY_CONT)
		{	
			point1[0] = point1[0] + m_pDCP06Model->active_tool_x;
			point1[1] = point1[1] + m_pDCP06Model->active_tool_y;
			point1[2] = point1[2] + m_pDCP06Model->active_tool_z;
		}
		set_aim();
	}
	else
	{
		//DestroyController( lCtrlID );	
		Close(EC_KEY_CONT);
	}
	DestroyController( lCtrlID );	
}

