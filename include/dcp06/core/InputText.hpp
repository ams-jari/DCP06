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

#include <dcp06/core/Types.hpp>
#include <dcp06/core/InputTextModel.hpp>
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
	class Model;
	class InputTextDialog;

    // Description: Tabbed controller for the Hello World application
    class InputTextController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
			InputTextController(Model* pModel);
			~InputTextController();

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
            InputTextController( const InputTextController& oInputTextController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            InputTextController& operator=( const InputTextController& oInputTextController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            InputTextDialog* m_pDlg;
    };


	class InputTextDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				
				eInfo=1,
				eText
			};

		   InputTextDialog(Model* pModel);

            // Description: Destructor
            virtual ~InputTextDialog();

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
			InputTextModel* GetDataModel() const;
			
	protected:
  			
			GUI::TextCtrlC* m_pInfo;
			GUI::EditStringCtrlC* m_pText;
			OBS_DECLARE_EXECUTE(InputTextDialog);

	private:
		OBS::ObserverC m_pObserver;
		virtual void OnValueChanged( int unNotifyCode, int ulParam2);
		Model* m_pModel;
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




