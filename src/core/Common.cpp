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
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include "calc.h"
#include <math.h>

#include <GUI_MessageDialog.hpp>
//#include <ABL_Types.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_AppSystemInfo.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>
#include <TPI_Correction.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
#include <TBL_Util.hpp>
#include <GUI_Util.hpp>
#include <GUI_ModalDelay.hpp>
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


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::Common::Common(DCP::Model* pModel): m_pModel(pModel)
{
	
}

//DCP::Common::Common(): m_pModel(0)
//{
//	
//}
// ****************************************************************************************
DCP::Common::~Common()
{
	
}
// ****************************************************************************************
bool DCP::Common::check_distance(double x, double y, double z, S_POINT_BUFF* points, short iCount, short iCurrentPoint)
{
	struct ams_vector cur_xyz, xyz;
	double dist=0.0;
	bool bRet = true;

	cur_xyz.x = x;
	cur_xyz.y = y;
	cur_xyz.z = z;

	for(int i=0; i < iCount; i++)
	{
		if( i != iCurrentPoint-1)
		{
			if(points[i].sta != POINT_NOT_DEFINED)
			{
				xyz.x = points[i].x;
				xyz.y = points[i].y;
				xyz.z = points[i].z;

				dist = calc_point_dist_from_point(&cur_xyz,&xyz);
				if(to_mm(dist) <= 5.0)
				{
					StringC msg;
					StringC sPoint;
					sPoint.Format(L"%d/%d %s", i+1, iCount,(const wchar_t*) StringC(points[i].point_id));
					msg.LoadTxt(AT_DCP05,M_DCP_SAME_MEASURED_VALUES_TOK);
					msg.Format(msg,(const wchar_t*)StringC(sPoint));
					MsgBox msgbox;
					if(!msgbox.ShowMessageYesNo(msg))
					{
						bRet = false;
					}
					break;
				}
			}
		}
	}
	return bRet;
}

// ********************************************************************************
//	
// ********************************************************************************
double DCP::Common::to_mm(double dist)
{
double ret;

	if(m_pModel->m_nUnit == INCH)
	{
		ret = dist * 25.400051;
	}
	else if(m_pModel->m_nUnit == FEET)
	{
		ret = dist * 304.800612;
	}
	else
	{
		ret = dist;
	}

	return ret;	
}

// ********************************************************************************
//	
// ********************************************************************************
short DCP::Common::points_count_in_plane(S_PLANE_BUFF* plane)
{
	short iCount = 0;
	for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
	{
		if(plane[0].points[i].sta != POINT_NOT_DEFINED)
			iCount++;
	}
	return iCount;
}

// ********************************************************************************
//	
// ********************************************************************************
short  DCP::Common::get_last_defined_point(S_POINT_BUFF* points, short max)
{
	short ret=0,i;
	StringC id;
	for(i=max-1;i >=0; i--)
	{
		if(points[i].sta != 0 || !strblank(points[i].point_id))
		{
			ret = i+1;
			break;
		}
	}
	return ret;
}
// ********************************************************************************
//	
// ********************************************************************************
short  DCP::Common::get_last_defined_point(S_POINT_BUFF *point_OCS, S_POINT_BUFF *point_DCS, short max)
{
short i,ret=0;

	for(i=max-1; i >= 0; i--)
	{
		if(point_OCS[i].sta != POINT_NOT_DEFINED || point_DCS[i].sta != POINT_NOT_DEFINED ||
			!strblank(point_OCS[i].point_id)) //&& point_DCS[i].sta != 0)
		{
			ret = i+1;
			break;
		}
	}
	return ret;
}

// ********************************************************************************
//	
// ********************************************************************************
short  DCP::Common::get_OCS_points_count(S_POINT_BUFF *point_OCS, short max)
{
short i,ret=0;

	for(i=0; i < max; i++)
	{
		if(point_OCS[i].sta != 0 ) //&& point_DCS[i].sta != 0)
			ret++;
	}
	return ret;
}

/************************************************************************
*************************************************************************/
void DCP::Common::empty_xyz_buffers(char *x, char *y, char *z, short pit)
{
	if(x != nullptr){ sprintf(x,"%-*.*s",pit,pit,""); x[pit] = '\0'; }
	if(y != nullptr){ sprintf(y,"%-*.*s",pit,pit,""); y[pit] = '\0'; }
	if(z != nullptr){ sprintf(z,"%-*.*s",pit,pit,""); z[pit] = '\0'; }
}

