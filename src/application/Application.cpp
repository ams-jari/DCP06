// ================================================================================================
// 
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Job Management
//
// $Workfile: Application.cpp $
//
// Summary  : DCP06 application entry point, job management, license handling
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 8.07.04 15:34 $
// $Revision: 2 $
// $Modtime: 8.07.04 15:31 $

/* $ History: $
*/ 
// $NoKeywords: $

#include "dcp06/core/CS35.hpp"

#include "stdafx.h"  
#include <stdlib.h>
#include <stdio.h>
#include <ABL_AppSystemInfo.hpp>
#include "GUI_AppFileInfo.hpp"

#include "dcp06/core/Defs.hpp"
#include <dcp06/application/Application.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Logger.hpp>

#include <dcp06/core/MsgBox.hpp>


#include <dcp06/init/Initialization.hpp>
#include <dcp06/init/Unit.hpp>
#include <dcp06/orientation/Alignment321.hpp>
#include <dcp06/orientation/Alignment321UserDef.hpp>
#include <dcp06/orientation/BestFit.hpp>
#include <dcp06/orientation/Chst.hpp>
#include <dcp06/measurement/3dmeas.hpp>
#include <dcp06/measurement/3dfbs.hpp>
#include <dcp06/calculation/CalculationMenu.hpp>
#include <dcp06/calculation/CalculationDist.hpp>
#include <dcp06/calculation/CalculationAngle.hpp>
#include <dcp06/application/ApplicationMenu.hpp>
#include <dcp06/file/DistFile.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/file/AngleFile.hpp>
#include <dcp06/file/CircleFile.hpp>
#include <dcp06/file/ShaftFile.hpp>
#include <dcp06/application/Shaft.hpp>
#include <dcp06/application/LineSetting.hpp>
//#include <dcp06/core/MsgBox.hpp>
#include "dcp06/core/InputText.hpp"
#include <dcp06/calculation/CalculationCircle.hpp>
#include <dcp06/measurement/PlaneScanning.hpp>
#include <dcp06/application/LineFitting.hpp>
#include <ABL_Types.hpp>
#include <ABL_LoadableApps.hpp>
#include <Onboard_Tok.hpp>
//#include <ABL_AppSystemInfo.hpp>
#include <TPI_Sensor.hpp>
#include <TPI_InstrumentInfo.hpp>
#include <TPI_MeasConfig.hpp>
#include <GMAT_UnitConverter.hpp>
#include <GUI_HourGlass.hpp>
//#include <GUI_Timer.hpp>
#include <GUI_Application.hpp>
#include <GUI_Desktop.hpp>
#include <UTL_StringFunctions.hpp>
#include <ABL_MsgDef.hpp>
#include <dcp06/measurement/AdvScan.hpp>
#include <GUI_Util.hpp>
#include "dcp06/core/Defs.hpp"
#include <dcp06/core/Common.hpp>
#include <dcp06/core/DemoLicense.hpp>

#include <GMAT_UnitConverter.hpp>

 

//#include <boost/date_time/gregorian/gregorian_types.hpp>
//#include <boost/date_time/gregorian/greg_duration.hpp>
//lint -e714 Symbol not referenced

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Registrations  =======================================
// ================================================================================================

namespace DCP
{
    APP_ENTRY_POINT( AT_DCP06, 15751, DCP::Application ) // DCP06: App.Id 15751
}     
   
using namespace DCP;
   
namespace ABL
{
//lint -save -e19 Useless Declaration
REGISTER_COMMAND_FACTORY_APPLICATION(Application, AT_DCP06,
                                     C_DCP_APPLICATION_NAME_TOK, 
                                     L_DCP_APPLICATION_NAME_TOK, AT_DCP06);
}
//lint -restore

// ================================================================================================
// ======================================  Static functions  ======================================
// ================================================================================================


// ================================================================================================
// ====================================  Application  =======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Application::Application():m_pModel(0),poConfigController(0)
{
	DCP06_TRACE_ENTER;
	m_pCode[0] = '\0';

    // Application ID for text database tokens (DCP06.men)
    SetTxtApplicationId( AT_DCP06 ); //lint !e1506 Call to virtual function within a contructor or destructor
    // TODO EI OLE VIVA
	//SetCaptionTok( C_DCP_APPLICATION_NAME_TOK );
	
	//DCP::Model* pModel = new Model;
	//(void)/*DCP::*/Application::SetModel( pModel );

	  //lint -save -e1732 -e1733 new in constructor for class which has no assignment operator
    // Create and set the model

	//DCP::MenuDialog* m_pMenuDlg = new DCP::MenuDialog();
	//AddDialog(MAIN_MENU,m_pMenuDlg,true); 

		
		
	StringC serialNumber = get_code(m_pCode);

	DCP::Controller* pDCP06Controller = new DCP::Controller(m_pCode);
	(void)AddController( 1, pDCP06Controller );
	//(void)GetController( 1 )->SetModel( pModel );
	
	
    //lint -save -e1732 -e1733 new in constructor for class which has no assignment operator
    // Create and set the model
    /*ConfigController**/ poConfigController = new ConfigController(pDCP06Controller/*, pModel*/);
	//AddConfigPanelController(poConfigController);

	//DCP::Model* pModel = new Model;
		
    // Removed namespace for eVC compability (WinCE Compiler) 
    //(void)/*DCP::*/Application::SetModel( pModel );
	 (void)/*DCP::*/Application::SetModel( poConfigController->GetModel()/*pModel*/ );
	 (void)GetController( 1 )->SetModel( poConfigController->GetModel() );

	 poConfigController->GetModel()->poConfigController = 	poConfigController;

     // Mind config model
     m_pModel = poConfigController->GetModel();
	
 	// is Motorized
     TPI::InstrumentInfoC instInfo;
	 TPI::SensorC::Instance()->GetInstrumentInfo(instInfo);
	 
	 // VIVA EI
	 //TPI::InstrumentInfoC::DeviceConfig devConf = instInfo.GetDeviceConfig();
	 TPI::InstrumentInfoC::DeviceConfigMaskT devConf =instInfo.GetDeviceConfig();

	 if(devConf & TPI::InstrumentInfoC::DC_IsMotorized)
	 {
		 m_pModel->isMotorized = true;
	 }
	 if(devConf & TPI::InstrumentInfoC::DC_HasATR)
	 {
		 m_pModel->isATR = true;
	 }
 
	m_pModel->SerialNumber = serialNumber;
	DCP06_TRACE_EXIT;

	
	/*
	 if(devConf & TPI::InstrumentInfoC::DC_HasScanning)
	 {
		m_pModel->isScanning = true;
	 }
	 if(devConf & TPI::InstrumentInfoC::InstrumentModelT::IMO_MS60)
	 {
		m_pModel->isScanning = true;
	 }
	 */
}  //lint !e429 Custodial pointer has not been freed or returned


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::Application::~Application()
{
	DCP06_TRACE_ENTER;
	//bool ret = poConfigController->StoreConfigData();		
	int x;
	x=1;

	if(poConfigController)
	{
		delete poConfigController;
		poConfigController = 0;
	}
	/*
	if(m_pModel)
	{
		delete m_pModel;
		m_pModel = 0;
	}
*/
	DCP06_TRACE_EXIT;
}	

