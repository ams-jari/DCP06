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
#include <dcp06/orientation/Chst.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/3dFileView.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/orientation/BestFitSelectPoints.hpp>
#include <dcp06/core/Measure.hpp>
//#include <dcp06/calculation/CalcBestFit.hpp>
#include <dcp06/calculation/CalculationChst.hpp>
#include <dcp06/orientation/ResBestFit.hpp>

#include <GUI_Desktop.hpp>
#include <GUI_Application.hpp>
#include <GUI_MessageDialog.hpp>

#include <ABL_MsgDef.hpp>
#include <UTL_StringFunctions.hpp>
#include <TPI_MeasConfig.hpp>
#include <TBL_Util.hpp>

//#include <ABL_util.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::ChangeStationDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::ChangeStationDialog::ChangeStationDialog(DCP::BestFitModel* pModel,DCP::ChangeStationController* pController):
	m_pInfo1(0)/*, m_pFile(0)*/, m_pPoints(0),m_pPointMeas(0),m_pDataModel(pModel),LAST_SEL(0),
		m_pCurrentStation(0),m_pRemeasurePos2(0),m_pCreateNewStation(0),m_pHorizPlane(0),
		m_pHorizPlaneObserver(OBS_METHOD_TO_PARAM0(ChangeStationDialog, OnComboBoxChanged), this),
		disable_hz_plane(false),use_hz_plane(false), active_plane(0),m_pController(pController)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);

	
}
 
 
            // Description: Destructor
DCP::ChangeStationDialog::~ChangeStationDialog()
{
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::ChangeStationDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
		m_pCommon = new Common(GetModel());

		check_hz_plane_status();

	/* here check to active coordinate system, 
		
	   if OCSD or OSCU and compensator is ON then show that use Horiz Plane
	   also if OCSC and station number is 1 then hz plane can be use
	*/

/*
		int A321_ROTATION = m_pCommon->get_rotation_status();
		short iMin = (!A321_ROTATION  && GetModel()->align321_hz_plane ? 2: 3);
		*/
		/*

		TPI::MeasConfigC oMeasConfig;
	
		oMeasConfig.Get();
		bool bComp = oMeasConfig.GetCompensatorMode();
		bool ret1 =TBL::CheckCompensator();
		active_plane = 0;

		if(bComp && ret1)
		{
			// hz plane can be use if...
			disable_hz_plane = true;

			
			if(m_pDataModel->old_active_coodinate_system == OCSD )
			{
				use_hz_plane =  m_pCommon->get_rotation_status() ? false : true;
				disable_hz_plane = m_pCommon->get_rotation_status() ? true : false;
				active_plane = GetModel()->align321_active_plane;
			}
			else if(m_pDataModel->old_active_coodinate_system == OCSC)
			{
				use_hz_plane = true;
				disable_hz_plane = false;
			}
			else if(m_pDataModel->old_active_coodinate_system == DCS)
			{
				use_hz_plane = true;
				disable_hz_plane = false;
				active_plane = XY_PLANE;
			}
			else
			{
				use_hz_plane = false;
				disable_hz_plane = true;
			}
		}
		else
		{
			disable_hz_plane = true;
			use_hz_plane = false;
		}

		old_active_plane = active_plane;
		*/
		
		

		//short iMin = (!bComp || !ret1) ? 3 :2;


	

	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );


	m_pCurrentStation = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCurrentStation->SetId(eCurrentStation);
	m_pCurrentStation->SetText(StringC(AT_DCP06,P_DCP_CHST_CURRENT_STATION_TOK));
	void(m_pCurrentStation->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCurrentStation->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pCurrentStation->SetAutoColon(false);
	//m_pCurrentStation->SetColonPosition(9 * 25);
	AddCtrl(m_pCurrentStation);

	
	m_pHorizPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pHorizPlane->SetId(eHorizPlane);
	m_pHorizPlane->SetText(StringC(AT_DCP06,P_DCP_USE_HORIZONTAL_PLANE_TOK));
	m_pHorizPlane->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06, V_DCP_NO_TOK), NO);
	m_pHorizPlane->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06, V_DCP_YES_TOK), YES);
	//m_pHorizPlane->SetAutoColon(false);
	//m_pHorizPlane->SetColonPosition(9*25);
	


	m_pHorizPlane->GetComboBoxInputCtrl()->SetSelectedId(use_hz_plane ? YES : NO);
	
	if(disable_hz_plane)
	{
		m_pHorizPlane->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	}
	
	AddCtrl(m_pHorizPlane);

	//InsertEmptyLine(); CAPTIVATE

	m_pInfo1 = new GUI::TextCtrlC();
	m_pInfo1->SetId(eInfo1);
	m_pInfo1->SetText(StringC(AT_DCP06, L_DCP_CHST_DEFINE_TRANSFER_POINTS_TOK));
	AddCtrl(m_pInfo1);

	/* removed 271011
	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->GetTextCtrl()->SetTextTok(P_DCP_CHST_SELECT_FILE_TOK);
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	m_pFile->SetAutoColon(false);
	m_pFile->SetColonPosition(9 * 25);
	AddCtrl(m_pFile);
	*/

	m_pPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPoints->SetId(ePoints);
	m_pPoints->SetText(StringC(AT_DCP06, P_DCP_CHST_SELECT_POINTS_TOK));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPoints->SetAutoColon(false);
	//m_pPoints->SetColonPosition(9 * 25);
	AddCtrl(m_pPoints);

	m_pPointMeas = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointMeas->SetId(ePointMeas);
	m_pPointMeas->SetText(StringC(AT_DCP06, P_DCP_CHST_MEASURE_POINTS_TOK));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointMeas->SetAutoColon(false);
	//m_pPointMeas->SetColonPosition(9 * 25);
	AddCtrl(m_pPointMeas);

	//InsertEmptyLine();

	m_pRemeasurePos2 = new GUI::TextCtrlC();
	m_pRemeasurePos2->SetId(eRemeasPos2);
	m_pRemeasurePos2->SetText(StringC(AT_DCP06, L_DCP_CHST_REMEASURE_POS2_TOK));
	AddCtrl(m_pRemeasurePos2);

	m_pCreateNewStation = new GUI::TextCtrlC();
	m_pCreateNewStation->SetId(eCreateNewStation);
	m_pCreateNewStation->SetText(StringC(AT_DCP06, L_DCP_CHST_CREATE_NEW_STATION_TOK));
	AddCtrl(m_pCreateNewStation);

	//SetHelpTok(H_DCP_CHANGE_STATION_TOK,0);

	update_hz_plane_control();
	/*
	m_pHorizPlane->GetComboBoxInputCtrl()->SetSelectedId(use_hz_plane ? YES : NO);
	GUI::BaseCtrlC::CtrlStateT state =  m_pHorizPlane->GetComboBoxInputCtrl()->GetCtrlState();

	if(disable_hz_plane && state != GUI::BaseCtrlC::CS_Disabled)
		m_pHorizPlane->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	else if(!disable_hz_plane && state == GUI::BaseCtrlC::CS_Disabled)
		m_pHorizPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);
	*/	
	m_pHorizPlaneObserver.Attach(m_pHorizPlane->GetSubject());

	m_pController->check_function_keys();
	DCP06_TRACE_EXIT;
}

