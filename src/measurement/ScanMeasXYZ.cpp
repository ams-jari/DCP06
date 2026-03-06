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
#include <dcp06/measurement/ScanMeasXYZ.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Tool.hpp>

#include "calc.h"
#include <TBL_Types.hpp>
#include <GUI_Types.hpp>
#include <GUI_HourGlass.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <TBL_Util.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_Correction.hpp>
#include <TPI_MeasConfig.hpp>
#include <GUI_Util.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::MeasureDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


//-------------------------------------------------------------------------------------------------
// UserController

DCP::DoScanMeasXYZController::DoScanMeasXYZController()
    :TBL::MeasurementC(),poSurveyModel(0),poErrorHandler(0)
{
	Log("DoScanMeasXYZController::DoScanMeasXYZController");

	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    /*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);


	//poErrorHandler = new TBL::MeasErrorHandlerC();
	poErrorHandler = new ScanErrorHandler();
	//poErrorHandler->pOwnerController = this;

	 USER_APP_VERIFY( AddGuardController( DEFINE_MEAS_START_CONTROLLER, CreateMeasStartController() ) );
	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DoScanMeasXYZController::~DoScanMeasXYZController()
{
	Log("DoScanMeasXYZController::~DoScanMeasXYZController");
		if(poErrorHandler)
	{
		delete poErrorHandler;
		poErrorHandler = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DoScanMeasXYZController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return false;//m_pMeasureDlg->SetModel( pModel );
}

TBL::MeasErrorHandlerC::HandlingKindT DCP::ScanErrorHandler::HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor)
{

	 //StopDist(); //Removed 20122014

	//pOwnerController->Close(EC_KEY_ESC);
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgAborted;

	//20102104
	//return TBL::MeasErrorHandlerC::HK_None;
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgOk;

	//// ignore meas errors from angle measurement
    //if( ES_AngleMeasurement == eSource )
    //{
    //    return HK_ErrorIgnored;
    //}

    // ignore angle meas errors from dist measurement
	if( ES_DistMeasurement == eSource )
    {
        return HK_ErrorIgnored;
    }

    if( eMeasError == ME_Unknown || eMeasError == ME_Void )
    {
        return HK_ErrorIgnored;
    }
    else
    {  
        // call base class
        return TBL::MeasErrorHandlerC::HandleMeasError(eMeasError, eSource, ulErrorCodeSensor);
    }
}

// Description: React on close of controller
void DCP::DoScanMeasXYZController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	Log("DoScanMeasXYZController::OnActiveControllerClosed",lCtrlID,lExitCode);
	// Call base class
    /*GUI::*/ControllerC::OnControllerClosed(lExitCode);
	
	DestroyController( lCtrlID );
}

void DCP::DoScanMeasXYZController::OnControllerActivated(void)
{
	ControllerC::OnControllerActivated();

	Log("DoScanMeasXYZController::OnControllerActivated / ActivateMeasurement");

	poErrorHandler->Attach();
	
	ActivateMeasurement();
	
	//OnF1Pressed();

	Log("DoScanMeasXYZController /ExecuteAll");
	TBL::MeasurementC::ExecuteAll();
}
void DCP::DoScanMeasXYZController::OnControllerClosed(int lExitCode)
{
	Log("DoScanMeasXYZController::OnControllerClosed",lExitCode);
	//MsgBox msgBox;
	//msgBox.ShowMessageOk(L"DoMeasXYZController::OnControllerClosed");

	poErrorHandler->Detach();

	DeactivateMeasurement(lExitCode);
}

void DCP::DoScanMeasXYZController::OnPeriodicInclineValidation(int ulParam1, int ulParam2)
{	
	TBL::CompensatorStatusT oStat = TBL::GetCompensatorStatus();
	if(oStat == TBL::CS_OFF || oStat == TBL::CS_IN_RANGE)
	{

	}
	else
	{
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"OnPeriodicInclineValidation");
		//Log("GetCompensatorStatus", (int ) oStat);

		Close(EC_KEY_ESC);
	}

	/*
	MsgBox MsgBox;
	MsgBox.ShowMessageOk(L"OnPeriodicInclineValidation");
	Log("DoMeasXYZController::OnPeriodicInclineValidation",ulParam1,ulParam2);
	*/
	
}


void DCP::DoScanMeasXYZController::OnFinish()
{
	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"Finish");		
}

void  DCP::DoScanMeasXYZController::OnStopDistEvent(int unParam1,  int ulParam2)
 {
	Close(EC_KEY_ESC);
 }