// ================================================================================================
// Description: // Description: Start application
// ================================================================================================
void DCP::Application::Run(/* bool bShowStartDialog NOT CAPTIVATE*/ )
{
	DCP06_TRACE_ENTER;
    (void)SetActiveController( 1, true );

	// Removed namespace for eVC compability (WinCE Compiler) 
    /*GUI::*/ApplicationC::Run(/* bShowStartDialog NOT CAPTIVATE */ );

	DateTime oDateTime = GUI::GetActualTime();

	if(oDateTime == 0.0)
	{
		MsgBox msgBox;
		StringC sTemp = L"Reading current date/time failed!";
		msgBox.ShowMessageOk(sTemp);
		Close(EC_KEY_CONT);
		DCP06_TRACE_EXIT;
		return;
	}
	
	

	//poConfigController->GetModel()->SetConfigKey(CNF_KEY_DEMO_LICENSES);
	//poConfigController->StoreConfigData();*/


	DCP::InfoDialog* poInfoDlg = new DCP::InfoDialog(dynamic_cast< DCP::Model* >( GetModel()));
	AddDialog(INFO_DLG,poInfoDlg); 	
	SetActiveDialog(INFO_DLG, true);

    GUI::BaseDialogC* pDialog = GetActiveDialog();
    if (pDialog)
    {
       // pDialog->SetTxtApplicationId( GetTxtApplicationId() );
    }
	DCP06_TRACE_EXIT;
}

bool DCP::Application::ConfirmClose(bool bEsc)
{
	DCP06_TRACE_ENTER;
		MsgBox msgbox;
		StringC msg;
		msg.LoadTxt(AT_DCP06,M_DCP_QUIT_PROGRAM_TOK);
		if(msgbox.ShowMessageYesNo(msg))
		{
			poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
			poConfigController->StoreConfigData();
			
			poConfigController->GetModel()->SetConfigKey(CNF_KEY_A321);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_BESTFIT);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_CHST);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_CIRCLE);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_USERDEF);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_SHAFT);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_LICENSE);
			poConfigController->StoreConfigData();

			poConfigController->GetModel()->SetConfigKey(CNF_KEY_LINE_FITTING);
			poConfigController->StoreConfigData();


			DCP06_TRACE_EXIT;
			return true;
		}
		DCP06_TRACE_EXIT;
		return false;
}

