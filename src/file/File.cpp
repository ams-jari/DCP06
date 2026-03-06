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
#include <dcp06/file/File.hpp>
#include <dcp06/file/SelectFile.hpp>
#ifndef DCP_USE_JSON_DATABASE
#define DCP_USE_JSON_DATABASE 1
#endif
#if DCP_USE_JSON_DATABASE
#include <dcp06/database/JsonDatabase.hpp>
#endif
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/measurement/3dFileView.hpp>

#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_Util.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>

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

// ******************************************************************************************
DCP06FileDlgC::DCP06FileDlgC(DCP::DCP06FileModelC* pFileModel):m_pFile(0), m_pNumberOfPoints(0), 
m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),m_pDataModel(pFileModel)
{
	//SetTxtApplicationId(AT_DCP06);
	
	//m_pAdfFile = new AdfFileFunc();
}

// Description: Destructor
// ******************************************************************************************
DCP06FileDlgC::~DCP06FileDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// ******************************************************************************************
void DCP06FileDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_13 );
	
	// InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_3D_FILE_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pFile);

	m_pNumberOfPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNumberOfPoints->SetId(eNumberOfPoints);
	m_pNumberOfPoints->SetText(StringC(AT_DCP06,P_DCP_NUMBER_OF_POINTS_TOK));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pNumberOfPoints->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pNumberOfPoints);

	m_pSize = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pSize->SetId(eSize);
	m_pSize->SetText(StringC(AT_DCP06,P_DCP_SIZE_OF_FILE_TOK));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pSize->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT);	 CAPTIVATE
	AddCtrl(m_pSize);

	m_pDate = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDate->SetId(eDate);
	m_pDate->SetText(StringC(AT_DCP06,P_DCP_DATE_OF_FILE_TOK));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pDate->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pDate);

	m_pTime = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTime->SetId(eTime);
	m_pTime->SetText(StringC(AT_DCP06,P_DCP_TIME_OF_FILE_TOK));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pTime->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pTime);

	m_pFreeSpace = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFreeSpace->SetId(eFreeSpace);
	m_pFreeSpace->SetText(StringC(AT_DCP06,P_DCP_FREE_SPACE_TOK));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pFreeSpace->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pFreeSpace);

	//SetHelpTok(H_DCP_3DFILE_TOK,0);
}

// ******************************************************************************************
void DCP06FileDlgC::CreateFile(StringC strNewFileName, StringC sPointId)
{
	char fname[CPI::LEN_PATH_MAX];
	char pointid[10];

	//UTL::UnicodeToAscii(fname, strNewFileName);
	BSS::UTI::BSS_UTI_WCharToAscii( strNewFileName, fname );

	//UTL::UnicodeToAscii(pointid, sPointId);
	BSS::UTI::BSS_UTI_WCharToAscii( sPointId, pointid );


	m_pDataModel->m_pAdfFile->create_adf_file(fname,pointid);
}

// ******************************************************************************************
void DCP06FileDlgC::CopyFile(StringC strNewFileName)
{
	int x;
	x=1;
}

// ******************************************************************************************
void DCP06FileDlgC::Swap()
{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_SWAP_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetDCP06Model()->ADFFileName);

		DCP06MsgBoxC MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{

		}
}
// ******************************************************************************************
void DCP06FileDlgC::Close()
{
	m_pDataModel->m_pAdfFile->CloseFile();
	sSelectedFile = L" ";

	RefreshControls();
}
// ******************************************************************************************
void DCP06FileDlgC::Delete()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
	{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetDCP06Model()->ADFFileName);

		DCP06MsgBoxC MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{

		}
	}
}
// ******************************************************************************************
void DCP06FileDlgC::Save()
{

}
void DCP06FileDlgC::View()
{

}
void DCP::DCP06FileDlgC::SelectFile(StringC sFile)
{
	sSelectedFile = sFile;
	RefreshControls();
}

// ******************************************************************************************
void DCP06FileDlgC::OnDialogActivated()
{
	sSelectedFile = GetDCP06Model()->ADFFileName;
	//DCP06MsgBoxC MsgBox;
	//MsgBox.ShowMessageOk(sSelectedFile);
	if(!sSelectedFile.IsEmpty())
		m_pDataModel->m_pAdfFile->setFile(GetDCP06Model()->ADFFileName);

	m_pCommon = new DCP06CommonC(GetDCP06Model());
	RefreshControls();
}


