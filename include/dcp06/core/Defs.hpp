// ====================================================================================================================

#ifndef DCP_DIALOGS_HPP      
#define DCP_DIALOGS_HPP

// C++03 / pre-C++11 compatibility: nullptr was introduced in C++11 (VS2010)
#if defined(_MSC_VER) && _MSC_VER < 1600
#ifndef nullptr
#define nullptr NULL
#endif
#endif

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

// Description: DCP06 - Onboard 3D measurement for Leica total stations (based on DCP05)
//
namespace DCP
{
	//#define AMS_DEBUG 1
	#define CUSTOMER_BUILD 1  // was DCP06_CUSTOMER
	
	// Do not use these (Captivate 8)
    //#define TS16 // WITHOUT SCANNING
    //#define CS35 1 
	//#define CS20 1

#ifdef CS35 // defined Peropocessor definitions in in visual studio 2022 project file: DCP05-CS.vcxproj
	//#define FILE_STORAGE  CPI::deviceLocalMemory
#else
	//#define FILE_STORAGE CPI::deviceSdCard

	// MS60 id default , select one of below definitions to compile TS16 or CS20

	//#define TS16 // WITHOUT SCANNING
	//#define CS20 1

#endif

    #define MAIN_MENU			100
	#define ORIENTATION_MENU	101
	#define USER_DLG			103
	#define UNIT_DLG			104
	#define A321_DLG				105   // 321 Alignment (was DOM)
	#define SPECIAL_MENU		106
	#define _3DFILE_DLG			107
	#define FILE_MENU			108
	#define APPLICATION_MENU			109
	#define CALCULATION_MENU			110
	#define DIST_FILE_DLG				111
	#define ANGLE_FILE_DLG				112
	#define LINE_SETTING_DLG			113
	#define LINE_SETTING_CONF_DLG		114
	#define HOME_POINTS_DLG			115
	#define CIRCLE_DLG			116
	#define CALC_DIST_DLG			117
	#define CALC_ANGLE_DLG			117
	#define  BESTFIT_POINT_DLG			118   // Best Fit point selection (was POM)
	#define _3DMEAS_DLG				119
	#define SELECT_FILE_DLG				120
	#define SELECT_MULTIPOINTS_DLG				121
	#define SELECT_POINT_DLG				122
	#define INPUT_TEXT_DLG				123
	#define OFFSV_DLG				124
	#define MEASV_DLG				125
	#define ROTATE_PLANE_DLG				126
	#define ROTATE_LINE_DLG				127
	#define A321_USERDEF_DLG				128   // 321 UserDef (was DOM)
	#define BESTFIT_DLG	129   // Best Fit Alignment (was POM)
	#define SELECT_ONE_POINT_DLG				130
	#define ADD_TOOL_DLG				131
	#define _3DFILEVIEW_DLG				132
	#define  RES_LINE_DLG				133
	#define  CHST_DLG					134
	#define  RES_PLANE_DLG				135
	#define  RES_BESTFIT_DLG				136
	#define  CALC_2_POINTS_DLG			137
	#define INFO_DLG					138
	#define SELECT_TOOL_DLG				139
	#define INIT_DLG					140
	#define MEAS_DLG					141
	#define PLANE_DLG					142
	#define	LINE_DLG					143
	#define _3DROLLER_DLG				144
	#define HIDDENPOINT_DLG				145
	#define HIDDENPOINT_CONF_DLG		146
	#define SEPARATE_RECORDING_DLG		147
	#define TOOL_DLG					148
	#define XORYORZ_DLG					148
	#define SELECT_COORDINATE_DLG		149
	#define AUTOMATCH_DLG				150
	#define RES_CIRCLE_DLG				151
	#define VIEWCDF_DLG					152
	#define VIEWAGF_DLG					153
	#define VIEWCDF_EDIT_DLG			154
	#define VIEWAGF_EDIT_DLG			155
	#define SHAFT_DLG					156
	#define REG_DLG						157
	#define CIRCLE_SAVE_DLG				158
	#define CIRCLE_FILE_DLG				159
	#define SHAFT_FILE_DLG				160
	#define SHAFT_SAVE_DLG				161
	#define RES_SHAFT_DLG				162
	#define CALC_CIRCLE_DLG				163
	#define PLANE_USERDEF_DLG			164
	#define LINE_USERDEF_DLG			165
	#define A321_USERDEF_STATUS_DLG		166
	
