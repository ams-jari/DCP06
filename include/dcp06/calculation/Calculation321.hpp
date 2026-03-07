// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
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

#ifndef DCP_CALC321_HPP
#define DCP_CALC321_HPP

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
	//class Model;
	//class ToolDialog;


    // Description: Tabbed controller for the Hello World application
    class CalcAlignment321
    {
        public:

            // Description: Constructor
			CalcAlignment321(Alignment321Model* pAlignment321Model);
			~CalcAlignment321();
			
			bool calc(bool show_message = true);

	private:
		Alignment321Model* m_pAlignment321Model;
		Model *m_pModel;
    };
};

#endif // DCP_BASEGUI_HPP




