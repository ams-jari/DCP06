#ifndef DCP_CS35_HPP
#define DCP_CS35_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================
//#include "dcp06/core/DCP_Defs.hpp"
// CS35
//#if defined(HW_X86)// && defined(CS35)
#if defined(CS35)
#include <comdef.h>
#include <Wbemidl.h>
#include <WbemCli.h>
#include <atlcomcli.h>

#pragma comment(lib, "wbemuuid.lib")

#include <BSS_UTI_String.hpp>
#include "BSS_UTI_UnicodeStringTools.hpp"

//#include "ABL_MsgDef.hpp"
#endif
// end CS35

// Description: The Hello World application
//
namespace DCP
{

    // Description: Tabbed controller for the Hello World application
    class DCP05CS35C
    {
        public:
		   DCP05CS35C();	
		   ~DCP05CS35C();
           void get_serialnumber(char* serialNumber);
    };
};

#endif // DCP_BASEGUI_HPP