// ================================================================================================
// Description:  OnActiveControllerClosed
// ================================================================================================
void DCP::Application::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06_TRACE_ENTER;
	DCP06_TRACE_POINT("ctrl=%d exit=%d", lCtrlID, lExitCode);
	Close(lExitCode,true);
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: OnActiveDialogClosed
// ================================================================================================
void DCP::Application::OnActiveDialogClosed(int lDlgID, int lExitCode)
{
	DCP06_TRACE_ENTER;
	DCP06_TRACE_POINT("dlg=%d exit=%d", lDlgID, lExitCode);
	if(lDlgID == INFO_DLG)
	{
		char keyCodeBuffer[21];
		keyCodeBuffer[0] = '\0';

		DestroyDialog(lDlgID);
		
		poConfigController->GetModel()->bDemoMode = true;
		
		get_code(keyCodeBuffer);


		if(poConfigController->GetModel()->startDate > 0.0)
		{
			DateTime oDateTime = GUI::GetActualTime();

			if(poConfigController->GetModel()->lastStartedDate > oDateTime)
			{
				MsgBox msgBox;
				StringC sTemp;
				sTemp = L"The current date is < the latest startup date of the DCP06!!!";
				msgBox.ShowMessageOk(sTemp);
				DestroyDialog(lDlgID);
				Close(EC_KEY_CONT);
				DCP06_TRACE_EXIT;
				return;
			}
		}

		DemoLicense demo(poConfigController->GetModel());

		// FULL MODE
		if(strcmp(poConfigController->GetModel()->sKeyCode,keyCodeBuffer) == 0 && keyCodeBuffer[0] != '\0')
		{
				poConfigController->GetModel()->bDemoMode = false;
		}
		else
		{
			if(poConfigController->GetModel()->startDate == 0.0) // keycode not entered correctly -> show license dialog
			{
				DCP::LicenseDialog* poLisDlg = new DCP::LicenseDialog(dynamic_cast< DCP::Model* >( GetModel()));
				AddDialog(REG_DLG,poLisDlg); 	
				SetActiveDialog(REG_DLG, true);
			}
			else if(demo.get_available_days(poConfigController->GetModel()->sKeyCodeDemo1, poConfigController->GetModel()->startDate) <= 0)
			{
				DCP::LicenseDialog* poLisDlg = new DCP::LicenseDialog(dynamic_cast< DCP::Model* >( GetModel()));
				AddDialog(REG_DLG,poLisDlg); 	
				SetActiveDialog(REG_DLG, true);					
			}
			else if(demo.get_available_days(poConfigController->GetModel()->sKeyCodeDemo1, poConfigController->GetModel()->startDate)  > 0)
			{
				// poConfigController->GetModel()->startDate = 0.0; // reset demo start date
				// memset(poConfigController->GetModel()->sKeyCodeDemo1, '\0', 10);
			
				DateTime oDateTime = GUI::GetActualTime();
				poConfigController->GetModel()->lastStartedDate = oDateTime;
				poConfigController->GetModel()->SetConfigKey(CNF_KEY_DEMO_LICENSES);
				poConfigController->StoreConfigData();
			}
		}
	}
	else if(lDlgID == REG_DLG)
	{

		if(lExitCode != EC_KEY_CONT)
		{
			MsgBox msgBox;
			StringC sTemp;
			sTemp = L"The license key was not accepted, restart program and try again!";
			msgBox.ShowMessageOk(sTemp);
			DestroyDialog(lDlgID);
			Close(EC_KEY_CONT);
			DCP06_TRACE_EXIT;
			return;
		}

		DateTime oDateTime = GUI::GetActualTime();

		char keyCodeBuffer[21];
		keyCodeBuffer[0] = '\0';
		
		get_code(keyCodeBuffer);
		
		// FULL MODE
		if(strcmp(poConfigController->GetModel()->sEnteredKeyCode, keyCodeBuffer) == 0 && keyCodeBuffer[0] != '\0')
		{	
			sprintf(poConfigController->GetModel()->sKeyCode,"%s", poConfigController->GetModel()->sEnteredKeyCode);
			poConfigController->GetModel()->startDate = 0.0; // reset demo start date
			memset(poConfigController->GetModel()->sKeyCodeDemo1, '\0', 10);
			
			poConfigController->GetModel()->SetConfigKey(CNF_KEY_DEMO_LICENSES);
			poConfigController->StoreConfigData();
			
			poConfigController->GetModel()->bDemoMode = false;
		}

		else 
		{
			DemoLicense demo(poConfigController->GetModel());

			// first run, check demo mode 1
			if(poConfigController->GetModel()->startDate == 0.0) // keycode not entered correctly -> show license dialog
			{
				demo.get_code_demo(1, keyCodeBuffer);

				// if correct code save it
				if(strcmp(poConfigController->GetModel()->sEnteredKeyCode, keyCodeBuffer) == 0 && keyCodeBuffer[0] != '\0')
				{
					// save keycode and startDate
					sprintf(poConfigController->GetModel()->sKeyCodeDemo1,"%s", poConfigController->GetModel()->sEnteredKeyCode);
					poConfigController->GetModel()->startDate = oDateTime;
					poConfigController->GetModel()->lastStartedDate = oDateTime;

					poConfigController->GetModel()->SetConfigKey(CNF_KEY_DEMO_LICENSES);
					poConfigController->StoreConfigData();
				}
				else
				{ 
					MsgBox msgBox;
					StringC sTemp;
					sTemp = L"The license key was not accepted, restart program and try again!";
					msgBox.ShowMessageOk(sTemp);
					Close(EC_KEY_CONT);
				}
			}

			// check if demo keycode is valid....
			else if(demo.is_license_ok(poConfigController->GetModel()->sEnteredKeyCode,poConfigController->GetModel()->startDate))
			{
				sprintf(poConfigController->GetModel()->sKeyCodeDemo1,"%s", poConfigController->GetModel()->sEnteredKeyCode);
				poConfigController->GetModel()->startDate = oDateTime;
				poConfigController->GetModel()->lastStartedDate = oDateTime;	
				poConfigController->GetModel()->SetConfigKey(CNF_KEY_DEMO_LICENSES);
				poConfigController->StoreConfigData();
				
			}
			else 
			{ 
				MsgBox msgBox;
				StringC sTemp;
				sTemp = L"The license key was not accepted, restart program and try again!";
				msgBox.ShowMessageOk(sTemp);
				Close(EC_KEY_CONT);
			}
		}
	
		poConfigController->GetModel()->SetConfigKey(CNF_KEY_LICENSE);
		poConfigController->StoreConfigData();
		
		DestroyDialog(lDlgID);
	}
	DCP06_TRACE_EXIT;
	return;
}

//bool DCP::Application::check_demo_mode(char *code)
//{
//	// if key not entered	
//	return true;
//}
/*
 #define SEC_KEY 	 "a"
 #define PROG_ID 	 "dphfbtyu"
 #define APPL_ID      "vrtjshg"
*/

#define SEC_KEY 	 "d"
#define PROG_ID 	 "dphfdtyv"
#define APPL_ID      "vqujsig"



StringC DCP::Application::get_code(char *sCode)
{
int nro=1L;
char *p, *p1,*p2;
char serialBuffer[STRING_BUFFER_SMALL];
int i,len;
short hashValues[STRING_BUFFER_SMALL];

unsigned int code = 0L;
StringC ret = L"";

#if !defined( CS20) && !defined(CS35)
    int snro;
	TPI::InstrumentInfoC oInstrumentInfo;
	TPI::SensorC::Instance()->GetInstrumentInfo(oInstrumentInfo);
 
	const char* cInstrumentNumber = oInstrumentInfo.GetInstrumentSerialNum();
	snro = atol(cInstrumentNumber);

	 sprintf(serialBuffer,"%-lu",snro);
	
#elif defined(CS20)
	MsgBox msgBox;

	StringC ss1 = L"";
	RcT ret1= CPI::SensorC::GetInstance()->GetSerialNumber(ss1);
	sprintf(serialBuffer,"%d",ret1);

	char asciiBuffer[STRING_BUFFER_SMALL];
	BSS::UTI::BSS_UTI_WCharToAscii(ss1, asciiBuffer);
 
	sprintf(serialBuffer,"%-s",asciiBuffer);

	//msgBox.ShowMessageOk(StringC(ssName), StringC(temp));

#elif defined(CS35)
	DCP::CS35 cs35;
	cs35.get_serialnumber(serialBuffer);
#endif
	 len = (int) strlen(serialBuffer);
	 serialBuffer[len] = '\0';
 
	 ret = StringC(serialBuffer);

     code = 0L;

	 p = serialBuffer;
	 p1 = PROG_ID;
	 p2 = SEC_KEY;
     nro= 1L;

    // ******************
    // Basic
    // ******************
    for(i=0;i<len;i++)
    {
        if(*p != *p1)
        hashValues[i] =(short)( ((*p*i*4) ^ (*p1) + (*p+i) ^ (*p2-1) + *p1 * *p2)); //*1.7);
		p++;
		p1++;
		p2++;

		nro = nro*hashValues[i]+i;

		if(*p1 == '\0') p1 = PROG_ID;
		if(*p2 == '\0') p2 = SEC_KEY;
    }

	code =(unsigned int) nro;

    while(code <= 9999999L)
    {
       code  = (unsigned int) (code * 2); //.5);
    }
	sprintf(sCode,"%lu",code);
	
	return ret; // palauta laitteen sarjanumero
}


// ================================================================================================
// ====================================  MenuDialog  ===========================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
MenuDialog::MenuDialog()
{
	//SetTxtApplicationId(AT_DCP06);
} 



