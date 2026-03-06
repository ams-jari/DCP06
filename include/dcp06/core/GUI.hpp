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

#ifndef DCP_BASEGUI_HPP
#define DCP_BASEGUI_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
    class DCP06ModelC;

    // Description: Hello World dialog
	class DCP06DlgC : public OBS::CommandC, public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
        public:

            enum eCtrlId
            {
                eCombo = 1,
				eCombo2,
				eComboBox
				
            };

            // Description: Constructor
            DCP06DlgC();

            // Description: Called when dialog has to initialize, called once in lifetime only
            virtual void OnInitDialog();

            // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: update the Hello World model with the new values
            virtual void UpdateData();

            // Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;

        protected:

            // Description: add all controls
            virtual void RefreshControls();

            GUI::ComboLineCtrlC* m_pCombo;
			GUI::ComboLineCtrlC* m_pCombo2;
			GUI::ComboLineCtrlC* m_pComboBox;

			OBS_DECLARE_EXECUTE(DCP06DlgC);

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged( int unNotifyCode, int ulParam2);
    };
};

#endif // DCP_BASEGUI_HPP




