// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: GUI.hpp $
//
// Summary  : DCP06 base dialog and GUI components
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

#ifndef DCP_BASEGUI_HPP
#define DCP_BASEGUI_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_AppBase.hpp>

// Description: DCP06 base dialog and GUI components
//
namespace DCP
{
    // Forward declaration
    class Model;

    // Description: DCP06 base dialog with combo controls
	class BaseDialog : public OBS::CommandC, public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
        public:

            enum eCtrlId
            {
                eCombo = 1,
				eCombo2,
				eComboBox
				
            };

            // Description: Constructor
            BaseDialog();

            // Description: Called when dialog has to initialize, called once in lifetime only
            virtual void OnInitDialog();

            // Description: Accept only DCP06 Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: Update the model with current control values
            virtual void UpdateData();

            // Description: Return the DCP06 Model instance
            Model* GetModel() const;

        protected:

            // Description: add all controls
            virtual void RefreshControls();

            GUI::ComboLineCtrlC* m_pCombo;
			GUI::ComboLineCtrlC* m_pCombo2;
			GUI::ComboLineCtrlC* m_pComboBox;

			OBS_DECLARE_EXECUTE(BaseDialog);

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged( int unNotifyCode, int ulParam2);
    };
};

#endif // DCP_BASEGUI_HPP




