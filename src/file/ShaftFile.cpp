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
#include "calc.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/file/ShaftFile.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/InputText.hpp>


#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
//#include <CPI_FileUtilities.hpp>

#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

#define ROW_LENGTH 78
// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP06ShaftFileDlgC::DCP06ShaftFileDlgC(DCP06ShaftFileModelC* pModel):
	m_pFile(0),  m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),
	m_pDataModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}
// Description: Destructor
DCP06ShaftFileDlgC::~DCP06ShaftFileDlgC()
{

}

void DCP06ShaftFileDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	
	//InsertEmptyLine(); // CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_SHAFT_FILE_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pFile);

	m_pSize = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pSize->SetId(eSize);
	m_pSize->SetText(StringC(AT_DCP06,P_DCP_SIZE_OF_FILE_TOK));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pSize);

	m_pDate = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDate->SetId(eDate);
	m_pDate->SetText(StringC(AT_DCP06,P_DCP_DATE_OF_FILE_TOK));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pDate);

	m_pTime = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTime->SetId(eTime);
	m_pTime->SetText(StringC(AT_DCP06,P_DCP_TIME_OF_FILE_TOK));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pTime);

	m_pFreeSpace = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFreeSpace->SetId(eFreeSpace);
	m_pFreeSpace->SetText(StringC(AT_DCP06,P_DCP_FREE_SPACE_TOK));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pFreeSpace);
	
	//SetHelpTok(H_DCP_SHAFT_FILE_TOK, 0);
}

void DCP06ShaftFileDlgC::OnDialogActivated()
{
	if(!GetDCP06Model()->sShaftFile.IsEmpty())
		m_pDataModel->m_pFileFunc->setFile(GetDCP06Model()->sShaftFile);

	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06ShaftFileDlgC::RefreshControls()
{	
	if(m_pFile && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		//m_pFileModel->m_pAdfFile->setFile(sSelectedFile/*GetDCP06Model()->ADFFileName*/);

		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getFileName());//GetDCP06Model()->ADFFileName);	
			m_pSize->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getFileSizeString());
			m_pDate->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getModDate());
			m_pTime->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getModTime());
		}
		else
		{
			m_pFile->GetStringInputCtrl()->SetString(L" ");
			m_pSize->GetStringInputCtrl()->SetString(L" ");
			m_pDate->GetStringInputCtrl()->SetString(L" ");
			m_pTime->GetStringInputCtrl()->SetString(L" ");
		}

			unsigned int uiFreeSpace = m_pDataModel->pCommon->get_free_space();
			char temp[100];
			sprintf(temp,"%lu",uiFreeSpace);
			m_pFreeSpace->GetStringInputCtrl()->SetString(StringC(temp));
	}
}

void DCP::DCP06ShaftFileDlgC::UpdateData()
{
	if(m_pDataModel->m_pFileFunc->IsOpen())
	{	
        GetDCP06Model()->sShaftFile = m_pDataModel->m_pFileFunc->getFileName();
		GetDCP06Model()->sShaftFile.RTrim();
	}
	else
		GetDCP06Model()->sShaftFile = L"";
}


// Description: only accept hello world Model objects
bool DCP::DCP06ShaftFileDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

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
DCP::DCP06ModelC* DCP::DCP06ShaftFileDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::DCP06ShaftFileControllerC::DCP06ShaftFileControllerC(DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SHAFT_FILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// FileModel
	m_pDataModel = new DCP::DCP06ShaftFileModelC(m_pDCP06Model); 
 
    // Create a dialog
    m_pDlg = new DCP::DCP06ShaftFileDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SHAFT_FILE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_OPEN_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_NEW_TOK);
	SetFunctionKey( FK2, vDef );
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_CLOSE_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
	SetFunctionKey( SHFK5, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP06ShaftFileControllerC::~DCP06ShaftFileControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06ShaftFileControllerC::SetModel( GUI::ModelC* pModel )
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
void DCP::DCP06ShaftFileControllerC::OnF1Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::DCP06SelectFileModelC* pModel = new DCP06SelectFileModelC;

		if(GetController(SELECT_FILE_CONTROLLER) == NULL)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(SHAFT_FILE, sTitle, m_pDCP06Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);

}

