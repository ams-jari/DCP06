// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
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
#include <dcp06/core/Logger.hpp>
#include <dcp06/init/Initialization.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
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

DCP::DoMeasXYZController::DoMeasXYZController()
    :TBL::MeasurementC(),poSurveyModel(0),poErrorHandler(0)
{
	DCP06_LOG_DEBUG("DoMeasXYZController::DoMeasXYZController");

	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    /*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);


	//poErrorHandler = new TBL::MeasErrorHandlerC();
	poErrorHandler = new MeasErrorHandler();
	//poErrorHandler->pOwnerController = this;

	 USER_APP_VERIFY( AddGuardController( DEFINE_MEAS_START_CONTROLLER, CreateMeasStartController() ) );
	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DoMeasXYZController::~DoMeasXYZController()
{
	DCP06_LOG_DEBUG("DoMeasXYZController::~DoMeasXYZController");
		if(poErrorHandler)
	{
		delete poErrorHandler;
		poErrorHandler = 0;
	}
	//MsgBox msgBox;
	//msgBox.ShowMessageOk(L"Close DoMeasXYZController");

}
// Description: Route model to everybody else
bool DCP::DoMeasXYZController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return false;//m_pMeasureDlg->SetModel( pModel );
	
  // Verify type
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

TBL::MeasErrorHandlerC::HandlingKindT DCP::MeasErrorHandler::HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor)
{

	// StopDist(); Removed 20122014

	//pOwnerController->Close(EC_KEY_ESC);
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgAborted;

	//20102104
	return TBL::MeasErrorHandlerC::HK_None;
	//return TBL::MeasErrorHandlerC::HK_ErrorMsgOk;
}

// Description: React on close of controller
void DCP::DoMeasXYZController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06_LOG_DEBUG("DoMeasXYZController::OnActiveControllerClosed ctrl=%d exit=%d",lCtrlID,lExitCode);
	// Call base class
    /*GUI::*/ControllerC::OnControllerClosed(lExitCode);
	
	DestroyController( lCtrlID );
}

void DCP::DoMeasXYZController::OnControllerActivated(void)
{
	ControllerC::OnControllerActivated();

	DCP06_LOG_DEBUG("DoMeasXYZController::OnControllerActivated / ActivateMeasurement");

	poErrorHandler->Attach();
	
	ActivateMeasurement();
	
	//OnF1Pressed();

	DCP06_LOG_DEBUG("DoMeasXYZController /ExecuteAll");
	TBL::MeasurementC::ExecuteAll();
}
void DCP::DoMeasXYZController::OnControllerClosed(int lExitCode)
{
	DCP06_LOG_DEBUG("DoMeasXYZController::OnControllerClosed exit=%d",lExitCode);
	//MsgBox msgBox;
	//msgBox.ShowMessageOk(L"DoMeasXYZController::OnControllerClosed");

	poErrorHandler->Detach();

	DeactivateMeasurement(lExitCode);
}

void DCP::DoMeasXYZController::OnPeriodicInclineValidation(int ulParam1, int ulParam2)
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
	DCP06_LOG_DEBUG("DoMeasXYZController::OnPeriodicInclineValidation p1=%u p2=%u",ulParam1,ulParam2);
	*/
	
}


void DCP::DoMeasXYZController::OnFinish()
{
	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"Finish");		
}

/*
 void DCP::DoMeasXYZController::OnOperationEvent(int unNotifyCode,  int ulOperationId)
 {
		//TBL::MeasurementC::OnOperationEvent(unNotifyCode,ulOperationId);
		//char temp[100];
		//sprintf(temp,"%d",unNotifyCode );
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(StringC(temp));		

 }
*/
/*
void DCP::DoMeasXYZController::OnF1Pressed()
{
	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"F1Press");	
	Close(EC_KEY_ESC);

}
bool DCP::DoMeasXYZController::OnKeyPress(wchar_t cKey, bool bDown)
{
	MsgBox MsgBox;
	MsgBox.ShowMessageOk(L"KeyPress");		
	return false;
}
*/
void  DCP::DoMeasXYZController::OnStopDistEvent(int unParam1,  int ulParam2)
 {

	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"OnStopDistEvent");
	Close(EC_KEY_ESC);
 }