	#define CAMERA_DLG					167
	#define SCAN_DLG					168 
	#define ADV_SCAN_DLG				169
	#define LINEFIT_DLG				169


		
	
	// CONTROLLERS
	#define INIT_CONTROLLER					2
	#define INIT_USER_CONTROLLER			3
	#define INIT_UNIT_CONTROLLER			4
	#define A321_CONTROLLER					5
	#define DEFINE_PLANE_CONTROLLER			6
	#define DEFINE_LINE_CONTROLLER			7
	#define MEAS_CONTROLLER					8
	#define SPECIAL_MENU_CONTROLLER			9
	#define BESTFIT_CONTROLLER					10
	#define CHST_CONTROLLER					11
	#define CALCULATION_CONTROLLER			12
	#define APPLICATION_MENU_CONTROLLER		13
	#define XYZ_CONTROLLER					14
	#define HIDDENPOINT_CONTROLLER			15
	#define HIDDENPOINT_CONF_CONTROLLER		16
	#define SEPARATE_RECORDING_CONTROLLER	17
	#define TOOL_CONTROLLER					18
	#define _3DMEAS_CONTROLLER				19
	#define DIST_FILE_CONTROLLER			20
	#define ANGLE_FILE_CONTROLLER			21
	#define FILE_CONTROLLER					22
	#define _3DFBS_CONTROLLER				23
	#define LINE_SETTING_CONTROLLER			24
	#define LINE_SETTING_CONF_CONTROLLER	25
	#define HOME_POINTS_CONTROLLER			26
	#define CIRCLE_CONTROLLER				27
	#define CALC_DIST_CONTROLLER			28
	#define CALC_ANGLE_CONTROLLER			29 
	#define BESTFIT_POINT_CONTROLLER			30
	#define SELECT_FILE_CONTROLLER			31
	#define SELECT_MULTIPOINTS_CONTROLLER	32
	#define SELECT_POINT_CONTROLLER			33
	#define INPUT_TEXT_CONTROLLER			34
	#define COPY_FILE_TO_CONTROLLER			35
	#define OFFSV_CONTROLLER				36
	#define MEASV_CONTROLLER				37
	#define ROTATE_PLANE_CONTROLLER			38
	#define ROTATE_LINE_CONTROLLER			39
	#define	MEAS_XYZ_CONTROLLER				40	
	#define DO_MEAS_XYZ_FACE1_CONTROLLER	41
	#define DO_MEAS_XYZ_FACE2_CONTROLLER	42
	#define CHANGE_FACE_CONTROLLER			43
	#define SELECT_ONE_POINT_CONTROLLER		44
	#define	MEAS_X_CONTROLLER				45
	#define	MEAS_Y_CONTROLLER				46	
	#define	MEAS_Z_CONTROLLER				47	
	#define	INPUT_FIRST_POINTID_CONTROLLER	48	
	#define	ADD_TOOL_CONTROLLER				49	
	#define	_3DFILEVIEW_CONTROLLER			50	
	#define	MEAS_DIST_CONTROLLER			51	
	#define	RES_LINE_CONTROLLER				52	
	#define	CHST_POINT_CONTROLLER			53
	#define	RES_PLANE_CONTROLLER			54	
	#define	RES_BESTFIT_CONTROLLER				55	
	#define	CALC_2_POINTS_CONTROLLER		56	
	#define SELECT_REF_POINT_CONTROLLER		57
	#define SELECT_TARGET_POINT_CONTROLLER	58
	#define CIRCLE_PLANE_MEAS_CONTROLLER	59
	#define RES_CIRCLE_CONTROLLER			60
	#define SELECT_TOOL_CONTROLLER			61
	#define AIM_CONTROLLER					62
	#define SELECT_MULTIPOINTS_LINE_CONTROLLER	63
	#define CALC_LINE_CONTROLLER				64
	#define SELECT_MULTIPOINTS_PLANE_CONTROLLER	65
	#define CALC_PLANE_CONTROLLER				66
	//#define SELECT_TARGET_POINT_CONTROLLER		67	
	#define SELECT_MULTIPOINTS_REF_LINE_CONTROLLER	68 
	#define SELECT_MULTIPOINTS_TARGET_LINE_CONTROLLER 69
	#define SELECT_MULTIPOINTS_REF_PLANE_CONTROLLER	70
	#define SELECT_MULTIPOINTS_TARGET_PLANE_CONTROLLER	71
	#define AUTOMATCH_CONTROLLER	72
	#define	MEAS_POS2_CONTROLLER			73	
	#define CALC_CIRCLE_CONTROLLER			74
	#define CHANGE_FACE_CONTROLLER_AND_EXIT		75
	#define VIEWCDF_CONTROLLER					76
	#define VIEWAGF_CONTROLLER					77
	#define VIEWCDF_EDIT_CONTROLLER					78
	#define VIEWAGF_EDIT_CONTROLLER					79
	#define SHAFT_ALIGMENT_CONTROLLER				80
	#define SHAFT_ALIGMENT_LINE_CONTROLLER			81
	#define	CIRCLE_SAVE_CONTROLLER					82
	#define	CIRCLE_FILE_CONTROLLER					83
	#define	SHAFT_FILE_CONTROLLER					84
	#define	SHAFT_SAVE_CONTROLLER					85
	#define	RES_SHAFT_CONTROLLER					85
	#define	CALCULATION_CIRCLE_CONTROLLER			86
	#define DO_AIM_CONTROLLER						87
	#define A321_USERDEF_CONTROLLER					88
	#define DEFINE_PLANE_USERDEF_CONTROLLER		89
	#define DEFINE_LINE_USERDEF_CONTROLLER		90

