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
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/file/DCP_AdfFileFunc.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include "calc.h"
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <boost/filesystem.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP05InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::AdfFileFunc::AdfFileFunc(ADF_TYPE type,DCP05ModelC* pDCP05Model): m_pFile(0), points(0), m_bExists(false),opened(0),file_updated(0),m_pDCP05Model(pDCP05Model)
{
	// get path
	adf_type = type;
	
	m_pCommon = new DCP05CommonC(pDCP05Model);

	getPath();
}

//DCP::AdfFileFunc::AdfFileFunc(DCP05ModelC* pDCP05Model): m_pFile(0), points(0), m_bExists(false),opened(0),file_updated(0),m_pDCP05Model(pDCP05Model)
//{
//	adf_type = ADF;
//	// get path
//	getPath();
//
//	m_pCommon = new DCP05CommonC(pDCP05Model);
//}

DCP::AdfFileFunc::AdfFileFunc(DCP05ModelC* pDCP05Model): m_pFile(0), points(0), m_bExists(false),opened(0),	m_pDCP05Model(pDCP05Model),file_updated(0)
{
	adf_type = ADF;
	
	m_pCommon = new DCP05CommonC(pDCP05Model);

	// get path
	getPath();
}
// ****************************************************************************************
/*
DCP::AdfFileFunc::AdfFileFunc(const char* filename, bool bCreate):m_pFile(0), points(0), m_bExists(false)
{
	CPI::FileIteratorC FileIterator;
	CPI::FileInfoC FileInfo;

	m_pCommon = new DCP05CommonC();

	m_cPathAndFileName[0] = '\0';
	m_cPath[0] = '\0';

	getPath();
		
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(CPI::devicePcCard, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
	strcat(temp, m_cPath);
	strcat(temp, filename);
	char* pSearch = &temp[0];

	if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
	{
		ParseFileNameAndPath(&FileInfo);
		
		ParseModTime(&FileInfo);

		points = ReadPointsCount(m_cPathAndFileName);
	}
}
*/
// ****************************************************************************************

DCP::AdfFileFunc::AdfFileFunc(boost::filesystem::path* FileInfo, DCP05ModelC* pDCP05Model):m_pFile(0), points(0), m_bExists(false),m_pDCP05Model(pDCP05Model)
{
	adf_type = ADF;

	m_pCommon = new DCP05CommonC(pDCP05Model);

	m_cPath[0] = '\0';
	m_cPathAndFileName[0] = '\0';

	getPath();
	
	ParseFileNameAndPath(FileInfo);
	ParseModTime(FileInfo);
	points = ReadPointsCount(m_cPathAndFileName);

}


// ****************************************************************************************
            // Description: Destructor
DCP::AdfFileFunc::~AdfFileFunc()
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
bool DCP::AdfFileFunc::setFile(const char* filename)
{
	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path  FileInfo;

	m_cPathAndFileName[0] = '\0';

	
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
	
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pCommon->m_pDCP05Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
	
	char temp[CPI::LEN_PATH_MAX];
	char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename);
	m_pCommon->strlower(temp_name);
	m_pCommon->strbtrim(temp_name);
	if(m_pCommon->strblank(temp_name))
		return false;
	
	if(strstr(temp_name,".ref") != NULL || strstr(temp_name,".REF") != NULL)
	{
		// Convert .Ref To .Adf
		if(!convert_ref_to_adf(temp_name) == true)
		{
			return false;
		}
	}
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp,temp_name);
	
	if(!strstr(temp,"."))
	{
		if(adf_type == STA)
			strcat(temp,".sta");
		else if (adf_type == SCN)
			strcat(temp,".scn");
		else if (adf_type == BFT)
			strcat(temp,".bft");
		else
			strcat(temp,".adf");
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
bool DCP::AdfFileFunc::setFile(StringC filename)
{
	//CPI::FileIteratorC FileIterator;
	//CPI::FileInfoC FileInfo;
	boost::filesystem::path FileInfo;

	char filename_temp[FILENAME_BUFF_LEN];
	//UTL::UnicodeToAscii(filename_temp, filename);
	BSS::UTI::BSS_UTI_WCharToAscii(filename, filename_temp);
	m_pCommon->strbtrim(filename_temp);
	m_pCommon->strlower(filename_temp);
	
	CloseFile();
	
	if(m_pCommon->strblank(filename_temp))
		return false;

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pCommon->m_pDCP05Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
	char temp[CPI::LEN_PATH_MAX];
		char temp_name[CPI::LEN_PATH_MAX];	
	
	sprintf(temp_name,"%s",filename_temp);
	if(strstr(temp_name,".ref") != NULL || strstr(temp_name,".REF") != NULL)
	{
		// Convert .Ref To .Adf
		if(!convert_ref_to_adf(temp_name) == true)
		{
			return false;
		}
	}
	temp[0] = '\0';
	strcat(temp, m_cPath);
	strcat(temp, temp_name);

	if(!strstr(temp,"."))
	{
		if(adf_type == STA)
			strcat(temp,".sta");
		else if (adf_type == SCN)
			strcat(temp,".scn");
		else if (adf_type == BFT)
			strcat(temp,".bft");
		else
			strcat(temp,".adf");
	}
	/*
	if(!strstr(temp,".adf"))
		strcat(temp,".adf");
	*/
	char* pSearch = &temp[0];

	//int rr = FileIterator.FindFirst(pSearch, FileInfo);
	int rr = m_pCommon->find_first_file(pSearch, &FileInfo);
	DCP05MsgBoxC msgbox;
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
		DCP05MsgBoxC msgbox;
		msgbox.ShowMessageOk(L"File opened!");
		*/
		return true;
	}
	else
	{	
		/*
		DCP05MsgBoxC msgbox;
		msgbox.ShowMessageOk(StringC(temp));
		*/
	}
	m_bExists = false;
	return false;
}

// ****************************************************************************************
void DCP::AdfFileFunc::getPath()
{
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pCommon->m_pDCP05Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
	//TODO EI OLE VIVASSA
	//CPI::SensorC::GetInstance()->MakeDir(m_cPath);
	//CPI::FileUtilitiesC::MakeDir(m_cPath);
	boost::filesystem::path filePath= m_cPath;
    boost::system::error_code errCode;
	boost::filesystem::create_directory(filePath, errCode);
}

// ****************************************************************************************
void DCP::AdfFileFunc::ParseModTime(boost::filesystem::path* FileInfo)
{
	//int lYear, lMonth, lDay, lHour, lMin, lSec;
	
	//DateTime dDate = FileInfo-GetModificationTime();
	std::time_t t = boost::filesystem::last_write_time(*FileInfo);
	struct tm* tt = localtime(&t);
	
	sprintf(m_cDate, "%02ld.%02ld.%04ld", tt->tm_mday ,tt->tm_mon +1, tt->tm_year + 1900);
	sprintf(m_cTime,"%02d:%02d",tt->tm_hour, tt->tm_min);

	//Unit::SetMjd(dDate,lYear, lMonth, lDay, lHour, lMin, lSec); 
	//sprintf(m_cDate, "%02ld.%02ld.%04ld", lDay,lMonth, lYear);
	//sprintf(m_cTime,"%02d:%02d",lHour,lMin);
}
// ****************************************************************************************
void DCP::AdfFileFunc::ParseFileNameAndPath(boost::filesystem::path* FileInfo)
{
	 
	StringC strFileNameAll = FileInfo->filename().c_str();
	//UTL::UnicodeToAscii(m_cFileName, strFileNameAll);
	BSS::UTI::BSS_UTI_WCharToAscii(strFileNameAll, m_cFileName);
	m_pCommon->strlower(m_cFileName);


	StringC strFileName = strFileNameAll.Mid(0,strFileNameAll.GetLength()-4);

	strcat(m_cPathAndFileName,m_cPath);
	strcat(m_cPathAndFileName,m_cFileName);
	
	m_lSize = (uint32_t) boost::filesystem::file_size(*FileInfo);//FileInfo->GetSize();
}
// ****************************************************************************************
char* DCP::AdfFileFunc::getFileName()
{
	return m_cFileName;
}
// ****************************************************************************************
char* DCP::AdfFileFunc::getModDate()
{
	return m_cDate;
}
// ****************************************************************************************
char* DCP::AdfFileFunc::getModTime()
{
	return m_cTime;
}

// ****************************************************************************************
char* DCP::AdfFileFunc::getPointsCountString()
{	
	if(strstr(m_cFileName,".ref") || strstr(m_cFileName,".REF"))
		sprintf(m_cPointsCount,"%s"," ");
	else
		sprintf(m_cPointsCount,"%d",points);
	return m_cPointsCount;
}
// ****************************************************************************************
char* DCP::AdfFileFunc::getFileSizeString()
{
	sprintf(m_cSize,"%ld",m_lSize);
	return m_cSize;
}
// ****************************************************************************************
short DCP::AdfFileFunc::getPointsCount()
{	
	return points;
}
// ****************************************************************************************
bool DCP::AdfFileFunc::isExists()
{
	return m_bExists;
}

// ****************************************************************************************
void DCP::AdfFileFunc::CloseFile()
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
short DCP::AdfFileFunc::ReadPointsCount(char* m_cPathAndFileName)
{
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
		reclen = (short) strlen(trow);
		points = (short) (m_lSize / (reclen) - 3);
		m_bExists = true;
		fclose(m_pFile);
		m_pFile = 0;
	}
	return points;
}


// ****************************************************************************************
short DCP::AdfFileFunc::select_pnt1(int newpnt, char *pid,char *xsta, char *xact, char *xdes,
													 char *ysta, char *yact, char *ydes,
													 char *zsta, char *zact, char *zdes,
													 char *note)
{
int new_pntnum=0l;

	if(m_pFile)
		{
			//form_save_pnt();
			new_pntnum = newpnt;

			if(new_pntnum < 1)new_pntnum= 1;
			if(new_pntnum > points) //-fstpnt)
				new_pntnum = points; //-fstpnt;

			form_pnt1(new_pntnum, pid, xsta,xact,xdes,ysta, yact, ydes, zsta, zact, zdes, note);
			return 1;
		}
		return 0;	
}

