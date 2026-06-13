// DCP06 - Onboard 3D measurement for Leica total stations
// Entry point DllMain (Windows only; POSIX plugins use Start15751 from Application.cpp)
//

#include "stdafx.h"

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
{
    return TRUE;
}
#endif
