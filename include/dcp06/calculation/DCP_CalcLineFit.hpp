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

#ifndef DCP_CALCLINEFIT_HPP
#define DCP_CALCLINEFIT_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GUI_AppBase.hpp>

#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/orientation/DCP_DCP05Dom.hpp>
#include <dcp06/core/DCP_Model.hpp>
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CalcLineFitC
    {
        public:

            // Description: Constructor
			DCP05CalcLineFitC(DCP05ModelC* pDCP05Model);//S_LINE_BUFF* line, S_POINT_BUFF* points, S_LINE_FITTING_RESULTS* results,  DCP::DCP05ModelC* pDCP05Model);//, DCP::DCP05ModelC* pDCP05Model);
			~DCP05CalcLineFitC();
			
			//bool calc();
			short CalcAllPoints( S_LINE_BUFF* line_buff,
								 S_POINT_BUFF* points,	
								 S_LINE_FITTING_RESULTS* results,
 								 double manualHeight,
     							double shiftValue,
								double rotateAngle,
								int selectedHeight,
								int selectedShift,
								int selectedRotate,
								DCP05DomModelC* domModel,
								S_LINE_BUFF* line_ocs,
								S_POINT_BUFF* pointsInLine,
								int selectedRefLine);

			short CalcPoint(short index, S_LINE_BUFF* line_buff,
								 S_POINT_BUFF* points,	
								 S_LINE_FITTING_RESULTS* results,
 								 double manualHeight,
     							double shiftValue,
								double rotateAngle,
								int selectedHeight,
								int selectedShift,
								int selectedRotate,DCP05DomModelC* domModel,
								S_LINE_BUFF* line_ocs,
								S_POINT_BUFF* pointsInLine,int selectedRefLine);//(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/);

			short CalcLineFitDom(DCP05DomModelC* domModel, S_LINE_BUFF* line_buff, int refLine);
			void delete_dom_values(DCP05DomModelC* domModel);
	private:
		DCP05CommonC* common;
		short set_horizontal_plane(DCP05DomModelC* domModel);
		short set_hz_plane1(DCP05DomModelC* domModel,short actualdesign);
		short calc_plane(S_PLANE_BUFF *plane, short actdes);
		
		short convert_point_to_ocs(S_POINT_BUFF* points, short index, DCP05DomModelC* domModel, double (*p_out)[4]);
		short convert_point_to_ocs(double p_in[4], DCP05DomModelC* domModel, double (*p_out)[4]);
		short convert_point_to_scs(double p_in[4], DCP05DomModelC* domModel, double (*p_out)[4]);
	//	//DCP::DCP05PomModelC *m_pDCP05PomModel;
		DCP05ModelC* m_pDCP05Model;
    };
};

#endif // DCP_BASEGUI_HPP




