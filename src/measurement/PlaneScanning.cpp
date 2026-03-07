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
#include <dcp06/measurement/PlaneScanning.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/ScanMeasXYZ.hpp>

#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <dcp06/core/Aim.hpp>
#include <dcp06/measurement/3dFileView.hpp>


#include <GUI_Desktop.hpp>
#include <GUI_Application.hpp>
#include <GUI_MessageDialog.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <math.h>
#include <GUI_HourGlass.hpp>

#include <ABL_MsgDef.hpp>
//#include <ABL_util.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::PlaneScanDialog);
// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::PlaneScanDialog::PlaneScanDialog(DCP::PlaneScanModel* pPlaneScanModel,PlaneScanController* controller):m_pFile(0), m_pBoundary(0), 
		m_pResolutionHeigth(0), m_pResolutionWidth(0),m_pPointId(0),
		m_pDataModel(pPlaneScanModel), pController(controller),
		m_pResolutionHeigthObserver(OBS_METHOD_TO_PARAM0(PlaneScanDialog, OnValueChanged), this),
		m_pResolutionWidthObserver(OBS_METHOD_TO_PARAM0(PlaneScanDialog, OnValueChanged), this),
		m_pFileObserver(OBS_METHOD_TO_PARAM0(PlaneScanDialog, OnValueChanged), this),
		m_pPointIdObserver(OBS_METHOD_TO_PARAM0(PlaneScanDialog, OnValueChanged), this)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::PlaneScanDialog::~PlaneScanDialog()
{

}

void DCP::PlaneScanDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();

	//InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_SCN_FILE_TOK));
	m_pFile->GetStringInputCtrl()->SetCharsCountMax(8);

	//m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pFile);
	
	//InsertEmptyLine();

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(3);
	AddCtrl(m_pPointId);
	
	//InsertEmptyLine();

	m_pBoundary = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pBoundary->SetId(eBoundary);
	m_pBoundary->SetText(StringC(AT_DCP06,P_DCP_BOUNDARY_TOK));
	void(m_pBoundary->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pBoundary->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pBoundary);

	//InsertEmptyLine();

	m_pResolutionHeigth = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Integer);
	m_pResolutionHeigth->SetId(eResolutionHeigth);
	m_pResolutionHeigth->SetText(StringC(AT_DCP06,P_DCP_RESOLUTION_HEIGTH_TOK));
	m_pResolutionHeigth->GetIntegerInputCtrl()->SetRange(10,500);
	AddCtrl(m_pResolutionHeigth);

	m_pResolutionWidth = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Integer);
	m_pResolutionWidth->SetId(eResolutionWidth);
	m_pResolutionWidth->SetText(StringC(AT_DCP06,P_DCP_RESOLUTION_WIDTH_TOK));
	m_pResolutionWidth->GetIntegerInputCtrl()->SetRange(10,500);
	AddCtrl(m_pResolutionWidth);

	//SetHelpTok(H_DCP_SCAN_TOK,0);

	m_pResolutionHeigthObserver.Attach(m_pResolutionHeigth->GetSubject());
	m_pResolutionWidthObserver.Attach(m_pResolutionWidth->GetSubject());
	m_pFileObserver.Attach(m_pFile->GetSubject());
	m_pPointIdObserver.Attach(m_pPointId->GetSubject());
	DCP06_TRACE_EXIT;
}

void DCP::PlaneScanDialog::OnDialogActivated()
{
	/*
	m_pDataModel->old_active_coodinate_system = GetModel()->active_coodinate_system;
	memcpy(m_pDataModel->matrix,GetModel()->ocsp_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->inv_matrix,GetModel()->ocsp_inv_matrix, sizeof(double) * 16);
	m_pDataModel->calculated = (GetModel()->ocsp_defined  && GetModel()->active_coodinate_system == OCSP) ? true :false;

	memcpy(&m_pDataModel->point_DCS[0], &GetModel()->BestFit_point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&m_pDataModel->point_OCS[0], &GetModel()->BestFit_point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&m_pDataModel->point_RES[0], &GetModel()->BestFit_point_RES[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	
	m_pDataModel->INTO_CAPTURE = GetModel()->bestFit_into_capture;
	m_pDataModel->INTO_TEMPLATE = GetModel()->bestFit_into_template;
	m_pDataModel->ocs_defined = GetModel()->ocsp_defined;
	*/
	RefreshControls();
}

// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::PlaneScanDialog::OnValueChanged(int unNotifyCode, int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		if (ulParam2 == eResolutionHeigth)
		{
			m_pDataModel->iResolutionHeigth = m_pResolutionHeigth->GetIntegerInputCtrl()->GetShort();
		}
		else if (ulParam2 == eResolutionWidth)
		{
			m_pDataModel->iResolutionWidth = m_pResolutionWidth->GetIntegerInputCtrl()->GetShort();
		}
		else if(ulParam2 == ePointId)
		{
			m_pDataModel->sPointId = m_pPointId->GetStringInputCtrl()->GetString();
		}
		else if(ulParam2 == eFile)
		{
			m_pDataModel->sFileName = m_pFile->GetStringInputCtrl()->GetString();
			if(m_pDataModel->m_pScanFile->IsOpen())
			{
				m_pDataModel->m_pScanFile->close_adf_file();
			}
			
			m_pDataModel->m_pScanFile->setFile(m_pDataModel->sFileName);
		}
		RefreshControls();
	}
}

// Description: refresh all controls
void DCP::PlaneScanDialog::RefreshControls()
{	
	if(m_pFile && m_pBoundary && m_pResolutionHeigth && m_pResolutionWidth && m_pPointId)
	{	
		
		m_pResolutionHeigth->GetIntegerInputCtrl()->SetShort(m_pDataModel->iResolutionHeigth);
		m_pResolutionWidth->GetIntegerInputCtrl()->SetShort(m_pDataModel->iResolutionWidth);
		m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->sFileName);
		m_pPointId->GetStringInputCtrl()->SetString(m_pDataModel->sPointId);

		StringC sStat = L"-";
		
		Common common(GetModel());
		
		int boundaryPoints = common.get_OCS_points_count(&m_pDataModel->boundary_plane[0].points[0],3);
		if(boundaryPoints == 3)
			sStat = L"+";

		m_pBoundary->GetStringInputCtrl()->SetString(sStat);

		if(!m_pFile->GetStringInputCtrl()->IsEmpty() && boundaryPoints == 3)
		{
			
			if(!m_pDataModel->isRunning)
			{
				pController->DisableFunctionKey(FK5);
				pController->EnableFunctionKey(FK4);
			}
			else
			{
				pController->DisableFunctionKey(FK4);
				pController->EnableFunctionKey(FK5);
			}
		}
		else
		{
			pController->DisableFunctionKey(FK4);
			pController->DisableFunctionKey(FK5);
		}
		/*
		if(m_pDataModel->sFileName.GetLength() > 0 && m_pDataModel->m_pScanFile->isExists())
			pController->EnableFunctionKey(SHFK5);
		else
			pController->DisableFunctionKey(SHFK5);
		*/


		GUI::DesktopC::Instance()->UpdateFunctionKeys();
	}
}

void DCP::PlaneScanDialog::UpdateData()
{

}