// ================================================================================================
// Description: Destructor
// ================================================================================================
MenuDialog::~MenuDialog()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void MenuDialog::OnInitDialog(void)
{
	GUI::GraphMenuDialogC::OnInitDialog(); 

	FKDef vDef;
	vDef.poOwner = this;
	//vDef.nAppId = AT_DCP06;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey(FK1, vDef);

	
	SetTitle(StringC(AT_DCP06,T_DCP_MAIN_MENU_TOK ));
	//SetHelpTok(H_DCP_MAIN_MENU_TOK,0);
	
	//AddItem(StringC(AT_DCP06,L_DCP_INITIALIZATION_TOK),11);
	/*AddItem(L"", L"", StringC(AT_DCP06,L_DCP_INITIALIZATION_TOK),L"", nullptr, 11);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_ORIENTATION_TOK),L"",nullptr,12);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_FILE_TOK),L"",nullptr,13);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_MEASUREMENT_TOK),L"",nullptr,		14);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_CALCULATION_TOK),L"",nullptr,		15);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_APPLICATION_TOK),L"",nullptr,		16);*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Settings_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Settings_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_INITIALIZATION_TOK),L"", nullptr, 11);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Orientation_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Orientation_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_ORIENTATION_TOK),L"",nullptr,12);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"File_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"File_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_FILE_TOK),L"",nullptr,13);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Measurement_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Measurement_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_MEASUREMENT_TOK),L"",nullptr,14);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Calculation_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Calculation_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_CALCULATION_TOK),L"",nullptr,15);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Application_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Application_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_APPLICATION_TOK),L"",nullptr,16);


	//DisableMenuItem(11);
}

// ================================================================================================
// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
// ================================================================================================
void MenuDialog::OnSelectionDone(void)
{
	short unId = GetSelected();

	switch (unId) {
		// initialization
		case 11:
				break;
		// orientation

		case 12:
				break;
		case 13:
				break;

		case 14:
				break;
	}
	OnF1Pressed();

}

// ================================================================================================
// Description: SetModel
// ================================================================================================
bool DCP::MenuDialog::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: GetModel
// ================================================================================================
DCP::Model* DCP::MenuDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// Description: F1
// ================================================================================================
void MenuDialog::OnF1Pressed(void)
{
	if(GetSelected() ==  2)
		;
	else
		Close(GetSelected());
	/*
	if(GetSelected() == 12)
	{
		Close(EC_KEY_QUIT);
	}
	else
	*/
	{
		//Close(GetSelected());
	}
}



// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::Controller::Controller(char* code) 
    : m_pMenuDlg( nullptr ),m_pCode(code)
{
	SetTxtApplicationId( AT_DCP06 ); 
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle( StringC(AT_DCP06, C_DCP_APPLICATION_NAME_TOK ));

    // Create a dialog
	/*DCP::MenuDialog**/ m_pMenuDlg = new DCP::MenuDialog();
	AddDialog(MAIN_MENU,m_pMenuDlg,true); 

    // Set the function key
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    //vDef.nLable = K_SET_TOK;
    vDef.poOwner = this;
    //SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_INFO_TOK);
	SetFunctionKey(SHFK2, vDef);

	

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::Controller::~Controller()
{

}
/*
bool DCP::Controller::ConfirmClose(bool bEsc)
{

		MsgBox msgbox;
		StringC msg;
		msg.LoadTxt(AT_DCP06,M_DCP_QUIT_PROGRAM_TOK);
		if(msgbox.ShowMessageYesNo(msg))
		{
			return true;
		}
		return false;

}*/
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::Controller::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );
	

	// check if demo mode -> show REGISTRATION BUTTON
	//Model* dcp05Model = dynamic_cast< DCP::Model* >(pModel());
	
	DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

	if(pDcpModel->startDate != 0.0) // = Demo mode
	{
		FKDef vDef;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06,K_DCP_REGISTRATION_TOK);
		SetFunctionKey(SHFK4, vDef);
	}

    // Set it to hello world dialog
    return m_pMenuDlg->SetModel( pDcpModel );
}


