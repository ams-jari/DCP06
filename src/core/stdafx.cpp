// stdafx.cpp : source file that includes just the standard includes
// Test.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if defined( WIN32 ) || defined( WIN64 )
#ifndef _WIN32_WCE
#include <crtdbg.h>
#endif
#endif

#ifdef _DEBUG
#if defined( WIN32 ) || defined( WIN64 )
#ifndef _WIN32_WCE
void* __cdecl operator new(size_t nSize, char* lpszFileName, int nLine)
{
	//  return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
	return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
void __cdecl operator delete(void* p, char* lpszFileName, int nLine)
{
	_free_dbg(p, _NORMAL_BLOCK);
}

void* __cdecl operator new[](size_t nSize, char* lpszFileName, int nLine)
	{
		//  return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
		return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
	}
	void __cdecl operator delete[](void* p, char* lpszFileName, int nLine)
		{
			_free_dbg(p, _NORMAL_BLOCK);
		}
#else	// _WIN32_WCE
	// Definitions for our debug zones                               
#define ZONEID_INIT      0

// These masks are useful for initialization of dpCurSettings
#define ZONEMASK_INIT      (1<<ZONEID_INIT) 

DBGPARAM dpCurSettings = {
TEXT("MemLeak"), {
	TEXT("Init"),TEXT("Trace Fn( );"),TEXT("Memory"),TEXT(""),
	TEXT(""),TEXT(""),TEXT(""),TEXT(""),
	TEXT(""),TEXT(""),TEXT(""),TEXT(""),
	TEXT(""),TEXT(""),TEXT(""),TEXT("")},
	// By default, turn on the zones for init and errors.
	ZONEMASK_INIT
};

#endif // _WIN32_WCE
#endif // WIN32
#endif // _DEBUG


