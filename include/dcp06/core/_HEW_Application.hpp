// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_Application.hpp $
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
// $Modtime: 2.06.04 9:41 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef HEW_APPLICATION_HPP
#define HEW_APPLICATION_HPP

// ================================================================================================
// ===========================================  Includes   ========================================
// ================================================================================================

#include <HEW_Types.hpp>
#include <UTL_Macro.hpp>
#include <GUI_Application.hpp>

// Description: The Hello World application
//
namespace HEW
{
    class HelloWorldModelC;

    // Description: Hello World application class
    //              
    class HelloWorldApplicationC : public GUI::ApplicationC, public GUI::ModelHandlerC
    {
        public:

            enum ControllerT
            {
                CTL_MAIN = 1
            };

            // Description: Constructor
            //
            HelloWorldApplicationC();

            // Description: Destructor
            //
            virtual ~HelloWorldApplicationC();

            // Description: Start application
            //
            virtual void Run();

            // Description: React on close of tabbed dialog
            //
            virtual void OnActiveControllerClosed( int lDlgID, int lExitCode );
    };
};

#endif // HEW_APPLICATION_HPP



