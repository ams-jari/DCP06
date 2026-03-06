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

#ifndef DCP_UNITDLG_HPP
#define DCP_UNITDLG_HPP

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

	class UnitDialog;

    // Description: Tabbed controller for the Hello World application
    class UnitController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            UnitController();
			~UnitController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            UnitController( const UnitController& oUnitController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            UnitController& operator=( const UnitController& oUnitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }


            UnitDialog* m_pDlg;
    };


    class UnitDialog:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eUnit=1,
				eDecimals
			};

		   UnitDialog();

            // Description: Destructor
            virtual ~UnitDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;


	protected:
			
			GUI::ComboLineCtrlC* m_pUnit;
			GUI::ComboLineCtrlC* m_pDecimals;
			OBS_DECLARE_EXECUTE(UnitDialog);

			// Description: add all controls
            virtual void RefreshControls();

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged( int unNotifyCode,  int ulParam2);
	};
};

#endif // DCP_BASEGUI_HPP




