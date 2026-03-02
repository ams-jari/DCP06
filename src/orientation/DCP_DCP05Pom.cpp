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
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/orientation/DCP_DCP05Pom.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/file/DCP_SelectFile.hpp>
#include <dcp06/orientation/DCP_PomSelectPoints.hpp>
#include <dcp06/core/DCP_DCP05Meas.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/file/DCP_AdfFileFunc.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/calculation/DCP_CalcPom.hpp>
#include <dcp06/orientation/DCP_ResPom.hpp>

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
// OBS_IMPLEMENT_EXECUTE(DCP::DCP05UnitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::DCP05PomDlgC::DCP05PomDlgC(DCP::DCP05PomModelC* pPomModel):m_pInfo1(0), m_pFile(0), m_pPoints(0), m_pInfo2(0),
			m_pPointMeas(0), m_pCalc(0),m_pDataModel(pPomModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP05);
}


            // Description: Destructor
DCP::DCP05PomDlgC::~DCP05PomDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP05PomDlgC::OnInitDialog(void)
{
	m_pCommon = new DCP05CommonC(GetDCP05Model());

	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );

	m_pInfo1 = new GUI::TextCtrlC();
	m_pInfo1->SetId(eInfo1);
	m_pInfo1->SetText(StringC(AT_DCP05,L_DCP_POM_SELECT_OCS_POINTS_TOK));
	AddCtrl(m_pInfo1);

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP05,P_DCP_TEMPLATE_SK_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pFile->SetAutoColon(false);
	//m_pFile->SetColonPosition(9*25);
	// m_pFile->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pFile);
	
	m_pPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPoints->SetId(ePoints);
	m_pPoints->SetText(StringC(AT_DCP05,P_DCP_OCS_POINTS_SK_TOK));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPoints->SetAutoColon(false);
	//m_pPoints->SetColonPosition(9*25);
	// m_pPoints->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPoints);

	m_pInfo2 = new GUI::TextCtrlC();
	m_pInfo2->SetId(eInfo2);
	m_pInfo2->SetText(StringC(AT_DCP05,L_DCP_POM_MEASURE_SCS_POINTS_TOK));
	
	AddCtrl(m_pInfo2);

	m_pPointMeas = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointMeas->SetId(ePointMeas);
	m_pPointMeas->SetText(StringC(AT_DCP05,P_DCP_POM_MEASURE_SCS_POINTS_TOK));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointMeas->SetAutoColon(false);
	//m_pPointMeas->SetColonPosition(9*25);
	// m_pPointMeas->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPointMeas);

	m_pCalc = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCalc->SetId(eCalc);
	m_pCalc->SetText(StringC(AT_DCP05,P_DCP_POM_CALC_TOK));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pCalc->SetAutoColon(false);
	//m_pCalc->SetColonPosition(9*25);
	// m_pCalc->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pCalc);

	//SetHelpTok(H_DCP_POM_TOK,0);
