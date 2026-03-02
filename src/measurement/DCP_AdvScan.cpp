// ====================================================================================================================
//
// Project  : Viva
//
// Component: Hello World Scanning
//
// Summary  : Controller
//
// --------------------------------------------------------------------------------------------------------------------
//
// Copyright 2013 by Leica Geosystems AG, Heerbrugg
//
// ====================================================================================================================

#include "stdafx.h"
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/measurement/DCP_AdvScan.hpp>

//#include <GUI_TabbedDialog.hpp>
#include <GUI_Desktop.hpp>
#include <ABL_CommandFactory.hpp>
#include <ABL_ControllerFactory.hpp>
#include <SBL_ScanningHelperInterface.hpp>
#include <Sio_SdbReader.hpp>
#include <Sio_SdbPoint.hpp>
#include <SIO_PointIterator.hpp>
#include <CPI_Modjul.hpp>
#include <UTL_UnitConfig.hpp>
#include <UTL_StringFunctions.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>


// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ====================================================================================================================
// =================================================   Helper   =======================================================
// ====================================================================================================================

const double dPI = 3.1415926535897932384626433832795;

double Gon2Rad(double dValue)
{
    return dValue / 200.0 * dPI;
}





// ====================================================================================================================
// ==============================================   ScanningDialogC   =================================================
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
//
// Constructor
//
DCP::DCPScanningDialogC::DCPScanningDialogC(DCPScanningModelC* poScanningModel)
: StandardDialogC()
, m_poAvailableScan(NULL)
, m_poScanningModel(poScanningModel)
{
    m_poAvailableScan = new GUI::ComboLineCtrlC( GUI::ComboLineCtrlC::IC_ComboBox );
    m_poAvailableScan->SetText(StringC(AT_DCP05, L_DCP_AVAIL_SCAN_TOK));
    m_poAvailableScan->SetId(DCPScanningModelC::MI_AVAILABLE_SCAN);
    AddCtrl(m_poAvailableScan);

    GUI::ComboLineCtrlC* poEstimatedPoints = new GUI::ComboLineCtrlC( GUI::ComboLineCtrlC::IC_Integer );
    poEstimatedPoints->SetText(StringC(AT_DCP05, P_DCP_NUMBER_OF_POINTS_TOK));
    poEstimatedPoints->SetId(DCPScanningModelC::MI_SCAN_NUMPOINTS);
    USER_APP_VERIFY( poEstimatedPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable) );
    AddCtrl(poEstimatedPoints);

	GUI::ComboLineCtrlC* poScanStatus = new GUI::ComboLineCtrlC( GUI::ComboLineCtrlC::IC_String );
    poScanStatus->SetText(StringC(AT_DCP05, P_DCP_POINT_STATUS_TOK));
    poScanStatus->SetId(DCPScanningModelC::MI_SCAN_STATUS);
    USER_APP_VERIFY( poScanStatus->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable) );
    AddCtrl(poScanStatus);


}

// --------------------------------------------------------------------------------------------------------------------
//
// Destructor
//
DCP::DCPScanningDialogC::~DCPScanningDialogC()
{
    m_poAvailableScan = NULL;
    m_poScanningModel = NULL;
}

// --------------------------------------------------------------------------------------------------------------------
//
// OnShowDialog
//
void DCP::DCPScanningDialogC::OnDialogActivated()
{
    // call base
    StandardDialogC::OnDialogActivated();
	
    // force notification on tab changed
    if(m_poAvailableScan)
    {
        m_poAvailableScan->GetSubject()->Notify(GUI::NC_ONCOMBOBOX_SELECTION_CHANGED, DCP::DCPScanningModelC::MI_AVAILABLE_SCAN);
    }
}

void DCP::DCPScanningDialogC::RefreshControls()
{
    // Update the combobox with all available scans
    if(m_poAvailableScan && m_poScanningModel)
    {
        m_poAvailableScan->GetComboBoxInputCtrl()->SetEmpty();

        uint16_t i = 0;
        for(std::vector<StringC>::iterator iter = m_poScanningModel->m_vecScanList.begin(); iter != m_poScanningModel->m_vecScanList.end(); iter++, i++)
        {
            (void)m_poAvailableScan->GetComboBoxInputCtrl()->AddItem(*iter, i);
        }
    }

    // call base
    StandardDialogC::RefreshControls();
}



// ====================================================================================================================
// =======================================   ScanningControllerC   ====================================================
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
//
// Constructor
//
OBS_IMPLEMENT_EXECUTE(DCP::DCPScanningControllerC);

