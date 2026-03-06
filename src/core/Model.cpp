// ================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_Model.cpp $
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
#include "calc.h"

#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#ifndef DCP_USE_JSON_DATABASE
#define DCP_USE_JSON_DATABASE 1
#endif
#if DCP_USE_JSON_DATABASE
#include <dcp06/database/JsonDatabase.hpp>
#else
#include <dcp06/database/StubDatabase.hpp>
#endif

#include <stdio.h>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================



// ================================================================================================
// ======================================  Static Functions  ======================================
// ================================================================================================



// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Instantiate template classes
DCP::DCP06ModelC::DCP06ModelC():m_nOption(2),m_nAutoIncrement(0), m_nOverWriteInfo(YES), m_n2FaceMeas(NO/*ALL_MANUAL*/),
					m_nDesignValues(YES),m_nToolInfo(YES), m_nAverageCount(1), m_nLeftRightHand(RIGHTHAND),
					m_nAutoMatch(NO),m_nUnit(MM), m_nDecimals(1),ADFFileName(L""),active_coodinate_system(DCS),
					isMotorized(false),active_tool(0),isATR(false),tool_trans_x(0.0),tool_trans_y(0.0),tool_trans_z(0.0),
					poConfigController(0),stationNumber(1),bDemoMode(true), m_nAmsLog(NO),iStartCount(0),chst_last_sel(0), m_nSaveTool(NO),
					chst_used_hz_plane(0), startDate(0), lastStartedDate(0),fullDemoMode(false)