/*
	 FKDef vDef;
	vDef.nAppId = AT_DCP05;
	vDef.poOwner = this;

	vDef.nLable	= K_DCP_FILE_TOK;
	SetFunctionKey( FK1, vDef );
*/
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP05PomDlgC::OnDialogActivated()
{
	m_pDataModel->old_active_coodinate_system = GetDCP05Model()->active_coodinate_system;
	memcpy(m_pDataModel->matrix,GetDCP05Model()->ocsp_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->inv_matrix,GetDCP05Model()->ocsp_inv_matrix, sizeof(double) * 16);
	m_pDataModel->calculated = (GetDCP05Model()->ocsp_defined  && GetDCP05Model()->active_coodinate_system == OCSP) ? true :false;

	memcpy(&m_pDataModel->point_DCS[0], &GetDCP05Model()->POM_point_DCS[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	memcpy(&m_pDataModel->point_OCS[0], &GetDCP05Model()->POM_point_OCS[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	memcpy(&m_pDataModel->point_RES[0], &GetDCP05Model()->POM_point_RES[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	
	m_pDataModel->INTO_CAPTURE = GetDCP05Model()->pom_into_capture;
	m_pDataModel->INTO_TEMPLATE = GetDCP05Model()->pom_into_template;
	m_pDataModel->ocs_defined = GetDCP05Model()->ocsp_defined;
	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP05PomDlgC::RefreshControls()
{	
	if(m_pFile && m_pPoints && 	m_pPointMeas &&  m_pCalc)
	{
		DCP05CommonC common(GetDCP05Model());
		StringC sStat=L"-";
		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_POM_POINTS) >= 3 && m_pDataModel->INTO_TEMPLATE)
			sStat = L"+";

		m_pFile->GetStringInputCtrl()->SetString(sStat);
		
		sStat = L"-";
		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_POM_POINTS) >= 3 && m_pDataModel->INTO_CAPTURE)
			sStat = L"+";	
		
		m_pPoints->GetStringInputCtrl()->SetString(sStat);
		sStat = L"-";
		if(common.get_OCS_SCS_points_count(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_POM_POINTS) >= 3 )
			sStat = L"+";	

		m_pPointMeas->GetStringInputCtrl()->SetString(sStat);
		sStat = L"-";
		if(m_pDataModel->calculated == true && m_pDataModel->ocs_defined == true && GetDCP05Model()->active_coodinate_system == OCSP)
			sStat = L"+";

		m_pCalc->GetStringInputCtrl()->SetString(sStat);
	}
}

void DCP::DCP05PomDlgC::UpdateData()
{

	// copy values

	//m_pDataModel->old_active_coodinate_system = GetDCP05Model()->active_coodinate_system;
	memcpy(GetDCP05Model()->ocsp_matrix,m_pDataModel->matrix, sizeof(double) * 16);
	memcpy(GetDCP05Model()->ocsp_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);

	memcpy(&GetDCP05Model()->POM_point_DCS[0],&m_pDataModel->point_DCS[0],  sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	memcpy( &GetDCP05Model()->POM_point_OCS[0], &m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	memcpy(&GetDCP05Model()->POM_point_RES[0],&m_pDataModel->point_RES[0],  sizeof(S_POINT_BUFF) * MAX_POM_POINTS);

	GetDCP05Model()->pom_into_capture  = m_pDataModel->INTO_CAPTURE;
	GetDCP05Model()->pom_into_template = m_pDataModel->INTO_TEMPLATE;
	GetDCP05Model()->ocsp_defined = m_pDataModel->ocs_defined;
	if(m_pDataModel->ocs_defined)
		GetDCP05Model()->active_coodinate_system = OCSP;
	//else
	//	GetDCP05Model()->active_coodinate_system = DCS;
	GetDCP05Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_POM);
	GetDCP05Model()->poConfigController->StoreConfigData();

	GetDCP05Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
	GetDCP05Model()->poConfigController->StoreConfigData();

	update_bft_adf();

	//GetDCP05Model()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}

void DCP::DCP05PomDlgC::update_bft_adf()
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

	if(GetDCP05Model()->ADFFileName.GetLength() == 0)
		return;
	else
	{
		StringC strFileName = GetDCP05Model()->ADFFileName.Mid(0,GetDCP05Model()->ADFFileName.GetLength()-4);
		//UTL::UnicodeToAscii(btf_filename, strFileName);
		BSS::UTI::BSS_UTI_WCharToAscii(strFileName, btf_filename);
	}
	
	AdfFileFunc* pAdf = new AdfFileFunc(BFT, GetDCP05Model());
	pAdf->always_single = 1;

	if(!pAdf->setFile(btf_filename))//"_bft_"))
	{
		pAdf->create_adf_file(btf_filename,"BF1",false);
		pAdf->setFile(btf_filename);//"_bft_");
	}

	int iCount = 0;

	if(pAdf->IsOpen())
	{
		for(int i = 0; i < MAX_POM_POINTS; i++)
		{
			if(!m_pCommon->strblank(m_pDataModel->point_OCS[i].point_id))
			{
				iCount++;
				m_pCommon->copy_xyz_to_buffer(	&m_pDataModel->point_OCS[i].x, 
												&m_pDataModel->point_OCS[i].y, 
												&m_pDataModel->point_OCS[i].z,
												&bXdes[0],&bYdes[0],&bZdes[0],9, GetDCP05Model()->m_nDecimals);

				if(m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
				{

					x_conv = m_pDataModel->point_RES[i].x + m_pDataModel->point_OCS[i].x;
					y_conv = m_pDataModel->point_RES[i].y + m_pDataModel->point_OCS[i].y;
					z_conv = m_pDataModel->point_RES[i].z + m_pDataModel->point_OCS[i].z;


					m_pCommon->copy_xyz_to_buffer(	&x_conv,&y_conv,&z_conv, 
													&bXmea[0],&bYmea[0],&bZmea[0],
													9, GetDCP05Model()->m_nDecimals);
					outOfCalc = false;
					
				}
				else
				{
					outOfCalc = true;
					//sprintf(bNote,"%-d", GetDCP05Model()->stationNumber);
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
													   9, GetDCP05Model()->m_nDecimals);

						if(outOfCalc == false)
						{	
							// laskennassa matkassa, päivitä mitatut arvot ja note myös

							m_pCommon->copy_xyz_to_buffer(&x_conv,&y_conv,&z_conv,
													  &pAdf->xmea_front[0],&pAdf->ymea_front[0],&pAdf->zmea_front[0],
													   9, GetDCP05Model()->m_nDecimals);
						
							// 17.8.2015 BestFit has not station number -> removed
							// add current station number to note field
							// copy current note...
							/*
							sprintf(temp,"%-6.6s", pAdf->note_front);
							m_pCommon->strbtrim(temp);

							sprintf(temp1,"%-d", GetDCP05Model()->stationNumber);
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
							//ei laskennassa: älä päivitä mitattuja arvoja eikä Note:a
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
													 NULL,&bXdes[0],
													 NULL,&bYdes[0],
													 NULL,&bZdes[0],&bNote[0]);

					}
					else
					{
						bNote[0] = '\0';
						/*
						sprintf(bNote,"%-d", GetDCP05Model()->stationNumber);
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

		for(int i = 0; i < MAX_POM_POINTS; i++)
		{
			if(!m_pCommon->strblank(m_pDataModel->point_OCS[i].point_id))
			{
				iCount++;

				m_pCommon->copy_xyz_to_buffer(	&m_pDataModel->point_OCS[i].x, 
												&m_pDataModel->point_OCS[i].y, 
												&m_pDataModel->point_OCS[i].z,
												&bXdes[0],&bYdes[0],&bZdes[0],9, GetDCP05Model()->m_nDecimals);
				
				if(m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
				{

					x_conv = m_pDataModel->point_RES[i].x + m_pDataModel->point_OCS[i].x;
					y_conv = m_pDataModel->point_RES[i].y + m_pDataModel->point_OCS[i].y;
					z_conv = m_pDataModel->point_RES[i].z + m_pDataModel->point_OCS[i].z;


					m_pCommon->copy_xyz_to_buffer(	&x_conv,&y_conv,&z_conv, 
													&bXmea[0],&bYmea[0],&bZmea[0],
													9, GetDCP05Model()->m_nDecimals);
				
					
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
	int iLast = m_pCommon->get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_POM_POINTS)
	
	if(iLast > 0)
	{
		
		pAdf->form_save_pnt();

	}
	*/
	delete pAdf;
	pAdf = 0;
}

// Description: only accept hello world Model objects
bool DCP::DCP05PomDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP05ModelC* DCP::DCP05PomDlgC::GetDCP05Model() const
{
    return (DCP::DCP05ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  DCP05ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP05UnitControllerC
// 
DCP::DCP05PomControllerC::DCP05PomControllerC(DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_POM_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new DCP05PomModelC;
	m_pDataModel->pom_chst = 0;

    // Create a dialog
    m_pDlg = new DCP::DCP05PomDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( POM_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
	vDef.poOwner = this;

	vDef.strLable = StringC(AT_DCP05,K_DCP_FILE_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_TOK);
	SetFunctionKey( FK2, vDef );
    
	vDef.strLable = StringC(AT_DCP05,K_DCP_MEAS_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CALC_TOK);
	SetFunctionKey( FK4, vDef );
	
	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_RMS_TOK);
	SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_RESIDUALS_TOK);
	SetFunctionKey( SHFK4, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05PomControllerC::~DCP05PomControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// Description: Route model to everybody else
bool DCP::DCP05PomControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DCP05PomControllerC::OnF1Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		// check if points existing
		DCP05CommonC common(m_pDlg->GetDCP05Model());

		if(common.get_OCS_points_count(&m_pDataModel->point_OCS[0],MAX_POM_POINTS) > 0)
		{
			if(!delete_pom())
			{
				return;
			}
		}
		
		DCP::DCP05SelectFileModelC* pModel = new DCP05SelectFileModelC;
		if(GetController(SELECT_FILE_CONTROLLER) == NULL)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP05SelectFileControllerC(ONLY_ADF, sTitle,m_pDCP05Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
}

void DCP::DCP05PomControllerC::OnF2Pressed()
{
	
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP05PomSelectPointsModelC* pModel = new DCP05PomSelectPointsModelC;
	DCP05CommonC common(m_pDCP05Model);

	memcpy(&pModel->points[0], &m_pDataModel->point_OCS[0],sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	memcpy(&pModel->points1[0], &m_pDataModel->point_DCS[0],sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	pModel->iDisplay = POM_DLG;
	pModel->iMaxPoint = MAX_POM_POINTS;
	pModel->iMinPoint = 3;
	pModel->iCurrentPoint = 1;
	pModel->m_iPointsCount = common.get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_POM_POINTS);
	m_pDataModel->INTO_CAPTURE = true;
	
	sprintf(pModel->default_pid,"%-s","BF");

	int to = pModel->m_iPointsCount < pModel->iMinPoint ? pModel->iMinPoint :  pModel->m_iPointsCount;
	
	for(int i = 0; i < to; i++)
	{
		sprintf(pModel->points[i].point_id,"BF%d",i+1);
	}

	if(GetController(POM_POINT_CONTROLLER) == NULL)
	{
		(void)AddController( POM_POINT_CONTROLLER, new DCP::DCP05PomSelectPointsControllerC(m_pDlg->GetDCP05Model()));
	}
	(void)GetController( POM_POINT_CONTROLLER )->SetModel(pModel);
	SetActiveController(POM_POINT_CONTROLLER, true);
	
}
void DCP::DCP05PomControllerC::OnF3Pressed()
{	
	//short i;

    if (m_pDlg == NULL) 
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP05CommonC common(m_pDCP05Model);
	DCP05MsgBoxC msgbox;
	StringC msg;

	short sum = common.defined_pom_points(&m_pDataModel->point_OCS[0],NULL);
	if(sum==0)
	{
		msg.LoadTxt(AT_DCP05,M_DCP_NO_POINTS_TOK	);
		msgbox.ShowMessageOk(msg);
		//msgbox(TXT_NIL_TOKEN, M_NO_POINTS_TOK,MB_OK);
			//show_message("No points!");
	}
	else  
	{
		sum = common.get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_POM_POINTS);
		
		old_coordinate = m_pDlg->GetDCP05Model()->active_coodinate_system;
		m_pDlg->GetDCP05Model()->active_coodinate_system = DCS;
				
		DCP::DCP05MeasModelC* pModel = new DCP05MeasModelC;
		pModel->m_iMaxPoint = sum < 3 ? 3 : sum;
		pModel->m_iMinPoint = 3;
		pModel->m_iPointsCount = sum;
		pModel->disable_point_editing = true;
		
		memcpy(&pModel->point_table[0],	&m_pDataModel->point_DCS[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
		memcpy(&pModel->point_table2[0],&m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
	


		if(GetController(MEAS_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::DCP05MeasControllerC(m_pDlg->GetDCP05Model()) );
		}
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);
	}
}
// calc
void DCP::DCP05PomControllerC::OnF4Pressed()
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

	DCP05CalcPomC calcpom(m_pDataModel,m_pDlg->GetDCP05Model());
	if(calcpom.calc_transform())
	{
		m_pDataModel->calculated = true;
		

		StringC sMsg;
		
		StringC sX, sY, sZ, sRMS;
		sX.LoadTxt(AT_DCP05, P_DCP_X_TOK);
		sY.LoadTxt(AT_DCP05, P_DCP_Y_TOK);
		sZ.LoadTxt(AT_DCP05, P_DCP_Z_TOK);
		sRMS.LoadTxt(AT_DCP05, P_DCP_RMS_TOK);
		
		StringC sXline;
		StringC sYline;
		StringC sZline;
		char temp[50];
		
		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_x);
		sXline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sX, (const wchar_t*) StringC(temp));

		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_y);
		sYline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sY, (const wchar_t*) StringC(temp));

		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_z);
		sZline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sZ, (const wchar_t*) StringC(temp));
		

		sMsg = sXline;
		sMsg += L"\n";
		sMsg += sYline;
		sMsg += L"\n";
		sMsg += sZline;
		
		DCP05MsgBoxC msgbox;
		short ret = msgbox.ShowMessageOkAbortRetry(sMsg);
		if( ret == 1)
		{
			if(GetController(RES_POM_CONTROLLER) == NULL)
			{
				StringC sTitle = GetTitle();	
				(void)AddController( RES_POM_CONTROLLER, new DCP::DCP05ResPomControllerC(m_pDataModel) );
			}
			(void)GetController( RES_POM_CONTROLLER )->SetModel(m_pDlg->GetDCP05Model());
			SetActiveController(RES_POM_CONTROLLER, true);
		}
		else if(ret == -1) // ABort
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP05,M_DCP_ORIENTATION_LOST_TOK);
			msgbox.ShowMessageOk(sMsg);
			m_pDataModel->calculated = false;
		}
		else				// retry
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP05,M_DCP_REMEASURE_SCS_POINTS_TOK);
			msgbox.ShowMessageOk(sMsg);
			m_pDataModel->calculated = false;
		}
	}
	else
	{

	}
}

// Description: Handle change of position values
void DCP::DCP05PomControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
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

void DCP::DCP05PomControllerC::OnSHF2Pressed()
{	
	delete_pom();
	m_pDlg->RefreshControls();

}

// RMS
void DCP::DCP05PomControllerC::OnSHF3Pressed()
{	
	if(m_pDlg->GetDCP05Model()->ocsp_defined && m_pDataModel->calculated)
	{
		StringC sMsg;
		
		StringC sX, sY, sZ, sRMS;
		sX.LoadTxt(AT_DCP05, P_DCP_X_TOK);
		sY.LoadTxt(AT_DCP05, P_DCP_Y_TOK);
		sZ.LoadTxt(AT_DCP05, P_DCP_Z_TOK);
		sRMS.LoadTxt(AT_DCP05, P_DCP_RMS_TOK);
		
		StringC sXline;
		StringC sYline;
		StringC sZline;
		char temp[50];
		
		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_x);
		sXline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sX, (const wchar_t*) StringC(temp));

		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_y);
		sYline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sY, (const wchar_t*) StringC(temp));

		sprintf(temp,"%9.*f",m_pDlg->GetDCP05Model()->m_nDecimals, m_pDataModel->rms_z);
		sZline.Format(L"%s %s:%s",(const wchar_t*)sRMS,  (const wchar_t*)sZ, (const wchar_t*) StringC(temp));
		

		sMsg = sXline;
		sMsg += L"\n";
		sMsg += sYline;
		sMsg += L"\n";
		sMsg += sZline;
		
		DCP05MsgBoxC msgbox;
		msgbox.ShowMessageOk(sMsg);
	}
	
}

