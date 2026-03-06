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
#include <dcp06/file/DistFile.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/calculation/CalculationView.hpp>

#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
//#include <CPI_FileUtilities.hpp>


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
DistFileDialog::DistFileDialog(DistFileModel* pModel):
	m_pFile(0), m_pNumberOfPoints(0), m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),
	m_pDataModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}
// Description: Destructor
DistFileDialog::~DistFileDialog()
{

}

void DistFileDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	
	//InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_DISTANCE_FILE_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pFile);

	m_pNumberOfPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNumberOfPoints->SetId(eNumberOfPoints);
	m_pNumberOfPoints->SetText(StringC(AT_DCP06,P_DCP_NUMBER_OF_POINTS_TOK));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pNumberOfPoints);

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
	
	//SetHelpTok(H_DCP_DIST_FILE_TOK, 0);
}

void DistFileDialog::OnDialogActivated()
{
	if(!GetModel()->sCalcDistFile.IsEmpty())
		m_pDataModel->m_pFileFunc->setFile(GetModel()->sCalcDistFile);

	RefreshControls();
}

// Description: refresh all controls
void DCP::DistFileDialog::RefreshControls()
{	
	if(m_pFile && m_pNumberOfPoints && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		//m_pFileModel->m_pAdfFile->setFile(sSelectedFile/*GetModel()->ADFFileName*/);

		if(m_pDataModel->m_pFileFunc->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getFileName());//GetModel()->ADFFileName);	
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getPointsCountString());
			m_pSize->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getFileSizeString());
			m_pDate->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getModDate());
			m_pTime->GetStringInputCtrl()->SetString(m_pDataModel->m_pFileFunc->getModTime());
		}
		else
		{
			m_pFile->GetStringInputCtrl()->SetString(L" ");
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(L" ");
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

void DCP::DistFileDialog::UpdateData()
{
	if(m_pDataModel->m_pFileFunc->IsOpen())
	{	
        GetModel()->sCalcDistFile = m_pDataModel->m_pFileFunc->getFileName();
		GetModel()->sCalcDistFile.RTrim();
	}
	else
		GetModel()->sCalcDistFile = L"";
}


// Description: only accept DCP06 Model objects
bool DCP::DistFileDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::DistFileDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::DistFileController::DistFileController(Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DIST_FILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// FileModel
	m_pDataModel = new DCP::DistFileModel(m_pModel); 
 
    // Create a dialog
    m_pDlg = new DCP::DistFileDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( DIST_FILE_DLG, m_pDlg, true );

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


DCP::DistFileController::~DistFileController()
{

}

// Description: Route model to everybody else
bool DCP::DistFileController::SetModel( GUI::ModelC* pModel )
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
void DCP::DistFileController::OnF1Pressed()
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
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(CALCDIST_FILE, sTitle,m_pModel) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);

}

// NEW
void DCP::DistFileController::OnF2Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::InputTextModel* pModel = new InputTextModel;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_NEW_FILENAME_TOK);
		pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = 8;
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
void DCP::DistFileController::OnF5Pressed()
{	
	if(m_pDataModel->m_pFileFunc->IsOpen())
		m_pDataModel->m_pFileFunc->CloseFile();

	m_pDlg->RefreshControls();

}

// CONT
void DCP::DistFileController::OnF6Pressed()
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
void DCP::DistFileController::OnSHF2Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;
	m_pDataModel->m_pFileFunc->delete_file();
	m_pDlg->RefreshControls();

}

