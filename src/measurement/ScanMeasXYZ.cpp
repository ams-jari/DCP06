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
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06MeasDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC

DCP::DCP06DoScanMeasXYZControllerC::DCP06DoScanMeasXYZControllerC()
    :TBL::MeasurementC(),poSurveyModel(0),poErrorHandler(0)
{
	DCP06Log("DCP06DoScanMeasXYZControllerC::DCP06DoScanMeasXYZControllerC");

	poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    /*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);


	//poErrorHandler = new TBL::MeasErrorHandlerC();
	poErrorHandler = new DCP06ScanErrorHandlerC();
	//poErrorHandler->pOwnerController = this;

	 USER_APP_VERIFY( AddGuardController( DEFINE_MEAS_START_CONTROLLER, CreateMeasStartController() ) );
	
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06DoScanMeasXYZControllerC::~DCP06DoScanMeasXYZControllerC()
{
	DCP06Log("DCP06DoScanMeasXYZControllerC::~DCP06DoScanMeasXYZControllerC");
		if(poErrorHandler)
	{
		delete poErrorHandler;
		poErrorHandler = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP06DoScanMeasXYZControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return false;//m_pDCP06MeasDlg->SetModel( pModel );
}

TBL::MeasErrorHandlerC::HandlingKindT DCP::DCP06ScanErrorHandlerC::HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor)
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
void DCP::DCP06DoScanMeasXYZControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06Log("DCP06DoScanMeasXYZControllerC::OnActiveControllerClosed",lCtrlID,lExitCode);
	// Call base class
    /*GUI::*/ControllerC::OnControllerClosed(lExitCode);
	
	DestroyController( lCtrlID );
}

void DCP::DCP06DoScanMeasXYZControllerC::OnControllerActivated(void)
{
	ControllerC::OnControllerActivated();

	DCP06Log("DCP06DoScanMeasXYZControllerC::OnControllerActivated / ActivateMeasurement");

	poErrorHandler->Attach();
	
	ActivateMeasurement();
	
	//OnF1Pressed();

	DCP06Log("DCP06DoScanMeasXYZControllerC /ExecuteAll");
	TBL::MeasurementC::ExecuteAll();
}
void DCP::DCP06DoScanMeasXYZControllerC::OnControllerClosed(int lExitCode)
{
	DCP06Log("DCP06DoScanMeasXYZControllerC::OnControllerClosed",lExitCode);
	//DCP06MsgBoxC msgBox;
	//msgBox.ShowMessageOk(L"DCP06DoMeasXYZControllerC::OnControllerClosed");

	poErrorHandler->Detach();

	DeactivateMeasurement(lExitCode);
}

void DCP::DCP06DoScanMeasXYZControllerC::OnPeriodicInclineValidation(int ulParam1, int ulParam2)
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


void DCP::DCP06DoScanMeasXYZControllerC::OnFinish()
{
	//DCP06MsgBoxC MsgBox;
	//MsgBox.ShowMessageOk(L"Finish");		
}

void  DCP::DCP06DoScanMeasXYZControllerC::OnStopDistEvent(int unParam1,  int ulParam2)
 {
	Close(EC_KEY_ESC);
 }

// Samalainen sitten searchille
void DCP::DCP06DoScanMeasXYZControllerC::OnOperationDistEvent(int unNotifyCode,  int ulOperationId)
{
	double dSlopeDist=0.0,dH=0.0,dV=0.0;
	
	// call base class
	TBL::MeasurementC::OnOperationDistEvent(unNotifyCode,ulOperationId);

	if(unNotifyCode == TBL::DistanceMeasProcedureC::NC_ON_START)
	{
		DCP06Log("DCP06DoScanMeasXYZControllerC::OnOperationDistEvent NC_ON_START");

		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON START");
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_SUCCESS)
	{
		SetMeasData(GetDistanceProc()->GetMeasDataHandle()); // MUISTA T�M�
		dSlopeDist = /*GetModel()*/poSurveyModel->GetMeas().GetSlopeDistance();
		dH = poSurveyModel->GetMeas().GetHorizontalAngle();
		dV = poSurveyModel->GetMeas().GetVerticalAngle();

		DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*) GetModel();

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

		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"NC_SUCCESS");
		DCP06Log("DCP06DoScanMeasXYZControllerC::OnOperationDistEvent NC_SUCCESS");
		Close(EC_KEY_CONT); 
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_STOP)
	{
		DCP06Log("DCP06DoScanMeasXYZControllerC::OnOperationDistEvent NC ON STOP");
		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"NC ON STOP");
		Close(EC_KEY_ESC);
	}
	else if(unNotifyCode ==TBL::DistanceMeasProcedureC::NC_ON_FAIL)
	{
		DCP06Log("DCP06DoScanMeasXYZControllerC::OnOperationDistEvent NC_ON_FAIL");
		
		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"Error measurement!");
		Close(EC_KEY_ESC);
	}
	else
	{
		//DCP06MsgBoxC MsgBox;
		//MsgBox.ShowMessageOk(L"Jokin muu");
		Close(EC_KEY_ESC);
	}
}