	#define DEFINE_MEAS_START_CONTROLLER		91
	#define DEFINE_DIST_MEAS_START_CONTROLLER		92
	#define SCAN_CONTROLLER							93
	#define FILE_CONTROLLER_BESTFIT						94
	#define FILE_CONTROLLER_CHST					95
	#define SIMPLE_SCAN_CONTROLLER							96
	#define LINE_FITTING_CONTROLLER							97
	#define LINEFIT_MEAS_CONTROLLER					98
	#define MID_POINT_CONTROLLER					99
	#define MEAS_MID_POINT_CONTROLLER				100

	#define	DO_MEAS_DIST_CONTROLLER			101	


	#define NO			0
	#define YES			1

	// line fitting
	#define FIRST_POINT 0
	#define LAST_POINT 1
	#define INTERPOLATION 3
	#define MANUALLY_ENTERED 4

	#define SHIFT_NO 0
	#define SHIFT_RIGHT 1
	#define SHIFT_LEFT 2
	#define SHIFT_UP 3
	#define SHIFT_DOWN 4
	#define SHIFT_FORWARD 3
	#define SHIFT_BACKWARD 4
	

	#define ROTATE_NO 0 
	// for refline = horizontal
	#define ROTATE_HORIZONTAL 1 
	#define ROTATE_VERTICAL 2 
	// for refline = vertical
	#define ROTATE_VERTICAL_LEFT_RIGHT 1 //around x-axis
	#define ROTATE_VERTICAL_DEPTH 2	// around y-axis


	#define REF_LINE_HORIZONTAL 0
	#define REF_LINE_VERTICAL 1

	// 2 Face Measurement

	#define PII 3.141592654
	#define DPR 180.0/PII

	

	/*
	#define X_LINE	1
	#define Y_LINE 	2
	#define Z_LINE	3
	
	#define XY_PLANE	1
	#define ZX_PLANE	2
	#define YZ_PLANE	3
	*/
	// LENGTHS
	#define DCP_USER_LENGTH			6
	#define DCP_POINT_ID_LENGTH		64  // max point ID length (DCP06; was 6 in DCP05)
	#define DCP_XYZ_VALUE_LENGTH	9
	#define DCP_HIDDENPOINT_DIST_LENGTH 9
	#define DCP_TOOL_ID_LENGTH 6


	#define HIDDEN_POINT	10
	#define X_OR_Y_OR_Z		11	
	#define CIRCLE			12
	#define SEPARATE_REC	13
	#define HOME_POINTS		14
	#define MID_POINT		15
	