// VIEW
void DCP::DistFileController::OnSHF5Pressed()
{
	if(!m_pDataModel->m_pFileFunc->IsOpen())
		return;

	if(GetController(VIEWCDF_CONTROLLER) == nullptr)
	{
		(void)AddController( VIEWCDF_CONTROLLER, new DCP::ViewCdfController(m_pDataModel->m_pFileFunc, m_pDlg->GetModel()) );
	}
	(void)GetController( VIEWCDF_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(VIEWCDF_CONTROLLER, true);
}

// Description: React on close of tabbed dialog
void DCP::DistFileController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DistFileController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
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
			BSS::UTI::BSS_UTI_WCharToAscii( strNewFile, fname );
			
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
DCP::DistFileModel::DistFileModel(Model* pModel)
{
	m_pFileFunc = new CdfFileFunc(pModel);
	pCommon = new Common(pModel);

}
DCP::DistFileModel::~DistFileModel()
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

//DCP::CdfFileFunc::CdfFileFunc(): m_pFile(0), points(0), m_bExists(false),opened(0),file_updated(0)
//{
//	m_cPath[0] = '\0';
//	m_cPathAndFileName[0] = '\0';
//	m_cFileName[0] = '\0';
//
//	// get path
//	getPath();
//
//	m_pCommon = new Common();
//}

DCP::CdfFileFunc::CdfFileFunc(Model* pModel): m_pFile(0), points(0), m_bExists(false),opened(0),
				m_pModel(pModel),file_updated(0)
{
	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';

	m_pCommon = new Common(pModel);

	// get path
	getPath();
}
// ****************************************************************************************
DCP::CdfFileFunc::CdfFileFunc(boost::filesystem::path* FileInfo,Model* pModel):m_pFile(0), points(0), m_bExists(false),file_updated(0),m_pModel(pModel)
{
	m_pCommon = new Common(pModel);

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
DCP::CdfFileFunc::~CdfFileFunc()
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
void DCP::CdfFileFunc::getPath()
{
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pModel->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	//CPI::FileUtilitiesC::MakeDir(m_cPath);
	boost::filesystem::path filePath= m_cPath;
    boost::system::error_code errCode;
	boost::filesystem::create_directory(filePath, errCode);
}

// ****************************************************************************************
bool DCP::CdfFileFunc::setFile(StringC filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

	char filename_temp[FILENAME_BUFF_LEN];
	//UTL::UnicodeToAscii(filename_temp, filename);
	BSS::UTI::BSS_UTI_WCharToAscii( filename, filename_temp );
	
	m_pCommon->strbtrim(filename_temp);

	CloseFile();
	
	//bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::devicePcCard, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
	char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename_temp);
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp, temp_name);

	if(!strstr(temp,".cdf")/* || !strstr(temp,".CDF")*/)
		strcat(temp,".cdf");

	char* pSearch = &temp[0];

	if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)   
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		ParseFileNameAndPath(&FileInfo);
		ParseModTime(&FileInfo);
		open_cdf_file_name("rb+");
		//points = ReadPointsCount(m_cPathAndFileName);
		m_bExists = true;
		return true;
	}
	m_bExists = false;
	return false;
}
// ****************************************************************************************
void DCP::CdfFileFunc::ParseModTime(boost::filesystem::path* FileInfo)
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
void DCP::CdfFileFunc::ParseFileNameAndPath(boost::filesystem::path* FileInfo)
{
	StringC strFileNameAll = FileInfo->filename().c_str();
	//UTL::UnicodeToAscii(m_cFileName, strFileNameAll);
	BSS::UTI::BSS_UTI_WCharToAscii( strFileNameAll, m_cFileName );

	StringC strFileName = strFileNameAll.Mid(0,strFileNameAll.GetLength()-4);

	strcat(m_cPathAndFileName,m_cPath);
	strcat(m_cPathAndFileName,m_cFileName);

	m_lSize = (uint32_t) boost::filesystem::file_size(*FileInfo);//FileInfo->GetSize();
}

// ****************************************************************************************
char* DCP::CdfFileFunc::getFileName()
{
	return m_cFileName;
}
// ****************************************************************************************
char* DCP::CdfFileFunc::getModDate()
{
	return m_cDate;
}
// ****************************************************************************************
char* DCP::CdfFileFunc::getModTime()
{
	return m_cTime;
}

// ****************************************************************************************
char* DCP::CdfFileFunc::getPointsCountString()
{	
	sprintf(m_cPointsCount,"%d",points);
	return m_cPointsCount;
}
// ****************************************************************************************
char* DCP::CdfFileFunc::getFileSizeString()
{
	sprintf(m_cSize,"%ld",m_lSize);
	return m_cSize;
}
// ****************************************************************************************
short DCP::CdfFileFunc::getPointsCount()
{	
	return points;
}
// ****************************************************************************************
bool DCP::CdfFileFunc::isExists()
{
	return m_bExists;
}


// ****************************************************************************************
void DCP::CdfFileFunc::CloseFile()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	opened = 0;
	points = 0;
	m_bExists = false;
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';
	m_lSize = 0;
}

