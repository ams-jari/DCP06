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
#include <dcp06/orientation/BestFit.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/orientation/BestFitSelectPoints.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/database/IDatabase.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>
#include <dcp06/core/Common.hpp>
#include <limits>
#include <dcp06/calculation/CalculationBestFit.hpp>
#include <dcp06/orientation/ResBestFit.hpp>

#include <GUI_Desktop.hpp>
#include <GUI_Application.hpp>
#include <GUI_MessageDialog.hpp>
#include <UTL_StringFunctions.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_util.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
// OBS_IMPLEMENT_EXECUTE(DCP::UnitDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::BestFitDialog::BestFitDialog(DCP::BestFitModel* pPomModel):m_pInfo1(0), m_pFile(0), m_pPoints(0), m_pInfo2(0),
			m_pPointMeas(0), m_pCalc(0),m_pDataModel(pPomModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::BestFitDialog::~BestFitDialog()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::BestFitDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	m_pCommon = new Common(GetModel());

	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );

	m_pInfo1 = new GUI::TextCtrlC();
	m_pInfo1->SetId(eInfo1);
	m_pInfo1->SetText(StringC(AT_DCP06,L_DCP_POM_SELECT_OCS_POINTS_TOK));
	AddCtrl(m_pInfo1);

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_TEMPLATE_SK_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pFile->SetAutoColon(false);
	//m_pFile->SetColonPosition(9*25);
	// m_pFile->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pFile);
	
	m_pPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPoints->SetId(ePoints);
	m_pPoints->SetText(StringC(AT_DCP06,P_DCP_OCS_POINTS_SK_TOK));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPoints->SetAutoColon(false);
	//m_pPoints->SetColonPosition(9*25);
	// m_pPoints->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPoints);

	m_pInfo2 = new GUI::TextCtrlC();
	m_pInfo2->SetId(eInfo2);
	m_pInfo2->SetText(StringC(AT_DCP06,L_DCP_POM_MEASURE_SCS_POINTS_TOK));
	
	AddCtrl(m_pInfo2);

	m_pPointMeas = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointMeas->SetId(ePointMeas);
	m_pPointMeas->SetText(StringC(AT_DCP06,P_DCP_POM_MEASURE_SCS_POINTS_TOK));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointMeas->SetAutoColon(false);
	//m_pPointMeas->SetColonPosition(9*25);
	// m_pPointMeas->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPointMeas);

	m_pCalc = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCalc->SetId(eCalc);
	m_pCalc->SetText(StringC(AT_DCP06,P_DCP_POM_CALC_TOK));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pCalc->SetAutoColon(false);
	//m_pCalc->SetColonPosition(9*25);
	// m_pCalc->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pCalc);

	//SetHelpTok(H_DCP_POM_TOK,0);
	DCP06_TRACE_EXIT;
}

void DCP::BestFitDialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
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
	RefreshControls();
	DCP06_TRACE_EXIT;
}

// Description: refresh all controls
void DCP::BestFitDialog::RefreshControls()
{
	DCP06_TRACE_ENTER;
	if(m_pFile && m_pPoints && 	m_pPointMeas &&  m_pCalc)
	{
		Common common(GetModel());
		StringC sStat=L"-";
		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_BESTFIT_POINTS) >= 3 && m_pDataModel->INTO_TEMPLATE)
			sStat = L"+";

		m_pFile->GetStringInputCtrl()->SetString(sStat);
		
		sStat = L"-";
		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_BESTFIT_POINTS) >= 3 && m_pDataModel->INTO_CAPTURE)
			sStat = L"+";	
		
		m_pPoints->GetStringInputCtrl()->SetString(sStat);
		sStat = L"-";
		if(common.get_OCS_SCS_points_count(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_BESTFIT_POINTS) >= 3 )
			sStat = L"+";	

		m_pPointMeas->GetStringInputCtrl()->SetString(sStat);
		sStat = L"-";
		if(m_pDataModel->calculated == true && m_pDataModel->ocs_defined == true && GetModel()->active_coodinate_system == OCSP)
			sStat = L"+";

		m_pCalc->GetStringInputCtrl()->SetString(sStat);
	}
	DCP06_TRACE_EXIT;
}

