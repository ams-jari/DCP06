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

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
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
    class DCP05ModelC;

    // Description: Hello World application class
    //              
    class DCP05ApplicationC : public GUI::ApplicationC, public GUI::ModelHandlerC
    {
        public:

            // Description: Constructor
            //
            DCP05ApplicationC();

            // Description: Destructor
            //
            virtual ~DCP05ApplicationC();

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
			 DCP05ModelC     *m_pDCP05Model;
			 DCP05ConfigControllerC* poConfigController;
			
			StringC get_code(char *code);

			char m_pCode[21];
			//void get_code_demo(int demoNumber, char *code);
			//bool check_demo_mode(char* code);

    };

	//class DCP05MenuDlgC:public GUI::MenuDialogC ,public GUI::ModelHandlerC
	class DCP05MenuDlgC:public GUI::GraphMenuDialogC ,public GUI::ModelHandlerC
	{
		 public:

		   DCP05MenuDlgC();

            // Description: Destructor
            virtual ~DCP05MenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
						
			// Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			 // Description: Hello World model
            DCP::DCP05ModelC* GetDCP05Model() const;

	};
	

    class DCP05DlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP05ControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05ControllerC(char* code);
			virtual ~DCP05ControllerC();
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
            DCP05ControllerC( const DCP05ControllerC& oDCP05Controller )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05ControllerC& operator=( const DCP05ControllerC& oDCP05Controller )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05MenuDlgC* m_pDCP05Dlg;
			char* m_pCode;
    };


	class DCP05OrientationMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP05OrientationMenuDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05OrientationMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
		private:
			DCP05ModelC* m_pDCP05Model;
	};

	class DCP05FileMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP05FileMenuDlgC();

            // Description: Destructor
            virtual ~DCP05FileMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);

			virtual void OnF1Pressed(void);
	};


	class DCP05InfoDlgC:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP05InfoDlgC(DCP05ModelC* pDCP05Model);
			~DCP05InfoDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF5Pressed();
			virtual void RefreshControls();
	protected:
			GUI::TextCtrlC* m_pText;
		
	private:
			DCP05ModelC* m_pDCP05Model;
		
	};

	class DCP05LisenceDlgC:public GUI::StandardDialogC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP05LisenceDlgC(DCP05ModelC* pDCP05Model);
			~DCP05LisenceDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF1Pressed();
			virtual void OnF6Pressed();
			virtual void RefreshControls();
	protected:
			GUI::ComboLineCtrlC* m_pText;
		
	private:
			DCP05ModelC* m_pDCP05Model;
		
	};
};

#endif // DCP_APPLICATION_HPP