// ****************************************************************************************
short DCP::AdfFileFunc::form_pnt1 (	int point,
					char *pid,
					char *xsta, char *xact, char *xdes,
					char *ysta, char *yact, char *ydes,
					char *zsta, char *zact, char *zdes,
					char *note)
{
int new_filpos;
int new_pntnum;
char bPid[7], bXsta, bYsta, bZsta, bNote[7];
char bXmea[10], bYmea[10], bZmea[10];
char bXdes[10], bYdes[10], bZdes[10];
//unsigned short w;

	if(m_pFile)
	{
		new_pntnum = point;
		new_filpos = fstpnt*(reclen)+(point-1)*(reclen);

		filpos = new_filpos;
		//FormFile_.active_point = (int) new_pntnum;

		fseek(m_pFile,filpos, SEEK_SET);//FIL_SETPOS);
		
		fgets(trow,82,m_pFile);trow[81] = '\0';
		//fread(trow,81,1,m_pFile); trow[81] = '\0';

		strncpy(bPid,trow,6);

		bXsta = *(trow+7);
		strncpy(bXmea,trow+9	,9);
		strncpy(bXdes,trow+19	,9);

		bYsta = *(trow+29);
		strncpy(bYmea, trow+31	,9);
		strncpy(bYdes, trow+41	,9);

		bZsta = *(trow+51);
		strncpy(bZmea, trow+53	,9);
		strncpy(bZdes, trow+63	,9);

		strncpy(bNote, trow+73, 6);

		if(pid != NULL) { strncpy(pid,bPid,6);pid[6] = '\0';}

		if(xsta != NULL){ *xsta = bXsta;}
		if(xact != NULL){ strncpy(xact, bXmea,9);xact[9] = '\0';}
		if(xdes != NULL){ strncpy(xdes, bXdes,9);xdes[9] = '\0';}

		if(ysta != NULL){ *ysta = bYsta;}
		if(yact != NULL){ strncpy(yact, bYmea,9);yact[9] = '\0';}
		if(ydes != NULL){ strncpy(ydes, bYdes,9);ydes[9] = '\0';}

		if(zsta != NULL){ *zsta = bZsta;}

		if(zact != NULL){ strncpy(zact,bZmea,9);zact[9] = '\0';}
		if(zdes != NULL){ strncpy(zdes,bZdes,9);zdes[9] = '\0';}

		if(note != NULL){ strncpy(note, bNote,6);note[6] = '\0';}

		return 1;
	}
	return 0;
}
   
// ****************************************************************************************
short DCP::AdfFileFunc::open_ADF_file_name(const char* mode)
{
short ret;
//FIL_DESCRIPTOR fp;
//FIL_DSTAT finfo;
int Result;
//unsigned short w;
	
	if(!m_pCommon->card_status())//(1) != 0)
		return -1;

	DCP05MsgBoxC msgbox;
	
	ret = -1;

	if(m_pFile)
		close_adf_file();

	if(!fopen1(mode))// /*FIL_ACC_RDWR */,&fp) != TRUE)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP05,M_DCP_FILE_OPEN_ERROR_TOK);
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
			m_lSize = (uint32_t) boost::filesystem::file_size(FileInfo);// FileInfo.GetSize();			
		}

		Result = fseek(m_pFile, 0L, SEEK_SET);
		fseek(m_pFile,0L, SEEK_END);

		Result = fseek(m_pFile, 0L, SEEK_SET);
		
		fgets(trow,85,m_pFile); 
		trow[81] = '\0';
		//Result = fread(trow,80,1,m_pFile); trow[81] = '\0';
		
		reclen = /*(int)*/ (short)  strlen(trow);

		if(trow[0] == '[' && trow[1] == '*' && trow[2] == '-')
		{
			fstpnt = 30;
		}
		else
		{
			fstpnt = 3;
		}

		reclen = reclen; /* <cr><lf> */
			points = (short)(m_lSize/(reclen)-fstpnt);

		active_point_front = 1;

		fgets(trow,82,m_pFile); trow[81] = '\0';
		//fread(trow,81,1,m_pFile);	trow[81] = '\0';

		if(reclen == strlen(trow)) // +1)
		{
			fgets(trow,82,m_pFile); trow[81] = '\0';
			//fread(trow, 81,1,m_pFile);trow[81] = '\0';

			if(reclen == strlen(trow)) //+1)
			{
				Result = fseek(m_pFile,0L,SEEK_SET);
				filpos = ftell(m_pFile);
				
				fgets(trow,82,m_pFile); trow[81] = '\0';
				//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';
				filpos = ftell(m_pFile);

				fgets(trow,82,m_pFile); trow[81] = '\0';
				//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';
				filpos = ftell(m_pFile);
				fgets(trow,82,m_pFile); trow[81] = '\0';
				//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';
				filpos = ftell(m_pFile);
				//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';
				fgets(trow,82,m_pFile); trow[81] = '\0';

				active_point_front = 1;
				filpos = fstpnt*(reclen)+(active_point_front-1)*(reclen);

				fseek(m_pFile,filpos,SEEK_SET);
				fgets(trow,82,m_pFile); trow[81] = '\0';
				//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';
				opened = 1;
				
				form_pnt(1L); // select first point
		
				ret = 1;
			}
			else
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05, M_DCP_RECLENGTH_ERROR_TOK);
				msgbox.ShowMessageOk(msg);
			}
		}
		else
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05, M_DCP_RECLENGTH_ERROR_TOK);
			msgbox.ShowMessageOk(msg);
		}
	}

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
short DCP::AdfFileFunc::close_adf_file()
{

	if(m_pFile)
	{
		if(!m_pCommon->card_status())//(1) != 0)
			return false;
		
		//if(file_updated == 1)
		//	add_header_to_adf();	

		//fclose(fstruct->f);
		fflush(m_pFile);
		fclose(m_pFile);
	}

		//strcpy(FormFile_.name,"");
	points = 0;
	active_point_front = 0;
	active_point_back = 0;
	m_pFile = 0;
	opened = 0;

	return 1;
}

/**************************************************************
**************************************************************/
short DCP::AdfFileFunc::fopen1(const char* mode)
{
	
	DCP05MsgBoxC msgbox;
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
	msg.LoadTxt(AT_DCP05,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(m_cPathAndFileName));
			msgbox.ShowMessageOk(msg);
	return false;
}

/**************************************************************
**************************************************************/
FILE* DCP::AdfFileFunc::fopen2(FILE *pFile , char* fname, const char* mode)
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
	DCP05MsgBoxC msgbox;
	StringC msg;
	msg.LoadTxt(AT_DCP05,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(temp));
			msgbox.ShowMessageOk(msg);

	return 0;
}

/**************************************************************************************************
	FUNCTION  : int form_pnt(int nextprev);
	INPUT     :
	OUTPUT    :
**************************************************************************************************/
short DCP::AdfFileFunc::form_pnt(int point)
{
int new_filpos;
int new_pntnum,pair;
char bPid[7];//, bXsta, bYsta, bZsta, bNote[7];
char apu[10],s;
short pp;
int Result;

	if(opened)
	{
		if(!m_pCommon->card_status())//card_status(1) != 0)
			return 0;

		new_pntnum = point;
		new_filpos = fstpnt*(reclen)+(point-1)*(reclen);

		filpos = new_filpos;

		Result = fseek(m_pFile,filpos,SEEK_SET);

		fgets(trow,82,m_pFile); trow[81] = '\0';
		//Result = fread(trow,80,1,m_pFile);trow[81] = '\0';

		strncpy(bPid,trow,6);
		bPid[6] = '\0';
		strncpy(apu, bPid,6);
		apu[6] = '\0';

		// TSEKATAAN ONKO SINGLE, VAI MUU PISTE
		UTL::Trim(apu);
		//strbtrim(apu);
		pp = (short) strlen(apu);

		s = apu[pp-1];

		if((s == 'F' || s == 'f' || s == 'B' || s == 'b') && always_single==0)
		{
			point_type = 1;
			if(s=='F' || s == 'f')
			{
				cpoint = 1; // Front

				// *********************************
				// HAETAAN FRONT-PISTE BUFFERIIN
				// *********************************

				active_point_front = (short) new_pntnum;

				strncpy(pointid_front,bPid,6);
				pointid_front[6] = '\0';

				xsta_front = *(trow+7);

				strncpy(xmea_front, trow+9	,9);
				xmea_front[9] = '\0';

				strncpy(xdes_front, trow+19	,9);
				xdes_front[9] = '\0';

				ysta_front = *(trow+29);

				strncpy(ymea_front,trow+31	,9);
				ymea_front[9] = '\0';

				strncpy(ydes_front,trow+41	,9);
				ydes_front[9] = '\0';

				zsta_front = *(trow+51);

				strncpy(zmea_front, trow+53	,9);
				zmea_front[9] = '\0';

				strncpy(zdes_front, trow+63	,9);
				zdes_front[9] = '\0';

				strncpy(note_front, trow+73, 6);
				note_front[6] = '\0';


				// ETSITŽŽN PARI
				apu[pp-1] = 'B';
				if((pair = seek_pid(apu)) == 0)
				{

					//if(fseek(fstruct->f, 0L, SEEK_END)==0)
					if(fseek(m_pFile, 0L, SEEK_END)==0)
					{

						// Fill buffer with new record values
						sprintf(trow,"%-6.6s %c %-9s %-9s %c %-9s %-9s %c %-9s %-9s %c%c%c%c%c%c%c%c",

						apu,
							'_', " ",  " ",
							'_', " ",  " ",
							'_', " ",  " ",
							' ',' ',' ',' ',' ',' ',13,10);

						// Write it to file
						
						if(fputs(trow,m_pFile) != EOF)
						//if(fwrite(trow,81,1,m_pFile) == RC_OK) // != EOF)
						{
							file_updated = 1;
							// fflush(fstruct->f);
							fflush(m_pFile);

							// Inc points count
							pair = points = points +1;
						}
					}
				}

				new_pntnum = pair;
				new_filpos = fstpnt*(reclen)+(pair-1)*(reclen);

				filpos = new_filpos;

				fseek(m_pFile,filpos,SEEK_SET);
				
				fgets(trow,82,m_pFile); trow[81] = '\0'	;
				//fread(trow,80,1,m_pFile);trow[81] = '\0';

				strncpy(bPid,trow,6);
				bPid[6] = '\0';

				active_point_back = (short) pair;

				strncpy(pointid_back,bPid,6);
				pointid_back[6] = '\0';

				xsta_back = *(trow+7);

				strncpy(xmea_back,trow+9	,9);
				xmea_back[9] = '\0';

				strncpy(xdes_back, trow+19	,9);
				xdes_back[9] = '\0';

				ysta_back = *(trow+29);

				strncpy(ymea_back, trow+31	,9);
				ymea_back[9] = '\0';

				strncpy(ydes_back, trow+41	,9);
				ydes_back[9] = '\0';

				zsta_back = *(trow+51);

				strncpy(zmea_back,trow+53	,9);
				zmea_back[9] = '\0';

				strncpy(zdes_back,trow+63	,9);
				zdes_back[9] = '\0';

				strncpy(note_back,trow+73, 6);
				note_back[6] = '\0';
			}
			else
			{
				// *********************************
				// HAETAAN BACK-PISTE BUFFERIIN
				// *********************************

				cpoint = 2; //back

				active_point_back = (short) new_pntnum;

				strncpy(pointid_back,bPid,6);
				pointid_back[6] = '\0';

				xsta_back = *(trow+7);

				strncpy(xmea_back, trow+9	,9);
				xmea_back[9] = '\0';

				strncpy(xdes_back,trow+19	,9);
				xdes_back[9] = '\0';

				ysta_back = *(trow+29);

				strncpy(ymea_back,trow+31	,9);
				ymea_back[9] = '\0';

				strncpy(ydes_back,trow+41	,9);
				ydes_back[9] = '\0';

				zsta_back = *(trow+51);

				strncpy(zmea_back,trow+53	,9);
				zmea_back[9] = '\0';

				strncpy(zdes_back,trow+63	,9);
				zdes_back[9] = '\0';

				strncpy(note_back, trow+73, 6);
				note_back[6] = '\0';


				// ETSITŽŽN PARI
				apu[pp-1] = 'F';
				if((pair = seek_pid(apu)) == 0)
				{
					 if(fseek(m_pFile, 0L, SEEK_END)==0)
					//if(FIL_Seek(fstruct->f, 0L, FIL_FROMEND)==RC_OK)
					{

						// Fill buffer with new record values
						sprintf(trow,"%-6.6s %c %-9s %-9s %c %-9s %-9s %c %-9s %-9s %c%c%c%c%c%c%c%c",

						apu,
							'_', " ",  " ",
							'_', " ",  " ",
							'_', " ",  " ",
							' ',' ',' ',' ',' ',' ',13,10);

						// Write it to file
						if(fputs(trow,m_pFile) != EOF)
						//if(fwrite(trow,81,1,m_pFile) == RC_OK) // != EOF)
						{
							file_updated = 1;
							fflush(m_pFile);

							// Inc points count
							pair = points = points +1;
						}
					}
				}

				new_pntnum = pair;
				new_filpos = fstpnt*(reclen)+(pair-1)*(reclen);

				filpos = new_filpos;

				fseek(m_pFile,filpos,SEEK_SET);

				fgets(trow,82,m_pFile); trow[81] = '\0';
				//fread(trow,81,1,m_pFile);trow[81] = '\0';

				strncpy(bPid,trow,6);

				active_point_front = (short) pair;

				strncpy(pointid_front,bPid,6);
				pointid_front[6] = '\0';

				xsta_front = *(trow+7);

				strncpy(xmea_front, trow+9	,9);
				xmea_front[9] = '\0';

				strncpy(xdes_front,trow+19	,9);
				xdes_front[9] = '\0';

				ysta_front = *(trow+29);

				strncpy(ymea_front,trow+31	,9);
				ymea_front[9] = '\0';

				strncpy(ydes_front,trow+41	,9);
				ydes_front[9] = '\0';

				zsta_front = *(trow+51);

				strncpy(zmea_front, trow+53	,9);
				zmea_front[9] = '\0';

				strncpy(zdes_front,trow+63	,9);
				zdes_front[9] = '\0';

				strncpy(note_front,trow+73, 6);
				note_front[6] = '\0';
			}
		}
		else
		{
			// *****************
			// SINGLE POINT
			// *****************

			cpoint   = 1; // single
			point_type = 0;

			// ******************************
			// Tyhj„t„„n BACK-point bufferi
			// ******************************

			active_point_back = 0;
			m_pCommon->empty_xyz_buffers(xdes_back,
							  ydes_back,
							  zdes_back,
							  9);
			m_pCommon->empty_xyz_buffers(xmea_back,
							  ymea_back,
							  zmea_back,
							  9);
			m_pCommon->empty_buffer(note_back,9);
			m_pCommon->empty_buffer(pointid_back,6);

			xsta_back = ' ';
			ysta_back = ' ';
			zsta_back = ' ';

			// *********************************
			// HAETAAN PISTE BUFFERIIN
			// *********************************
			strncpy(pointid_front,bPid,6);
			pointid_front[6] = '\0';


			active_point_front = (short) new_pntnum;

			// X
			xsta_front = *(trow+7);

			strncpy(xmea_front,trow+9	,9);
			xmea_front[9] = '\0';

			strncpy(xdes_front, trow+19	,9);
			xdes_front[9] = '\0';

			// Y
			ysta_front = *(trow+29);

			strncpy(ymea_front, trow+31	,9);
			ymea_front[9]='\0';

			strncpy(ydes_front, trow+41	,9);
			ydes_front[9] = '\0';

			// Z
			zsta_front = *(trow+51);

			strncpy(zmea_front,trow+53	,9);
			zmea_front[9] = '\0';

			strncpy(zdes_front, trow+63	,9);
			zdes_front[9] = '\0';

			strncpy(note_front,trow+73, 6);
			note_front[6] = '\0';
		}
		return 1;
	}
	return 0;
}