/************************************************************************
*************************************************************************/
void DCP::Common::empty_buffer(char *xyz, short pit)
{

	if(xyz != nullptr)
	{
		sprintf(xyz,"%-*.*s",pit,pit,"");
		xyz[pit] = '\0';
	}
}
/************************************************************************
*************************************************************************/
short DCP::Common::strblank(char *s)
{
   char *p = s;

   for(p = s; (*p != '\0'); p++)
	  if('!' <= *p)
		 return(0);                /* non-blank                     */
   return(1);
}  /* strblank() */


void DCP::Common::delete_point(S_POINT_BUFF *points)
{
	/*
	show_message(points->point_id);
	*/
	//memset(points,'\0',sizeof(struct point_buff_));

	//setmem(points->point_id,'\0',7);

	memset(&points[0],0,sizeof(S_POINT_BUFF));
	/*
	points[0].sta = 0;
	points[0].dsta = 0;
	points[0].x = 0.0;
	points[0].y = 0.0;
	points[0].z = 0.0;
	points[0].xdes = 0.0;
	points[0].ydes = 0.0;
	points[0].zdes = 0.0;
	points[0].diameter = 0.0;
	points[0].cds= 0;
	sprintf(points[0].point_id,"%6.6s","");
	strbtrim(points[0].point_id);
	*/
}


