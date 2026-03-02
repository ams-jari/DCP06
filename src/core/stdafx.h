// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#ifndef HEW_STDAFX_H
#define HEW_STDAFX_H

#pragma once

#ifdef _WIN32_WCE
	//dynamic_cast' used on polymorphic type
#pragma warning(disable: 4541)
//this' : used in base member initializer list
#pragma warning(disable: 4355)
// C++ exception handler used, but unwind semantics are not enabled. Specify -G
#pragma warning(disable: 4530)
#include <exception>
#endif

#if defined( WIN32 ) || defined( WIN64 )

	// Default include
#include <windows.h>

// using non-exported as public in exported
#pragma warning(disable: 4251)

#ifndef _WIN32_WCE
	// Win32 debug we trace memory leaks
#ifdef _DEBUG
void* __cdecl operator new(size_t nSize, char* lpszFileName, int nLine);
void __cdecl operator delete(void* p, char* lpszFileName, int nLine);

void* __cdecl operator new[](size_t nSize, char* lpszFileName, int nLine);
void __cdecl operator delete[](void* p, char* lpszFileName, int nLine);

#define DEBUG_NEW new( __FILE__ , __LINE__ )
#else
	// In release mode we do not track anything
#define DEBUG_NEW new
#endif
#else
#if (defined(_WIN32_WCE) && !defined(GLOBAL_STD_EXCEPTION_ON))
#define GLOABL_EXCEPTION
using std::exception;  // for the exception class
#endif	
#define DEBUG_NEW new			
#endif
#else

#include <vxworks.h>

// Cross compiler memory leaks tracking not supported
#define DEBUG_NEW new
#endif


// Global GUI includes
#include <UTL_Macro.hpp>
#include <Onboard_Tok.hpp>
#include <DCP05_TOK.HPP>

#endif  // HEW_STDAFX_H