// Description: refresh all controls
void DCP::DCP06FileDlgC::RefreshControls()
{	
	if(m_pFile && m_pNumberOfPoints && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		//m_pDataModel->m_pAdfFile->setFile(sSelectedFile/*GetDCP06Model()->ADFFileName*/);

		if(m_pDataModel->m_pAdfFile->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getFileName());//GetDCP06Model()->ADFFileName);	
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getPointsCountString());
			m_pSize->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getFileSizeString());
			m_pDate->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getModDate());
			m_pTime->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getModTime());
		}
		else
		{
			m_pFile->GetStringInputCtrl()->SetString(L" ");
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(L" ");
			m_pSize->GetStringInputCtrl()->SetString(L" ");
			m_pDate->GetStringInputCtrl()->SetString(L" ");
			m_pTime->GetStringInputCtrl()->SetString(L" ");
		}

			unsigned int uiFreeSpace = m_pCommon->get_free_space();
			char temp[100];
			sprintf(temp,"%lu",uiFreeSpace);
			m_pFreeSpace->GetStringInputCtrl()->SetString(StringC(temp));

	}
} 

void DCP::DCP06FileDlgC::UpdateData()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
        GetDCP06Model()->ADFFileName = m_pDataModel->m_pAdfFile->getFileName();
	else
		GetDCP06Model()->ADFFileName = L"";
	//GetDCP06Model()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}


// Description: only accept hello world Model objects
bool DCP::DCP06FileDlgC::SetModel( GUI::ModelC* pModel )
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
DCP::DCP06ModelC* DCP::DCP06FileDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::DCP06FileControllerC::DCP06FileControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_3DFILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	
	// FileModel
	m_pDataModel = new DCP::DCP06FileModelC(m_pDCP06Model); 
    // Create a dialog
    m_pDlg = new DCP::DCP06FileDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DFILE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_OPEN_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_NEW_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_COPY_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_SWAP_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CLOSE_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = L"Import";
	SetFunctionKey( SHFK1, vDef );
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_SAVE_TOK);
	SetFunctionKey( SHFK3, vDef );

	vDef.strLable = L"Export";
	SetFunctionKey( SHFK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
	SetFunctionKey( SHFK5, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06FileControllerC::~DCP06FileControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP06FileControllerC::SetModel( GUI::ModelC* pModel )
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


// Import from ADF (legacy file -> DB)
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnSHF1Pressed()
{
	if (m_pDlg == NULL) { USER_APP_VERIFY(false); return; }
	DCP::DCP06SelectFileModelC* pModel = new DCP06SelectFileModelC;
	if (GetController(SELECT_FILE_CONTROLLER) == NULL)
	{
		StringC sTitle = GetTitle();
		(void)AddController(SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(ONLY_ADF, sTitle, m_pDCP06Model));
	}
	(void)GetController(SELECT_FILE_CONTROLLER)->SetModel(pModel);
	SetActiveController(SELECT_FILE_CONTROLLER, true);
}

// Export to ADF (DB -> file when user requests)
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnSHF4Pressed()
{
	if (!m_pDataModel->m_pAdfFile->IsOpen() || m_pDCP06Model->m_currentJobId.empty())
		return;
	DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if !DCP_USE_JSON_DATABASE
	(void)db;
	return;
#else
	DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
	if (!jdb) return;
	std::string workPath = jdb->getJobWorkingPath(m_pDCP06Model->m_currentJobId);
	if (workPath.empty()) return;
	// Refresh DB from working ADF, then export to _export.adf (TODO: file picker)
	db->createJob(m_pDCP06Model->m_currentJobId);
	db->importFromADF(workPath);
	std::string exportPath = workPath;
	size_t dot = exportPath.rfind('.');
	if (dot != std::string::npos)
		exportPath = exportPath.substr(0, dot) + "_export.adf";
	else
		exportPath += "_export.adf";
	db->exportToADF(exportPath);
	m_pDlg->RefreshControls();
#endif
}

// OPEN (DB-primary: list jobs from database)
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF1Pressed()
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
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(DCP06_JOBS, sTitle, m_pDCP06Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
}

// NEW
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF2Pressed()
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
			(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP06InputTextControllerC(m_pDCP06Model ));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);
}

// copy
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF3Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	if (m_pDlg == NULL)
	{
		USER_APP_VERIFY( false );
		return;
	}
		
		DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_COPY_FILE_TO_TOK);
		pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = 8;
		pModel->m_StrText = L" ";

		if ( NULL == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
		{
			USER_APP_VERIFY( false );
			return;
		}

		if(GetController(COPY_FILE_TO_CONTROLLER) == NULL)
		{
			(void)AddController( COPY_FILE_TO_CONTROLLER, new DCP::DCP06InputTextControllerC( m_pDCP06Model));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
		(void)GetController( COPY_FILE_TO_CONTROLLER )->SetModel(pModel);
		SetActiveController(COPY_FILE_TO_CONTROLLER, true);
}

// CLOSE
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF5Pressed()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
		m_pDataModel->m_pAdfFile->CloseFile();
	m_pDCP06Model->m_currentJobId.clear();
	m_pDCP06Model->ADFFileName = L"";

	m_pDlg->RefreshControls();
}