/**************************************************************
		  check how many points are defined
***************************************************************/
short DCP::Common::defined_pom_points(S_POINT_BUFF *point_OCS, short *lastpoint)
{
short points_defined=0,i;
	
	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(point_OCS[i].sta == 1 || point_OCS[i].sta == 2 /* && point_DCS[i].sta != 0 */)
			points_defined++;
		if(point_OCS[i].sta !=0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return points_defined;
}


/*************************************************************************
*************************************************************************/

char *DCP::Common::strltrim(char *s)
{
   char *p;
   char *q;

   p = q = s;
   while(('!' > *p) && (*p != '\0'))
	  p++;
   while(*p != '\0')
	  *q++ = *p++;
   *q = '\0';
   return(s);
}


/*************************************************************************
*************************************************************************/

char *DCP::Common::strtrim(char *s)
{
   short  i;

   for(i = (short) strlen(s) - 1; (33 > s[i]) && i >= 0; i--)
	  ;
   s[i+1] = '\0';
   return(s);
}

/*************************************************************************
*************************************************************************/

char *DCP::Common::strbtrim(char *s)
{
	strltrim(s);
	strtrim(s);

	return (s);
}

short DCP::Common::get_OCS_SCS_points_count(S_POINT_BUFF *point_OCS,S_POINT_BUFF *point_DCS, short max)
{
short i,ret=0;

	for(i=0; i < max; i++)
	{
		//if(point_OCS[i].sta != 0  && point_DCS[i].sta != 0)
		if((point_OCS[i].sta == POINT_MEASURED || point_OCS[i].sta == POINT_DESIGN)  && 
			(point_DCS[i].sta == POINT_MEASURED || point_DCS[i].sta == POINT_DESIGN))
			ret++;
	}
	return ret;
}


short DCP::Common::overwrite_point(StringC sPid)
{
	MsgBox msgbox;
	StringC msg;
	
	msg.LoadTxt(AT_DCP05,M_DCP_OVERWRITE_POINT_TOK);
	msg.Format(msg,(const wchar_t*)sPid);
			
	return msgbox.ShowMessageYesNo(msg);
	{
				
	}
}

unsigned int DCP::Common::get_free_space()
{
	if(card_status())
	{
		// VIVA
		//unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(ABL::AppSystemInfoC::ePCCard);
#ifdef CS35
		ABL::AppSystemInfoC::eDevicesT device;
		
		if(m_pModel->FILE_STORAGE1 == CPI::deviceLocalMemory)
			device =  ABL::AppSystemInfoC::eLocalMemory;
		else
			device = ABL::AppSystemInfoC::eUSB;

		unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(device);
#else
		ABL::AppSystemInfoC::eDevicesT device;
		
		if(m_pModel->FILE_STORAGE1 == CPI::deviceSdCard)
			device =  ABL::AppSystemInfoC::eSDCard;
		else
			device = ABL::AppSystemInfoC::eUSB;

		unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(device);
#endif
		return uiFreeSpace;
	}
	return 0;
}   

short DCP::Common::check_free_space(unsigned int si)
{
	//ABL::AppSystemInfoC::eDevicesT eRet = ABL::AppSystemInfoC::Instance()->GetAvailableDevices();

	//if(0==(eRet & ABL::AppSystemInfoC::ePCCard))
	if(card_status())
	{
		// VIVA
		//unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(ABL::AppSystemInfoC::ePCCard);
#ifdef CS35
		ABL::AppSystemInfoC::eDevicesT device;
		
		if(m_pModel->FILE_STORAGE1 == CPI::deviceLocalMemory)
			device =  ABL::AppSystemInfoC::eLocalMemory;
		else
			device = ABL::AppSystemInfoC::eUSB;

		unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(device);
#else
		ABL::AppSystemInfoC::eDevicesT device;
		
		if(m_pModel->FILE_STORAGE1 == CPI::deviceSdCard)
			device =  ABL::AppSystemInfoC::eSDCard;
		else
			device = ABL::AppSystemInfoC::eUSB;

		unsigned int uiFreeSpace = ABL::AppSystemInfoC::Instance()->GetFreeSpaceOnDevice(device);
#endif
		if(uiFreeSpace <= si)
		{
			MsgBox msgbox;
			StringC msg;
			msg.LoadTxt(AT_DCP05,M_DCP_NOT_FREE_SPACE_TOK);
			msgbox.ShowMessageOk(msg);
			return false;
		}
		return true;
	}
	return false;
}

short DCP::Common::card_status()
{
#ifdef CS35
		//return 1;
	ABL::AppSystemInfoC::eDevicesT eRet = ABL::AppSystemInfoC::Instance()->GetAvailableDevices();

	if(m_pModel->FILE_STORAGE1 == CPI::deviceLocalMemory)
	{
		if((eRet & ABL::AppSystemInfoC::eLocalMemory))
			return 1;
	}
	else
	{
		if((eRet & ABL::AppSystemInfoC::eUSB))
			return 1;
	}
	return 0;
#else
	ABL::AppSystemInfoC::eDevicesT eRet = ABL::AppSystemInfoC::Instance()->GetAvailableDevices();
		
	if(m_pModel->FILE_STORAGE1 == CPI::deviceSdCard)
	{
		if((eRet & ABL::AppSystemInfoC::eSDCard))
			return 1;
	}
	else
	{
		if((eRet & ABL::AppSystemInfoC::eUSB))
			return 1;
	}

	//if(/*1==*/(eRet & ABL::AppSystemInfoC::eSDCard/*:ePCCard*/))
	//{
	//	return 1;
	//}
	return 0;
#endif
}

short DCP::Common::read_allow_edit()
{
	return 1;
}

short DCP::Common::GetDate(int* iDay, int* iMonth, int* iYear)
{
	//static?DateTime?GUI::EditTimeCtrlC::GetActualTime() 
	DateTime oDateTime = GUI::GetActualTime();
	
	int year, day, month;
	int hour, min, seg;

	GMAT::SetMjd( (double)oDateTime, year, month, day, hour, min, seg );
	


	if(iDay && iMonth && iYear)
	{
		*iDay = day;
		*iMonth = month;
		*iYear = year;
	}
	return 1;
}

short DCP::Common::GetTime(int* iHour, int* iMin, int* iSec)
{
	
	//DateTime oDateTime = GUI::EditTimeCtrlC::GetActualTime();
	DateTime oDateTime = GUI::GetActualTime();
	
	int year, day, month;
	int hour, min, seg;

	GMAT::SetMjd( (double)oDateTime, year, month, day, hour, min, seg );
	


	if(iHour&& iMin && iSec)
	{
		*iHour = hour;
		*iMin = min;
		*iSec = seg;
	}
	return 1;
}


short DCP::Common::GetUserName(char *name)
{
	if(m_pModel)
	{
		StringC sUser = m_pModel->m_sUser;
		//UTL::UnicodeToAscii(name, sUser);
		BSS::UTI::BSS_UTI_WCharToAscii(sUser, name);

	}
	else
		name[0] = '\0';

	return 1;
}

short DCP::Common::GetInstrumentNo(int* no)
{
	TPI::InstrumentInfoC oInstrumentInfo;
	TPI::SensorC::Instance()->GetInstrumentInfo(oInstrumentInfo);

	const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentSerialNum();

	if(no)
		*no = atoi(cInstrumentNumber);

	
	return 1;
}

short DCP::Common::GetInstrumentName(char* intrument_name)
{
	TPI::InstrumentInfoC oInstrumentInfo;
	TPI::SensorC::Instance()->GetInstrumentInfo(oInstrumentInfo);

	//const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentName();
	const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentType();
	if(intrument_name)
		sprintf(intrument_name,"%-s",cInstrumentNumber);

	
	return 1;
}

/************************************************************************
*************************************************************************/
void DCP::Common::get_dist_(char *act, char *des, char *dist)
{
char temp[20];

	if(!strblank(act) && !strblank(des))
	{
		sprintf(temp,"%8.*f", m_pModel->m_nDecimals, atof(act) - atof(des));
		if(strlen(temp) > 8 || act[0] =='*' || des[0] =='*')
		{
			sprintf(temp,"%8.8s", "*******");
		}
	}
	else
	{
		sprintf(temp,"%8.8s","");
	}

	sprintf(dist,"%-8.8s",temp);
}

/************************************************************************
*************************************************************************/
void DCP::Common::copy_xyz_to_buffer(double *x, double *y, double *z, char *bX, char *bY, char *bZ, short pit, short dec)
{
		sprintf(bX,"%*.*f",pit,dec, *x); bX[pit] = '\0';
		sprintf(bY,"%*.*f",pit,dec, *y); bY[pit] = '\0';
		sprintf(bZ,"%*.*f",pit,dec, *z); bZ[pit] = '\0';
}

/************************************************************************
*************************************************************************/
void DCP::Common::copy_xyz_to_buffer_f(float *x, float *y, float *z, char *bX, char *bY, char *bZ, short pit, short dec)
{
		sprintf(bX,"%*.*f",pit,dec, *x);
		sprintf(bY,"%*.*f",pit,dec, *y);
		sprintf(bZ,"%*.*f",pit,dec, *z);
}


short DCP::Common::convert_to_ascii(StringC from, char *to, short iLen)
{
	//UTL::UnicodeToAscii(to, iLen, from);
	BSS::UTI::BSS_UTI_WCharToAscii(from, to,iLen);
	to[iLen] = '\0';
	return 1; 
}


StringC DCP::Common::get_info_text(short& ind)
{
		StringC ret;
		StringC ret2;
		char buff[100];
		short cid;
		TPI::MeasConfigC oMeasConfig;
		TPI::CorrectionC oCorrectionData;
		float fRef ;
		float fPPM;
		short iActivetool;

		switch(ind)
		{
			// CID
			case 0:
					cid = m_pModel->active_coodinate_system;
					if(cid == DCS)
					{
						ret = L"SCS";
						//sprintf(buff1,"%-8.8s","SCS");
					}
					else if(cid == OCSD)
					{
						ret = L"OCSd";
						//sprintf(buff1,"%-8.8s","OCSd");
					}
					else if(cid == OCSP)
					{
						ret = L"OCSp";
						//sprintf(buff1,"%-8.8s","OCSp");
					}
					else if(cid == OCSU)
					{
						ret = L"OCSu";
						//sprintf(buff1,"%-8.8s","OCSu");
					}

					else
					{
						ret = L"OCSc";
						//sprintf(buff1,"%-8.8s","OCSc");
					}

					cid = m_pModel->m_nUnit;//get_ACTIVE_UNIT();

					if(cid == MM)
					{
						ret2.LoadTxt(AT_DCP05,V_DCP_MM_TOK);
						ret +=  L"  ";
						ret +=	ret2;
						//sprintf(buff2,"%-8.8s", TextTokenToString(AppID, TXT_MM_TOK)); // "mm");
					}
					else if(cid == FEET)
					{
						ret2.LoadTxt(AT_DCP05,V_DCP_FEET_TOK);
						ret +=  L"  ";
						ret +=	ret2;

						//sprintf(buff2,"%-8.8s",TextTokenToString(AppID, TXT_FEET_TOK)); //"feet");
					}
					else
					{
						ret2.LoadTxt(AT_DCP05,V_DCP_INCH_TOK);
						ret +=  L"  ";
						ret +=	ret2;

						//sprintf(buff2,"%-8.8s",TextTokenToString(AppID, TXT_INCH_TOK));// "inch");
					}
					ind++;
					break;

			// USER
			case 1:  	
				ret.LoadTxt(AT_DCP05, K_DCP_USER_TOK);
				ret += "   ";
				ret += m_pModel->m_sUser;
				//sprintf(buff1,"%-8.8s",   TextTokenToString(AppID,TXT_USER_TOK));
				//get_active_user(buff2);
				ind++;
				break;

			// Prism constant
			case 2:	
						//TPI::MeasConfigC oMeasConfig;
						oMeasConfig.Get();
						/*float */fRef = oMeasConfig.GetReflectorConstant();

						//TPI::CorrectionC oCorrectionData;
						oCorrectionData.Get();
						/*float */fPPM = oCorrectionData.GetAtmoshericPPM();
						//TMC_GetPrismCorr(constant);

						sprintf(buff,"%+.1f   %8.4f",fRef * 1000, fPPM);
						ret = StringC(buff);
						//sprintf(buff2,"%8.3f",get_atm_ppm());
						ind++;
						break;

			// TOOL
			case 3:     
						#ifndef AMS_DCP05
						iActivetool = m_pModel->active_tool;
						if(iActivetool)
						//if(isToolActive() == TRUE)
						{
							//get_toolname(tmp);
							ret.LoadTxt(AT_DCP05,V_DCP_TOOL_TEXT_TOK);
							ret = ret + L":";
							ret += StringC(m_pModel->tool_table[iActivetool-1].tool_id);
						}
						else
						{
							// sprintf(buff2,"%-8.8s"," ");

						}
						ind++;
						#else
							ind++;
						#endif

						break;

			// AVERAGE MEAS
			case 4:
						if(m_pModel->m_nAverageCount > 1)
						//if(get_AVERAGE_ON_OFF() == TRUE)
						{
							ret.LoadTxt(AT_DCP05,V_DCP_AVERAGE_TEXT_TOK);
							//sprintf(buff1,"%-8.8s", TextTokenToString(AppID,TXT_AVERAGE_TOK));
							//sprintf(buff2,"%-8.8s"," ");
						}
						ind= 0;
						break;
		}
		return ret;
}


void DCP::Common::to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs)
{
double x_new, y_new, z_new;
double dist,theta,fii;
struct ams_vector xyz;
struct sphvect temp;
double point1[4], point2[4];
//double Ovalues[4];
//short table;

	xyz.x = 0.0; xyz.y = 0.0; xyz.z = 0.0;
	temp.r = 0.0; temp.theta = 0.0; temp.fii = 0.0;
	x_new = y_new = z_new = 0.0;

	hor = hor * DPR;
	ver = ver * DPR;

	// Hz
	theta = hor;

	// V
	fii=ver;
	fii = 90 - fii;

	/* distance */
	dist = dis * 1000;

	// table = get_prismtape_table();
	// dist = dist + check_calibration(dist, table);

	/* angles to radians */
	theta = degtorad(theta);
	fii = degtorad(fii);
		
		temp.r = dist;
		temp.theta = theta;
		temp.fii = fii;

		sphcart(&temp, &xyz); /* To xyz */

		// save scs values
		if(x_scs != 0 && y_scs != 0 && z_scs != 0)
		{
			*x_scs = xyz.x;
			*y_scs = xyz.y;
			*z_scs = xyz.z;
		}

		//if(get_LEFT_HAND_COORDINATE()== FALSE)
		if(m_pModel->m_nLeftRightHand == RIGHTHAND)
		{
			xyz.x = -xyz.x;	
		}
		else
		{
			xyz.x = xyz.x;
		}

		/***************************
			Conversion mm ->
		****************************/

		//if(get_ACTIVE_UNIT() == INCH)
		if(m_pModel->m_nUnit == INCH)
		{
			xyz.x = xyz.x / 25.400051;
			xyz.y = xyz.y / 25.400051;
			xyz.z = xyz.z / 25.400051;
		}
		else if(m_pModel->m_nUnit == FEET)
		//else if(get_ACTIVE_UNIT() == FEET)
		{
			xyz.x = xyz.x / 304.800612;
			xyz.y = xyz.y / 304.800612;
			xyz.z = xyz.z / 304.800612;
		}

		point1[0] = xyz.x;
		point1[1] = xyz.y;
		point1[2] = xyz.z;
		point1[3] = 1;

		if (m_pModel->active_coodinate_system == OCSP)
		//if (get_ACTIVE_CID() == OCSP)
		{
			ptrans(point1, m_pModel->ocsp_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

		//else if (get_ACTIVE_CID() == OCSD)
		else if (m_pModel->active_coodinate_system  == OCSD)
		{
			/*
			Ovalues[0] = ocsd_matrix[0][3];
			Ovalues[1] = ocsd_matrix[1][3];
			Ovalues[2] = ocsd_matrix[2][3];
			Ovalues[3] = ocsd_matrix[3][3];
			*/
			ptrans(point1,m_pModel->ocsd_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}
		
		//else if (get_ACTIVE_CID() == OCSC)
		else if (m_pModel->active_coodinate_system == OCSC)
		{
			ptrans(point1, m_pModel->ocsc_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}
		//else if (get_ACTIVE_CID() == OCSU)
		else if (m_pModel->active_coodinate_system == OCSU)
		{
			ptrans(point1,m_pModel->ocsu_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

		else
		{							  //DCS

			ptrans(point1,m_pModel->dcs_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

	// Added 19.3.1997

    #ifndef AMS_DCP05
	if(tool)
	{
		x_new = x_new - m_pModel->active_tool_x;
		y_new = y_new - m_pModel->active_tool_y;
		z_new = z_new - m_pModel->active_tool_z;
		
		/*
		x_new = x_new - get_TOOL_X_TOT();
		y_new = y_new - get_TOOL_Y_TOT();
		z_new = z_new - get_TOOL_Z_TOT();
		*/
	}
    #endif 

	*x = x_new;
	*y = y_new;
	*z = z_new;
}


void DCP::Common::to_AMS_xyz(double x_in, double y_in, double z_in, double *x_out, double *y_out, double *z_out)
{
double x_new, y_new, z_new;
//double dist,theta,fii;
//struct ams_vector xyz;
//struct sphvect temp;
double point1[4], point2[4];

	/*
	xyz.x = 0.0; xyz.y = 0.0; xyz.z = 0.0;
	temp.r = 0.0; temp.theta = 0.0; temp.fii = 0.0;
	x_new = y_new = z_new = 0.0;

	hor = hor * DPR;
	ver = ver * DPR;

	// Hz
	theta = hor;

	// V
	fii=ver;
	fii = 90 - fii;

	// distance 
	dist = dis * 1000;

	// table = get_prismtape_table();
	// dist = dist + check_calibration(dist, table);

	// angles to radians
	theta = degtorad(theta);
	fii = degtorad(fii);
		
		temp.r = dist;
		temp.theta = theta;
		temp.fii = fii;

		sphcart(&temp, &xyz); // To xyz 

		// save scs values
		if(x_scs != 0 && y_scs != 0 && z_scs != 0)
		{
			*x_scs = xyz.x;
			*y_scs = xyz.y;
			*z_scs = xyz.z;
		}
		*/

		
		// convert m to mm
		x_in = x_in * 1000.0;
		y_in = y_in * 1000.0;
		z_in = z_in * 1000.0;
		
		//if(get_LEFT_HAND_COORDINATE()== FALSE)
		if(m_pModel->m_nLeftRightHand == RIGHTHAND)
		{
			x_in = -x_in;
			//xyz.x = -xyz.x;	
		}
		else
		{
			//xyz.x = xyz.x;
		}

		/***************************
			Conversion mm ->
		****************************/

		//if(get_ACTIVE_UNIT() == INCH)
		if(m_pModel->m_nUnit == INCH)
		{
			x_in = x_in / 25.400051;
			y_in = y_in / 25.400051;
			z_in = z_in / 25.400051;
		}
		else if(m_pModel->m_nUnit == FEET)
		//else if(get_ACTIVE_UNIT() == FEET)
		{
			x_in = x_in / 304.800612;
			y_in = y_in / 304.800612;
			z_in = z_in / 304.800612;
		}

		point1[0] = x_in;
		point1[1] = y_in;
		point1[2] = z_in;
		point1[3] = 1;

		if (m_pModel->active_coodinate_system == OCSP)
		//if (get_ACTIVE_CID() == OCSP)
		{
			ptrans(point1, m_pModel->ocsp_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

		//else if (get_ACTIVE_CID() == OCSD)
		else if (m_pModel->active_coodinate_system  == OCSD)
		{

			ptrans(point1,m_pModel->ocsd_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}
		
		//else if (get_ACTIVE_CID() == OCSC)
		else if (m_pModel->active_coodinate_system == OCSC)
		{
			ptrans(point1, m_pModel->ocsc_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}
		//else if (get_ACTIVE_CID() == OCSU)
		else if (m_pModel->active_coodinate_system == OCSU)
		{
			ptrans(point1,m_pModel->ocsu_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

		else
		{							  //DCS

			ptrans(point1,m_pModel->dcs_matrix, &point2);
			x_new = point2[0];
			y_new = point2[1];
			z_new = point2[2];
		}

	//convert back to m unit
	if(m_pModel->m_nUnit == INCH)
	{
		x_new = x_new * 25.400051;
		y_new = y_new * 25.400051;
		z_new = z_new * 25.400051;
	}
	else if(m_pModel->m_nUnit == FEET)
	//else if(get_ACTIVE_UNIT() == FEET)
	{
		x_new = x_new * 304.800612;
		y_new = y_new * 304.800612;
		z_new = z_new * 304.800612;
	}

	// mm to m
	x_new = x_new / 1000.0;
	y_new = y_new / 1000.0;
	z_new = z_new / 1000.0;

	*x_out = x_new;
	*y_out = y_new;
	*z_out = z_new;
}


/************************************************************************
*************************************************************************/
short DCP::Common::get_rotation_status()
{
double deg;
short ACTIVE_PLANE, rotation=false;

		ACTIVE_PLANE = m_pModel->dom_active_plane; //get_active_plane_dom();

		if(ACTIVE_PLANE == XY_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.x;//get_x_plane_rot();
			if(deg != 0.0)
			{
			  rotation = true;
			} 

			deg = m_pModel->dom_rot_plane_buff.y;//get_y_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}
		} 
		else if(ACTIVE_PLANE == ZX_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.x;//get_x_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

			deg = m_pModel->dom_rot_plane_buff.z; //get_z_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}
		}
		else if(ACTIVE_PLANE == YZ_PLANE)
		{
			deg = m_pModel->dom_rot_plane_buff.y; //get_y_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

			deg = m_pModel->dom_rot_plane_buff.z;//get_z_plane_rot();
			if(deg != 0.0)
			{
				rotation = true;
			}

		}
		return rotation;
}

// *********************************************************************
// Laskee monta pistett� oetetaan laskentaan	
// *********************************************************************
short DCP::Common::defined_points_count_in_line(S_LINE_BUFF *line, short *lastpoint)
{
short count=0,i,sta;

	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_LINE; i++)
	{
		sta = line[0].points[i].sta;
		
		if(sta == 1 || sta == 2)
		{
			count++;
		}

		if(sta != 0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return count;
}

// *********************************************************************
//
// *********************************************************************
short DCP::Common::get_max_defined_point_line(S_LINE_BUFF *lines)
{
short ret=0,i;

	for(i=MAX_POINTS_IN_LINE-1;i >=0; i--)
	{
		if(lines[0].points[i].sta != 0 ||
		   !strblank(lines[0].points[i].point_id))
		{
			ret = i+1;
			break;
		}
	}
	return ret;
}


short  DCP::Common::defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint)
{
	short count=0,i,sta;

	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_PLANE; i++)
	{
		sta = plane[0].points[i].sta;

		if(sta == 1 || sta == 2)
			count++;
		
		if(sta != 0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return count;
}


/************************************************************************
*************************************************************************/
void DCP::Common::get_dist_len1(char *act, char *des, char *dist, short len)
{
char temp[20];
short i;

	if(!strblank(act) && !strblank(des))
	{
		sprintf(temp,"%*.*f", len, m_pModel->m_nDecimals, fabs(atof(act) - atof(des)));
		if(strlen(temp) > (unsigned) len || act[0] =='*' || des[0] =='*')
		{
			for(i=1;i<=len;i++)
				temp[i-1] = '*';
		}
	}
	else
	{
		sprintf(temp,"%*.*s",len,len,"");
	}

	sprintf(dist,"%-*.*s",len,len,temp);
}

/************************************************************************
*************************************************************************/
void DCP::Common::get_dist_len(char *act, char *des, char *dist, short len)
{
char temp[20];
short i;

	if(!strblank(act) && !strblank(des))
	{
		sprintf(temp,"%*.*f", len, m_pModel->m_nDecimals, atof(act) - atof(des));
		if(strlen(temp) > (unsigned) len || act[0] =='*' || des[0] =='*')
		{
			for(i=1;i<=len;i++)
				temp[i-1] = '*';
		}
	}
	else
	{
		sprintf(temp,"%*.*s",len,len,"");
	}

	sprintf(dist,"%-*.*s",len,len,temp);
}

// ************************************************************************
// ************************************************************************
void DCP::Common::calc_di(char *tulos,char *x1, char *y1, char *z1,char *x2, char *y2, char *z2)
{
double dist;
struct ams_vector m,n;

			empty_buffer(tulos,10);

			if(!strblank(x1) && !strblank(y1) && !strblank(z1) &&
				!strblank(x2) && !strblank(y2) && !strblank(z2))
			{
				m.x =  atof(x1);
				m.y =  atof(y1);
				m.z =  atof(z1);

				n.x = atof(x2);
				n.y = atof(y2);
				n.z = atof(z2);

			/* Calculate distance */
				dist = calc_point_dist_from_point(&m, &n);
				sprintf(tulos,"%10.*f",m_pModel->m_nDecimals,dist);
			}
}


// ************************************************************************
// ************************************************************************
short DCP::Common::defined_circle_points(S_CIRCLE_BUFF *circles, short *lastpoint)
{
short i,count,sta;
	
	count= 0;
	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0;i<MAX_POINTS_IN_CIRCLE;i++)
	{	
		sta = circles[0].points[i].sta;

		if(sta == 1 || sta == 2)
			count++;
		if(sta != 0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;	
		}
	}

	return count;
}


// *********************************************************************
//
// *********************************************************************
short DCP::Common::get_max_defined_point_circle(S_CIRCLE_BUFF *circles)
{
short ret=0,i;

	for(i=MAX_POINTS_IN_CIRCLE-1;i >=0; i--)
	{
		if(circles[0].points[i].sta != 0 ||
		   !strblank(circles[0].points[i].point_id))
		{
			ret = i+1;
			break;
		}
	}
	return ret;
}

/************************************************************************
*************************************************************************/
void DCP::Common::inc_id(char *id)
{
char apu[10],apu2[10],numstr[10];
short last,first,i,j,length,pit;
// int ret;

	 sprintf(apu,"%-6.6s",id);
	 strbtrim(apu);

	 pit = (short) strlen(apu);

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
/************************************************************************
*************************************************************************/
char* DCP::Common::strlower(char *s)
{
short a;

	while(*s)
	{
		a = *s;
		*s++ = tolower(a);
		
	}
	return s;
}

/************************************************************************
*************************************************************************/
bool DCP::Common::check_edm_mode()
{
	bool ret = true;

	TPI::MeasConfigC oMeasConfig;
	SDI::TPSMeasDataC::EDMModeT edm;

	oMeasConfig.Get();

	edm = oMeasConfig.GetEDMMode();

	//if(edm == TPI::MeasDataC::EM_Standard || edm == TPI::MeasDataC::EM_Fast ||edm == TPI::MeasDataC::EM_Averaging)
	if(edm == TPI::MeasDataC::EM_Standard || edm == TPI::MeasDataC::EM_Fast ||edm == TPI::MeasDataC::EM_Averaging || edm == TPI::MeasDataC::EM_Precise)
	{

	}
	else
	{
		MsgBox msgbox;
		StringC sMsg;

		sMsg.LoadTxt(AT_DCP05, M_DCP_WRONG_EDM_MODE_TOK);
		msgbox.ShowMessageOk(sMsg);
		ret = false;
	}

	//check also compensator
	bool ret1 =TBL::CheckCompensator();
	if(!ret1)
		ret = false;

	return ret;	
}

void DCP::Common::delay()
{
	GUI::ModalDelayC* delay = new GUI::ModalDelayC();
	delay->SetDelay(1);
	delay->Execute();
	//delete delay;
}

short  DCP::Common::find_first_file(const char* file_name, boost::filesystem::path* FileInfo)
{
	boost::filesystem::path oDirPath = file_name;
	boost::system::error_code errCode;
	if( boost::filesystem::exists( oDirPath, errCode ) )
	{
		*FileInfo = oDirPath;
		//boost::filesystem::directory_iterator itrDir( oDirPath );
		
		//*FileInfo = *itrDir;
		return 0;
	}
	return 1;
}

short DCP::Common::calc_mid_point(S_POINT_BUFF *p1, S_POINT_BUFF *p2, double *x, double *y, double *z)
{
	short ret = 1; 

	if(p1->sta != 0 && p2->sta != 0)
	{
		*x = (p1->x + p2->x) / 2.0; 
		*y = (p1->y + p2->y) / 2.0; 
		*z = (p1->z + p2->z) / 2.0; 
		ret = 0;
	}

	return ret;
}

short DCP::Common::calc_mid_point(S_POINT_BUFF *points ,double *x, double *y, double *z)
{
	short ret = 1; 
	
	double x_tot = 0.0;
	double y_tot = 0.0;
	double z_tot = 0.0;
	short count=0;

	for(int i=0; i < MAX_MID_POINTS; i++)
	{
		if(points[i].sta != 0)
		{
			x_tot += points[i].x;
			y_tot += points[i].y;
			z_tot += points[i].z;

			count ++;
		}
	}
	if(count >= 2)
	{
		*x = x_tot / count; 
		*y = y_tot / count; 
		*z = z_tot / count; 
		ret = 0;
	}
	return ret;
}