// RESIDUALS
void DCP::DCP05PomControllerC::OnSHF4Pressed()
{	/*
    if (m_pDCP05InitDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(GetController(3) == NULL)
	{
		(void)AddController( 3, new DCP::DCP05UserControllerC );
	}
	(void)GetController( 3 )->SetModel(m_pDCP05InitDlg->GetDCP05Model());
	SetActiveController(3, true);
	*/
}
// Description: React on close of tabbed dialog
void DCP::DCP05PomControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
 
// Description: React on close of controller
void DCP::DCP05PomControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == POM_POINT_CONTROLLER) 
	{
		DCP::DCP05PomSelectPointsModelC* pModel = (DCP::DCP05PomSelectPointsModelC*) GetController( POM_POINT_CONTROLLER )->GetModel();

		DCP05CommonC common(m_pDCP05Model);  
		
		memcpy(&m_pDataModel->point_OCS[0], &pModel->points[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
		memcpy(&m_pDataModel->point_DCS[0], &pModel->points1[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);

		// copy point ids
		for(int i = 1; i <= MAX_POM_POINTS; i++)
		{
			strcpy(m_pDataModel->point_DCS[i-1].point_id, m_pDataModel->point_OCS[i-1].point_id);
		}
		m_pDataModel->calculated = false;
	}
	if(lCtrlID == MEAS_CONTROLLER) 
	{
		m_pDlg->GetDCP05Model()->active_coodinate_system = old_coordinate;
		
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::DCP05MeasModelC* pModel = (DCP::DCP05MeasModelC*) GetController( MEAS_CONTROLLER )->GetModel();	

			memcpy(&m_pDataModel->point_DCS[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
			memcpy(&m_pDataModel->point_OCS[0], &pModel->point_table2[0], sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
			m_pDataModel->calculated = false;
		}	
	}

	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectFileModelC* pModel = (DCP::DCP05SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		//m_pDlg->SelectFile(strSelectedFile);

		// select points from file
		//char filename_temp[20];
		//UTL::UnicodeToAscii(filename_temp, strSelectedFile);

		// create
		AdfFileFunc adf(m_pDCP05Model);
		adf.always_single = 1;
		DCP05CommonC common(m_pDCP05Model);

		if(adf.setFile(strSelectedFile))
		{
			short count = 0;
			m_pDataModel->calculated = false;

			char bXmea[15], bYmea[15], bZmea[15];
			char bXdes[15], bYdes[15], bZdes[15],pid[7];//,fname[13];

			for(int i = 1; i <= adf.getPointsCount(); i++)
			{
				adf.select_pnt1((int) i, pid, NULL, bXmea, bXdes, NULL, bYmea, bYdes, NULL, bZmea, bZdes, NULL);
				 
				sprintf(m_pDataModel->point_OCS[i-1].point_id,"%-6.6s",pid);
				strcpy(m_pDataModel->point_DCS[i-1].point_id, m_pDataModel->point_OCS[i-1].point_id);

				m_pDataModel->point_OCS[i-1].x = atof(bXdes);
				m_pDataModel->point_OCS[i-1].y = atof(bYdes);
				m_pDataModel->point_OCS[i-1].z = atof(bZdes);
				m_pDataModel->point_OCS[i-1].sta=(!common.strblank(bXdes) && !common.strblank(bYdes) && !common.strblank(bZdes)) ? POINT_DESIGN: POINT_NOT_DEFINED;
				count++;
				if(i >= MAX_POM_POINTS)
				{
					break;
				}
				m_pDataModel->INTO_TEMPLATE = true;
			}
		}
	}
	if(lCtrlID == RES_POM_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->calculated = true;
		m_pDlg->GetDCP05Model()->active_coodinate_system = OCSP;
		m_pDlg->update_bft_adf();
	}


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

bool DCP::DCP05PomControllerC::delete_pom()
{
		StringC strPomText;
		strPomText.LoadTxt(AT_DCP05,L_DCP_POM_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP05,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strPomText);
		
		DCP05MsgBoxC msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			
			memset(&m_pDataModel->point_DCS[0],0,sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
			memset(&m_pDataModel->point_OCS[0],0,sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
			memset(&m_pDataModel->point_RES[0],0,sizeof(S_POINT_BUFF) * MAX_POM_POINTS);
			m_pDataModel->calculated 		= false;
			m_pDataModel->INTO_TEMPLATE 	= false;
			m_pDataModel->INTO_CAPTURE	= false;
			m_pDlg->GetDCP05Model()->ocsp_defined = false;
			m_pDlg->GetDCP05Model()->active_coodinate_system = DCS;

			return true;
		}
		else
			return false;
}

// ===========================================================================================
// POMModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP05PomModelC::DCP05PomModelC()
{
	INTO_TEMPLATE=false;INTO_CAPTURE=false;calculated=false;
	memset(&point_DCS[0],0, sizeof(S_POINT_BUFF) *MAX_POM_POINTS);
	memset(&point_OCS[0],0, sizeof(S_POINT_BUFF) *MAX_POM_POINTS);
	memset(&point_RES[0],0, sizeof(S_POINT_BUFF) *MAX_POM_POINTS);
	rms_x=0.0;
	rms_y=0.0;
	rms_z=0.0;
}


DCP::DCP05PomModelC::~DCP05PomModelC()
{
}
