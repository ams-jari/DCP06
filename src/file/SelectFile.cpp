// ================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright 2002 by Leica Geosystems AG, Heerbrugg
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
#include <dcp06/core/Model.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#ifndef DCP_USE_JSON_DATABASE
#define DCP_USE_JSON_DATABASE 1
#endif
#if DCP_USE_JSON_DATABASE
#include <dcp06/database/JsonDatabase.hpp>
#endif
#include <dcp06/file/DistFile.hpp>
#include <dcp06/file/AngleFile.hpp>
#include <dcp06/file/CircleFile.hpp>
#include <dcp06/file/ShaftFile.hpp>


#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>

#include <boost/filesystem.hpp>

#include <dcp06/core/MsgBox.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP06SelectFileDlgC::DCP06SelectFileDlgC(short iFileType,DCP06ModelC* pDCP06Model):m_iFileType(iFileType), poMultiColCtrl(NULL),m_pDCP06Model(pDCP06Model)
	{
	//SetTxtApplicationId(AT_DCP06);
	m_iFileType = iFileType;
}
// Description: Destructor
DCP06SelectFileDlgC::~DCP06SelectFileDlgC()
{

}

void DCP06SelectFileDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();

	/* CAPTIVATE
	GUI::ListConfigColsC oConfigCols1;

	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_File));
	*/

	//USER_APP_VERIFY(oConfigCols1.AddCol(50, GUI::ListConfigColsC::AL_RIGHT, CI_Points));
	//USER_APP_VERIFY(oConfigCols1.AddCol(CW_Date, GUI::ListConfigColsC::AL_RIGHT, CI_Date));

	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);

	
	// text
	//poMultiColCtrl->AddColStr(L"Select file",CI_File);
	
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_SELECT_FILE_TOK),CI_File));
	//USER_APP_VERIFY(poMultiColCtrl->AddColTok(P_DCP_FILE_TOK, AT_DCP06,CI_File));
	//USER_APP_VERIFY(poMultiColCtrl->AddColTok(P_DCP_POINTS_TOK, AT_DCP06, CI_Points));
	//USER_APP_VERIFY(poMultiColCtrl->AddColTok(P_DCP_DATE_TOK, AT_DCP06,CI_Date));
	
	poMultiColCtrl->AddColSelection(CI_File);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
	/*
	FKDef vDef;
	vDef.strLable = L"CONT";
	vDef.poOwner = this;
	SetFunctionKey(FK1, vDef);
	*/
	
}

void DCP06SelectFileDlgC::OnF1Pressed(void)
{

}

void DCP06SelectFileDlgC::OnDialogActivated()
{
	DCP06MsgBoxC msgbox;

	GUI::TableDialogC::OnDialogActivated();
	/*
	unsigned short unCount  = 0;
	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, L"Filename1"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date, (short) unCount, L"DateFilename1"));

	unCount  = 1;
	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, L"Filename2"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date, (short) unCount, L"DateFilename2"));
	*/

	// 

	//boost::filesystem::path  FileInfo;
	//CPI::FileIteratorC FileIterator;
	char strPath[CPI::LEN_PATH_MAX];
	char* pSearch = &strPath[0];
	short unCount=0;
	

	//BeginDraw();

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, strPath);
	
	StringC sLine;
	char cTemp[80];
	if(m_iFileType == DCP06_JOBS)
	{
		DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if DCP_USE_JSON_DATABASE
		Database::JsonDatabase* jdb = db ? dynamic_cast<Database::JsonDatabase*>(db) : 0;
		if (jdb)
		{
			std::vector<std::string> ids = jdb->listJobIds();
			for (size_t i = 0; i < ids.size(); i++)
			{
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short)i));
				sprintf(cTemp, "%-3d %-24.24s", (int)(i + 1), ids[i].c_str());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short)i, sLine));
			}
		}