{
	
			sEnteredKeyCode[0] = '\0';
			sKeyCode[0] = '\0';
			sKeyCodeDemo1[0] = '\0';
			sKeyCodeDemo2[0] = '\0';
			sKeyCodeDemo3[0] = '\0';
			sKeyCodeDemo4[0] = '\0';
			sKeyCodeDemo5[0] = '\0';
			ocsd_defined = false;
			ocsp_defined = false;
			ocsc_defined = false;
			ocsu_defined = false;

			pom_into_template	 = 0;
			pom_into_capture	 = 0;

			circle_plane_type = CIRCLE_POINTS_PLANE;
			circle_cx = 0.0;
			circle_cy = 0.0;
			circle_cz = 0.0;
			circle_vi = 0.0;
			circle_vj = 0.0;
			circle_vk = 0.0;

			circle_diameter = 0.0;
			circle_rms = 0.0;

			memset(&circle_planes[0],0,sizeof(S_PLANE_BUFF));
			memset(&circle_points[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));

			// DOM
			dom_active_plane	= XY_PLANE;
			dom_active_line		= X_LINE; 
			dom_hz_plane		= false;
			memset(&dom_plane_buff[0],0,sizeof(S_PLANE_BUFF));
			memset(&dom_hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
			memset(&dom_line_buff[0], 0, sizeof(S_LINE_BUFF));
			memset(&dom_ovalues_buff, 0, sizeof(S_POINT_BUFF));
			memset(&dom_ovalues_tool_buff, 0, sizeof(S_POINT_BUFF));
			memset(&dom_ref_point_buff, 0, sizeof(S_POINT_BUFF));
			memset(&dom_rot_plane_buff, 0, sizeof(S_POINT_BUFF));
			memset(&dom_rot_line_buff, 0, sizeof(S_POINT_BUFF));

			// USERDEF
			userdef_active_plane	= XY_PLANE;
			userdef_active_line		= X_LINE; 
			userdef_hz_plane		= false;
			memset(&userdef_plane_buff[0],0,sizeof(S_PLANE_BUFF));
			memset(&userdef_hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
			memset(&userdef_line_buff[0], 0, sizeof(S_LINE_BUFF));
			memset(&userdef_ovalues_buff, 0, sizeof(S_POINT_BUFF));
			memset(&userdef_ovalues_tool_buff, 0, sizeof(S_POINT_BUFF));
			memset(&userdef_ref_point_buff, 0, sizeof(S_POINT_BUFF));
			memset(&userdef_rot_plane_buff, 0, sizeof(S_POINT_BUFF));
			memset(&userdef_rot_line_buff, 0, sizeof(S_POINT_BUFF));

			memset(&userdef_measured_points[0], 0, sizeof(S_POINT_BUFF) *MAX_USERDEF_POINTS);

			memset(&userdef_plane_points_no[0], 0, sizeof(short)* MAX_USERDEF_POINTS);
			memset(&userdef_line_points_no[0], 0, sizeof(short)* MAX_USERDEF_POINTS); 
			userdef_point_no = 0;

			// POM
			memset(&POM_point_DCS, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);
			memset(&POM_point_OCS, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);
			memset(&POM_point_RES, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);

			memset(&CHST_point_DCS, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);
			memset(&CHST_point_OCS, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);
			memset(&CHST_point_RES, 0, sizeof(S_POINT_BUFF)*MAX_BESTFIT_POINTS);

			memset(&tool_table[0], 0, sizeof(S_TOOL)* MAX_TOOLS);

			memset(&hidden_point_bar[0],0,sizeof(double) * MAX_POINTS_IN_HIDDENPOINT_BAR);


			memset(&home_points[0],0,sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);

			memset(&shaft_calc_plane[0],0,sizeof(S_PLANE_BUFF));
			memset(&shaft_line[0],0,sizeof(S_LINE_BUFF));
			shaft_circle_plane_type = CIRCLE_POINTS_PLANE;

			memset(&shaft_circle_points[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&shaft_circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
			shaft_circle_cx = 0.0;
			shaft_circle_cy = 0.0;
			shaft_circle_cz = 0.0;
			shaft_circle_vi = 0.0;
			shaft_circle_vj = 0.0;
			shaft_circle_vk = 0.0;
			shaft_circle_diameter = 0.0;
			shaft_ref_line = X_LINE;  // X,_LINE, Y_LINE_Z_LINE or MEAS_LINE
			shaft_tool_radius = 0.0;
			shaft_circle_rms = 0.0;
			
			// line fitting
			memset(&linefitting_line[0],0,sizeof(S_LINE_BUFF));
			memset(&linefitting_points[0],0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);	
			memset(&linefitting_results[0], 0, sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);

			// MID POINT
			memset(&mid_points[0],0, sizeof(S_POINT_BUFF) * MAX_MID_POINTS);	

			linefitting_manualHeight = 0.0;
			linefitting_shiftValue = 0.0;
			linefitting_rotateAngle = 0.0;
			linefitting_selectedHeight = FIRST_POINT;
			linefitting_selectedShift = SHIFT_NO;
			linefitting_selectedRotate = ROTATE_NO;
			linefitting_selectedRefLine = REF_LINE_HORIZONTAL;

			initialize_matrix4x4(dcs_matrix);	
			initialize_matrix4x4(ocsd_matrix);	
			initialize_matrix4x4(ocsp_matrix);	
			initialize_matrix4x4(ocsc_matrix);	
			initialize_matrix4x4(ocsu_matrix);
			
			initialize_matrix4x4(dcs_inv_matrix);	
			initialize_matrix4x4(ocsd_inv_matrix);	
			initialize_matrix4x4(ocsp_inv_matrix);	
			initialize_matrix4x4(ocsc_inv_matrix);	
			initialize_matrix4x4(ocsu_inv_matrix);

			/*SDI::VersionInfoC oVers;
			
			bool ret = ReadAppVersion(AT_DCP06,oVers);*/

			// 3.x
			/*iVersion = oVers.getVersionAsInt();
			iRelease = oVers..getRelease();*/
			
			ABL::AppConfigModelC configModel;
			configModel.SetApplicationNumber(AT_DCP06);

			//DateTime iDate = configModel.GetAppVersionDate();

			//int lYear, lMonth, lDay, lHour, lMin, lSec;
			//
			//GMAT::SetMjd(iDate, lYear, lMonth, lDay, lHour, lMin, lSec);

			//sprintf(sProgramVersion," %d.%02d ", iVersion,iRelease);
			//sprintf(sProgramDate," %02d.%02d.%04d", lDay, lMonth, lYear);

			#ifdef CS35
			FILE_STORAGE1 = CPI::deviceLocalMemory;
			#else
			FILE_STORAGE1 = CPI::deviceSdCard;
			#endif
			SerialNumber = L" ";

			// Database (DCP9: 321, BestFit, Cylinder, ChangeStation)
#if DCP_USE_JSON_DATABASE
			m_pDatabase = DCP_UNIQUE_PTR<Database::JsonDatabase>(new Database::JsonDatabase());
#else
			m_pDatabase = DCP_UNIQUE_PTR<Database::IDatabase>(new Database::StubDatabase());
#endif
			char dbPath[CPI::LEN_PATH_MAX];
			if (CPI::SensorC::GetInstance()->GetPath(FILE_STORAGE1, CPI::ftUserAscii, dbPath))
			{
				std::string dir(dbPath);
				dir += "/DCP06/jobs";
				m_pDatabase->setDataDirectory(dir);
			}
			//getSpecialInfo();
			//oVers.
}
DCP::DCP06ModelC::~DCP06ModelC()
{
}

DCP::Database::IDatabase* DCP::DCP06ModelC::GetDatabase() const
{
	return m_pDatabase.get();
}

void DCP::DCP06ModelC::SetDatabaseDataDirectory(const char* path)
{
	if (m_pDatabase.get())
		m_pDatabase->setDataDirectory(path ? path : "");
}

void DCP::DCP06ModelC::initialize_matrix4x4(double pMatrix[4][4])
{
	for(int i=0; i < 4; i++)
	{
		for(int j=0; j < 4; j++)
		{
			pMatrix[i][j]=0.0;
			pMatrix[i][j]=0.0;
			pMatrix[i][j]=0.0;
			pMatrix[i][j]=0.0;
		}
		pMatrix[i][i]=1.0;
		pMatrix[i][i]=1.0;
		pMatrix[i][i]=1.0;
		pMatrix[i][i]=1.0;
   }
}


DCP::DCP06ConfigControllerC::DCP06ConfigControllerC(GUI::ControllerC* pParent, DCP06ModelC* poModel)
    : ABL::AppConfigControllerC(pParent)
{
    // Create data model if necessary
    if (NULL == poModel)
    {
        //lint -save -e1732 -e1733 new in constructor which has no assignment operator / copy constructor
        poModel = new DCP06ModelC();
        //lint -restore
    }

    // Set application info
    poModel->SetConfigKey(CNF_KEY_INIT);
    poModel->SetAppName(StringC(AT_DCP06,C_DCP_APPLICATION_NAME_TOK));
    poModel->SetApplicationNumber(AT_DCP06);
	poModel->SetConfigVersionNumber(1);

    // Set data model
    //lint -save -e1506 Call to virtual function within contructor
    USER_APP_VERIFY(SetModel(poModel));
    //lint -restore

    // Load configuration data to model if necessary
	//poModel->SetConfigKey(CNF_KEY_CONFIG);
	if (!poModel->IsConfigLoaded())
    //if (!GetModel()->IsConfigLoaded())
    {
        //lint -save -e1506 Call to virtual function within constructor
        (void)LoadConfigData(); // no further overriding function implemented
    
		poModel->SetConfigKey(CNF_KEY_A321);
		 (void)LoadConfigData();

 		poModel->SetConfigKey(CNF_KEY_BESTFIT);
		 (void)LoadConfigData();

		 poModel->SetConfigKey(CNF_KEY_CHST);
		 (void)LoadConfigData();

		 poModel->SetConfigKey(CNF_KEY_CIRCLE);
		 (void)LoadConfigData();

 		 poModel->SetConfigKey(CNF_KEY_USERDEF);
		 (void)LoadConfigData();

		  poModel->SetConfigKey(CNF_KEY_LICENSE);
		 (void)LoadConfigData();

		 poModel->SetConfigKey(CNF_KEY_SHAFT);
		 (void)LoadConfigData();
		 
		 poModel->SetConfigKey(CNF_KEY_LINE_FITTING);
	    (void)LoadConfigData();

		 poModel->SetConfigKey(CNF_KEY_DEMO_LICENSES);
	    (void)LoadConfigData();
	
		 //lint -restore
    }
	/*
	poModel->SetConfigKey(CNF_KEY_CONFIG2);
    //if (!GetModel()->IsConfigLoaded())
	if (poModel->IsConfigLoaded())
    {
        //lint -save -e1506 Call to virtual function within constructor
        (void)LoadConfigData(); // no further overriding function implemented
        //lint -restore
    }
	*/
    //GetTabbedDialog()->SetTxtApplicationId(GetTxtApplicationId());

    // Create pages
    //lint -save -e1732 -e1733 new in constructor which has no assignment operator / copy constructor
    //GUI::StandardDialogC* poGeneralPageDlg = new HEWGeneralPageDialogC( GetTxtApplicationId(), poModel ); 
    //ABL::AppConfigLogPageC* poConfigLogPage = new ABL::AppConfigLogPageC( poModel );
    //poConfigLogPage->SetDefaultLogFile(L"Adjust.txt");
    //poConfigLogPage->SetDefaultFormatFile(L"Adjust.frt");
    //lint -restore

    // Set help token
    //poGeneralPageDlg->SetHelpTok(H_HEW_CONFIG_GENERAL_TOK,0);

    // Add pages
    //USER_APP_VERIFY( GetTabbedDialog()->AddPage(poGeneralPageDlg, T_HEW_TAB_GENERAL_TOK) );
    //USER_APP_VERIFY( GetTabbedDialog()->AddPage(new ABL::AppConfigLogPageC(poModel), T_HEW_TAB_LOG_TOK) );
    //USER_APP_VERIFY( GetTabbedDialog()->AddPage(poConfigLogPage, T_HEW_TAB_LOG_TOK) );
//lint -save -e429 Custodial pointer has not been freed or returned
}

// --------------------------------------------------------------------------------------------------------------------
//
// Destructor
//
DCP::DCP06ConfigControllerC::~DCP06ConfigControllerC(void)
{
}

// --------------------------------------------------------------------------------------------------------------------
//
// GetModel
// 
DCP::DCP06ModelC* DCP::DCP06ConfigControllerC::GetModel(void) const
{
    // Return the configuration model
    USER_APP_ASSERT(ModelHandlerC::GetModel() != NULL)
    return dynamic_cast<DCP06ModelC*>(ModelHandlerC::GetModel());
}

/*
bool DCP::DCP06ConfigControllerC::UpdateModelMigrated(
                unsigned int ulKey, CPI::CFG::ArchiveC* poArchive, unsigned int ulCnfVersion,
				unsigned int ulAppVersion)
{
	return true;
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// UpdateModel
//
void DCP::DCP06ConfigControllerC::UpdateModel(unsigned int ulKey, CPI::CFG::ArchiveC* poArchive)
{
	
    DCP06ModelC* poModel = GetModel();
    if (NULL == poModel)
    {
        return;
    }

   if(NULL == poArchive)
   {	
		//DCP06MsgBoxC msgbox;
		//StringC sMsg;
		//sMsg.Format(L"Cannot read data from archive (Key=%d )!",ulKey);
		//msgbox.ShowMessageOk(sMsg);
	   return;
   }
	// set common model data
       switch(ulKey)
    {
		case CNF_KEY_INIT:
			if(NULL != poArchive)
			{	
				load_adf_file_name(poArchive, poModel);
				
				//DCP06MsgBoxC msgbox;
				//StringC sMsg;
				//msgbox.ShowMessageOk(poModel->ADFFileName);
				load_init_data(poArchive, poModel);
				load_matrix_data(poArchive, poModel);
				load_hiddenpointbar_conf(poArchive,poModel);
				load_home_points(poArchive, poModel);
				load_tool_data(poArchive, poModel);
			}	
			break;

	    case CNF_KEY_A321:
			if(NULL != poArchive)
			{	
				load_dom_data(poArchive, poModel);
			}
			break;
	
		case CNF_KEY_USERDEF:
			if(NULL != poArchive)
			{	
				load_userdef_data(poArchive, poModel);
			}
			break;

		case CNF_KEY_BESTFIT:
			if(NULL != poArchive)
			{	
				load_pom_data(poArchive, poModel);
				*poArchive >> poModel->pom_into_capture;
				*poArchive >> poModel->pom_into_template;
			}
			break;

	case CNF_KEY_CHST:
     if(NULL != poArchive)
		{	
			load_chst_data(poArchive, poModel);
			//save_home_points(poArchive, poModel);
	 }	
		break;

	case CNF_KEY_CIRCLE:
     if(NULL != poArchive)
		{	
			load_circle_data(poArchive, poModel);
			load_crl_file_name(poArchive, poModel);
			//save_home_points(poArchive, poModel);
	 }	
		break;


	 case CNF_KEY_LICENSE:
     if(NULL != poArchive)
	 {	
			load_lisence(poArchive, poModel);
	 }	
		break;
 
	case CNF_KEY_SHAFT:
		if(NULL != poArchive)
		{	
			load_shaft_file_name(poArchive, poModel);
			load_shaft_data(poArchive, poModel);
			//save_home_points(poArchive, poModel);
		}	
		break;

	case CNF_KEY_LINE_FITTING:
		if(NULL != poArchive)
		{
			load_linefitting_data(poArchive, poModel);
		}
		break;

	case CNF_KEY_DEMO_LICENSES:
		if(NULL != poArchive)
		{
			load_demo_licenses(poArchive, poModel);
		}
		break;
    default:
        break;
    }
}  

// --------------------------------------------------------------------------------------------------------------------
//
// ReadModel
//
void DCP::DCP06ConfigControllerC::ReadModel(unsigned int ulKey, CPI::CFG::ArchiveC* poArchive)
{
    DCP06ModelC* poModel = GetModel();
    if (NULL == poModel)
    {
        return;
    }

	 if(NULL == poArchive)
	 {
    		DCP06MsgBoxC msgbox;
			msgbox.ShowMessageOk(L"Cannot save data into archive! ");
			return;
	 }
    // read common model data
    switch(ulKey)
    {
		case CNF_KEY_INIT:
			if(NULL != poArchive)
			{	
				save_adf_file_name(poArchive, poModel);
				save_init_data(poArchive, poModel);
				save_matrix_data(poArchive, poModel);
				save_hiddenpointbar_conf(poArchive,poModel);
				save_home_points(poArchive, poModel);
				save_tool_data(poArchive, poModel);
			}	
			break;

	    case CNF_KEY_A321:
			if(NULL != poArchive)
			{	
				save_dom_data(poArchive, poModel);
			}
			break;
	
		case CNF_KEY_BESTFIT:
			if(NULL != poArchive)
			{	
				save_pom_data(poArchive, poModel);
				*poArchive << poModel->pom_into_capture;
				*poArchive << poModel->pom_into_template;

			}
			break;

	case CNF_KEY_CHST:
     if(NULL != poArchive)
		{	
			save_chst_data(poArchive, poModel);
			//save_home_points(poArchive, poModel);
	 }	
		break;

	case CNF_KEY_CIRCLE:
     if(NULL != poArchive)
		{	
			save_circle_data(poArchive, poModel);
			save_crl_file_name(poArchive, poModel);
			//save_home_points(poArchive, poModel);
	 }	
		break;

	case CNF_KEY_USERDEF:
     if(NULL != poArchive)
	 {	
			save_userdef_data(poArchive, poModel);
	 }	
		break;

	case CNF_KEY_LICENSE:
     if(NULL != poArchive)
	 {	
			save_lisence(poArchive, poModel);
			//load_lisence(poArchive, poModel);
	 }	
		break;
	case CNF_KEY_SHAFT:
     if(NULL != poArchive)
	 {	
			save_shaft_file_name(poArchive, poModel);
			save_shaft_data(poArchive, poModel);
	 }	
		break;
	
	case CNF_KEY_LINE_FITTING:
		if(NULL != poArchive)
		{
			save_linefitting_data(poArchive, poModel);
		}
		break;

	case CNF_KEY_DEMO_LICENSES:
		if(NULL != poArchive)
		{
			save_demo_licenses(poArchive, poModel);
		}
		break;
	default:
        break;
    }
}

void DCP::DCP06ConfigControllerC::save_matrix4x4(CPI::CFG::ArchiveC* poArchive, double matrix[4][4])
{
	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			*poArchive << matrix[i][j];


}

void DCP::DCP06ConfigControllerC::load_matrix4x4(CPI::CFG::ArchiveC* poArchive, double matrix[4][4])
{
	for(int i=0; i < 4; i++)
		for(int j=0; j < 4; j++)
			*poArchive >> matrix[i][j];
}

void DCP::DCP06ConfigControllerC::save_hiddenpointbar_conf(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* pModel)
{
	for(int i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR ; i++)
		*poArchive << pModel->hidden_point_bar[i];
}

void DCP::DCP06ConfigControllerC::load_hiddenpointbar_conf(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* pModel)
{
	for(int i=0; i < MAX_POINTS_IN_HIDDENPOINT_BAR ; i++)
		*poArchive >> pModel->hidden_point_bar[i];
}

void DCP::DCP06ConfigControllerC::load_adf_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	char szUser[20+1];

	memset(&szUser[0],0,sizeof(char) * (20+1));
	*poArchive >> szUser;	
	poModel->ADFFileName = StringC(szUser);
}

void DCP::DCP06ConfigControllerC::load_crl_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	char szUser[20+1];

	memset(&szUser[0],0,sizeof(char) * (20+1));
	*poArchive >> szUser;	
	poModel->sCircleFile = StringC(szUser);
}

void DCP::DCP06ConfigControllerC::load_shaft_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	char szUser[20+1];

	memset(&szUser[0],0,sizeof(char) * (20+1));
	*poArchive >> szUser;	
	poModel->sShaftFile = StringC(szUser);
}
void DCP::DCP06ConfigControllerC::load_lisence(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	//char temp[21];
	*poArchive >> poModel->sKeyCode;	
	*poArchive >> poModel->iStartCount;
}

void DCP::DCP06ConfigControllerC::load_init_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	char szUser[DCP_USER_LENGTH+1];

		memset(&szUser[0],0,sizeof(char) * (DCP_USER_LENGTH+1));
			*poArchive >> szUser;	
			poModel->m_sUser = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser1 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser2 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser3 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser4 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser5 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser6 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser7 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser8 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser9 = StringC(szUser);
			*poArchive >> szUser;	
			poModel->m_sUser10 = StringC(szUser);


			*poArchive >> poModel->m_nOverWriteInfo;
			*poArchive >> poModel->m_n2FaceMeas; 
			*poArchive >> poModel->m_nDesignValues;
			*poArchive >> poModel->m_nAutoIncrement;
			*poArchive >> poModel->m_nToolInfo;
			*poArchive >> poModel->m_nAverageCount;
			*poArchive >> poModel->m_nLeftRightHand;
			*poArchive >> poModel->m_nAutoMatch;
			*poArchive >> poModel->m_nUnit;
			*poArchive >> poModel->m_nDecimals;
			
			*poArchive >> poModel->active_coodinate_system;
			*poArchive >> poModel->m_nAmsLog;
			*poArchive >> poModel->m_nSaveTool;
			*poArchive >> poModel->FILE_STORAGE1;

	
}

void DCP::DCP06ConfigControllerC::load_dom_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
			*poArchive >> poModel->dom_active_plane; 
			*poArchive >>  poModel->dom_active_line;
			*poArchive >>  poModel->dom_hz_plane;
			load_plane(poArchive,&poModel->dom_plane_buff[0]);
			load_plane(poArchive,&poModel->dom_hz_plane_buff[0]);
			load_line(poArchive,&poModel->dom_line_buff[0]);
			load_point(poArchive,&poModel->dom_ovalues_buff);
			load_point(poArchive,&poModel->dom_ovalues_tool_buff);
			load_point(poArchive,&poModel->dom_ref_point_buff);
			load_point(poArchive,&poModel->dom_rot_plane_buff);
			load_point(poArchive,&poModel->dom_rot_line_buff);

			*poArchive >> poModel->ocsd_defined;

}

void DCP::DCP06ConfigControllerC::load_userdef_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	int i = 0;

	// TODO loputkin
			*poArchive >> poModel->userdef_active_plane; 
			*poArchive >>  poModel->userdef_active_line;
			*poArchive >>  poModel->userdef_hz_plane;
			load_plane(poArchive,&poModel->userdef_plane_buff[0]);
			load_plane(poArchive,&poModel->userdef_hz_plane_buff[0]);
			load_line(poArchive,&poModel->userdef_line_buff[0]);
			load_point(poArchive,&poModel->userdef_ovalues_buff);
			load_point(poArchive,&poModel->userdef_ovalues_tool_buff);
			load_point(poArchive,&poModel->userdef_ref_point_buff);
			load_point(poArchive,&poModel->userdef_rot_plane_buff);
			load_point(poArchive,&poModel->userdef_rot_line_buff);

			*poArchive >> poModel->ocsu_defined;

			// ADDED 14.07.2011
			// measured points
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				load_point(poArchive,&poModel->userdef_measured_points[i]);

			// plane points number
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				*poArchive >> poModel->userdef_plane_points_no[i];

			// line points number
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				*poArchive >> poModel->userdef_line_points_no[i];

			*poArchive >> poModel->userdef_point_no;

}

void DCP::DCP06ConfigControllerC::load_tool_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{			
		*poArchive >> poModel->active_tool;
		*poArchive >> poModel->active_tool_x;
		*poArchive >> poModel->active_tool_y;
		*poArchive >> poModel->active_tool_z;
		*poArchive >> poModel->tool_trans_x;
		*poArchive >> poModel->tool_trans_y;
		*poArchive >> poModel->tool_trans_z;
		
		for(int i=0; i < MAX_TOOLS; i++)
		{
			*poArchive >> poModel->tool_table[i].tool_id;
			*poArchive >> poModel->tool_table[i].x;
			*poArchive >> poModel->tool_table[i].y;
			*poArchive >> poModel->tool_table[i].z;
			*poArchive >> poModel->tool_table[i].prism_or_tape;
			*poArchive >> poModel->tool_table[i].constant;
		}


}
void DCP::DCP06ConfigControllerC::load_circle_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{		
		*poArchive >> poModel->circle_plane_type;

		 // circle planes
		load_plane(poArchive,&poModel->circle_planes[0]);
	
		load_circle_buff(poArchive,&poModel->circle_points[0]);
		load_circle_buff(poArchive,&poModel->circle_points_in_plane[0]);

		*poArchive >> poModel->circle_cx;
		*poArchive >> poModel->circle_cy;
		*poArchive >> poModel->circle_cz;
		*poArchive >> poModel->circle_diameter;
		*poArchive >> poModel->circle_vi;
		*poArchive >> poModel->circle_vj;
		*poArchive >> poModel->circle_vk;
}
void DCP::DCP06ConfigControllerC::load_shaft_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{			
		short i = 0;

		*poArchive >> poModel->shaft_circle_plane_type;

		load_circle_buff(poArchive,&poModel->shaft_circle_points[0]);
		load_circle_buff(poArchive,&poModel->shaft_circle_points_in_plane[0]);
		
		load_line(poArchive, &poModel->shaft_line[0]);
	
		*poArchive >> poModel->shaft_circle_cx;
		*poArchive >> poModel->shaft_circle_cy;
		*poArchive >> poModel->shaft_circle_cz;
		*poArchive >> poModel->shaft_circle_diameter;
		*poArchive >> poModel->shaft_circle_rms;
		*poArchive >> poModel->shaft_ref_line;
		*poArchive >> poModel->shaft_circle_vi;
		*poArchive >> poModel->shaft_circle_vj;
		*poArchive >> poModel->shaft_circle_vk;

}

void DCP::DCP06ConfigControllerC::save_circle_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
		*poArchive << poModel->circle_plane_type;

		 // circle planes
		save_plane(poArchive,&poModel->circle_planes[0]);
		
		save_circle_buff(poArchive,&poModel->circle_points[0]);
		save_circle_buff(poArchive,&poModel->circle_points_in_plane[0]);

		*poArchive << poModel->circle_cx;
		*poArchive << poModel->circle_cy;
		*poArchive << poModel->circle_cz;
		*poArchive << poModel->circle_diameter;
		*poArchive << poModel->circle_vi;
		*poArchive << poModel->circle_vj;
		*poArchive << poModel->circle_vk;
}
void DCP::DCP06ConfigControllerC::save_shaft_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
		*poArchive << poModel->shaft_circle_plane_type;

		save_circle_buff(poArchive,&poModel->shaft_circle_points[0]);
		save_circle_buff(poArchive,&poModel->shaft_circle_points_in_plane[0]);

		save_line(poArchive, &poModel->shaft_line[0]);

		*poArchive << poModel->shaft_circle_cx;
		*poArchive << poModel->shaft_circle_cy;
		*poArchive << poModel->shaft_circle_cz;
		*poArchive << poModel->shaft_circle_diameter;
		*poArchive << poModel->shaft_circle_rms;
		*poArchive << poModel->shaft_ref_line;
		*poArchive << poModel->shaft_circle_vi;
		*poArchive << poModel->shaft_circle_vj;
		*poArchive << poModel->shaft_circle_vk;
}
void DCP::DCP06ConfigControllerC::save_lisence(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
		*poArchive << poModel->sKeyCode;
		*poArchive << poModel->iStartCount;
}
void DCP::DCP06ConfigControllerC::load_pom_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		load_point(poArchive,&poModel->POM_point_DCS[i]);	
		load_point(poArchive,&poModel->POM_point_OCS[i]);	
		load_point(poArchive,&poModel->POM_point_RES[i]);	
	}

	*poArchive >> poModel->ocsp_defined;

}
void DCP::DCP06ConfigControllerC::load_chst_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		load_point(poArchive,&poModel->CHST_point_DCS[i]);	
		load_point(poArchive,&poModel->CHST_point_OCS[i]);	
		load_point(poArchive,&poModel->CHST_point_RES[i]);	
	}

	*poArchive >> poModel->ocsc_defined;
	*poArchive >> poModel->chst_last_sel;
	*poArchive >> poModel->stationNumber;
	*poArchive >> poModel->chst_used_hz_plane;
}

void DCP::DCP06ConfigControllerC::load_matrix_data(CPI::CFG::ArchiveC*poArchive, DCP06ModelC* pModel)
{
	load_matrix4x4(poArchive, pModel->dcs_matrix);
	load_matrix4x4(poArchive, pModel->ocsd_matrix);
	load_matrix4x4(poArchive, pModel->ocsp_matrix);
	load_matrix4x4(poArchive, pModel->ocsc_matrix);
	load_matrix4x4(poArchive, pModel->ocsu_matrix);

	matinv4x4(pModel->dcs_matrix, &pModel->dcs_inv_matrix);
	matinv4x4(pModel->ocsd_matrix, &pModel->ocsd_inv_matrix);
	matinv4x4(pModel->ocsp_matrix, &pModel->ocsp_inv_matrix);
	matinv4x4(pModel->ocsc_matrix, &pModel->ocsc_inv_matrix);
	matinv4x4(pModel->ocsu_matrix, &pModel->ocsu_inv_matrix);
}

void DCP::DCP06ConfigControllerC::load_home_points(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_HOME_POINTS; i++)
	{
		load_point(poArchive,&poModel->home_points[i]);	
	}
}

void DCP::DCP06ConfigControllerC::load_linefitting_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* pModel)
{
	load_line(poArchive,&pModel->linefitting_line[0]);

	int i = 0;
	for(i=0; i < MAX_LINEFIT_POINTS; i++)
		load_point(poArchive,&pModel->linefitting_points[i]);
	
	for(i=0; i < MAX_LINEFIT_POINTS; i++)
	{
		*poArchive >> pModel->linefitting_results[i].height_diff;
		*poArchive >> pModel->linefitting_results[i].line_offset;
		*poArchive >> pModel->linefitting_results[i].points_distance;
		*poArchive >> pModel->linefitting_results[i].distance_along_line;
		*poArchive >> pModel->linefitting_results[i].sta;
	}
	// setup data
	*poArchive >>  pModel->linefitting_manualHeight;
	*poArchive >>  pModel->linefitting_shiftValue;
	*poArchive >>  pModel->linefitting_rotateAngle;
	*poArchive >>  pModel->linefitting_selectedHeight;
	*poArchive >>  pModel->linefitting_selectedShift;
	*poArchive >>  pModel->linefitting_selectedRotate;
	*poArchive >> pModel->linefitting_selectedRefLine;
}

void DCP::DCP06ConfigControllerC::save_linefitting_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	int i = 0;

	/*DCP06MsgBoxC msgbox;
	msgbox.ShowMessageOk(L"Save_linefitting! ");*/


	save_line(poArchive,&poModel->linefitting_line[0]);

	for(i=0; i < MAX_LINEFIT_POINTS; i++)
		save_point(poArchive,&poModel->linefitting_points[i]);;

	for(i=0; i < MAX_LINEFIT_POINTS; i++)
	{
		*poArchive << poModel->linefitting_results[i].height_diff;
		*poArchive << poModel->linefitting_results[i].line_offset;
		*poArchive << poModel->linefitting_results[i].points_distance;
		*poArchive << poModel->linefitting_results[i].distance_along_line;
		*poArchive << poModel->linefitting_results[i].sta;
	}
	// setup data
	*poArchive <<  poModel->linefitting_manualHeight;
	*poArchive <<  poModel->linefitting_shiftValue;
	*poArchive <<  poModel->linefitting_rotateAngle;
	*poArchive <<  poModel->linefitting_selectedHeight;
	*poArchive <<  poModel->linefitting_selectedShift;
	*poArchive <<  poModel->linefitting_selectedRotate;
	*poArchive << poModel->linefitting_selectedRefLine;
}

void DCP::DCP06ConfigControllerC::save_adf_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
			char szTemp[20+1];
			//UTL::UnicodeToAscii(szTemp, poModel->ADFFileName);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->ADFFileName, szTemp );

			//sprintf(&szTemp[0],"%-*.*s", 10,10, DCP_USER_LENGTH,DCP_USER_LENGTH, szTemp);	
			*poArchive << szTemp;
}

void DCP::DCP06ConfigControllerC::save_crl_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
			char szTemp[20+1];
			//UTL::UnicodeToAscii(szTemp, poModel->sCircleFile);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->sCircleFile, szTemp );

			//sprintf(&szTemp[0],"%-*.*s", 10,10, DCP_USER_LENGTH,DCP_USER_LENGTH, szTemp);	
			*poArchive << szTemp;
}
void DCP::DCP06ConfigControllerC::save_shaft_file_name(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
			char szTemp[20+1];
			//UTL::UnicodeToAscii(szTemp, poModel->sShaftFile);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->sShaftFile, szTemp );
			//sprintf(&szTemp[0],"%-*.*s", 10,10, DCP_USER_LENGTH,DCP_USER_LENGTH, szTemp);	
			*poArchive << szTemp;
}
void DCP::DCP06ConfigControllerC::save_init_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{

			char szTemp[DCP_USER_LENGTH+1];
			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser, szTemp );

			//sprintf(&szTemp[0],"%-*.*s", 10,10, DCP_USER_LENGTH,DCP_USER_LENGTH, szTemp);	
			*poArchive << szTemp;
			
			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser1);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser1, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"1");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser1);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser2);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser2, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"2");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser2);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser3);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser3, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"3");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser3);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser4);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser4, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"4");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser4);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser5);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser5, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"5");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser5);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser6);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser6, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"6");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser6);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser7);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser7, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"7");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser7);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser8);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser8, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"8");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser8);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser9);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser9, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"9");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser9);	
			*poArchive << szTemp;

			//UTL::UnicodeToAscii(szTemp, poModel->m_sUser10);
			BSS::UTI::BSS_UTI_WCharToAscii( poModel->m_sUser10, szTemp );
			//sprintf(szTemp,"%-*.*s", 10,10,"10");//DCP_USER_LENGTH,DCP_USER_LENGTH,poModel->m_sUser10);	
			*poArchive << szTemp;

			*poArchive << poModel->m_nOverWriteInfo;
			*poArchive << poModel->m_n2FaceMeas; 
			*poArchive << poModel->m_nDesignValues;
			*poArchive << poModel->m_nAutoIncrement;
			*poArchive << poModel->m_nToolInfo;
			*poArchive << poModel->m_nAverageCount;
			*poArchive << poModel->m_nLeftRightHand;
			*poArchive << poModel->m_nAutoMatch;
			*poArchive << poModel->m_nUnit;
			*poArchive << poModel->m_nDecimals;

			*poArchive << poModel->active_coodinate_system;
			*poArchive << poModel->m_nAmsLog;
			*poArchive << poModel->m_nSaveTool;
			*poArchive << poModel->FILE_STORAGE1;

}