// SWAP
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF4Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	m_pDataModel->m_pAdfFile->swap_meas_design();
	m_pDlg->RefreshControls();
}

// CONT
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    //m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}
// DEL
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnSHF2Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;
	m_pDataModel->m_pAdfFile->delete_adf_file();
	m_pDlg->RefreshControls();
}

// SAVE (DB-primary: working ADF -> DB)
// *********************************************************************************************
void DCP::DCP06FileControllerC::OnSHF3Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;
	m_pDataModel->m_pAdfFile->save_adf_to_ref_dat();  // DCP05 legacy .dat/.ref
	// DB-primary: sync working ADF -> database
	if (!m_pDCP06Model->m_currentJobId.empty())
	{
		DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if DCP_USE_JSON_DATABASE
		DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
		if (jdb)
		{
			std::string workPath = jdb->getJobWorkingPath(m_pDCP06Model->m_currentJobId);
			if (!workPath.empty())
			{
				db->createJob(m_pDCP06Model->m_currentJobId);
				db->importFromADF(workPath);
				db->saveJob(m_pDCP06Model->m_currentJobId);
			}
		}
#endif
	}
	m_pDlg->RefreshControls();
}

// VIEW
// ******************************************************************************************
void DCP::DCP06FileControllerC::OnSHF5Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	
	if(GetController(_3DFILEVIEW_CONTROLLER) == NULL)		
	{
		(void)AddController( _3DFILEVIEW_CONTROLLER, new DCP::DCP063DFileViewControllerC (m_pDataModel));
	}
	(void)GetController( _3DFILEVIEW_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
	SetActiveController(_3DFILEVIEW_CONTROLLER, true);
	
}


