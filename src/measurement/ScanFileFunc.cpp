//================================================================================================
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
#include <dcp06/measurement/ScanFileFunc.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include "calc.h"
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG


DCP::ScanFileFunc::ScanFileFunc(DCP06ModelC* pDCP06Model): m_pFile(0), points(0), m_bExists(false),opened(0),file_updated(0),m_pDCP06Model(pDCP06Model)
{
	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';

	m_pCommon = new DCP06CommonC(m_pDCP06Model);

	// get path
	getPath();
}

// ****************************************************************************************
            // Description: Destructor
DCP::ScanFileFunc::~ScanFileFunc()
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
bool DCP::ScanFileFunc::setFile(const char* filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path  FileInfo;

	m_cPathAndFileName[0] = '\0';

	
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	//CPI::SensorC::GetInstance()->
	
	char temp[CPI::LEN_PATH_MAX];
	char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename);
	m_pCommon->strlower(temp_name);
	m_pCommon->strbtrim(temp_name);
	if(m_pCommon->strblank(temp_name))
		return false;
	
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp,temp_name);
	
	if(!strstr(temp,"."))
	{
		strcat(temp,".scn");
	}
	/*
	if(!strstr(temp,".adf"))
		strcat(temp,".adf");
	*/
	char* pSearch = &temp[0];

	if(m_pCommon->find_first_file(pSearch, &FileInfo) == 0)
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		ParseFileNameAndPath(&FileInfo);
		ParseModTime(&FileInfo);
		open_ADF_file_name("rb+");
		points = ReadPointsCount(m_cPathAndFileName);
		m_bExists = true;
		return true;
	}
	
	m_bExists = false;
	return false;
}

// ****************************************************************************************
bool DCP::ScanFileFunc::setFile(StringC filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;
	char filename_temp[FILENAME_BUFF_LEN];
	//UTL::UnicodeToAscii(filename_temp, filename);
	BSS::UTI::BSS_UTI_WCharToAscii(filename, filename_temp);
	m_pCommon->strbtrim(filename_temp);
	m_pCommon->strlower(filename_temp);
	
	CloseFile();
	
	if(m_pCommon->strblank(filename_temp))
		return false;

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
		char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename_temp);
	
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp, temp_name);

	if(!strstr(temp,"."))
	{
		strcat(temp,".scn");
	}
	/*
	if(!strstr(temp,".adf"))
		strcat(temp,".adf");
	*/
	char* pSearch = &temp[0];

	int rr = m_pCommon->find_first_file(pSearch, &FileInfo);
	DCP06MsgBoxC msgbox;
	// for test
	/*
	StringC ss;
	ss.Format(L"ret=%d (",rr);
	ss += StringC(temp);
	msgbox.ShowMessageOk(ss);
	*/
	if(rr == 0)
	//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		ParseFileNameAndPath(&FileInfo);
		ParseModTime(&FileInfo);
		open_ADF_file_name("rb+");
		points = ReadPointsCount(m_cPathAndFileName);
		m_bExists = true;
		/*
		DCP06MsgBoxC msgbox;
		msgbox.ShowMessageOk(L"File opened!");
		*/
		return true;
	}
	else
	{	
		/*
		DCP06MsgBoxC msgbox;
		msgbox.ShowMessageOk(StringC(temp));
		*/
	}
	m_bExists = false;
	return false;
}

// ****************************************************************************************
void DCP::ScanFileFunc::getPath()
{
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);

	//boost::filesystem::
	//CPI::FileUtilitiesC::MakeDir(m_cPath);
	boost::filesystem::path filePath= m_cPath;
    boost::system::error_code errCode;
	boost::filesystem::create_directory(filePath, errCode);
	
}

// ****************************************************************************************
void DCP::ScanFileFunc::ParseModTime(boost::filesystem::path* FileInfo)
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
void DCP::ScanFileFunc::ParseFileNameAndPath(boost::filesystem::path* FileInfo)
{
	StringC strFileNameAll = FileInfo->filename().c_str();
	BSS::UTI::BSS_UTI_WCharToAscii(strFileNameAll, m_cFileName);

	//StringC strFileNameAll = FileInfo->GetName();
	//UTL::UnicodeToAscii(m_cFileName, strFileNameAll);
	m_pCommon->strlower(m_cFileName);


	StringC strFileName = strFileNameAll.Mid(0,strFileNameAll.GetLength()-4);

	strcat(m_cPathAndFileName,m_cPath);
	strcat(m_cPathAndFileName,m_cFileName);

	m_lSize = (uint32_t) boost::filesystem::file_size(*FileInfo);//FileInfo->GetSize();
}
// ****************************************************************************************
char* DCP::ScanFileFunc::getFileName()
{
	return m_cFileName;
}
// ****************************************************************************************
char* DCP::ScanFileFunc::getModDate()
{
	return m_cDate;
}
// ****************************************************************************************
char* DCP::ScanFileFunc::getModTime()
{
	return m_cTime;
}