// ****************************************************************************************
short DCP::CdfFileFunc::open_cdf_file_name(const char* mode)
{
short ret;
int Result;
	
	if(!m_pCommon->card_status())//(1) != 0)
		return -1;

	MsgBox msgbox;
	
	ret = -1;

	if(m_pFile)
		close_cdf_file();

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
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);//FileInfo->GetSize();			
		}

		Result = fseek(m_pFile, 0L, SEEK_SET);
		fseek(m_pFile,0L, SEEK_END);

		Result = fseek(m_pFile, 0L, SEEK_SET);
		
		points = (short)(m_lSize/ ROW_LENGTH) -3;

		if( points <= 0) 
		{
			points = 0;
		}
		else
		{
			active_point = 1;
			calcdist_pnt(1);
		}
		opened = 1;
		ret = 1;
	}

	return  ret;
}

/**************************************************************
**************************************************************/
short DCP::CdfFileFunc::fopen1(const char* mode)
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
short DCP::CdfFileFunc::close_cdf_file()
{

	if(m_pFile)
	{
		if(!m_pCommon->card_status())//(1) != 0)
			return false;

		if(file_updated)
			add_header(1);
		//fclose(fstruct->f);
		fflush(m_pFile);
		fclose(m_pFile);
	}

		//strcpy(FormFile_.name,"");
	points = 0;
	active_point = 0;
	m_pFile = 0;
	opened = 0;

	return 1;
}

// ************************************************************************
// ************************************************************************
void DCP::CdfFileFunc::add_header(short Ind)
{
char linebuff[255];
char temp[100];
int year, day, month;
S_HEADER_INFO header_info;
int no;

		if(!opened)
		{
			return;
		}

		if(!m_pCommon->card_status()) //1) != 0)
		{
			return;
		}

		fseek(m_pFile,0L, SEEK_SET);

		sprintf(linebuff,"%-28s %-33s %-13.13s%c%c","CALCDIST",m_cFileName," ",13,10);	
		fputs(linebuff,m_pFile);

		// DATE
		m_pCommon->GetDate(&day, &month,&year);
		//CSV_GetDate(da);
		sprintf(header_info.date,"%02d/%02d/%4d", day, month, year);
	
		// USER
		m_pCommon->GetUserName(temp);
		m_pCommon->strbtrim(temp);
		sprintf(header_info.measurer,"%-10.10s", temp);
		
		// LAITTEEN SARJANUMERO
		m_pCommon->GetInstrumentNo(&no);
		sprintf(header_info.instrument_id,"%-lu", no);
		
		// LAITE
		m_pCommon->GetInstrumentName(temp);
		m_pCommon->strbtrim(temp);
		sprintf(header_info.instrument,"%-30.30s", temp);
			
		sprintf(header_info.tempair,"%-5.5s", " ");
		sprintf(header_info.tempobj,"%-5.5s", " ");

		sprintf(linebuff,"%-10.10s %-10.10s %-10.10s %-10.10s %32.32s%c%c\n",
																header_info.measurer,
																header_info.instrument,
																header_info.instrument_id,
																header_info.date,
																" ",
																13,10);
		
	
		fputs(linebuff,m_pFile);
}