// Samalainen sitten searchille
void DCP::DoMeasXYZController::OnOperationDistEvent(int unNotifyCode,  int ulOperationId)
{
	double dSlopeDist=0.0,dH=0.0,dV=0.0;
	
	// call base class
	TBL::MeasurementC::OnOperationDistEvent(unNotifyCode,ulOperationId);

	if(unNotifyCode == TBL::DistanceMeasProcedureC::NC_ON_START)
	{
		DCP06_LOG_DEBUG("DoMeasXYZController::OnOperationDistEvent NC_ON_START");

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
		DCP06_LOG_DEBUG("DoMeasXYZController::OnOperationDistEvent NC_SUCCESS");
		Close(EC_KEY_CONT); 
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_STOP)
	{
		DCP06_LOG_DEBUG("DoMeasXYZController::OnOperationDistEvent NC ON STOP");
		//MsgBox MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON STOP");
		Close(EC_KEY_ESC);
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_FAIL)
	{
		DCP06_LOG_DEBUG("DoMeasXYZController::OnOperationDistEvent NC_ON_FAIL");
		
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

void DCP::DoMeasXYZController::OnOperationSearchEvent(int unNotifyCode, int ulOperationId)
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


short DCP::DoMeasXYZController::get_xyz_values(double* x, double* y, double* z)
{
	*x = m_dX;
	*y = m_dY;
	*z = m_dZ;

	return 1;
}
// ======================================================================================== 
DCP::MeasXYZController::MeasXYZController(DCP::Model* pModel)
    :TBL::MeasurementC(),poSurveyModel(0), poModel(pModel),m_iCount(0),m_iCount2(0),m_iUseTool(0),poHourGlass(0)
{
	DCP06_LOG_DEBUG("MeasXYZController::MeasXYZController");

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

	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
	
		
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

	
	

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::MeasXYZController::~MeasXYZController()
{

	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"Close MeasXYZController!");

	DCP06_LOG_DEBUG("MeasXYZController::~MeasXYZController");

	if(poHourGlass)
	{
		delete poHourGlass;
		poHourGlass = 0;
	}
}
// Description: Route model to everybody else
bool DCP::MeasXYZController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     //return false;//m_pMeasureDlg->SetModel( pModel ); 220807
	return true;
	
  // Verify type
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
void DCP::MeasXYZController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06_LOG_DEBUG("MeasXYZController::OnActiveControllerClosed ctrl=%d exit=%d", lCtrlID,lExitCode);
	DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();

	if(lCtrlID == TOOL_CONTROLLER)
	{	
		if(lExitCode == EC_KEY_CONT)
		{
			if(poModel->active_tool > 0 && pModel->tooli == 1)
				m_iUseTool = 1;

			// set target type.....
		}
		poHourGlass->Continue();

		DestroyController( lCtrlID );

		if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
		{
			(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );
		}
		(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
		SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
	}

	// Handle averaging and 2 face measurement
	if(lCtrlID == DO_MEAS_XYZ_FACE1_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			m_iCount++;
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->GetModel();		
			
			dist_tot	+=	pModel->m_dD;
			ver_tot		+=	pModel->m_dV;
			hor_tot		+=	pModel->m_dH;

			
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
				/*
				x_new1 = x_tot /poModel->m_nAverageCount;
				y_new1 = y_tot /poModel->m_nAverageCount;
				z_new1 = z_tot /poModel->m_nAverageCount;
				*/
				dist1 = dist_tot / poModel->m_nAverageCount;
				hor1  = hor_tot / poModel->m_nAverageCount;
				ver1  = ver_tot / poModel->m_nAverageCount;
				
				to_xyz(dist1, hor1, ver1, &x_new1, &y_new1, &z_new1,m_iUseTool, &x_scs1, &y_scs1, &z_scs1);
				
				x_scs_tot = x_scs1;
				y_scs_tot = y_scs1;
				z_scs_tot = z_scs1;

				// maybe change face
				if(poModel->m_n2FaceMeas != NO)
				{
					// I<>II
					TBL::ChangeFaceControllerC* poChangeFace = new TBL::ChangeFaceControllerC();
					poChangeFace->EnableMessageBox(true);
					GUI::ControllerC::AddController(CHANGE_FACE_CONTROLLER, poChangeFace);
					SetActiveController(CHANGE_FACE_CONTROLLER, true);
				}
				else
				{
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
				
					if(poModel->m_nAmsLog == 1)
					{
						// save log
						if(	m_iUseTool ==1)
						{
							short iActivetool = poModel->active_tool;
							if(iActivetool)
							{
								sprintf(pModel->tool_name,"%-10.10s\0",poModel->tool_table[iActivetool-1].tool_id);
								m_pCommon->strbtrim(pModel->tool_name);

							}
						}

						AmsLog AmsLog(pModel, poModel);
					}
					Close(EC_KEY_CONT);
				}
			}
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

void DCP::MeasXYZController::Run()
{
	DCP06_LOG_DEBUG("MeasXYZController::Run()");

	DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*)GetModel();

	m_pCommon = new Common(poModel);
	
	// n�yt� tool
	if(pModel->tooli == 1 || pModel->tooli == 10)
	{
		if(pModel->tooli == 10 || poModel->active_tool > 0)
		{
			
			if(poModel->m_nToolInfo == 1)
			{
				//poHourGlass->Pause();
				if(GetController(TOOL_CONTROLLER) == nullptr)
				{
					(void)AddController( TOOL_CONTROLLER, new DCP::ToolController());
				}
				(void)GetController( TOOL_CONTROLLER )->SetModel( poModel);
				SetActiveController(TOOL_CONTROLLER, true);
			}
			else
			{
				m_iUseTool = 1; // use tool but do not show dialog
				
				poHourGlass->Continue();

				if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
				{
					(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );
				}
				(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
			}

		}
		else
		{
			poHourGlass->Continue();

			if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
			{
				(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );
			}
			(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
			SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
		}
	}
	// tai k�ynnist� mittaus
	else
	{
		poHourGlass->Continue();
		if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == nullptr)
		{
			DCP06_LOG_DEBUG("(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );");		
			(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );
		}
		(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
		SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
	}
}
void DCP::MeasXYZController::OnControllerActivated(void)
{

}
void DCP::MeasXYZController::OnControllerClosed(int lExitCode)
{
	DCP06_LOG_DEBUG( "MeasXYZController::OnControllerClosed" );
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
	//	DeactivateMeasurement(lExitCode);
}

short DCP::MeasXYZController::get_xyz_values(double* x, double* y, double* z)
{
	*x = x_new;
	*y = y_new;
	*z = z_new;

	return 1;
}
void DCP::MeasXYZController::to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs)
{
	m_pCommon->to_xyz(dis, hor, ver, x, y,  z,  m_iUseTool, x_scs, y_scs, z_scs);
}


DCP::DCPSurveyModelC::DCPSurveyModelC(/*HEWConfigModelC* poConfigModel*/) :
//m_poConfigModel(poConfigModel),
m_dDeltaHt(0.0)
{
    // The config model is multiple used!
  //  (void)m_poConfigModel->IncrementRefCount();
}

DCP::DCPSurveyModelC::~DCPSurveyModelC()
{
    // The config model is multiple used!
    //(void)m_poConfigModel->DecrementRefCount();
} //lint !e1740 pointer member 'm_poConfigModel' not directly freed or zero'ed by destructor

// --------------------------------------------------------------------------------------------------------------------
//
// GetConfigModel
//
/*
DCP::DCPSurveyModelC* HEW::HEWSurveyModelC::GetConfigModel(void)
{
    return m_poConfigModel;
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// GetDeltaHt
// 
double DCP::DCPSurveyModelC::GetDeltaHt(void) const
{
    return m_dDeltaHt;
}

// --------------------------------------------------------------------------------------------------------------------
//
// SetDeltaHt
// 
void DCP::DCPSurveyModelC::SetDeltaHt(double dDeltaHt)
{
    m_dDeltaHt = dDeltaHt;
}



// ===========================================================================================
// 
// ===========================================================================================
// Instantiate template classes
DCP::MeasXYZModel::MeasXYZModel()
{
	tooli = 0;
	m_dX = 0.0;
	m_dY = 0.0;
	m_dZ = 0.0;
	m_dD = 0.0;
	m_dH = 0.0;
	m_dV = 0.0;

	memset(&sPointId,0,POINT_ID_BUFF_LEN);
	
	m_iAverageDistCount =0;
	m_dAveragedDistStdDev = 0.0;
	m_fPrismConst = 0.0;
	m_fPrismHeight = 0.0;
	m_dPPM = 0.0;
	x_scs = 0.0;
	y_scs = 0.0;
	z_scs = 0.0;
	tool_name[0] = '\0';
}

DCP::MeasXYZModel::~MeasXYZModel()
{
}



DCP::Log::Log(char* sMsg):m_pFile(0)
{
	#ifdef AMS_DEBUG
	char m_cPath[CPI::LEN_PATH_MAX];
	char temp[CPI::LEN_PATH_MAX];

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::deviceSdCard/*CPI::deviceSdCard*//*devicePcCard*/, CPI::ftUserAscii, m_cPath);
	strcat(m_cPath,"ams.log");
	
	if((m_pFile = fopen(m_cPath, "rb+")) == 0)
	{
		m_pFile = fopen(m_cPath,"wb+");
	}
	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		sprintf(temp,"%s%c%c",sMsg,13,10);
		fputs(temp, m_pFile);
		fflush(m_pFile);
		fclose(m_pFile);
	}
	#endif
}