DCP::DCPScanningControllerC::DCPScanningControllerC(DCP::DCP05ModelC* pDCP05Model) 
: GUI::GControllerC()
, MeasurementC()
, m_poScanningModel(new DCP::DCPScanningModelC())
, m_spScan()
, m_poSurveyModel(new TBL::SurveyModelC())
//, m_dHorizScanV(70.0)
, m_pOnApplicationClosedObserver(OBS_METHOD_TO_PARAM0(DCPScanningControllerC, OnApplicationClosed), this)
,m_poDCPScanningDialogC(0)
,m_pDCP05Model(pDCP05Model)
{

	m_pCommon = new DCP05CommonC(m_pDCP05Model);

    // Set Title
    SetTitle(StringC(AT_DCP05, T_DCP_ADV_SCANNING_TOK));

    // Create Tabbed dialog
    //GUI::TabbedDialogC* poTabbedDialog = new GUI::TabbedDialogC();
    //USER_APP_VERIFY( AddDialog(DLG_Scanning, poTabbedDialog, true) );

    // Create and add dialog 
     m_poDCPScanningDialogC = new DCP::DCPScanningDialogC(m_poScanningModel);
	(void)AddDialog( ADV_SCAN_DLG, m_poDCPScanningDialogC, true );
   // USER_APP_VERIFY( poTabbedDialog->AddPage(poScanningPredefDlg, StringC(AT_HEWScanning, TT_PREDEFINED_SCAN_TOK)) );

    //HEW::ScanningAvailableDialogC* poScanningAvailDlg = new HEW::ScanningAvailableDialogC(m_poScanningModel);
    //USER_APP_VERIFY( poTabbedDialog->AddPage(poScanningAvailDlg, StringC(AT_HEWScanning, TT_AVAILABLE_SCAN_TOK)) );

    // Set model
    USER_APP_VERIFY( SetModel(m_poScanningModel) ); //lint !e1506 Call to virtual function

    // Initialize model
    UpdateAvailableScans();

    // Set survey model and create measure controller
    SetSurveyModel(m_poSurveyModel);
    USER_APP_VERIFY( AddGuardController(CTL_MeasStart, CreateMeasStartController(TBL::MSF_NoConnectionConf)) );

    // Set user event handler
    SetUserEventHandler(new GUI::UserEventHandlerC()); //lint !e1506 Call to virtual function

    // Update function keys
    SetFunctionKeys();

	m_pOnApplicationClosedObserver.Attach(GUI::AppMgntC::Instance()->GetAppCloseSubject());
} //lint !e429