/**************************************************************
**************************************************************/
short DCP::AdfFileFunc::seek_pid(char *pid)
{
short ret, pp;
char trow[0xFF],apu[7],apu2[7];

	ret = 0;
	pp=0;

	sprintf(apu,"%-6.6s",pid);
	apu[6] = '\0';
	
	m_pCommon->strlower(apu);

	// POJ DOstrlwr(apu);

	if(opened)
	{
		if(points > 0)
		{
			fseek(m_pFile,0L,SEEK_SET);

			fgets(trow,82,m_pFile);trow[81] = '\0';
			fgets(trow,82,m_pFile);trow[81] = '\0';
			fgets(trow,82,m_pFile);trow[81] = '\0';

			while(fgets(trow,82,m_pFile) != NULL)
			{
				trow[81] = '\0';
				pp++;
				strncpy(apu2,trow,6);
				apu2[6] = '\0';
				m_pCommon->strlower(apu2);
				// POJ DO_strlwr(apu2);
				//m_pCommon->strbtrim(apu);
				//m_pCommon->strbtrim(apu2);

				if(strncmp(apu, apu2,6) == 0)
				{
					ret = pp;
					break;
				}
			}
		 }
	}
	else
	{
		ret = 0;

	}

	return ret;
}


/**************************************************************
**************************************************************/
short DCP::AdfFileFunc::GetPointList(S_SELECT_POINTS* pList, short iMaxPoints, short iDef)
{
	short iCount = 0;

	if(m_pFile)
	{
		DCP05CommonC common(m_pDCP05Model);
		short i;
		bool mea,des;
		char pid[7];
		char xmea[15],ymea[15],zmea[15];
		char xdes[15],ydes[15],zdes[15];
		
		if(!m_pCommon->card_status())//card_status(1) != 0)
		{
			return 0;
		}
		
		//(void) MMI_DrawBusyField(ON);

		for(i=1; i <= points; i++)
		{
			if(i >= iMaxPoints)
				break;

			iCount++;
			select_pnt1((int) i, pid, NULL, xmea, xdes, NULL, ymea, ydes, NULL, zmea, zdes, NULL);

			mea=des=false;

			if(!common.strblank(xmea) && !common.strblank(ymea) && !common.strblank(zmea))
				mea = true;

			if(!common.strblank(xdes) && !common.strblank(ydes) && !common.strblank(zdes))
				des = true;
  
			pList[iCount-1].iId = i;
			sprintf(pList[iCount-1].point_id,"%-6.6s",pid);
			pList[iCount-1].bActualDefined = mea;
			pList[iCount-1].bDesignDefined = des;
			if(iDef == BOTH)
			 {
				pList[iCount-1].bDesignSelected = (des) ? true : false;
				pList[iCount-1].bActualSelected = (mea) ? true : false;

			// 	sprintf(Temp, "%-6.6s %c/%c ", pid,(mea==1)? actual_selected[0] /*'A'*/: '-', (des==1) ? design_selected[0]/*'D'*/: '-');
			 }
			 else if(iDef == ACTUAL)
			 {
				pList[iCount-1].bDesignSelected = false;
				pList[iCount-1].bActualSelected = (mea) ? true : false;

				// 050112
				if(!pList[iCount-1].bActualSelected)
					pList[iCount-1].bDesignSelected = (des) ? true : false;
					

					
			 //	sprintf(Temp, "%-6.6s %c/%c ", pid,actual_selected[0] /*'A'*/,(des==1) ? design_nonselected[0] /*'d'*/: '-');
			 }
			 else
			 {
					pList[iCount-1].bDesignSelected = (des) ? true : false;
					pList[iCount-1].bActualSelected = false;

					// 050112
					if(!pList[iCount-1].bDesignSelected)
						pList[iCount-1].bActualSelected = (mea) ? true : false;

			 }

			//if(!mea)
			//		pList[iCount-1].bActualSelected = false;*/

				 // 	sprintf(Temp, "%-6.6s %c/%c ", pid,(mea == 0) ? '-':actual_nonselected[0] /*'a'*/, (des==0) ? '-': design_selected[0] /*'D'*/);
			 
			
			// strncpy(szPointList[i-1],Temp,15);
			// szPointList[i-1][15] = '\0';
			
		}
		//(void) MMI_DrawBusyField(OFF);

	}
	return iCount;
}

/**************************************************************
**************************************************************/
short DCP::AdfFileFunc::GetPointList(S_SELECT_POINT* pList, short iMaxPoints)
{
	short iCount = 0;
	StringC sActualSelected,sDesignSelected;

	sActualSelected.LoadTxt(AT_DCP05,P_DCP_ACTUAL_SELECTED_TOK);
	sDesignSelected.LoadTxt(AT_DCP05,P_DCP_DESIGN_SELECTED_TOK);

    char cActSelected[10];
	char cDesSelected[10];

	//UTL::UnicodeToAscii(cActSelected, 2, sActualSelected);
	//UTL::UnicodeToAscii(cDesSelected, 2, sDesignSelected);
	
	BSS::UTI::BSS_UTI_WCharToAscii(sActualSelected, cActSelected,2);
	BSS::UTI::BSS_UTI_WCharToAscii(sDesignSelected, cDesSelected,2);

	
    
	if(m_pFile)
	{
		DCP05CommonC common(m_pDCP05Model);
		short i;
		bool mea,des;
		char pid[7];
		char xmea[15],ymea[15],zmea[15];
		char xdes[15],ydes[15],zdes[15];
		/*
		for(i=0;i<maxpoints;i++)
		{
			szPointList[i][0] = '\0';
		}
		*/
		
		if(!m_pCommon->card_status())//card_status(1) != 0)
		{
			return 0;
		}
		
		//(void) MMI_DrawBusyField(ON);

		for(i=1; i <= points; i++)
		{
			if(i >= iMaxPoints)
				break;

			iCount++;
			select_pnt1((int) i, pid, NULL, xmea, xdes, NULL, ymea, ydes, NULL, zmea, zdes, NULL);

			mea=des=false;

			if(!common.strblank(xmea) && !common.strblank(ymea) && !common.strblank(zmea))
				mea = true;

			if(!common.strblank(xdes) && !common.strblank(ydes) && !common.strblank(zdes))
				des = true;

			pList[iCount-1].no = i;
			sprintf(pList[iCount-1].point_id,"%-6.6s",pid);
			sprintf(pList[iCount-1].point_status,"%s/%s",(mea==1) ? cActSelected : "-", (des==1) ? cDesSelected : "-");
			
		}
		//(void) MMI_DrawBusyField(OFF);

	}
	return iCount;
}