// Description: only accept DCP06 Model objects
bool DCP::PlaneScanDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: DCP06 model
DCP::Model* DCP::PlaneScanDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::PlaneScanController::PlaneScanController(Model* pModel)
    : m_pDlg( nullptr ),m_pCommon(0),m_pDataModel(0),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'

    SetTitle(StringC( AT_DCP06, T_DCP_SCAN_TOK));

	m_pCommon = new Common(m_pModel);
	// create model
	m_pDataModel = new PlaneScanModel(m_pModel);
	//m_pDataModel->bestFit_chst = 0;

    // Create a dialog
    m_pDlg = new DCP::PlaneScanDialog(m_pDataModel, this);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SCAN_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;

	vDef.strLable	= StringC(AT_DCP06,K_DCP_BOUNDARY_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable	= StringC(AT_DCP06, K_DCP_SCAN_TOK);
	SetFunctionKey( FK4, vDef );
	
	vDef.strLable	= StringC(AT_DCP06,K_DCP_ABORT_TOK);
	SetFunctionKey( FK5, vDef );
	DisableFunctionKey(FK5);

	vDef.strLable	= StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable	= StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	/*
	vDef.nLable	= K_DCP_VIEW_TOK;
	SetFunctionKey( SHFK5, vDef );
	*/
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::PlaneScanController::~PlaneScanController()
{
	if(GetController(AIM_CONTROLLER) != nullptr)
		DestroyController(AIM_CONTROLLER);

	if(GetController(MEAS_XYZ_CONTROLLER) != nullptr)
		DestroyController(MEAS_XYZ_CONTROLLER);

	if(m_pDataModel)
	{
		//m_pDataModel->fileModel.m_pScanFile = 0;
		delete m_pDataModel;
		m_pDataModel = 0;
	}
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// Description: Route model to everybody else
bool DCP::PlaneScanController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::PlaneScanController::OnF1Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}

		DCP::MeasureModel* pModel = new MeasureModel;

		pModel->m_iMaxPoint = BOUNDARY_PLANE_POINTS;
		pModel->m_iMinPoint = BOUNDARY_PLANE_POINTS;
		//pModel->m_iPointsCount = BOUNDARY_PLANE_POINTS;
		pModel->m_iCurrentPoint = 1;

		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * BOUNDARY_PLANE_POINTS);
		memcpy(&pModel->point_table[0],&m_pDataModel->boundary_plane[0].points[0], sizeof(S_POINT_BUFF) * BOUNDARY_PLANE_POINTS);

		
		if(GetController(MEAS_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
		}
		(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_SCAN_BOUNDARY_MEAS_TOK));
		
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);
}

// BOUNDARY MEAS
void DCP::PlaneScanController::OnF4Pressed()
{	
	if(m_pDataModel->generate_points(m_pDlg->GetModel()))
	{
		CancelMeas = false;
		DisableFunctionKey(FK1);
		DisableFunctionKey(FK4);
		DisableFunctionKey(FK6);
		DisableFunctionKey(SHFK2);


		//ActivateFunctionKeys();
		GUI::DesktopC::Instance()->UpdateFunctionKeys();
		//m_pDataModel->m_pAdfFile->form_pnt((long) 1);

		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		//if(!m_pCommon->strblank(m_pDataModel->m_pAdfFile->xdes_front) && !m_pCommon->strblank(m_pDataModel->m_pAdfFile->ydes_front) && 
		//		!m_pCommon->strblank(m_pDataModel->m_pAdfFile->zdes_front))
		if(m_pDataModel->points_count > 0)
		{
			m_pDataModel->current_point = 1;
			/*
			x = atof(m_pDataModel->m_pAdfFile->xdes_front);
			y = atof(m_pDataModel->m_pAdfFile->ydes_front);
			z = atof(m_pDataModel->m_pAdfFile->zdes_front);
			*/
			x = m_pDataModel->des_points[m_pDataModel->current_point-1].x;
			y = m_pDataModel->des_points[m_pDataModel->current_point-1].y;
			z = m_pDataModel->des_points[m_pDataModel->current_point-1].z;

			m_pDataModel->isRunning = true;
		
			if(GetController(AIM_CONTROLLER) == nullptr)
			{
				(void)AddController( AIM_CONTROLLER, new DCP::AimController(	x,//atof(m_pDataModel->xdes_ptr),
																					y,//atof(m_pDataModel->ydes_ptr),
																					z, //atof(m_pDataModel->zdes_ptr),
																					m_pDlg->GetModel()->active_coodinate_system) );
				(void)GetController( AIM_CONTROLLER )->SetModel(m_pDlg->GetModel());
			}
			else
			{
				AimController* pCont = (AimController*)(GetController( AIM_CONTROLLER ));
				pCont->set_values(x,y,z,m_pDlg->GetModel()->active_coodinate_system);
			}

//			(void)GetController( AIM_CONTROLLER )->SetModel(m_pDlg->GetModel());
			SetActiveController(AIM_CONTROLLER, true);

		}	
	}
}
// CANCEL SCAN MEAS
void DCP::PlaneScanController::OnF5Pressed()
{
	CancelMeas = true;
	m_pDataModel->isRunning = false;
}
// Description: Handle change of position values
void DCP::PlaneScanController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