/************************************************************************
*************************************************************************/
short DCP::CdfFileFunc::calcdist_pnt(short point)
{
int new_filpos,filpos;
int new_pntnum;
char trow[255];

	if(!m_pCommon->card_status())//card_status(1) != 0)
			return 0;

	if(opened)
	{
		new_pntnum = point;
		new_filpos = 3*ROW_LENGTH+(point-1)*ROW_LENGTH;

		filpos = new_filpos;
		active_point = (int) new_pntnum;
		
		fseek(m_pFile,filpos,SEEK_SET);

		fgets(trow, ROW_LENGTH,m_pFile); trow[ROW_LENGTH] = '\0';
		
		strncpy(id,		 trow+4		,6);
		strncpy(dist,		 trow+11	,10);
		strncpy(ref,		 trow+22	,10);
		strncpy(reftype, 	 trow+33	,10);
		strncpy(target,	 trow+44	,10);
		strncpy(targettype,trow+55	,10);
		strncpy(note,		 trow+66	,10);

		id[6] = '\0';
		dist[10]='\0'; 
		ref[10]='\0'; 
		reftype[10]='\0'; 
		target[10]='\0'; 
		targettype[10]='\0'; 
		note[10]='\0'; 

		return 1;
	}
	else
	{
		return 0;
	}
}
// *************************************************************************
//
// *************************************************************************
short DCP::CdfFileFunc::delete_point_from_cdf(short pno)
{
short i,j;
char out_name[40];
FILE* fp_out=0;
char buff[CPI::LEN_PATH_MAX];
char temp_pno[10];
StringC msg;		
MsgBox msgbox;
char temp[CPI::LEN_PATH_MAX];

		if(points <=1)
		{	
			msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_DELETE_POINT_TOK);
			msgbox.ShowMessageOk(msg);
			return 0;
		}	

		if(!m_pCommon->card_status() == 0)
		{
			if(m_pCommon->check_free_space(30000L))
			{			
		
				//CalcFile_out.open = -1;
				
				//sprintf(CalcFile_in.name,  "%-s",fstruct->name);
				sprintf(out_name, "%-s","temp.tmp");
				if(!(fp_out = fopen2(fp_out,out_name,"wb+")))
				{
					msg.LoadTxt(AT_DCP06,M_DCP_FILE_OPEN_ERROR_TOK);
					msg.Format(msg,(const wchar_t*)StringC(out_name));
					msgbox.ShowMessageOk(msg);
					return 0;
				}

				fseek(m_pFile, 0L,SEEK_SET);

				//FIL_Tell(fstruct->f,filpos);	
				
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);
				fgets(buff,ROW_LENGTH,m_pFile); buff[ROW_LENGTH] = '\0';
				fputs(buff,fp_out);

				//FIL_Tell(fstruct->f,filpos);	
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);
				fgets(buff,ROW_LENGTH,m_pFile); buff[ROW_LENGTH] = '\0';
				fputs(buff,fp_out);


				//FIL_Tell(fstruct->f,filpos);	
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);
				fgets(buff,ROW_LENGTH,m_pFile); buff[ROW_LENGTH] = '\0';
				fputs(buff,fp_out);


				j = 1;
				for(i=1;i<=points;i++)
				{
					//FIL_Tell(fstruct->f,filpos);
					//FIL_Read(fstruct->f, ROW_LENGTH,1,buff, w);buff[ROW_LENGTH] = '\0';
					fgets(buff,ROW_LENGTH,m_pFile); buff[ROW_LENGTH] = '\0';
					if(i != pno)
					{		
						sprintf(temp_pno,"%-3d",j);
						buff[0] = temp_pno[0];
						buff[1] = temp_pno[1];
						buff[2] = temp_pno[2];
						//FIL_Write(fp_out,ROW_LENGTH,1,buff, w);
						fputs(buff,fp_out);
						j++;
					}	
				}		
				if(fp_out) 
				{	
					fclose(fp_out);
					fp_out = 0;
				}
				//FIL_Close(fp_out);
				if(m_pFile)
				{	
					fclose(m_pFile);
					m_pFile = 0;
				}
		
				//FIL_Close(fp_out);
				//FIL_Close(fstruct->f);
				//remove1(fstruct->name);
				remove1(m_cFileName);
				sprintf(buff,"%s%s",m_cPath, m_cFileName);
				sprintf(temp,"%s%s",m_cPath, "temp.tmp");
				// VIVA
				//CPI::FileUtilitiesC::RenameFile(StringC(temp), StringC(buff));
				boost::filesystem::rename(temp, buff);
				//rename(temp,buff);

				//sprintf(buff,"a:\\%-s",fstruct->name);
				//Result = FIL_Rename("a:\\temp.tmp",buff);
				//PrintLn("Result=%x %d Buff=%-s",Result,Result,buff);
				//open_calcdist_file("rb+");	
				open_cdf_file_name("rb+");
				return 1;
			}
		}
		return 0;
}

 
// **************************************************************************************
//
// **************************************************************************************
short DCP::CdfFileFunc::get_next_id(char *did)
{
char temp[20];
int filpos;

		if(opened)
		{
			if(points == 0)
			{
				sprintf(did,"D%-5d", points+1);
			}
			else
			{
				fseek(m_pFile, 0L, SEEK_END);
				filpos = 3*ROW_LENGTH+(points-1)*ROW_LENGTH;

				fseek(m_pFile,filpos,SEEK_SET);

				fgets(trow,ROW_LENGTH,m_pFile);trow[ROW_LENGTH] = '\0';
				strncpy(temp,trow+4,6);
				temp[6] = '\0';
				m_pCommon->inc_id(temp);
				sprintf(did,"%-6.6s",temp);
			}
		}
		else
		{
			sprintf(did,"%-s","");
		}
		return 1;
}	