// Samalainen sitten searchille
void DCP::DoScanMeasXYZController::OnOperationDistEvent(int unNotifyCode,  int ulOperationId)
{
	double dSlopeDist=0.0,dH=0.0,dV=0.0;
	
	// call base class
	TBL::MeasurementC::OnOperationDistEvent(unNotifyCode,ulOperationId);

	if(unNotifyCode == TBL::DistanceMeasProcedureC::NC_ON_START)
	{
		Log("DoScanMeasXYZController::OnOperationDistEvent NC_ON_START");

		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON START");
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_SUCCESS)
	{
		SetMeasData(GetDistanceProc()->GetMeasDataHandle()); // MUISTA T�M�
		dSlopeDist = /*GetModel()*/poSurveyModel->GetMeas().GetSlopeDistance();
		dH = poSurveyModel->GetMeas().GetHorizontalAngle();
		dV = poSurveyModel->GetMeas().GetVerticalAngle();

		DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetModel();

		pModel->m_dD = dSlopeDist;
		pModel->m_dH = dH;
		pModel->m_dV = dV;

		
		TPI::CorrectionC oCorrectionData;
		oCorrectionData.Get();
		pModel->m_dPPM  = oCorrectionData.GetAtmoshericPPM();
		pModel->m_fPrismConst = poSurveyModel->GetMeas().GetReflectorConstant();
		pModel->m_fPrismHeight = poSurveyModel->GetMeas().GetReflectorHeight();
		pModel->m_iAverageDistCount = poSurveyModel->GetMeas().GetAveragedDistCount();
		pModel->m_dAveragedDistStdDev = poSurveyModel->GetMeas().GetAveragedDistStdDev ();

		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"NC_SUCCESS");
		Log("DoScanMeasXYZController::OnOperationDistEvent NC_SUCCESS");
		Close(EC_KEY_CONT); 
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_STOP)
	{
		Log("DoScanMeasXYZController::OnOperationDistEvent NC ON STOP");
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON STOP");
		Close(EC_KEY_ESC);
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_FAIL)
	{
		Log("DoScanMeasXYZController::OnOperationDistEvent NC_ON_FAIL");
		
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"Error measurement!");
		Close(EC_KEY_ESC);
	}
	else
	{
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"Jokin muu");
		Close(EC_KEY_ESC);
	}
}

void DCP::DoScanMeasXYZController::OnOperationSearchEvent(int unNotifyCode, int ulOperationId)
{
		// call base class
	TBL::MeasurementC::OnOperationSearchEvent(unNotifyCode,ulOperationId);
	
	if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_SUCCESS)
	{
	
		//Close(EC_KEY_CONT); 
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_STOP)
	{
		Close(EC_KEY_ESC);
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_FAIL)
	{
		Close(EC_KEY_ESC);
	}
}


short DCP::DoScanMeasXYZController::get_xyz_values(double* x, double* y, double* z)
{
	*x = m_dX;
	*y = m_dY;
	*z = m_dZ;

	return 1;
}
// ======================================================================================== 
DCP::ScanMeasXYZController::ScanMeasXYZController(DCP::Model* pModel)
    :TBL::MeasurementC(),poSurveyModel(0), poModel(pModel),m_iCount(0),m_iCount2(0),m_iUseTool(0),poHourGlass(0)
{
	Log("MeasXYZController::MeasXYZController");

	poHourGlass = new GUI::HourGlassC();
	poHourGlass->Pause();

	dist_tot=0.0; ver_tot=0.0; hor_tot=0.0;
	dist_tot2=0.0; hor_tot2=0.0; ver_tot2=0.0;
	dist1=0.0; hor1=0.0; ver1=0.0;
	dist2=0.0; ver2=0.0; hor2=0.0; 

	x_tot=0.0; y_tot=0.0; z_tot=0.0;
	x_tot=0.0; y_tot=0.0; z_tot=0.0;

	x_scs1 = 0.0;
	y_scs1 = 0.0;
	z_scs1 = 0.0;
	x_scs2 = 0.0;
	y_scs2 = 0.0;
	z_scs2 = 0.0;
	x_scs_tot = 0.0;
	y_scs_tot = 0.0;
	z_scs_tot = 0.0;

	m_iAverageDistCount =0;
	m_dAveragedDistStdDev = 0.0;
	m_fPrismConst = 0.0;
	m_fPrismHeight = 0.0;
	m_dPPM = 0.0;
			
	//short m_iCount;
	//short m_iCount2;


    // Create a dialog
    //m_pMeasureDlg = new DCP::MeasureDialog(pMeasModel);  //lint !e1524 new in constructor for class 
    //(void)AddDialog( 0, m_pMeasureDlg, true );

    // Set the function key

	//poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
	
		
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

	
	

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::ScanMeasXYZController::~ScanMeasXYZController()
{

	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"Close MeasXYZController!");

	Log("ScanMeasXYZController::~ScanMeasXYZController");

	if(poHourGlass)
	{
		delete poHourGlass;
		poHourGlass = 0;
	}
}
// Description: Route model to everybody else
bool DCP::ScanMeasXYZController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     //return false;//m_pMeasureDlg->SetModel( pModel ); 220807
	return true;
	
  // Verify type
   // DCP::Model* pModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}


