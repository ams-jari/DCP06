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

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_AppBase.hpp>

#include <dcp06/core/Common.hpp>
#include <dcp06/orientation/Alignment321.hpp>
#include <dcp06/core/Model.hpp>
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	//class ToolDialog;


    // Description: Tabbed controller for the Hello World application
    class CalcLineFit
    {
        public:

            // Description: Constructor
			CalcLineFit(Model* pModel);//S_LINE_BUFF* line, S_POINT_BUFF* points, S_LINE_FITTING_RESULTS* results,  DCP::Model* pModel);//, DCP::Model* pModel);
			~CalcLineFit();
			
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
								Alignment321Model* domModel,
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
								int selectedRotate,Alignment321Model* domModel,
								S_LINE_BUFF* line_ocs,
								S_POINT_BUFF* pointsInLine,int selectedRefLine);//(/*point_buff_ *point_OCS,point_buff_ *point_DCS*/);

			short CalcLineFitDom(Alignment321Model* domModel, S_LINE_BUFF* line_buff, int refLine);
			void delete_dom_values(Alignment321Model* domModel);
	private:
		Common* common;
		short set_horizontal_plane(Alignment321Model* domModel);
		short set_hz_plane1(Alignment321Model* domModel,short actualdesign);
		short calc_plane(S_PLANE_BUFF *plane, short actdes);
		
		short convert_point_to_ocs(S_POINT_BUFF* points, short index, Alignment321Model* domModel, double (*p_out)[4]);
		short convert_point_to_ocs(double p_in[4], Alignment321Model* domModel, double (*p_out)[4]);
		short convert_point_to_scs(double p_in[4], Alignment321Model* domModel, double (*p_out)[4]);
	//	//DCP::BestFitModel *m_pBestFitModel;
		Model* m_pModel;
    };
};

#endif // DCP_BASEGUI_HPP