/**************************************************************
**************************************************************/
FILE* DCP::CdfFileFunc::fopen2(FILE *pFile , char* fname, const char* mode)
{
	char temp[CPI::LEN_PATH_MAX];
	temp[0] = '\0';

	strcat(temp, m_cPath);
	strcat(temp, fname);
	
	pFile = fopen(temp, mode);	
	
	if(pFile)
	{
		return pFile;		
	}
	MsgBox msgbox;
	StringC msg;
	msg.LoadTxt(AT_DCP06,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(temp));
			msgbox.ShowMessageOk(msg);

	return 0;
}


//*****************************************************************************
short DCP::CdfFileFunc::remove1(char *fname)
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

short DCP::CdfFileFunc::save_calcdist_to_file(char *dist,char *refname,char *reftype, char *trgname,char *trgttype, char *bDid, char *bNote,short askdid)
{
char temp1[100];
StringC msg;
MsgBox msgbox;

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

	//sprintf(temp,"%9s", " ");

	if(check_id(bDid, 1) == 0)
	{
			
		return 0;
	}

	fseek(m_pFile, 0L, SEEK_END);
	//FIL_Seek(fs->f, 0L, FIL_FROMEND);

	sprintf(temp1,"%-3d %-6.6s %10.10s %-10.10s %-10.10s %-10.10s %-10.10s %-10.10s%c%c",
																++points,
																bDid,
																dist,
																refname,
																reftype,
																trgname,
																trgttype,	
																bNote,
		
																13,10);
	if(fputs(temp1,m_pFile) == EOF)
	//if(FIL_Write(fs->f,ROW_LENGTH,1,temp1, w) != RC_OK)
	{
		 points--;
		 msg.LoadTxt(AT_DCP06,M_DCP_ERR_WRITE_TO_FILE_TOK);
		 msgbox.ShowMessageOk(msg);
		 return 0;
	}
	else
	{
		active_point = points;
		//Result = FIL_StartList(fs->name, &finfo);
							
		//fs->size = finfo.ulFsize;
		//fs->points = (short) (fs->size / ROW_LENGTH)-3;
		//FIL_EndList();
		//points++;
		file_updated = 1;
		return 1;
	}
}


// **********************************************************************************************
//
// **********************************************************************************************

//short cdf_save_pnt(CALCFILESTRUCT *CalcDistFile, char *dist,char *refname,char *reftype, char *trgname, char *trgttype,char *bDid, char *note)
short DCP::CdfFileFunc::cdf_save_pnt()
{
int new_filpos,filpos;
char temp[100]; 

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

	new_filpos = 3*ROW_LENGTH+(active_point-1) * ROW_LENGTH;

	filpos = new_filpos;

	fseek(m_pFile,filpos,SEEK_SET);

	sprintf(temp,"%-3d %-6.6s %-10.10s %-10.10s %-10.10s %-10.10s %-10.10s %-10.10s%c%c",	
																		active_point,
																		id,
																		dist,
																		ref,
																		reftype,
																		target,
																		targettype,
																		note,
																		13,10);
																		/*	
																		bDid,
																		dist,
																		refname,
																		reftype,
																		trgname,
																		trgttype,
																		note,	
																		13,10);
																		*/		
		//FIL_Write(CalcDistFile->f, ROW_LENGTH,1,temp,w);
		fputs(temp,m_pFile);
		fflush(m_pFile);
		file_updated = 1;
		return 1;
}
// ************************************************************************
// ************************************************************************
short DCP::CdfFileFunc::check_id(char *id, short Ind)
{
short ret = 1;
	/*
	while(is_id_exists(id, Ind) == 1)
	{
		sprintf(id,"%6.6s", " ");

	}
*/
	if(is_id_exists(id, Ind) == 1)
		ret = 0;

	return ret;
}