#endif
		return;
	}
	if(m_iFileType == ONLY_ADF)
		strcat(strPath,"*.adf");
	else if(m_iFileType == ALL_FILES)
		strcat(strPath,"*.adf");
	else if(m_iFileType == CALCDIST_FILE)
		strcat(strPath,"*.cdf");
	else if(m_iFileType == CALCANGLE_FILE)
		strcat(strPath,"*.agf");
	else if(m_iFileType == CIRCLE_FILE)
		strcat(strPath,"*.crl");
	else if(m_iFileType == SHAFT_FILE)
		strcat(strPath,"*.sft");
	else if(m_iFileType == ADF_BF_STA)
		strcat(strPath,"*.adf");

	char tempFileName[256];
	
	boost::filesystem::path oDirPath = pSearch;
	
	boost::filesystem::path pat = oDirPath.parent_path();
	
	boost::system::error_code errCode;
	if( !boost::filesystem::exists( pat, errCode ) )
	{
		return;
	}

	for( boost::filesystem::directory_iterator itrDir( pat );
		itrDir != boost::filesystem::directory_iterator();
		++itrDir )
	{

		
		boost::filesystem::path FileInfo = *itrDir;

		BSS::UTI::BSS_UTI_WCharToAscii(FileInfo.c_str(), tempFileName);

		//DCP06MsgBoxC msgbox;
		//StringC msg;
		//char tt1[200];
		//sprintf(tt1,"%s %d",tempFileName,itrDir->status().type());
		//msgbox.ShowMessageOk(StringC(tt1));//FileInfo.c_str()));

		if( boost::filesystem::regular_file == itrDir->status().type()) 
		{
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)
	//{
		if(m_iFileType == CALCDIST_FILE)
		{
			if(strstr(tempFileName, ".cdf") != NULL || strstr(tempFileName, ".CDF") != NULL)
			{
				CdfFileFunc CdfFile(&FileInfo, m_pDCP06Model);
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,CdfFile.getFileName(),CdfFile.getPointsCountString(), CdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			}
		}
		else if(m_iFileType == CALCANGLE_FILE)
		{
			if(strstr(tempFileName, ".agf") != NULL || strstr(tempFileName, ".AGF") != NULL)
			{
				AgfFileFunc AgfFile(&FileInfo,m_pDCP06Model );
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AgfFile.getFileName(),AgfFile.getPointsCountString(), AgfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			}
		}
		else if(m_iFileType == CIRCLE_FILE)
		{
			if(strstr(tempFileName, ".crl") != NULL || strstr(tempFileName, ".CRK") != NULL)
			{
				CircleFileFunc CircleFile(&FileInfo, m_pDCP06Model);
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1, CircleFile.getFileName(),"  ", CircleFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			}

		}
		else if(m_iFileType == SHAFT_FILE)
		{
			if(strstr(tempFileName, ".sft") != NULL || strstr(tempFileName, ".SFT") != NULL)
			{
				ShaftFileFunc ShaftFile(&FileInfo, m_pDCP06Model);
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1, ShaftFile.getFileName(),"  ", ShaftFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			}

		}

		else 
		{
			if(strstr(tempFileName, ".adf") != NULL || strstr(tempFileName, ".ADF") != NULL )
			{

				//DCP06MsgBoxC msgbox;
				//StringC msg;
				//msgbox.ShowMessageOk(StringC(tempFileName));
				
				AdfFileFunc AdfFile(&FileInfo, m_pDCP06Model);
				AdfFile.always_single = 1;
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			}
		}

		//unCount++;
		/*
		while(++itrDir != boost::filesystem::directory_iterator())
		//while(FileIterator.FindNext(FileInfo) == 0)
		{
			boost::filesystem::path FileInfo = *itrDir;
			if( boost::filesystem::regular_file == itrDir->status().type()) 
		{
			if(m_iFileType == CALCDIST_FILE)
			{
				CdfFileFunc CdfFile(&FileInfo);
			
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,CdfFile.getFileName(),CdfFile.getPointsCountString(), CdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));

			}
			else if(m_iFileType == CALCANGLE_FILE)
			{
				AgfFileFunc AgfFile(&FileInfo);
			
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AgfFile.getFileName(),AgfFile.getPointsCountString(), AgfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));

			}
			else if(m_iFileType == CIRCLE_FILE)
			{
				CircleFileFunc CircleFile(&FileInfo);
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1, CircleFile.getFileName(),"  ", CircleFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
			}
			else if(m_iFileType == SHAFT_FILE)
			{
				ShaftFileFunc ShaftFile(&FileInfo);
				
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1, ShaftFile.getFileName(),"  ", ShaftFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));

			}


			else
			{
				AdfFileFunc AdfFile(&FileInfo);
				AdfFile.always_single = 1;
			
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
			}
	
			unCount++;
		}
		}*/
		}
	}

	if(m_iFileType == ALL_FILES)
	{
		CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, strPath);
		strcat(strPath,"*.ref");	
		
		boost::filesystem::path oDirPath = strPath;

		for( boost::filesystem::directory_iterator itrDir( pat );
		itrDir != boost::filesystem::directory_iterator();
		++itrDir )
		{
			boost::filesystem::path FileInfo = *itrDir;
			BSS::UTI::BSS_UTI_WCharToAscii(FileInfo.c_str(), tempFileName);

		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)
		//{
			if( boost::filesystem::regular_file == itrDir->status().type()) 
			{
				if(strstr(tempFileName, ".ref") != NULL || strstr(tempFileName, ".REF") != NULL)
				{
					AdfFileFunc AdfFile(&FileInfo, m_pDCP06Model);
					AdfFile.always_single = 1;
				
					USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
					sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
					sLine = cTemp;
					USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
					unCount++;
				}
		
			//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, AdfFile.getFileName()));
			//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Points,(short) unCount, L" ")); 
			//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date,(short) unCount, AdfFile.getModDate())); 

			//unCount++;
			/*
			while(++itrDir != boost::filesystem::directory_iterator())
			//while(FileIterator.FindNext(FileInfo) == 0)
			{
				AdfFileFunc AdfFile(&FileInfo);
				AdfFile.always_single = 1;

				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
	
				//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_File, (short) unCount, AdfFile.getFileName()));
				//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Points,(short) unCount, L" ")); 
		 		//USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_Date,(short) unCount, AdfFile.getModDate())); 

				unCount++;
			} 
			*/
			}
		}
	}
	if(m_iFileType == ADF_BF_STA)
	{
		// bestfit files
		CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, strPath);
		strcat(strPath,"*.bft");	
		
		boost::filesystem::path oDirPath = strPath;

		for( boost::filesystem::directory_iterator itrDir( pat );
		itrDir != boost::filesystem::directory_iterator();
		++itrDir )
		{
			boost::filesystem::path FileInfo = *itrDir;
			BSS::UTI::BSS_UTI_WCharToAscii(FileInfo.c_str(), tempFileName);

			if( boost::filesystem::regular_file == itrDir->status().type()) 
			{
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)
		//{
				if( boost::filesystem::regular_file == itrDir->status().type()) 
				{
					if(strstr(tempFileName, ".ref") != NULL || strstr(tempFileName, ".REF") != NULL)
					{
						AdfFileFunc AdfFile(&FileInfo, m_pDCP06Model);
						AdfFile.always_single = 1;
					
						USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
						sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
						sLine = cTemp;
						USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				
						unCount++;
					}
				}
			/*
			while(++itrDir != boost::filesystem::directory_iterator())
			//while(FileIterator.FindNext(FileInfo) == 0)
			{
				AdfFileFunc AdfFile(&FileInfo);
				AdfFile.always_single = 1;

				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			} */
			}
		}

		// move station files
		CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, strPath);
		strcat(strPath,"*.sta");	
		
		oDirPath = strPath;
	
		for( boost::filesystem::directory_iterator itrDir( pat );
		itrDir != boost::filesystem::directory_iterator();
		++itrDir )
		{
			boost::filesystem::path FileInfo = *itrDir;
			BSS::UTI::BSS_UTI_WCharToAscii(FileInfo.c_str(), tempFileName);

		if( boost::filesystem::regular_file == itrDir->status().type()) 
		{
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)
		//{
			if(strstr(tempFileName, ".sta") != NULL || strstr(tempFileName, ".STA") != NULL)
			{
				AdfFileFunc AdfFile(&FileInfo, m_pDCP06Model);
				AdfFile.always_single = 1;
			
				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
		
				unCount++;
			}
			/*
			while(++itrDir != boost::filesystem::directory_iterator())
			//while(FileIterator.FindNext(FileInfo) == 0)
			{
				AdfFileFunc AdfFile(&FileInfo);
				AdfFile.always_single = 1;

				USER_APP_VERIFY(poMultiColCtrl->AddRow((short) unCount));
				sprintf(cTemp,"%-3d %-12.12s %-3.3s %-s",unCount+1,AdfFile.getFileName(),AdfFile.getPointsCountString(), AdfFile.getModDate());
				sLine = cTemp;
				USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_File, (short) unCount, sLine));
				unCount++;
			} */
			}
		}
	}
	//EndDraw();
}