DCP::Log::Log(char* sMsg, int ret):m_pFile(0)
{
	#ifdef AMS_DEBUG
	char m_cPath[CPI::LEN_PATH_MAX];
	char temp[CPI::LEN_PATH_MAX];

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::deviceSdCard/*CPI::deviceSdCard*//*devicePcCard*/, CPI::ftUserAscii, m_cPath);
	strcat(m_cPath,"ams.log");
	
	if((m_pFile = fopen(m_cPath, "rb+")) == 0)
	{
		m_pFile = fopen(m_cPath,"wb+");
	}
	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		sprintf(temp,"%s (%d)%c%c",sMsg, ret, 13,10);	
		fputs(temp, m_pFile);
		fflush(m_pFile);
		fclose(m_pFile);
	}
   #endif
}

DCP::Log::Log(char* sMsg, int ctrl, int exit_code):m_pFile(0)
{
	#ifdef AMS_DEBUG
	char m_cPath[CPI::LEN_PATH_MAX];
	char temp[CPI::LEN_PATH_MAX];

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::deviceSdCard/*CPI::deviceSdCard*//*devicePcCard*/, CPI::ftUserAscii, m_cPath);
	strcat(m_cPath,"ams.log");
	
	if((m_pFile = fopen(m_cPath, "rb+")) == 0)
	{
		m_pFile = fopen(m_cPath,"wb+");
	}
	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		sprintf(temp,"%s (ctrl=%d)(exitcode=%d)%c%c",sMsg, ctrl, exit_code, 13,10);	
		fputs(temp, m_pFile);
		fflush(m_pFile);
		fclose(m_pFile);
	}
	#endif
}