// ****************************************************************************************
char* DCP::ScanFileFunc::getPointsCountString()
{	
	if(strstr(m_cFileName,".ref") /*|| strstr(m_cFileName,".REF")*/)
		sprintf(m_cPointsCount,"%s"," ");
	else
		sprintf(m_cPointsCount,"%d",points);
	return m_cPointsCount;
}
// ****************************************************************************************
char* DCP::ScanFileFunc::getFileSizeString()
{
	sprintf(m_cSize,"%ld",m_lSize);
	return m_cSize;
}
// ****************************************************************************************
short DCP::ScanFileFunc::getPointsCount()
{	
	return points;
}
// ****************************************************************************************
bool DCP::ScanFileFunc::isExists()
{
	return m_bExists;
}

// ****************************************************************************************
void DCP::ScanFileFunc::CloseFile()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	opened = 0;
	points = 0;
	file_updated = 0;
	m_bExists = false;
	m_cPathAndFileName[0] = '\0';
	m_cFileName[0] = '\0';
	m_lSize = 0;
}

// ****************************************************************************************
short DCP::ScanFileFunc::ReadPointsCount(char* m_cPathAndFileName)
{
	/*
	FILE* m_pFile = fopen(m_cPathAndFileName,"rb+");
	if(m_pFile)
	{
		char trow[82];
		fseek(m_pFile,0L,SEEK_SET);
		fgets(trow,82,m_pFile);
		fgets(trow,82,m_pFile);
		fgets(trow,82,m_pFile);
		fgets(trow,82,m_pFile);
		fseek(m_pFile,0L,SEEK_SET);
		fread(trow,81,1,m_pFile);
		fread(trow,81,1,m_pFile);
		fread(trow,81,1,m_pFile);
		fread(trow,81,1,m_pFile);
		trow[81] = '\0';
		reclen = strlen(trow);
		points = (int) m_lSize / (reclen) - 3;
		m_bExists = true;
		fclose(m_pFile);
		m_pFile = 0;
	}
	*/
	return points;
}



// ****************************************************************************************
short DCP::ScanFileFunc::open_ADF_file_name(const char* mode)
{
short ret;
//FIL_DESCRIPTOR fp;
//FIL_DSTAT finfo;
//int Result;
//unsigned short w;
	
	if(!m_pCommon->card_status())//(1) != 0)
		return -1;

	DCP06MsgBoxC msgbox;
	
	ret = -1;

	if(m_pFile)
		close_adf_file();

	if(!fopen1(mode))// /*FIL_ACC_RDWR */,&fp) != TRUE)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP06,M_DCP_FILE_OPEN_ERROR_TOK);
		msg.Format(msg,(const wchar_t*)StringC(m_cPathAndFileName/*m_cFileName*/));
		msgbox.ShowMessageOk(msg);
	}
	else
		ret = 1;

	if(ret != 1)
	{
		close_adf_file();
	}

	return  ret;
}

/***********************************************************************
	FUNCTION   :int close_adf_file();
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::ScanFileFunc::close_adf_file()
{

	if(m_pFile)
	{
		if(!m_pCommon->card_status())//(1) != 0)
			return false;

		fflush(m_pFile);
		fclose(m_pFile);
	}
	points = 0;
	m_pFile = 0;
	opened = 0;

	return 1; 
}

/**************************************************************
**************************************************************/
short DCP::ScanFileFunc::fopen1(const char* mode)
{
	
	DCP06MsgBoxC msgbox;
	//StringC sT;
	//sT = L"fopen1  ";
	//sT += StringC(m_cPathAndFileName);
	m_pFile = fopen(m_cPathAndFileName, mode);	
	//msgbox.ShowMessageOk(sT);
	if(m_pFile)
	{
		return true;		
	}
	StringC msg;
	msg.LoadTxt(AT_DCP06,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(m_cPathAndFileName));
			msgbox.ShowMessageOk(msg);
	return false;
}

/**************************************************************
**************************************************************/
FILE* DCP::ScanFileFunc::fopen2(FILE *pFile , char* fname, const char* mode)
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
	DCP06MsgBoxC msgbox;
	StringC msg;
	msg.LoadTxt(AT_DCP06,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(temp));
			msgbox.ShowMessageOk(msg);

	return 0;
}



