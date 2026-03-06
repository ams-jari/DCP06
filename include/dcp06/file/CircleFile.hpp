// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
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

#ifndef DCP_CIRCLEFILEDLG_HPP
#define DCP_CIRCLEFILEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/Circle.hpp>
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
	class CircleFileDialog;
	class CircleFileModel;

	class CircleFileFunc;

    // Description: Tabbed controller for the Hello World application
    class CircleFileController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            CircleFileController(Model* pModel);
			~CircleFileController();

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
            CircleFileController( const CircleFileController& oCircleFileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CircleFileController& operator=( const CircleFileController& oCircleFileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			Model* m_pModel;
            CircleFileDialog* m_pDlg;
			CircleFileModel* m_pDataModel;
    };


    class CircleFileDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   CircleFileDialog(CircleFileModel* pModel);

            // Description: Destructor
            virtual ~CircleFileDialog();

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
			GUI::ComboLineCtrlC* m_pSize;
			GUI::ComboLineCtrlC* m_pDate;
			GUI::ComboLineCtrlC* m_pTime;
			GUI::ComboLineCtrlC* m_pFreeSpace;

			CircleFileModel* m_pDataModel;
	};

	class CircleFileModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            CircleFileModel(Model* pModel);

            // Description: Destructor
            //
            virtual ~CircleFileModel();
			Common* pCommon;
			CircleFileFunc* m_pFileFunc;
			char x_diff[20];
			char y_diff[20];
			char z_def[20];

	private:
			Model* m_pModel;
    };

   // Description: Tabbed controller for the Hello World application
    class CircleFileFunc
    {
        public:

            // Description: Constructor
            //CircleFileFunc();
			//AdfFileFunc(const char* filename, bool bCreate=false);
			CircleFileFunc(boost::filesystem::path * FileInfo,Model* pModel);
			CircleFileFunc(Model* pModel);

			~CircleFileFunc();

			bool setFile(StringC filename);
			short open_crl_file();

			short open_circle_file(const char* mode);
			char* getFileName();
			char* getModDate();
			char* getModTime();
			char* getFileSizeString();
			bool isExists();
			//short get_next_id(char *did);
			//short delete_point_from_cdf(short pno);
			//short open_crl_file();
			short save_circle_to_file(StringC id, CircleModel* pModel);
			short create_new_circlefile(char  *fname);
			short create_new_file(char  *fname);
			short delete_file(void);
			void CloseFile();
			short IsOpen() { return opened;}


	private:
			Model* m_pModel;
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
			void ParseModTime(boost::filesystem::path * FileInfo);
			void ParseFileNameAndPath(boost::filesystem::path * FileInfo);
			short fopen1(const char* mode);
			short access1(char *fname);
			short remove1(char *fname);
			char trow[0xFF];
			Common* m_pCommon;
    };

};

#endif // DCP_BASEGUI_HPP