// --------------------------------------------------------------------------------------------------------------------
//
// Destructor
//
DCP::DCPScanningControllerC::~DCPScanningControllerC(void)
{
    m_poScanningModel = NULL; //lint !e423
    m_poSurveyModel = NULL;   //lint !e423

	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// --------------------------------------------------------------------------------------------------------------------
//
// SetFunctionKeys
//
void DCP::DCPScanningControllerC::SetFunctionKeys()
{
    FK_DefinitionT  vFKDef;
    vFKDef.bEnabled = true;
    vFKDef.poOwner  = this;
	/*
    vFKDef.strLable = StringC(0, K_ADD_TOK);
    SetFunctionKey(FK1, vFKDef);

    vFKDef.strLable = L"SDB";
    SetFunctionKey(FK2, vFKDef);
	*/
    vFKDef.strLable = StringC(AT_DCP05, K_DCP_NEW_TOK);
    SetFunctionKey(FK1, vFKDef);

    vFKDef.strLable = StringC(AT_DCP05, K_DCP_CONFIG_TOK);
    SetFunctionKey(FK2, vFKDef);

    vFKDef.strLable = StringC(AT_DCP05, K_DCP_START_TOK);
    SetFunctionKey(FK3, vFKDef);

	vFKDef.strLable = StringC(AT_DCP05, K_DCP_EXPORT_TOK);
    SetFunctionKey(FK5, vFKDef);
	DisableFunctionKey (FK5);

	vFKDef.strLable = StringC(AT_DCP05, K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vFKDef );

	/*
    vFKDef.strLable = L"AutoS";
    SetFunctionKey(SHFK5, vFKDef);
	*/
    ActivateFunctionKeys();

	ReadDb();
}

// --------------------------------------------------------------------------------------------------------------------
//
// OnUserEvent
//

void DCP::DCPScanningControllerC::OnUserEvent(int iParam1, int iId)
{
    // call base class
    GUI::GControllerC::OnUserEvent(iParam1, iId);
    bool bUpdateInfo = false;
	int selectedId = -1;
   
    if(iParam1 == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED && iId == DCP::DCPScanningModelC::MI_AVAILABLE_SCAN)
    {
        int iAvailableScan;
        USER_APP_VERIFY( m_poScanningModel->GetItemVal(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, iAvailableScan) );
		
		//USER_APP_VERIFY( m_poScanningModel->GetSelectedId(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, selectedId) );

        if(iAvailableScan != -1)
        {
            boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
            ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
            if (spScanningHelper)
            {
                SDI::SmartPtrScanC spScan;
                if(spScanningHelper->GetScan(m_poScanningModel->m_vecScanList[iAvailableScan], spScan))
                {
                    m_spScan = CreateScanCopy(spScan);
                }
            }
        }
        else
        {
            m_spScan.reset();
        }

        bUpdateInfo = true;
    }

    // Update estimated points and estimated time
    if(bUpdateInfo)
    {
        boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
        ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
        if (spScanningHelper && m_spScan)
        {
            // Get Estimate Points
            uint64_t ullEstimatedPoints = 0;
            (void)spScanningHelper->GetNumberOfEstimatedPoints(m_spScan, ullEstimatedPoints);
            USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_NUMPOINTS, static_cast<int>(ullEstimatedPoints)) );

			// ScanName
			//SDI::SmartPtrScanC spScan;
			//int iAvailableScan;
			//USER_APP_VERIFY( m_poScanningModel->GetItemVal(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, iAvailableScan) );
            //spScanningHelper->GetScan(m_poScanningModel->m_vecScanList[iAvailableScan], spScan);
			StringC scanName = StringC(m_spScan->GetScanName());
			GUI::DesktopC::Instance()->MessageShow(scanName);
			// Scan state
			SDI::ScanC::ScanStateEnumT scanState;
			scanState = m_spScan->GetScanState();
			
			if (scanState == SDI::ScanC::SS_SCANNED )
			{
				StringC strScanned = StringC(AT_DCP05, V_DCP_SCANNED_TOK);
				USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_STATUS, strScanned));
			}

			else if (scanState == SDI::ScanC::SS_DEFINED) {
				USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_STATUS, StringC(AT_DCP05, V_DCP_DEFINED_TOK)));
			}

			else {
				USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_STATUS, StringC(AT_DCP05, V_DCP_NOT_DEFINED_TOK)));
			}
			

            // Get Estimated Scan time
            double dScanTime = 0.0;
            (void)spScanningHelper->GetEstmatedScanTime(m_spScan, dScanTime);

            int iScanTime = static_cast<int>(dScanTime);
            int iSeconds = iScanTime % 60;
            int iMinutes = (iScanTime/60) % 60;
            int iHours   = (iScanTime/60/60);

            StringC strScanTime;
            (void)strScanTime.Format( L"%d:%02d:%02d", iHours, iMinutes, iSeconds );
            //USER_APP_VERIFY( m_poScanningModel->SetItemVal(HEW::ScanningModelC::MI_SCAN_TIME, strScanTime) );   

			if(scanState == SDI::ScanC::SS_SCANNED )
			{
				EnableFunctionKey(FK5);
			}
			else
			{
				DisableFunctionKey(FK5);
			}
			
        }
        else
        {
			
            USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_NUMPOINTS, 0) );
			USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_STATUS, StringC(L"-")));
            /*
			StringC strScanTime;
            (void)strScanTime.Format( L"%d:%02d:%02d", 0, 0, 0 );
            USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_SCAN_TIME, strScanTime) ); 
			*/
        }
		GUI::DesktopC::Instance()->UpdateFunctionKeys();
    }
}

