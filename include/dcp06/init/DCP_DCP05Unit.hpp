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

#ifndef DCP_UNITDLG_HPP
#define DCP_UNITDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>

#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;

	class DCP05UnitDlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP05UnitControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05UnitControllerC();
			~DCP05UnitControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05UnitControllerC( const DCP05UnitControllerC& oDCP05UnitController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05UnitControllerC& operator=( const DCP05UnitControllerC& oDCP05UnitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }


            DCP05UnitDlgC* m_pDlg;
    };


    class DCP05UnitDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eUnit=1,
				eDecimals
			};

		   DCP05UnitDlgC();

            // Description: Destructor
            virtual ~DCP05UnitDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;


	protected:
			
			GUI::ComboLineCtrlC* m_pUnit;
			GUI::ComboLineCtrlC* m_pDecimals;
			OBS_DECLARE_EXECUTE(DCP05UnitDlgC);

			// Description: add all controls
            virtual void RefreshControls();

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged( int unNotifyCode,  int ulParam2);
	};
};

#endif // DCP_BASEGUI_HPP




