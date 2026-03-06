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

#ifndef DCP_LINESETTINGCONFDLG_HPP
#define DCP_LINESETTINGCONGDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class LineSettingConfDialog;
	class LineSettingConfModel;


    // Description: Tabbed controller for the Hello World application
    class LineSettingConfController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            LineSettingConfController(Model *pModel);
			~LineSettingConfController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
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
            LineSettingConfController( const LineSettingConfController& oLineSettingConfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            LineSettingConfController& operator=( const LineSettingConfController& oLineSettingConfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            LineSettingConfDialog* m_pDlg;
			Model *m_pModel;
    };


	class LineSettingConfDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eInfo=1,
				eHor,
				eVer
			};


		   LineSettingConfDialog(Model *pModel);

            // Description: Destructor
            virtual ~LineSettingConfDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			LineSettingConfModel* GetDataModel() const;

	protected:
  			GUI::TextCtrlC* m_pInfo; 
			GUI::ComboLineCtrlC* m_pHor;
			GUI::ComboLineCtrlC* m_pVer;

		private:	
			Model *m_pModel;
	};

	 class LineSettingConfModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            LineSettingConfModel();

            // Description: Destructor
            //
            virtual ~LineSettingConfModel();


			double dHor;
			double dVer;
    };

};

#endif // DCP_BASEGUI_HPP




