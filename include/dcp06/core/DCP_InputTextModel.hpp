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
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_INPUTTEXTMODEL_HPP      
#define DCP_INPUTTEXTMODEL_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Description: Hello World application class
    //              
    class DCP05InputTextModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05InputTextModelC();

            // Description: Destructor
            //
            virtual ~DCP05InputTextModelC();
			
			StringC m_StrTitle;
			StringC m_StrInfoText;
			StringC m_StrText;
			short m_iTextLength;

    };
};

#endif // DCP_MODEL_HPP