// NEW
void DCP::DCP06ShaftFileControllerC::OnF2Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_NEW_FILENAME_TOK);
		pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = 8;
		pModel->m_StrText = L" ";

		if ( NULL == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
		{
			USER_APP_VERIFY( false );
			return;
		}

		if(GetController(INPUT_TEXT_CONTROLLER) == NULL)
		{
			(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP06InputTextControllerC( m_pDCP06Model));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDCP06FileDlg->GetDCP06Model());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);

}

// CLOSE
void DCP::DCP06ShaftFileControllerC::OnF5Pressed()
{	
	if(m_pDataModel->m_pFileFunc->IsOpen())
		m_pDataModel->m_pFileFunc->CloseFile();

	m_pDlg->RefreshControls();

}

// CONT
void DCP::DCP06ShaftFileControllerC::OnF6Pressed()
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
void DCP::DCP06ShaftFileControllerC::OnSHF2Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;
	m_pDataModel->m_pFileFunc->delete_file();
	m_pDlg->RefreshControls();

}

// VIEW
void DCP::DCP06ShaftFileControllerC::OnSHF5Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;

	(void)ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_UTIL_FILE_VIEWER_ID);
}

// Description: React on close of tabbed dialog
void DCP::DCP06ShaftFileControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06ShaftFileControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectFileModelC* pModel = (DCP::DCP06SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		m_pDataModel->m_pFileFunc->setFile(strSelectedFile);
	}

		if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
		{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
	
			char fname[CPI::LEN_PATH_MAX];
			//DCP06MsgBoxC msgbox;
			//msgbox.ShowMessageOk(strNewFile);
			//UTL::UnicodeToAscii(fname, strNewFile);
			BSS::UTI::BSS_UTI_WCharToAscii( strNewFile, fname );
			
			m_pDataModel->m_pFileFunc->create_new_file(fname);
			m_pDataModel->m_pFileFunc->setFile(strNewFile);
		
			 //m_pDCP06FileDlg->CreateFile(sFileToCreate, pModel->m_StrText);
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// ======================================  DCP06AngleFileModelC====================================
// ================================================================================================


// ===========================================================================================
// DCP06AngleFileModelC
// ===========================================================================================

// Instantiate template classes
DCP::DCP06ShaftFileModelC::DCP06ShaftFileModelC(DCP06ModelC* pDCP06Model)
{
	m_pFileFunc = new ShaftFileFunc(pDCP06Model);
	pCommon = new DCP06CommonC(pDCP06Model);

}
DCP::DCP06ShaftFileModelC::~DCP06ShaftFileModelC()
{
	if(m_pFileFunc)
	{
		delete m_pFileFunc;
		m_pFileFunc = 0;
	}
	if(pCommon)
	{
		delete pCommon;
		pCommon = 0;
	}
}



// ================================================================================================
// ======================================  DCP06AngleFileModelC====================================
// ================================================================================================

// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

//DCP::ShaftFileFunc::ShaftFileFunc(): m_pFile(0), m_bExists(false),opened(0)
//{
//	m_cPath[0] = '\0';
//	m_cPathAndFileName[0] = '\0';
//	m_cFileName[0] = '\0';
//
//	// get path
//	getPath();
//
//	m_pCommon = new DCP06CommonC(m_pDCP06Model);
//}

DCP::ShaftFileFunc::ShaftFileFunc(DCP06ModelC* pDCP06Model): m_pFile(0), m_bExists(false),opened(0),
				m_pDCP06Model(pDCP06Model)
{
	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';

	m_pCommon = new DCP06CommonC(pDCP06Model);

	// get path
	getPath();
}
// ****************************************************************************************
DCP::ShaftFileFunc::ShaftFileFunc(boost::filesystem::path * FileInfo,DCP06ModelC* pDCP06Model):m_pFile(0), m_bExists(false),m_pDCP06Model(pDCP06Model)
{
	m_pCommon = new DCP06CommonC(m_pDCP06Model);

	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';

	getPath();
	
	ParseFileNameAndPath(FileInfo);
	ParseModTime(FileInfo);
	//points = ReadPointsCount(m_cPathAndFileName);

}


// ****************************************************************************************
            // Description: Destructor
DCP::ShaftFileFunc::~ShaftFileFunc()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// ****************************************************************************************
void DCP::ShaftFileFunc::getPath()
{
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	//CPI::FileUtilitiesC::MakeDir(m_cPath);
	boost::filesystem::path filePath= m_cPath;
    boost::system::error_code errCode;
	boost::filesystem::create_directory(filePath, errCode);
}

// ****************************************************************************************
bool DCP::ShaftFileFunc::setFile(StringC filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path  FileInfo;

	char filename_temp[FILENAME_BUFF_LEN];
	BSS::UTI::BSS_UTI_WCharToAscii( filename, filename_temp );
	//UTL::UnicodeToAscii(filename_temp, filename);
	m_pCommon->strbtrim(filename_temp);


	CloseFile();
	
	//bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::devicePcCard, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
	char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename_temp);
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp, temp_name);

	if(!strstr(temp,".sft"))
		strcat(temp,".sft");

	char* pSearch = &temp[0];

	if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		ParseFileNameAndPath(&FileInfo);
		ParseModTime(&FileInfo);
		open_crl_file_name("rb+");
		//points = ReadPointsCount(m_cPathAndFileName);
		m_bExists = true;
		return true;
	}
	m_bExists = false;
	return false;
}
// ****************************************************************************************
void DCP::ShaftFileFunc::ParseModTime(boost::filesystem::path* FileInfo)
{
	std::time_t t = boost::filesystem::last_write_time(*FileInfo);
	struct tm* tt = localtime(&t);
	
	sprintf(m_cDate, "%02ld.%02ld.%04ld", tt->tm_mday ,tt->tm_mon +1, tt->tm_year + 1900);
	sprintf(m_cTime,"%02d:%02d",tt->tm_hour, tt->tm_min);
	/*
	int lYear, lMonth, lDay, lHour, lMin, lSec;
	
	DateTime dDate = FileInfo->GetModificationTime();
	Unit::SetMjd(dDate,lYear, lMonth, lDay, lHour, lMin, lSec); 
	sprintf(m_cDate, "%02ld.%02ld.%04ld", lDay,lMonth, lYear);
	
	sprintf(m_cTime,"%02d:%02d",lHour,lMin);
	*/
}
// ****************************************************************************************
void DCP::ShaftFileFunc::ParseFileNameAndPath(boost::filesystem::path* FileInfo)
{
	StringC strFileNameAll =  FileInfo->filename().c_str();
	//UTL::UnicodeToAscii(m_cFileName, strFileNameAll);
	BSS::UTI::BSS_UTI_WCharToAscii(strFileNameAll, m_cFileName);
	

	StringC strFileName = strFileNameAll.Mid(0,strFileNameAll.GetLength()-4);

	strcat(m_cPathAndFileName,m_cPath);
	strcat(m_cPathAndFileName,m_cFileName);

	m_lSize = (uint32_t) boost::filesystem::file_size(*FileInfo);//FileInfo->GetSize();
}