// --------------------------------------------------------------------------------------------------------------------
//
// OnF1Pressed
//
/*
void HEW::DCPScanningControllerC::OnF1Pressed(void)
{
    if(m_spScan)
    {
        // Create and set scan name (need to be unique)
        StringC strScanName;
        CPI::ModjulC oTime;
        oTime.SetFromUTCTime();

        (void)strScanName.Format(L"HZ_%03.0f°", m_dHorizScanV);        
        m_dHorizScanV += 10.;
        USER_APP_VERIFY( m_spScan->SetScanName(strScanName) );

        // store scan to database
        boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
        ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
        if (spScanningHelper)
        {
            USER_APP_VERIFY( spScanningHelper->StoreScanToDatabase(m_spScan) );
        }
    }
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// OnF2Pressed
//
/*
void HEW::ScanningControllerC::OnF2Pressed(void)
{
    // get scan from database
    boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
    ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
    if (spScanningHelper)
    {
        int iAvailableScan;
        USER_APP_VERIFY( m_poScanningModel->GetItemVal(HEW::ScanningModelC::MI_AVAILABLE_SCAN, iAvailableScan) );

        if(iAvailableScan == -1)
        {
            // no scan available
            return;
        }
        SDI::SmartPtrScanC spScan;
        if(spScanningHelper->GetScan(m_poScanningModel->m_vecScanList[iAvailableScan], spScan))
        {
            // read SDB
            if ( spScan->GetScanState() == SDI::ScanC::SS_SCANNED )
            {
                StringC strPathFileName = L"C:\\Users\\gruh\\Documents\\System Viva\\TS Viva\\SD Card\\DBX\\Default_0001_0118_215924\\Scans\\";
                //strPathFileName += spScan->GetScanDataFileName();
                //strPathFileName += L".sdb";
                strPathFileName += L"HEW_Scan_841304964_180114_210718.sdb";
                SIO::SdbReaderC mSdbReader;
                //if ( CPI::RC_OK == mSdbReader.OpenFile( spScan->GetScanDataFilePath(), spScan->GetScanDataFileName() ))
                RcT oRcT;
                oRcT = mSdbReader.OpenFile( strPathFileName );
                if ( 0 == oRcT )
                {
                    GUI::DesktopC::Instance()->MessageShow(spScan->GetScanDataFileName());
                    SIO::SdbReaderIteratorC mSdbReaderIterator( 1000 );

                    std::vector<SIO::SdbPointC> rvecSdbPoints;
                    if ( 0 < mSdbReader.ReadPoints( mSdbReaderIterator, rvecSdbPoints ) )
                    {
                        StringC strMsg;
                        (void)strMsg.Format( L"E:%.5f N:%.5f H:%.5f", rvecSdbPoints[0].m_dEasting, rvecSdbPoints[0].m_dNorthing, rvecSdbPoints[0].m_dHeight );
                        GUI::DesktopC::Instance()->MessageShow(strMsg);
                    }
                    mSdbReader.CloseFile();
                }
                else
                {
                    StringC strMsg;
                    (void)strMsg.Format( L"RCT:%d", oRcT );
                    GUI::DesktopC::Instance()->MessageShow(strMsg);
                }
            }
        }
    }
}
*/

// --------------------------------------------------------------------------------------------------------------------
//
// OnF3Pressed
//
void DCP::DCPScanningControllerC::OnF1Pressed(void)
{
    ABL::CommandFactoryI* poFactory = ABL::CommandFactoryContainerC::Instance()->GetFactory(ABL::CMD_SCAN_CREATE_DEFINITION_ID);
    if( !poFactory || !poFactory->CanExecute() )
    {
        return;
    }

    USER_APP_VERIFY( ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_SCAN_CREATE_DEFINITION_ID) );
}

// --------------------------------------------------------------------------------------------------------------------
//
// OnF4Pressed
//
void DCP::DCPScanningControllerC::OnF2Pressed(void)
{
    ABL::CommandFactoryI* poFactory = ABL::CommandFactoryContainerC::Instance()->GetFactory(ABL::CMD_SCAN_CONFIGURATION_ID);
    if( !poFactory || !poFactory->CanExecute() )
    {
        return;
    }

    USER_APP_VERIFY( ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_SCAN_CONFIGURATION_ID) );
}

// --------------------------------------------------------------------------------------------------------------------
//
// OnF5Pressed
//
void DCP::DCPScanningControllerC::OnF3Pressed(void)
{
	ReadDb();

    ABL::CommandFactoryI* poFactory = ABL::CommandFactoryContainerC::Instance()->GetFactory(ABL::CMD_SCAN_START_ID);
    if( !poFactory || !poFactory->CanExecute() )
    {
        return;
    }

    USER_APP_VERIFY( ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_SCAN_START_ID) );
}


// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::DCPScanningControllerC::OnF6Pressed()
{
	/*
     if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	*/
    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}
