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

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <dcp06/orientation/DCP_DCP05Pom.hpp>

#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/file/DCP_File.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05ChangeStationDlgC;
	//class DCP05ChStModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05ChangeStationControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05ChangeStationControllerC(DCP05ModelC* pDCP05Model);
			~DCP05ChangeStationControllerC();

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
            DCP05ChangeStationControllerC( const DCP05ChangeStationControllerC& oDCP05ChangeStationController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05ChangeStationControllerC& operator=( const DCP05ChangeStationControllerC& oDCP05ChangeStationController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			DCP05PomModelC* m_pDataModel;

            DCP05ChangeStationDlgC* m_pDlg;
			bool check_values(int& min, int& max);
			DCP05MsgBoxC* m_pMsgBox;

			void update_dialog();
			//void DCP::DCP05ChangeStationControllerC::check_function_keys();
		
			//AdfFileFunc* adf;
			DCP05CommonC* m_pCommon;
			DCP05FileModelC* m_pFileModel;
			DCP05ModelC* m_pDCP05Model;
    };
   

    class DCP05ChangeStationDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
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

			DCP05ChangeStationDlgC(DCP05PomModelC* pModel,DCP05ChangeStationControllerC* pController);

            // Description: Destructor
            virtual ~DCP05ChangeStationDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;

			virtual void RefreshControls();
			virtual bool delete_chst();
			void update_ms_adf();
			short LAST_SEL;
			DCP05CommonC* m_pCommon;

			bool get_horizontal_plane_in_use();
			short get_active_plane();
			short get_min_point_count();

	protected:
			//DCP05ChangeStationDlgC* m_pDCP05ChStDlg;
			DCP05PomModelC* m_pDataModel;
			GUI::ComboLineCtrlC* m_pCurrentStation;
			GUI::TextCtrlC* m_pInfo1;
			//GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPoints;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::TextCtrlC* m_pRemeasurePos2;
			GUI::TextCtrlC* m_pCreateNewStation;
			GUI::ComboLineCtrlC* m_pHorizPlane;

			OBS_DECLARE_EXECUTE(DCP05ChangeStationDlgC);

			//OBS_DECLARE_EXECUTE(DCP05DomDlgC);

			// Description: add all controls
            

	private:
			//DCP05CommonC* m_pCommon;
			OBS::ObserverC m_pHorizPlaneObserver;
			virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			bool disable_hz_plane;
			bool use_hz_plane;
			short active_plane;
			short old_active_plane;

			DCP05ChangeStationControllerC* m_pController;

			void check_hz_plane_status();
			void update_hz_plane_control();

	};
	
};

#endif // DCP_BASEGUI_HPP