void DCP::PlaneScanController::OnSHF2Pressed()
{	
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_SCAN_TOK);
	MsgBox MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		m_pDataModel->iResolutionHeigth = 100;
		m_pDataModel->iResolutionWidth = 100;
		m_pDataModel->sFileName = L"";
		m_pDataModel->sPointId = L"SC";
		memset(&m_pDataModel->boundary_plane[0].points[0],0,sizeof(S_POINT_BUFF)* BOUNDARY_PLANE_POINTS);

		memset(&m_pDataModel->des_points[0], 0, sizeof(S_SCAN_POINT_BUFF) * MAX_SCAN_POINTS);
		m_pDataModel->current_point = 0;
		m_pDataModel->points_count = 0;
	}
	m_pDlg->RefreshControls();

}

// VIEW
// ******************************************************************************************
void DCP::PlaneScanController::OnSHF5Pressed()
{
	/*
	if(!m_pDataModel->m_pScanFile->IsOpen())
	{
		if(m_pDataModel->sFileName.GetLength() > 0)
			m_pDataModel->m_pScanFile->setFile(m_pDataModel->sFileName);		
		else
		 return;
	}
	
		//return;
	 m_pDataModel->m_pAdfFile->form_pnt(1);

	m_pDataModel->fileModel.m_pAdfFile = m_pDataModel->m_pAdfFile;
	if(GetController(_3DFILEVIEW_CONTROLLER) == nullptr)		
	{
		(void)AddController( _3DFILEVIEW_CONTROLLER, new DCP::FileView3DController (&m_pDataModel->fileModel, false));
	}
	(void)GetController( _3DFILEVIEW_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(_3DFILEVIEW_CONTROLLER, true);
	*/
	
}

// ******************************************************************************************
// Description: React on close of tabbed dialog
void DCP::PlaneScanController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
 
