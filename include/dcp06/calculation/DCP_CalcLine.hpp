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

#ifndef DCP_CALCLINE_HPP
#define DCP_CALCLINE_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_AppBase.hpp>
#include <dcp06/core/DCP_Defs.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CalcLineC
    {
        public:

            // Description: Constructor
            DCP05CalcLineC();
			~DCP05CalcLineC();

			short calc(S_LINE_BUFF *line, short actdes);
			short defined_points_count_in_line(S_LINE_BUFF *line,short *lastpoint);
			short points_count_in_line(S_LINE_BUFF *line);

	private:
    };
};

#endif // DCP_BASEGUI_HPP