/***********************************************************************
	FUNCTION   :
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::AdfFileFunc::create_adf_file(char *fname, char* pointid,bool showOKMessage)
{
unsigned int new_file_size;
char filename[FILENAME_BUFF_LEN];
DCP05MsgBoxC msgbox;		
		

	if(!m_pCommon->card_status())//card_status(1) != 0)
			return -1;

		if(adf_type == STA)
			sprintf(filename,"%s.sta",fname);
		else if (adf_type == SCN)
			sprintf(filename,"%s.scn",fname);
		else if (adf_type == BFT)
			sprintf(filename,"%s.bft",fname);
		else
			sprintf(filename,"%s.adf",fname);


		//free_disk_space =  free_space();
		new_file_size = (1+3) * 81L; // 80=>81
		/*
		if(check_free_space(new_file_size+30000L) == FALSE)
			return -1;
		*/
		CloseFile();
		sprintf(m_cPathAndFileName,"%-s%-s",m_cPath,filename);
		
		//msgbox.ShowMessageOk((const wchar_t*) StringC(m_cPathAndFileName));
		//if(fstruct->open == RC_OK)
		//close_adf_file(fstruct);

		if(access1(filename) == 1)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_DELETE_OLD_FILE_TOK);
			msg.Format(msg,StringC(filename));
			if(!msgbox.ShowMessageYesNo(msg))
				return -1;
			/*
			if(msgbox1(TXT_NIL_TOKEN,M_DELETE_OLD_FILE_TOK,(void *) filename, MB_YESNO) != TRUE)
				return -1;
			*/

			if(remove1(filename) != 0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_DELETE_FILE_TOK);
				msgbox.ShowMessageOk(msg);
				
				return -1;
			}
		}
		if(fopen1("wb+") == false)
		{
			remove1(filename);
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg,(const wchar_t*) StringC(filename));
			msgbox.ShowMessageOk(msg);
			//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK,(void *) filename,MB_OK);
			return -1;
		}
		
		/*******************************
			First point id
		********************************/
		/*
		sprintf(id,"%-6s", "P1");

		if(input_text(T_3DFILE_TOK,M_ENTER_FIRST_PID_TOK,emptyinfo,6,id)== FALSE)
		{
		}
		*/
		//sprintf(linebuff,"%-10.10s %19.19s %-9.9s %-9.9s %28.28s%c%c","3D-FORM",filename," ", " "," ",13,10); //_strdate(str_date),_strtime(str_time),"8",13,10);
		sprintf(linebuff,"%-10.10s %19.19s %-9.9s %-9.9s %28.28s%c%c","3D-FORM",filename," ", " "," ",13,10); //_strdate(str_date),_strtime(str_time),"8",13);
		//FIL_Write(fp, 81, 1, linebuff,w); 
		fputs(linebuff,m_pFile);
		
		// for header space
		//sprintf(linebuff,"1%-77.77s1%c%c", " ",13,10);
		sprintf(linebuff,"1%-77.77s1%c%c", " ",13,10);
		//FIL_Write(fp, 81, 1, linebuff,w); 
		fputs(linebuff,m_pFile);

		sprintf(linebuff,"%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %-6.6s%c%c", //%c",
					"PID", //"PID",
					'X', "X_ACTUAL","X_DESIGN", // "X_ACTUAL", "X_DESIGN",
					'Y', "Y_ACTUAL","Y_DESIGN", // "Y_ACTUAL", "Y_DESIGN",
					'Z', "Z_ACTUAL","Z_DESIGN", // "Z_ACTUAL", "Z_DESIGN",
					"NOTE",13,10);//,10); 
		fputs(linebuff,m_pFile);
		//FIL_Write(fp, 81, 1, linebuff,w); 

		sprintf(linebuff,
				"%-6.6s %c %-9.9s %-9.9s %c %-9.9s %-9.9s %c %-9.9s %-9.9s %-6.6s%c%c",//%c",
				pointid,
				'_', " ", " ", // X
				'_', " ", " ", // Y
				'_', " ", " ", // Z
				"      ",13,10);//,10);

		fputs(linebuff,m_pFile);
		//FIL_Write(fp, 81, 1, linebuff,w); 

		fflush(m_pFile);
		fclose(m_pFile);
		m_pFile = 0;
		
		if(showOKMessage)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_CREATE_3DF_OK_TOK);
			msgbox.ShowMessageOk(msg);
		}

		//msgbox(TXT_NIL_TOKEN,M_CREATE_3DF_OK_TOK,MB_OK);

		return 0;
}

/**************************************************************
**************************************************************/
short  DCP::AdfFileFunc::access1(char *fname)
{
//char apu[100];
int attr = 0;

	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

	//char filename_temp[20];
	//UTL::UnicodeToAscii(filename_temp, fname);
	
	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pCommon->m_pDCP05Model->FILE_STORAGE1, CPI::ftUserAscii, m_cPath);
	
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
short  DCP::AdfFileFunc::remove1(char *fname)
{
char apu[CPI::LEN_PATH_MAX];
bool Result;
	DCP05CommonC common(m_pDCP05Model);

    sprintf(apu,"%s%-s",m_cPath,common.strbtrim(fname));

	// VIVA
	boost::filesystem::path f = apu;
	Result = boost::filesystem::remove(f);
	//Result = CPI::FileUtilitiesC::RemoveFile(StringC(apu));
	// Result = remove(apu);

	if(Result)
	{
		return 0;
	}
	//int y = errno;
	return -1;

}

/************************************************************************
*************************************************************************/
short DCP::AdfFileFunc::copy_adf_file(char *to_fname)
{
int filpos;
char temp1[20],temp2[20];

		DCP05MsgBoxC msgbox;
		if(!m_pCommon->card_status()) ///*1*/) != 0)
			return false;
		
		// get disk free space
		if(m_pCommon->check_free_space(30000L) == false)
			return false;
		
		sprintf(temp1,"%-s",m_cFileName);
		sprintf(temp2,"%-s",to_fname);
	
		m_pCommon->strbtrim(temp1);
		m_pCommon->strbtrim(temp2);
		
		if(strcmp(temp1,temp2) == 0)
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_SAME_FILE_NAME_TOK);
			msgbox.ShowMessageOk(msg);
			//msgbox(TXT_NIL_TOKEN, M_SAME_NAME_TOK,MB_OK);
			return false;	
		}

		// Open new adf file
//		ffo = fopen1(to_fname,FIL_ACC_CREATE|FIL_ACC_RDWR, &ffo);
		FILE *ffo = 0;
		/*
		char temp[200];
		temp[0] = '\0';
		strcat(temp, m_cPath);
		strcat(temp, temp2);
		*/
		if(access1(to_fname/*temp2*/) == 1)
		{	
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_DELETE_OLD_FILE_TOK);
			msg.Format(msg,to_fname);
			if(!msgbox.ShowMessageYesNo(msg))
					return false;
		}
		if(!(ffo=fopen2(ffo,to_fname/*temp*/,"wb+")))
		{
			StringC msg;
			msg.LoadTxt(AT_DCP05,	M_DCP_FILE_OPEN_ERROR_TOK);
			msg.Format(msg, (const wchar_t*)StringC(to_fname));
			msgbox.ShowMessageOk(msg);
			//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK,(void *) to_fname,MB_OK);
			return false;
		}
		//reclen = reclen+1; /* <cr><lf> */
		//recnum = m_lSize / reclen;

		// Pointer to the file's beginning
		fseek(m_pFile,0L,SEEK_SET);
		fgets(trow,82,m_pFile); trow[81] = '\0';
		//FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';

//		reclen = strlen(trow) +1;

		// Pointer to the file's beginning
		fseek(m_pFile,0L,SEEK_SET);
		//FIL_Seek(fstruct->f,0L,FIL_SETPOS);

		// Change filename to the new one
		//FIL_Tell(fstruct->f, filpos);
		filpos = ftell(m_pFile);
		fgets(trow,82,m_pFile); trow[81] = '\0';
		//FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';

		sprintf(trow,"%-10s %19s %-9s %-9s %28s%c%c","3D-FORM",to_fname," ", " "," ",13,10);
		fputs(trow,ffo);
		//FIL_Write(ffo,81,1,trow,w);

		//FIL_Tell(fstruct->f, filpos);
		
		//FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		filpos = ftell(m_pFile);
		fgets(trow,82,m_pFile); trow[81] = '\0';
		//trow[79] = 10; 
		trow[81] = '\0';
		fputs(trow,ffo);
		//FIL_Write(ffo,81,1,trow,w);
		/*
		FIL_Tell(fstruct->f, filpos);
		FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		FIL_Write(ffo,81,1,trow,w);
		*/
		filpos = ftell(m_pFile);
		fgets(trow,82,m_pFile); trow[81] = '\0';
		//trow[79] = 10; 
		trow[81] = '\0';
		fputs(trow,ffo);

		do
		{
				filpos = ftell(m_pFile);
				fgets(trow,82,m_pFile); trow[81] = '\0';
				//trow[79] = 10; 
				trow[81] = '\0';
				fputs(trow,ffo);
				/*
				FIL_Tell(fstruct->f, filpos);
				FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		    	FIL_Write(ffo,81,1,trow,w);
				*/
			  filpos = filpos + reclen;
		    // FIL_Seek(fstruct->f,filpos,FIL_SETPOS);

	   } while(filpos <= (m_lSize - (reclen)));


		fclose(ffo);
		//FIL_Close(ffo);
		StringC msg;
		msg.LoadTxt(AT_DCP05,M_DCP_COPYING_OK_TOK);
		msgbox.ShowMessageOk(msg);
		//msgbox(TXT_NIL_TOKEN,M_COPYING_OK_TOK,MB_OK);

	 return true;
}

