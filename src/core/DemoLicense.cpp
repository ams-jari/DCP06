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
#include <ABL_AppSystemInfo.hpp>
#include "dcp06/core/CS35.hpp"
#include <dcp06/core/Defs.hpp>

#include <dcp06/core/DemoLicense.hpp>
#include <dcp06/core/MsgBox.hpp>

#include <GUI_MessageDialog.hpp>

#include <ABL_MsgDef.hpp>
//#include <ABL_AppSystemInfo.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <GUI_Util.hpp>

#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>

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

DCP::DCP06DemoLicenseC::DCP06DemoLicenseC(DCP::DCP06ModelC* pDCP06Model): m_pDCP06Model(pDCP06Model)
{
	
}

//DCP::DCP06CommonC::DCP06CommonC(): m_pDCP06Model(0)
//{
//	
//}
// ****************************************************************************************
DCP::DCP06DemoLicenseC::~DCP06DemoLicenseC()
{
	
}

#define SEC_KEY_DEMO1 	 "r"
#define PROG_ID_DEMO1 	 "adffdtyv"
#define APPL_ID_DEMO1    "klujsig"

#define SEC_KEY_DEMO2 	 "e"
#define PROG_ID_DEMO2 	 "ttuityj"
#define APPL_ID_DEMO2    "kluksig"

#define SEC_KEY_DEMO3	 "s"
#define PROG_ID_DEMO3 	 "ghjtryj"
#define APPL_ID_DEMO3    "aluksjkg"

#define SEC_KEY_DEMO4 	 "h"
#define PROG_ID_DEMO4 	 "ioputrj"
#define APPL_ID_DEMO4    "clujsjkr"

#define SEC_KEY_DEMO5 	 "i"
#define PROG_ID_DEMO5 	 "mnbutrd"
#define APPL_ID_DEMO5    "clyjsjkp"


// ****************************************************************************************
int DCP::DCP06DemoLicenseC::find_keycode(char* keycode)
{
	int ret = 0;
	char tempCode[21];
	tempCode[0] = '\0';

	for(int i=1; i <= 5; i++)
	{
		get_code_demo(i, tempCode);
		if(strcmp(keycode, tempCode) == 0) // = entered keycode
		{
			ret = i;
			break;
		}
	}
	return ret;
}
 
// ****************************************************************************************
bool DCP::DCP06DemoLicenseC::is_license_ok(char* keycode, DateTime startDate)
{
	if(keycode[0] == '\0')
		return false;

	char tempCode[21];
	tempCode[0] = '\0';

	if(startDate == 0.0) // first demo 
	{
		get_code_demo(1,tempCode);
		if(strcmp(keycode, tempCode) == 0 && tempCode[0] != '\0')
		{
			return true;
		}
		else
			return false;
	}
	
	int demoNumber = find_keycode(keycode);

	if(demoNumber <= 1)
	{
		return false;
	}
	else
	{
		// check previous code
		
		char oldCode[21];
		oldCode[0] = '\0';

		get_code_demo(demoNumber -1, oldCode);

		get_code_demo(demoNumber,tempCode);

		if(strcmp(oldCode, m_pDCP06Model->sKeyCodeDemo1) == 0) // last one  is ok
		{
			if(strcmp(tempCode, keycode) == 0) // check new one
			{
				return true;
			}
		}
	}
	
	return false;
}

// ****************************************************************************************
int DCP::DCP06DemoLicenseC::get_available_days(char* keycode, DateTime startDate)
{
	int days  = 0;

	char tempCode[21];
	tempCode[0] = '\0';
	
	if(keycode[0] == '\0')
		return 0;
	
	if(startDate == 0.0)
		return 0;

	for(int i=1; i <= 5; i++) 
	{
		get_code_demo(i, tempCode);
		if(strcmp(keycode, tempCode) == 0) // = entered keycode
		{ 
			
			DateTime oDateTime = GUI::GetActualTime();		
			/*int year_diff, day_diff, month_diff;
			int hour_diff, min_diff, sec_diff;

			bool ret = GMAT::GetMjdDifference( (double) (long) startDate, (double)(long) oDateTime, year_diff, month_diff, day_diff, hour_diff, min_diff, sec_diff );*/
			int day_diff;
			day_diff = (int) oDateTime - (int)startDate;
			/*if(ret)
			{*/
				days = 30 - day_diff;  //  
				break;
			/*}
			else
			{
				break;
			}*/
		}
	}
	return days;
}