// --------------------------------------------------------------------------------------------------------------------
//
// OnF5Pressed
//
void DCP::DCPScanningControllerC::OnF5Pressed(void)
{
	

	boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
    ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
    if(spScanningHelper)
    {
		int iAvailableScan;
		 USER_APP_VERIFY( m_poScanningModel->GetItemVal(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, iAvailableScan) );
		if(iAvailableScan == -1 || m_poScanningModel->m_vecScanList.size() == 0)
		{
			// no scan available
			return;
		}
		
		SDI::SmartPtrScanC spScan;
		if(spScanningHelper->GetScan(m_poScanningModel->m_vecScanList[iAvailableScan], spScan))
		{
			if(spScan &&  spScan->GetScanState() == SDI::ScanC::SS_SCANNED )
			{
				StringC strScanFileName;
				StringC strScanDataFilePath;
				strScanFileName = spScan->GetScanDataFileName();
				strScanDataFilePath = spScan->GetScanDataFilePath();
				//GUI::DesktopC::Instance()->MessageShow(spScan->GetScanDataFileName());
				uint64_t poitsCount = spScan->GetNumberOfPoints();

				StringC scanName;

				scanName = spScan->GetScanName();
				
				strScanDataFilePath += strScanFileName;
				strScanDataFilePath += L".sdb";
				SIO::SdbReaderC mSdbReader;
				boost::filesystem::path sdbPath( strScanDataFilePath.c_str() );
				SIO::RcT oRcT(3);// RC_NOTOK
                //oRcT = mSdbReader.OpenFile( strScanDataFilePath);
				oRcT = mSdbReader.OpenFile( sdbPath );

                if ( SIO::RC_OK == oRcT )
                {
                    //GUI::DesktopC::Instance()->MessageShow(spScan->GetScanDataFileName());
					unsigned int uiPointsToRead = 1000;
                    unsigned int uiPointsRead = 0;
                    SIO::SdbReaderIteratorC mSdbReaderIterator( uiPointsToRead );

					// output file
					char cPath[CPI::LEN_PATH_MAX];
					char cFileName[CPI::LEN_PATH_MAX];
					char m_cPathAndFileName[CPI::LEN_PATH_MAX];
					char line[512];

					bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pDCP05Model->FILE_STORAGE1, CPI::ftUserAscii, cPath);
					FILE* m_pFile=0;
					
					//UTL::UnicodeToAscii(cFileName, scanName);
					BSS::UTI::BSS_UTI_WCharToAscii(scanName, cFileName);
					sprintf(m_cPathAndFileName,"%-s%-s%-s",cPath,cFileName,".pts"); 

					m_pFile = fopen(m_cPathAndFileName, "wb+");	
					if(m_pFile)
					{
						sprintf(line,"%lu\r\n",(int) poitsCount);//,13,10);
						fputs(line,m_pFile);

						std::vector<SIO::SdbPointC> rvecSdbPoints;
						
						uint16_t i = 0;
						double x_in, y_in, z_in;
						double x_out, y_out, z_out;
						uint32_t readed=0;


						SIO::SdbPointC* pPointArray = new SIO::SdbPointC[uiPointsToRead];
						SIO::ArrayBasedPointIteratorC<SIO::SdbPointC, SIO::SdbPointModifierC> oPointIterator(pPointArray);

						//while(1/* todo0 < mSdbReader.ReadPoints( mSdbReaderIterator, rvecSdbPoints  )*/)
						//if ( 0 < mSdbReader.ReadPoints( mSdbReaderIterator, rvecSdbPoints ) )
						while(mSdbReaderIterator.Not_done())
						{
							// reset the point iterator
							oPointIterator.Reset();


							// read the points
							if ( SIO::RC_OK == mSdbReader.ReadPoints( mSdbReaderIterator, oPointIterator, uiPointsRead ) )
							{
								SIO::SdbPointC* pCurrent = pPointArray;

								i = 0;
								//for(std::vector<SIO::SdbPointC>::iterator iter = rvecSdbPoints.begin(); iter != rvecSdbPoints.end(); iter++, i++)
								// get all points from the array
								for(unsigned int j = 0; j < uiPointsRead; j++)
								{
									x_in = pCurrent->m_dNorthing;
									y_in = pCurrent->m_dEasting;
									z_in = pCurrent->m_dHeight;

									m_pCommon->to_AMS_xyz(x_in, y_in, z_in, &x_out, &y_out, &z_out);

									sprintf(line,"%f %f %f %d %d %d %d%c%c",
																x_out,//rvecSdbPoints[i].m_dEasting, 
																y_out,//rvecSdbPoints[i].m_dNorthing, 
																z_out,//rvecSdbPoints[i].m_dHeight,
																pCurrent->m_unIntensity,
																pCurrent->m_uiRed,
																pCurrent->m_uiGreen,
																pCurrent->m_uiBlue,

																13,10);
									fputs(line,m_pFile);
									//(void)m_poAvailableScan->GetComboBoxInputCtrl()->AddItem(*iter, i);
									pCurrent++;
								}
								mSdbReaderIterator.Advance();
							//StringC strMsg;
							//(void)strMsg.Format( L"E:%.5f N:%.5f H:%.5f", rvecSdbPoints[0].m_dEasting, rvecSdbPoints[0].m_dNorthing, rvecSdbPoints[0].m_dHeight );
							//GUI::DesktopC::Instance()->MessageShow(strMsg);
							
							}
							//here you could apply the transformation for each point for PTS exports
						}

						if(m_pFile)
						{
							fclose(m_pFile);
							m_pFile = 0;
						}
						//delete point array
						delete[] pPointArray;

						GUI::DesktopC::Instance()->MessageShow(StringC(L"Exported"));
						
					}
					else
					{
						DCP05MsgBoxC msgbox;
						StringC msg;
						msg.LoadTxt(AT_DCP05,	M_DCP_FILE_OPEN_ERROR_TOK);
						msg.Format(msg, (const wchar_t*)StringC(m_cPathAndFileName));
						msgbox.ShowMessageOk(msg);
					}
					// delete point array
                    //delete[] pPointArray;

					mSdbReader.CloseFile();
                }
                else
                {
                    StringC strMsg;
                    (void)strMsg.Format( L"RCT:%d", oRcT );
                    GUI::DesktopC::Instance()->MessageShow(strMsg);
                }

				
			}
		}
	}
}