	// FLAGS TO SHOW SPECIALMENUAITEMS
	#define DISABLE_HIDDEN_POINT  1
	#define DISABLE_X_OR_Y_OR_Z	2
	#define DISABLE_CIRCLE			4
	#define DISABLE_SEPARATE_REC	8
	#define DISABLE_HOME_POINTS	16
	#define DISABLE_MID_POINT		32
	#define SHOW_ALL_SPECIAL_MENU 0	

	#define CALC_DISTANCE	10
	#define CALC_ANGLE		11
	#define CALC_CIRCLE		12

	#define	LINE_SETTING 10
	#define FRONT_BACK_SINGLE 11
	#define SHAFT_ALIGMENT	  12
	#define SCANNING		13
	#define SIMPLE_SCANNING 14
	#define LINE_FITTING	15

	// ORIENTATION (321 Alignment / Best Fit - legacy DOM/POM)
	#define DCP_A321_MENU			11
	#define DCP_BESTFIT_MENU			12
	#define DCP_CHANGE_STATION_MENU 13
	#define DCP_USER_DEFINED_MENU	14

	// FILE
	// DCP_SKIP_FILE_TYPE_MENU: when defined (in project PreprocessorDefinitions), "File" goes
	// directly to Job File (OPEN/NEW/COPY/SWAP/CLOSE) instead of showing the file-type selection.
	#define DCP_3DFILE_MENU			11
	#define DCP_DIST_FILE_MENU		12
	#define DCP_ANGLE_FILE_MENU		13
	#define DCP_CIRCLE_FILE_MENU		14
	#define DCP_SHAFT_FILE_MENU		15


	#define MAX_POINTS_IN_FILE	500
	#define MAX_HOME_POINTS		10
	// Best Fit point limits (legacy POM)
	#define MAX_BESTFIT_POINTS		20
	#define MIN_BESTFIT_POINTS		3

	#define MAX_MID_POINTS 20
	// userdef
	#define MAX_USERDEF_POINTS 20

	//#define ADF_FILE		1

	#define ONLY_ADF 	1
	#define ALL_FILES 	2
	#define	CALCDIST_FILE	3
	#define	CALCANGLE_FILE	4
	#define	CIRCLE_FILE	5
	#define SHAFT_FILE	6
	#define ADF_BF_STA 7
	#define FILE_TYPE_JOBS 8  // was DCP06_JOBS
	#define IMPORT_ADF 9

	#define ACTUAL	1
	#define DESIGN	2
	#define BOTH	3
	//structures

	#define POINT_ID_BUFF_LEN	65  // DCP_POINT_ID_LENGTH + 1 (was 7 in DCP05)
	// Format for sprintf into point_id buffers: "%-64.64s"
	#define DCP_XSTR(x) #x
	#define DCP_STR(x) DCP_XSTR(x)
	#define DCP_POINT_ID_FMT "%" "-" DCP_STR(DCP_POINT_ID_LENGTH) "." DCP_STR(DCP_POINT_ID_LENGTH) "s"
	#define POINT_STATUS_BUFF_LEN	10
	#define MEASURER_BUFF_LEN	11
	#define INSTRUMENT_BUFF_LEN	31
	#define INSTRUMENT_ID_BUFF_LEN	11
	#define DATE_BUFF_LEN 11
	#define TEMP_AIR_BUFF_LEN	6
	#define TEMP_OBJ_BUFF_LEN	6
	#define MATERIAL_BUFF_LEN	11
	#define PLANE_ID_BUFF_LEN 65   // match POINT_ID_BUFF_LEN for plane point IDs
	#define LINE_ID_BUFF_LEN 65   // match POINT_ID_BUFF_LEN for line point IDs
	#define CIRCLE_ID_BUFF_LEN 65 // match POINT_ID_BUFF_LEN for circle point IDs

	#define XYZ_VALUE_BUFF_LEN	10
	#define NOTE_BUFF_LEN	7
	#define FILENAME_BUFF_LEN	20
	#define DCP_JOB_ID_MAX_LEN	64  // max job/file name length (DCP06; was 8 in DCP05)
	#define POINTS_COUNT_BUFF_LEN	10
	#define FILE_SIZE_BUFF_LEN	20
	#define FILE_DATE_BUFF_LEN	12
	#define FILE_TIME_BUFF_LEN	12