void DCP::BestFitDialog::UpdateData()
{
	DCP06_TRACE_ENTER;
	// copy values

	//m_pDataModel->old_active_coodinate_system = GetModel()->active_coodinate_system;
	memcpy(GetModel()->ocsp_matrix,m_pDataModel->matrix, sizeof(double) * 16);
	memcpy(GetModel()->ocsp_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);

	memcpy(&GetModel()->BestFit_point_DCS[0],&m_pDataModel->point_DCS[0],  sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy( &GetModel()->BestFit_point_OCS[0], &m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&GetModel()->BestFit_point_RES[0],&m_pDataModel->point_RES[0],  sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	GetModel()->bestFit_into_capture  = m_pDataModel->INTO_CAPTURE;
	GetModel()->bestFit_into_template = m_pDataModel->INTO_TEMPLATE;
	GetModel()->ocsp_defined = m_pDataModel->ocs_defined;
	if(m_pDataModel->ocs_defined)
		GetModel()->active_coodinate_system = OCSP;
	//else
	//	GetModel()->active_coodinate_system = DCS;
	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_BESTFIT);
	GetModel()->poConfigController->StoreConfigData();

	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
	GetModel()->poConfigController->StoreConfigData();

	update_bft_adf();
	DCP06_TRACE_EXIT;

	//GetModel()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}

void DCP::BestFitDialog::update_bft_adf()
{
	// DB-primary: save BestFit to database when job is loaded
	DCP::Database::IDatabase* pDb = GetModel()->GetDatabase();
	if (pDb && !GetModel()->m_currentJobId.empty())
	{
		DCP::Database::BestFitAlignmentData bfData;
		bfData.type = "bestfit";
		bfData.bestFitAlignmentType = "BF1";
		bfData.calculated = m_pDataModel->calculated;
		bfData.rms = m_pDataModel->rms_x;  // use primary RMS
		for (int i = 0; i < MAX_BESTFIT_POINTS; i++)
		{
			if (m_pCommon->strblank(m_pDataModel->point_OCS[i].point_id))
				continue;
			std::string pid(m_pDataModel->point_OCS[i].point_id);
			DCP::Database::PointData ptOcs, ptScs, ptRes;
			ptOcs.id = ptScs.id = ptRes.id = pid;
			ptOcs.x_dsg = m_pDataModel->point_OCS[i].x;
			ptOcs.y_dsg = m_pDataModel->point_OCS[i].y;
			ptOcs.z_dsg = m_pDataModel->point_OCS[i].z;
			if (m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
			{
				ptScs.x_mea = m_pDataModel->point_OCS[i].x + m_pDataModel->point_RES[i].x;
				ptScs.y_mea = m_pDataModel->point_OCS[i].y + m_pDataModel->point_RES[i].y;
				ptScs.z_mea = m_pDataModel->point_OCS[i].z + m_pDataModel->point_RES[i].z;
				ptRes.x_scs = m_pDataModel->point_RES[i].x;
				ptRes.y_scs = m_pDataModel->point_RES[i].y;
				ptRes.z_scs = m_pDataModel->point_RES[i].z;
			}
			else
			{
				ptScs.x_mea = ptScs.y_mea = ptScs.z_mea = std::numeric_limits<double>::quiet_NaN();
			}
			bfData.points_ocs[pid] = ptOcs;
			bfData.points_scs[pid] = ptScs;
			bfData.points_residuals[pid] = ptRes;
		}
		if (pDb->updateBestFitAlignment("BF1", bfData) || pDb->addBestFitAlignment("BF1", bfData))
			pDb->saveJob(GetModel()->m_currentJobId);
		return;
	}

	// No job loaded: inform user to open a job first (ADF is only for import/export in File dialog)
	MsgBox msgBox;
	StringC msg;
	msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK);
	msgBox.ShowMessageOk(msg);
}

// Description: only accept DCP06 Model objects
bool DCP::BestFitDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::BestFitDialog::GetModel() const
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
DCP::BestFitController::BestFitController(Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_POM_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new BestFitModel;
	m_pDataModel->bestFit_chst = 0;

    // Create a dialog
    m_pDlg = new DCP::BestFitDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( BESTFIT_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;

	vDef.strLable = StringC(AT_DCP06,K_DCP_FILE_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
	SetFunctionKey( FK2, vDef );
    
	vDef.strLable = StringC(AT_DCP06,K_DCP_MEAS_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
	SetFunctionKey( FK4, vDef );
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_RMS_TOK);
	SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_RESIDUALS_TOK);
	SetFunctionKey( SHFK4, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::BestFitController::~BestFitController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// Description: Route model to everybody else
bool DCP::BestFitController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::BestFitController::OnF1Pressed()
{
	if (m_pDlg == nullptr)
	{
		USER_APP_VERIFY(false);
		return;
	}
	// DB-primary: job must be loaded; ADF only for import/export in File dialog
	DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ? dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
	if (!jdb || !jdb->isJobLoaded() || m_pModel->m_currentJobId.empty())
	{
		MsgBox msgBox;
		StringC msg;
		msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK);
		msgBox.ShowMessageOk(msg);
		return;
	}
	Common common(m_pDlg->GetModel());
	if (common.get_OCS_points_count(&m_pDataModel->point_OCS[0], MAX_BESTFIT_POINTS) > 0)
	{
		if (!delete_bestFit())
			return;
	}
	// Load points from current job (design coords for OCS template)
	S_SELECT_POINTS selPts[MAX_BESTFIT_POINTS];
	char pid[POINT_ID_BUFF_LEN], bXdes[DCP_COORD_STR_BUFF_LEN], bYdes[DCP_COORD_STR_BUFF_LEN], bZdes[DCP_COORD_STR_BUFF_LEN];
	char bXmea[DCP_COORD_STR_BUFF_LEN], bYmea[DCP_COORD_STR_BUFF_LEN], bZmea[DCP_COORD_STR_BUFF_LEN];
	short nPts = jdb->getPointListAsSelectPointsForList(selPts, MAX_BESTFIT_POINTS, DESIGN, DCP::Database::PointSource::DCP06_3D_MEAS);
	if (nPts <= 0)
	{
		MsgBox msgBox;
		StringC msg;
		msg.LoadTxt(AT_DCP06, M_DCP_NO_POINTS_TOK);
		msgBox.ShowMessageOk(msg);
		return;
	}
	for (short i = 0; i < nPts && i < MAX_BESTFIT_POINTS; i++)
	{
		if (jdb->getPointByIndexForList(DCP::Database::PointSource::DCP06_3D_MEAS, i + 1, false, pid, bXmea, bXdes, bYmea, bYdes, bZmea, bZdes, 0))
		{
			snprintf(m_pDataModel->point_OCS[i].point_id, sizeof(m_pDataModel->point_OCS[i].point_id), DCP_POINT_ID_FMT, pid);
			strcpy(m_pDataModel->point_DCS[i].point_id, m_pDataModel->point_OCS[i].point_id);
			m_pDataModel->point_OCS[i].x = atof(bXdes);
			m_pDataModel->point_OCS[i].y = atof(bYdes);
			m_pDataModel->point_OCS[i].z = atof(bZdes);
			m_pDataModel->point_OCS[i].sta = (!common.strblank(bXdes) && !common.strblank(bYdes) && !common.strblank(bZdes)) ? POINT_DESIGN : POINT_NOT_DEFINED;
		}
	}
	m_pDataModel->INTO_TEMPLATE = true;
	m_pDataModel->calculated = false;
	m_pDlg->RefreshControls();
}

void DCP::BestFitController::OnF2Pressed()
{
	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::BestFitSelectPointsModel* pModel = new BestFitSelectPointsModel;
	Common common(m_pModel);

	memcpy(&pModel->points[0], &m_pDataModel->point_OCS[0],sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&pModel->points1[0], &m_pDataModel->point_DCS[0],sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	pModel->iDisplay = BESTFIT_DLG;
	pModel->iMaxPoint = MAX_BESTFIT_POINTS;
	pModel->iMinPoint = 3;
	pModel->iCurrentPoint = 1;
	pModel->m_iPointsCount = common.get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_BESTFIT_POINTS);
	m_pDataModel->INTO_CAPTURE = true;
	
	sprintf(pModel->default_pid,"%-s","BF");

	int to = pModel->m_iPointsCount < pModel->iMinPoint ? pModel->iMinPoint :  pModel->m_iPointsCount;
	
	for(int i = 0; i < to; i++)
	{
		sprintf(pModel->points[i].point_id,"BF%d",i+1);
	}

	if(GetController(BESTFIT_POINT_CONTROLLER) == nullptr)
	{
		(void)AddController( BESTFIT_POINT_CONTROLLER, new DCP::BestFitSelectPointsController(m_pDlg->GetModel()));
	}
	(void)GetController( BESTFIT_POINT_CONTROLLER )->SetModel(pModel);
	SetActiveController(BESTFIT_POINT_CONTROLLER, true);
	
}
void DCP::BestFitController::OnF3Pressed()
{	
	//short i;

    if (m_pDlg == nullptr) 
    {
        USER_APP_VERIFY( false );
        return;
    }

	Common common(m_pModel);
	MsgBox msgbox;
	StringC msg;

	short sum = common.defined_bestFit_points(&m_pDataModel->point_OCS[0],nullptr);
	if(sum==0)
	{
		msg.LoadTxt(AT_DCP06,M_DCP_NO_POINTS_TOK	);
		msgbox.ShowMessageOk(msg);
		//msgbox(TXT_NIL_TOKEN, M_NO_POINTS_TOK,MB_OK);
			//show_message("No points!");
	}
	else  
	{
		sum = common.get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_BESTFIT_POINTS);
		
		old_coordinate = m_pDlg->GetModel()->active_coodinate_system;
		m_pDlg->GetModel()->active_coodinate_system = DCS;
				
		DCP::MeasureModel* pModel = new MeasureModel;
		pModel->m_iMaxPoint = sum < 3 ? 3 : sum;
		pModel->m_iMinPoint = MIN_POINTS_FOR_DISTANCE;
		pModel->m_iPointsCount = sum;
		pModel->disable_point_editing = true;
		
		memcpy(&pModel->point_table[0],	&m_pDataModel->point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&pModel->point_table2[0],&m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	


		if(GetController(MEAS_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
		}
		(void)GetController( MEAS_CONTROLLER )->SetTitle(StringC(AT_DCP06, T_DCP_BF_MEAS_TOK));
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);
	}
}
// calc
void DCP::BestFitController::OnF4Pressed()
{
	CalcBestFit calcpom(m_pDataModel,m_pDlg->GetModel());
	if(calcpom.calc_transform())
	{
		m_pDataModel->calculated = true;
		

		StringC sMsg;
		
		StringC sX, sY, sZ, sRMS;
		sX.LoadTxt(AT_DCP06, P_DCP_X_TOK);
		sY.LoadTxt(AT_DCP06, P_DCP_Y_TOK);
		sZ.LoadTxt(AT_DCP06, P_DCP_Z_TOK);
		sRMS.LoadTxt(AT_DCP06, P_DCP_RMS_TOK);
		
		StringC sXline;
		StringC sYline;
		StringC sZline;
		char formatBuffer[STRING_BUFFER_MEDIUM];
		
		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_x);
		sXline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sX, (const wchar_t*) StringC(formatBuffer));

		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_y);
		sYline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sY, (const wchar_t*) StringC(formatBuffer));

		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_z);
		sZline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sZ, (const wchar_t*) StringC(formatBuffer));
		

		sMsg = sXline;
		sMsg += L"\n";
		sMsg += sYline;
		sMsg += L"\n";
		sMsg += sZline;
		
		MsgBox msgbox;
		short ret = msgbox.ShowMessageOkAbortRetry(sMsg);
		if( ret == 1)
		{
			if(GetController(RES_BESTFIT_CONTROLLER) == nullptr)
			{
				StringC sTitle = GetTitle();	
				(void)AddController( RES_BESTFIT_CONTROLLER, new DCP::ResBestFitController(m_pDataModel) );
			}
			(void)GetController( RES_BESTFIT_CONTROLLER )->SetModel(m_pDlg->GetModel());
			SetActiveController(RES_BESTFIT_CONTROLLER, true);
		}
		else if(ret == -1) // ABort
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_ORIENTATION_LOST_TOK);
			msgbox.ShowMessageOk(sMsg);
			m_pDataModel->calculated = false;
		}
		else				// retry
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_REMEASURE_SCS_POINTS_TOK);
			msgbox.ShowMessageOk(sMsg);
			m_pDataModel->calculated = false;
		}
	}
	else
	{

	}
}

