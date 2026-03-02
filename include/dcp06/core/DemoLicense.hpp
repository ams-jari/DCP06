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

#ifndef DCP_DEMOLICENSE_HPP
#define DCP_DEMOLICENSE_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================


#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
//#include <GUI_AppBase.hpp>



// Description: The Hello World application
//
namespace DCP
{

    // Description: Tabbed controller for the Hello World application
    class DCP05DemoLicenseC
    {
        public:

            // Description: Constructor
            DCP05DemoLicenseC(DCP05ModelC* pDCP05Model);
			// DCP05CommonC();
			~DCP05DemoLicenseC();
			  
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
			DCP05ModelC *m_pDCP05Model;
    };
};

#endif // DCP_BASEGUI_HPP