	#define XYZ_MEA_AND_DIFF_BUFF_LEN 20
	typedef struct
	{
		short	iId;
		char	point_id[POINT_ID_BUFF_LEN];
		bool	bActualDefined;
		bool	bDesignDefined;
		bool	bActualSelected;
		bool	bDesignSelected;
		bool	bPointSelected;

	} S_SELECT_POINTS;

	typedef struct {
	char measurer[MEASURER_BUFF_LEN];
	char instrument[INSTRUMENT_BUFF_LEN];
	char instrument_id[INSTRUMENT_ID_BUFF_LEN];
	char date[DATE_BUFF_LEN];
	char tempair[TEMP_AIR_BUFF_LEN];
	char tempobj[TEMP_OBJ_BUFF_LEN];
	char material[MATERIAL_BUFF_LEN];

	}S_HEADER_INFO;


	typedef struct
	{
		short no;
		char point_id[POINT_ID_BUFF_LEN];
		char point_status[POINT_STATUS_BUFF_LEN];
	}S_SELECT_POINT;
	
	#define MAX_SELECT_POINTS 500
	#define MAX_POINTS_IN_PLANE 20
	#define MAX_POINTS_IN_HIDDENPOINT_BAR 5
	#define MAX_POINTS_IN_LINE  20 
	#define MAX_POINTS_IN_CIRCLE 20 // oli 30 4.11.97

	// ACTIVE COORDINATE SYSTEM

	#define POINT 0
	#define LINE	1
	#define PLANE 2

	enum DCP_LEFTRIGHTHAND
	{
		LEFTHAND=0,
		RIGHTHAND
	};

	enum DCP_2_FACE_MEAS_TYPES
	{
		ALL=1,
		DIST,
		ALL_MANUAL,
		DIST_MANUAL
	};
	/*
	enum DCP_POINT_STATUS
	{
		POINT_NOT_DEFINED=0,
		POINT_DESIGN,
		POINT_MEASURED,
		POINT_DESIGN_REJECTED,
		POINT_MEASURED_REJECTED
	};
	*/
	#define POINT_NOT_DEFINED		0
	#define POINT_DESIGN			1
	#define POINT_MEASURED			2
	#define POINT_DESIGN_REJECTED	3
	#define POINT_MEASURED_REJECTED 4
	
	/*
	enum DCP_LINE_STATUS
	{
		LINE_NOT_DEFINED=0,
		LINE_DEFINED
	};
	*/
	#define LINE_NOT_DEFINED	0
	#define LINE_DEFINED		1
	
	/*
	enum DCP_PLANE_STATUS
	{
		PLANE_NOT_DEFINED=0,
		PLANE_DEFINED
	};
	*/
	#define PLANE_NOT_DEFINED	0
	#define PLANE_DEFINED	1
	/*
	enum DCP_PLANE_TYPE
	{
			XY_PLANE =0,
			ZX_PLANE,
			YZ_PLANE,
			MEAS_PLANE,
			CIRCLE_POINTS_PLANE
	};
	*/
	#define XY_PLANE 1
	#define ZX_PLANE 2
	#define YZ_PLANE 3
	#define MEAS_PLANE 4
	#define CIRCLE_POINTS_PLANE 5

	/*
	enum DCP_LINE_TYPE
	{
		X_LINE=0,
		Y_LINE,
		Z_LINE
	};
	*/
	#define X_LINE 1
	#define Y_LINE 2
	#define Z_LINE 3
	#define MEAS_LINE 4

	/*
	enum DCP_UNIT
	{
		MM=1,
		INCH,
		FEET
	};
	*/
	#define MM		1
	#define INCH	2
	#define FEET	3


	/*
	enum DCP_COORDINATE_SYSTEM
	{
		DCS=0,
		OCSP,
		OCSD,
		OCSC,
		OCSU
	};
	*/
	#define DCS		0
	#define OCSP	1
	#define OCSD	2
	#define OCSC	3
	#define OCSU	4