void DCP::DCP06DoScanMeasXYZControllerC::OnOperationSearchEvent(int unNotifyCode, int ulOperationId)
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


short DCP::DCP06DoScanMeasXYZControllerC::get_xyz_values(double* x, double* y, double* z)
{
	*x = m_dX;
	*y = m_dY;
	*z = m_dZ;

	return 1;
}
// ======================================================================================== 
DCP::DCP06ScanMeasXYZControllerC::DCP06ScanMeasXYZControllerC(DCP::DCP06ModelC* pDCP06Model)
    :TBL::MeasurementC(),poSurveyModel(0), poDCP06Model(pDCP06Model),m_iCount(0),m_iCount2(0),m_iUseTool(0),poHourGlass(0)
{
	DCP06Log("DCP06MeasXYZControllerC::DCP06MeasXYZControllerC");

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
    //m_pDCP06MeasDlg = new DCP::DCP06MeasDlgC(pMeasModel);  //lint !e1524 new in constructor for class 
    //(void)AddDialog( 0, m_pDCP06MeasDlg, true );

    // Set the function key

	//poSurveyModel = new DCPSurveyModelC(/*m_poConfigModel*/);
	//TPI::MeasDataC oMesData(poSurveyModel->GetMeas());
	//oMesData.SetAveragedDistCount(4);
	
	
		
    // Set model in controller and measurement class
    // removed namespaces for eVC compability (WinCE Compiler)
    //USER_APP_VERIFY(/*GUI::*/ModelHandlerC::SetModel(poSurveyModel));
    ///*TBL::*/MeasurementC::SetSurveyModel(poSurveyModel);

	
	

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06ScanMeasXYZControllerC::~DCP06ScanMeasXYZControllerC()
{

	//DCP06MsgBoxC MsgBox;
	//MsgBox.ShowMessageOk(L"Close DCP06MeasXYZControllerC!");

	DCP06Log("DCP06ScanMeasXYZControllerC::~DCP06ScanMeasXYZControllerC");

	if(poHourGlass)
	{
		delete poHourGlass;
		poHourGlass = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP06ScanMeasXYZControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     //return false;//m_pDCP06MeasDlg->SetModel( pModel ); 220807
	return true;
	
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
void DCP::DCP06ScanMeasXYZControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06Log("DCP06ScanMeasXYZControllerC::OnActiveControllerClosed", lCtrlID,lExitCode);
	//DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*)GetModel();

	// Handle averaging and 2 face measurement
	if(lCtrlID == DO_MEAS_XYZ_FACE1_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			m_iCount++;
			DCP::DCP06MeasXYZModelC* pModel1 = (DCP::DCP06MeasXYZModelC*) GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->GetModel();		
			
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
			if(poDCP06Model->m_nAverageCount > m_iCount)
			{	
				// activate measurement

				//DestroyController( lCtrlID );
				if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == NULL)
				{
					(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DCP06DoMeasXYZControllerC );
				}
				(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
			}
			else if(poDCP06Model->m_nAverageCount == m_iCount)
			{
			*/
				/*
				x_new1 = x_tot /poDCP06Model->m_nAverageCount;
				y_new1 = y_tot /poDCP06Model->m_nAverageCount;
				z_new1 = z_tot /poDCP06Model->m_nAverageCount;
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

					DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*)GetModel();
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
			if(poDCP06Model->m_n2FaceMeas == ALL_MANUAL || poDCP06Model->m_n2FaceMeas == DIST_MANUAL)
			{
				DCP06MsgBoxC msgbox;
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_AIM_TO_POINT_TOK);
				if(!msgbox.ShowMessageOkAbort(msg))
					Close(EC_KEY_ESC);
			}
				// start 2. face measurement
				if(GetController(DO_MEAS_XYZ_FACE2_CONTROLLER) == NULL)
				{
					(void)AddController( DO_MEAS_XYZ_FACE2_CONTROLLER, new DCP::DCP06DoMeasXYZControllerC );
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
			DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*) GetController( DO_MEAS_XYZ_FACE2_CONTROLLER )->GetModel();		

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
			if(poDCP06Model->m_nAverageCount > m_iCount2)
			{
				//DestroyController( lCtrlID );
				if(GetController(DO_MEAS_XYZ_FACE2_CONTROLLER) == NULL)
				{
					(void)AddController( DO_MEAS_XYZ_FACE2_CONTROLLER, new DCP::DCP06DoMeasXYZControllerC );
				}
				(void)GetController( DO_MEAS_XYZ_FACE2_CONTROLLER )->SetModel(GetModel());
				SetActiveController(DO_MEAS_XYZ_FACE2_CONTROLLER, true);
			}
			else if(poDCP06Model->m_nAverageCount == m_iCount2)
			{
				dist2 = dist_tot2 / poDCP06Model->m_nAverageCount;
				hor2 = hor_tot2 / poDCP06Model->m_nAverageCount;
				ver2 = ver_tot2 / poDCP06Model->m_nAverageCount;
				/*
				x_new2 = x_tot2 /poDCP06Model->m_nAverageCount;
				y_new2 = y_tot2 /poDCP06Model->m_nAverageCount;
				z_new2 = z_tot2 /poDCP06Model->m_nAverageCount;
				*/
				// average all
				if(poDCP06Model->m_n2FaceMeas == ALL || poDCP06Model->m_n2FaceMeas == ALL_MANUAL)
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

				DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*)GetModel();
				pModel->m_dX = x_new;
				pModel->m_dY = y_new;
				pModel->m_dZ = z_new;

				pModel->x_scs = x_scs_tot;
				pModel->y_scs = y_scs_tot;
				pModel->z_scs = z_scs_tot;
		
	    		// save log
				if(poDCP06Model->m_nAmsLog == 1)
				{
						// save log
						if(	m_iUseTool ==1)
						{
							short iActivetool = poDCP06Model->active_tool;
							if(iActivetool)
							{
								sprintf(pModel->tool_name,"%-10.10s",poDCP06Model->tool_table[iActivetool-1].tool_id);
								m_pCommon->strbtrim(pModel->tool_name);
							}
						}

						DCP06AmsLogC AmsLog(pModel, poDCP06Model);
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

void DCP::DCP06ScanMeasXYZControllerC::Run()
{
	DCP06Log("DCP06MeasXYZControllerC::Run()");

	//DCP::DCP06MeasXYZModelC* pModel = (DCP::DCP06MeasXYZModelC*)GetModel();

	m_pCommon = new DCP06CommonC(poDCP06Model);
	
		poHourGlass->Continue();
		if(GetController(DO_MEAS_XYZ_FACE1_CONTROLLER) == NULL)
		{
			DCP06Log("(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DCP06DoMeasXYZControllerC );");		
			(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DCP06DoScanMeasXYZControllerC );
		}
		(void)GetController( DO_MEAS_XYZ_FACE1_CONTROLLER )->SetModel(GetModel());
		SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
}
void DCP::DCP06ScanMeasXYZControllerC::OnControllerActivated(void)
{

}
void DCP::DCP06ScanMeasXYZControllerC::OnControllerClosed(int lExitCode)
{
	DCP06Log( "DCP06MeasXYZControllerC::OnControllerClosed" );
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
	//	DeactivateMeasurement(lExitCode);
}

short DCP::DCP06ScanMeasXYZControllerC::get_xyz_values(double* x, double* y, double* z)
{
	*x = x_new;
	*y = y_new;
	*z = z_new;

	return 1;
}
void DCP::DCP06ScanMeasXYZControllerC::to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs)
{
	m_pCommon->to_xyz(dis, hor, ver, x, y,  z,  m_iUseTool, x_scs, y_scs, z_scs);
}


