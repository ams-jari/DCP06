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

#ifndef DCP_INITDLG_HPP
#define DCP_INITDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================
//#include <GUI_Controller.hpp>
#include <GUI_Application.hpp>

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>

#include <GUI_AppBase.hpp>
//#include <GUI_GController.hpp>
//#include <OBS_Observer.hpp>
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
    class Model;

	class InitializationDialog;

    
    // Description: Hello World dialog
	class InitializationDialog : /*public OBS::CommandC,*/ public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
        public:

            enum eCtrlId
            {
                eCombo = 1,
				eCombo2,
				eComboBox,
				eTarget,
				eUser,
				eUnit,
				eAtmCorr,
				e2Face,
				eDesignValues,
				eOverWrite,
				eToolInfo,
				eAutoIncrement,
				eAverageCount,
				eLeftRightHand,
				eAutoMatch,
				eAmsLog,
				eSaveTool,
				eFileStorage

				
            };

            // Description: Constructor
            InitializationDialog(bool disableFileStorage = true);

            // Description: Called when dialog has to initialize, called once in lifetime only
            virtual void OnInitDialog();

            // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: update the Hello World model with the new values
            virtual void UpdateData();

            // Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();
			
        protected:

            // Description: add all controls
			/*
            GUI::ComboLineCtrlC* m_pCombo;
			GUI::ComboLineCtrlC* m_pCombo2;
			GUI::ComboLineCtrlC* m_pComboBox;
			*/
			//GUI::TextCtrlC* m_pTextTarget;
			GUI::ComboLineCtrlC* m_pTextTarget;
			GUI::ComboLineCtrlC* m_pUser;
			GUI::ComboLineCtrlC* m_pUnit;
			GUI::ComboLineCtrlC* m_pAtmCorr;
			GUI::ComboLineCtrlC* m_p2Face;
			GUI::ComboLineCtrlC* m_pDesignValues;
			GUI::ComboLineCtrlC* m_pOverwriteInfo;
			GUI::ComboLineCtrlC* m_ToolInfo;
			GUI::ComboLineCtrlC* m_pAutoIncrement;
			GUI::ComboLineCtrlC* m_pAverageCount;
			GUI::ComboLineCtrlC* m_pLeftRightHand;
			GUI::ComboLineCtrlC* m_pAutoMatch;
			GUI::ComboLineCtrlC* m_pAmsLog;
			GUI::ComboLineCtrlC* m_pSaveTool;
			GUI::ComboLineCtrlC* m_pFileStorage;


			//OBS_DECLARE_EXECUTE(InitializationDialog);

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_sMMText;
			StringC m_sINCHText;
			StringC m_sFEETText;
			bool m_pDisableFileStorage;

    };
// Description: Tabbed controller for the Hello World application
    class InitializationController : public GUI::ControllerC , public OBS::CommandC
    {
        public:

            // Description: Constructor
            InitializationController(bool disableFileStorage = true);
			~InitializationController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
			//InitializationController( const InitializationController& oInitController )
   //         {
   //             USER_APP_VERIFY( false );
   //         }

            // Description: Assignment operator
            // Remarks    : not implemented
            InitializationController& operator=( const InitializationController& oInitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            InitializationDialog* m_pDlg;

			OBS_DECLARE_EXECUTE(InitializationController);

	private:
			OBS::ObserverC m_pOnApplicationClosedObserver;
			virtual void OnApplicationClosed( int unNewApp, int ulAppOwner);
    };


};

#endif // DCP_BASEGUI_HPP