/************************************************************************
*************************************************************************/
short DCP::CdfFileFunc::is_id_exists(char *id, short Ind)
{
short ret = 0, i=0; 
char apu1[20],apu[20];
MsgBox msgbox;
StringC msg;

	if(!opened)
	{
		return 0;
	}

	if(!m_pCommon->card_status())
	{
		return 0;
	}

	if(m_pCommon->strblank(id))
		return 1;

	sprintf(apu,"%-s",id);
	m_pCommon->strlower(apu);
	m_pCommon->strbtrim(apu);

	fseek(m_pFile,0,SEEK_SET);
	i=0;
	//while(FIL_Read(fileptr->f, ROW_LENGTH,1,trow, w) == RC_OK)

	while(fgets(trow,ROW_LENGTH,m_pFile) != nullptr)
	{
		i++;

		if(i > 3)
		{
			sprintf(apu1,"%-6.6s",trow+4);
			m_pCommon->strbtrim(apu1);
			m_pCommon->strlower(apu1);

			if(strcmp(apu,apu1) == 0)
			{
				msg.LoadTxt(AT_DCP06,M_DCP_DIST_ID_IN_USE_TOK);

				msg.Format(msg,(const wchar_t*)StringC(apu1));
				msgbox.ShowMessageOk(msg);
				ret = 1;
				break;	
			}
		}
	}
	return ret;
}

/************************************************************************
*************************************************************************/
short DCP::CdfFileFunc::delete_file(void)
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
		close_cdf_file();
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
short DCP::CdfFileFunc::create_new_file(char* filename)
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

		strcat(fname, ".cdf");
		
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

		close_cdf_file();
		
		ret = create_new_calcdistfile(m_cPathAndFileName);

	    //strcpy(CalcDistFile_->name, fname);
	//}

	if(ret == 1)
		open_file(ROW_LENGTH);
	return ret;
}

/**************************************************************
**************************************************************/
short DCP::CdfFileFunc::access1(char *fname)
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
//
// ************************************************************************
short DCP::CdfFileFunc::create_new_calcdistfile(char  *fname)
{
short ret=0;
char temp[100];
	
	
	if(fopen1("wb+"))
	{
		//m_pFile = fp;

		sprintf(temp,"%-28s %-33s %-13.13s%c%c","CALCDIST", getFileName()/*fname*/," ",13,10);
		fputs(temp,m_pFile);//FIL_Write(CalcDistFile_->f,ROW_LENGTH,1,temp, w);

		sprintf(temp,"%-76.76s%c%c"," ",13,10);
		fputs(temp,m_pFile);//FIL_Write(CalcDistFile_->f,ROW_LENGTH,1,temp, w);

		sprintf(temp,"%-3s %-6s %10s %-10s %-10s %-10s %-10s %-10s%c%c","No", "Id", "Dist","RefId","RefType","TrgtId","TrgtType","Note",13,10);
		fputs(temp,m_pFile);//FIL_Write(CalcDistFile_->f,ROW_LENGTH,1,temp, w);
		ret = 1;
		fflush(m_pFile);
	}
	else
	{
	}

	fclose(m_pFile);
	m_pFile = 0;
	return ret;
}

// ************************************************************************
// ************************************************************************
short DCP::CdfFileFunc::open_file(short reclen)
{
	if(opened)
	{
		if(!m_pCommon->card_status())
			return 1;

		fclose(m_pFile);
	}

	if(fopen1("rb+"))
	{

		//CalcDistFile_->f = fp;		
	
		/* add header */

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
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);// FileInfo.GetSize();			
		}

		fseek(m_pFile, 0L, SEEK_SET);
		fseek(m_pFile,0L, SEEK_END);

		fseek(m_pFile, 0L, SEEK_SET);
		
		points = (short)(m_lSize/ ROW_LENGTH) -3;

		/*
		Result = FIL_StartList(CalcDistFile_->name, &finfo);
							
		CalcDistFile_->size = finfo.ulFsize;
		CalcDistFile_->time_ = finfo.unFtime;
		CalcDistFile_->date_ = finfo.unFdate;
		*/
		points = (short) (m_lSize / ROW_LENGTH)-3; //47->54

		//FIL_EndList();

		if(points < 0)
			points = 0;
		opened = 1;
		file_updated = 0;
		return 1;
	}
	else
	{
		//strcpy(CalcDistFile_->name, " ");
		points = 0;
		opened = 0;
		return 0;
	}
}


