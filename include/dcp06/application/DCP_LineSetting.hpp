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

#ifndef DCP_LINESETTINGDLG_HPP
#define DCP_LINESETTINGDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05LineSettingDlgC;
	class DCP05LSetModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05LineSettingControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05LineSettingControllerC();
			~DCP05LineSettingControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF6Pressed();
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
            DCP05LineSettingControllerC( const DCP05LineSettingControllerC& oDCP05LineSettingController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05LineSettingControllerC& operator=( const DCP05LineSettingControllerC& oDCP05LineSettingController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05LineSettingDlgC* m_pDlg;
			DCP05LSetModelC* m_pDataModel;
    };


	class DCP05LineSettingDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eHor=1,
				eVer
			};

			DCP05LineSettingDlgC(DCP05LSetModelC* pDCP05LSetModel);

            // Description: Destructor
            virtual ~DCP05LineSettingDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05ModelC* GetDCP05Model() const;

			double get_hor_angle();
			double get_ver_angle();

	protected:
  			
			GUI::ComboLineCtrlC* m_pHor;
			GUI::ComboLineCtrlC* m_pVer;
	private:
			//virtual void OnTimer(void);
			//GUI::TimerC m_pTimer;
			DCP05LSetModelC* m_pDataModel;
	};

	class DCP05LSetModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05LSetModelC();

            // Description: Destructor
            //
            virtual ~DCP05LSetModelC();

			double dHor;
			double dVer;
			
			double ang,ang1,ang2,pit,kulma,ang_h,ang_p;
			double ver_temp, theta1, fii1; 
			short left_cds;
			S_LINE_BUFF line_dom[1];
			S_LINE_BUFF line_userdef[1];
			S_LINE_BUFF* line;

    };


};

#endif // DCP_BASEGUI_HPP