void DCP::DCP06ConfigControllerC::save_dom_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{		
			*poArchive << poModel->dom_active_plane; 
			*poArchive <<  poModel->dom_active_line;
			*poArchive <<  poModel->dom_hz_plane;
			save_plane(poArchive,&poModel->dom_plane_buff[0]);
			save_plane(poArchive,&poModel->dom_hz_plane_buff[0]);
			save_line(poArchive,&poModel->dom_line_buff[0]);
			save_point(poArchive,&poModel->dom_ovalues_buff);
			save_point(poArchive,&poModel->dom_ovalues_tool_buff);
			save_point(poArchive,&poModel->dom_ref_point_buff);
			save_point(poArchive,&poModel->dom_rot_plane_buff);
			save_point(poArchive,&poModel->dom_rot_line_buff);

			*poArchive << poModel->ocsd_defined;

			/*
			DCP_PLANE_TYPE dom_active_plane;
			DCP_LINE_TYPE  dom_active_line; 
			bool dom_hz_plane;
			S_PLANE_BUFF	dom_plane_buff[1];
			S_PLANE_BUFF	dom_hz_plane_buff[1];
			S_LINE_BUFF		dom_line_buff[1];
			S_POINT_BUFF	dom_ovalues_buff;
			S_POINT_BUFF	dom_ovalues_tool_buff;
			S_POINT_BUFF	dom_ref_point_buff;
			S_POINT_BUFF	dom_rot_plane_buff;
			S_POINT_BUFF	dom_rot_line_buff;
			*/
}

