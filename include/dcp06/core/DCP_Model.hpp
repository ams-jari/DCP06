// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: Base control of the GUI (graphical user interface)
//
// $Workfile: HEW_Model.hpp $
//
// Summary  : The base control manages GUI control properties which are common for all controls 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright by Leica Geosystems AG, Heerbrugg 2002
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_MODEL_HPP      
#define DCP_MODEL_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================
#include <ABL_AppConfig.hpp>

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>

#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
	class DCP05ConfigControllerC;

    // Description: Hello World application class
    //              
    class DCP05ModelC : public  ABL::AppConfigModelC//public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05ModelC();

            // Description: Destructor
            //
            virtual ~DCP05ModelC();
			/*
			typedef struct
			{
				char user[DCP_USER_LENGTH+1];
				char user1[DCP_USER_LENGTH+1];
				char user2[DCP_USER_LENGTH+1];
				char user3[DCP_USER_LENGTH+1];
				char user4[DCP_USER_LENGTH+1];
				char user5[DCP_USER_LENGTH+1];
				char user6[DCP_USER_LENGTH+1];
				char user7[DCP_USER_LENGTH+1];
				char user8[DCP_USER_LENGTH+1];
				char user9[DCP_USER_LENGTH+1];
				char user10[DCP_USER_LENGTH+1];
			} DCP_USERNAMES;

			typedef struct 
			{
				double dcs_matrix[4][4];
				double ocsd_matrix[4][4]; 
				double ocsp_matrix[4][4]; 
				double ocsc_matrix[4][4]; 
				double ocsu_matrix[4][4];	
			} DCP_MATRIX;
			*/
			/*
			typedef struct
			{
				DCP_USERNAMES users;
				DCP_MATRIX matrix;
				double hidden_point_bar[MAX_POINTS_IN_HIDDENPOINT_BAR];				

				// DOM
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

			}DCP_DATA;
			*/

			short m_nOption;
			short iTest;

			// INIT
			/*
			char m_sUser[DCP_USER_LENGTH+1];
			char m_sUser1[DCP_USER_LENGTH+1];
			char m_sUser2[DCP_USER_LENGTH+1];
			char m_sUser3[DCP_USER_LENGTH+1];
			char m_sUser4[DCP_USER_LENGTH+1];
			char m_sUser5[DCP_USER_LENGTH+1];
			char m_sUser6[DCP_USER_LENGTH+1];
			char m_sUser7[DCP_USER_LENGTH+1];
			char m_sUser8[DCP_USER_LENGTH+1];
			char m_sUser9[DCP_USER_LENGTH+1];
			char m_sUser10[DCP_USER_LENGTH+1];
			*/
			StringC m_sUser;
			StringC m_sUser1;
			StringC m_sUser2;
			StringC m_sUser3;
			StringC m_sUser4;
			StringC m_sUser5;
			StringC m_sUser6;
			StringC m_sUser7;
			StringC m_sUser8;
			StringC m_sUser9;
			StringC m_sUser10;

			short m_nOverWriteInfo;
			short m_n2FaceMeas; 
			short m_nDesignValues;
			short m_nAutoIncrement;
			short m_nToolInfo;
			short m_nAverageCount;
			short m_nLeftRightHand;
			short m_nAutoMatch;
			short m_nUnit;
			short m_nDecimals;
			short m_nAmsLog;
			short m_nSaveTool;
			short m_nTargetType;
			float m_fTargetConst;

			

			StringC ADFFileName;

			//short m_ActiveCoordinateSystem;
			
			short /*DCP_COORDINATE_SYSTEM*/ active_coodinate_system;

			double dcs_matrix[4][4];
			double ocsd_matrix[4][4]; 
			double ocsp_matrix[4][4]; 
			double ocsc_matrix[4][4]; 
			double ocsu_matrix[4][4];	

			double dcs_inv_matrix[4][4];
			double ocsd_inv_matrix[4][4]; 
			double ocsp_inv_matrix[4][4]; 
			double ocsc_inv_matrix[4][4];
			double ocsu_inv_matrix[4][4];

			S_TOOL tool_table[MAX_TOOLS];
			short active_tool;
			double active_tool_x;
			double active_tool_y;
			double active_tool_z;
			double tool_trans_x;
			double tool_trans_y;
			double tool_trans_z;


			double hidden_point_bar[MAX_POINTS_IN_HIDDENPOINT_BAR];

			bool ocsd_defined;
			bool ocsp_defined;
			bool ocsc_defined;
			bool ocsu_defined;
			
			// DOM
			short dom_active_plane;
			short dom_active_line; 
			bool dom_hz_plane;
			S_PLANE_BUFF	dom_plane_buff[1];
			S_PLANE_BUFF	dom_hz_plane_buff[1];
			S_LINE_BUFF		dom_line_buff[1];
			S_POINT_BUFF	dom_ovalues_buff;
			S_POINT_BUFF	dom_ovalues_tool_buff;
			S_POINT_BUFF	dom_ref_point_buff;
			S_POINT_BUFF	dom_rot_plane_buff;
			S_POINT_BUFF	dom_rot_line_buff;
			
			// USERDEF
			short userdef_active_plane;
			short userdef_active_line; 
			bool userdef_hz_plane;
			S_PLANE_BUFF	userdef_plane_buff[1];
			S_PLANE_BUFF	userdef_hz_plane_buff[1];
			S_LINE_BUFF		userdef_line_buff[1];
			S_POINT_BUFF	userdef_ovalues_buff;
			S_POINT_BUFF	userdef_ovalues_tool_buff;
			S_POINT_BUFF	userdef_ref_point_buff;
			S_POINT_BUFF	userdef_rot_plane_buff;
			S_POINT_BUFF	userdef_rot_line_buff;
			
			S_POINT_BUFF	userdef_measured_points[MAX_USERDEF_POINTS];
			
			short userdef_plane_points_no[MAX_USERDEF_POINTS];
			short userdef_line_points_no[MAX_USERDEF_POINTS];
			short userdef_point_no;


			// POM
			S_POINT_BUFF	POM_point_DCS[MAX_POM_POINTS];
			S_POINT_BUFF	POM_point_OCS[MAX_POM_POINTS];
			S_POINT_BUFF	POM_point_RES[MAX_POM_POINTS];

			short pom_into_capture;
			short pom_into_template;

			// CHST
			S_POINT_BUFF	CHST_point_DCS[MAX_POM_POINTS];
			S_POINT_BUFF	CHST_point_OCS[MAX_POM_POINTS];
			S_POINT_BUFF	CHST_point_RES[MAX_POM_POINTS];
			short chst_last_sel;
			short chst_used_hz_plane;
			short stationNumber;

			// circle
			short circle_plane_type;
			S_PLANE_BUFF circle_planes[1];
			S_CIRCLE_BUFF circle_points[1];
			S_CIRCLE_BUFF circle_points_in_plane[1];
			double circle_cx;
			double circle_cy;
			double circle_cz;
			double circle_diameter;
			double circle_tool_radius;
			double circle_rms;
			double circle_vi;
			double circle_vj;
			double circle_vk;
			


			//SHAFT

			S_PLANE_BUFF shaft_calc_plane[1];
			S_LINE_BUFF shaft_line[1];
			short shaft_circle_plane_type;
			S_CIRCLE_BUFF shaft_circle_points[1];
			S_CIRCLE_BUFF shaft_circle_points_in_plane[1];
			double shaft_circle_cx;
			double shaft_circle_cy;
			double shaft_circle_cz;
			double shaft_circle_vi;
			double shaft_circle_vj;
			double shaft_circle_vk;
			double shaft_circle_diameter;
			short  shaft_ref_line;  // X,_LINE, Y_LINE_Z_LINE or MEAS_LINE
			double shaft_tool_radius;
			double shaft_circle_rms;
			
			// LINE FITTING
			S_LINE_BUFF linefitting_line[1];
			S_POINT_BUFF linefitting_points[MAX_LINEFIT_POINTS];	
			S_LINE_FITTING_RESULTS linefitting_results[MAX_LINEFIT_POINTS];

			double linefitting_manualHeight;
			double linefitting_shiftValue;
			double linefitting_rotateAngle;
			int linefitting_selectedHeight;
			int linefitting_selectedShift;
			int linefitting_selectedRotate;
			int linefitting_selectedRefLine;

			// home points
			S_POINT_BUFF home_points[MAX_HOME_POINTS];

			// files	
			StringC sPomFile;
			StringC sChstFile;
			StringC s3DFile;
			StringC sCalcDistFile;
			StringC sCalcAngleFile;
			StringC sCircleFile;
			StringC sShaftFile;


			bool isMotorized;
			bool isATR;
			bool isScanning;

			char sKeyCode[21];
			bool bDemoMode;
			short iStartCount;

			DCP05ConfigControllerC* poConfigController;

			/*unsigned int iVersion;
			unsigned int iRelease;
			*/
			//char sProgramVersion[100];
			//char sProgramDate[100];

			S_POINT_BUFF mid_points[MAX_MID_POINTS];

			uint8_t FILE_STORAGE1;

			DateTime startDate;
			DateTime lastStartedDate;
			bool fullDemoMode;
			char sKeyCodeDemo1[21];
			char sKeyCodeDemo2[21];
			char sKeyCodeDemo3[21];
			char sKeyCodeDemo4[21];
			char sKeyCodeDemo5[21];

			char sEnteredKeyCode[21];

			StringC SerialNumber;

		private:
			void initialize_matrix4x4(double pMatrix[4][4]);


    };

	
	 // Description: Survey config controller
    class DCP05ConfigControllerC : public ABL::AppConfigControllerC
    {
        public:

            // Description: Constructor
            // Input      : pParent - Pointer to parent controller
            //              poModel - Configuration model
            DCP05ConfigControllerC(GUI::ControllerC* pParent, DCP05ModelC* poModel = NULL);

            // Description: Destructor
            virtual ~DCP05ConfigControllerC(void);

            // Description: Get the configuration model
            // Return     : configuration model
            DCP05ModelC*    GetModel(void) const;

        protected:

            // Description: Copies the configuration data from the archive to the model
            // Remarks    : Must be overwritten by each application. If poArchive is NULL
            //              factory default values are set
            // Input      : ulKey       - configuration key
            //              poArchive   - configuration database archive
            virtual void UpdateModel(unsigned int ulKey, CPI::CFG::ArchiveC* poArchive);

            // Description: Copies the configuration data from the model to the archive
            // Remarks    : Must be overwritten by each application
            // Input      : ulKey       - configuration key
            //              poArchive   - configuration database archive
            virtual void ReadModel(unsigned int ulKey, CPI::CFG::ArchiveC* poArchive);

			/*virtual bool UpdateModelMigrated(
                unsigned int ulKey, CPI::CFG::ArchiveC* poArchive, unsigned int ulCnfVersion,
                unsigned int ulAppVersion);
			*/

        private:
			void save_matrix4x4(CPI::CFG::ArchiveC* poArchive, double matrix[4][4]);
			void load_matrix4x4(CPI::CFG::ArchiveC* poArchive, double matrix[4][4]);
			void save_hiddenpointbar_conf(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_hiddenpointbar_conf(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_home_points(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_circle_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_shaft_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);

			void load_adf_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_crl_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_shaft_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_init_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_dom_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_pom_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_chst_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_userdef_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_matrix_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_tool_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void load_lisence(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);

			void save_adf_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_crl_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_shaft_file_name(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_init_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_dom_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_pom_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_chst_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_userdef_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_matrix_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_tool_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_home_points(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_circle_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_shaft_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_lisence(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			
			void save_point(CPI::CFG::ArchiveC* poArchive, S_POINT_BUFF* pPoint);
			void save_line(CPI::CFG::ArchiveC* poArchive, S_LINE_BUFF* pLine);
			void save_plane(CPI::CFG::ArchiveC* poArchive, S_PLANE_BUFF* pPlane);


			void load_point(CPI::CFG::ArchiveC* poArchive, S_POINT_BUFF* pPoint);
			void load_plane(CPI::CFG::ArchiveC* poArchive, S_PLANE_BUFF* pPlane);
			void load_line(CPI::CFG::ArchiveC* poArchive, S_LINE_BUFF* pLine);

			void load_circle_buff(CPI::CFG::ArchiveC* poArchive, S_CIRCLE_BUFF* pBuff);
			void save_circle_buff(CPI::CFG::ArchiveC* poArchive, S_CIRCLE_BUFF* pBuff);	


			void load_linefitting_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_linefitting_data(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);

			void load_demo_licenses(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);
			void save_demo_licenses(CPI::CFG::ArchiveC* poArchive, DCP05ModelC* pModel);

	};
};

#endif // DCP_MODEL_HPP