DCP::Log::~Log()
{

}




DCP::AmsLog::AmsLog(MeasXYZModel* pMeasModel, Model* pModel):m_pFile(0), m_pModel(pModel)
{
	char m_cPath[CPI::LEN_PATH_MAX];
	char logFilenameBuffer[STRING_BUFFER_PATH];
	char logLineBuffer[STRING_BUFFER_PATH];
	short iNo = 1;
	int iInstNo = 0;
	char temp_hz[20];
	char temp_v[20];
	char temp_dist[20];
	char temp_x[20];
	char temp_y[20];
	char temp_z[20];
	char temp_ppm[20];
	char temp_prism[20];

	TPI::MeasConfigC oMeasConfig;
	TPI::CorrectionC oCorrectionData;

	TPI::InstrumentInfoC oInstrumentInfo;
	
	TPI::SensorC::Instance()->GetInstrumentInfo(oInstrumentInfo);

	const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentSerialNum();
	const char* cInstrumentType = oInstrumentInfo.GetInstrumentType();
	
	//MsgBox msgbox;
	//msgbox.ShowMessageOk(StringC(cInstrumentNumber));

	iInstNo = atoi(cInstrumentNumber);

	// date and time
	//DateTime oDateTime = GUI::EditTimeCtrlC::GetActualTime();
	DateTime oDateTime = GUI::GetActualTime();
	
	int year, day, month;
	int hour, min, sec;
	GMAT::SetMjd( (double)oDateTime, year, month, day, hour, min, sec );

	// compensator status
	oMeasConfig.Get();
	
	//3.x
	//bool bComp = oMeasConfig.GetCompensatorMode();
	bool bComp = oMeasConfig.IsCompensatorEnabled();
	bool bHzCorr =oMeasConfig.GetHzCorrectionMode();

	//prism const
	float fRef = oMeasConfig.GetReflectorConstant();

	oCorrectionData.Get();
	float fPPM = oCorrectionData.GetAtmoshericPPM();
	//sprintf(buff,"%+.1f   %8.4f",fRef * 1000, fPPM);
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pModel->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	//strcat(m_cPath,"ams.log");

	sprintf(logFilenameBuffer,"L_%02d%02d%02d.log\0", day, month, year-2000);
	strcat(m_cPath,logFilenameBuffer);

	if((m_pFile = fopen(m_cPath, "rb+")) == 0)
	{
		m_pFile = fopen(m_cPath,"wb+");
	}
	else
	{
		int offset = -100;
		char buff[501];

		fseek(m_pFile,0,SEEK_SET);
		int iStart = ftell(m_pFile);
		fseek(m_pFile,0,SEEK_END); 
		int iEnd = ftell(m_pFile);
		bool bFound = false;
		// read last pointno
		for(short j = iEnd-1; j >= iStart; j--)
		{
			fseek(m_pFile, j,SEEK_SET);
			if(fgets(buff,2,m_pFile) != 0)
			{
				if(buff[0] == 13 || buff[0] == 10)
				{
					if( (iEnd -j) > 10)
					{
						fgets(buff,500,m_pFile);
						if(buff[0] == '1' && buff[1] == '1')
						{
							char pointNoBuffer[10];
							sprintf(pointNoBuffer,"%4.4s",(buff +2));
							iNo = atoi(pointNoBuffer);
							iNo++;
							bFound = true;
							break;
						}
					}
				}
			}
			else
			{	

				break;
			}
		}
		if(!bFound)
		{
			fseek(m_pFile,0,SEEK_SET);
			if(fgets(buff,500,m_pFile) != 0)
			{
				if(buff[0] == '1' && buff[1] == '1')
				{
					char pointNoBuffer[10];
					sprintf(pointNoBuffer,"%4.4s",(buff +2));
					iNo = atoi(pointNoBuffer);
					iNo++;
				}
			}
		}
	}

	
	if(fRef < 0)
	{
		fRef = -fRef;
		sprintf(temp_prism +1,"%08.08d", (int)(fRef * 10000));	
		temp_prism[0] = '-';
	}
	else
	{
		sprintf(temp_prism +1,"%08.08d", (int)(fRef * 10000));
		temp_prism[0] = '+';
	}

	
	if(fPPM < 0)
	{
		fPPM = -fPPM;
		sprintf(temp_ppm +1,"%08.08d", (int) (fPPM * 10000));
		temp_ppm[0] = '-';
	}
	else
	{
		sprintf(temp_ppm +1,"%08.08d", (int)(fPPM * 10000));	
		temp_ppm[0] = '+';
	}


	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		//sprintf(temp,"11%04d+%08.8s 12....+%08.8d 13....%08.8s 19....+%02d%02d%02d%02d 21.%c%c%c%09.9s 22.%c%c%c%09.9s 31..0%10.10s 58..16%9.9s 59..16%9.9s 72....+%08.8s 81..0%10.10s 82..0%10.10s 83..0%10.10s %c%c", iNo,
		sprintf(logLineBuffer,"11%04d+%08.8s 12....+%08.8d 13....+%08.8s 19....+%02d%02d%02d%02d 21.%c%c%c%09.9s 22.%c%c%c%09.9s 31..0%10.10s 58..16%9.9s 59..16%9.9s 72....+%08.8s 81..0%10.10s 82..0%10.10s 83..0%10.10s %c%c", iNo,
																											pMeasModel->sPointId,	//11
																								  			iInstNo,			//12	
																											cInstrumentType,	//13	
																											month,day,hour,min, //19
																											bComp ? '3' : '0',	//21
																											bHzCorr ? '2' : '3',//21
																											'3' , // alwayd 360 dec ?? //21
																											remove_dot(pMeasModel->m_dH * DPR, temp_hz), //21
																											bComp ? '3' : '0',	//22
																											bHzCorr ? '2' : '3', //22
																											'3' , // alwayd 360 dec ?? //22
																											remove_dot(pMeasModel->m_dV *DPR, temp_v), //22
																											get_distance(pMeasModel->m_dD, temp_dist, m_pModel), //31
																											temp_prism,//(short) (fRef * 10000.0), // 1/10mm	//58
																											temp_ppm,//(short) (fPPM * 1000.0),	//59
																											pMeasModel->tool_name, // 72 TOOL name
																											get_xyz_distance(pMeasModel->y_scs, temp_x, m_pModel),	// 82=x
																											get_xyz_distance(pMeasModel->x_scs, temp_y, m_pModel),	// 81=y
																											get_xyz_distance(pMeasModel->z_scs, temp_z, m_pModel),	// 83=z



			
																											13,10);
		fputs(logLineBuffer, m_pFile);
		fflush(m_pFile);
		fclose(m_pFile);
	}
}