// Description: React on close of controller
void DCP::PlaneScanController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{	
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDataModel->boundary_plane[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * BOUNDARY_PLANE_POINTS);


	}
	/*
	else if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		//m_pDataModel->m_pFileFunc->setFile(strSelectedFile);
	}
	*/
	else if(lCtrlID == AIM_CONTROLLER)
	{
		if(lExitCode == EC_KEY_CONT && !CancelMeas)
		{
			
			DCP::MeasXYZModel* pModel = new MeasXYZModel();

			if(GetController(MEAS_XYZ_CONTROLLER) == nullptr)
			{
				//Log("(void)AddController( DO_MEAS_XYZ_FACE1_CONTROLLER, new DCP::DoMeasXYZController );");		
				(void)AddController( MEAS_XYZ_CONTROLLER, new DCP::ScanMeasXYZController(m_pDlg->GetModel()) );
				(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel(pModel);
			}
			else
			{
			
			}
		
			//pMeasXYZModel =  new MeasXYZModel();
			//(void)GetController( MEAS_XYZ_CONTROLLER )->SetModel(pModel);
			SetActiveController(MEAS_XYZ_CONTROLLER, true);
			
		}
		else
		{
			
		}
	}
	else if(lCtrlID == MEAS_XYZ_CONTROLLER)// && lExitCode == EC_KEY_CONT)
	{

		//if(lExitCode == EC_KEY_CONT && !CancelMeas)
		if(!CancelMeas)
		{

			
			DCP::MeasXYZModel* pModel = (DCP::MeasXYZModel*) GetController( MEAS_XYZ_CONTROLLER )->GetModel();		
		
			//m_pDataModel->m_pScanFile
			/*
			if(m_pDataModel->current_point > 1)
			{
				//m_pDataModel->m_pScanFile->add_point(m_pDataModel->des_points[m_pDataModel->current_point-1].point_id);
			}
			*/
			/*
			sprintf(m_pDataModel->m_pAdfFile->xmea_front,"%9.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dX);
			sprintf(m_pDataModel->m_pAdfFile->ymea_front,"%9.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dY);
			sprintf(m_pDataModel->m_pAdfFile->zmea_front,"%9.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dZ);
			
			sprintf(m_pDataModel->m_pAdfFile->xdes_front,"%s","");
			sprintf(m_pDataModel->m_pAdfFile->ydes_front,"%s","");
			sprintf(m_pDataModel->m_pAdfFile->zdes_front,"%s","");

			m_pDataModel->m_pAdfFile->form_save_pnt();
			*/
			
			char temp_x[20];
			char temp_y[20];
			char temp_z[20];

			sprintf(temp_x,"%.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dX);
			sprintf(temp_y,"%.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dY);
			sprintf(temp_z,"%.*f",m_pDlg->GetModel()->m_nDecimals, pModel->m_dZ);
		

			if(lExitCode == EC_KEY_CONT)
				m_pDataModel->m_pScanFile->add_new_pnt(m_pDataModel->des_points[m_pDataModel->current_point-1].point_id, temp_x, temp_y, temp_z);
			else
				m_pDataModel->m_pScanFile->add_new_pnt(m_pDataModel->des_points[m_pDataModel->current_point-1].point_id, "", "", "");
			
			//if(m_pDataModel->m_pAdfFile->form_next_pnt() == 1)
			if(m_pDataModel->current_point  < (int) m_pDataModel->points_count)
			{
				m_pDataModel->current_point++;
				char formatBuffer[STRING_BUFFER_MEDIUM];
				sprintf(formatBuffer, "%d / %d", m_pDataModel->current_point, m_pDataModel->points_count);
				GUI::DesktopC::Instance()->MessageShow(StringC(formatBuffer));//,4,false);
				//GUI::DesktopC::Instance()->MessageShow(StringC(m_pDataModel->m_pAdfFile->pointid_front),4,false);

				double x = 0.0;
				double y = 0.0;
				double z = 0.0;
				
				//if(!m_pCommon->strblank(m_pDataModel->m_pAdfFile->xdes_front) && !m_pCommon->strblank(m_pDataModel->m_pAdfFile->ydes_front) && 
				//	!m_pCommon->strblank(m_pDataModel->m_pAdfFile->zdes_front))
				if(1)
				{
					x = m_pDataModel->des_points[m_pDataModel->current_point-1].x;
					y = m_pDataModel->des_points[m_pDataModel->current_point-1].y;
					z = m_pDataModel->des_points[m_pDataModel->current_point-1].z;
					/*
					x = atof(m_pDataModel->m_pAdfFile->xdes_front);
					y = atof(m_pDataModel->m_pAdfFile->ydes_front);
					z = atof(m_pDataModel->m_pAdfFile->zdes_front);
					*/
					if(GetController(AIM_CONTROLLER) == nullptr)
					{
						(void)AddController( AIM_CONTROLLER, new DCP::AimController(	x,//atof(m_pDataModel->xdes_ptr),
																							y,//atof(m_pDataModel->ydes_ptr),
																							z, //atof(m_pDataModel->zdes_ptr),
																							m_pDlg->GetModel()->active_coodinate_system) );
						
						(void)GetController( AIM_CONTROLLER )->SetModel(m_pDlg->GetModel());
					}
					else
					{
						AimController* pCont = (AimController*)(GetController( AIM_CONTROLLER ));
						pCont->set_values(x,y,z,m_pDlg->GetModel()->active_coodinate_system);
					}
					//(void)GetController( AIM_CONTROLLER )->SetModel(m_pDlg->GetModel());
					SetActiveController(AIM_CONTROLLER, true);

				}
				//SetActiveController(DO_MEAS_XYZ_FACE1_CONTROLLER, true);
			}
			else
			{
				DestroyController( lCtrlID );
				EnableFunctionKey(FK1);
				EnableFunctionKey(FK4);
				EnableFunctionKey(FK6);
				EnableFunctionKey(SHFK2);
				GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_SCAN_FINISHED_TOK));
				m_pDataModel->isRunning = false;
			}
		}
		else
		{
			if(CancelMeas)
			{
				m_pDataModel->isRunning = false;
				GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,M_DCP_ABORTED_TOK));
			}

			DestroyController( lCtrlID );
			EnableFunctionKey(FK1);
			EnableFunctionKey(FK4);
			EnableFunctionKey(FK6);
			EnableFunctionKey(SHFK2);
		}
	}
	m_pDlg->RefreshControls();
	//DestroyController( lCtrlID );
	//if(lCtrlID != MEAS_XYZ_CONTROLLER && lCtrlID != AIM_CONTROLLER )
	//if(lCtrlID != AIM_CONTROLLER )
	//	DestroyController( lCtrlID );
}


// ===========================================================================================
// POMModel
// ===========================================================================================

