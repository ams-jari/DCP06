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

#ifndef DCP_MSGBOX_HPP
#define DCP_MSGBOX_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_AppBase.hpp>


// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP05ModelC;
	//class DCP05ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05MsgBoxC
    {
        public:

            // Description: Constructor
            DCP05MsgBoxC();
			~DCP05MsgBoxC();
			
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