void DCP::DCPScanningControllerC::ReadDb() 
{
	/*
		boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
    ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
    if(spScanningHelper)
    {

		std::vector<SDI::SmartPtrScanC> vecScans;
        if(spScanningHelper->GetAllScans(vecScans))
        {
            for(std::vector<SDI::SmartPtrScanC>::iterator iter = vecScans.begin(); iter != vecScans.end(); iter++)
            {
                m_poScanningModel->m_vecScanList.push_back((*iter)->GetScanName());
            }
        }

		int iAvailableScan;
		 USER_APP_VERIFY( m_poScanningModel->GetItemVal(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, iAvailableScan) );
		if(iAvailableScan == -1 || m_poScanningModel->m_vecScanList.size() == 0)
		{
			// no scan available
			return;
		}
		
		SDI::SmartPtrScanC spScan;
		if(spScanningHelper->GetScan(m_poScanningModel->m_vecScanList[iAvailableScan], spScan))
		{
			if(spScan &&  spScan->GetScanState() == SDI::ScanC::SS_SCANNED )
			{
				StringC strScanFileName;
				StringC strScanDataFilePath;
				strScanFileName = spScan->GetScanDataFileName();
				strScanDataFilePath = spScan->GetScanDataFilePath();
				GUI::DesktopC::Instance()->MessageShow(spScan->GetScanDataFileName());


				StringC scanName;

				scanName = spScan->GetScanName();

				strScanDataFilePath += strScanFileName;
				strScanDataFilePath += L".sdb";
				SIO::SdbReaderC mSdbReader;
				RcT oRcT;
                oRcT = mSdbReader.OpenFile( strScanDataFilePath);
                if ( 0 == oRcT )
                {
                    GUI::DesktopC::Instance()->MessageShow(spScan->GetScanDataFileName());
                    SIO::SdbReaderIteratorC mSdbReaderIterator( 1000 );

                    std::vector<SIO::SdbPointC> rvecSdbPoints;
                    if ( 0 < mSdbReader.ReadPoints( mSdbReaderIterator, rvecSdbPoints ) )
                    {
                        StringC strMsg;
                        (void)strMsg.Format( L"E:%.5f N:%.5f H:%.5f", rvecSdbPoints[0].m_dEasting, rvecSdbPoints[0].m_dNorthing, rvecSdbPoints[0].m_dHeight );
                        GUI::DesktopC::Instance()->MessageShow(strMsg);


						//here you could apply the transformation for each point for PTS exports
                    }
                    mSdbReader.CloseFile();
                }
                else
                {
                    StringC strMsg;
                    (void)strMsg.Format( L"RCT:%d", oRcT );
                    GUI::DesktopC::Instance()->MessageShow(strMsg);
                }


			}
		}
	}
*/
}
void DCP::DCPScanningControllerC::OnApplicationClosed( int unNewApp,  int ulAppOwner)
{
	
	if(unNewApp == 	ABL::CMD_SCAN_START_ID || unNewApp == ABL::CMD_SCAN_CREATE_DEFINITION_ID || unNewApp == ABL::CMD_SCAN_CONFIGURATION_ID)
	{
		UpdateAvailableScans();
		//GUI::DesktopC::Instance()->MessageShow(L"ApplicationClosed CMD_SCAN_START_ID");
	}
}
// --------------------------------------------------------------------------------------------------------------------
//
// OnSHF5Pressed
//
/*
void HEW::ScanningControllerC::OnSHF5Pressed(void)
{
    ABL::CommandFactoryI* poFactory = ABL::CommandFactoryContainerC::Instance()->GetFactory(ABL::CMD_SCAN_AUTOSTART_ID);
    if( !poFactory || !poFactory->CanExecute() )
    {
        return;
    }

    USER_APP_VERIFY( ABL::CommandFactoryContainerC::Instance()->ExecuteCommand(ABL::CMD_SCAN_AUTOSTART_ID) );
}
*/

