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
	class DCP06ModelC;
	class DCP06LineSettingDlgC;
	class DCP06LSetModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06LineSettingControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06LineSettingControllerC();
			~DCP06LineSettingControllerC();

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
            DCP06LineSettingControllerC( const DCP06LineSettingControllerC& oDCP06LineSettingController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06LineSettingControllerC& operator=( const DCP06LineSettingControllerC& oDCP06LineSettingController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06LineSettingDlgC* m_pDlg;
			DCP06LSetModelC* m_pDataModel;
    };


	class DCP06LineSettingDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eHor=1,
				eVer
			};

			DCP06LineSettingDlgC(DCP06LSetModelC* pDCP06LSetModel);

            // Description: Destructor
            virtual ~DCP06LineSettingDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06ModelC* GetDCP06Model() const;

			double get_hor_angle();
			double get_ver_angle();

	protected:
  			
			GUI::ComboLineCtrlC* m_pHor;
			GUI::ComboLineCtrlC* m_pVer;
	private:
			//virtual void OnTimer(void);
			//GUI::TimerC m_pTimer;
			DCP06LSetModelC* m_pDataModel;
	};

	class DCP06LSetModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06LSetModelC();

            // Description: Destructor
            //
            virtual ~DCP06LSetModelC();

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