//
void DCP::ChangeStationDialog::check_hz_plane_status()
{
		TPI::MeasConfigC oMeasConfig;
	
		oMeasConfig.Get();
		//3.x
		//bool bComp = oMeasConfig.GetCompensatorMode();
		bool bComp = oMeasConfig.IsCompensatorEnabled();
		bool ret1 =TBL::CheckCompensator();
		active_plane = 0;

		if(bComp && ret1)
		{
			// hz plane can be use if...
			disable_hz_plane = true;
			
			//if(m_pDataModel->old_active_coodinate_system == OCSD )
			if(GetModel()->active_coodinate_system == OCSD)
			{

				use_hz_plane =  m_pCommon->get_rotation_status() ? false : true;
				disable_hz_plane = m_pCommon->get_rotation_status() ? true : false;
				active_plane = GetModel()->align321_active_plane;
			}
			else if(GetModel()->active_coodinate_system == OCSC)
			{
				
				disable_hz_plane = true;
				active_plane = GetModel()->chst_used_hz_plane;

				use_hz_plane = active_plane == 0 ? false : true;
			}
			else if(GetModel()->active_coodinate_system == DCS)
			{
				use_hz_plane = true;
				disable_hz_plane = false;
				active_plane = XY_PLANE;
			}
			else
			{
				use_hz_plane = false;
				disable_hz_plane = true;
			}
		}
		else
		{
			disable_hz_plane = true;
			use_hz_plane = false;
		}

		old_active_plane = active_plane;

		//m_pController->check_function_keys();

}
//
void DCP::ChangeStationDialog::OnDialogActivated()
{
	/* 011111 moved to controller */
	/*
	m_pDataModel->old_active_coodinate_system = GetModel()->active_coodinate_system;
	memcpy(m_pDataModel->matrix,GetModel()->ocsc_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->inv_matrix,GetModel()->ocsc_inv_matrix, sizeof(double) * 16);

	memcpy(&m_pDataModel->point_DCS[0], &GetModel()->CHST_point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&m_pDataModel->point_OCS[0], &GetModel()->CHST_point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&m_pDataModel->point_RES[0], &GetModel()->CHST_point_RES[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	LAST_SEL =  GetModel()->chst_last_sel;
	*/
	RefreshControls();
}

void DCP::ChangeStationDialog::update_hz_plane_control()
{
	m_pHorizPlane->GetComboBoxInputCtrl()->SetSelectedId(use_hz_plane ? YES : NO);
	GUI::BaseCtrlC::CtrlStateT state =  m_pHorizPlane->GetComboBoxInputCtrl()->GetCtrlState();

	if(disable_hz_plane && state != GUI::BaseCtrlC::CS_Disabled)
		m_pHorizPlane->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	else if(!disable_hz_plane && state == GUI::BaseCtrlC::CS_Disabled)
		m_pHorizPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadWrite);

}

short DCP::ChangeStationDialog::get_active_plane()
{
	return active_plane;
}

short DCP::ChangeStationDialog::get_min_point_count()
{
	return (active_plane == 0 || !get_horizontal_plane_in_use()) ? 3 : 2;
}

void DCP::ChangeStationDialog::OnComboBoxChanged(int unNotifyCode, int ulParam2)
{
	if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	{
		
		int ret = 0;

		ret = m_pHorizPlane->GetComboBoxInputCtrl()->GetSelectedId();
		
		if(ret == NO)
		{
			use_hz_plane = false;
			active_plane = 0;
		}
		else if(ret == YES)
		{
			use_hz_plane = true;
			active_plane = old_active_plane;
		}
		
		m_pController->check_function_keys();
	}
}

