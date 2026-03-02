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
	class DCP05ModelC;
	class DCP05InputTextDlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP05InputTextControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
			DCP05InputTextControllerC(DCP05ModelC* pDCP05Model);
			~DCP05InputTextControllerC();

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
            DCP05InputTextControllerC( const DCP05InputTextControllerC& oDCP05InputTextController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05InputTextControllerC& operator=( const DCP05InputTextControllerC& oDCP05InputTextController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05InputTextDlgC* m_pDlg;
    };


	class DCP05InputTextDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				
				eInfo=1,
				eText
			};

		   DCP05InputTextDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05InputTextDlgC();

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
			DCP05InputTextModelC* GetDataModel() const;
			
	protected:
  			
			GUI::TextCtrlC* m_pInfo;
			GUI::EditStringCtrlC* m_pText;
			OBS_DECLARE_EXECUTE(DCP05InputTextDlgC);

	private:
		OBS::ObserverC m_pObserver;
		virtual void OnValueChanged( int unNotifyCode, int ulParam2);
		DCP05ModelC* m_pDCP05Model;
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