DCP::AmsLog::~AmsLog()
{

}


char* DCP::AmsLog::remove_dot(double dAngle, char* dest)
{
//char temp[100];

	

	//double angle = atof(source);
	char sign;
	if(dAngle < 0.0)
	{
		sign = '-';
		dAngle = -dAngle;
	}
	else
	{
		sign = '+';
	}
	
    char buff2[20];
	buff2[0] = sign;
	sprintf(buff2+1,"%9.5f",dAngle);

    char *ptr = buff2;
    // loop
    while(*ptr)
    {
        if(*ptr == ' ')
            *ptr = '0';
        ptr++;
    }

   // and
    int pit = (int) strlen(buff2);
	
    int pit2=0;
    for(int i=0;i<pit;i++)
    {
        if(buff2[i] != '.')
        {
            dest[pit2] = buff2[i];
            pit2++;
        }
    }
    dest[pit2] = '\0';
    
    return dest;
}


// dDist is in meter like 9.25678m
char* DCP::AmsLog::get_distance(double Dist, char* dest, Model* pModel)
{
	//Dist is in meter
	char sign;
	char unit_char;
	double dDist = Dist;
	short dec = pModel->m_nDecimals;
	short dist_unit = pModel->m_nUnit;
	
	// to mm
	dDist = dDist * 1000;
	
	if(dDist < 0.0)
	{
		sign =  '-';
		dDist = -dDist;
	}
	else
	{
		sign =  '+';
	}
	if(dist_unit == MM)  
	{
		dDist = dDist / 1000.0;

		if(dec == 1) // 1/10mm
		{
			unit_char = '6';
			dec = 4;
		}
		else //if(dec == 2) 1 / 100 mm
		{
			unit_char = '8';
			dec = 5;
		}
	}

	// convert to mm
	else if(dist_unit == INCH)
	{
		dDist = dDist / 1000.0;
		if(dec == 2)
		{
			dec = 4;
			unit_char = '6';
		}
		else // dec == 3)
		{
			dec = 5;
			unit_char = '8';
		}
	}
	else // FEET
	{
		dDist = dDist / 304.800612;
		if(dec == 3)
		{
			unit_char = '1';
			dec = 3;
		}
		else // dec == 4)
		{
			unit_char = '7';
			dec = 4;
		}
	}


    char buff2[20];
	buff2[0] = unit_char;
	buff2[1] = sign;
	sprintf(buff2+2,"%9.*f",dec,dDist);

    char *ptr = buff2;
    // loop
    while(*ptr)
    {
        if(*ptr == ' ')
            *ptr = '0';
        ptr++;
    }

   // and copy and remove '.'
    int pit = (int) strlen(buff2);
	
    int pit2=0;
    for(int i=0;i<pit;i++)
    {
        if(buff2[i] != '.')
        {
            dest[pit2] = buff2[i];
            pit2++;
        }
    }
    dest[pit2] = '\0';

    return dest;
}