/***********************************************************************
	FUNCTION   :int swap_meas_nomi(char *txt);
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::AdfFileFunc::swap_meas_design()
{
	int pntnum;
	char x_sta, y_sta, z_sta, x_act[10], y_act[10], z_act[10];
	char x_des[10], y_des[10],z_des[10], Pid[7], Note[7];
	DCP05MsgBoxC msgbox;
	StringC msg;

	//
	//	          01234567890123456789
	if(!opened)
	{
		msg.LoadTxt(AT_DCP05,M_DCP_3DFILE_ISNOT_OPEN_TOK);
		msgbox.ShowMessageOk(msg);
		return 0;
	}

	if(!m_pCommon->read_allow_edit())
	{
		msg.LoadTxt(AT_DCP05, M_DCP_EDIT_NOT_ALLOWED_TOK);
		msgbox.ShowMessageOk(msg);
		return 0;
	}

	if(!m_pCommon->card_status())// (1) != 0)
		return 1;

	msg.LoadTxt(AT_DCP05,M_DCP_SWAP_FILE_TOK);
	if(msgbox.ShowMessageYesNo(msg))
	{
		fseek(m_pFile,0,SEEK_SET);
		fgets(trow,82,m_pFile);trow[81] = '\0';
		fgets(trow,82,m_pFile);trow[81] = '\0';
		fgets(trow,82,m_pFile);trow[81] = '\0';
		fgets(trow,82,m_pFile);trow[81] = '\0';
		/*
		FIL_Seek(fstruct->f,0L,FIL_SETPOS);
		FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';

		// reclen = strlen(trow) +1;

		FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
		*/
		pntnum = 1;

		filpos = fstpnt*(reclen)+(pntnum-1)*(reclen);

		do
		{
			fseek(m_pFile, filpos, SEEK_SET);
			fgets(trow,82,m_pFile);trow[81] = '\0';
			/*
			FIL_Seek(fstruct->f,filpos,FIL_SETPOS);
			FIL_Read(fstruct->f, 81,1,trow, w);trow[81] = '\0';
			*/
			strncpy(Pid,trow,6);
			Pid[6] = '\0';

			x_sta = *(trow+7);

			strncpy(x_act, trow+9	,9);
			x_act[9] = '\0';

			sprintf(x_des,"%9s", " ");

			x_des[9] = '\0';

			y_sta = *(trow+29);

			strncpy(y_act, trow+31	,9);
			y_act[9] = '\0';

			sprintf(y_des,"%9s", " ");

			y_des[9] = '\0';

			z_sta = *(trow+51);

			strncpy(z_act,trow+53	,9);
			z_act[9] = '\0';

			sprintf(z_des,"%9s", " ");

			z_des[9] = '\0';

			strncpy(Note, trow+73, 6);
			Note[6] = '\0';

		/* swap measured and nominal values */

			if(m_pCommon->strblank(x_des))
				x_sta = '_';
			else
				x_sta = 'X';

			if(m_pCommon->strblank(y_des))
				y_sta = '_';
			else
				y_sta = 'Y';

			if(m_pCommon->strblank(z_des))
				z_sta = '_';
			else
				z_sta = 'Z';


				sprintf(trow,"%6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %c%c%c%c%c%c%c%c",

				Pid,
				x_sta, x_des,  x_act,
				y_sta, y_des,  y_act,
				z_sta, z_des,  z_act,
				Note[0],
				Note[1],
				Note[2],
				Note[3],
				Note[4],
				Note[5],13,10);

				fseek(m_pFile,filpos,SEEK_SET);
				fputs(trow,m_pFile);
				/*
				FIL_Seek(fstruct->f,filpos,FIL_SETPOS);
		    	FIL_Write(fstruct->f,81,1,trow,w);
					
				filpos = filpos + reclen;
				FIL_Seek(fstruct->f,filpos,FIL_SETPOS);
				*/
				filpos = filpos + reclen;
				fseek(m_pFile,filpos,SEEK_SET);


			} while(filpos <= (m_lSize-(reclen)));

		add_header_to_adf();	
		form_pnt(1L);

		msg.LoadTxt(AT_DCP05,M_DCP_SWAPPING_OK_TOK);
		msgbox.ShowMessageOk(msg);
	}
	return 1;
}


void DCP::AdfFileFunc::add_header_to_adf()
{
//struct DATE_TYPE da;
//unsigned short w;
char temp[100];
int no;
int year, day, month;
S_HEADER_INFO header_info;

		if(!opened)
		{
			return;
		}

		if(!m_pCommon->card_status()) //1) != 0)
		{
			return;
		}
		
		fseek(m_pFile,0L, SEEK_SET);
		//FIL_Seek(fstruct->f, 0L, FIL_SETPOS);

		sprintf(linebuff,"%-10.10s %19.19s %-9.9s %-9.9s %28.28s%c%c","3D_FORM",m_cFileName," ", " "," ",13,10);
		fputs(linebuff,m_pFile);
		//FIL_Write(fstruct->f, 81, 1, linebuff,w); 

		// PÄIVÄ
		m_pCommon->GetDate(&day, &month,&year);
		//CSV_GetDate(da);
		sprintf(header_info.date,"%02d/%02d/%4d", day, month, year);
		
		// USER
		m_pCommon->GetUserName(temp);
		//get_active_user(temp);
		m_pCommon->strbtrim(temp);
		sprintf(header_info.measurer,"%-10.10s", temp);

		// LAITTEEN SARJANUMERO
		m_pCommon->GetInstrumentNo(&no);
		//CSV_GetInstrumentNo(no);
		sprintf(header_info.instrument_id,"%-lu", no);
		
		// LAITE
		m_pCommon->GetInstrumentName(temp);
		//CSV_GetInstrumentName(temp);
		m_pCommon->strbtrim(temp);
		sprintf(header_info.instrument,"%-30.30s", temp);
		
		sprintf(header_info.tempair,"%-5.5s", " ");
		sprintf(header_info.tempobj,"%-5.5s", " ");
		
		sprintf(linebuff,"%-8.8s   %-10.10s %-10.10s %-10.10s %-5.5s %-5.5s %-10.10s %-12.12s%c%c", 
																header_info.measurer,
																header_info.instrument,
																header_info.instrument_id,
																header_info.date,
																header_info.tempair,
																header_info.tempobj,
																" ",
																" ",13,10);
		//FIL_Write(fstruct->f, 81, 1, linebuff,w); 
		fputs(linebuff,m_pFile);
 }


/***********************************************************************
	FUNCTION   :int delete_mnf_file();
	INPUT      :
	OUTPUT     :
************************************************************************/
short DCP::AdfFileFunc::delete_adf_file(bool showMsg)
{
	DCP05MsgBoxC msgbox;
	StringC msg;

	short result=-1;
	short ret=true;
	char Temp[13];

	if(!m_pCommon->card_status())//1) != 0)
		return false;
	
	if(!opened)
		return false;

	//if(msgbox(TXT_NIL_TOKEN,M_DELETE_FILE_TOK, MB_YESNO) == TRUE)
	msg.LoadTxt(AT_DCP05,M_DCP_DELETE_FILE_TOK);
	msg.Format(msg,(const wchar_t*)StringC(m_cFileName));
	if(msgbox.ShowMessageYesNo(msg))
	//if(msgbox1(TXT_NIL_TOKEN,M_DELETE_FILE_TOK, (void *) fstruct->name, MB_YESNO) == TRUE)
	{
		strcpy(Temp, m_cFileName);
		close_adf_file();
		result = remove1(Temp);
		if(result == -1)
		{
			if(showMsg)
			{
				msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_DELETE_FILE_TOK);
				msgbox.ShowMessageOk(msg);
			}
			ret = false;
		}
	}
	return ret;
}

// ****************************************************************************************
//	Save .Adf to .Dat and .Ref
// ****************************************************************************************

void DCP::AdfFileFunc::save_adf_to_ref_dat()
{
FILE *f_ref=0;
FILE *f_dat=0;

char temp1[20],/*temp2[20],*/*ptr;
char dat_name[20],ref_name[20];
short i,temp,Err1, write_ref, write_dat;
int length;
int Result;

		DCP05MsgBoxC msgbox;
		StringC msg;

		Err1 = 0;
		write_dat = 1;
		write_ref = 1;
		
		if(!m_pCommon->card_status())//(1) != 0)
			return;

		// get disk free space
		if(!m_pCommon->check_free_space(30000L))
			return;

		sprintf(temp1,"%-s",m_cFileName);
		ptr = strchr(temp1,'.');
		
		if(ptr != NULL)
		{
			*ptr = '\0';
			sprintf(dat_name,"%-s.dat",temp1);
			sprintf(ref_name,"%-s.ref",temp1);
	
			m_pCommon->strbtrim(dat_name);
			m_pCommon->strbtrim(ref_name);
		
			msg.LoadTxt(AT_DCP05, M_DCP_SAVE_ADF_TO_REF_DAT_TOK);
			msg.Format(msg,(const wchar_t*)StringC(m_cFileName),(const wchar_t*)StringC(dat_name), (const wchar_t*)StringC(ref_name));
			if(!msgbox.ShowMessageYesNo(msg))
			//if(msgbox3(TXT_NIL_TOKEN, M_SAVE_ADF_TO_REF_DAT_TOK, (void *)fstruct->name, (void *) dat_name, (void *) ref_name,MB_YESNO) != TRUE)
			{
				return;
			}

			if(access1(dat_name) == 1)
			{
				msg.LoadTxt(AT_DCP05, M_DCP_DELETE_OLD_FILE_TOK);
				msg.Format(msg,(const wchar_t*)StringC(dat_name));
				if(!msgbox.ShowMessageYesNo(msg))
				//if(msgbox1(TXT_NIL_TOKEN, M_DELETE_OLD_FILE_TOK,(void *) dat_name, MB_YESNO) != TRUE)
				{
					write_dat = 0;
				}
				else
				{
					remove1(dat_name);
				}	
			}

			if(access1(ref_name) == 1)
			{
				msg.LoadTxt(AT_DCP05, M_DCP_DELETE_OLD_FILE_TOK);
				msg.Format(msg,(const wchar_t*)StringC(ref_name));
				if(!msgbox.ShowMessageYesNo(msg))

				//if(msgbox1(TXT_NIL_TOKEN, M_DELETE_OLD_FILE_TOK,(void *) ref_name, MB_YESNO) != TRUE)
				{
					write_ref = 0;
				}
				else
				{	
					remove1(ref_name);
				}
			}	
			
				
			if(write_dat)
			{	/*
				char temp[200];
				temp[0] = '\0';
				strcat(temp, m_cPath);
				strcat(temp, dat_name);
				*/
				if(!(f_dat= fopen2(f_dat,dat_name,"wb+")))
			//if(fopen1(dat_name,FIL_ACC_CREATE|FIL_ACC_WRONLY, &f_dat) != TRUE)
				{
					msg.LoadTxt(AT_DCP05,M_DCP_FILE_OPEN_ERROR_TOK);
					msg.Format(msg,(const wchar_t*)StringC(dat_name));
					msgbox.ShowMessageOk(msg);
					//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK,(void *) dat_name,MB_OK);
					return;
			}
			}
			if(write_ref)
			{
				/*
				char temp[200];
				temp[0] = '\0';
				strcat(temp, m_cPath);
				strcat(temp, ref_name);
				*/
				if(!(f_ref = fopen2(f_ref, ref_name,"wb+")))
				//if(fopen1(ref_name,FIL_ACC_CREATE|FIL_ACC_WRONLY, &f_ref) != TRUE)
				{
					msg.LoadTxt(AT_DCP05,M_DCP_FILE_OPEN_ERROR_TOK);
					msg.Format(msg,(const wchar_t*)StringC(ref_name));
					msgbox.ShowMessageOk(msg);
					//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK,(void *) ref_name,MB_OK);
					fclose(f_dat);
					return;
				}
			}

			temp = always_single;
			always_single = 1;

			//(void) MMI_DrawBusyField(ON);

			for(i=1;i <= points; i++)
			{
				form_pnt((int) i);

				// WRITE TO .REF
				if(write_ref)
				{
					sprintf(trow,"%-6s\t%-s\t%-s\t%-s%c%c", pointid_front, xdes_front,ydes_front,zdes_front,13,10);		
					length = (int) strlen(trow);			
					Result = fputs(trow,f_ref);
		    		//Result = FIL_Write(f_ref,length,1,trow,w);
					if(Result == EOF)		
					{
						Err1 = true;
						break;
					}
				}
				if(write_dat)
				{
					// WRITE TO .DAT
					sprintf(trow,"%-6s\t%-s\t%-s\t%-s%c%c", pointid_front, xmea_front,ymea_front,zmea_front,13,10);		
					length = (int) strlen(trow);			
		    		//Result = FIL_Write(f_dat,length,1,trow,w);
					Result = fputs(trow,f_dat);
					if(Result == EOF)		
					{
						Err1 = true;
						break;
					}
				}
			}	

			always_single = temp;
			
			if(f_ref)	fclose(f_ref);
			if(f_dat)	fclose(f_dat);
	
			//(void) MMI_DrawBusyField(OFF);

		if(Err1)
		{
			msg.LoadTxt(AT_DCP05, M_DCP_ERR_ADF_TO_DATREF_TOK);
			msgbox.ShowMessageOk(msg);
			//msgbox(TXT_NIL_TOKEN, M_ERR_ADF_TO_DATREF_TOK,MB_OK);	
			remove1(ref_name);
			remove1(dat_name);				
		}
	}
}