// Description: Handle change of position values
void DCP::BestFitController::OnF6Pressed()
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

void DCP::BestFitController::OnSHF2Pressed()
{	
	delete_bestFit();
	m_pDlg->RefreshControls();

}

// RMS
void DCP::BestFitController::OnSHF3Pressed()
{	
	if(m_pDlg->GetModel()->ocsp_defined && m_pDataModel->calculated)
	{
		StringC sMsg;
		
		StringC sX, sY, sZ, sRMS;
		sX.LoadTxt(AT_DCP06, P_DCP_X_TOK);
		sY.LoadTxt(AT_DCP06, P_DCP_Y_TOK);
		sZ.LoadTxt(AT_DCP06, P_DCP_Z_TOK);
		sRMS.LoadTxt(AT_DCP06, P_DCP_RMS_TOK);
		
		StringC sXline;
		StringC sYline;
		StringC sZline;
		char formatBuffer[STRING_BUFFER_MEDIUM];
		
		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_x);
		sXline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sX, (const wchar_t*) StringC(formatBuffer));

		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_y);
		sYline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sY, (const wchar_t*) StringC(formatBuffer));

		sprintf(formatBuffer,"%9.*f",m_pDlg->GetModel()->m_nDecimals, m_pDataModel->rms_z);
		sZline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sZ, (const wchar_t*) StringC(formatBuffer));
		

		sMsg = sXline;
		sMsg += L"\n";
		sMsg += sYline;
		sMsg += L"\n";
		sMsg += sZline;
		
		MsgBox msgbox;
		msgbox.ShowMessageOk(sMsg);
	}
	
}

