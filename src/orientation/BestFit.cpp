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
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/core/Common.hpp>
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
/*
	 FKDef vDef;
	vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;

	vDef.nLable	= K_DCP_FILE_TOK;
	SetFunctionKey( FK1, vDef );
*/
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
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
//	char bPid[ 100 ] ;
	char bXmea[ 100 ] ;
	char  bYmea[ 100 ] ;
	char bZmea[ 100 ] ;

	char bXdes[ 100 ] ;
	char bYdes[ 100 ] ;
	char bZdes[ 100 ] ;

//	char bXdif[ 100 ] ;
//	char bYdif[ 100 ] ;
//	char bZdif[ 100 ] ;

	char bNote[15];

	double x_conv = 0.0;
	double y_conv = 0.0;
	double z_conv = 0.0;
	
//	char temp[30];
//	char temp1[30];
//	char temp2[30];

	bool outOfCalc = false; 
	
	char btf_filename[100];
	btf_filename[0] = '\0';

	if(GetModel()->ADFFileName.GetLength() == 0)
		return;
	else
	{
		StringC strFileName = GetModel()->ADFFileName.Mid(0,GetModel()->ADFFileName.GetLength()-4);
		//UTL::UnicodeToAscii(btf_filename, strFileName);
		BSS::UTI::BSS_UTI_WCharToAscii(strFileName, btf_filename);
	}
	
	AdfFileFunc* pAdf = new AdfFileFunc(BFT, GetModel());
	pAdf->always_single = 1;

	if(!pAdf->setFile(btf_filename))//"_bft_"))
	{
		pAdf->create_adf_file(btf_filename,"BF1",false);
		pAdf->setFile(btf_filename);//"_bft_");
	}

	int iCount = 0;

	if(pAdf->IsOpen())
	{
		for(int i = 0; i < MAX_BESTFIT_POINTS; i++)
		{
			if(!m_pCommon->strblank(m_pDataModel->point_OCS[i].point_id))
			{
				iCount++;
				m_pCommon->copy_xyz_to_buffer(	&m_pDataModel->point_OCS[i].x, 
												&m_pDataModel->point_OCS[i].y, 
												&m_pDataModel->point_OCS[i].z,
												&bXdes[0],&bYdes[0],&bZdes[0],9, GetModel()->m_nDecimals);

				if(m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
				{

					x_conv = m_pDataModel->point_RES[i].x + m_pDataModel->point_OCS[i].x;
					y_conv = m_pDataModel->point_RES[i].y + m_pDataModel->point_OCS[i].y;
					z_conv = m_pDataModel->point_RES[i].z + m_pDataModel->point_OCS[i].z;


					m_pCommon->copy_xyz_to_buffer(	&x_conv,&y_conv,&z_conv, 
													&bXmea[0],&bYmea[0],&bZmea[0],
													9, GetModel()->m_nDecimals);
					outOfCalc = false;
					
				}
				else
				{
					outOfCalc = true;
					//sprintf(bNote,"%-d", GetModel()->stationNumber);
					//bNote[6] = '\0';
					//m_pCommon->empty_xyz_buffers(&bXmea[0],&bYmea[0],&bZmea[0],9);
				}

				long pno = pAdf->seek_pid(m_pDataModel->point_OCS[i].point_id);

				if(pno != 0)
				{
					// point is founded, update
					if(pAdf->form_pnt(pno)== 1)
					{
						m_pCommon->copy_xyz_to_buffer(	&m_pDataModel->point_OCS[i].x, 
														&m_pDataModel->point_OCS[i].y, 
														&m_pDataModel->point_OCS[i].z,
														&pAdf->xdes_front[0],&pAdf->ydes_front[0],&pAdf->zdes_front[0],
													   9, GetModel()->m_nDecimals);

						if(outOfCalc == false)
						{	
							// laskennassa matkassa, p�ivit� mitatut arvot ja note my�s

							m_pCommon->copy_xyz_to_buffer(&x_conv,&y_conv,&z_conv,
													  &pAdf->xmea_front[0],&pAdf->ymea_front[0],&pAdf->zmea_front[0],
													   9, GetModel()->m_nDecimals);
						
							// 17.8.2015 BestFit has not station number -> removed
							// add current station number to note field
							// copy current note...
							/*
							sprintf(temp,"%-6.6s", pAdf->note_front);
							m_pCommon->strbtrim(temp);

							sprintf(temp1,"%-d", GetModel()->stationNumber);
							temp1[6] = '\0';
							m_pCommon->strbtrim(temp1);

							// try to found current station number from the note field
							if(!strstr(temp,temp1))
							{
							
								sprintf(temp2,"%-s%-s%c",temp,temp1,'\0');
								m_pCommon->strbtrim(temp2);

								int len1 = strlen(temp2);
								if(len1 > 6)
								{
									int ii = 0;
									for(int x = (len1-6); x <= len1; x++)
									{
										pAdf->note_front[ii] = temp2[x];
										ii++;
									}
									pAdf->note_front[6] = '\0';
								}
								else
								{
									sprintf(pAdf->note_front,"%-s",temp2);		
									pAdf->note_front[6] = '\0';
								}
							}
							*/
						}
						else
						{
							//ei laskennassa: �l� p�ivit� mitattuja arvoja eik� Note:a
						}
						
				

						pAdf->form_save_pnt();
					}
				}
				// point not founded, add
				else
				{

					if(outOfCalc == true)
					{
						bNote[0] = '\0';
						pAdf->add_new_pnt(pAdf->get_file_pointer(), m_pDataModel->point_OCS[i].point_id,
													 nullptr,&bXdes[0],
													 nullptr,&bYdes[0],
													 nullptr,&bZdes[0],&bNote[0]);

					}
					else
					{
						bNote[0] = '\0';
						/*
						sprintf(bNote,"%-d", GetModel()->stationNumber);
						bNote[6] = '\0';
						*/
						pAdf->add_new_pnt(pAdf->get_file_pointer(), m_pDataModel->point_OCS[i].point_id,
													 &bXmea[0],&bXdes[0],
													 &bYmea[0],&bYdes[0],
													 &bZmea[0],&bZdes[0],&bNote[0]);
					}
				}
			}
		}
		/*
		// delete all points except 1 onr
		for(int j = pAdf->points; j > 1; j--)
		{
			pAdf->delete_point_from_adf(j);
		}
		int iCount = 0;

		for(int i = 0; i < MAX_BESTFIT_POINTS; i++)
		{
			if(!m_pCommon->strblank(m_pDataModel->point_OCS[i].point_id))
			{
				iCount++;

				m_pCommon->copy_xyz_to_buffer(	&m_pDataModel->point_OCS[i].x, 
												&m_pDataModel->point_OCS[i].y, 
												&m_pDataModel->point_OCS[i].z,
												&bXdes[0],&bYdes[0],&bZdes[0],9, GetModel()->m_nDecimals);
				
				if(m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
				{

					x_conv = m_pDataModel->point_RES[i].x + m_pDataModel->point_OCS[i].x;
					y_conv = m_pDataModel->point_RES[i].y + m_pDataModel->point_OCS[i].y;
					z_conv = m_pDataModel->point_RES[i].z + m_pDataModel->point_OCS[i].z;


					m_pCommon->copy_xyz_to_buffer(	&x_conv,&y_conv,&z_conv, 
													&bXmea[0],&bYmea[0],&bZmea[0],
													9, GetModel()->m_nDecimals);
				
					
				}
				else
				{
					m_pCommon->empty_xyz_buffers(&bXmea[0],&bYmea[0],&bZmea[0],9);
				}
				


				pAdf->add_new_pnt(pAdf->get_file_pointer(), m_pDataModel->point_OCS[i].point_id,
												 &bXmea[0],&bXdes[0],
												 &bYmea[0],&bYdes[0],
												 &bZmea[0],&bZdes[0],&m_pDataModel->point_DCS[i].note[0]);
				
			}
			else
				break;
		}
		pAdf->close_adf_file();
		if(pAdf->setFile("_ms_"))
		{
			if(pAdf->points > 1)
				pAdf->delete_point_from_adf(1);
		}
		*/
		//pAdf->close_adf_file();

		if(iCount == 0)
			pAdf->delete_adf_file(false);
		else
			pAdf->close_adf_file();
	}
	/*
	int iLast = m_pCommon->get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_BESTFIT_POINTS)
	
	if(iLast > 0)
	{
		
		pAdf->form_save_pnt();

	}
	*/
	delete pAdf;
	pAdf = 0;
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
		    USER_APP_VERIFY( false );
			return;
		}
		// check if points existing
		Common common(m_pDlg->GetModel());

		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_BESTFIT_POINTS) > 0)
		{
			if(!delete_bestFit())
			{
				return;
			}
		}
		
		DCP::SelectFileModel* pModel = new SelectFileModel;
		if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle,m_pModel) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
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
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);
	}
}
// calc
void DCP::BestFitController::OnF4Pressed()
{	
	/*
	sprintf(m_pDataModel->point_OCS[0].point_id,"%-s", "P1");
	m_pDataModel->point_OCS[0].x = 0.0;  m_pDataModel->point_OCS[0].y = 0.0;  m_pDataModel->point_OCS[0].z = 0.0; m_pDataModel->point_OCS[0].sta = 1;

	sprintf(m_pDataModel->point_OCS[1].point_id,"%-s", "P2");
	m_pDataModel->point_OCS[1].x = 1.0;  m_pDataModel->point_OCS[1].y = 0.0;  m_pDataModel->point_OCS[1].z = 0.0; m_pDataModel->point_OCS[1].sta = 1;


	sprintf(m_pDataModel->point_OCS[2].point_id,"%-s", "P3");
	m_pDataModel->point_OCS[2].x = 2.0;  m_pDataModel->point_OCS[2].y = 0.0;  m_pDataModel->point_OCS[2].z = 0.0; m_pDataModel->point_OCS[2].sta = 1;

	sprintf(m_pDataModel->point_OCS[3].point_id,"%-s", "P4");
	m_pDataModel->point_OCS[3].x = 0.0;  m_pDataModel->point_OCS[3].y = 1.0;  m_pDataModel->point_OCS[3].z = 0.0; m_pDataModel->point_OCS[3].sta = 1;

	sprintf(m_pDataModel->point_OCS[4].point_id,"%-s", "P5");
	m_pDataModel->point_OCS[4].x = 0.0;  m_pDataModel->point_OCS[4].y = 2.0;  m_pDataModel->point_OCS[4].z = 0.0; m_pDataModel->point_OCS[4].sta = 1;

	// DCS
	sprintf(m_pDataModel->point_DCS[0].point_id,"%-s", "P1");
	m_pDataModel->point_DCS[0].x = 0.0;  m_pDataModel->point_DCS[0].y = 0.1;  m_pDataModel->point_DCS[0].z = 0.0; m_pDataModel->point_DCS[0].sta = 1;

	sprintf(m_pDataModel->point_DCS[1].point_id,"%-s", "P2");
	m_pDataModel->point_DCS[1].x = 1.1;  m_pDataModel->point_DCS[1].y = 0.2;  m_pDataModel->point_DCS[1].z = 0.0; m_pDataModel->point_DCS[1].sta = 1;


	sprintf(m_pDataModel->point_DCS[2].point_id,"%-s", "P3");
	m_pDataModel->point_DCS[2].x = 2.2;  m_pDataModel->point_DCS[2].y = 0.0;  m_pDataModel->point_DCS[2].z = 0.0; m_pDataModel->point_DCS[2].sta = 1;

	sprintf(m_pDataModel->point_DCS[3].point_id,"%-s", "P4");
	m_pDataModel->point_DCS[3].x = 0.0;  m_pDataModel->point_DCS[3].y = 1.1;  m_pDataModel->point_DCS[3].z = 0.0; m_pDataModel->point_DCS[3].sta = 1;

	sprintf(m_pDataModel->point_DCS[4].point_id,"%-s", "P5");
	m_pDataModel->point_DCS[4].x = 0.0;  m_pDataModel->point_DCS[4].y = 2.2;  m_pDataModel->point_DCS[4].z = 0.0; m_pDataModel->point_DCS[4].sta = 1;
	*/

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
{	/*
    if (m_pInitDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(3) == nullptr)
	{
		(void)AddController( 3, new DCP::UserController );
	}
	(void)GetController( 3 )->SetModel(m_pInitDlg->GetModel());
	SetActiveController(3, true);
	*/
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

	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		//m_pDlg->SelectFile(strSelectedFile);

		// select points from file
		//char filename_temp[20];
		//UTL::UnicodeToAscii(filename_temp, strSelectedFile);

		// create
		AdfFileFunc adf(m_pModel);
		adf.always_single = 1;
		Common common(m_pModel);

		if(adf.setFile(strSelectedFile))
		{
			short count = 0;
			m_pDataModel->calculated = false;

			char bXmea[15], bYmea[15], bZmea[15];
			char bXdes[15], bYdes[15], bZdes[15],pid[POINT_ID_BUFF_LEN];//,fname[13];

			for(int i = 1; i <= adf.getPointsCount(); i++)
			{
				adf.select_pnt1((int) i, pid, nullptr, bXmea, bXdes, nullptr, bYmea, bYdes, nullptr, bZmea, bZdes, nullptr);
				 
				snprintf(m_pDataModel->point_OCS[i-1].point_id, sizeof(m_pDataModel->point_OCS[i-1].point_id), DCP_POINT_ID_FMT, pid);
				strcpy(m_pDataModel->point_DCS[i-1].point_id, m_pDataModel->point_OCS[i-1].point_id);

				m_pDataModel->point_OCS[i-1].x = atof(bXdes);
				m_pDataModel->point_OCS[i-1].y = atof(bYdes);
				m_pDataModel->point_OCS[i-1].z = atof(bZdes);
				m_pDataModel->point_OCS[i-1].sta=(!common.strblank(bXdes) && !common.strblank(bYdes) && !common.strblank(bZdes)) ? POINT_DESIGN: POINT_NOT_DEFINED;
				count++;
				if(i >= MAX_BESTFIT_POINTS)
				{
					break;
				}
				m_pDataModel->INTO_TEMPLATE = true;
			}
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
