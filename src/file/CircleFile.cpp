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
#include <dcp06/core/Model.hpp>
#include <dcp06/file/CircleFile.hpp>
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
CircleFileDialog::CircleFileDialog(CircleFileModel* pModel):
	m_pFile(0),  m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),
	m_pDataModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}
// Description: Destructor
CircleFileDialog::~CircleFileDialog()
{

}

void CircleFileDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	
	// InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_FILE_TOK));
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
	
	//SetHelpTok(H_DCP_CIRCLE_FILE_TOK, 0);
}

void CircleFileDialog::OnDialogActivated()
{
	if(!GetModel()->sCircleFile.IsEmpty())
		m_pDataModel->m_pFileFunc->setFile(GetModel()->sCircleFile);

	RefreshControls();
}

// Description: refresh all controls
void DCP::CircleFileDialog::RefreshControls()
{	
	if(m_pFile && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		//m_pFileModel->m_pAdfFile->setFile(sSelectedFile/*GetModel()->ADFFileName*/);

		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getFileName());//GetModel()->ADFFileName);	
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

void DCP::CircleFileDialog::UpdateData()
{
	if(m_pDataModel->m_pFileFunc->IsOpen())
	{	
        GetModel()->sCircleFile = m_pDataModel->m_pFileFunc->getFileName();
		GetModel()->sCircleFile.RTrim();
	}
	else
		GetModel()->sCircleFile = L"";
}


// Description: only accept DCP06 Model objects
bool DCP::CircleFileDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: DCP06 model
DCP::Model* DCP::CircleFileDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::CircleFileController::CircleFileController(Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_CIRCLE_FILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// FileModel
	m_pDataModel = new DCP::CircleFileModel(m_pModel); 
 
    // Create a dialog
    m_pDlg = new DCP::CircleFileDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CIRCLE_FILE_DLG, m_pDlg, true );

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


DCP::CircleFileController::~CircleFileController()
{

}

// Description: Route model to everybody else
bool DCP::CircleFileController::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// OPEN
void DCP::CircleFileController::OnF1Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::SelectFileModel* pModel = new SelectFileModel;

		if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(CIRCLE_FILE, sTitle,m_pModel) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);

}

// NEW
void DCP::CircleFileController::OnF2Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::InputTextModel* pModel = new InputTextModel;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_NEW_FILENAME_TOK);
			pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = DCP_JOB_ID_MAX_LEN;
		pModel->m_StrText = L" ";

		if ( nullptr == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
		{
			USER_APP_VERIFY( false );
			return;
		}

		if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
		{
			(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController(m_pModel ));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDCP06FileDlg->GetModel());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);

}

// CLOSE
void DCP::CircleFileController::OnF5Pressed()
{	
	if(m_pDataModel->m_pFileFunc->IsOpen())
		m_pDataModel->m_pFileFunc->CloseFile();

	m_pDlg->RefreshControls();

}

// CONT
void DCP::CircleFileController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
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
void DCP::CircleFileController::OnSHF2Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;
	m_pDataModel->m_pFileFunc->delete_file();
	m_pDlg->RefreshControls();

}

// VIEW
void DCP::CircleFileController::OnSHF5Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;

	(void)ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_UTIL_FILE_VIEWER_ID);
}