// Instantiate template classes
DCP::PlaneScanModel::PlaneScanModel(Model* pModel)
{
	iResolutionHeigth = 100;
	iResolutionWidth = 100;
	sFileName = L"";
	sPointId = L"SC";
	isRunning = false;
	//memset(&boundary_points[0],0,sizeof(S_POINT_BUFF)* 3);
	memset(&boundary_plane[0],0,sizeof(S_PLANE_BUFF));

	memset(&des_points[0], 0, sizeof(S_SCAN_POINT_BUFF) * MAX_SCAN_POINTS);
	current_point = 0;
	points_count = 0;
	/*
	boundary_plane[0].points[0].x = 0.0;
	boundary_plane[0].points[0].y = 0.0;
	boundary_plane[0].points[0].z = 0.0;
	boundary_plane[0].points[0].sta = 1;

	boundary_plane[0].points[1].x = DEFAULT_BOUNDARY_SIZE;
	boundary_plane[0].points[1].y = 0.0;
	boundary_plane[0].points[1].z = 0.0;
	boundary_plane[0].points[1].sta = 1;

	boundary_plane[0].points[2].x = DEFAULT_BOUNDARY_SIZE;
	boundary_plane[0].points[2].y = DEFAULT_BOUNDARY_SIZE;
	boundary_plane[0].points[2].z = 0.0;
	boundary_plane[0].points[2].sta = 1;
	*/
	m_pScanFile = new ScanFileFunc(pModel);
}


DCP::PlaneScanModel::~PlaneScanModel()
{
		if(m_pScanFile)
		{

			delete m_pScanFile;
			m_pScanFile = 0;
		}
}

