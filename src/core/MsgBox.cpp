// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
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

#include "stdafx.h"

#include <dcp06/core/MsgBox.hpp>
#include <GUI_MessageDialog.hpp>

#include <ABL_MsgDef.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>

#include <string>
#include <wchar.h>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::InitializationDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================

namespace {

/** Insert newlines so each line stays within Captivate message-dialog width. Keeps paragraph breaks (\n) from the token. */
StringC WrapMessageDialogTextLimitedWidth(StringC strMessage)
{
	const wchar_t* sIn = strMessage;
	if (!sIn)
		return strMessage;

	const size_t totalLen = wcslen(sIn);
	if (totalLen <= (size_t)DCP_MSG_DIALOG_CHARS_PER_LINE)
		return strMessage;

	enum { HARD_CAP = 8192 };
	size_t cappedLen = totalLen > (size_t)HARD_CAP ? (size_t)HARD_CAP : totalLen;

	const size_t maxCol = (size_t)DCP_MSG_DIALOG_CHARS_PER_LINE > 10 ? (size_t)DCP_MSG_DIALOG_CHARS_PER_LINE : 29;

	std::wstring out;
	out.reserve(cappedLen + (cappedLen / maxCol) + 64);

	size_t paragraphStart = 0;
	bool firstParagraph = true;

	while (paragraphStart < cappedLen)
	{
		size_t paragraphEndExclusive = paragraphStart;
		while (paragraphEndExclusive < cappedLen && sIn[paragraphEndExclusive] != L'\n')
			paragraphEndExclusive++;

		const wchar_t* pPara = sIn + paragraphStart;
		const size_t plen = paragraphEndExclusive - paragraphStart;

		paragraphStart = paragraphEndExclusive + 1;

		if (plen == 0)
			continue;

		if (!firstParagraph)
			out.push_back(L'\n');
		firstParagraph = false;

		size_t wrapPos = 0;
		bool firstWrappedLineOfPara = true;
		while (wrapPos < plen)
		{
			while (wrapPos < plen && pPara[wrapPos] == L' ')
				wrapPos++;
			if (wrapPos >= plen)
				break;

			const size_t sliceEndExclusive = wrapPos + maxCol <= plen ? wrapPos + maxCol : plen;

			size_t chunkEndExclusive = sliceEndExclusive;
			if (sliceEndExclusive < plen)
			{
				bool foundSep = false;
				for (size_t kk = sliceEndExclusive; kk > wrapPos; )
				{
					--kk;
					if (pPara[kk] == L' ')
					{
						chunkEndExclusive = kk;
						foundSep = true;
						break;
					}
				}
				if (!foundSep)
					chunkEndExclusive = sliceEndExclusive;
			}

			if (chunkEndExclusive <= wrapPos)
			{
				chunkEndExclusive = wrapPos + 1 <= plen ? wrapPos + 1 : wrapPos;
			}

			if (!firstWrappedLineOfPara)
				out.push_back(L'\n');
			firstWrappedLineOfPara = false;

			out.append(pPara + wrapPos, chunkEndExclusive - wrapPos);

			wrapPos = chunkEndExclusive;
			while (wrapPos < plen && pPara[wrapPos] == L' ')
				wrapPos++;
		}
	}

	StringC wrapped(out.c_str());
	return wrapped.IsEmpty() ? strMessage : wrapped;
}

} /* namespace */

// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::MsgBox::MsgBox()
{
	
}
// ****************************************************************************************
DCP::MsgBox::~MsgBox()
{
	
}
// ****************************************************************************************
bool DCP::MsgBox::ShowMessageYesNo(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information, 0);    
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Yes|GUI::MessageDialogC::MBR_No ));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Yes)
		{
			return true;
		}
		return false;
}		

// ****************************************************************************************
bool DCP::MsgBox::ShowMessageOkAbort(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort ));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Ok)
		{
			return true;
		}
		return false;
}		

// ****************************************************************************************
void DCP::MsgBox::ShowMessageOk(StringC strMessage, StringC Title)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok ));
		(void)pDlg->Execute();
}

// ****************************************************************************************
short DCP::MsgBox::ShowMessageOkAbortRetry(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0);    
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );
		pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		if(eRes == GUI::MessageDialogC::MBR_Ok)
		{
			return 1;
		}
		else if(eRes == GUI::MessageDialogC::MBR_Abort)
			return -1;
		return 0;
}

// ****************************************************************************************
short DCP::MsgBox::DeleteChstMessage(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0); 
		
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );

		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = pDlg;
		
		vDef.strLable	= StringC(AT_DCP06,K_DCP_MEAS_TOK);
		pDlg->SetFunctionKey( FK2, vDef );

		vDef.strLable	= StringC(AT_DCP06,K_DCP_STATION_TOK);
		pDlg->SetFunctionKey( FK4, vDef );

		vDef.strLable	=StringC(AT_DCP06, K_DCP_ABORT_TOK);
		pDlg->SetFunctionKey( FK6, vDef );

		//pDlg->ActivateFunctionKeys();
		
		//pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		
		//GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		FunctionKeyT eRes = pDlg->GetResultAsFK();
		if(eRes == FK2) // ALL
		{
			return 1;
		}
		else if(eRes == FK4)
		{
			return 2;
		}
		else
			return -1;
	}

short DCP::MsgBox::DeletePointsMeas(StringC strMessage)
{
	
		GUI::MessageDialogC* pDlg = new GUI::MessageDialogC;
		pDlg->SetMessageTitle(GUI::MessageDialogC::MSG_Information , 0); 
		
		pDlg->SetMessageText( WrapMessageDialogTextLimitedWidth(strMessage) );

		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = pDlg;
		
		vDef.strLable	= StringC(AT_DCP06,K_DCP_MEAS_TOK);
		pDlg->SetFunctionKey( FK2, vDef );

		vDef.strLable	= StringC(AT_DCP06,K_DCP_CURRENT_POINT_TOK);
		pDlg->SetFunctionKey( FK4, vDef );

		vDef.strLable	=StringC(AT_DCP06, K_DCP_ABORT_TOK);
		pDlg->SetFunctionKey( FK6, vDef );

		//pDlg->ActivateFunctionKeys();
		
		//pDlg->SetResultMask((GUI::MessageDialogC::MessageBoxResultT) (GUI::MessageDialogC::MBR_Ok|GUI::MessageDialogC::MBR_Abort| GUI::MessageDialogC::MBR_Retry));
		(void)pDlg->Execute();
		
		//GUI::MessageDialogC::MessageBoxResultT eRes = pDlg->GetResult();
		FunctionKeyT eRes = pDlg->GetResultAsFK();
		if(eRes == FK2) // ALL
		{
			return 1;
		}
		else if(eRes == FK4)
		{
			return 2;
		}
		else
			return -1;
}