//
bool DCP::ChangeStationDialog::get_horizontal_plane_in_use()
{
	bool ret = false;

	int ind = 0;

		ind = m_pHorizPlane->GetComboBoxInputCtrl()->GetSelectedId();
		
		if(ind == NO)
		{
			ret = false;
		}
		else if(ind == YES)
		{
			ret = true;
		}

		return ret;
}



// Description: refresh all controls
void DCP::ChangeStationDialog::RefreshControls()
{	
	if(m_pInfo1/* && m_pFile*/ &&  m_pPoints && m_pPointMeas && m_pCurrentStation)
	{
		/*
		int A321_ROTATION = m_pCommon->get_rotation_status();
		short iMin = (!A321_ROTATION  && GetModel()->align321_hz_plane ? 2: 3);
		*/
		/*
		TPI::MeasConfigC oMeasConfig;
	
		oMeasConfig.Get();
		bool bComp = oMeasConfig.GetCompensatorMode();
		bool ret1 =TBL::CheckCompensator();

		short iMin = (!bComp || !ret1) ? 3 :2;
		*/

		short iMin = get_min_point_count();
		
		/*
		m_pHorizPlane->GetComboBoxInputCtrl()->SetSelectedId(use_hz_plane ? YES : NO);

		GUI::BaseCtrlC::CtrlStateT state =  m_pHorizPlane->GetComboBoxInputCtrl()->GetCtrlState();

		if(disable_hz_plane && state != GUI::BaseCtrlC::CtrlStateT::CS_Disabled)
			m_pHorizPlane->GetComboBoxInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CtrlStateT::CS_Disabled);
		else if(!disable_hz_plane && state == GUI::BaseCtrlC::CtrlStateT::CS_Disabled)
			m_pHorizPlane->GetComboBoxInputCtrl()->SetCtrlState(GUI::BaseCtrlC::CtrlStateT::CS_ReadWrite);
		*/
	
		StringC sTemp;
		char stNb[10];
		sprintf(stNb,"%d",GetModel()->stationNumber);
		m_pCurrentStation->GetStringInputCtrl()->SetString(StringC((const char*) stNb));

		int iPointCount = m_pCommon->get_OCS_points_count(&m_pDataModel->point_OCS[0], MAX_BESTFIT_POINTS);
		sTemp = L"-";

		

		/*
		if(iPointCount >= iMin && LAST_SEL ==1) sTemp = L"+";
		m_pFile->GetStringInputCtrl()->SetString(sTemp);
		*/

		sTemp = L"-";
		if(iPointCount >= iMin && LAST_SEL ==2) sTemp = L"+";
		m_pPoints->GetStringInputCtrl()->SetString(sTemp);

		sTemp = L"-";
		if(iPointCount >= iMin && LAST_SEL ==3) sTemp = L"+";
		m_pPointMeas->GetStringInputCtrl()->SetString(sTemp);
	}
}
 