bool DCP::PlaneScanModel::generate_points(DCP::Model *pModel)
{
	bool ret = false;
	struct ams_vector plane_p1,plane_p2,plane_p3;
	struct ams_vector corner_p1, corner_p2, corner_p3, corner_p4;
	char first_pointid[STRING_BUFFER_SMALL];
	char pointIdBuffer[STRING_BUFFER_SMALL];
	struct line lineP2P3, lineP1P2, lineP4P3;
	StringC strMsg;
	bool too_many_points = false;
	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(L"generate points");
	// check that two points is defined
	memset(&des_points[0], 0, sizeof(S_SCAN_POINT_BUFF)* MAX_SCAN_POINTS);
	current_point = 0;
	points_count = 0;
	total_points = 0;

	if(boundary_plane[0].points[0].sta != 0 && boundary_plane[0].points[1].sta !=0 && boundary_plane[0].points[2].sta !=0)
	{
		/*
		CalcPlane calcPlane;
		if(calcPlane.calc(&boundary_plane[0],ACTUAL))
		{
				
		}
		*/
		
		plane_p1.x = boundary_plane[0].points[0].x;
		plane_p1.y = boundary_plane[0].points[0].y;
		plane_p1.z = boundary_plane[0].points[0].z;

		plane_p2.x = boundary_plane[0].points[1].x;
		plane_p2.y = boundary_plane[0].points[1].y;
		plane_p2.z = boundary_plane[0].points[1].z;

		plane_p3.x = boundary_plane[0].points[2].x;
		plane_p3.y = boundary_plane[0].points[2].y;
		plane_p3.z = boundary_plane[0].points[2].z;

		
		equation_of_line(&plane_p2, &plane_p3, &lineP2P3);

		corner_p1.x = plane_p1.x;
		corner_p1.y = plane_p1.y;
		corner_p1.z = plane_p1.z;

		corner_p2.x = plane_p2.x;
		corner_p2.y = plane_p2.y;
		corner_p2.z = plane_p2.z;

		corner_p3.x = plane_p3.x;
		corner_p3.y = plane_p3.y;
		corner_p3.z = plane_p3.z;

		double distP2P3 = calc_point_dist_from_point(&plane_p2, &plane_p3);

		corner_p4.x = plane_p1.x + lineP2P3.ux * distP2P3;
        corner_p4.y = plane_p1.y + lineP2P3.uy * distP2P3;
        corner_p4.z = plane_p1.z + lineP2P3.uz * distP2P3;

		// calculate distance between points
        ams_vector p1temp, p2temp, p3temp;

        p1temp.x = (double)corner_p1.x;
        p1temp.y = (double)corner_p1.y;
        p1temp.z = (double)corner_p1.z;

        p2temp.x = (double)corner_p2.x;
        p2temp.y = (double)corner_p2.y;
        p2temp.z = (double)corner_p2.z;

        p3temp.x = (double)corner_p4.x;
        p3temp.y = (double)corner_p4.y;
        p3temp.z = (double)corner_p4.z;

        double distHeigth = calc_point_dist_from_point(&p1temp, &p2temp);
        int pointCountHeigth = (int)distHeigth / iResolutionHeigth;

		double distWidth = calc_point_dist_from_point(&p1temp, &p3temp);
		int pointCountWidth = (int)distWidth / iResolutionWidth;

		equation_of_line(&corner_p1, &corner_p2, &lineP1P2);
		
		equation_of_line(&corner_p4, &corner_p3, &lineP4P3);
		
			// create the file
			//AdfFileFunc* m_pAdfFile = new AdfFileFunc();
			//m_pAdfFile->always_single = 1;

			char fname[CPI::LEN_PATH_MAX];
			//UTL::UnicodeToAscii(fname, sFileName);
			BSS::UTI::BSS_UTI_WCharToAscii( sFileName, fname );

			//UTL::UnicodeToAscii(first_pointid, sPointId);
			BSS::UTI::BSS_UTI_WCharToAscii( sPointId, first_pointid );
	
			sprintf(pointIdBuffer,"%-s%d",first_pointid,1);
			
			if(m_pScanFile->create_adf_file(fname, false)==0)
			{
				GUI::HourGlassC* poHourGlass = new GUI::HourGlassC();
			
				m_pScanFile->setFile(sFileName);

				struct ams_vector po1, po2, po11, po22;
				struct line line1;
				struct line line2;

				for (int i = 0; i <= pointCountHeigth; i++)
				{
					po1.x = lineP1P2.px + (i * iResolutionHeigth) * lineP1P2.ux;
					po1.y = lineP1P2.py + (i * iResolutionHeigth) * lineP1P2.uy;
					po1.z = lineP1P2.pz + (i * iResolutionHeigth) * lineP1P2.uz;

					po2.x = lineP4P3.px + (i * iResolutionHeigth) * lineP4P3.ux;
					po2.y = lineP4P3.py + (i * iResolutionHeigth) * lineP4P3.uy;
					po2.z = lineP4P3.pz + (i * iResolutionHeigth) * lineP4P3.uz;

					equation_of_line(&po1, &po2, &line1);

					if(add_line_points(m_pScanFile, &line1, i, pointCountWidth, distWidth, iResolutionWidth, first_pointid,pModel) == false)
					{
						too_many_points = true;
						//break;
					}
					     // check if points to be added
					if (i == pointCountHeigth)
					{
						//if((int) distHeigth % iResolutionHeigth > 0)
						if (fmod(distHeigth,iResolutionHeigth) > 0.0)
						{
							// 201011
							double te = fmod(distHeigth, iResolutionHeigth);
							//double te = ((int) distHeigth % iResolutionHeigth);
							if (te > (iResolutionHeigth / 2))
							{
								po11.x = lineP1P2.px + (i * iResolutionHeigth + (te)) * lineP1P2.ux;
								po11.y = lineP1P2.py + (i * iResolutionHeigth + (te)) * lineP1P2.uy;
								po11.z = lineP1P2.pz + (i * iResolutionHeigth + (te)) * lineP1P2.uz;

								po22.x = lineP4P3.px + (i * iResolutionHeigth + (te)) * lineP4P3.ux;
								po22.y = lineP4P3.py + (i * iResolutionHeigth + (te)) * lineP4P3.uy;
								po22.z = lineP4P3.pz + (i * iResolutionHeigth + (te)) * lineP4P3.uz;

								equation_of_line(&po11, &po22, &line2);

								if(add_line_points(m_pScanFile, &line2, i, pointCountWidth, distWidth, iResolutionWidth, first_pointid,pModel) == false)
								{
									too_many_points = true;
									//break;
								}
							}
						}
					}
				}

				if(poHourGlass)
				{
					delete poHourGlass;
					poHourGlass = 0;
				}

				if(too_many_points == true)
				{
					MsgBox msgbox;	
					StringC msg;
					msg.LoadTxt(AT_DCP06,M_DCP_SCAN_TOO_MANY_POINTS_TOK);
					msg.Format(msg,total_points, MAX_SCAN_POINTS);
					msgbox.ShowMessageOk(msg);
				}
				else
				{
					GUI::DesktopC::Instance()->MessageShow(StringC(AT_DCP06,I_SCANNING_POINTS_CREATED_TOK));

				}

				ret = too_many_points ? false : true;

				
				Common common(pModel);
				common.delay();
			}
			/*
			if(m_pAdfFile)
			{
				delete m_pAdfFile;
				m_pAdfFile = 0;
			}
			*/
	}
	return ret;
}

