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

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/orientation/Alignment321.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CalcDomC
    {
        public:

            // Description: Constructor
			DCP06CalcDomC(DCP06DomModelC* pDCP06DomModel);//, DCP::DCP06ModelC* pDCP06Model);
			~DCP06CalcDomC();
			
			bool calc(bool show_message = true);

	private:
		DCP06DomModelC *m_pDCP06DomModel;
		DCP06ModelC *m_pDCP06Model;
    };
};

#endif // DCP_BASEGUI_HPP