// ****************************************************************************************
//	Convert .Ref to .Adf
// ****************************************************************************************
short DCP::AdfFileFunc::convert_ref_to_adf(char *filename)
{
char *ptr,adf_name[15],ref_name[15],temp[15];
FILE *f_ref=0;
FILE *f_adf=0;
short ret=1;
int Result;
char bPid[30], bNote[40];

char bXdes[25], bYdes[25], bZdes[25];
//int pos;

	DCP05MsgBoxC msgbox;
	StringC msg;

	sprintf(ref_name,"%-s",filename);
	m_pCommon->strbtrim(ref_name);
	
	sprintf(temp,"%-s", ref_name);
	m_pCommon->strbtrim(temp);

	ptr = strchr(temp,'.');

	if(ptr != NULL)
	{
		*ptr= '\0';
		sprintf(adf_name,"%-s.adf", temp);
	}
	else
	{
		return false;
	}

	f_ref = fopen2(f_ref,ref_name,"rb+");
	//ret = fopen1(ref_name,FIL_ACC_RDONLY, &f_ref);


	if(access1(adf_name) == 1)
	{
		msg.LoadTxt(AT_DCP05,M_DCP_DELETE_OLD_FILE_TOK);
		msg.Format(msg,(const wchar_t*)StringC(adf_name));

		//if(msgbox1(TXT_NIL_TOKEN, M_DELETE_OLD_FILE_TOK,(void *) adf_name, MB_YESNO) != TRUE)
		if(!msgbox.ShowMessageYesNo(msg))
		{
				if(f_ref)
					fclose(f_ref);
				return false;
		}
		else
		{
				remove1(adf_name);
		}
	}	

	if(ret)
	{
		f_adf = fopen2(f_adf,adf_name,"wb+");
		//ret = fopen1(adf_name, FIL_ACC_CREATE|FIL_ACC_WRONLY,&f_adf);
	}
	if(f_adf)
	{
		ret = add_header_fields(f_adf, adf_name);
	}

	if(ret)
	{
		//(void) MMI_DrawBusyField(ON);
		while(1)
		{	
			Result = get_ref_line(f_ref, bPid, bXdes,bYdes,bZdes,bNote);

			if(Result == 1)
			{
				ret = add_new_pnt(f_adf, bPid, NULL, bXdes, NULL, bYdes,NULL,bZdes,bNote); 
				if(ret == false) // ERROR
				{
					break;
				}							 	
			}
			else if(Result == EOF)
			{
				ret = true;
				break;
			}
			else 
			{
				ret = false;
				break;
			}
		}	
		//(void) MMI_DrawBusyField(OFF);
	}		

	// *************
	// Close files
	// *************
	
	if(f_adf) 
		fclose(f_adf);
	if(f_ref)
		fclose(f_ref);
	
	if(ret == false)
	{	
		// Delete Adf-file
		remove1(adf_name);
		return false;
	}
	else
	{
		sprintf(filename,"%-13.13s",adf_name);
		m_pCommon->strbtrim(filename);
		return true;
	}
}