void DCP::ChangeStationDialog::UpdateData()
{
	
	memcpy(GetModel()->ocsc_matrix,m_pDataModel->matrix, sizeof(double) * 16);
	memcpy(GetModel()->ocsc_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);

	memcpy(&GetModel()->CHST_point_DCS[0],&m_pDataModel->point_DCS[0],  sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy( &GetModel()->CHST_point_OCS[0], &m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&GetModel()->CHST_point_RES[0],&m_pDataModel->point_RES[0],  sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	GetModel()->chst_last_sel = LAST_SEL;

	GetModel()->chst_used_hz_plane = active_plane;

	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_CHST);
	GetModel()->poConfigController->StoreConfigData();

	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
	GetModel()->poConfigController->StoreConfigData();

	// update _ms_.adf
	update_ms_adf();
	//GetModel()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}
void DCP::ChangeStationDialog::update_ms_adf()
{
	//char bPid[ 100 ] ;
	char bXmea[ 100 ] ;
	char  bYmea[ 100 ] ;
	char bZmea[ 100 ] ;

	char bXdes[ 100 ] ;
	char bYdes[ 100 ] ;
	char bZdes[ 100 ] ;

	//char bXdif[ 100 ] ;
	//char bYdif[ 100 ] ;
	//char bZdif[ 100 ] ;

	char bNote[15];

	double x_conv = 0.0;
	double y_conv = 0.0;
	double z_conv = 0.0;
	
	char noteBuffer[STRING_BUFFER_MEDIUM];
	char stationBuffer[STRING_BUFFER_MEDIUM];
	char combinedNoteBuffer[STRING_BUFFER_MEDIUM];

	bool outOfCalc = false; 
	
	char btf_filename[100];
	btf_filename[0] = '\0';

	AdfFileFunc* pAdf = new AdfFileFunc(STA, GetModel());
	pAdf->always_single = 1;

	if(GetModel()->ADFFileName.GetLength() == 0)
		return;
	else
	{
		StringC strFileName = GetModel()->ADFFileName.Mid(0,GetModel()->ADFFileName.GetLength()-4);
		//UTL::UnicodeToAscii(btf_filename, strFileName);
		BSS::UTI::BSS_UTI_WCharToAscii(strFileName, btf_filename);
	}

	if(!pAdf->setFile(btf_filename))//"_bft_"))
	{
		pAdf->create_adf_file(btf_filename,"REF1",false);
		pAdf->setFile(btf_filename);//"_bft_");
	}
	/*
	if(!pAdf->setFile("_ms_"))
	{
		pAdf->create_adf_file("_ms_","REF1",false);
		pAdf->setFile("_ms_");
	}
	*/
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
						
							// add current station number to note field
							// copy current note...
							sprintf(noteBuffer,"%-6.6s", pAdf->note_front);
							m_pCommon->strbtrim(noteBuffer);

							sprintf(stationBuffer,"%-d", GetModel()->stationNumber);
							stationBuffer[6] = '\0';
							m_pCommon->strbtrim(stationBuffer);

							// try to found current station number from the note field
							if(!strstr(noteBuffer,stationBuffer))
							{
							
								sprintf(combinedNoteBuffer,"%-s%-s%c",noteBuffer,stationBuffer,'\0');
								m_pCommon->strbtrim(combinedNoteBuffer);

								int len1 = (int) strlen(combinedNoteBuffer);
								if(len1 > 6)
								{
									int ii = 0;
									for(int x = (len1-6); x <= len1; x++)
									{
										pAdf->note_front[ii] = combinedNoteBuffer[x];
										ii++;
									}
									pAdf->note_front[6] = '\0';
								}
								else
								{
									sprintf(pAdf->note_front,"%-s",combinedNoteBuffer);		
									pAdf->note_front[6] = '\0';
								}
							}

						}
						else
						{
							//ei laskennassa: �l� p�ivit� mitattuja arvoja eik� Note:a
							/*
							if(GetModel()->stationNumber == 1)
							{
								sprintf(stationBuffer,"%-d", GetModel()->stationNumber);
								sprintf(pAdf->note_front,"%-s",stationBuffer);		
								pAdf->note_front[6] = '\0';
							}
							*/
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
						sprintf(bNote,"%-d", GetModel()->stationNumber);
						bNote[6] = '\0';

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
 
bool DCP::ChangeStationDialog::delete_chst()
{
		bool ret = false;
		short sRet = -1;
	
		StringC strText;
		//strText.LoadTxt(AT_PMP7,L_DCP_CHST_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_CHST_TOK);
		strMsg.Format(strMsg,GetModel()->stationNumber);

		//strMsg.LoadTxt(AT_PMP7,M_DCP_DELETE_ALL_TOK);
		//strMsg.Format(strMsg,(const wchar_t*)strText);
		MsgBox msgbox;
		sRet = msgbox.DeleteChstMessage(strMsg);

		if(sRet == 1) // ALL
		{
			m_pDataModel->calculated 		= false;
			//m_pDataModel->INTO_TEMPLATE 	= false;
			//m_pDataModel->INTO_CAPTURE	= false;
			GetModel()->ocsc_defined = false;
			
			// 011111
			GetModel()->active_coodinate_system = DCS;
			

			memset(&m_pDataModel->point_DCS[0], 0, sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			memset(&m_pDataModel->point_OCS[0], 0, sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			
			GetModel()->stationNumber = 1;

			ret = true;
			//return true;
		}
		else if(sRet == 2) // POS2 VALUES
		{
			m_pDataModel->calculated 		= false;
			GetModel()->ocsc_defined = false;
			memset(&m_pDataModel->point_DCS[0], 0, sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			//memset(&m_pDataModel->point_OCS[0], 0, sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			//LAST_SEL = 0;
			//return true;
			ret = true;
		}
		else
			ret = false;
			//return false;
		
		check_hz_plane_status();
		update_hz_plane_control();

		m_pController->check_function_keys();
		
		return ret;
}

// Description: only accept DCP06 Model objects
bool DCP::ChangeStationDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
		m_pDataModel->old_active_coodinate_system = pDcpModel->active_coodinate_system;
		memcpy(m_pDataModel->matrix, pDcpModel->ocsc_matrix, sizeof(double) * 16);
		memcpy(m_pDataModel->inv_matrix,pDcpModel->ocsc_inv_matrix, sizeof(double) * 16);

		memcpy(&m_pDataModel->point_DCS[0], &pDcpModel->CHST_point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&m_pDataModel->point_OCS[0], &pDcpModel->CHST_point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&m_pDataModel->point_RES[0], &pDcpModel->CHST_point_RES[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		LAST_SEL =  pDcpModel->chst_last_sel;

        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: DCP06 model
DCP::Model* DCP::ChangeStationDialog::GetModel() const
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
DCP::ChangeStationController::ChangeStationController(Model* pModel)
    : m_pDlg( nullptr ),m_pCommon(0), m_pFileModel(0),m_pMsgBox(0), m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_CHANGE_STATION_POS1_TOK));
	
	// create model
	m_pDataModel = new BestFitModel;
	m_pDataModel->bestFit_chst = 1;

	m_pFileModel = new FileModel (pModel);


	


    // Create a dialog
    m_pDlg = new DCP::ChangeStationDialog(m_pDataModel, this);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CHST_DLG, m_pDlg, true );
	
	m_pMsgBox = new MsgBox;

    // Set the function key
	 
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;

	//vDef.nLable	= K_DCP_FILE_TOK;
	//SetFunctionKey( FK1, vDef );

	vDef.strLable	= StringC(AT_DCP06,K_DCP_CHST_SELECT_REF_POINTS_TOK);
	SetFunctionKey( FK1, vDef );
    
	//vDef.nLable	= K_DCP_MEAS_TOK;
	vDef.strLable	= StringC(AT_DCP06,K_DCP_CHST_MEAS_REF_POINTS_TOK);
	SetFunctionKey( FK2, vDef );

	//vDef.nLable	= K_DCP_CALC_TOK;
	//SetFunctionKey( FK4, vDef );
	
	//vDef.nLable	= K_DCP_POS2_TOK;

	vDef.strLable	= StringC(AT_DCP06,K_DCP_CHST_REMEAS_POS2_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable	= StringC(AT_DCP06,K_DCP_CHST_NEWST_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable	= StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable	= StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );
	/*
	vDef.nLable	= K_DCP_RMS_TOK;
	SetFunctionKey( SHFK3, vDef );

	vDef.nLable	= K_DCP_RESIDUALS_TOK;
	SetFunctionKey( SHFK4, vDef );
	*/
	vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
	SetFunctionKey( SHFK5, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );
} //lint !e818 Pointer parameter could be declared as pointing to const

// **************************************************************************
// Description: destructor
// **************************************************************************
DCP::ChangeStationController::~ChangeStationController()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}


	if(m_pMsgBox)
	{
		delete m_pMsgBox;
		m_pMsgBox = 0;
	}
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}

	if(m_pFileModel)
	{
		delete m_pFileModel;
		m_pFileModel = 0;
	}
}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::ChangeStationController::OnControllerActivated(void)
{
	check_function_keys();
}

// **************************************************************************
// Description: Route model to everybody else
// **************************************************************************
bool DCP::ChangeStationController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
	bool ret  = m_pDlg->SetModel( pModel );
	
	m_pCommon = new Common(m_pDlg->GetModel());
	
    return ret;
}

// **************************************************************************
// **************************************************************************
/*
void DCP::ChangeStationController::OnF1Pressed()
{
	// check....
	int min=3, max=MAX_BESTFIT_POINTS;

	if(!check_values(min,max))
		return;

	StringC sText;
	sText.LoadTxt(AT_DCP06,M_CHG_INST_POS_TOK);
	m_pMsgBox->ShowMessageOk(sText);

	// select file
	DCP::SelectFileModel* pModel = new SelectFileModel;
	if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
	{
		StringC sTitle = GetTitleStr();	
		(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle,false) );
	}
	(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_FILE_CONTROLLER, true);
	
}
*/
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnF1Pressed()
{
   if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	int min=3, max=MAX_BESTFIT_POINTS;

	/*
	int A321_ROTATION = m_pCommon->get_rotation_status();
	min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	min = (!bComp || !ret1) ? 3 :2;
	*/

	min = m_pDlg->get_min_point_count();

	// removed 041111
	//if(!check_values(min,max))
	//	return;

	//StringC sText;
	//sText.LoadTxt(AT_DCP06,M_CHG_INST_POS_TOK);
	//m_pMsgBox->ShowMessageOk(sText);

	DCP::BestFitSelectPointsModel* pModel = new BestFitSelectPointsModel;
	//Common common;

	memcpy(&pModel->points[0], &m_pDataModel->point_OCS[0],sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&pModel->points1[0], &m_pDataModel->point_DCS[0],sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	pModel->iDisplay = CHST_DLG;
	pModel->iMaxPoint = MAX_BESTFIT_POINTS;
	pModel->iMinPoint = min;
	pModel->iCurrentPoint = 1;
	pModel->m_iPointsCount = m_pCommon->get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_BESTFIT_POINTS);
	pModel->disable_point_editing = true;
	sprintf(pModel->default_pid,"%-s","REF");

	// 271011
	// check pid
	int to = pModel->m_iPointsCount < min ? min :  pModel->m_iPointsCount;
	for(int i = 0; i < to; i++)
	{
		sprintf(pModel->points[i].point_id,"REF%d",i+1);
	}

	//m_pDataModel->INTO_CAPTURE = true;
	if(GetController(CHST_POINT_CONTROLLER) == nullptr)
	{
		(void)AddController( CHST_POINT_CONTROLLER, new DCP::BestFitSelectPointsController(m_pDlg->GetModel()));
	}
	(void)GetController( CHST_POINT_CONTROLLER )->SetModel(pModel);
	SetActiveController(CHST_POINT_CONTROLLER, true);

}
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnF2Pressed()
{ 
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	int min=3, max=MAX_BESTFIT_POINTS;

	//Common common;
	// added 041111
	/*
	int A321_ROTATION = m_pCommon->get_rotation_status();
	min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	min = (!bComp || !ret1) ? 3 :2;
	*/
	min = m_pDlg->get_min_point_count();

	// removed 041111
	//if(!check_values(min,max))
	//	return;

	//StringC sText;
	//sText.LoadTxt(AT_DCP06,M_DCP_MEAS_TRANSFER_POINTS_TOK);
	//m_pMsgBox->ShowMessageOk(sText);


	
	int sum = m_pCommon->get_last_defined_point(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0],MAX_BESTFIT_POINTS);
		
	//old_coordinate = m_pDCP06PomDlg->GetModel()->active_coodinate_system;
	//m_pDlg->GetModel()->active_coodinate_system = DCS;
				
		DCP::MeasureModel* pModel = new MeasureModel;
		pModel->m_iMaxPoint = MAX_BESTFIT_POINTS;
		pModel->m_iMinPoint = min;
		pModel->m_iPointsCount = sum;
		pModel->disable_point_editing = true;
		
		sprintf(pModel->default_pid,"%-s","REF");

		memcpy(&pModel->point_table[0],	&m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&pModel->point_table2[0],	&m_pDataModel->point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

		// 271011
		// check pid
		int to = sum < min ? min : sum;
		for(int i = 0; i < to; i++)
		{
			sprintf(pModel->point_table[i].point_id,"REF%d",i+1);
		}

		if(GetController(MEAS_CONTROLLER) == nullptr)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
		}
		(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_CHANGE_STATION_POS1_MEAS_TOK));
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);

}

// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnF4Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
		int min=3, max=MAX_BESTFIT_POINTS;
	int i;

	//Common common;

	// removed 041111
	//check_values(min,max);
	// added 041111
	/*
	int A321_ROTATION = m_pCommon->get_rotation_status();
	min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	min = (!bComp || !ret1) ? 3 :2;
	*/
	min = m_pDlg->get_min_point_count();

	int count = 0;
	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_OCS[i].sta != 0)
		{
			count++;
		}
	}
	if(count < min)
	{	
		MsgBox MsgBox;
		StringC sMsg;
		
		if(min ==2)
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_2_POINTS_TOK);	
		}
		else
		{	
			sMsg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_3_POINTS_TOK);
		}
		MsgBox.ShowMessageOk(sMsg);
		return;
	}
	
	m_pDlg->GetModel()->active_coodinate_system = DCS;

	

	// set values...
	DCP::MeasureModel* pModel = new MeasureModel;
	pModel->disable_point_editing = true;
	pModel->m_iMaxPoint = 20;
	pModel->m_iMinPoint = min;
	pModel->m_iPointsCount = m_pCommon->get_last_defined_point(&m_pDataModel->point_OCS[0], MAX_BESTFIT_POINTS);

	
	memcpy(&pModel->point_table[0],	&m_pDataModel->point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&pModel->point_table2[0],&m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	
	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(pModel->point_table2[i].sta != 0)
		{
			sprintf(pModel->point_table[i].point_id,"%-6.6s",pModel->point_table2[i].point_id);
		}
	}
	// load measurement display
 
	if(GetController(MEAS_POS2_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_POS2_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
	}

	StringC msg;
	msg.LoadTxt(AT_DCP06,T_DCP_CHST_POS2_MEAS_TOK);
	msg.Format(msg,m_pDlg->GetModel()->stationNumber);

	//(void)GetController(MEAS_POS2_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_CHST_POS2_MEAS_TOK);
	(void)GetController(MEAS_POS2_CONTROLLER)->SetTitle(msg);

	(void)GetController( MEAS_POS2_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_POS2_CONTROLLER, true);
}
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnF5Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	int min=3, max=MAX_BESTFIT_POINTS;
	int i;

	//Common common;

	// removed 041111
	//check_values(min,max);
	// added 041111
	/*
	int A321_ROTATION = m_pCommon->get_rotation_status();
	min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	min = (!bComp || !ret1) ? 3 :2;
	*/

	min = m_pDlg->get_min_point_count();

	int count = 0;
	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_OCS[i].sta != 0)
		{
			count++;
		}
	}

	MsgBox MsgBox;
	StringC sMsg;
	
	if(count < min)
	{	
		//MsgBox MsgBox;
		//StringC sMsg;
		
		if(min ==2)
		{
			sMsg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_2_POINTS_TOK);	
		}
		else
		{	
			sMsg.LoadTxt(AT_DCP06,M_DCP_IN_MIN_3_POINTS_TOK);
		}
		MsgBox.ShowMessageOk(sMsg);
		return;
	}
	
	// 071111
	//check if measured values exists
	int count2 = 0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_DCS[i].sta != 0)
		{
			count2++;
		}
	}
	/* 130212
	if(count2 > 0)
	{
		sMsg.LoadTxt(AT_DCP06,M_DCP_CHST_DELETE_STA_POINTS_TOK);
		sMsg.Format(sMsg, m_pDlg->GetModel()->stationNumber);

		if(MsgBox.ShowMessageYesNo(sMsg))
		{
		}
		else
		{
			return;
		}

	} 
	*/
		// show always the message
		sMsg.LoadTxt(AT_DCP06,M_DCP_CHST_DELETE_STA_POINTS_TOK);
		sMsg.Format(sMsg, m_pDlg->GetModel()->stationNumber);

		if(MsgBox.ShowMessageYesNo(sMsg))
		{
		}
		else
		{
			return;
		}
	
	// end added 071111

	m_pDlg->GetModel()->active_coodinate_system = DCS;

	

	// set values...
	DCP::MeasureModel* pModel = new MeasureModel;
	pModel->m_iMaxPoint = 20;
	pModel->m_iMinPoint = min;
	pModel->disable_point_editing = true;
	pModel->m_iPointsCount = m_pCommon->get_last_defined_point(&m_pDataModel->point_OCS[0], MAX_BESTFIT_POINTS);

	
	// delete pos2 measured points when create NEWST
	memset(&m_pDataModel->point_DCS[0], 0, sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	//memcpy(&pModel->point_table[0],	&m_pDataModel->point_DCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	memcpy(&pModel->point_table2[0],&m_pDataModel->point_OCS[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	m_pDlg->GetModel()->stationNumber++;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(pModel->point_table2[i].sta != 0)
		{
			sprintf(pModel->point_table[i].point_id,"%-6.6s",pModel->point_table2[i].point_id);
		}
	}
	// load measurement display
 
	if(GetController(MEAS_POS2_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_POS2_CONTROLLER, new DCP::MeasureController(m_pDlg->GetModel()) );
	}
	StringC msg;
	msg.LoadTxt(AT_DCP06,T_DCP_CHST_POS2_MEAS_TOK);
	msg.Format(msg,m_pDlg->GetModel()->stationNumber);

	//(void)GetController(MEAS_POS2_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_CHST_POS2_MEAS_TOK);
	(void)GetController(MEAS_POS2_CONTROLLER)->SetTitle(msg);

	(void)GetController( MEAS_POS2_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_POS2_CONTROLLER, true);

}