// Description: React on close of controller
void DCP::ScanMeasXYZController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	Log("ScanMeasXYZController::OnActiveControllerClosed", lCtrlID,lExitCode);
	//DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();

	// Handle averaging and 2 face measurement
	if(lCtrlID == DO_MEAS_XYZ_FACE1_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			m_iCount++;
			DCP::MeasXYZModel* pModel1 = (DCP::MeasXYZModel*) GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->GetModel();		
			
			dist_tot	+=	pModel1->m_dD;
			ver_tot		+=	pModel1->m_dV;
			hor_tot		+=	pModel1->m_dH;

			
			//DestroyController( lCtrlID );

			/*
			to_xyz(pModel->m_dDist,pModel->m_dHor,pModel->m_dVer, &x_temp,&y_temp,&z_temp,USE_TOOL);
			
			x_tot += x_temp;
			y_tot += y_temp;
			z_tot += z_temp;
			
			m_dDist_tot +=  pModel->m_dDist;
			m_dHor_tot +=  pModel->m_dHor;
			m_dVer_tot +=  pModel->m_dVer;
			*/
			/*
			if(poModel->m_nAverageCount > m_iCount)
			{	
				// activate measurement

				//DestroyController( lCtrlID );
				if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
				{
					(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );
				}
				(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
			}
			else if(poModel->m_nAverageCount == m_iCount)
			{
			*/
				/*
				x_new1 = x_tot /poModel->m_nAverageCount;
				y_new1 = y_tot /poModel->m_nAverageCount;
				z_new1 = z_tot /poModel->m_nAverageCount;
				*/
				dist1 = dist_tot / 1;
				hor1  = hor_tot / 1;
				ver1  = ver_tot / 1;
				
				to_xyz(dist1, hor1, ver1, &x_new1, &y_new1, &z_new1,m_iUseTool, &x_scs1, &y_scs1, &z_scs1);
				
				x_scs_tot = x_scs1;
				y_scs_tot = y_scs1;
				z_scs_tot = z_scs1;

					x_new = x_new1;
					y_new = y_new1;
					z_new = z_new1;

					DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();
					pModel->m_dX = x_new;
					pModel->m_dY = y_new;
					pModel->m_dZ = z_new;

					pModel->x_scs = x_scs_tot;
					pModel->y_scs = y_scs_tot;
					pModel->z_scs = z_scs_tot;

					Close(EC_KEY_CONT);
			//}
		}
		else
		{
			DestroyController( lCtrlID );
			Close(lExitCode);//160807
		}
	}
	else if(lCtrlID == CHANGE_FACE_CONTROLLER)
	{
		
		DestroyController( lCtrlID );
		if(lExitCode == EC_KEY_CONT)
		{
			if(poModel->m_n2FaceMeas == ALL_MANUAL || poModel->m_n2FaceMeas == DIST_MANUAL)
			{
				MsgBox msgbox;
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_AIM_TO_POINT_TOK);
				if(!msgbox.ShowMessageOkAbort(msg))
					Close(EC_KEY_ESC);
			}
				// start 2. face measurement
				if(GetController(DO_MEAS_XYZ_FACE2_CONTROLLER) == nullptr)
				{
					(void)AddController( DO_MEAS_XYZ_FACE2_CONTROLLER, new DCP::DoMeasXYZController );
				}
				(void)GetController( DO_MEAS_XYZ_FACE2_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE2_CONTROLLER, true);
		}
		else
			Close(lExitCode);//160807
	}
		// Handle averaging and 2 face measurement
	else if(lCtrlID == DO_MEAS_XYZ_FACE2_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			m_iCount2++;
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( DO_MEAS_XYZ_FACE2_CONTROLLER )->GetModel();		

			dist_tot2		+=	pModel->m_dD;
			ver_tot2		+=	pModel->m_dV;
			hor_tot2		+=	pModel->m_dH;

			//DestroyController( lCtrlID );//2007

			/*
			to_xyz(pModel->m_dDist,pModel->m_dHor,pModel->m_dVer, &x_temp,&y_temp,&z_temp,USE_TOOL);
			
			x_tot2 += x_temp;
			y_tot2 += y_temp;
			z_tot2 += z_temp;
			*/
			if(poModel->m_nAverageCount > m_iCount2)
			{
				//DestroyController( lCtrlID );
				if(GetController(DO_MEAS_XYZ_FACE2_CONTROLLER) == nullptr)
				{
					(void)AddController( DO_MEAS_XYZ_FACE2_CONTROLLER, new DCP::DoMeasXYZController );
				}
				(void)GetController( DO_MEAS_XYZ_FACE2_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE2_CONTROLLER, true);
			}
			else if(poModel->m_nAverageCount == m_iCount2)
			{
				dist2 = dist_tot2 / poModel->m_nAverageCount;
				hor2 = hor_tot2 / poModel->m_nAverageCount;
				ver2 = ver_tot2 / poModel->m_nAverageCount;
				/*
				x_new2 = x_tot2 /poModel->m_nAverageCount;
				y_new2 = y_tot2 /poModel->m_nAverageCount;
				z_new2 = z_tot2 /poModel->m_nAverageCount;
				*/
				// average all
				if(poModel->m_n2FaceMeas == ALL || poModel->m_n2FaceMeas == ALL_MANUAL)
				{
					to_xyz(dist2, hor2, ver2, &x_new2, &y_new2, &z_new2, m_iUseTool, &x_scs2, &y_scs2, &z_scs2);
					x_new = (x_new1 + x_new2) / 2;
					y_new = (y_new1 + y_new2) / 2;
					z_new = (z_new1 + z_new2) / 2;

					x_scs_tot = (x_scs1 + x_scs2) / 2;
					y_scs_tot = (y_scs1 + y_scs2) / 2;
					z_scs_tot = (z_scs1 + z_scs2) / 2;
				}
				// average dist only
				else
				{
					dist2 = (dist1 +dist2) / 2;
					hor2 = hor1;
					ver2 = ver1;
					to_xyz(dist2, hor2, ver2, &x_new2, &y_new2, &z_new2, m_iUseTool, &x_scs2, &y_scs2, &z_scs2);
					
					x_scs_tot = (x_scs1 + x_scs2) / 2;
					y_scs_tot = (y_scs1 + y_scs2) / 2;
					z_scs_tot = (z_scs1 + z_scs2) / 2;
				}
				
				x_new = x_new2;
				y_new = y_new2;
				z_new = z_new2;

				DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();
				pModel->m_dX = x_new;
				pModel->m_dY = y_new;
				pModel->m_dZ = z_new;

				pModel->x_scs = x_scs_tot;
				pModel->y_scs = y_scs_tot;
				pModel->z_scs = z_scs_tot;
		
	    		// save log
				if(poModel->m_nAmsLog == 1)
				{
						// save log
						if(	m_iUseTool ==1)
						{
							short iActivetool = poModel->active_tool;
							if(iActivetool)
							{
								sprintf(pModel->tool_name,"%-10.10s",poModel->tool_table[iActivetool-1].tool_id);
								m_pCommon->strbtrim(pModel->tool_name);
							}
						}

						AmsLog AmsLog(pModel, poModel);
				}


				// Change face to back.... I<>II
				TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
				poChangeFace->EnableMessageBox(true);
				GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER_AND_EXIT, poChangeFace);
				SetActiveController(CHANGE_FACE_CONTROLLER_AND_EXIT, true);
		

				//Close(EC_KEY_CONT);
			}
		}
		else
		{
			DestroyController( lCtrlID );
			Close(lExitCode);//160807
		}
	}
	else if(lCtrlID == CHANGE_FACE_CONTROLLER_AND_EXIT)
	{
		DestroyController( lCtrlID ); //2007
		Close(EC_KEY_CONT);
	}
	else if(lExitCode != EC_KEY_CONT)
	{
		if(TOOL_CONTROLLER != lCtrlID)
		{
			DestroyController( lCtrlID );//2007
			Close(lExitCode);
		}
	}
	//DestroyController( lCtrlID );
}

void DCP::ScanMeasXYZController::Run()
{
	Log("MeasXYZController::Run()");

	//DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();

	m_pCommon = new Common(poModel);
	
		poHourGlass->Continue();
		if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
		{
			Log("(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );");		
			(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoScanMeasXYZController );
		}
		(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
		SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
}
void DCP::ScanMeasXYZController::OnControllerActivated(void)
{

}
void DCP::ScanMeasXYZController::OnControllerClosed(int lExitCode)
{
	Log( "MeasXYZController::OnControllerClosed" );
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
	//	DeactivateMeasurement(lExitCode);
}

short DCP::ScanMeasXYZController::get_xyz_values(double* x, double* y, double* z)
{
	*x = x_new;
	*y = y_new;
	*z = z_new;

	return 1;
}
void DCP::ScanMeasXYZController::to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs)
{
	m_pCommon->to_xyz(dis, hor, ver, x, y,  z,  m_iUseTool, x_scs, y_scs, z_scs);
}


