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

#ifndef DCP_LINESETTINGDLG_HPP
#define DCP_LINESETTINGDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
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
	class Model;
	class LineSettingDialog;
	class LineSettingModel;

    // Description: Tabbed controller for the Hello World application
    class LineSettingController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            LineSettingController();
			~LineSettingController();

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
            LineSettingController( const LineSettingController& oLineSettingController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            LineSettingController& operator=( const LineSettingController& oLineSettingController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            LineSettingDialog* m_pDlg;
			LineSettingModel* m_pDataModel;
    };


	class LineSettingDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eHor=1,
				eVer
			};

			LineSettingDialog(LineSettingModel* pLineSettingModel);

            // Description: Destructor
            virtual ~LineSettingDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			Model* GetModel() const;

			double get_hor_angle();
			double get_ver_angle();

	protected:
  			
			GUI::ComboLineCtrlC* m_pHor;
			GUI::ComboLineCtrlC* m_pVer;
	private:
			//virtual void OnTimer(void);
			//GUI::TimerC m_pTimer;
			LineSettingModel* m_pDataModel;
	};

	class LineSettingModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            LineSettingModel();

            // Description: Destructor
            //
            virtual ~LineSettingModel();

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




