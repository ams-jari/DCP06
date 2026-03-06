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

#ifndef DCP_MSGBOX_HPP
#define DCP_MSGBOX_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class Model;
	//class ToolDialog;


    // Description: Tabbed controller for the Hello World application
    class MsgBox
    {
        public:

            // Description: Constructor
            MsgBox();
			~MsgBox();
			
			bool ShowMessageYesNo(StringC strMessage, StringC Title=L"");	
			void ShowMessageOk(StringC strMessage, StringC Title=L"");	
			bool ShowMessageOkAbort(StringC strMessage, StringC Title=L"");	
			short ShowMessageOkAbortRetry(StringC strMessage);
			short DeleteChstMessage(StringC strMessage);
			short DeletePointsMeas(StringC strMessage);

	private:
    };
};

#endif // DCP_BASEGUI_HPP




