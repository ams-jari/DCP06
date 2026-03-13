// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_MEASV_DLG_HPP
#define DCP_MEASV_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================
#include <dcp06/orientation/Offsv.hpp>
#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>


#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp> 
//#include <GUI_Timer.hpp>
#include <GUI_AppBase.hpp>

#include <cfa_Types.hpp>
#include <GSV_GeospatialViewDialog.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class MeasVDialog;


    // Description: Tabbed controller for the Hello World application
    class MeasVController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            MeasVController(Model* pModel);
			~MeasVController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF3Pressed();
			virtual void OnSHF4Pressed();
			virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			void show_function_keys();
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            MeasVController( const MeasVController& oMeasVController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            MeasVController& operator=( const MeasVController& oMeasVController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            MeasVDialog* m_pDlg;
			Model* m_pModel;
			Common* m_pCommon;

			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;
			bool isATR;
    };


		class MeasVDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				ePointId=1,
				eX,
				eY,
				eZ,
				eInfo
			};

		   MeasVDialog(Model* pModel);

            // Description: Destructor
            virtual ~MeasVDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			OffsetVModel* GetDataModel() const;

			virtual void delete_point();
			virtual void set_values(double x, double y, double z);

	protected:
  			
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			//GUI::TextCtrlC* m_pInfo;

			Model* m_pModel;

		OBS_DECLARE_EXECUTE(MeasVDialog);
	private:
		//virtual void OnTimer(void);
		//GUI::TimerC m_pTimer;
		Common* m_pCommon;
		short iInfoInd;
		StringC strInfoText;
		OBS::ObserverC m_pPointIdObserver;
		virtual void OnPointIdChanged(int unNotifyCode, int ulParam2);
	};
};

#endif // DCP_BASEGUI_HPP




