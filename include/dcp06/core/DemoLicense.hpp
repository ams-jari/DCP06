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

#ifndef DCP_DEMOLICENSE_HPP
#define DCP_DEMOLICENSE_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================


#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
//#include <GUI_AppBase.hpp>



// Description: The Hello World application
//
namespace DCP
{

    // Description: Tabbed controller for the Hello World application
    class DemoLicense
    {
        public:

            // Description: Constructor
            DemoLicense(Model* pModel);
			// Common();
			~DemoLicense();
			  
			void get_code_demo(int demoNumber, char *sCode);
			bool is_license_ok(char* keycode, DateTime startDate);
			int get_available_days(char* keycode, DateTime startDate);
			int find_keycode(char* keycode);


	/*		1. license 1-30
			2. license 31-60
			3. license 61-90
			4. license 121-150
			5. license 151-180*/
	private:
			Model *m_pModel;
    };
};

#endif // DCP_BASEGUI_HPP




