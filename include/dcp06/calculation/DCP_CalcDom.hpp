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

#ifndef DCP_CALCDOM_HPP
#define DCP_CALCDOM_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/orientation/DCP_DCP05Dom.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CalcDomC
    {
        public:

            // Description: Constructor
			DCP05CalcDomC(DCP05DomModelC* pDCP05DomModel);//, DCP::DCP05ModelC* pDCP05Model);
			~DCP05CalcDomC();
			
			bool calc(bool show_message = true);

	private:
		DCP05DomModelC *m_pDCP05DomModel;
		DCP05ModelC *m_pDCP05Model;
    };
};

#endif // DCP_BASEGUI_HPP