// ****************************************************************************************
char* DCP::ShaftFileFunc::getFileName()
{
	return m_cFileName;
}
// ****************************************************************************************
char* DCP::ShaftFileFunc::getModDate()
{
	return m_cDate;
}
// ****************************************************************************************
char* DCP::ShaftFileFunc::getModTime()
{
	return m_cTime;
}

// ****************************************************************************************
char* DCP::ShaftFileFunc::getFileSizeString()
{
	sprintf(m_cSize,"%ld",m_lSize);
	return m_cSize;
}

// ****************************************************************************************
bool DCP::ShaftFileFunc::isExists()
{
	return m_bExists;
}


// ****************************************************************************************
void DCP::ShaftFileFunc::CloseFile()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	opened = 0;
	m_bExists = false;
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';
	m_lSize = 0;
}

// ****************************************************************************************
short DCP::ShaftFileFunc::open_crl_file_name(const char* mode)
{
short ret;
int Result;
	
	if(!m_pCommon->card_status())//(1) != 0)
		return -1;

	DCP06MsgBoxC msgbox;
	
	ret = -1;

	if(m_pFile)
		close_crl_file();

	if(!fopen1(mode))// /*FIL_ACC_RDWR */,&fp) != TRUE)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP06,M_DCP_FILE_OPEN_ERROR_TOK);
		msg.Format(msg,(const wchar_t*)StringC(m_cPathAndFileName/*m_cFileName*/));
		msgbox.ShowMessageOk(msg);
		//msgbox1(TXT_NIL_TOKEN,M_DCP_FILE_OPEN_ERROR_TOK,(void *) fname,MB_OK);
	}
	else
	{

		//CPI::FileIteratorC FileIterator;
		boost::filesystem::path  FileInfo;

		char temp[CPI::LEN_PATH_MAX];
		temp[0] = '\0';
		strcat(temp, m_cPath);
		strcat(temp, m_cFileName);
		char* pSearch = &temp[0];

		if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0) 
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
		{
			m_lSize = (uint32_t)boost::filesystem::file_size(FileInfo);		
		}

		Result = fseek(m_pFile, 0L, SEEK_SET);
		fseek(m_pFile,0L, SEEK_END);

		Result = fseek(m_pFile, 0L, SEEK_SET);
		
		opened = 1;
		ret = 1;
	}

	return  ret;
}

