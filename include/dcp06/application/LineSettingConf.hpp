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
	class DCP06ModelC;
	class DCP06LineSettingConfDlgC;
	class DCP06LSetConfModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06LineSettingConfControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06LineSettingConfControllerC(DCP06ModelC *pDCP06Model);
			~DCP06LineSettingConfControllerC();

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
            DCP06LineSettingConfControllerC( const DCP06LineSettingConfControllerC& oDCP05LineSettingConfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06LineSettingConfControllerC& operator=( const DCP06LineSettingConfControllerC& oDCP05LineSettingConfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06LineSettingConfDlgC* m_pDlg;
			DCP06ModelC *m_pDCP06Model;
    };


	class DCP06LineSettingConfDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eInfo=1,
				eHor,
				eVer
			};


		   DCP06LineSettingConfDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06LineSettingConfDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06LSetConfModelC* GetDataModel() const;

	protected:
  			GUI::TextCtrlC* m_pInfo; 
			GUI::ComboLineCtrlC* m_pHor;
			GUI::ComboLineCtrlC* m_pVer;

		private:	
			DCP06ModelC *m_pDCP06Model;
	};

	 class DCP06LSetConfModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06LSetConfModelC();

            // Description: Destructor
            //
            virtual ~DCP06LSetConfModelC();


			double dHor;
			double dVer;
    };

};

#endif // DCP_BASEGUI_HPP




