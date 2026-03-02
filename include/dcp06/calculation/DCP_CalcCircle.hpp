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

#ifndef DCP_CALCCIRCLE_HPP
#define DCP_CALCCIRCLE_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <GUI_AppBase.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Common.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CalcCircleC
    {
        public:

            // Description: Constructor
			DCP06CalcCircleC(short plane_type,S_CIRCLE_BUFF *circle_points, S_PLANE_BUFF* planes,S_CIRCLE_BUFF *circle_points_in_plane, double dToolRadius,DCP06ModelC* pDCP06Model);
			~DCP06CalcCircleC();

			short calc_center_of_circle(/*S_CIRCLE_BUFF *circle_points, S_CIRCLE_BUFF *circle_points_in_plane*/);
						
			//short show_circle_points();
			double diameter, cx, cy, cz, vi, vj, vk;
			void get_results(double* dCx, double* dCy, double* dCz, double* dVi, double* dVj, double* dVk, double* dDiameter, double* rms, short* iPno);
			double get_max_dist_and_rms_circle(S_CIRCLE_BUFF *circle_, short *pno, double *rms_diameter);

	private:
			DCP06CommonC* m_pCommon;
			DCP06MsgBoxC* m_pMsgBox;
			short m_iPlaneType;	//XY, YZ, ZX, MEAS or using circle points
			S_CIRCLE_BUFF* m_pCircle_points;
			S_CIRCLE_BUFF* m_pCircle_points_in_plane;
			S_PLANE_BUFF* m_pPlanes;
			double m_dToolRadius;
			double ccs_matrix[4][4];
			double ccs_inv_matrix[4][4];
			S_PLANE_BUFF temp_plane_table[1];

			short convert_points_to_plane(S_CIRCLE_BUFF *circle_points, S_CIRCLE_BUFF *circle_points_in_plane, short count);
			short calc_matrix(S_POINT_BUFF *points);
			short calc_circle(S_CIRCLE_BUFF *points, int count, double *cxtod,double *cytod, double *diameter);
			// more than 3 points
			short pns_ymp(S_CIRCLE_BUFF *points, int count, double *cxtod,double *cytod, double *diameter);

    };
};

#endif // DCP_BASEGUI_HPP




