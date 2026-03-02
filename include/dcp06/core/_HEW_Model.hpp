// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: Base control of the GUI (graphical user interface)
//
// $Workfile: HEW_Model.hpp $
//
// Summary  : The base control manages GUI control properties which are common for all controls 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright by Leica Geosystems AG, Heerbrugg 2002
//
// ================================================================================================


// $Author: Hlar $
// $Date: 9.06.04 14:37 $
// $Revision: 1 $
// $Modtime: 26.05.04 15:38 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef HEW_MODEL_HPP      
#define HEW_MODEL_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <HEW_Types.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_GModel.hpp>


// Description: The Hello World application
//
namespace HEW
{
    // ========================================================================================================
    // ==========================================  HelloWorldModelC  ==========================================
    // ========================================================================================================

    class HelloWorldModelC : public GUI::GModelC
    {
        public:
            
            // Description: ID of model items
            enum ModelItemsT
            {
                MI_CHECKBOX = 1,
                MI_STRING1,
                MI_STRING2
            };

            // Description: Constructor
            //
            HelloWorldModelC();

            // Description: Destructor
            //
            virtual ~HelloWorldModelC();
    };
};

#endif // HEW_MODEL_HPP
