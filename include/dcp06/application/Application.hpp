// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_Application.hpp $
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

#ifndef DCP_APPLICATION_HPP
#define DCP_APPLICATION_HPP

// ================================================================================================
// ===========================================  Includes   ========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <UTL_Macro.hpp>
#include <GUI_Application.hpp>

//#include <GUI_MenuDialog.hpp> NOT CAPTIVTE
#include <GUI_GraphMenuDialog.hpp> 

#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>

//#include <GUI_TextBoxCtrl.hpp> NOT CAPTIVTE

// Description: The Hello World application
//
namespace DCP
{
    class DCP06ModelC;

    // Description: Hello World application class
    //              
    class DCP06ApplicationC : public GUI::ApplicationC, public GUI::ModelHandlerC
    {
        public:

            // Description: Constructor
            //
            DCP06ApplicationC();

            // Description: Destructor
            //
            virtual ~DCP06ApplicationC();

            // Description: Start application
            virtual void Run(/* bool bShowStartDialog */);

            // Description: React on close of tabbed dialog
            virtual void OnActiveControllerClosed( int lDlgID, int lExitCode );
			virtual bool ConfirmClose(bool bEsc);
 
			             
			// Description: called by the framework after the active dialog is closed
            //
            // Input      : lDlgID      - Id of closed dialog
            //              lExitCode   - How was the dialog closed (exit code)
            //
            virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
		private:
			 DCP06ModelC     *m_pDCP06Model;
			 DCP06ConfigControllerC* poConfigController;
			
			StringC get_code(char *code);

			char m_pCode[21];
			//void get_code_demo(int demoNumber, char *code);
			//bool check_demo_mode(char* code);

    };

	//class DCP06MenuDlgC:public GUI::MenuDialogC ,public GUI::ModelHandlerC
	class DCP06MenuDlgC:public GUI::GraphMenuDialogC ,public GUI::ModelHandlerC
	{
		 public:

		   DCP06MenuDlgC();

            // Description: Destructor
            virtual ~DCP06MenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
						
			// Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			 // Description: Hello World model
            DCP::DCP06ModelC* GetDCP06Model() const;

	};
	

    class DCP06DlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP06ControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ControllerC(char* code);
			virtual ~DCP06ControllerC();
            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnSHF2Pressed(void);
			virtual void OnSHF4Pressed(void);

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			//virtual bool ConfirmClose(bool bEsc);

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06ControllerC( const DCP06ControllerC& oDCP06Controller )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ControllerC& operator=( const DCP06ControllerC& oDCP06Controller )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06MenuDlgC* m_pDCP06Dlg;
			char* m_pCode;
    };


	class DCP06OrientationMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP06OrientationMenuDlgC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06OrientationMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
		private:
			DCP06ModelC* m_pDCP06Model;
	};

	class DCP06FileMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP06FileMenuDlgC();

            // Description: Destructor
            virtual ~DCP06FileMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
	};


	class DCP06InfoDlgC:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP06InfoDlgC(DCP06ModelC* pDCP06Model);
			~DCP06InfoDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF5Pressed();
			virtual void RefreshControls();
	protected:
			GUI::TextCtrlC* m_pText;
		
	private:
			DCP06ModelC* m_pDCP06Model;
		
	};

	class DCP06LisenceDlgC:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP06LisenceDlgC(DCP06ModelC* pDCP06Model);
			~DCP06LisenceDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF1Pressed();
			virtual void OnF6Pressed();
			virtual void RefreshControls();
	protected:
			GUI::ComboLineCtrlC* m_pText;
		
	private:
			DCP06ModelC* m_pDCP06Model;
		
	};
};

#endif // DCP_APPLICATION_HPP



