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

#ifndef DCP_CHSTDLG_HPP
#define DCP_CHSTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <dcp06/orientation/BestFit.hpp>

#include <dcp06/core/Common.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/file/File.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class ChangeStationDialog;
	//class ChangeStationModel;

    // Description: Tabbed controller for the Hello World application
    class ChangeStationController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            ChangeStationController(Model* pModel);
			~ChangeStationController();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			//virtual void OnF3Pressed(void);
			virtual void OnF4Pressed(void);
			virtual void OnF5Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			virtual void OnSHF3Pressed(void);
			virtual void OnSHF4Pressed(void);
			virtual void OnSHF5Pressed(void);


			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int /*lExitCode*/ );
			virtual void OnControllerActivated(void);

			void check_function_keys();

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            ChangeStationController( const ChangeStationController& oChangeStationController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            ChangeStationController& operator=( const ChangeStationController& oChangeStationController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			BestFitModel* m_pDataModel;

            ChangeStationDialog* m_pDlg;
			bool check_values(int& min, int& max);
			MsgBox* m_pMsgBox;

			void update_dialog();
			//void DCP::ChangeStationController::check_function_keys();
		
			//AdfFileFunc* adf;
			Common* m_pCommon;
			FileModel* m_pFileModel;
			Model* m_pModel;
    };
   

    class ChangeStationDialog:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eCurrentStation=1,
				eInfo1,
				eHorizPlane,
				//eFile, 
				ePoints,
				ePointMeas,
				eRemeasPos2,
				eCreateNewStation

			};

			ChangeStationDialog(BestFitModel* pModel,ChangeStationController* pController);

            // Description: Destructor
            virtual ~ChangeStationDialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();
			virtual bool delete_chst();
			void update_ms_adf();
			short LAST_SEL;
			Common* m_pCommon;

			bool get_horizontal_plane_in_use();
			short get_active_plane();
			short get_min_point_count();

	protected:
			//ChangeStationDialog* m_pChangeStationDlg;
			BestFitModel* m_pDataModel;
			GUI::ComboLineCtrlC* m_pCurrentStation;
			GUI::TextCtrlC* m_pInfo1;
			//GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPoints;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::TextCtrlC* m_pRemeasurePos2;
			GUI::TextCtrlC* m_pCreateNewStation;
			GUI::ComboLineCtrlC* m_pHorizPlane;

			OBS_DECLARE_EXECUTE(ChangeStationDialog);

			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			// Description: add all controls
            

	private:
			//Common* m_pCommon;
			OBS::ObserverC m_pHorizPlaneObserver;
			virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			bool disable_hz_plane;
			bool use_hz_plane;
			short active_plane;
			short old_active_plane;

			ChangeStationController* m_pController;

			void check_hz_plane_status();
			void update_hz_plane_control();

	};
	
};

#endif // DCP_BASEGUI_HPP




