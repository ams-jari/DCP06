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

#ifndef DCP_DISTFILEDLG_HPP
#define DCP_DISTFILEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class DistFileDialog;
	class DistFileModel;

	class CdfFileFunc;

    // Description: Tabbed controller for the Hello World application
    class DistFileController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DistFileController(Model* pModel);
			~DistFileController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF5Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DistFileController( const DistFileController& oDistFileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DistFileController& operator=( const DistFileController& oDistFileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DistFileDialog* m_pDlg;
			DistFileModel* m_pDataModel;
			Model* m_pModel;
    };


    class DistFileDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
	 public:

			enum eCtrlId
			{
				eFile=1,
				eNumberOfPoints,
				eSize,
				eDate,
				eTime,
				eFreeSpace
			};

		   DistFileDialog(DistFileModel* pModel);

            // Description: Destructor
            virtual ~DistFileDialog();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();


	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pNumberOfPoints;
			GUI::ComboLineCtrlC* m_pSize;
			GUI::ComboLineCtrlC* m_pDate;
			GUI::ComboLineCtrlC* m_pTime;
			GUI::ComboLineCtrlC* m_pFreeSpace;

			DistFileModel* m_pDataModel;
	};

	class DistFileModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DistFileModel(Model* pModel);

            // Description: Destructor
            //
            virtual ~DistFileModel();
			Common* pCommon;
			CdfFileFunc* m_pFileFunc;
			char x_diff[20];
			char y_diff[20];
			char z_def[20];
    };


	   // Description: Tabbed controller for the Hello World application
    class CdfFileFunc
    {
        public:

            // Description: Constructor
            //CdfFileFunc();
			//AdfFileFunc(const char* filename, bool bCreate=false);
			CdfFileFunc(boost::filesystem::path* FileInfo,Model* pModel);//CPI::FileInfoC* FileInfo);
			CdfFileFunc(Model* pModel);

			~CdfFileFunc();

			bool setFile(StringC filename);


			short open_calcdist_file(const char* mode);
			char* getFileName();
			char* getModDate();
			char* getModTime();
			char* getPointsCountString();
			char* getFileSizeString();
			short getPointsCount();
			bool isExists();
			short get_next_id(char *did);
			short delete_point_from_cdf(short pno);
			short open_cdf_file();
			short calcdist_pnt(short point);
			short cdf_save_pnt();
			short copy_distfile();
			short copy_distfile_(char *to_fname, short rec);
			short save_calcdist_to_file(char *dist,char *refname,char *reftype, char *trgname,char *trgttype, char *bDid, char *bNote,short askdid);
			short create_new_calcdistfile(char  *fname);
			short create_new_file(char  *fname);
			short delete_file(void);
			short is_id_exists(char *id, short Ind);
			short check_id(char *id, short Ind);
			short open_file(short reclen);
			
			short close_distfile();
			void CdfFileFunc::CloseFile();

			short points;
			short active_point;
			short IsOpen() { return opened;}

			char id[10];
			char dist[15];
			char actual[15];
			char design[15];
			char dev[15];
			char ref[15];
			char reftype[15];
			char target[15];
			char targettype[15];
			char note[15];

	private:
			Model* m_pModel;
			char m_cPath[CPI::LEN_PATH_MAX];
			short opened;
			bool m_bExists;
			int m_lSize;
			char m_cPathAndFileName[CPI::LEN_PATH_MAX];
			char m_cDate[FILE_DATE_BUFF_LEN]; //20
			char m_cTime[20];
			char m_cPointsCount[POINTS_COUNT_BUFF_LEN];
			char m_cFileName[FILENAME_BUFF_LEN];//20
			char m_cSize[FILE_SIZE_BUFF_LEN];
			FILE* m_pFile;

		

			void getPath();
			void ParseModTime(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			void ParseFileNameAndPath(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			short open_cdf_file_name(const char* mode);
			short close_cdf_file();
			short file_updated;
			void add_header(short Ind);
			short fopen1(const char* mode);
			short access1(char *fname);
			FILE* fopen2(FILE *pFile , char* fname, const char* mode);
			short  remove1(char *fname);
			char trow[0xFF];
			Common* m_pCommon;
	
    };
};

#endif // DCP_BASEGUI_HPP