void DCP::DCP06ConfigControllerC::save_userdef_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{		
	int i = 0;

			*poArchive << poModel->userdef_active_plane; 
			*poArchive <<  poModel->userdef_active_line;
			*poArchive <<  poModel->userdef_hz_plane;
			save_plane(poArchive,&poModel->userdef_plane_buff[0]);
			save_plane(poArchive,&poModel->userdef_hz_plane_buff[0]);
			save_line(poArchive,&poModel->userdef_line_buff[0]);
			save_point(poArchive,&poModel->userdef_ovalues_buff);
			save_point(poArchive,&poModel->userdef_ovalues_tool_buff);
			save_point(poArchive,&poModel->userdef_ref_point_buff);
			save_point(poArchive,&poModel->userdef_rot_plane_buff);
			save_point(poArchive,&poModel->userdef_rot_line_buff);

			*poArchive << poModel->ocsu_defined;
			
			// ADDED 14.07.2011
			// measured points
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				save_point(poArchive,&poModel->userdef_measured_points[i]);

			// plane points number
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				*poArchive << poModel->userdef_plane_points_no[i];

			// line points number
			for(i=0; i < MAX_USERDEF_POINTS; i++)
				*poArchive << poModel->userdef_line_points_no[i];

			*poArchive << poModel->userdef_point_no;
}

