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

#ifndef DCP_CALC_CHST_HPP
#define DCP_CALC_CHST_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/orientation/DCP_DCP05Pom.hpp>
#include <dcp06/orientation/DCP_DCP05ChSt.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CalcChStC
    {
        public:

            // Description: Constructor
			DCP05CalcChStC(DCP05PomModelC* pDCP05PomModel, DCP05ModelC* pDCP05Model, short active_plane_);//, DCP::DCP05ModelC* pDCP05Model);
			~DCP05CalcChStC();
			
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
		DCP05PomModelC *m_pDCP05PomModel;
		DCP05ModelC *m_pDCP05Model;

		short active_plane;
    };

	
};

#endif // DCP_BASEGUI_HPP