// ******************************************************************************************
void DCP::DCP06FileControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06FileControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectFileControllerC* selCtrl = dynamic_cast<DCP::DCP06SelectFileControllerC*>(GetController(SELECT_FILE_CONTROLLER));
		DCP::DCP06SelectFileModelC* pModel = (DCP::DCP06SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelected = pModel->m_strSelectedFile;
		strSelected.Trim();
		if (strSelected.IsEmpty()) { m_pDlg->RefreshControls(); return; }

		// DB-primary: Open job from database
		if (selCtrl && selCtrl->GetFileType() == DCP06_JOBS)
		{
			char jobIdBuf[64];
			jobIdBuf[0] = '\0';
			BSS::UTI::BSS_UTI_WCharToAscii(strSelected, jobIdBuf);
			std::string jobId(jobIdBuf);
			DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if DCP_USE_JSON_DATABASE
			DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
			if (jdb && !jobId.empty())
			{
				if (db->loadJob(jobId))
				{
					std::string workPath = jdb->getJobWorkingPath(jobId);
					if (!workPath.empty() && db->exportToADF(workPath))
					{
						if (m_pDataModel->m_pAdfFile->setFileFromFullPath(workPath.c_str()))
						{
							m_pDCP06Model->m_currentJobId = jobId;
							m_pDCP06Model->ADFFileName = StringC(jobId.c_str());
						}
					}
				}
			}
#endif
		}
		else
		{
			// Import from ADF: create job from legacy file (ADF only for import)
			m_pDataModel->m_pAdfFile->setFile(strSelected);
			if (m_pDataModel->m_pAdfFile->IsOpen())
			{
				DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
				const char* fullPath = m_pDataModel->m_pAdfFile->getFullPath();
				if (db && fullPath && fullPath[0])
				{
					std::string path(fullPath);
					std::string jobId = path;
					size_t lastSlash = path.find_last_of("/\\");
					if (lastSlash != std::string::npos)
						jobId = path.substr(lastSlash + 1);
					size_t dot = jobId.rfind('.');
					if (dot != std::string::npos)
						jobId = jobId.substr(0, dot);
					if (!jobId.empty())
					{
						db->createJob(jobId);
						db->importFromADF(path);
						db->saveJob(jobId);
#if DCP_USE_JSON_DATABASE
						DCP::Database::JsonDatabase* jdb = dynamic_cast<DCP::Database::JsonDatabase*>(db);
						if (jdb && db->loadJob(jobId))
						{
							std::string workPath = jdb->getJobWorkingPath(jobId);
							if (!workPath.empty() && db->exportToADF(workPath))
							{
								m_pDataModel->m_pAdfFile->CloseFile();
								if (m_pDataModel->m_pAdfFile->setFileFromFullPath(workPath.c_str()))
								{
									m_pDCP06Model->m_currentJobId = jobId;
									m_pDCP06Model->ADFFileName = StringC(jobId.c_str());
								}
							}
						}
#endif
					}
				}
			}
		}
	}

	if(lExitCode == EC_KEY_CONT)
	{
		if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
		{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			sFileToCreate = strNewFile;
			sFileToCreate.Trim();
			// and ask first point id
			
			DCP::DCP06InputTextModelC* pModel1 = new DCP06InputTextModelC;
			pModel1->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
			pModel1->m_StrTitle = GetTitle();
			pModel1->m_iTextLength = DCP_POINT_ID_LENGTH;//8;
			pModel1->m_StrText = L"P1";

			if ( NULL == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
			{
				USER_APP_VERIFY( false );
				return;
			}

			if(GetController(INPUT_FIRST_POINTID_CONTROLLER) == NULL)
			{
				(void)AddController( INPUT_FIRST_POINTID_CONTROLLER, new DCP::DCP06InputTextControllerC( m_pDCP06Model));
			}

			//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
			(void)GetController( INPUT_FIRST_POINTID_CONTROLLER )->SetModel(pModel1);
			SetActiveController(INPUT_FIRST_POINTID_CONTROLLER, true);
			

			 //m_pDlg->CreateFile(sFileToCreate, pModel->m_StrText);


		}
		else if(lCtrlID == COPY_FILE_TO_CONTROLLER)
		{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( COPY_FILE_TO_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char fname[CPI::LEN_PATH_MAX];
			BSS::UTI::BSS_UTI_WCharToAscii(strNewFile, fname);
			std::string targetId(fname);
			size_t dot = targetId.rfind('.');
			if (dot != std::string::npos) targetId = targetId.substr(0, dot);
			if (targetId.empty()) targetId = "copy";

			DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if DCP_USE_JSON_DATABASE
			DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
			if (jdb && !m_pDCP06Model->m_currentJobId.empty())
			{
				if (db->copyJob(m_pDCP06Model->m_currentJobId, targetId))
				{
					m_pDataModel->m_pAdfFile->CloseFile();
					if (db->loadJob(targetId))
					{
						std::string workPath = jdb->getJobWorkingPath(targetId);
						if (!workPath.empty() && db->exportToADF(workPath))
						{
							if (m_pDataModel->m_pAdfFile->setFileFromFullPath(workPath.c_str()))
							{
								m_pDCP06Model->m_currentJobId = targetId;
								m_pDCP06Model->ADFFileName = StringC(targetId.c_str());
							}
						}
					}
				}
			}
			else
#endif
			{
				strcat(fname, ".adf");
				if (m_pDataModel->m_pAdfFile->copy_adf_file(fname))
				{
					m_pDataModel->m_pAdfFile->setFile(strNewFile);
					m_pDataModel->m_pAdfFile->add_header_to_adf();
				}
			}
		}
		
		else if(lCtrlID == INPUT_FIRST_POINTID_CONTROLLER)
		{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_FIRST_POINTID_CONTROLLER )->GetModel();		
			
			StringC sPointId = pModel->m_StrText;
			char fname[CPI::LEN_PATH_MAX];
			char pointid[10];
			BSS::UTI::BSS_UTI_WCharToAscii(sFileToCreate, fname);
			BSS::UTI::BSS_UTI_WCharToAscii(sPointId, pointid);

			// DB-primary: create job in DB, create working ADF
			DCP::Database::IDatabase* db = m_pDCP06Model->GetDatabase();
#if DCP_USE_JSON_DATABASE
			DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
			if (jdb)
			{
				std::string jobId(fname);
				jobId = jobId.substr(0, jobId.find('.'));
				if (jobId.empty()) jobId = "job1";
				if (db->createJob(jobId))
				{
					std::string workPath = jdb->getJobWorkingPath(jobId);
					if (!workPath.empty() && m_pDataModel->m_pAdfFile->create_adf_file_at_path(workPath.c_str(), pointid, true) == 0)
					{
						if (m_pDataModel->m_pAdfFile->setFileFromFullPath(workPath.c_str()))
						{
							m_pDCP06Model->m_currentJobId = jobId;
							m_pDCP06Model->ADFFileName = StringC(jobId.c_str());
							db->saveJob(jobId);
						}
					}
				}
			}
			else
#endif
			{
				m_pDataModel->m_pAdfFile->create_adf_file(fname, pointid);
				m_pDataModel->m_pAdfFile->setFile(sFileToCreate);
			}
			//m_pDlg->CreateFile(sFileToCreate,pModel->m_StrText);
			

		}

	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06FileModelC::DCP06FileModelC(DCP::DCP06ModelC* pDCP06Model):m_pDCP06Model(pDCP06Model)
{
	m_pAdfFile = 
		new AdfFileFunc(m_pDCP06Model);
	m_pAdfFile->always_single = 1;

}
DCP::DCP06FileModelC::~DCP06FileModelC()
{
	if(m_pAdfFile)
	{
		delete m_pAdfFile;
		m_pAdfFile = 0;
	}
}