void DCP::DCP06ConfigControllerC::save_tool_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
		*poArchive << poModel->active_tool;
		*poArchive << poModel->active_tool_x;
		*poArchive << poModel->active_tool_y;
		*poArchive << poModel->active_tool_z;
		*poArchive << poModel->tool_trans_x;
		*poArchive << poModel->tool_trans_y;
		*poArchive << poModel->tool_trans_z;
		
		for(int i=0; i < MAX_TOOLS; i++)
		{
			*poArchive << poModel->tool_table[i].tool_id;
			*poArchive << poModel->tool_table[i].x;
			*poArchive << poModel->tool_table[i].y;
			*poArchive << poModel->tool_table[i].z;
			*poArchive << poModel->tool_table[i].prism_or_tape;
			*poArchive << poModel->tool_table[i].constant;
		}
}

void DCP::DCP06ConfigControllerC::save_pom_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		save_point(poArchive,&poModel->POM_point_DCS[i]);	
		save_point(poArchive,&poModel->POM_point_OCS[i]);	
		save_point(poArchive,&poModel->POM_point_RES[i]);	
	}

	*poArchive << poModel->ocsp_defined;
}

void DCP::DCP06ConfigControllerC::save_home_points(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_HOME_POINTS; i++)
	{
		save_point(poArchive,&poModel->home_points[i]);	
	}
}
void DCP::DCP06ConfigControllerC::save_chst_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	for(int i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		save_point(poArchive,&poModel->CHST_point_DCS[i]);	
		save_point(poArchive,&poModel->CHST_point_OCS[i]);	
		save_point(poArchive,&poModel->CHST_point_RES[i]);	
	}
	*poArchive << poModel->ocsc_defined;
	*poArchive << poModel->chst_last_sel;
	*poArchive << poModel->stationNumber;
	*poArchive << poModel->chst_used_hz_plane;
}