// **************************************************************************
// **************************************************************************
// Description: Handle change of position values
void DCP::ChangeStationController::OnF6Pressed()
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

// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnSHF2Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->delete_chst();
	
	update_dialog();
	//m_pDlg->RefreshControls();
	/*
	if(GetController(3) == nullptr)
	{
		(void)AddController( 3, new DCP::UserController );
	}
	(void)GetController( 3 )->SetModel(m_pInitDlg->GetModel());
	SetActiveController(3, true);
	*/
}
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnSHF3Pressed()
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
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnSHF4Pressed()
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
// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::OnSHF5Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	//if(!pAdf->setFile("_ms_"))
	if(m_pFileModel->m_pAdfFile->setFile("_ms_"))
	{
		if(!m_pFileModel->m_pAdfFile->IsOpen())
			return;
	}
	else
		return;
	
	if(GetController(_3DFILEVIEW_CONTROLLER) == nullptr)		
	{
		(void)AddController( _3DFILEVIEW_CONTROLLER, new DCP::FileView3DController (m_pFileModel));//, false));CAPTIVATE TBD
	}
	(void)GetController( _3DFILEVIEW_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(_3DFILEVIEW_CONTROLLER, true);
	
}
// **************************************************************************
// **************************************************************************
// Description: React on close of tabbed dialog
void DCP::ChangeStationController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// **************************************************************************
// **************************************************************************
// Description: React on close of controller
void DCP::ChangeStationController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// selected file, pos1 
	/*
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		//m_pDCP06PomDlg->SelectFile(strSelectedFile);

		// select points from file
		//char filename_temp[20];
		//UTL::UnicodeToAscii(filename_temp, strSelectedFile);

		// create
		AdfFileFunc adf;
		adf.always_single = 1;
		Common common;  

		if(adf.setFile(strSelectedFile))
		{
			short count = 0;
			m_pDataModel->calculated = false; 

			char bXmea[15], bYmea[15], bZmea[15];
			char bXdes[15], bYdes[15], bZdes[15],pid[7];//,fname[13];

			for(int i = 1; i <= adf.getPointsCount(); i++)
			{
				adf.select_pnt1((long) i, pid, nullptr, bXmea, bXdes, nullptr, bYmea, bYdes, nullptr, bZmea, bZdes, nullptr);
				 
				sprintf(m_pDataModel->point_OCS[i-1].point_id,"%-6.6s",pid);
				strcpy(m_pDataModel->point_DCS[i-1].point_id, m_pDataModel->point_OCS[i-1].point_id);

				m_pDataModel->point_OCS[i-1].x = atof(bXmea);
				m_pDataModel->point_OCS[i-1].y = atof(bYmea);
				m_pDataModel->point_OCS[i-1].z = atof(bZmea);
				m_pDataModel->point_OCS[i-1].sta=(!common.strblank(bXmea) && !common.strblank(bYmea) && !common.strblank(bZmea)) ? POINT_DESIGN: POINT_NOT_DEFINED;
				count++;
				if(i >= MAX_BESTFIT_POINTS)
				{
					break;
				}
				//m_pPomModel->INTO_TEMPLATE = true;
			}
			m_pDlg->LAST_SEL = 1;
		}
	}
	// selected point , pos1
 	else 
	*/
	if(lCtrlID == CHST_POINT_CONTROLLER) 
	{
		DCP::BestFitSelectPointsModel* pModel = (DCP::BestFitSelectPointsModel*) GetController( CHST_POINT_CONTROLLER )->GetModel();

		Common common(m_pDlg->GetModel());
		
		memcpy(&m_pDataModel->point_OCS[0], &pModel->points[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		memcpy(&m_pDataModel->point_DCS[0], &pModel->points1[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
		m_pDataModel->calculated = false;
		m_pDlg->LAST_SEL = 2;


		// 271011 update _ms_.adf file
		//m_pDlg->update_ms_adf();
	}
	// measured points, pos1
	if(lCtrlID == MEAS_CONTROLLER) 
	{
		//m_pDlg->GetModel()->active_coodinate_system = old_coordinate;
		
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();	

			memcpy(&m_pDataModel->point_OCS[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			// otettu matkaan 041111
			memcpy(&m_pDataModel->point_DCS[0], &pModel->point_table2[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			
			short i;
			for(i=0; i < MAX_BESTFIT_POINTS; i++)
			{
				//if(pModel->point_table2[i].sta != 0)
				//{
					sprintf(m_pDataModel->point_DCS[i].point_id,"%-6s",m_pDataModel->point_OCS[i].point_id);
				//}
			}

			m_pDataModel->calculated = false;
			m_pDlg->LAST_SEL = 3;
		}	
	}
	
	// measured points, pos1
	if(lCtrlID == MEAS_POS2_CONTROLLER) 
	{
		m_pDlg->GetModel()->active_coodinate_system = m_pDataModel->old_active_coodinate_system;
		
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_POS2_CONTROLLER )->GetModel();	

			memcpy(&m_pDataModel->point_OCS[0], &pModel->point_table2[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
			memcpy(&m_pDataModel->point_DCS[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

			m_pDataModel->calculated = false;
			
			CalcChangeStation calcpom(m_pDataModel,m_pDlg->GetModel(),m_pDlg->get_active_plane());
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
				if(ret == 1)
				{
					if(GetController(RES_BESTFIT_CONTROLLER) == nullptr)
					{
						StringC sTitle = GetTitle();	
						(void)AddController( RES_BESTFIT_CONTROLLER, new DCP::ResBestFitController(m_pDataModel) );
					}
					(void)GetController( RES_BESTFIT_CONTROLLER )->SetModel(m_pDlg->GetModel());
					SetActiveController(RES_BESTFIT_CONTROLLER, true);
				}
				else if(ret == -1) // Abort
				{
					StringC sMsg;
					sMsg.LoadTxt(AT_DCP06,M_DCP_ORIENTATION_LOST_TOK);
					msgbox.ShowMessageOk(sMsg);
					m_pDataModel->calculated = false;
				}
				else	// remeasure
				{
					StringC sMsg;
					sMsg.LoadTxt(AT_DCP06,M_DCP_REMEASURE_POS2_POINTS_TOK);
					msgbox.ShowMessageOk(sMsg);
					m_pDataModel->calculated = false;
				}
			}
			else
			{
				m_pDlg->GetModel()->active_coodinate_system = m_pDataModel->old_active_coodinate_system;						
			}
		}	
	}

	if(lCtrlID == RES_BESTFIT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->calculated = true;
		m_pDlg->GetModel()->active_coodinate_system = OCSC;
		m_pDlg->GetModel()->ocsc_defined = true;
		m_pDlg->update_ms_adf();
	}


	update_dialog();
	//m_pDlg->RefreshControls();
	DestroyController( lCtrlID );

	// Close display(and controller)
	if(lCtrlID == RES_BESTFIT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		Close(EC_KEY_CONT);
	}
}

// **************************************************************************
// **************************************************************************
bool DCP::ChangeStationController::check_values(int& min, int& count)
{
	if (m_pDlg == nullptr)
	{
		USER_APP_VERIFY( false );
		return false;
	}
	
	int trans_points = 0;

	// check if points existing
	Common common(m_pDlg->GetModel());

	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_OCS[i].sta != 0 && m_pDataModel->point_DCS[i].sta != 0)
			trans_points++;
	}

	int sum = common.get_last_defined_point(&m_pDataModel->point_OCS[0], &m_pDataModel->point_DCS[0], MAX_BESTFIT_POINTS);
	
	int points_defined = trans_points;
	
	if(points_defined > 0)
	{
		if(!m_pDlg->delete_chst())
		{
			//return;
		}
		else
			trans_points = 0;
	}
	/* removed 28122012
	int A321_ROTATION = common.get_rotation_status();
	min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	min = (!bComp || !ret1) ? 3 :2;
	*/
	min = m_pDlg->get_min_point_count();

	if(trans_points <= 2)
		trans_points = min;
	else if(trans_points > MAX_BESTFIT_POINTS)
		trans_points = MAX_BESTFIT_POINTS;

	if(sum < min) sum = min;

	count = sum;

	return true;
}

// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::update_dialog()
{
	check_function_keys();
	m_pDlg->RefreshControls();
}

// **************************************************************************
// **************************************************************************
void DCP::ChangeStationController::check_function_keys()
{
	bool disable_newst = false;
	bool disable_remeas = false;

	int ref_count = 0;
	int pos2_count = 0;

	//int A321_ROTATION = m_pDlg->m_pCommon->get_rotation_status();
	/* removed 28122012
	int A321_ROTATION =m_pCommon->get_rotation_status();
	int min = (!A321_ROTATION  && m_pDlg->GetModel()->align321_hz_plane ? 2: 3);
	*/
	/*
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	bool bComp = oMeasConfig.GetCompensatorMode();
	bool ret1 =TBL::CheckCompensator();

	int min = (!bComp || !ret1) ? 3 :2;
	*/
	int min = m_pDlg->get_min_point_count();

	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_OCS[i].sta != 0)
		{
			ref_count++;
			if(m_pDataModel->point_DCS[i].sta != 0)
				pos2_count++;
		}
	}
	
	if(ref_count < min)
	{
		DisableFunctionKey(FK4);
		DisableFunctionKey(FK5);
	}
	if(ref_count >= min)
	{
		if(m_pDlg->GetModel()->stationNumber == 1)
		{
			DisableFunctionKey(FK4);
			EnableFunctionKey(FK5);
		}
		else
		{
			EnableFunctionKey(FK4);
			EnableFunctionKey(FK5);

		}
	}

	// check if we can measure the reference points ie. ...

	//if(m_pDataModel->old_active_coodinate_system != DCS)
		EnableFunctionKey(FK2);
	//else
	//	DisableFunctionKey(FK2);

	// check view
	AdfFileFunc adf(m_pModel);
	if(adf.setFile("_ms_"))
		EnableFunctionKey(SHFK5);
	else
		DisableFunctionKey(SHFK5);

	GUI::DesktopC::Instance()->UpdateFunctionKeys();
			

}

// ===========================================================================================
// POMModel
// ===========================================================================================
/*
// Instantiate template classes
DCP::ChangeStationModel::ChangeStationModel()
{
	calculated=false;
	memset(&point_DCS[0],0,sizeof(S_POINT_BUFF)* MAX_BESTFIT_POINTS);
	memset(&point_OCS[0],0,sizeof(S_POINT_BUFF)* MAX_BESTFIT_POINTS);
	memset(&point_RES[0],0,sizeof(S_POINT_BUFF)* MAX_BESTFIT_POINTS);
	// copy from DCP05Model
}


DCP::ChangeStationModel::~ChangeStationModel()
{

}
*/