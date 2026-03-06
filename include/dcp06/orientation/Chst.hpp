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
    class DCP06ModelC;
	class DCP06ChangeStationDlgC;
	//class DCP06ChStModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06ChangeStationControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ChangeStationControllerC(DCP06ModelC* pDCP06Model);
			~DCP06ChangeStationControllerC();

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
            DCP06ChangeStationControllerC( const DCP06ChangeStationControllerC& oDCP06ChangeStationController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ChangeStationControllerC& operator=( const DCP06ChangeStationControllerC& oDCP06ChangeStationController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			DCP06PomModelC* m_pDataModel;

            DCP06ChangeStationDlgC* m_pDlg;
			bool check_values(int& min, int& max);
			DCP06MsgBoxC* m_pMsgBox;

			void update_dialog();
			//void DCP::DCP06ChangeStationControllerC::check_function_keys();
		
			//AdfFileFunc* adf;
			DCP06CommonC* m_pCommon;
			DCP06FileModelC* m_pFileModel;
			DCP06ModelC* m_pDCP06Model;
    };
   

    class DCP06ChangeStationDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
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

			DCP06ChangeStationDlgC(DCP06PomModelC* pModel,DCP06ChangeStationControllerC* pController);

            // Description: Destructor
            virtual ~DCP06ChangeStationDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;

			virtual void RefreshControls();
			virtual bool delete_chst();
			void update_ms_adf();
			short LAST_SEL;
			DCP06CommonC* m_pCommon;

			bool get_horizontal_plane_in_use();
			short get_active_plane();
			short get_min_point_count();

	protected:
			//DCP06ChangeStationDlgC* m_pDCP05ChStDlg;
			DCP06PomModelC* m_pDataModel;
			GUI::ComboLineCtrlC* m_pCurrentStation;
			GUI::TextCtrlC* m_pInfo1;
			//GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPoints;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::TextCtrlC* m_pRemeasurePos2;
			GUI::TextCtrlC* m_pCreateNewStation;
			GUI::ComboLineCtrlC* m_pHorizPlane;

			OBS_DECLARE_EXECUTE(DCP06ChangeStationDlgC);

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			// Description: add all controls
            

	private:
			//DCP06CommonC* m_pCommon;
			OBS::ObserverC m_pHorizPlaneObserver;
			virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			bool disable_hz_plane;
			bool use_hz_plane;
			short active_plane;
			short old_active_plane;

			DCP06ChangeStationControllerC* m_pController;

			void check_hz_plane_status();
			void update_hz_plane_control();

	};
	
};

#endif // DCP_BASEGUI_HPP