void DCP::DCP06ConfigControllerC::save_matrix_data(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* pModel)
{
	save_matrix4x4(poArchive, pModel->dcs_matrix);
	save_matrix4x4(poArchive, pModel->ocsd_matrix);
	save_matrix4x4(poArchive, pModel->ocsp_matrix);
	save_matrix4x4(poArchive, pModel->ocsc_matrix);
	save_matrix4x4(poArchive, pModel->ocsu_matrix);
}


void DCP::DCP06ConfigControllerC::save_plane(CPI::CFG::ArchiveC* poArchive, S_PLANE_BUFF* pPlane)
{	
		*poArchive <<	pPlane->id;
		for(int i=0; i < MAX_POINTS_IN_PLANE; i++)
			save_point(poArchive,&pPlane->points[i]);

		*poArchive <<	pPlane->px;
		*poArchive <<	pPlane->py;
		*poArchive <<	pPlane->pz;
		*poArchive <<	pPlane->nx;
		*poArchive <<	pPlane->ny;
		*poArchive <<	pPlane->nz;
		*poArchive <<	pPlane->calc;
		*poArchive <<	pPlane->sta;
		*poArchive <<	pPlane->des_px;
		*poArchive <<	pPlane->des_py;
		*poArchive <<	pPlane->des_pz;
		*poArchive <<	pPlane->des_nx;
		*poArchive <<	pPlane->des_ny;
		*poArchive <<	pPlane->des_nz;
		*poArchive <<	pPlane->des_calc;
		*poArchive <<	pPlane->des_sta;
}