/**************************************************************
**************************************************************/
short DCP::ShaftFileFunc::fopen1(const char* mode)
{
	m_pFile = fopen(m_cPathAndFileName, mode);	
	
	if(m_pFile)
	{
		return true;		
	}
	DCP06MsgBoxC msgbox;
	StringC msg;
	msg.LoadTxt(AT_DCP06,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(m_cPathAndFileName));
			msgbox.ShowMessageOk(msg);
	return false;
}


/***********************************************************************
	FUNCTION   :int close_adf_file();
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::ShaftFileFunc::close_crl_file()
{

	if(m_pFile)
	{
		if(!m_pCommon->card_status())
			return false;

		fflush(m_pFile);
		fclose(m_pFile);
	}
	m_pFile = 0;
	opened = 0;

	return 1;
}


//*****************************************************************************
short DCP::ShaftFileFunc::remove1(char *fname)
{
char apu[CPI::LEN_PATH_MAX];
bool Result;
	DCP06CommonC common(m_pDCP06Model);

    sprintf(apu,"%s%-s",m_cPath,common.strbtrim(fname));

		// VIVA
	//Result = CPI::FileUtilitiesC::RemoveFile(StringC(apu));
	//Result = remove(apu);	
	boost::filesystem::path f = apu;
	Result = boost::filesystem::remove(f);

	if(Result)
	{
		return 0;
	}
	//int y = errno;
	return -1;
   
} 

/************************************************************************
*************************************************************************/
//	save_calcdist_to_file(&CalcDistFile_,strDist, strRef,strRefType,strTrgt,strTrgtType,bDid,bNote,TRUE);

