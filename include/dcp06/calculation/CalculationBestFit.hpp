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

#ifndef DCP_CALCPOM_HPP
#define DCP_CALCPOM_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/orientation/BestFit.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CalcPomC
    {
        public:

            // Description: Constructor
			DCP06CalcPomC(DCP06PomModelC* pDCP06PomModel, DCP06ModelC* pDCP06Model);//, DCP::DCP06ModelC* pDCP06Model);
			~DCP06CalcPomC();
			
			//bool calc();
			short defined_pom_points(/*point_buff_ *point_OCS,*/ short *lastpoint);
			short get_last_defined_point(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/);
			short get_OCS_points_count(/*point_buff_ *point_OCS*/);
			short get_OCS_SCS_points_count(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/);
			short get_min_points_count();
			short get_rotation_status();
			short direction_of_point(short plane,double a[4], double b[4], double (*unit)[4]);
			short calc_transform();
			short calculate_3points_levelled();
			short calculate_2points_levelled();
			short calculate_3points_not_levelled();

	private:
			DCP06PomModelC *m_pDCP06PomModel;
			DCP06ModelC *m_pDCP06Model;
    };
};

#endif // DCP_BASEGUI_HPP