void DCP::DCP06ConfigControllerC::save_line(CPI::CFG::ArchiveC* poArchive, S_LINE_BUFF* pLine)
{
	//DCP06MsgBoxC msgbox;
	//msgbox.ShowMessageOk(L"Save_line! ");

	*poArchive <<	pLine->id;
	for(int i=0; i < MAX_POINTS_IN_LINE; i++)
	{
		save_point(poArchive,&pLine->points[i]);
	}
	*poArchive <<	pLine->px;
	*poArchive <<	pLine->py;
	*poArchive <<	pLine->pz;
	*poArchive <<	pLine->ux;
	*poArchive <<	pLine->uy;
	*poArchive <<	pLine->uz;
	*poArchive <<	pLine->calc;
	*poArchive <<	pLine->sta;
	*poArchive <<	pLine->des_px;
	*poArchive <<	pLine->des_py;
    *poArchive <<	pLine->des_pz;
	*poArchive <<	pLine->des_ux;
	*poArchive <<	pLine->des_uy;
	*poArchive <<	pLine->des_uz;
	*poArchive <<	pLine->des_calc;
	*poArchive <<	pLine->des_sta;
}

void DCP::DCP06ConfigControllerC::save_point(CPI::CFG::ArchiveC* poArchive, S_POINT_BUFF* pPoint)
{	
	//short temp_short;
	char temp[POINT_ID_BUFF_LEN + 1];
	
	sprintf(temp,"%-*.*s",POINT_ID_BUFF_LEN-1,POINT_ID_BUFF_LEN-1, pPoint->point_id);
	*poArchive <<	temp;
	
	//*poArchive <<	pPoint->point_id;
	*poArchive <<	 pPoint->x;
	*poArchive <<	 pPoint->y;
	*poArchive <<	 pPoint->z;
	*poArchive <<	 pPoint->xdes;
	*poArchive <<	 pPoint->ydes;
	*poArchive <<	 pPoint->zdes;
	*poArchive <<	 pPoint->sta;
	*poArchive <<	 pPoint->dsta;
	*poArchive <<	 pPoint->diameter;
	*poArchive <<	 pPoint->cds;
	*poArchive <<	 pPoint->xsta;
	*poArchive <<	 pPoint->ysta;
	*poArchive <<	 pPoint->zsta;


	/*
	typedef struct
	{
		char point_id[7];
		double x;
		double y;
		double z;
		double xdes;
		double ydes;
		double zdes;
		DCP_POINT_STATUS sta; // 0=not defined, 1=design, 2=measured 3=Design rejected 4=measured/rejected
		DCP_POINT_STATUS dsta;
		double diameter;
		DCP_COORDINATE_SYSTEM cds; // coordinate system 0=scs, 1=ocs
		
		// for x or y or z
		DCP_POINT_STATUS xsta;
		DCP_POINT_STATUS ysta;
		DCP_POINT_STATUS zsta;
	} S_POINT_BUFF;
	*/
}