// ================================================================================================
// Description: F1
// ================================================================================================
void DCP::Controller::OnF1Pressed()
{
    if (m_pMenuDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pMenuDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: SHF2 Info
// ================================================================================================
void Controller::OnSHF2Pressed(void)
{
	DCP::InfoDialog* poInfoDlg = new DCP::InfoDialog(dynamic_cast< DCP::Model* >( GetModel()));
	AddDialog(INFO_DLG,poInfoDlg); 	
	SetActiveDialog(INFO_DLG, true);
}

void Controller::OnSHF4Pressed(void)
{
	DCP::LicenseDialog* poLisDlg = new DCP::LicenseDialog(dynamic_cast< DCP::Model* >( GetModel()));
	AddDialog(REG_DLG,poLisDlg); 	
	SetActiveDialog(REG_DLG, true);
}
// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::Controller::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == REG_DLG)
	{
		// check if correct key is entered -> activate full software
		if(lExitCode == EC_KEY_CONT)
		{

			DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >(GetModel());

			if(strcmp(pDcpModel->sEnteredKeyCode, m_pCode) == 0 && m_pCode[0] != '\0')
			{	
				sprintf(pDcpModel->sKeyCode,"%s", pDcpModel->sEnteredKeyCode);
				pDcpModel->startDate = 0.0; // reset demo start date
				memset(pDcpModel->sKeyCodeDemo1, '\0', 10);
				
				pDcpModel->SetConfigKey(CNF_KEY_DEMO_LICENSES);
				pDcpModel->poConfigController->StoreConfigData();
				
				//poConfigController->GetModel()->bDemoMode = false;
			}
			else
			{
				MsgBox msgBox;
				StringC sTemp;
				sTemp = L"The license key was not accepted!";
				msgBox.ShowMessageOk(sTemp);
			}
		}
	}
	else if(lDlgID == MAIN_MENU)
	{
		if(lExitCode == 2)
		{
			SetActiveDialog(MAIN_MENU, true);
		}

		
		// INITIALIZATION
		else if(lExitCode == 11)
		{
			if(GetController(INIT_CONTROLLER) == nullptr)
			{
				(void)AddController( INIT_CONTROLLER, new DCP::InitializationController(false) );
			}
			(void)GetController( INIT_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(INIT_CONTROLLER, true);
			
		}
		else if(lExitCode == EC_KEY_QUIT)
		{
			(void) Close(lExitCode, true);
		}	
		
		// Orientation
		else if(lExitCode == 12) 
		{
			DCP::OrientationMenuDialog* poMenuDlg = new DCP::OrientationMenuDialog(dynamic_cast< DCP::Model* >(GetModel()));
			AddDialog(ORIENTATION_MENU,poMenuDlg); 	
			// TODO VIVA
			//poMenuDlg->SetTxtApplicationId( GetTxtApplicationId());
			SetActiveDialog(ORIENTATION_MENU, true);
		}
		// File
		else if(lExitCode == 13) 
		{

			
			DCP::FileMenuDialog* poMenuDlg = new DCP::FileMenuDialog();
			AddDialog(FILE_MENU,poMenuDlg); 	
			// TODO VIVA	
			//poMenuDlg->SetTxtApplicationId( GetTxtApplicationId());
			SetActiveDialog(FILE_MENU, true);
			
		}
		// Measurement
		else if(lExitCode == 14) 
		{
			if(GetController(_3DMEAS_CONTROLLER) == nullptr)
			{
				(void)AddController( _3DMEAS_CONTROLLER, new DCP::Meas3DController(false, dynamic_cast< DCP::Model* >(GetModel())));
			}
			(void)GetController( _3DMEAS_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(_3DMEAS_CONTROLLER, true);

		}
		// Calculation
		else if(lExitCode == 15) 
		{
			DCP::CalculationMenuDialog* poMenuDlg = new DCP::CalculationMenuDialog(dynamic_cast< DCP::Model* >(GetModel()));
			AddDialog(CALCULATION_MENU,poMenuDlg); 	
			// TODO VIVA
			//poMenuDlg->SetTxtApplicationId( GetTxtApplicationId());
			SetActiveDialog(CALCULATION_MENU, true);

		}
		// Application
		else if(lExitCode == 16) 
		{
			DCP::ApplicationMenuDialog* poMenuDlg = new DCP::ApplicationMenuDialog(dynamic_cast< DCP::Model* >(GetModel()));
			AddDialog(APPLICATION_MENU,poMenuDlg); 	
			// TODO VIVA
			//poMenuDlg->SetTxtApplicationId( GetTxtApplicationId());
			SetActiveDialog(APPLICATION_MENU, true);

		}
		else
		{
			//MsgBox msgbox;
			//msgbox.ShowMessageOk(L"Joo");
			//SetActiveDialog(MAIN_MENU, true);	
		}
	}

	// ORIENTATION
	else if (lDlgID ==ORIENTATION_MENU)
	{
		if(lExitCode == 2)
		{
			SetActiveDialog(MAIN_MENU, true);
		}
		else if(lExitCode == DCP_A321_MENU)
		{
			//SetActiveDialog(1, true);
			
			if(GetController(A321_CONTROLLER) == nullptr)
			{
				(void)AddController( A321_CONTROLLER, new DCP::Alignment321Controller );
			}
			(void)GetController( A321_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(A321_CONTROLLER, true);
		}
		else if(lExitCode == DCP_BESTFIT_MENU)
		{
			if(dynamic_cast< DCP::Model* >(GetModel())->ADFFileName.GetLength() == 0)
			{
				MsgBox msgbox;		
				StringC msg;
				msg.LoadTxt(AT_DCP06, M_DCP_ADF_NOT_OPEN_TOK);

				if(msgbox.ShowMessageYesNo(msg))
				{
					if(GetController(FILE_CONTROLLER_BESTFIT) == nullptr)
					{
						(void)AddController( FILE_CONTROLLER_BESTFIT, new DCP::FileController(dynamic_cast< DCP::Model* >(GetModel())) );
					}
					(void)GetController( FILE_CONTROLLER_BESTFIT)->SetModel( Controller::GetModel());
					SetActiveController(FILE_CONTROLLER_BESTFIT, true);
				}
				else
				{
					if(GetController(BESTFIT_CONTROLLER) == nullptr)
					{
						(void)AddController( BESTFIT_CONTROLLER, new DCP::BestFitController(dynamic_cast< DCP::Model* >(GetModel())) );
					}
					(void)GetController( BESTFIT_CONTROLLER )->SetModel( Controller::GetModel());
					SetActiveController(BESTFIT_CONTROLLER, true);
				}
			}
			else
			{
				if(GetController(BESTFIT_CONTROLLER) == nullptr)
				{
					(void)AddController( BESTFIT_CONTROLLER, new DCP::BestFitController(dynamic_cast< DCP::Model* >(GetModel())) );
				}
				(void)GetController( BESTFIT_CONTROLLER )->SetModel( Controller::GetModel());
				SetActiveController(BESTFIT_CONTROLLER, true);
			}

		}
		else if(lExitCode == DCP_CHANGE_STATION_MENU)
		{
			if(dynamic_cast< DCP::Model* >(GetModel())->ADFFileName.GetLength() == 0)
			{
				MsgBox msgbox;		
				StringC msg;
				msg.LoadTxt(AT_DCP06, M_DCP_ADF_NOT_OPEN_TOK);

				if(msgbox.ShowMessageYesNo(msg))
				{
					if(GetController(FILE_CONTROLLER_CHST) == nullptr)
					{
						(void)AddController( FILE_CONTROLLER_CHST, new DCP::FileController(dynamic_cast< DCP::Model* >(GetModel())) );
					}
					(void)GetController( FILE_CONTROLLER_CHST)->SetModel( Controller::GetModel());
					SetActiveController(FILE_CONTROLLER_CHST, true);
				}
				else
				{
					if(GetController(CHST_CONTROLLER) == nullptr)
					{
						(void)AddController( CHST_CONTROLLER, new DCP::ChangeStationController(dynamic_cast< DCP::Model* >(GetModel())) );
					}
					(void)GetController( CHST_CONTROLLER )->SetModel( Controller::GetModel());
					SetActiveController(CHST_CONTROLLER, true);
				}
			}
			else
			{
				if(GetController(CHST_CONTROLLER) == nullptr)
				{
					(void)AddController( CHST_CONTROLLER, new DCP::ChangeStationController(dynamic_cast< DCP::Model* >(GetModel())) );
				}
				(void)GetController( CHST_CONTROLLER )->SetModel( Controller::GetModel());
				SetActiveController(CHST_CONTROLLER, true);
			}

		}
		else if(lExitCode == DCP_USER_DEFINED_MENU)
		{
			if(GetController(A321_USERDEF_CONTROLLER) == nullptr)
			{
				(void)AddController( A321_USERDEF_CONTROLLER, new DCP::Alignment321UserDefController(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			(void)GetController( A321_USERDEF_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(A321_USERDEF_CONTROLLER, true);
		}
	}	
	// FILE MENU
	else if (lDlgID == FILE_MENU)
	{
		if(lExitCode == 2)
		{
			SetActiveDialog(MAIN_MENU, true);
		}
		else if(lExitCode == DCP_3DFILE_MENU)
		{
			if(GetController(FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( FILE_CONTROLLER, new DCP::FileController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( FILE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(FILE_CONTROLLER, true);
		}
		else if(lExitCode == DCP_DIST_FILE_MENU)
		{
			if(GetController(DIST_FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( DIST_FILE_CONTROLLER, new DCP::DistFileController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( DIST_FILE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(DIST_FILE_CONTROLLER, true);
		}
		else if(lExitCode == DCP_ANGLE_FILE_MENU)
		{
			if(GetController(ANGLE_FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( ANGLE_FILE_CONTROLLER, new DCP::AngleFileController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( ANGLE_FILE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(ANGLE_FILE_CONTROLLER, true);

		}
		else if(lExitCode == DCP_CIRCLE_FILE_MENU)
		{
			if(GetController(CIRCLE_FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( CIRCLE_FILE_CONTROLLER, new DCP::CircleFileController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( CIRCLE_FILE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(CIRCLE_FILE_CONTROLLER, true);

		}
		else if(lExitCode == DCP_SHAFT_FILE_MENU)
		{
			if(GetController(SHAFT_FILE_CONTROLLER) == nullptr)
			{
				(void)AddController( SHAFT_FILE_CONTROLLER, new DCP::ShaftFileController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( SHAFT_FILE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(SHAFT_FILE_CONTROLLER, true);

		}

	}
	// APPLICATION MENU
	else if (lDlgID == APPLICATION_MENU)
	{
		if(lExitCode == 2)
		{
			SetActiveDialog(MAIN_MENU, true);
		}
		else if(lExitCode == LINE_SETTING)
		{
			MsgBox msgBox;
			short ret=1;
			
			DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( Controller::GetModel() );

			// check if 321 line and horizontal plane is defined	
			if(pDcpModel->align321_line_buff[0].sta == 0)
			{
				StringC sMsg;
				sMsg.LoadTxt(AT_DCP06,M_DCP_DEFINE_LINE_LSET_TOK);
				msgBox.ShowMessageOk(sMsg);
				ret = 0;
			}
			
			if(ret==1 && !pDcpModel->align321_hz_plane)
			{
				StringC sMsg;
				sMsg.LoadTxt(AT_DCP06,M_DCP_HZ_PLANE_NOT_DEFINED_TOK);
				msgBox.ShowMessageOk(sMsg);
				ret = 0;
			}
			
			if(ret)
			{

				if(GetController(LINE_SETTING_CONTROLLER) == nullptr)
				{
					(void)AddController( LINE_SETTING_CONTROLLER, new DCP::LineSettingController );
				}
				(void)GetController( LINE_SETTING_CONTROLLER )->SetModel( Controller::GetModel());
				SetActiveController(LINE_SETTING_CONTROLLER, true);
			}
			else
				SetActiveDialog(MAIN_MENU, true);	
		}
		else if(lExitCode == FRONT_BACK_SINGLE)
		{
			if(GetController(_3DFBS_CONTROLLER) == nullptr)
			{
				(void)AddController( _3DFBS_CONTROLLER, new DCP::Fbs3DController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( _3DFBS_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(_3DFBS_CONTROLLER, true);
		}

		else if(lExitCode == SHAFT_ALIGMENT)
		{
			if(GetController(SHAFT_ALIGMENT_CONTROLLER) == nullptr)
			{
				(void)AddController( SHAFT_ALIGMENT_CONTROLLER, new DCP::ShaftController(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			(void)GetController( SHAFT_ALIGMENT_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(SHAFT_ALIGMENT_CONTROLLER, true);

		}
		else if(lExitCode == SIMPLE_SCANNING)
		{
			if(GetController(SIMPLE_SCAN_CONTROLLER) == nullptr)
			{
				(void)AddController(SIMPLE_SCAN_CONTROLLER, new DCP::PlaneScanController(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			(void)GetController( SIMPLE_SCAN_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(SIMPLE_SCAN_CONTROLLER, true);
		}
		
		else if(lExitCode == LINE_FITTING)
		{
			if(GetController(LINE_FITTING_CONTROLLER) == nullptr)
			{
				(void)AddController( LINE_FITTING_CONTROLLER, new DCP::LineFitController(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			(void)GetController( LINE_FITTING_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(LINE_FITTING_CONTROLLER, true);
		}

		#ifdef TS16
		#else
		if(lExitCode == SCANNING)
		{
			if(GetController(SCAN_CONTROLLER) == nullptr)
			{
				(void)AddController(SCAN_CONTROLLER, new DCP::DCPScanningControllerC(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			//(void)GetController(SCAN_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(SCAN_CONTROLLER, true);
		}
		
		#endif

	}
	// CALCULATION MENU
	else if (lDlgID == CALCULATION_MENU)
	{
		if(lExitCode == 2)
		{
			SetActiveDialog(MAIN_MENU, true);
		}
		else if(lExitCode == CALC_DISTANCE)
		{
						
			if(GetController(CALC_DIST_CONTROLLER) == nullptr)
			{
				(void)AddController( CALC_DIST_CONTROLLER, new DCP::CalculationDistController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( CALC_DIST_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(CALC_DIST_CONTROLLER, true);
			
		}
		else if(lExitCode == CALC_ANGLE)
		{
			if(GetController(CALC_ANGLE_CONTROLLER) == nullptr)
			{
				(void)AddController( CALC_ANGLE_CONTROLLER, new DCP::CalculationAngleController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( CALC_ANGLE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(CALC_ANGLE_CONTROLLER, true);

		}
		else if(lExitCode == CALC_CIRCLE)
		{
			if(GetController(CALCULATION_CIRCLE_CONTROLLER) == nullptr)
			{
				(void)AddController( CALCULATION_CIRCLE_CONTROLLER, new DCP::CalculationCircleController(dynamic_cast< DCP::Model* >( Controller::GetModel())));
			}
			(void)GetController( CALCULATION_CIRCLE_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(CALCULATION_CIRCLE_CONTROLLER, true);

		}

	}
	
	else
        SetActiveDialog(MAIN_MENU, true);

	if(lDlgID != MAIN_MENU)
		DestroyDialog(lDlgID);
}

// ================================================================================================
// Description: OnActiveControllerClosed
// ================================================================================================
void DCP::Controller::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DestroyController(lCtrlID);
	
	if(lExitCode == EC_KEY_QUIT)
	{
		(void) Close(lExitCode, true);
	}	
	else
	{
		if(lCtrlID == MAIN_MENU)
		{
			//(void) Close(lExitCode, false);
			SetActiveDialog(MAIN_MENU, true);

			//DestroyController(1);
		}
		else if(lCtrlID == FILE_CONTROLLER_BESTFIT)
		{
			if(GetController(BESTFIT_CONTROLLER) == nullptr)
			{
				(void)AddController( BESTFIT_CONTROLLER, new DCP::BestFitController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( BESTFIT_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(BESTFIT_CONTROLLER, true);
		}
		else if(lCtrlID == FILE_CONTROLLER_CHST)
		{
			if(GetController(CHST_CONTROLLER) == nullptr)
			{
				(void)AddController( CHST_CONTROLLER, new DCP::ChangeStationController(dynamic_cast< DCP::Model* >(GetModel())) );
			}
			(void)GetController( CHST_CONTROLLER )->SetModel( Controller::GetModel());
			SetActiveController(CHST_CONTROLLER, true);
		}
		else
			SetActiveDialog(MAIN_MENU, true);
	}
}



// ================================================================================================
// ====================================  OrientationMenuDialog  ================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
OrientationMenuDialog::OrientationMenuDialog(Model* pModel): m_pModel(pModel)
{

	//SetTxtApplicationId( AT_DCP06 );
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
OrientationMenuDialog::~OrientationMenuDialog()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void OrientationMenuDialog::OnInitDialog(void)
{
	ResetFunctionKeys();

	FKDef vDef;
	vDef.poOwner = this;
	//vDef.nAppId = AT_DCP06;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey(FK1, vDef);
	
	// HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK2, vDef1 );
	SetFunctionKey( SHFK6, vDef1 );

	
	//SetHelpTok(H_DCP_ORIENTATION_TOK,0);
	
	SetTitle(StringC(AT_DCP06,T_DCP_ORIENTATION_MENU_TOK ));
	/*
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_DOM_TOK),L"", nullptr, DCP_A321_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_POM_TOK),L"", nullptr, DCP_BESTFIT_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_CHANGE_STATION_TOK),L"", nullptr,	DCP_CHANGE_STATION_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_USER_DEFINED_TOK),L"", nullptr, DCP_USER_DEFINED_MENU);
	*/

	// TODO: Rename icons DOM_*.png and POM_*.png to Alignment321_*.png and BestFitAlignment_*.png in SWXRes when DCP06 icon set is updated
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"DOM_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"DOM_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_DOM_TOK),L"", nullptr, DCP_A321_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"POM_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"POM_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_POM_TOK),L"",nullptr, DCP_BESTFIT_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Change_Station_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Change_Station_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_CHANGE_STATION_TOK),L"",nullptr, DCP_CHANGE_STATION_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"User_Def_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"User_Def_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_USER_DEFINED_TOK),L"",nullptr, DCP_USER_DEFINED_MENU);
	
	/* CAPTIVATE TBD
	if(m_pModel->bDemoMode)
	{	
		DisableMenuItem(DCP_BESTFIT_MENU);
		DisableMenuItem(DCP_CHANGE_STATION_MENU);
		DisableMenuItem(DCP_USER_DEFINED_MENU);
	}
	*/
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
//

// ================================================================================================
// Description: OnSelectionDone
// ================================================================================================
void OrientationMenuDialog::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void OrientationMenuDialog::OnF1Pressed(void)
{
/*	if(m_pModel->bDemoMode && GetSelected() != DCP_A321_MENU)
	{
		MsgBox msgBox;
		msgBox.ShowMessageOk(StringC(AT_DCP06,M_DCP_NOT_AVAILABLE_IN_DEMO_TOK ));
		Close(EC_KEY_ESC);
	}
	else
	{*/	
		Close(GetSelected());
	//}
	/*
	{
		Close(GetSelected());
	}
	*/
}


// ================================================================================================
// ====================================  FileMenuDialog ========================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
FileMenuDialog::FileMenuDialog()
{
	//SetTxtApplicationId( GetTxtApplicationId() );
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
FileMenuDialog::~FileMenuDialog()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void FileMenuDialog::OnInitDialog(void)
{
	
	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey(FK1, vDef);
	
	// HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );

	//SetHelpTok(H_DCP_FILE_MENU_TOK,0);
	SetTitle(StringC(AT_DCP06,T_DCP_FILE_MENU_TOK ));

	/*
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_3DFILE_TOK),L"", nullptr,		DCP_3DFILE_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_DIST_FILE_TOK), L"", nullptr,	DCP_DIST_FILE_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_ANGLE_FILE_TOK),L"", nullptr,	DCP_ANGLE_FILE_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_CIRCLE_FILE_TOK),L"", nullptr,DCP_CIRCLE_FILE_MENU);
	AddItem(L"", L"", StringC(AT_DCP06,L_DCP_SHAFT_FILE_TOK),L"", nullptr,	DCP_SHAFT_FILE_MENU);
	*/

	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"3D_file_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"3D_file_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_3DFILE_TOK),L"", nullptr, DCP_3DFILE_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Distace_file_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Distace_file_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_DIST_FILE_TOK),L"",nullptr,DCP_DIST_FILE_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Angle_file_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Angle_file_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_ANGLE_FILE_TOK),L"",nullptr,DCP_ANGLE_FILE_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Circle_file_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Circle_file_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_CIRCLE_FILE_TOK),L"",nullptr,DCP_CIRCLE_FILE_MENU);
	AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Shaft_align_file_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Shaft_align_file_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_SHAFT_FILE_TOK),L"",nullptr,DCP_SHAFT_FILE_MENU);
	//AddItem(GUI::GetAppResourceUrl(AT_DCP06,"Leica_data_formats_$SCALEFACTOR$.png"), GUI::GetAppResourceUrl(AT_DCP06,"Leica_data_formats_$SCALEFACTOR$.png"), StringC(AT_DCP06,L_DCP_MEASUREMENT_TOK),L"",nullptr,DCP_LEICA_DATA_FORMATS_MENU);
	
}


// ================================================================================================
// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
// ================================================================================================

void FileMenuDialog::OnSelectionDone(void)
{
	short unId = GetSelected();
	
	OnF1Pressed();
}

// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
void FileMenuDialog::OnF1Pressed(void)
{
	/*
	if(GetSelected() == 12)
	{
		Close(EC_KEY_QUIT);
	}
	else
	*/
	{
		Close(GetSelected());
	}
}


// ================================================================================================
// Description: OnF1Pressed
// ================================================================================================
DCP::LicenseDialog::LicenseDialog(Model* pModel):m_pText(0),m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
	m_pModel->sEnteredKeyCode[0] = '\0';
}

DCP::LicenseDialog::~LicenseDialog()
{

}
void DCP::LicenseDialog::OnInitDialog(void)
{
	ResetFunctionKeys();

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );


	/*vDef.strLable = StringC(AT_DCP06,K_DCP_EXIT_TOK);
	SetFunctionKey( FK6, vDef );*/

	/*if(m_pModel->iStartCount >= 30)
	{
		DisableFunctionKey(FK4);
	}*/

  // Hide quit 
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );
	
	//InsertEmptyLine();
	//InsertEmptyLine();

	m_pText = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pText->SetId(1);
	//m_pText->GetStringInputCtrl()->SetAllowedCharsDef(GUI::EditCtrlC::AC_NumInt);
	m_pText->SetText(StringC(AT_DCP06,P_DCP_REGISTRATION_TOK));
	m_pText->GetStringInputCtrl()->SetAllowedCharsDef(GUI::EditStringCtrlC::AC_NumInt);
	//m_pText->SetAutoColon(false);
	//m_pText->SetColonPosition(9 * 11);
	AddCtrl(m_pText);

	//SetHelpTok(H_DCP_REGISTRATION_TOK,0);
}

void DCP::LicenseDialog::OnDialogActivated()
{
	RefreshControls();
} 

void DCP::LicenseDialog::OnF1Pressed()
{
	int ret = 0;
	if(!m_pText->GetStringInputCtrl()->IsEmpty())
	{
		StringC sNo = m_pText->GetStringInputCtrl()->GetString();

		char asciiBuffer[21];
		BSS::UTI::BSS_UTI_WCharToAscii(sNo, asciiBuffer,20);
		sprintf(m_pModel->sEnteredKeyCode,"%s", asciiBuffer); // was sKeyCode...

		//m_pModel->lKeyCode = (unsigned int) atol((const char*) temp);
		Close(1);

	}
	else
		m_pModel->sEnteredKeyCode[0] = '\0'; // was sKeyCode...
}

void DCP::LicenseDialog::OnF6Pressed()
{
	m_pModel->sEnteredKeyCode[0] = '\0'; 
	Close(0);
}

void DCP::LicenseDialog::RefreshControls()
{
	if(m_pText)
	{	
	}
}

DCP::InfoDialog::InfoDialog(Model* pModel):m_pText(0),m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
}

DCP::InfoDialog::~InfoDialog()
{

}
void DCP::InfoDialog::OnInitDialog(void)
{ 
// CAPTIVATE TBD
	ResetFunctionKeys();

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	StringC s = "";
	StringC s1 = "";
	//int heigth=0, width= 0;
	GUI::RectT rect;

	s = StringC(AT_DCP06,K_DCP_OK_TOK);
	s1 = StringC(0,K_DCP_OK_TOK);
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_OK_TOK);

	SetFunctionKey( FK5, vDef );

  // Hide quit 
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );
	SetFunctionKey( SHFK2, vDef1 );

	//GUI::DesktopC::Instance()->GetSizeClient(rect); CAPTIVATE
	//m_pText = new GUI::TextBoxCtrlC(0,0,rect.Width(),240);CAPTIVATE
	m_pText = new GUI::TextCtrlC();
	m_pText->SetId(1);

	//m_pText->SetAlign(AlignmentT::AL_CENTER);CAPTIVATE
	AddCtrl(m_pText);
	
	SetTitle(StringC(AT_DCP06,T_DCP_ABOUT_TOK));
}

void DCP::InfoDialog::OnDialogActivated()
{
	RefreshControls();
} 

void DCP::InfoDialog::OnF5Pressed()
{
	Close(1);
}  

void DCP::InfoDialog::RefreshControls()
{
	if(m_pText)
	{	  
		// 3.x
		/*StringC sVers(m_pModel->sProgramVersion);
		StringC sDate(m_pModel->sProgramDate);*/
		StringC sMsg;
//#ifdef DCP05_LEICA_DEMO
//		sMsg = L"             DEMO\n ";
//#else       

		if(m_pModel->sKeyCode[0] == '\0' && m_pModel->startDate > 0)
		{
			DemoLicense demo(m_pModel);
			
			double mjd = GMAT::GetMjd(2018, 6,1,1,0,0);

			int days = demo.get_available_days(m_pModel->sKeyCodeDemo1, m_pModel->startDate);
			if(days <= 0)
				sMsg.Format(L"          Demo license expired.\n ", days);
			else
				sMsg.Format(L"   Demo license. Expires in %d days\n ", days);
		}     
//#endif    
#if defined TS16   
		sMsg += L"	      DCP05 (TSxx)\n ";
#elif defined CS35
		sMsg += L"	      DCP05 (CS35)\n ";
#elif defined CS20
		sMsg += L"	      DCP05 (CS20)\n ";
#else		 
		sMsg += L"	      DCP05 (MS60)\n ";
#endif   
		  
		sMsg += L"    Dimensional Control Program\n ";
		//sMsg += L"       Version 2.00 22.11.2016 ";
		sMsg += L"      Version 10.00 24.02.2026 ";
		sMsg += L"\n"; 
		sMsg +=L"          Copyright @A.M.S.\n";
		sMsg +=L" Accuracy Management Services Ltd\n";
		sMsg +=L"            All rights reserved";

		/*
		//       123456789012345678901234567890
		sMsg = L"  Dimensional Control Program\n ";
		sMsg +=L"   Version 2.21.05 21.08.2007\n";
		sMsg +=L"       Copyright @A.M.S.\n";
		sMsg +=L" Accuracy Management Services Ltd\n";
		sMsg +=L"      All rights reserved";
		*/
		sMsg += "\n\n(sn:";
		sMsg += m_pModel->SerialNumber;
		sMsg += ")";

		m_pText->SetText(sMsg);
	}
}