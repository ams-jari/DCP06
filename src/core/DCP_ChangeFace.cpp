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

#include <dcp06/core/DCP_ChangeFace.hpp>
#include <dcp06/core/DCP_Defs.hpp>


#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>



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

DCP::DCP06ChangeFaceC::DCP06ChangeFaceC(int iCtrlId)
{
		// I<>II
		TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
		poChangeFace->EnableMessageBox(true);
		GUI::ControllerC::AddController(iCtrlId, poChangeFace);
		SetActiveController(iCtrlId, true);
}
// ****************************************************************************************
DCP::DCP06ChangeFaceC::~DCP06ChangeFaceC()
{
	
}