void DCP::DCP06ConfigControllerC::load_point(CPI::CFG::ArchiveC* poArchive, S_POINT_BUFF* pPoint)
{	
//short temp_short;
	char temp[POINT_ID_BUFF_LEN + 1];
	*poArchive >>	temp;
	
	sprintf(pPoint->point_id,"%*.*s", POINT_ID_BUFF_LEN-1,POINT_ID_BUFF_LEN-1,temp);
	//*poArchive >>	pPoint->point_id;
	*poArchive >>	pPoint->x;
	*poArchive >>	pPoint->y;
	*poArchive >>	pPoint->z;
	*poArchive >>	pPoint->xdes;
	*poArchive >>	pPoint->ydes;
	*poArchive >>	pPoint->zdes;
	*poArchive >>	pPoint->sta;
	*poArchive >>	pPoint->dsta;
	*poArchive >>	pPoint->diameter;
	*poArchive >>	pPoint->cds;
	*poArchive >>	pPoint->xsta;
	*poArchive >>	pPoint->ysta;
	*poArchive >>	pPoint->zsta;
}

void DCP::DCP06ConfigControllerC::load_plane(CPI::CFG::ArchiveC* poArchive, S_PLANE_BUFF* pPlane)
{
		*poArchive >>	pPlane->id;
		for(int i=0; i < MAX_POINTS_IN_PLANE; i++)
			load_point(poArchive,&pPlane->points[i]);

		*poArchive >>	pPlane->px;
		*poArchive >>	pPlane->py;
		*poArchive >>	pPlane->pz;
		*poArchive >>	pPlane->nx;
		*poArchive >>	pPlane->ny;
		*poArchive >>	pPlane->nz;
		*poArchive >>	pPlane->calc;
		*poArchive >>	pPlane->sta;
		*poArchive >>	pPlane->des_px;
		*poArchive >>	pPlane->des_py;
		*poArchive >>	pPlane->des_pz;
		*poArchive >>	pPlane->des_nx;
		*poArchive >>	pPlane->des_ny;
		*poArchive >>	pPlane->des_nz;
		*poArchive >>	pPlane->des_calc;
		*poArchive >>	pPlane->des_sta;

}
void DCP::DCP06ConfigControllerC::load_line(CPI::CFG::ArchiveC* poArchive, S_LINE_BUFF* pLine)
{
	*poArchive >>	pLine->id;
	for(int i=0; i < MAX_POINTS_IN_LINE; i++)
	{
		load_point(poArchive,&pLine->points[i]);
	}
	*poArchive >>	pLine->px;
	*poArchive >>	pLine->py;
	*poArchive >>	pLine->pz;
	*poArchive >>	pLine->ux;
	*poArchive >>	pLine->uy;
	*poArchive >>	pLine->uz;
	*poArchive >>	pLine->calc;
	*poArchive >>	pLine->sta;
	*poArchive >>	pLine->des_px;
	*poArchive >>	pLine->des_py;
    *poArchive >>	pLine->des_pz;
	*poArchive >>	pLine->des_ux;
	*poArchive >>	pLine->des_uy;
	*poArchive >>	pLine->des_uz;
	*poArchive >>	pLine->des_calc;
	*poArchive >>	pLine->des_sta;

}



void DCP::DCP06ConfigControllerC::save_circle_buff(CPI::CFG::ArchiveC* poArchive, S_CIRCLE_BUFF* pBuff)
{
	*poArchive <<	pBuff->id;
	for(int i=0; i < MAX_POINTS_IN_CIRCLE; i++)
	{
		save_point(poArchive,&pBuff->points[i]);
	}

	*poArchive <<	pBuff->cx;
	*poArchive <<	pBuff->cy;
	*poArchive <<	pBuff->cz;
	*poArchive <<	pBuff->diameter;
	*poArchive <<	pBuff->calc;
	*poArchive <<	pBuff->sta;
	*poArchive <<	pBuff->vi_;
	*poArchive <<	pBuff->vj_;
	*poArchive <<	pBuff->vk_;
}

void DCP::DCP06ConfigControllerC::load_circle_buff(CPI::CFG::ArchiveC* poArchive, S_CIRCLE_BUFF* pBuff)
{
	*poArchive >>	pBuff->id;
	for(int i=0; i < MAX_POINTS_IN_CIRCLE; i++)
	{
		load_point(poArchive,&pBuff->points[i]);
	}

	*poArchive >>	pBuff->cx;
	*poArchive >>	pBuff->cy;
	*poArchive >>	pBuff->cz;
	*poArchive >>	pBuff->diameter;
	*poArchive >>	pBuff->calc;
	*poArchive >>	pBuff->sta;
	*poArchive >>	pBuff->vi_;
	*poArchive >>	pBuff->vj_;
	*poArchive >>	pBuff->vk_;
} 

void DCP::DCP06ConfigControllerC::load_demo_licenses(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{
	DateTime sta, last;

	//*poArchive >> poModel->startDate;
	*poArchive >> sta;
	sta = (sta-10.3) * 3.1;
    poModel->startDate = sta;
	

	// *poArchive >> poModel->lastStartedDate;
	*poArchive >> last;
	last = (last -10.3) * 2.5;
	poModel->lastStartedDate = last;

	*poArchive >> poModel->sKeyCodeDemo1;
	//*poArchive >> poModel->sKeyCodeDemo2;
	//*poArchive >> poModel->sKeyCodeDemo3;
	//*poArchive >> poModel->sKeyCodeDemo4;
	//*poArchive >> poModel->sKeyCodeDemo5;

}

void DCP::DCP06ConfigControllerC::save_demo_licenses(CPI::CFG::ArchiveC* poArchive, DCP06ModelC* poModel)
{

	DateTime temp1 = poModel->startDate / 3.1 + 10.3;
	DateTime temp2 = poModel->lastStartedDate / 2.5 + 10.3;

	*poArchive << temp1;//poModel->startDate;
	*poArchive << temp2;//poModel->lastStartedDate;
	*poArchive << poModel->sKeyCodeDemo1;
/*	*poArchive << poModel->sKeyCodeDemo2;
	*poArchive << poModel->sKeyCodeDemo3;
	*poArchive << poModel->sKeyCodeDemo4;
	*poArchive << poModel->sKeyCodeDemo5*/;
}