bool DCP::PlaneScanModel::add_line_points(ScanFileFunc* m_pScanFile, struct line* line1, 
							short i, short pointCountWidth, double distWidth, double resolutionWidth, char* first_pointid,
							DCP::Model *pModel)
{
	char pointIdBuffer[STRING_BUFFER_SMALL];
	bool ret = true;

	for (int j = 0; j <= pointCountWidth; j++)
    {
		/*
		if(i == 0 && j == 0)
		{
			m_pAdfFile->form_pnt((long) 1);
		}
		else
		{
			
			sprintf(temp,"%-s%d",first_pointid, m_pAdfFile->points +1);

			m_pAdfFile->add_point(temp);
			m_pAdfFile->form_pnt((long) m_pAdfFile->points);
		}
		
		sprintf(m_pAdfFile->xdes_front,"%9.*f",pModel->m_nDecimals, line1->px + (j * resolutionWidth) * line1->ux);
		sprintf(m_pAdfFile->ydes_front,"%9.*f",pModel->m_nDecimals, line1->py + (j * resolutionWidth) * line1->uy);
		sprintf(m_pAdfFile->zdes_front,"%9.*f",pModel->m_nDecimals, line1->pz + (j * resolutionWidth) * line1->uz);
		
		m_pAdfFile->form_save_pnt();
		*/
		if(points_count  < MAX_SCAN_POINTS)
		{
			sprintf(pointIdBuffer,"%-s%d",first_pointid, points_count + 1);

			sprintf(des_points[points_count].point_id, "%-s", pointIdBuffer);
			des_points[points_count].x = line1->px + (j * resolutionWidth) * line1->ux;
			des_points[points_count].y = line1->py + (j * resolutionWidth) * line1->uy;
			des_points[points_count].z = line1->pz + (j * resolutionWidth) * line1->uz;
			points_count++;
			total_points++;
		}
		else
		{
			ret = false;
			total_points++;
			//break;
		}

        if (j == pointCountWidth)
        {
            if ((int) distWidth % (int) resolutionWidth > 0.0)

			//if(fmod(distWidth, resolutionWidth) > 0.0)
            {
				// 201011
                double te = fmod(distWidth, resolutionWidth);
				//double te = ((int) distWidth %  (int) resolutionWidth);
                if (te > (resolutionWidth / 2))
                {
					// add one point more
					/*
					sprintf(temp,"%-s%d",first_pointid, m_pAdfFile->points + 1);
					m_pAdfFile->add_point(temp);
	               
					m_pAdfFile->form_pnt((long) m_pAdfFile->points);

					sprintf(m_pAdfFile->xdes_front,"%9.*f",pModel->m_nDecimals, line1->px + (j * resolutionWidth + ((int) distWidth % (int) resolutionWidth) * line1->ux ));
					sprintf(m_pAdfFile->ydes_front,"%9.*f",pModel->m_nDecimals, line1->py + (j * resolutionWidth + ((int) distWidth % (int) resolutionWidth) * line1->uy ));
					sprintf(m_pAdfFile->zdes_front,"%9.*f",pModel->m_nDecimals, line1->pz + (j * resolutionWidth + ((int) distWidth % (int) resolutionWidth) * line1->uz ));
					m_pAdfFile->form_save_pnt();
					*/
					if(points_count < MAX_SCAN_POINTS)
					{
						sprintf(pointIdBuffer,"%-s%d",first_pointid, points_count + 1);

						sprintf(des_points[points_count].point_id,"%-s", pointIdBuffer);
						des_points[points_count].x = line1->px + (j * resolutionWidth + (te)) * line1->ux ;
						des_points[points_count].y = line1->py + (j * resolutionWidth + (te)) * line1->uy ;
						des_points[points_count].z =  line1->pz + (j * resolutionWidth + (te)) * line1->uz ;
						points_count++;
						total_points++;
					}
					else
					{
						total_points++;
						ret = false;
						//break;
					}

				}
            }
        }

    }
	
	return ret;
}