// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 9.06.04 14:37 $
// $Revision: 1 $
// $Modtime: 26.05.04 15:40 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef HEW_BASEGUI_HPP
#define HEW_BASEGUI_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <HEW_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_GController.hpp>

// Description: The Hello World application
//
namespace HEW
{
    // Forward declaration
    class HelloWorldModelC;
    class HelloWorldDlgC;

    // ========================================================================================================
    // ==========================================  HelloWorldControllerC  =====================================
    // ========================================================================================================

    class HelloWorldControllerC : public GUI::GControllerC
    {
        public:

            enum DialogsT
            {
                DLG_MAIN = 1
            };

            // Description: Constructor
            //
            HelloWorldControllerC(HelloWorldModelC* poHelloWorldModel);

            // Description: Handle change of position values
            //
            virtual void OnF1Pressed();

            // Updates the state of the controls
            //
            void UpdateControlStates();


            // Description: Called when user has edited a control
            //
            // Input      : unNotifyCode    - notification code
            //              ulCtrlId        - control ID
            //
            virtual void OnUserEvent(int unNotifyCode, int ulCtrlId);


        private:

            // Description: Copy constructor
            //
            // Remarks    : not implemented
            //
            HelloWorldControllerC( const HelloWorldControllerC& oHelloWorldController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            //
            // Remarks    : not implemented
            //
            HelloWorldControllerC& operator=( const HelloWorldControllerC& oHelloWorldController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            HelloWorldModelC* m_poHelloWorldModel;
    };


    // ========================================================================================================
    // ==========================================  HelloWorldDialogC  =========================================
    // ========================================================================================================

    class HelloWorldDlgC : public GUI::StandardDialogC
    {
        public:

            // Description: Constructor
            //
            HelloWorldDlgC();
    };
};

#endif // HEW_BASEGUI_HPP