short DCP::ShaftFileFunc::save_shaft_to_file(StringC Id, DCP06ShaftModelC* pModel)
{
char temp1[100];
StringC msg;
DCP06MsgBoxC msgbox;
int iInstNo=0;
short i;
struct ams_vector m;
struct plane wplane;
double dist;

	if(!opened)
	{
		return 0;
	}

	if(!m_pCommon->card_status())//(1) != 0)
	{
		return 0;
	}

	if(!m_pCommon->check_free_space(30000L))
	{
		return 0;
	}

	fseek(m_pFile, 0L, SEEK_END);

	char temp[100];
	//UTL::UnicodeToAscii(temp,Id);
	BSS::UTI::BSS_UTI_WCharToAscii( Id, temp );

	sprintf(temp1,"\nShaftId:%s%c%c", temp,13,10); 
	fputs(temp1,m_pFile);

	int iYear, iMonth, iDay, iHour, iMin, iSec;
	m_pCommon->GetDate(&iDay, &iMonth, &iYear);
	m_pCommon->GetTime(&iHour, &iMin, &iSec);

	sprintf(temp1,"Date/Time:%02d.%02d.%04d %02d:%02d:%02d%c%c", iDay, iMonth, iYear, iHour, iMin, iSec,13,10); 
	fputs(temp1,m_pFile);

	
	m_pCommon->GetUserName(temp);
	sprintf(temp1,"User:%s%c%c", temp,13,10); 
	fputs(temp1,m_pFile);

	m_pCommon->GetInstrumentName(temp);
	m_pCommon->GetInstrumentNo(&iInstNo);
	sprintf(temp1,"Instrument(type/no):%s %d%c%c", temp,iInstNo,13,10); 
	fputs(temp1,m_pFile);
	/*
	sprintf(temp1,"Center point(x,y,z):%9.*f %9.*f %9.*f%c%c", m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cx, 
													m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cy, 
													m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cz,13,10); 
	fputs(temp1,m_pFile);
	*/
	sprintf(temp1,"Center point:%c%c",13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  x:   %9.*f%c%c",m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cx,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  y:   %9.*f%c%c",m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cy,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  z:   %9.*f%c%c",m_pDCP06Model->m_nDecimals,pModel->shaft_circle_cz,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"Radius:%9.*f%c%c", m_pDCP06Model->m_nDecimals,pModel->shaft_circle_diameter,13,10); 
	fputs(temp1,m_pFile);

	sprintf(temp1,"Normal:%c%c",13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  i:   %9.*f%c%c",6,pModel->shaft_circle_vi,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  j:   %9.*f%c%c",6,pModel->shaft_circle_vj,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  k:   %9.*f%c%c",6,pModel->shaft_circle_vk,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"RMS:   %9.*f%c%c", m_pDCP06Model->m_nDecimals,pModel->shaft_circle_rms,13,10); 
	fputs(temp1,m_pFile);
	
	sprintf(temp1,"Angle: %9.*f%c%c", m_pDCP06Model->m_nDecimals,pModel->angleLines,13,10); 
	fputs(temp1,m_pFile);
	
	sprintf(temp1,"Distance: %9.*f%c%c", m_pDCP06Model->m_nDecimals,pModel->centerOfCircleDist,13,10); 
	fputs(temp1,m_pFile);

	sprintf(temp1,"Deviations of points%c%c",13,10); 
	fputs(temp1,m_pFile);
	
	short sta;
	short count=0;
	for(i =0; i < MAX_POINTS_IN_CIRCLE; i++)
	{
		sta = pModel->shaft_circle_points[0].points[i].sta;
		
		if(sta == 1 || sta == 2) // measured or design
		{
			count++;
			sprintf(temp1,"%-2d.%-6.6s %+9.*f%c%c", count,pModel->shaft_circle_points[0].points[i].point_id, 
									m_pDCP06Model->m_nDecimals, pModel->shaft_circle_points[0].points[i].diameter - pModel->shaft_circle_points[0].diameter,
									13,10);
			fputs(temp1,m_pFile);
		}
	}

	sprintf(temp1,"Point distances from plane%c%c",13,10); 
	fputs(temp1,m_pFile);
	count=0;
	for(i =0; i < MAX_POINTS_IN_CIRCLE; i++)
	{
		sta = pModel->shaft_circle_points[0].points[i].sta;
		
		if(sta == 1 || sta == 2) // measured or design
		{
			count++;
			

			
			
			m.x = pModel->shaft_circle_points->points[i].x;
			m.y = pModel->shaft_circle_points->points[i].y;
			m.z = pModel->shaft_circle_points->points[i].z;

			wplane.px = pModel->calc_plane[0].px;
			wplane.py = pModel->calc_plane[0].py;
			wplane.pz = pModel->calc_plane[0].pz;

			wplane.nx = pModel->calc_plane[0].nx;
			wplane.ny = pModel->calc_plane[0].ny;
			wplane.nz = pModel->calc_plane[0].nz;

			/* Calculate distance */
			dist = calc_point_dist_from_plane(&m, &wplane);

			sprintf(temp1,"%-2d.%-6.6s %+9.*f%c%c", count,pModel->shaft_circle_points[0].points[i].point_id, 
									m_pDCP06Model->m_nDecimals,dist,
									13,10);
			fputs(temp1,m_pFile);
			
		}
	}
	

	sprintf(temp1,"%c%c%c%c",13,10,13,10);
	fputs(temp1,m_pFile);

	fflush(m_pFile);
	
	close_crl_file();
	open_file();
	
	return 1;
}




/************************************************************************
*************************************************************************/
short DCP::ShaftFileFunc::delete_file(void)
{
	DCP06MsgBoxC msgbox;
	StringC msg;

	short result=-1;
	short ret=true;
	char Temp[13];

	if(!m_pCommon->card_status())//1) != 0)
		return false;
	
	if(!opened)
		return false;

	//if(msgbox(TXT_NIL_TOKEN,M_DELETE_FILE_TOK, MB_YESNO) == TRUE)
	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_FILE_TOK);
	msg.Format(msg,(const wchar_t*)StringC(m_cFileName));
	if(msgbox.ShowMessageYesNo(msg))
	//if(msgbox1(TXT_NIL_TOKEN,M_DELETE_FILE_TOK, (void *) fstruct->name, MB_YESNO) == TRUE)
	{
		strcpy(Temp, m_cFileName);
		close_crl_file();
		result = remove1(Temp);
		if(result == -1)
		{
			msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_DELETE_FILE_TOK);
			msgbox.ShowMessageOk(msg);
			ret = false;
		}
	}
	return ret;

}

