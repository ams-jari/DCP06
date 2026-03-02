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

#ifndef DCP_INPUTTEXTDLG_HPP
#define DCP_INPUTTEXTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_InputTextModel.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_KeyTypes.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP06InputTextDlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP06InputTextControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
			DCP06InputTextControllerC(DCP06ModelC* pDCP06Model);
			~DCP06InputTextControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			//virtual void OnSHF5Pressed();
			//virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06InputTextControllerC( const DCP06InputTextControllerC& oDCP06InputTextController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06InputTextControllerC& operator=( const DCP06InputTextControllerC& oDCP06InputTextController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06InputTextDlgC* m_pDlg;
    };


	class DCP06InputTextDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				
				eInfo=1,
				eText
			};

		   DCP06InputTextDlgC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06InputTextDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
			/*virtual void OnDialogClosed(int  iExitCode );*/
			virtual bool OnKeyPress  ( GUI::Key  eKey ) ;
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			/*virtual bool OnKeyPress  (GUI::Key  eKey ); */
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06InputTextModelC* GetDataModel() const;
			
	protected:
  			
			GUI::TextCtrlC* m_pInfo;
			GUI::EditStringCtrlC* m_pText;
			OBS_DECLARE_EXECUTE(DCP06InputTextDlgC);

	private:
		OBS::ObserverC m_pObserver;
		virtual void OnValueChanged( int unNotifyCode, int ulParam2);
		DCP06ModelC* m_pDCP06Model;
	};

	/*
	class InputTextC:GUI::GeneralDialogC, public OBS::CommandC
	{
		public:
			InputTextC();
			~InputTextC();
			void InputStr();
		protected:
			OBS_DECLARE_EXECUTE(InputTextC);
			GUI::EditStringCtrlC* m_pText;
			void OnInputEnterPressed( int unNotifyCode, int ulParam2);
		private:
			OBS::ObserverC m_pObsInput;
			short iClose;

	};
	*/
};

#endif // DCP_BASEGUI_HPP




