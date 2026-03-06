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

#ifndef DCP_CHANGE_FACE_HPP
#define DCP_CHANGE_FACE_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_AppBase.hpp>
#include <OBS_Observer.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class Model;
	//class ToolDialog;


    // Description: Tabbed controller for the Hello World application
	class ChangeFace: public TBL::ChangeFaceControllerC
    {
        public:

            // Description: Constructor
            ChangeFace(int iCtrlId);
			~ChangeFace();

	private:
    };
};

#endif // DCP_BASEGUI_HPP