// ************************************************************************
// ************************************************************************
short DCP::ShaftFileFunc::create_new_file(char* filename)
{
char fname[13];
short ret;
StringC msg;
DCP06MsgBoxC msgbox;
char temp[CPI::LEN_PATH_MAX];

	if(!m_pCommon->check_free_space(30000L))
			return 1;

	sprintf(fname,"%-8s",filename );
	/*
	if(input_text(T_CALCDISTFILE_TOK, M_ENTER_FILENAME_TOK,emptyinfo, 8,fname) == TRUE)
	{
	*/
		m_pCommon->strbtrim(fname);
		if(m_pCommon->strblank(fname))
		{
			return 0;
		}

		strcat(fname, ".sft");
		
		temp[0] = '\0';
		strcat(temp,m_cPath);
		strcat(temp,fname);
		sprintf(m_cPathAndFileName,"%-s%-s",m_cPath,fname);
		
		if(access1(fname) == 1)
		{
			msg.LoadTxt(AT_DCP06,M_DCP_DELETE_OLD_FILE_TOK);
			msg.Format(msg,(const wchar_t*)StringC(fname/*m_cFileName*/));
			if(msgbox.ShowMessageYesNo(msg))
			{
				return 0;
			}

			if(remove1(fname) != 0)
			{
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_DELETE_FILE_TOK);
				msgbox.ShowMessageOk(msg);
				return 0;
			}

		}

		close_crl_file();
		
		ret = create_new_shaftfile(m_cPathAndFileName);

	    //strcpy(CalcDistFile_->name, fname);
	//}

	open_file();
	return 1;
}

/**************************************************************
**************************************************************/
short DCP::ShaftFileFunc::access1(char *fname)
{
int attr = 0;

	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo; FileInfo;

	//char filename_temp[20];
	//UTL::UnicodeToAscii(filename_temp, fname);

	//bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::devicePcCard, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
	temp[0] = '\0';

	strcat(temp, m_cPath);
	strcat(temp, fname);
	char* pSearch = &temp[0];

	if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		return 1;
	}
	return 0;
}


// ************************************************************************
// ************************************************************************
short DCP::ShaftFileFunc::open_file()
{
	if(opened)
	{
		if(!m_pCommon->card_status())
			return 1;

		fclose(m_pFile);
	}

	if(fopen1("rb+"))
	{
		fseek(m_pFile,0,SEEK_SET);

		//CPI::FileIteratorC FileIterator;
		boost::filesystem::path  FileInfo;

		char temp[CPI::LEN_PATH_MAX];
		temp[0] = '\0';
		strcat(temp, m_cPathAndFileName);
		//strcat(temp, m_cPath);
		//strcat(temp, m_cFileName);
		char* pSearch = &temp[0];

		if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
		{
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);//		
		}

		fseek(m_pFile, 0L, SEEK_SET);
		fseek(m_pFile,0L, SEEK_END);

		fseek(m_pFile, 0L, SEEK_SET);
		
		opened = 1;
		return 1;
	}
	else
	{
		opened = 0;
		return 0;
	}
}


// ************************************************************************
//
// ************************************************************************
short DCP::ShaftFileFunc::create_new_shaftfile(char  *fname)
{
short ret=0;
//char temp[100];
	
	
	if(fopen1("wb+"))
	{
		ret = 1;
	}
	else
	{
	}

	fclose(m_pFile);
	return ret;
}