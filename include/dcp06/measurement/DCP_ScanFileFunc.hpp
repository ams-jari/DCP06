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

#ifndef DCP_ScanFileFunc_HPP
#define DCP_ScanFileFunc_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class ScanFileFunc
    {
        public:

            // Description: Constructor
            ScanFileFunc(DCP06ModelC* pDCP06Model);

			~ScanFileFunc();
			char* getFileName();
			char* getModDate();
			char* getModTime();
			char* getPointsCountString();
			short getPointsCount();
			char* getFileSizeString();
		
			bool setFile(const char* filename);
			bool setFile(StringC filename);
			bool isExists();
			void CloseFile();
			short create_adf_file(char *fname, bool showOKMessage=true);

			//long lCurrentPoint;
			short close_adf_file();
			short IsOpen() { return opened;}

			short delete_adf_file(bool showMsg = true);
			short ScanFileFunc::add_new_pnt(char *pid, char *xact,char *yact,char *zact);
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
			FILE* get_file_pointer();

	private:
			DCP06ModelC* m_pDCP06Model;
			long fstpnt,filpos;
			char trow[0xFF];
			char linebuff[255];

			// variables
			FILE* m_pFile;
			char m_cFileName[FILENAME_BUFF_LEN];//20
			long m_lSize;
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
			long size;
			
			
			short opened;
			short file_updated;
			
			DCP06CommonC* m_pCommon;
    };
};

#endif // DCP_BASEGUI_HPP