char* DCP::AmsLog::get_xyz_distance(double Dist, char* dest, Model* pModel)
{
	
	char sign;
	char unit_char;
	double dDist = Dist;
	short dec = pModel->m_nDecimals;
	short dist_unit = pModel->m_nUnit;

	if(dDist < 0.0)
	{
		sign =  '-';
		dDist = -dDist;
	}
	else
	{
		sign =  '+';
	}
	if(dist_unit == MM)  
	{
		dDist = dDist / 1000.0;

		if(dec == 1) // 1/10mm
		{
			unit_char = '6';
			dec = 4;
		}
		else //if(dec == 2) 1 / 100 mm
		{
			unit_char = '8';
			dec = 5;
		}
	}

	// convert to meter
	else if(dist_unit == INCH)
	{
		dDist = dDist / 1000.0;

		if(dec == 2)
		{
			unit_char = '6';
			dec = 4;
		}
		else // dec == 3)
		{
			dec = 5;
			unit_char = '8';
		}
	}
	else // FEET
	{
		dDist = dDist / 304.800612;
		if(dec == 3)
		{
			unit_char = '1';
			dec = 3;
		}
		else // dec == 4)
		{
			unit_char = '7';
			dec = 4;
		}
	}
    char buff2[20];
	buff2[0] = unit_char;
	buff2[1] = sign;
	sprintf(buff2+2,"%9.*f",dec,dDist);

    char *ptr = buff2;
    // loop
    while(*ptr)
    {
        if(*ptr == ' ')
            *ptr = '0';
        ptr++;
    }

   // and copy and remove '.'
    int pit = (int) strlen(buff2);
	
    int pit2=0;
    for(int i=0;i<pit;i++)
    {
        if(buff2[i] != '.')
        {
            dest[pit2] = buff2[i];
            pit2++;
        }
    }
    dest[pit2] = '\0';

    return dest;
}