// RESIDUALS
void DCP::BestFitController::OnSHF4Pressed()
{
}
// Description: React on close of tabbed dialog
void DCP::BestFitController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
 
// Description: React on close of controller
void DCP::BestFitController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == BESTFIT_POINT_CONTROLLER) 
	{
		DCP::BestFitSelectPointsModel* pModel = (DCP::BestFitSelectPointsModel*) GetController( BESTFIT_POINT_CONTROLLER )->GetModel();

		Common common(m_pModel);  
		
		memcpy(&m_pDataModel->point_OCS[0], &pModel->points[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&m_pDataModel->point_DCS[0], &pModel->points1[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

		// copy point ids
		for(int i = 1; i <= MAX_BESTFIT_POINTS; i++)
		{
			strcpy(m_pDataModel->point_DCS[i-1].point_id, m_pDataModel->point_OCS[i-1].point_id);
		}
		m_pDataModel->calculated = false;
	}
	if(lCtrlID == MEAS_CONTROLLER) 
	{
		m_pDlg->GetModel()->active_coodinate_system = old_coordinate;
		
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();	

			memcpy(&m_pDataModel->point_DCS[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			memcpy(&m_pDataModel->point_OCS[0], &pModel->point_table2[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			m_pDataModel->calculated = false;
		}	
	}

	if(lCtrlID == RES_BESTFIT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->calculated = true;
		m_pDlg->GetModel()->active_coodinate_system = OCSP;
		m_pDlg->update_bft_adf();
	}


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

bool DCP::BestFitController::delete_bestFit()
{
		StringC strPomText;
		strPomText.LoadTxt(AT_DCP06,L_DCP_POM_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strPomText);
		
		MsgBox msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			
			memset(&m_pDataModel->point_DCS[0],0,sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			memset(&m_pDataModel->point_OCS[0],0,sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			memset(&m_pDataModel->point_RES[0],0,sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			m_pDataModel->calculated 		= false;
			m_pDataModel->INTO_TEMPLATE 	= false;
			m_pDataModel->INTO_CAPTURE	= false;
			m_pDlg->GetModel()->ocsp_defined = false;
			m_pDlg->GetModel()->active_coodinate_system = DCS;

			return true;
		}
		else
			return false;
}

// ===========================================================================================
// POMModel
// ===========================================================================================

// Instantiate template classes
DCP::BestFitModel::BestFitModel()
{
	INTO_TEMPLATE=false;INTO_CAPTURE=false;calculated=false;
	memset(&point_DCS[0],0, sizeof(S_POINT_BUFF) *MAX_BESTFIT_POINTS);
	memset(&point_OCS[0],0, sizeof(S_POINT_BUFF) *MAX_BESTFIT_POINTS);
	memset(&point_RES[0],0, sizeof(S_POINT_BUFF) *MAX_BESTFIT_POINTS);
	rms_x=0.0;
	rms_y=0.0;
	rms_z=0.0;
}


DCP::BestFitModel::~BestFitModel()
{
}
