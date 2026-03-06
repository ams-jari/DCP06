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

#ifndef DCP_POINT_BUFF_MODEL_HPP
#define DCP_POINT_BUFF_MODEL_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

#include <TBL_Measurement.hpp>

namespace ABL
{
    // Description: Forward declaration
    class SurveyModelC;
}

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	/*
	class DCP06ModelC;
	//class DCP06MeasDlgC;
	class DCP06MeasXYZModelC;
	class DCPSurveyModelC;
	*/
   
	class DCP06PointBuffModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06PointBuffModelC();

            // Description: Destructor
            //
            virtual ~DCP06PointBuffModelC();
						
			S_POINT_BUFF m_pPointBuff[1];
    };
	
};

#endif // DCP_BASEGUI_HPP




