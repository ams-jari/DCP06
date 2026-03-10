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

#ifndef DCP_FILEDLG_HPP
#define DCP_FILEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================


#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>

#include <dcp06/core/Types.hpp>
#include <dcp06/file/AdfFileFunc.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class FileDialog;
	class FileModel;

    // Description: Tabbed controller for the Hello World application
    class FileController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            FileController(Model* pModel);
			~FileController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF3Pressed();
			virtual void OnSHF4Pressed();
			virtual void OnSHF5Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            FileController( const FileController& oFileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            FileController& operator=( const FileController& oFileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            FileDialog* m_pDlg;
			FileModel* m_pDataModel;
			StringC sFileToCreate;
			Model* m_pModel;
			short m_pendingExportFormat;  // ONLY_ADF/CSV/TXT/ASC when awaiting export filename
    };


    class FileDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   FileDialog(FileModel* pFileModel);

            // Description: Destructor
            virtual ~FileDialog();

			virtual void OnInitDialog(void);
			
			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();

			virtual void CreateFile(StringC strNewFileName, StringC sPointId);
			virtual void CopyFile(StringC strNewFileName);
			virtual void Swap();
			virtual void Close();
			virtual void Delete();
			virtual void Save();
			virtual void View();
			virtual void SelectFile(StringC sFile);


	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pNumberOfPoints;
			GUI::ComboLineCtrlC* m_pSize;
			GUI::ComboLineCtrlC* m_pDate;
			GUI::ComboLineCtrlC* m_pTime;
			GUI::ComboLineCtrlC* m_pFreeSpace;

			FileModel* m_pDataModel;
			StringC sSelectedFile;
		private:
			Common* m_pCommon;
			Model* m_pModel;

	};

   class FileModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            FileModel(Model* pModel);

            // Description: Destructor
            //
            virtual ~FileModel();
			
			AdfFileFunc* m_pAdfFile;
			char x_diff[20];
			char y_diff[20];
			char z_def[20];

		private:
			Model* m_pModel;
    };

};

#endif // DCP_BASEGUI_HPP




