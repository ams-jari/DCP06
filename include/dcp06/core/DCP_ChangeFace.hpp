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

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_AppBase.hpp>
#include <OBS_Observer.hpp>
#include <BSS_TSK_ApiComponent.hpp>
#include <TBL_PositTelescope.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
	class DCP05ChangeFaceC: public TBL::ChangeFaceControllerC
    {
        public:

            // Description: Constructor
            DCP05ChangeFaceC(int iCtrlId);
			~DCP05ChangeFaceC();

	private:
    };
};

#endif // DCP_BASEGUI_HPP