// *****************************************************************************************
//
// *****************************************************************************************
short  DCP::AdfFileFunc::add_header_fields(FILE *fp, char *filename)
{
// 3.x
//RC_TYPE Result;
int Result;

		sprintf(linebuff,"%-10.10s %19.19s %-9.9s %-9.9s %28.28s%c%c","3D_FORM",filename," ", " "," ",13,10);
		Result = fputs(linebuff, fp); 
		//Result = FIL_Write(fp, 81, 1, linebuff,w); 
		
		if(Result != EOF)
		{
			sprintf(linebuff,"1%-77.77s1%c%c", " ",13,10);
			Result = fputs(linebuff,fp); 
			//FIL_Write(fp, 81, 1, linebuff,w); 
		}
		if(Result != EOF)
		{
			sprintf(linebuff,"%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %-6.6s%c%c",
					"PID", //"PID",
					'X', "X_ACTUAL","X_DESIGN", 
					'Y', "Y_ACTUAL","Y_DESIGN", 
					'Z', "Z_ACTUAL","Z_DESIGN", 
					"NOTE",
					13,
					10); 

			Result = fputs(linebuff,fp); 
			//Result = FIL_Write(fp, 81, 1 linebuff,w); 
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

// *****************************************************************************************
//
// *****************************************************************************************
short DCP::AdfFileFunc::add_new_pnt(FILE *fp, char *pid, char *xact, char *xdes,char *yact,char *ydes,char *zact,char *zdes,char *note) 
{
char bXact[15],bXdes[15], bYact[15],bYdes[15], bZact[15],bZdes[15], xsta,ysta,zsta;
int Result;
  
		Result = fseek(fp, 0, SEEK_END);
		//Result = FIL_Seek(fp, 0L, FIL_FROMEND);
		
		if(Result == 0)
		{
			sprintf(bXact,"%-9.9s", " ");
			sprintf(bYact,"%-9.9s", " ");
			sprintf(bZact,"%-9.9s", " ");

			sprintf(bXdes,"%-9.9s", " ");
			sprintf(bYdes,"%-9.9s", " ");
			sprintf(bZdes,"%-9.9s", " ");
			xsta = ' ';
			ysta = ' ';
			zsta = ' ';

			if(xact != NULL)
			{
				sprintf(bXact,"%9.9s", m_pCommon->strbtrim(xact));	
				xsta = 'X';
			}
			if(yact != NULL)
			{
				sprintf(bYact,"%9.9s", m_pCommon->strbtrim(yact));	
				ysta = 'Y';
			}

			if(zact != NULL)
			{
				sprintf(bZact,"%9.9s", m_pCommon->strbtrim(zact));	
				zsta = 'Z';
			}

			if(xdes != NULL)
			{
				sprintf(bXdes,"%9.9s", m_pCommon->strbtrim(xdes));	
			}
			if(ydes != NULL)
			{
				sprintf(bYdes,"%9.9s", m_pCommon->strbtrim(ydes));	
			}

			if(zdes != NULL)
			{
				sprintf(bZdes,"%9.9s", m_pCommon->strbtrim(zdes));	
			}
			
			sprintf(trow,"%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %c%c%c%c%c%c%c%c",
			pid,
			xsta, bXact,bXdes,
			ysta, bYact,bYdes,
			zsta, bZact,bZdes,
			' ',' ',' ',' ',' ',' ',13,10);
		
//			PrintLn("Write adf =%-s", trow);
			Result = fputs(trow,fp);
			//Result = FIL_Write(fp, 81,1,trow,w);
		}

		if(Result != EOF)
		{
			Result = fflush(fp);
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


// *****************************************************************************************
//
// *****************************************************************************************
short DCP::AdfFileFunc::get_ref_line(FILE *fp, char *pid, char *xdes, char *ydes,char *zdes,char *note)
{
char buff[257],bu[2];
short i,current_read;
int Result;

			// ****************************
			// Seek end of line
			// ****************************

			current_read = 0;
			sprintf(buff,"%-255.255s"," "); 
			sprintf(pid,"%-6.6s", "");
			sprintf(xdes,"%-9.9s", "");
			sprintf(ydes,"%-9.9s", "");
			sprintf(zdes,"%-9.9s", "");

			for(i=0;i<255;i++)
			{
				
				
				//Result = FIL_Read(fp,1,1,bu,w); 
				// PrintLn("Bu=%c (%d %d)",bu[0],bu[0],bu[1]);

				if(fgets(bu,2,fp))
				{
					if(bu[0] == 10 || bu[0] == 13)
					{
						if(current_read == 0)
						{
							;
						}
						else
						{	
							break;	
							current_read = 0;
						}
					}
					else
					{	
						current_read++;
						buff[current_read-1] = bu[0];	
					}
					Result = 0;
				}
				else
				{
//					PrintLn("Error %x %d",Result,Result);
					Result = EOF;
					break;
				}
			}
			buff[current_read] = '\0';
			
			sscanf(buff,"%6s %9s %9s %9s", pid, xdes, ydes, zdes);
			pid[6] = '\0';
			xdes[9]= '\0';
			ydes[9]= '\0';
			zdes[9]= '\0';
			
			// PrintLn("RefBuff=%-s", buff);
			
			if(Result == 0)
			{
				return 1;				
			}
			
			if(Result == EOF)
			{
				return EOF;
			}	
			
			return 0;
}


/**************************************************************************************************
	FUNCTION  :  int form_next_pnt(char *txt);
	INPUT     :
	OUTPUT    :
**************************************************************************************************/
short DCP::AdfFileFunc::form_next_pnt()
{
short newpnt,i;
short ret;

	newpnt = 0;
	ret = 0;

	if(!opened)
		 return -1;

	if(!m_pCommon->card_status())
		return -1;

	if(point_type == 0 || always_single)
	{
		if(active_point_front < points) // - fstpnt)
		{
			newpnt = active_point_front+1;
			form_pnt((int)newpnt);
			ret = 1;
		}
	}

	// **********************************************************
	// BACK-FRONT PARI / AKTIIVINEN PISTE FRONT
	// **********************************************************
	else  if(point_type == 1  && cpoint == 1)
	{
		// Järjestys : front, back
		if(active_point_front < active_point_back)
		{
			// Onko peräkkäin
			if((active_point_back - active_point_front) == 1)
			{
				if(active_point_back < points)
				{
					 newpnt = active_point_back+1;
					 form_pnt((int) newpnt);
					 ret = 1;
				}
			}
			else
			{
				newpnt = active_point_front+1;
				form_pnt((int) newpnt);
				ret = 1;
			}
		}
		// Järjestys: back,front
		else
		{
			if(active_point_front < points)
			  {
				newpnt=active_point_front+1;
				form_pnt((int) newpnt);
				ret = 1;
			  }
		}
	}

	// **********************************************************
	// BACK-FRONT PARI / AKTIIVINEN PISTE BACK
	// **********************************************************
	else if(point_type == 1  && cpoint == 2)
	{
		// Järjestys : front, back
		if(active_point_front < active_point_back)
		{
				if(active_point_back < points)
				{
					 newpnt = active_point_back+1;
					 form_pnt((int) newpnt);
					 ret = 1;	
				}
		}
		// Järjestys : back,front
		else
		{
			// Onko peräkkäin
			if((active_point_front - active_point_back) == 1)
			{
				if(active_point_front < points)
				{
					 newpnt = active_point_front+1;
					 for(i=0;i<=30000;i++)
					 	;
				/*
					 sprintf(msgstr,"Haettava=%d",newpnt);
					 msgbox("",msgstr,MB_OK);
				*/
					 form_pnt((int) newpnt);
					 ret = 1;
				}
			}
			else
			{
				 newpnt = active_point_back+1;
				 form_pnt((int) newpnt);
				 ret = 1;	
			}
		}
	}
	return ret;
}


/**************************************************************************************************
	FUNCTION  :int form_prev_pnt(char *txt);
	INPUT     :
	OUTPUT    :
**************************************************************************************************/
short DCP::AdfFileFunc::form_prev_pnt()
{
short newpnt;

		newpnt = 0;

		if(!opened)
			return 0;

		if(!m_pCommon->card_status())//1) != 0)
			return 0;

		// Added 6.5.98

		if(point_type == 0 || always_single)
		{
			if(active_point_front > 1)
			{
				newpnt = active_point_front-1;
				form_pnt( newpnt);
			}
		}

		// **********************************************************
		// BACK-FRONT PARI / AKTIIVINEN PISTE FRONT
		// **********************************************************

		else  if(point_type == 1  && cpoint == 1)   
		{
			// Järjestys: front,back
			if(active_point_front < active_point_back)
			{
					if(active_point_front > 1)
					{
						newpnt = active_point_front-1;
						form_pnt(newpnt);
					}
			}
			// Järjestys: back,front
			else
			{
				//  back,front peräkkäin
				if( (active_point_front - active_point_back) == 1)
				{
					// ***********Onko piste ensinmäinen lomakkeella
					if(active_point_back > 1)
					{
						newpnt = active_point_back-1;
						form_pnt(newpnt);
					}
				}
				// ************pisteet eivät ole peräkkäin ******************
				else
				{
						newpnt = active_point_front-1;
						form_pnt(newpnt);
				}

			}
		}

		// **********************************************************
		//  BACK-FRONT PARI / AKTIIVINEN PISTE BACK
		// **********************************************************
		else  if(point_type == 1  && cpoint == 2)
		{
			// Järjestys: front,back
			if(active_point_front < active_point_back)
			{
				//  front,back peräkkäin
				if( (active_point_back -active_point_front) == 1)
				{
					if(active_point_front > 1)
					{
						newpnt = active_point_front-1;
						form_pnt(newpnt);
					}
				}
				else
				{
					if(active_point_back > 1)
					{
						newpnt = active_point_back-1;
						form_pnt(newpnt);
					}
				}
			}

			// Järjestys: back,front
			else
			{
					// ***********Onko piste ensinmäinen lomakkeella
					if(active_point_back > 1)
					{
						newpnt = active_point_back-1;
						form_pnt(newpnt);
					}
			}
		}
		//else show_info(FIRST_POINT);

	return 0;
}


// *************************************************************************
//
// *************************************************************************
short DCP::AdfFileFunc::delete_point_from_adf(short pno)
{
short i,j;
char out_name[40];
FILE* fp_out=0;
char buff[CPI::LEN_PATH_MAX];
char temp[CPI::LEN_PATH_MAX];
//unsigned short w;
//int filpos;
//int Result;
		DCP05MsgBoxC msgbox;
		StringC msg;
		if(points <=1)
		{
			msg.LoadTxt(AT_DCP05,M_DCP_CANNOT_DELETE_POINT_TOK);
			msg.Format(msg,(const wchar_t*)StringC(L""));
			msgbox.ShowMessageOk(msg);
			//msgbox(TXT_NIL_TOKEN,M_CANNOT_DELETE_PNT_TOK,MB_OK);
			return false;
		}	
		if(!m_pCommon->card_status() == 0)
		{
			if(m_pCommon->check_free_space(30000L))
			{
		
				//CalcFile_out.open = -1;
				
				//sprintf(CalcFile_in.name,  "%-s",fstruct->name);
				sprintf(out_name, "%-s","temp.tmp");
				if(!(fp_out = fopen2(fp_out,out_name,"wb+"))) //FIL_ACC_CREATE|FIL_ACC_WRONLY,&fp_out)!= TRUE)
				{
					msg.LoadTxt(AT_DCP05,M_DCP_FILE_OPEN_ERROR_TOK);
					msg.Format(msg,(const wchar_t*)StringC(out_name));
					msgbox.ShowMessageOk(msg);
					//msgbox1(TXT_NIL_TOKEN,M_FILE_OPEN_ERROR_TOK, (void *) out_name,MB_OK);
					return false;	
				}
				fseek(m_pFile, 0L,SEEK_SET);
				//ftell(
				//FIL_Tell(fstruct->f,filpos);	
				
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				fgets(buff,82,m_pFile);
				//buff[79] = 10;
				buff[81] = '\0';
				fputs(buff,fp_out);
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);

				//FIL_Tell(fstruct->f,filpos);	
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);
				fgets(buff,82,m_pFile);
				//buff[79] = 10;
				buff[81] = '\0';
				fputs(buff,fp_out);
	

				//FIL_Tell(fstruct->f,filpos);	
				//FIL_Read(fstruct->f,ROW_LENGTH,1,buff,w); buff[ROW_LENGTH] = '\0';
				//FIL_Write(fp_out,ROW_LENGTH,1,buff,w);
				fgets(buff,82,m_pFile);
				//buff[79] = 10;
				buff[81] = '\0';
				fputs(buff,fp_out);
	
				j = 1;
				for(i=1;i<= points;i++)
				{
					//FIL_Tell(fstruct->f,filpos);

					//FIL_Read(fstruct->f, ROW_LENGTH,1,buff, w);buff[ROW_LENGTH] = '\0';
					fgets(buff,82,m_pFile);
					if(i != pno)
					{		
						//buff[79] = 10;
						buff[81] = '\0';
						fputs(buff,fp_out);
						//FIL_Write(fp_out,ROW_LENGTH,1,buff, w);
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
				//FIL_Close(fstruct->f);
				remove1(m_cFileName);
				sprintf(buff,"%s%s",m_cPath, m_cFileName);
				sprintf(temp,"%s%s",m_cPath, "temp.tmp");

				// VIVA
				boost::filesystem::path f_old = buff;
				boost::filesystem::path f_new = temp;

				boost::filesystem::rename(temp,buff);
				//CPI::FileUtilitiesC::RenameFile(StringC(temp), StringC(buff));
				//rename(temp,buff);
				
				
				//Result = FIL_Rename("a:\\temp.tmp",buff);
				//PrintLn("Result=%x %d Buff=%-s",Result,Result,buff);
				file_updated = 1;
				open_ADF_file_name("rb+");//, fstruct->name,FIL_ACC_RDWR);
				return true;
			}
		}
		return false;
}


/**************************************************************************************************
	FUNCTION  :   int form_save_pnt(void);
	INPUT     :
	OUTPUT    :
**************************************************************************************************/
short DCP::AdfFileFunc::form_save_pnt()
{
short uu_f;
//unsigned short w;

	
	if(opened)
	{
			if(!m_pCommon->card_status())
				return 0;
				/*
				#ifndef LEICA_DCP05_DEMO
                if(get_prog_mode() == PROG_MODE_DEMO)
                {
                	if(active_point_front-1 > 10)
					{
                		msgbox(TXT_NIL_TOKEN, M_MORE_THAN_10_POINTS_TOK,MB_OK);    
				    	return 0;
					}
                 }
			#endif
			*/
			if(!m_pCommon->strblank(xmea_front))
				xsta_front = 'X';

			if(!m_pCommon->strblank(ymea_front))
				ysta_front = 'Y';

			if(!m_pCommon->strblank(zmea_front))
				zsta_front = 'Z';

			sprintf(trow,"%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %c%c%c%c%c%c%c%c",

			pointid_front,
			xsta_front, xmea_front, xdes_front,
			ysta_front, ymea_front, ydes_front,
			zsta_front, zmea_front, zdes_front,
			note_front[0],
			note_front[1],
			note_front[2],
			note_front[3],
			note_front[4],
			note_front[5],13,10);

			fseek(m_pFile, fstpnt*(reclen)+(active_point_front-1)*(reclen), SEEK_SET);
			//FIL_Seek(fstruct->f,fstpnt*reclen+(fstruct->active_point_front-1)*reclen,FIL_SETPOS);
			fputs(trow,m_pFile);
			//FIL_Write(fstruct->f,81,1,trow,w);

			if(point_type != 0 && active_point_back != 0)
			{
				if(!m_pCommon->strblank(xmea_back))
					xsta_back = 'X';

				if(!m_pCommon->strblank(ymea_back))
					ysta_back = 'Y';

				if(!m_pCommon->strblank(zmea_back))
					zsta_back = 'Z';

				sprintf(trow,"%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %c%c%c%c%c%c%c%c",

				pointid_back,
				xsta_back, xmea_back, xdes_back,
				ysta_back, ymea_back, ydes_back,
				zsta_back, zmea_back, zdes_back,
				note_back[0],
				note_back[1],
				note_back[2],
				note_back[3],
				note_back[4],
				note_back[5],13,10);

				fseek(m_pFile, fstpnt*(reclen)+(active_point_back-1)*(reclen), SEEK_SET);
				//FIL_Seek(fstruct->f,fstpnt*reclen+(fstruct->active_point_back-1)*reclen,FIL_SETPOS);
				fputs(trow,m_pFile);
				//FIL_Write(fstruct->f,81,1,trow,w);
			}
			fflush(m_pFile);
			fflush(m_pFile);
			//FIL_Flush(fstruct->f);
			//FIL_Flush(fstruct->f);
			fclose(m_pFile);
			//FIL_Close(fstruct->f);
			opened  = -10;
			uu_f = active_point_front;
			file_updated = 1;
			open_ADF_file_name("rb+");//fstruct,fstruct->name,FIL_ACC_RDWR);

			form_pnt(uu_f);
	} 
	return 0;
}

// ***********************************************************************
// ***********************************************************************
FILE* DCP::AdfFileFunc::get_file_pointer()
{
	return m_pFile;
}

/************************************************************************
	Add new point to the 3d-form.
*************************************************************************/
short DCP::AdfFileFunc::add_point(char *id)
{
short ret=0,pp,etsi,ret1;
int s;
char apu[7];
//int new_filpos;
//int Result;
	
	DCP05MsgBoxC msgbox;

	if(!m_pCommon->card_status())
		return false;

	if(!m_pCommon->check_free_space(30000L))
		return false;
	/*
	if(points == 0)
	{
		sprintf(id,"P%-5d", points+1);
	}
	else
	{
		// seek the last point id
		new_filpos = fstpnt*(reclen+1)+(points-1)*(reclen+1);
		fseek(m_pFile,new_filpos, SEEK_SET);
		//FIL_Seek(fstruct->f,new_filpos,FIL_SETPOS);
		fgets(line,82,m_pFile);
		line[81] = '\0';
		//FIL_Read(fstruct->f, 81,1,line, w); line[81] = '\0';
		
		strncpy(id,line,6);
		id[6] = '\0';
		inc_id(id);

		// **************************
		// Add 27.8.1998
		// **************************
		ret1 = get_point_type(id);
		if(ret1 == BACK)
		{
			id[strlen(id)-1] = 'F';
		}
		else if(ret1 == FRONT)
		{
				id[strlen(id)-1] = 'B';
		}

		// *************
		// End add
		// *************
	}
	*/
	// ***********************	
	// Ask new point id
	// ***********************	
	
	// Auto increment checking added 220399

	// siirrä tämä sinne mistä kutsutaan addpoint -funktiota
	//if(get_AUTO_INCREMENT() == TRUE) 
	/*
	if(m_pDCP05Model->m_nAutoIncrement)
	{
		ret2 = true;
	}
	else
	{
		//ret2 = input_text(T_3DFILE_TOK,M_ENTER_PID_TOK,emptyinfo, 6,id);
	}
	*/
	if(1)//ret2 == true)
	{	
		// ********************************		
		// Add 27.8.1998
		// If pid is empty return FALSE
		// ********************************		

		m_pCommon->strbtrim(id);
		if(m_pCommon->strblank(id))
		{
			return false;
		}
		
		// *********************************
		// End add
		// *********************************
	
		ret1 = get_point_type(id);
		
		if(always_single) //== TRUE)
			ret1= SINGLE;

		if(ret1 == BACK)  
		{
			if(seek_pid(id) != 0)  // Etsitään BACK-piste
			{			
				// Jos löytyi ei lisätä mitään	
				StringC msg;
				msg.LoadTxt(AT_DCP05, M_DCP_POINT_ID_EXISTS_TOK);
				msg.Format(msg,(const wchar_t*)StringC(id));
				msgbox.ShowMessageOk(msg);
				//msgbox1(TXT_NIL_TOKEN,M_PID_IN_USE_TOK,(void *) id,MB_OK);
				return false;
			}
		}
		else if(ret1 == FRONT)
		{
			// Etsitään Ensiksi Back-piste

			strncpy(apu,id,6);apu[6] = '\0';	
			apu[strlen(id)-1] = 'B';
					
			if(seek_pid(apu) == 0) // Ei löytynyt, lisätään back
			{
				
				sprintf(id,"%-6.6s",apu);
			}
			else 
			{	
				if(seek_pid(id) != 0) // Etsitään FRONT-piste				
				{
					StringC msg;
					msg.LoadTxt(AT_DCP05, M_DCP_POINT_ID_EXISTS_TOK);
					msg.Format(msg,(const wchar_t*)StringC(id));
					msgbox.ShowMessageOk(msg);
					//msgbox1(TXT_NIL_TOKEN,M_PID_IN_USE_TOK,(void *) id,MB_OK);
					return false;
				}
			}
		}
		else
		{
			if(seek_pid(id) != 0)
			{
				StringC msg;
				msg.LoadTxt(AT_DCP05, M_DCP_POINT_ID_EXISTS_TOK);
				msg.Format(msg,(const wchar_t*)StringC(id));
				msgbox.ShowMessageOk(msg);
				return false;
			}
		}

		// **************************
		// Seek the end of file
		// **************************

		//if(FIL_Seek(fstruct->f, 0L, FIL_FROMEND)==RC_OK)
		if(fseek(m_pFile,  0L, SEEK_END) == 0)
		{
			strncpy(apu,id,6);apu[6] = '\0';
			pp = (short) strlen(apu);
			s = toupper(apu[pp-1]);

			// *****************************************
			// Fill buffer with new record values
			// *****************************************

			sprintf(trow,"%-6.6s %c %-9s %-9s %c %-9s %-9s %c %-9s %-9s %c%c%c%c%c%c%c%c",

			id,
			'_', " ",  " ",
			'_', " ",  " ",
			'_', " ",  " ",
			' ',' ',' ',' ',' ',' ',13,10);

			// ***************************
			// Write it to file
			// ***************************
			if(fputs(trow,m_pFile) != EOF)
			//if(FIL_Write(fstruct->f, 81,1,trow,w)==RC_OK)
			{
				fflush(m_pFile);
				//FIL_Flush(fstruct->f);
			
				// ***********************
				// Inc points count
				// ***********************			
				points = points +1;
				etsi = points;

				// New size
				//CPI::FileIteratorC FileIterator;
				boost::filesystem::path  FileInfo;

				char temp[CPI::LEN_PATH_MAX];
				temp[0] = '\0';
				strcat(temp, m_cPath);
				strcat(temp, m_cFileName);
				char* pSearch = &temp[0];

				if(m_pCommon->find_first_file(pSearch,&FileInfo) == 0)
				//if(FileIterator.FindFirst(pSearch, FileInfo) == 0)    
				{
					//m_lSize = FileInfo.GetSize();		
					m_lSize = (uint32_t)boost::filesystem::file_size(FileInfo);
				}

				//Result = FIL_StartList(fstruct->name, &finfo);	
				//fstruct->size =  finfo.ulFsize; //filelength(fileno(fstruct->f)); //FormFile_.size + 80;
			 }

			// *****************************
			// Get new point from file
			// *****************************

			form_pnt(etsi);

			// Return true
			ret = true;
		}
	}

	return ret;
}

/************************************************************************
*************************************************************************/
void DCP::AdfFileFunc::inc_id(char *id)
{
char apu[10],apu2[10],numstr[10];
short last,first,i,j,length,pit;
// int ret;

	 sprintf(apu,"%-6.6s",id);
	 m_pCommon->strbtrim(apu);

	 pit = (short)  strlen(apu);

	last = 0;

	// ******************************
	// last num
	// ******************************

	for(i=pit-1;i>=0;i--)
	{
		// ret = isdigit((int) apu[i]);
		// ret = isalnum((int) apu[i]);
		// PrintLn("i=%d %c Isdigit = %d",i,apu[i],ret);
		
		if(apu[i]== '0' ||apu[i] == '1' ||apu[i] == '2' ||apu[i] == '3' ||apu[i] == '4' ||apu[i] == '5' ||
			apu[i] == '6' ||apu[i] == '7' ||apu[i] == '8' ||apu[i] == '9' )
		{
			last = i+1;
			break;
		}
	}

	if(last == 0)
		return;

	first = last;

	// ******************************
	// first num
	// ******************************

	for(i=last-1;i>=0;i--)
	{
		// if(isdigit((int) apu[i]) != 0)
		// if(isalnum((int) apu[i]) != 0)
		if(apu[i]== '0' ||apu[i] == '1' ||apu[i] == '2' ||apu[i] == '3' ||apu[i] == '4' ||apu[i] == '5' ||
			apu[i] == '6' ||apu[i] == '7' ||apu[i] == '8' ||apu[i] == '9' )

		{
			first = i+1;
		}
	}

	// **********************
	// copy numbers to buffer
	// **********************

	for( i = first-1,j=0; i<=last-1;i++,j++)
	{
		apu2[j] = apu[i];
	}

	apu2[j] = '\0';

	// **********************
	// Kasvatetaan numeroa
	// **********************
	
	sprintf(numstr,"%-d",atoi(apu2)+1);
	length = (short) strlen(numstr);

	if((length + first-1 + pit-last ) <= 6)
	{
		sprintf(id,"%*s", first-1, apu);
		sprintf(id+first-1,"%*s", length, numstr);
		sprintf(id+first-1+length,"%s",apu+last);
     	 id[6] = '\0';
	}
	else
	{
	}
}

// ****************************************************************************************
//	tsekataan onko piste front, back  tai single
// ****************************************************************************************
short DCP::AdfFileFunc::get_point_type(char *pid)
{
char id[15],ch;
short ret=SINGLE;
			
		sprintf(id,"%-6.6s",pid);
	
		m_pCommon->strbtrim(id);
		if(!m_pCommon->strblank(id))
		{
			ch = id[strlen(id)-1];
			if(ch == 'B' || ch == 'b')
			{
				ret = BACK;
			}
			else if(ch == 'F' || ch == 'f')
			{
				ret = FRONT;
			}
		}

	return ret;
}

// *************************************************************************
//	Auto match
// *************************************************************************
short DCP::AdfFileFunc::find_point(double x, double y, double z)
{
//#ifndef AMS_DCP05

int i,pno;
char pid[7],pid2[7];
char xdes[15],ydes[15],zdes[15];
struct ams_vector cur_xyz,xyz;
//char msg[255];
double dist, min;
double xdsg,ydsg,zdsg;
	
	DCP05MsgBoxC msgbox;

	min = -999999.0;
	pno = -1;
	cur_xyz.x = x;
	cur_xyz.y = y;
	cur_xyz.z = z;
	
	for(i=1;i<=points;i++)
	{
				
		form_pnt1((int) i, pid, NULL,NULL,xdes,NULL, NULL, ydes, NULL, NULL, zdes, NULL);
		
		if(!m_pCommon->strblank(xdes) && !m_pCommon->strblank(ydes) && !m_pCommon->strblank(zdes))
		{
			xyz.x = atof(xdes);
			xyz.y = atof(ydes);
			xyz.z = atof(zdes); 
			dist = calc_point_dist_from_point(&cur_xyz,&xyz);
			
			if(dist < min || min == -999999.0)
			{
				
				xdsg = atof(xdes);
				ydsg = atof(ydes);
				zdsg = atof(zdes);
				pno = i;
				min = dist;		
				sprintf(pid2,"%-s",pid);
			}
		}
	}
	if(pno != -1)
	{	
		
		//if(!show_automatch_point(pid2,&x,&y,&z,&xdsg, &ydsg, &zdsg))
		//	pno = -1;
	}
	else
	{	
		StringC msg;
		msg.LoadTxt(AT_DCP05, M_DCP_AUTOMATCH_FAILED_TOK);
		msgbox.ShowMessageOk(msg);
		//msgbox(TXT_NIL_TOKEN, M_AUTOMATCH_FAILED_TOK,MB_OK);
	}
	return pno;
//#else
//     return -1;
//#endif
}

short DCP::AdfFileFunc::get_next_point_id(char *id)
{
short ret=0,ret1;
char line[255];
int new_filpos;


	if(!m_pCommon->card_status())
		return false;
	if(points == 0)
	{
		sprintf(id,"P%-5d", points+1);
	}
	else
	{
		// seek the last point id
		new_filpos = fstpnt*(reclen)+(points-1)*(reclen);
		fseek(m_pFile,new_filpos, SEEK_SET);
		//FIL_Seek(fstruct->f,new_filpos,FIL_SETPOS);
		fgets(line,82,m_pFile);
		line[81] = '\0';
		//FIL_Read(fstruct->f, 81,1,line, w); line[81] = '\0';
		
		strncpy(id,line,6);
		id[6] = '\0';
		inc_id(id);

		// **************************
		// Add 27.8.1998
		// **************************
		ret1 = get_point_type(id);
		if(ret1 == BACK)
		{
			id[strlen(id)-1] = 'F';
		}
		else if(ret1 == FRONT)
		{
				id[strlen(id)-1] = 'B';
		}

		// *************
		// End add
		// *************
	}
	return true;
}
