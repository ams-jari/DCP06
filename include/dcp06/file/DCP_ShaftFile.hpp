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

#ifndef DCP_SHAFTFILEDLG_HPP
#define DCP_SHAFTFILEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/application/DCP_Shaft.hpp>
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
    class DCP05ModelC;
	class DCP05ShaftFileDlgC;
	class DCP05ShaftFileModelC;

	class ShaftFileFunc;

    // Description: Tabbed controller for the Hello World application
    class DCP05ShaftFileControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05ShaftFileControllerC(DCP05ModelC* pDCP05Model);
			~DCP05ShaftFileControllerC();

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
            DCP05ShaftFileControllerC( const DCP05ShaftFileControllerC& oDCP05ShaftFileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05ShaftFileControllerC& operator=( const DCP05ShaftFileControllerC& oDCP05ShaftFileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05ShaftFileDlgC* m_pDlg;
			DCP05ShaftFileModelC* m_pDataModel;
			DCP05ModelC* m_pDCP05Model;
    };


    class DCP05ShaftFileDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
	 public:

			enum eCtrlId
			{
				eFile=1,
				eSize,
				eDate,
				eTime,
				eFreeSpace
			};

		   DCP05ShaftFileDlgC(DCP05ShaftFileModelC* pModel);

            // Description: Destructor
            virtual ~DCP05ShaftFileDlgC();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;

			virtual void RefreshControls();


	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pSize;
			GUI::ComboLineCtrlC* m_pDate;
			GUI::ComboLineCtrlC* m_pTime;
			GUI::ComboLineCtrlC* m_pFreeSpace;

			DCP05ShaftFileModelC* m_pDataModel;
	};

	class DCP05ShaftFileModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05ShaftFileModelC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            //
            virtual ~DCP05ShaftFileModelC();
			DCP05CommonC* pCommon;
			ShaftFileFunc* m_pFileFunc;
			char x_diff[20];
			char y_diff[20];
			char z_def[20];
    };

   // Description: Tabbed controller for the Hello World application
    class ShaftFileFunc
    {
        public:

            // Description: Constructor
            //ShaftFileFunc();
			//AdfFileFunc(const char* filename, bool bCreate=false);
			ShaftFileFunc(boost::filesystem::path* FileInfo, DCP05ModelC* pDCP05Model);//CPI::FileInfoC* FileInfo);
			ShaftFileFunc(DCP05ModelC* pDCP05Model);

			~ShaftFileFunc();

			bool setFile(StringC filename);
			short open_crl_file();

			short open_Shaft_file(const char* mode);
			char* getFileName();
			char* getModDate();
			char* getModTime();
			char* getFileSizeString();
			bool isExists();
			//short get_next_id(char *did);
			//short delete_point_from_cdf(short pno);
			//short open_crl_file();
			short save_shaft_to_file(StringC id, DCP05ShaftModelC* pModel);
			short create_new_shaftfile(char  *fname);
			short create_new_file(char  *fname);
			short delete_file(void);
			void CloseFile();
			short IsOpen() { return opened;}


	private:
			DCP05ModelC* m_pDCP05Model;
			char m_cPath[CPI::LEN_PATH_MAX];
			short opened;
			bool m_bExists;
			int m_lSize;
			char m_cPathAndFileName[CPI::LEN_PATH_MAX];
			char m_cDate[FILE_DATE_BUFF_LEN]; //20
			char m_cTime[20];
			char m_cFileName[FILENAME_BUFF_LEN];//20
			char m_cSize[FILE_SIZE_BUFF_LEN];
			FILE* m_pFile;

			short open_file();
			short open_crl_file_name(const char* mode);
			short close_crl_file();
			void getPath();
			void ParseModTime(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			void ParseFileNameAndPath(boost::filesystem::path* FileInfo);//CPI::FileInfoC* FileInfo);
			short fopen1(const char* mode);
			short access1(char *fname);
			short remove1(char *fname);
			char trow[0xFF];
			DCP05CommonC* m_pCommon;
    };

};

#endif // DCP_BASEGUI_HPP




