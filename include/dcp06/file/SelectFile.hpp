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

#ifndef DCP_SELECTFILEDLG_HPP
#define DCP_SELECTFILEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
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
    class DCP06ModelC;

	class DCP06SelectFileDlgC;
	class DCP06SelectFileModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06SelectFileControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06SelectFileControllerC(short iFileType, StringC sTitle,DCP06ModelC* pDCP06Model);
			~DCP06SelectFileControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF5Pressed();

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06SelectFileControllerC( const DCP06SelectFileControllerC& oDCP05SelectFileController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06SelectFileControllerC& operator=( const DCP06SelectFileControllerC& oDCP05SelectFileController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
            DCP06ModelC* m_pDCP06Model;
            DCP06SelectFileDlgC* m_pDlg;
			short m_iFileType;
		public:
			short GetFileType() const { return m_iFileType; }
    };


    class DCP06SelectFileDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC
	{
	 public:
			/*
			enum CW_ColumnWidth
			{
				CW_File = GUI::ListConfigColsC::CW_16,
				CW_Date = GUI::ListConfigColsC::CW_10
			};
			*/
			enum CI_ColumnId
			{
				CI_File = 0,
				CI_Points,
				CI_Date
			};

		   DCP06SelectFileDlgC(short iFileType,DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06SelectFileDlgC();

			virtual void OnInitDialog(void);

			virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06SelectFileModelC* GetDataModel() const;


	protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;
	private:
			short m_iFileType;
			DCP06ModelC* m_pDCP06Model;
	};

    class DCP06SelectFileModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06SelectFileModelC();

            // Description: Destructor
            //
            virtual ~DCP06SelectFileModelC();
						
			StringC m_strSelectedFile;
    };

};

#endif // DCP_BASEGUI_HPP