// ****************************************************************************************
void DCP::DCP06DemoLicenseC::get_code_demo(int demoNumber, char *sCode)
{
int nro=1L;
char *p, *p1,*p2;
char temp[20];
int i,len;
short temp2[20];

unsigned int code = 0L;

	//DCP06CommonC common(m_pDCP06Model);
	//int year, month, day;

	//common.GetDate(&day, &month, &year);

//#ifndef CS20
#if !defined( CS20) && !defined(CS35)
	int snro;
	TPI::InstrumentInfoC oInstrumentInfo;
	TPI::SensorC::Instance()->GetInstrumentInfo(oInstrumentInfo);
 
	const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentSerialNum();
	snro = atol(cInstrumentNumber);
	sprintf(temp,"%-lu",snro);

#elif defined(CS20)
	DCP06MsgBoxC msgBox;

	StringC ss1 = L"";
	RcT ret= CPI::SensorC::GetInstance()->GetSerialNumber(ss1);
	sprintf(temp,"%d",ret);

	char ttt[20];
	BSS::UTI::BSS_UTI_WCharToAscii(ss1, ttt);
	//snro = atol(ttt);
 
	sprintf(temp,"%-s",ttt);

	//msgBox.ShowMessageOk(StringC(ssName), StringC(temp));
 
#elif defined(CS35)
	DCP06CS35C cs35;
	cs35.get_serialnumber(temp);
#endif

	//snro = 235063;
   	 //sprintf(temp,"%-lu",snro);
	 len = (int) strlen(temp);
	 temp[len] = '\0';
	 
	 sCode[0] = '\0';
     code = 0L;

	 p = temp;
	 if(demoNumber == 1)
	 {
		 p1 = PROG_ID_DEMO1;
		 p2 = SEC_KEY_DEMO1;
	 }
	 else if(demoNumber == 2)
	 {
		 p1 = PROG_ID_DEMO2;
		 p2 = SEC_KEY_DEMO2;
	 }
	 else if(demoNumber == 3)
	 {
		 p1 = PROG_ID_DEMO3;
		 p2 = SEC_KEY_DEMO3;
	 }
	 else if(demoNumber == 4)
	 {
		 p1 = PROG_ID_DEMO4;
		 p2 = SEC_KEY_DEMO4;
	 }
	 else if(demoNumber == 5)
	 {
		 p1 = PROG_ID_DEMO5;
		 p2 = SEC_KEY_DEMO5;
	 }


     nro= 1L;

    // ******************
    // Basic
    // ******************
    for(i=0;i<len;i++)
    {
        if(*p != *p1)
        temp2[i] =(short)( ((*p*i*4) ^ (*p1) + (*p+i) ^ (*p2-1) + *p1 * *p2)); //*1.7);
		p++;
		p1++;
		p2++;

		nro = nro*temp2[i]+i;

		if(*p1 == '\0')
		{
		 if(demoNumber == 1)
		 {
			 p1 = PROG_ID_DEMO1;
		 }
		 else if(demoNumber == 2)
		 {
			 p1 = PROG_ID_DEMO2;
		 }
		 else if(demoNumber == 3)
		 {
			 p1 = PROG_ID_DEMO3;
		 }
		 else if(demoNumber == 4)
		 {
			 p1 = PROG_ID_DEMO4;
		 }
		 else if(demoNumber == 5)
		 {
			 p1 = PROG_ID_DEMO5;
		 }
		}
		if(*p2 == '\0') 
		{
			 if(demoNumber == 1)
			 {
				 p2 = SEC_KEY_DEMO1;
			 }
			 else if(demoNumber == 2)
			 {
				 p2 = SEC_KEY_DEMO2;
			 }
			 else if(demoNumber == 3)
			 {
				 p2 = SEC_KEY_DEMO3;
			 }
			 else if(demoNumber == 4)
			 {
				 p2 = SEC_KEY_DEMO4;
			 }
			 else if(demoNumber == 5)
			 {
				 p2 = SEC_KEY_DEMO5;
			 }
		}
    }

	code =(unsigned int) nro;

    while(code <= 9999999L)
    {
       code  = (unsigned int) (code * 2); //.5);
    }
	sprintf(sCode,"%lu",code);
	//return code;
}