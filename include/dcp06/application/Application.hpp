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
    class Model;

    // Description: Hello World application class
    //              
    class Application : public GUI::ApplicationC, public GUI::ModelHandlerC
    {
        public:

            // Description: Constructor
            //
            Application();

            // Description: Destructor
            //
            virtual ~Application();

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
			 Model     *m_pModel;
			 ConfigController* poConfigController;
			
			StringC get_code(char *code);

			char m_pCode[21];
			//void get_code_demo(int demoNumber, char *code);
			//bool check_demo_mode(char* code);

    };

	//class MenuDialog:public GUI::MenuDialogC ,public GUI::ModelHandlerC
	class MenuDialog:public GUI::GraphMenuDialogC ,public GUI::ModelHandlerC
	{
		 public:

		   MenuDialog();

            // Description: Destructor
            virtual ~MenuDialog();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
						
			// Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			 // Description: Hello World model
            DCP::Model* GetModel() const;

	};
	

    class BaseDialog;

    // Description: Tabbed controller for the Hello World application
    class Controller : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            Controller(char* code);
			virtual ~Controller();
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
            Controller( const Controller& oController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            Controller& operator=( const Controller& oController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            MenuDialog* m_pMenuDlg;
			char* m_pCode;
    };


	class OrientationMenuDialog:public GUI::GraphMenuDialogC
	{
		 public:

		   OrientationMenuDialog(Model* pModel);

            // Description: Destructor
            virtual ~OrientationMenuDialog();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
		private:
			Model* m_pModel;
	};

	class FileMenuDialog:public GUI::GraphMenuDialogC
	{
		 public:

		   FileMenuDialog();

            // Description: Destructor
            virtual ~FileMenuDialog();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
	};


	class InfoDialog:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			InfoDialog(Model* pModel);
			~InfoDialog();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF5Pressed();
			virtual void RefreshControls();
	protected:
			GUI::TextCtrlC* m_pText;
		
	private:
			Model* m_pModel;
		
	};

	class LicenseDialog:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			LicenseDialog(Model* pModel);
			~LicenseDialog();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF1Pressed();
			virtual void OnF6Pressed();
			virtual void RefreshControls();
	protected:
			GUI::ComboLineCtrlC* m_pText;
		
	private:
			Model* m_pModel;
		
	};
};

#endif // DCP_APPLICATION_HPP