// --------------------------------------------------------------------------------------------------------------------
//
// CreateScan
//
/*
StringC DCP::DCPScanningControllerC::GetDescription(HEW::ScanningModelC::PredefinedScanT eScanType)
{
    switch(eScanType)
    {
    case HEW::ScanningModelC::PS_40X40_RECT:
        return StringC(AT_HEWScanning, P_RECT_DESCRIPTION_TOK);

    case HEW::ScanningModelC::PS_TRIANGLE_POLY:
        return StringC(AT_HEWScanning, P_POLY_DESCRIPTION_TOK);

    case HEW::ScanningModelC::PS_100_POINTS:
        return StringC(AT_HEWScanning, P_POINT_DESCRIPTION_TOK);

    case HEW::ScanningModelC::PS_HZ_V45:
        {
        StringC strScanName;        
        (void)strScanName.Format(L"HZ_%03.0f°", m_dHorizScanV);
        return strScanName;
        }

    case HEW::ScanningModelC::PS_HZ_V90:
        return L"Horizontal Scan V = 90°";

    case HEW::ScanningModelC::PS_HZ_V135:
        return L"Horizontal Scan V = 135°";

    default:
        return L"";
    }
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreateScan
//
/*
SDI::SmartPtrScanC DCP::DCPScanningControllerC::CreateScan(HEW::ScanningModelC::PredefinedScanT eScanType)
{
    SDI::SmartPtrScanC spScan(new SDI::ScanC);
    spScan->SetScanState(SDI::ScanC::SS_DEFINED);

    switch(eScanType)
    {
    case ScanningModelC::PS_40X40_RECT:
        CreateRectangleScan(spScan);
        break;
    case ScanningModelC::PS_TRIANGLE_POLY:
        CreatePolygonScan(spScan);
        break;
    case ScanningModelC::PS_100_POINTS:
        CreatePointListScan(spScan);
        break;
    case ScanningModelC::PS_HZ_V45:
        CreateHorizontalScan(spScan, m_dHorizScanV );
        break;
    //case ScanningModelC::PS_HZ_V90:
    //    CreateHorizontalScan(spScan, 90.0 );
    //    break;
    //case ScanningModelC::PS_HZ_V135:
    //    CreateHorizontalScan(spScan, 135.0 );
    //    break;
    default:
        USER_APP_ASSERT(false);
        break;
    }

    return spScan;
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreateRectangleScan
//
/*
void DCP::DCPScanningControllerC::CreateRectangleScan(SDI::SmartPtrScanC spScan)
{
    SDI::ScanRectangularAreaC oScanDefinition;

    // define area
    oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(10.0), Gon2Rad(80.0)));
    oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(50.0), Gon2Rad(120.0)));

    // define resolution
    oScanDefinition.SetAngleResolution(Gon2Rad(0.2), Gon2Rad(0.2));

    // create scan definition
    (void)spScan->CreateScanDefinition(&oScanDefinition);
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreatePolygonScan
//
/*
void DCP::DCPScanningControllerC::CreatePolygonScan(SDI::SmartPtrScanC spScan)
{
    SDI::ScanPolygonAreaC oScanDefinition;

    // define area
    oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(50.0), Gon2Rad(75.0)));
    oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(25.0), Gon2Rad(125.0)));
    oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(75.0), Gon2Rad(125.0)));

    // define resolution
    oScanDefinition.SetAngleResolution(Gon2Rad(0.5), Gon2Rad(0.5));

    // create scan definition
    (void)spScan->CreateScanDefinition(&oScanDefinition);
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreatePointListScan
//
/*
void DCP::DCPScanningControllerC::CreatePointListScan(SDI::SmartPtrScanC spScan)
{
    SDI::ScanPointListC oScanDefinition;

    // define list
    for(int i = 0; i < 100; i++)
    {
        oScanDefinition.AddPoint(SDI::ScanDefPointC(Gon2Rad(i), Gon2Rad((double)i/2 + 75)));
    }

    // create scan definition
    (void)spScan->CreateScanDefinition(&oScanDefinition);
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreateHorizontalScan
//
/*
void DCP::DCPScanningControllerC::CreateHorizontalScan(SDI::SmartPtrScanC spScan, double dVertical )
{
    SDI::ScanPointListC oScanDefinition;

    // define list
    for(int i = 0; i < 180; i++)
    {
        oScanDefinition.AddPoint(SDI::ScanDefPointC(((i*2) / 180.0 * dPI), (dVertical / 180.0 * dPI)));
    }

    // create scan definition
    (void)spScan->CreateScanDefinition(&oScanDefinition);
}
*/
// --------------------------------------------------------------------------------------------------------------------
//
// CreateScanCopy
//

