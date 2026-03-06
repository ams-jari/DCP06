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
    class DCP06ModelC;
	class DCP06FileDlgC;
	class DCP06FileModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06FileControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06FileControllerC(DCP06ModelC* pDCP06Model);
			~DCP06FileControllerC();

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
            DCP06FileControllerC( const DCP06FileControllerC& oDCP06FileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06FileControllerC& operator=( const DCP06FileControllerC& oDCP06FileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06FileDlgC* m_pDlg;
			DCP06FileModelC* m_pDataModel;
			StringC sFileToCreate;
			DCP06ModelC* m_pDCP06Model;
    };


    class DCP06FileDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   DCP06FileDlgC(DCP06FileModelC* pFileModel);

            // Description: Destructor
            virtual ~DCP06FileDlgC();

			virtual void OnInitDialog(void);
			
			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;

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

			DCP06FileModelC* m_pDataModel;
			StringC sSelectedFile;
		private:
			DCP06CommonC* m_pCommon;
			DCP06ModelC* m_pDCP06Model;

	};

   class DCP06FileModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06FileModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            //
            virtual ~DCP06FileModelC();
			
			AdfFileFunc* m_pAdfFile;
			char x_diff[20];
			char y_diff[20];
			char z_def[20];

		private:
			DCP06ModelC* m_pDCP06Model;
    };

};

#endif // DCP_BASEGUI_HPP




