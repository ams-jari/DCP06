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
#include <dcp06/core/Model.hpp>
#include <dcp06/init/Initialization.hpp>

#include <dcp06/measurement/MeasDist.hpp>

#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>

#include "calc.h"
#include <TBL_Types.hpp>
#include <GUI_Types.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <TBL_Util.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_Correction.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PII 3.141592654
#define DPR 180.0/PII


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06MeasDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC

DCP::DCP06DoMeasDistControllerC::DCP06DoMeasDistControllerC()
    :TBL::MeasurementC(),poSurveyModel(0),poErrorHandler(0)
{
	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    /*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

	poErrorHandler = new DCP06DistErrorHandlerC();

	 USER_APP_VERIFY( AddGuardController( DEFINE_DIST_MEAS_START_CONTROLLER, CreateMeasStartController() ) );
	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06DoMeasDistControllerC::~DCP06DoMeasDistControllerC()
{
	if(poErrorHandler)
	{
		delete poErrorHandler;
		poErrorHandler = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP06DoMeasDistControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return false;//m_pDCP06MeasDlg->SetModel( pModel );
	
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

TBL::MeasErrorHandlerC::HandlingKindT DCP::DCP06DistErrorHandlerC::HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor)
{

	// StopDist(); Removed 20122014

	//pOwnerController->Close(EC_KEY_ESC);
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgAborted;

	//20102104
	return TBL::MeasErrorHandlerC::HK_None;
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgOk;
}


// Description: React on close of controller
void DCP::DCP06DoMeasDistControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	 /*GUI::*/ControllerC::OnControllerClosed(lExitCode);
	DestroyController( lCtrlID );
}

void DCP::DCP06DoMeasDistControllerC::OnControllerActivated(void)
{
	ControllerC::OnControllerActivated();
	poErrorHandler->Attach();
	ActivateMeasurement();
	TBL::MeasurementC::ExecuteAll();
}
void DCP::DCP06DoMeasDistControllerC::OnControllerClosed(int lExitCode)
{
	poErrorHandler->Detach();
	DeactivateMeasurement(lExitCode);
}

void DCP::DCP06DoMeasDistControllerC::OnPeriodicInclineValidation(int ulParam1, int ulParam2)
{	
	TBL::CompensatorStatusT oStat = TBL::GetCompensatorStatus();
	if(oStat == TBL::CS_OFF || oStat == TBL::CS_IN_RANGE)
	{

	}
	else
	{
		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"OnPeriodicInclineValidation");
		//DCP06Log("GetCompensatorStatus", (int ) oStat);

		Close(EC_KEY_ESC);
	}

	/*
	DCP06MsgBoxC MsgBox;
	MsgBox.ShowMessageOk(L"OnPeriodicInclineValidation");
	DCP06Log("DCP06DoMeasXYZControllerC::OnPeriodicInclineValidation",ulParam1,ulParam2);
	*/
}

void  DCP::DCP06DoMeasDistControllerC::OnStopDistEvent(int unParam1,  int ulParam2)
 {

	//DCP06MsgBoxC MsgBox;
	//MsgBox.ShowMessageOk(L"OnStopDistEvent");
	Close(EC_KEY_ESC);
 }

// Samalainen sitten searchille
void DCP::DCP06DoMeasDistControllerC::OnOperationDistEvent(int unNotifyCode,  int ulOperationId)
{
	double dSlopeDist=0.0,dH=0.0,dV=0.0;

	// call base class
	TBL::MeasurementC::OnOperationDistEvent(unNotifyCode,ulOperationId);

	if(unNotifyCode == TBL::DistanceMeasProcedureC::NC_ON_START)
	{
		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON START");
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_SUCCESS)
	{
		SetMeasData(GetDistanceProc()->GetMeasDataHandle()); // MUISTA T�M�
		dSlopeDist = /*GetModel()*/poSurveyModel->GetMeas().GetSlopeDistance();
		dH = poSurveyModel->GetMeas().GetHorizontalAngle();
		dV = poSurveyModel->GetMeas().GetVerticalAngle();
		int iCount = poSurveyModel->GetMeas().GetAveragedDistCount();

		DCP::DCP06MeasDistModelC* pModel = (DCP::DCP06MeasDistModelC*) GetModel();

		pModel->m_dD = dSlopeDist;
		pModel->m_dH = dH;
		pModel->m_dV = dV;
		
		Close(EC_KEY_CONT);
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_STOP)
	{
		DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON STOP");
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_FAIL)
	{
		DCP06MsgBoxC MsgBox;
		MsgBox.ShowMessageOk(L"Error measurement!");
	}
}

short DCP::DCP06DoMeasDistControllerC::get_xyz_values(double* x, double* y, double* z)
{
	/*
	*x = m_dX;
	*y = m_dY;
	*z = m_dZ;
	*/
	return 1;
}
// ======================================================================================== 
DCP::DCP06MeasDistControllerC::DCP06MeasDistControllerC(DCP::DCP06ModelC* pDCP06Model)
    :TBL::MeasurementC(),poSurveyModel(0), poDCP06Model(pDCP06Model)
{
	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP06MeasDistControllerC::~DCP06MeasDistControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06MeasDistControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	int x = 1;
	x++;
    // Set it to hello world dialog
     return false;//m_pDCP06MeasDlg->SetModel( pModel );
	
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


// Description: React on close of controller
void DCP::DCP06MeasDistControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// Handle averaging and 2 face measurement
	if(lCtrlID == DO_MEAS_DIST_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06MeasDistModelC* pModel = (DCP::DCP06MeasDistModelC*) GetController( DO_MEAS_DIST_CONTROLLER )->GetModel();		
		
		//dist_tot	+=	pModel->m_dD
		//ver_tot		+=	pModel->m_dV;
		//hor_tot		+=	pModel->m_dH;
		Close(EC_KEY_CONT);
	}
	else if(lExitCode != EC_KEY_CONT)
	{
		//DestroyController( lCtrlID );
	}
	DestroyController( lCtrlID );
}

void DCP::DCP06MeasDistControllerC::OnControllerActivated(void)
{
	//ActivateMeasurement();
	//ExecuteAll();
	// start measuring
	/*if(GetController(DO_MEAS_DIST_CONTROLLER) == NULL)
	{
		(void)AddController( DO_MEAS_DIST_CONTROLLER, new DCP::DCP06DoMeasDistControllerC );
	}
	(void)GetController( DO_MEAS_DIST_CONTROLLER )->SetModel(GetModel());
	SetActiveController(DO_MEAS_DIST_CONTROLLER, true);*/

}

void DCP::DCP06MeasDistControllerC::Run()
{
	if(GetController(DO_MEAS_DIST_CONTROLLER) == NULL)
	{
		(void)AddController( DO_MEAS_DIST_CONTROLLER, new DCP::DCP06DoMeasDistControllerC );
	}
	(void)GetController( DO_MEAS_DIST_CONTROLLER )->SetModel(GetModel());
	SetActiveController(DO_MEAS_DIST_CONTROLLER, true);
}

void DCP::DCP06MeasDistControllerC::OnControllerClosed(int lExitCode)
{
	//	DeactivateMeasurement(lExitCode);
}






// ===========================================================================================
// 
// ===========================================================================================
// Instantiate template classes
DCP::DCP06MeasDistModelC::DCP06MeasDistModelC()
{

}

DCP::DCP06MeasDistModelC::~DCP06MeasDistModelC()
{
}