SDI::SmartPtrScanC DCP::DCPScanningControllerC::CreateScanCopy(SDI::SmartPtrScanC spScan)
{
    SDI::SmartPtrScanC spNewScan(new SDI::ScanC);
    spNewScan->SetScanState(SDI::ScanC::SS_DEFINED);

    (void)spNewScan->CreateScanDefinition(spScan->GetScanDefinition());
    spNewScan->SetScanDescription(spScan->GetScanDescription());
    spNewScan->SetOutputFormat(spScan->GetOutputFormat());
	spNewScan->SetScanState(spScan->GetScanState());
	spNewScan->SetNumberOfPoints(spScan->GetNumberOfPoints  ( ));
	spNewScan->SetScanName(spScan->GetScanName());

    return spNewScan;
}
// --------------------------------------------------------------------------------------------------------------------
//
// UpdateAvailableScans
//

void DCP::DCPScanningControllerC::UpdateAvailableScans()
{
    if(m_poScanningModel == NULL)
    {
        USER_APP_ASSERT(false);
        return;
    }

    (void)m_poScanningModel->m_vecScanList.clear();//empty();
    boost::shared_ptr<SBL::ScanningHelperI> spScanningHelper;
    ABL::BusinessFactoryContainerC::Instance()->CreateBusiness(ABL::BSN_TPS_SCANNING_HELPER_ID, spScanningHelper);
    if (spScanningHelper)
    {
        std::vector<SDI::SmartPtrScanC> vecScans;
        if(spScanningHelper->GetAllScans(vecScans))
        {
            for(std::vector<SDI::SmartPtrScanC>::iterator iter = vecScans.begin(); iter != vecScans.end(); iter++)
            {
                m_poScanningModel->m_vecScanList.push_back((*iter)->GetScanName());
            }
        }
    }
	m_poDCPScanningDialogC->RefreshControls();

	// select last one
	uint16_t count = (uint16_t) m_poScanningModel->m_vecScanList.size();

	if(count > 0) {
		USER_APP_VERIFY( m_poScanningModel->SetItemVal(DCP::DCPScanningModelC::MI_AVAILABLE_SCAN, static_cast<int>(count - 1) ));
		OnUserEvent(GUI::NC_ONCOMBOBOX_SELECTION_CHANGED, DCP::DCPScanningModelC::MI_AVAILABLE_SCAN);
	}
}
// End Module

// --------------------------------------------------------------------------------------------------------------------
//
// Constructor
//
DCP::DCPScanningModelC::DCPScanningModelC()
: GModelC()
, m_vecScanList()
{
    //USER_APP_VERIFY( AddItem(MI_PREDEF_SCAN, GUI::GModelC::VT_Long) );
    USER_APP_VERIFY( AddItem(MI_AVAILABLE_SCAN, GUI::GModelC::VT_Long) );
    //USER_APP_VERIFY( AddItem(MI_SCAN_DESCIPTION, GUI::GModelC::VT_WString) );
    USER_APP_VERIFY( AddItem(MI_SCAN_NUMPOINTS, GUI::GModelC::VT_Long) );
    //USER_APP_VERIFY( AddItem(MI_SCAN_TIME, GUI::GModelC::VT_WString) );
	USER_APP_VERIFY( AddItem(MI_SCAN_STATUS, GUI::GModelC::VT_WString) );
	USER_APP_VERIFY( AddItem(MI_SCAN_NUMPOINTS, GUI::GModelC::VT_Long) );
	
    // Set default values
    SetDefault();
}

// --------------------------------------------------------------------------------------------------------------------
//
// Destructor
//
DCP::DCPScanningModelC::~DCPScanningModelC(void)
{
}

// --------------------------------------------------------------------------------------------------------------------
//
// SetDefault
//
void DCP::DCPScanningModelC::SetDefault()
{
    //USER_APP_VERIFY( SetItemVal(MI_PREDEF_SCAN, static_cast<int>(PS_40X40_RECT)) );
}

// End Module