void DCP::DCP06SelectFileDlgC::UpdateData()
{
	StringC sSelected;
	short iSelected = poMultiColCtrl->GetSelectedId();
	poMultiColCtrl->GetCellText(CI_File,iSelected, sSelected);
	StringC sTemp = (m_iFileType == DCP06_JOBS) ? sSelected.Mid(4, 24) : sSelected.Mid(4, 12);
	sTemp.Trim();
	GetDataModel()->m_strSelectedFile = sTemp;
}
// Description: only accept hello world Model objects
bool DCP::DCP06SelectFileDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06SelectFileModelC* pDCP06Model = dynamic_cast< DCP::DCP06SelectFileModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP06SelectFileModelC* DCP::DCP06SelectFileDlgC::GetDataModel() const
{
    return (DCP::DCP06SelectFileModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::DCP06SelectFileControllerC::DCP06SelectFileControllerC(short iFileType, StringC sTitle,DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model), m_iFileType(iFileType)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    //SetTitleTok( AT_DCP06, T_DCP_3D_FILE_DLG_TOK /*C_DCP_APPLICATION_NAME_TOK */);
	SetTitle(sTitle);

    // Create a dialog
    m_pDlg = new DCP::DCP06SelectFileDlgC(iFileType,pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( ANGLE_FILE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	/*
	vDef.nLable = K_DCP_OPEN_TOK;
	SetFunctionKey( FK1, vDef );

	vDef.nLable = K_DCP_NEW_TOK;
	SetFunctionKey( FK2, vDef );

	vDef.nLable = K_DCP_COPY_TOK;
	SetFunctionKey( FK3, vDef );

	vDef.nLable = K_DCP_SWAP_TOK;
	SetFunctionKey( FK4, vDef );

	vDef.nLable = K_DCP_CLOSE_TOK;
	SetFunctionKey( FK5, vDef );
	*/
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

	/*
	// SHIFT
	vDef.nLable = K_DCP_DEL_TOK;
	SetFunctionKey( SHFK2, vDef );
	
	vDef.nLable = K_DCP_SAVE_TOK;
	SetFunctionKey( SHFK3, vDef );

	vDef.nLable = K_DCP_VIEW_TOK;
	SetFunctionKey( SHFK5, vDef );
	*/
} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06SelectFileControllerC::~DCP06SelectFileControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06SelectFileControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// OPEN
void DCP::DCP06SelectFileControllerC::OnF1Pressed()
{

}

// NEW
void DCP::DCP06SelectFileControllerC::OnF2Pressed()
{
}

// CLOSE
void DCP::DCP06SelectFileControllerC::OnF5Pressed()
{

}

// CONT
void DCP::DCP06SelectFileControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}
// DEL
void DCP::DCP06SelectFileControllerC::OnSHF2Pressed()
{
}

// VIEW
void DCP::DCP06SelectFileControllerC::OnSHF5Pressed()
{
	/*
	if(GetController(INIT_CONTROLLER) == NULL)
	{
		(void)AddController( INIT_CONTROLLER, new DCP::DCP06InitControllerC );
	}
	(void)GetController( INIT_CONTROLLER )->SetModel( m_pDCP06MeasDlg->GetDataModel());
	SetActiveController(INIT_CONTROLLER, true);
	*/
}

// Description: React on close of tabbed dialog
void DCP::DCP06SelectFileControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06SelectFileControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06SelectFileModelC::DCP06SelectFileModelC()
{
	m_strSelectedFile = L"";
}
DCP::DCP06SelectFileModelC::~DCP06SelectFileModelC()
{
}