	typedef struct
	{
		char point_id[POINT_ID_BUFF_LEN];
		double x;
		double y;
		double z;
		double xdes;
		double ydes;
		double zdes;
		short /*DCP_POINT_STATUS*/ sta; // 0=not defined, 1=design, 2=measured 3=Design rejected 4=measured/rejected
		short /*DCP_POINT_STATUS*/ dsta;
		double diameter;
		short /*DCP_COORDINATE_SYSTEM*/ cds; // coordinate system 0=scs, 1=ocs
		
		// for x or y or z
		short /*DCP_POINT_STATUS*/ xsta;
		short /*DCP_POINT_STATUS*/ ysta;
		short /*DCP_POINT_STATUS*/ zsta;
	} S_POINT_BUFF;
	
	#define MAX_LINEFIT_POINTS 50

	// String/buffer sizes for formatting and display
	#define STRING_BUFFER_SMALL      20
	#define STRING_BUFFER_MEDIUM    100
	#define STRING_BUFFER_LARGE     512
	#define STRING_BUFFER_PATH      1024

	// Geometry calculation limits
	#define MIN_POINTS_FOR_DISTANCE     3
	#define MIN_POINTS_FOR_LINE         2
	#define MIN_POINTS_FOR_PLANE        3
	#define MIN_POINTS_FOR_CIRCLE       3
	#define BOUNDARY_PLANE_POINTS       3
	#define DEFAULT_BOUNDARY_SIZE       500.0
	typedef struct {
		double height_diff;
		double line_offset;
		double points_distance;
		double distance_along_line;
		short sta; // 0=Not calculated 1=Calculated
		
	} S_LINE_FITTING_RESULTS;

	#define MAX_SCAN_POINTS 10000
	#define SCAN_POINT_ID_BUFF_LEN 10
	typedef struct
	{
		char point_id[SCAN_POINT_ID_BUFF_LEN];
		double x;
		double y;
		double z;
	} S_SCAN_POINT_BUFF;

	typedef struct {
		char id[PLANE_ID_BUFF_LEN];
		
		S_POINT_BUFF points[MAX_POINTS_IN_PLANE];
		double px;
		double py;
		double pz;
		double nx;
		double ny;
		double nz;
		short calc;
		short /*DCP_PLANE_STATUS*/ sta;                       /* 0=No defined 1=Design 2=Measured */
		
		// For design Values
		double des_px;
		double des_py;
		double des_pz;
		double des_nx;
		double des_ny;
		double des_nz;
		short des_calc;
		short /*DCP_PLANE_STATUS*/ des_sta;                       /* 0=No defined 1=Design 2=Measured */
	}S_PLANE_BUFF;

	typedef struct  {
		char id[LINE_ID_BUFF_LEN];
		S_POINT_BUFF  points[MAX_POINTS_IN_LINE]; 	/* Max 5 points in line */
		double px;
		double py;
		double pz;
		double ux;
		double uy;
		double uz;
		short calc;
		short /*DCP_LINE_STATUS*/ sta;                        /* 0=No defined 1=Design 2=Measured */
		// For design
		double des_px;
		double des_py;
		double des_pz;
		double des_ux;
		double des_uy;
		double des_uz;
		short des_calc;
		short /*DCP_LINE_STATUS*/ des_sta;                        /* 0=No defined 1=Design 2=Measured */
	}S_LINE_BUFF;

	typedef struct  {
		char id[CIRCLE_ID_BUFF_LEN ];
		S_POINT_BUFF  points[MAX_POINTS_IN_CIRCLE]; 	
		double cx;
		double cy;
		double cz;
		double vi_;
		double vj_;
		double vk_;
		double diameter;
		short calc;
		short /*DCP_POINT_STATUS*/ sta;                        /* 0=No defined 1=Design 2=Measured */
	} S_CIRCLE_BUFF;



	#define BACK 		0
	#define FRONT 		1
	#define FRONTBACK	2
	#define SINGLE		3

	#define TOOL_VALUE_MIN -9999.9
	#define TOOL_VALUE_MAX 99999.9
	#define MAX_TOOLS		10

	typedef struct {
		char tool_id[11];
		double x;
		double y;
		double z;
		short  prism_or_tape;
		double constant;
	} S_TOOL;
	
	enum ADF_TYPE
	{
		ADF=0,
		SCN,
		STA,
		BFT
	};	

	#define SD_CARD 0
	#define LOCAL_MEMORY 0
	#define USB_STICK 1
};

#endif // DCP_DIALOGS_HPP




