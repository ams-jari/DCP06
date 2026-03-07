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

#ifndef DCP_CALCBESTFIT_HPP
#define DCP_CALCBESTFIT_HPP

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
	class Model;
	//class ToolDialog;


    // Description: Tabbed controller for the Hello World application
    class CalcBestFit
    {
        public:

            // Description: Constructor
			CalcBestFit(BestFitModel* pBestFitModel, Model* pModel);//, DCP::Model* pModel);
			~CalcBestFit();
			
			//bool calc();
			short defined_bestFit_points(/*point_buff_ *point_OCS,*/ short *lastpoint);
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
			BestFitModel *m_pBestFitModel;
			Model *m_pModel;
    };
};

#endif // DCP_BASEGUI_HPP