// Description: React on close of tabbed dialog
void DCP::CircleFileController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::CircleFileController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		m_pDataModel->m_pFileFunc->setFile(strSelectedFile);
	}

		if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
		{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
	
			char fname[CPI::LEN_PATH_MAX];
			//MsgBox msgbox;
			//msgbox.ShowMessageOk(strNewFile);
			//UTL::UnicodeToAscii(fname, strNewFile);
			BSS::UTI::BSS_UTI_WCharToAscii(strNewFile, fname);
			
			m_pDataModel->m_pFileFunc->create_new_file(fname);
			m_pDataModel->m_pFileFunc->setFile(strNewFile);
		
			 //m_pDCP06FileDlg->CreateFile(sFileToCreate, pModel->m_StrText);
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// ======================================  AngleFileModel====================================
// ================================================================================================


// ===========================================================================================
// AngleFileModel
// ===========================================================================================

// Instantiate template classes
DCP::CircleFileModel::CircleFileModel(Model* pModel):m_pModel(pModel)
{
	m_pFileFunc = new CircleFileFunc(m_pModel);
	pCommon = new Common(m_pModel);

}
DCP::CircleFileModel::~CircleFileModel()
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
// ======================================  AngleFileModel====================================
// ================================================================================================

// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

//DCP::CircleFileFunc::CircleFileFunc(): m_pFile(0), m_bExists(false),opened(0)
//{
//	m_cPath[0] = '\0';
//	m_cPathAndFileName[0] = '\0';
//	m_cFileName[0] = '\0';
//
//	// get path
//	getPath();
//
//	m_pCommon = new Common(m_pModel);
//}

DCP::CircleFileFunc::CircleFileFunc(Model* pModel): m_pFile(0), m_bExists(false),opened(0),
				m_pModel(pModel)
{
	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';

	m_pCommon = new Common(pModel);

	// get path
	getPath();
}
// ****************************************************************************************
DCP::CircleFileFunc::CircleFileFunc(boost::filesystem::path* FileInfo,Model* pModel):m_pFile(0), m_bExists(false),m_pModel(pModel)
{
	m_pCommon = new Common(m_pModel);

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
DCP::CircleFileFunc::~CircleFileFunc()
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
void DCP::CircleFileFunc::getPath()
{
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pModel->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	//CPI::FileUtilitiesC::MakeDir(m_cPath);

	boost::filesystem::path filePath= m_cPath;
    boost::system::error_code errCode;
	boost::filesystem::create_directory(filePath, errCode);
}

// ****************************************************************************************
bool DCP::CircleFileFunc::setFile(StringC filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

	char filename_temp[FILENAME_BUFF_LEN];
	//UTL::UnicodeToAscii(filename_temp, filename);
	BSS::UTI::BSS_UTI_WCharToAscii(filename, filename_temp);
	m_pCommon->strbtrim(filename_temp);


	CloseFile();
	
	//bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::devicePcCard, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
	char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename_temp);
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp, temp_name);

	if(!strstr(temp,".crl")/* || !strstr(temp,".CRL")*/)
		strcat(temp,".crl");

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
void DCP::CircleFileFunc::ParseModTime(boost::filesystem::path* FileInfo)
{
	/*
	int lYear, lMonth, lDay, lHour, lMin, lSec;
	
	DateTime dDate = FileInfo->GetModificationTime();
	Unit::SetMjd(dDate,lYear, lMonth, lDay, lHour, lMin, lSec); 
	sprintf(m_cDate, "%02ld.%02ld.%04ld", lDay,lMonth, lYear);
	
	sprintf(m_cTime,"%02d:%02d",lHour,lMin);
	*/
	std::time_t t = boost::filesystem::last_write_time(*FileInfo);
	struct tm* tt = localtime(&t);
	
	sprintf(m_cDate, "%02ld.%02ld.%04ld", tt->tm_mday ,tt->tm_mon +1, tt->tm_year + 1900);
	sprintf(m_cTime,"%02d:%02d",tt->tm_hour, tt->tm_min);
}
// ****************************************************************************************
void DCP::CircleFileFunc::ParseFileNameAndPath(boost::filesystem::path* FileInfo)
{
	StringC strFileNameAll = FileInfo->filename().c_str();
	//UTL::UnicodeToAscii(m_cFileName, strFileNameAll);
	BSS::UTI::BSS_UTI_WCharToAscii(strFileNameAll, m_cFileName);
	StringC strFileName = strFileNameAll.Mid(0,strFileNameAll.GetLength()-4);

	strcat(m_cPathAndFileName,m_cPath);
	strcat(m_cPathAndFileName,m_cFileName);

	m_lSize = (uint32_t) boost::filesystem::file_size(*FileInfo);//FileInfo->GetSize();
}

// ****************************************************************************************
char* DCP::CircleFileFunc::getFileName()
{
	return m_cFileName;
}
// ****************************************************************************************
char* DCP::CircleFileFunc::getModDate()
{
	return m_cDate;
}
// ****************************************************************************************
char* DCP::CircleFileFunc::getModTime()
{
	return m_cTime;
}

// ****************************************************************************************
char* DCP::CircleFileFunc::getFileSizeString()
{
	sprintf(m_cSize,"%ld",m_lSize);
	return m_cSize;
}

// ****************************************************************************************
bool DCP::CircleFileFunc::isExists()
{
	return m_bExists;
}


// ****************************************************************************************
void DCP::CircleFileFunc::CloseFile()
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
short DCP::CircleFileFunc::open_crl_file_name(const char* mode)
{
short ret;
int Result;
	
	if(!m_pCommon->card_status())//(1) != 0)
		return -1;

	MsgBox msgbox;
	
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
		boost::filesystem::path FileInfo;

		char temp[CPI::LEN_PATH_MAX];
		temp[0] = '\0';
		strcat(temp, m_cPath);
		strcat(temp, m_cFileName);
		char* pSearch = &temp[0];

		if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
		{
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);//FileInfo->GetSize();		
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
short DCP::CircleFileFunc::fopen1(const char* mode)
{
	m_pFile = fopen(m_cPathAndFileName, mode);	
	
	if(m_pFile)
	{
		return true;		
	}
	MsgBox msgbox;
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
short DCP::CircleFileFunc::close_crl_file()
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
short DCP::CircleFileFunc::remove1(char *fname)
{
char apu[CPI::LEN_PATH_MAX];
bool Result;
	Common common(m_pModel);

    sprintf(apu,"%s%-s",m_cPath,common.strbtrim(fname));
	
	// VIVA
	//Result = CPI::FileUtilitiesC::RemoveFile(StringC(apu));
	boost::filesystem::path f = apu;
	Result = boost::filesystem::remove(f);
	//Result = remove(apu);

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

short DCP::CircleFileFunc::save_circle_to_file(StringC Id, CircleModel* pModel)
{
char temp1[100];
StringC msg;
MsgBox msgbox;
int  iInstNo=0;

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
	BSS::UTI::BSS_UTI_WCharToAscii(Id, temp );

	sprintf(temp1,"\nCircleIId:%s%c%c", temp,13,10); 
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

	sprintf(temp1,"Center point:%c%c",13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  x:   %9.*f%c%c",m_pModel->m_nDecimals,pModel->cx,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  y:   %9.*f%c%c",m_pModel->m_nDecimals,pModel->cy,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  z:   %9.*f%c%c",m_pModel->m_nDecimals,pModel->cz,13,10);
	fputs(temp1,m_pFile);

	/*
	sprintf(temp1,"Center point(x,y,z):%9.*f %9.*f %9.*f%c%c", m_pModel->m_nDecimals,pModel->cx, 
													m_pModel->m_nDecimals,pModel->cy, 
													m_pModel->m_nDecimals,pModel->cz,13,10); 
	
	fputs(temp1,m_pFile);
	*/

	sprintf(temp1,"Radius:%9.*f%c%c", m_pModel->m_nDecimals,pModel->diameter,13,10); 
	fputs(temp1,m_pFile);

	sprintf(temp1,"Normal:%c%c",13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  i:   %9.*f%c%c",6,pModel->vi,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  j:   %9.*f%c%c",6,pModel->vj,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"  k:   %9.*f%c%c",6,pModel->vk,13,10);
	fputs(temp1,m_pFile);

	sprintf(temp1,"RMS:   %9.*f%c%c", m_pModel->m_nDecimals,pModel->rms_diameter,13,10); 
	fputs(temp1,m_pFile);

	sprintf(temp1,"Deviations of points%c%c",13,10); 
	fputs(temp1,m_pFile);
	
	short sta;
	short count=0;
	for(int i =0; i < MAX_POINTS_IN_CIRCLE; i++)
	{
		sta = pModel->circle_points[0].points[i].sta;
		
		if(sta == 1 || sta == 2) // measured or design
		{
			count++;
			sprintf(temp1,"%-2d.%-6.6s %+9.*f%c%c", count,pModel->circle_points[0].points[i].point_id, 
									m_pModel->m_nDecimals, pModel->circle_points[0].points[i].diameter - pModel->circle_points[0].diameter,
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
short DCP::CircleFileFunc::delete_file(void)
{
	MsgBox msgbox;
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
short DCP::CircleFileFunc::create_new_file(char* filename)
{
char fname[13];
short ret;
StringC msg;
MsgBox msgbox;
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

		strcat(fname, ".crl");
		
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
		
		ret = create_new_circlefile(m_cPathAndFileName);

	    //strcpy(CalcDistFile_->name, fname);
	//}

	open_file();
	return 1;
}

/**************************************************************
**************************************************************/
short DCP::CircleFileFunc::access1(char *fname)
{
int attr = 0;

	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

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
short DCP::CircleFileFunc::open_file()
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
		boost::filesystem::path FileInfo;

		char temp[CPI::LEN_PATH_MAX];
		temp[0] = '\0';
		//strcat(temp, m_cPath);
		//strcat(temp, m_cFileName);
		strcat(temp, m_cPathAndFileName);
		char* pSearch = &temp[0];

		if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)  
		//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
		{
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);//FileInfo->GetSize();		
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
short DCP::CircleFileFunc::create_new_circlefile(char  *fname)
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