// ================================================================================================
// Description: AmsToolLogC
// ================================================================================================
DCP::AmsToolLog::AmsToolLog(char* pFileName, char* pointid, Common* pCommon, Model* pModel):m_pFile(0) 
{
	char m_cPath[CPI::LEN_PATH_MAX];
	char outputBuffer[STRING_BUFFER_PATH];
	char pathBuffer[STRING_BUFFER_PATH];
	char *ptr;
	short iDec;
	short iActiveTool = 0;
	char toolname[20];
	
	if(!pModel->m_nSaveTool)
		return;

	iActiveTool = pModel->active_tool;

	if(iActiveTool == 0)
		return;

	// get disk free space
	if(pCommon->check_free_space(30000L) == false)
		return;

	sprintf(pathBuffer,"%-s", pFileName);
	ptr = strchr(pathBuffer,'.');
		
	if(ptr != nullptr)
	{
		*ptr = '\0';
		sprintf(outputBuffer,"%s.tof",pathBuffer);
	}
	
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(pModel->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	strcat(m_cPath,outputBuffer);
	
	if((m_pFile = fopen(m_cPath, "rb+")) == 0)
	{
		m_pFile = fopen(m_cPath,"wb+");
		if(m_pFile)
		{
			sprintf(outputBuffer,"%-6.6s %8.8s %9.9s %9.9s %9.9s %9.9s %9.9s %9.9s %9.9s %9.9s %9.9s%c%c", "PID", "TOOL ID" ,
																							"TOOL X", "TOOL Y", "TOOL Z",
																							"TRANS X", "TRANS Y", "TRANS Z",
																							"TOT X", "TOT Y", "TOT Z",
																							13,10);
			fputs(outputBuffer, m_pFile);
			fflush(m_pFile);
		}
	}
	
	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		iDec = pModel->m_nDecimals;
		sprintf(toolname,"%-s",pModel->tool_table[iActiveTool-1].tool_id);
		pCommon->strbtrim(toolname);
		sprintf(outputBuffer,"%-6.6s %8.8s %9.*f %9.*f %9.*f %9.*f %9.*f %9.*f %9.*f %9.*f %9.*f%c%c", pointid, toolname,
																							   iDec, pModel->tool_table[iActiveTool-1].x,
																							   iDec, pModel->tool_table[iActiveTool-1].y,
																							   iDec, pModel->tool_table[iActiveTool-1].z,
																							   iDec, pModel->tool_trans_x,
																							   iDec, pModel->tool_trans_y,
																							   iDec, pModel->tool_trans_z,
																							   iDec, pModel->active_tool_x,
																							   iDec, pModel->active_tool_y,
																							   iDec, pModel->active_tool_z,
																							   13,10);

		
		fputs(outputBuffer, m_pFile);
		fflush(m_pFile);
		fclose(m_pFile);
	}
}

DCP::AmsToolLog::~AmsToolLog()
{

}
