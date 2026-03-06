// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ----------------------------------------------------------------
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

#ifndef DCP_ADFFILEFUNC_HPP
#define DCP_ADFFILEFUNC_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class AdfFileFunc
    {
        public:

            // Description: Constructor
            AdfFileFunc(DCP06ModelC* pDCP06Model);
			AdfFileFunc(ADF_TYPE type,DCP06ModelC* pDCP06Model);
			//AdfFileFunc(const char* filename, bool bCreate=false);
			
			AdfFileFunc(boost::filesystem::path* FileInfo,DCP06ModelC* pDCP06Model);
			//AdfFileFunc(DCP06ModelC* pDCP06Model);
			

			~AdfFileFunc();
			char* getFileName();
			const char* getFullPath() const;
			char* getModDate();
			char* getModTime();
			char* getPointsCountString();
			short getPointsCount();
			char* getFileSizeString();
		
			bool setFile(const char* filename);
			bool setFile(StringC filename);
			bool setFileFromFullPath(const char* fullPath);
			bool isExists();
			void CloseFile();
			short select_pnt1(int newpnt, char *pid,char *xsta, char *xact, char *xdes,
													 char *ysta, char *yact, char *ydes,
													 char *zsta, char *zact, char *zdes,
													 char *note);
			short form_pnt1 (
						int point,
						char *pid,
						char *xsta, char *xact, char *xdes,
						char *ysta, char *yact, char *ydes,
						char *zsta, char *zact, char *zdes,
						char *note);

			short form_pnt(int point);

			short GetPointList(S_SELECT_POINTS* pList, short iMaxPoints, short iDef);
			short GetPointList(S_SELECT_POINT* pList, short iMaxPoints);
			short create_adf_file(char *fname, char* pointid, bool showOKMessage=true);
			short create_adf_file_at_path(const char* fullPath, char* pointid, bool showOKMessage = true);

			//int lCurrentPoint;
			short close_adf_file();
				short IsOpen() { return opened;}

			short copy_adf_file(char *to_fname);
			short swap_meas_design();
			void add_header_to_adf();
			short delete_adf_file(bool showMsg = true);
			void save_adf_to_ref_dat();
			short convert_ref_to_adf(char *filename);
			short add_header_fields(FILE *fp, char *filename);
			short get_ref_line(FILE *fp, char *pid, char *xdes, char *ydes,char *zdes,char *note);
			short add_new_pnt(FILE *fp, char *pid, char *xact, char *xdes,char *yact,char *ydes,char *zact,char *zdes,char *note);
			short form_next_pnt();
			short form_prev_pnt();
			short delete_point_from_adf(short pno);
			short form_save_pnt();
			short add_point(char *pid);
			short get_point_type(char *pid);
			short find_point(double x, double y, double z);
			void inc_id(char *id);
			short get_next_point_id(char *id);

			short cpoint;         // 1=FRONT 2=BACK
			short points;
			short point_type; 	// 0=single 1=front and back
			short always_single;
	
			// FRONT or SINGLE POINT
			short active_point_front;
			char pointid_front[POINT_ID_BUFF_LEN];
			char xdes_front[XYZ_VALUE_BUFF_LEN];
			char ydes_front[XYZ_VALUE_BUFF_LEN];
			char zdes_front[XYZ_VALUE_BUFF_LEN];
			char xmea_front[XYZ_VALUE_BUFF_LEN];
			char ymea_front[XYZ_VALUE_BUFF_LEN];
			char zmea_front[XYZ_VALUE_BUFF_LEN];
			char xsta_front;
			char ysta_front;
			char zsta_front;
			char note_front[NOTE_BUFF_LEN];

			// BACK POINT
			short active_point_back;
			char pointid_back[POINT_ID_BUFF_LEN];
			char xdes_back[XYZ_VALUE_BUFF_LEN];
			char ydes_back[XYZ_VALUE_BUFF_LEN];
			char zdes_back[XYZ_VALUE_BUFF_LEN];
			char xmea_back[XYZ_VALUE_BUFF_LEN];
			char ymea_back[XYZ_VALUE_BUFF_LEN];
			char zmea_back[XYZ_VALUE_BUFF_LEN];
			char xsta_back;
			char ysta_back;
			char zsta_back;
			char note_back[NOTE_BUFF_LEN];
			short seek_pid(char *pid);

			FILE* get_file_pointer();

	private:
			DCP06ModelC* m_pDCP06Model;
			int fstpnt,filpos;
			char trow[0xFF];
			char linebuff[255];

			// variables
			FILE* m_pFile;
			char m_cFileName[FILENAME_BUFF_LEN];//20
			int m_lSize;
			//short m_iPointsCount;
			char m_cPath[CPI::LEN_PATH_MAX];
			char m_cPathAndFileName[CPI::LEN_PATH_MAX];
			char m_cDate[FILE_DATE_BUFF_LEN]; //20
			char m_cTime[20];
			char m_cPointsCount[POINTS_COUNT_BUFF_LEN];
			bool m_bExists;
			char m_cSize[FILE_SIZE_BUFF_LEN];
							
			short reclen;
			//  functions
			void getPath();
			void ParseFileNameAndPath(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			void ParseModTime(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			short open_ADF_file_name(const char* mode);
			short fopen1(const char* mode);
			short ReadPointsCount(char* m_cPathAndFileName);
			short  access1(char *fname);
			FILE* fopen2(FILE *pFile , char* fname, const char* mode);
			
			
			short  remove1(char *fname);

			

			//char name[20];
			unsigned short date_;
			unsigned short time_;
			int size;
			
			
			short opened;
			short file_updated;
			
			DCP06CommonC* m_pCommon;
			ADF_TYPE adf_type;
    };
};

#endif // DCP_BASEGUI_HPP