/***********************************************************************
	FUNCTION   :
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::ScanFileFunc::create_adf_file(char *fname, bool showOKMessage)
{
//unsigned long new_file_size;
char filename[FILENAME_BUFF_LEN];
DCP06MsgBoxC msgbox;		
		
	if(!m_pCommon->card_status())//card_status(1) != 0)
			return -1;

	sprintf(filename,"%s.scn",fname);

	CloseFile();
	sprintf(m_cPathAndFileName,"%-s%-s",m_cPath,filename);
		
		if(access1(filename) == 1)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_DELETE_OLD_FILE_TOK);
			msg.Format(msg,StringC(filename));
			if(!msgbox.ShowMessageYesNo(msg))
				return -1;

			if(remove1(filename) != 0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_DELETE_FILE_TOK);
				msgbox.ShowMessageOk(msg);
				
				return -1;
			}
		}
		if(fopen1("wb+") == false)
		{
			remove1(filename);
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg,(const wchar_t*) StringC(filename));
			msgbox.ShowMessageOk(msg);
			//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK,(void *) filename,MB_OK);
			return -1;
		}
		

		fflush(m_pFile);
		fclose(m_pFile);
		m_pFile = 0;
		
		if(showOKMessage)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_CREATE_3DF_OK_TOK);
			msgbox.ShowMessageOk(msg);
		}

		return 0;
}

/**************************************************************
**************************************************************/
short  DCP::ScanFileFunc::access1(char *fname)
{
//char apu[100];
int attr = 0;

	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

	//char filename_temp[20];
	//UTL::UnicodeToAscii(filename_temp, fname);

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP06Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
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



//*****************************************************************************
short  DCP::ScanFileFunc::remove1(char *fname)
{
char apu[CPI::LEN_PATH_MAX];
bool Result;
	DCP06CommonC common(m_pDCP06Model);

    sprintf(apu,"%s%-s",m_cPath,common.strbtrim(fname));
	boost::filesystem::path f = apu;
	Result = boost::filesystem::remove(f);
	if(Result)
	{
		return 0;
	}
	//int y = errno;
	return -1;

}


/***********************************************************************
	FUNCTION   :int delete_mnf_file();
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::ScanFileFunc::delete_adf_file(bool showMsg)
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


	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_FILE_TOK);
	msg.Format(msg,(const wchar_t*)StringC(m_cFileName));
	
	if(!showMsg || (showMsg && msgbox.ShowMessageYesNo(msg)))
	{
		strcpy(Temp, m_cFileName);
		close_adf_file();
		result = remove1(Temp);
		if(result == -1)
		{
			if(showMsg)
			{
				msg.LoadTxt(AT_DCP06,M_DCP_CANNOT_DELETE_FILE_TOK);
				msgbox.ShowMessageOk(msg);
			}
			ret = false;
		}
	}
	return ret;
}

// *****************************************************************************************
//
// *****************************************************************************************
short DCP::ScanFileFunc::add_new_pnt(char *pid, char *xact,char *yact,char *zact) 
{
char bXact[15], bYact[15], bZact[15];
int Result;

	DCP06MsgBoxC msgbox;

	if(!m_pCommon->card_status())
		return false;

	if(!m_pCommon->check_free_space(30000L))
		return false;
  
		Result = fseek(m_pFile, 0, SEEK_END);
		//Result = FIL_Seek(fp, 0L, FIL_FROMEND);
		
		if(Result == 0)
		{
			sprintf(bXact,"%-9.9s", " ");
			sprintf(bYact,"%-9.9s", " ");
			sprintf(bZact,"%-9.9s", " ");
			
			if(xact != NULL)
			{
				sprintf(bXact,"%s", m_pCommon->strbtrim(xact));	
			}
			if(yact != NULL)
			{
				sprintf(bYact,"%s", m_pCommon->strbtrim(yact));	
			}

			if(zact != NULL)
			{
				sprintf(bZact,"%s", m_pCommon->strbtrim(zact));	
			}
			
			sprintf(trow,"%-s,%s,%s,%s%c%c",
			pid,
			bXact,
			bYact,
			bZact,
			13,10);
		
			Result = fputs(trow,m_pFile);
		}

		if(Result != EOF)
		{
			Result = fflush(m_pFile);
		}

		if(Result != EOF)
		{
			return true;	
		}
		else
		{
			return false;
		}
}


// ***********************************************************************
// ***********************************************************************
FILE* DCP::ScanFileFunc::get_file_pointer()
{
	return m_pFile;
}
