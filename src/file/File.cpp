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
#include <dcp06/core/Logger.hpp>
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
FileDialog::FileDialog(DCP::FileModel* pFileModel):m_pFile(0), m_pNumberOfPoints(0), 
m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),m_pDataModel(pFileModel)
{
	//SetTxtApplicationId(AT_DCP06);
	
	//m_pAdfFile = new AdfFileFunc();
}

// Description: Destructor
// ******************************************************************************************
FileDialog::~FileDialog()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// ******************************************************************************************
void FileDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
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
	DCP06_TRACE_EXIT;
}

// ******************************************************************************************
void FileDialog::CreateFile(StringC strNewFileName, StringC sPointId)
{
	DCP06_TRACE_ENTER;
	// DB-primary: create job in database (no working ADF)
	(void)strNewFileName;
	(void)sPointId;
	// CreateFile is invoked via OnActiveControllerClosed(INPUT_FIRST_POINTID_CONTROLLER)
	// which handles DB createJob directly
	DCP06_TRACE_EXIT;
}

// ******************************************************************************************
void FileDialog::CopyFile(StringC strNewFileName)
{
	DCP06_TRACE_ENTER;
	int x;
	x=1;
	DCP06_TRACE_EXIT;
}

// ******************************************************************************************
void FileDialog::Swap()
{
	DCP06_TRACE_ENTER;
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_SWAP_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetModel()->ADFFileName);

		MsgBox MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{

		}
	DCP06_TRACE_EXIT;
}
// ******************************************************************************************
void FileDialog::Close()
{
	DCP06_TRACE_ENTER;
#if DCP_USE_JSON_DATABASE
	DCP::Database::IDatabase* db = GetModel() ? GetModel()->GetDatabase() : 0;
	DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
	if (jdb) jdb->closeJob();
#endif
	GetModel()->m_currentJobId.clear();
	GetModel()->ADFFileName = L"";
	sSelectedFile = L" ";

	RefreshControls();
	DCP06_TRACE_EXIT;
}
// ******************************************************************************************
void FileDialog::Delete()
{
	DCP06_TRACE_ENTER;
	if (!GetModel()->m_currentJobId.empty())
	{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetModel()->ADFFileName);

		MsgBox MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{
#if DCP_USE_JSON_DATABASE
			DCP::Database::IDatabase* db = GetModel()->GetDatabase();
			DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
			if (jdb)
			{
				if (db->deleteJob(GetModel()->m_currentJobId))
				{
					jdb->closeJob();
					GetModel()->m_currentJobId.clear();
					GetModel()->ADFFileName = L"";
				}
			}
#endif
		}
	}
	DCP06_TRACE_EXIT;
}
// ******************************************************************************************
void FileDialog::Save()
{
	DCP06_TRACE_ENTER;
	DCP06_TRACE_EXIT;
}
void FileDialog::View()
{
	DCP06_TRACE_ENTER;
	DCP06_TRACE_EXIT;
}
void DCP::FileDialog::SelectFile(StringC sFile)
{
	DCP06_TRACE_ENTER;
	sSelectedFile = sFile;
	RefreshControls();
	DCP06_TRACE_EXIT;
}

// ******************************************************************************************
void FileDialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
	sSelectedFile = GetModel()->ADFFileName;
	//MsgBox MsgBox;
	//MsgBox.ShowMessageOk(sSelectedFile);
	// DB-primary: no working ADF; job name comes from m_currentJobId / ADFFileName
	m_pCommon = new Common(GetModel());
	RefreshControls();
	DCP06_TRACE_EXIT;
}


// Description: refresh all controls
void DCP::FileDialog::RefreshControls()
{
	DCP06_TRACE_ENTER;	
	if(m_pFile && m_pNumberOfPoints && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		// DB-primary: job loaded = m_currentJobId not empty; display from JsonDatabase
		bool jobLoaded = GetModel() && !GetModel()->m_currentJobId.empty();
#if DCP_USE_JSON_DATABASE
		DCP::Database::JsonDatabase* jdb = 0;
		if (jobLoaded && GetModel()->GetDatabase())
			jdb = dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase());
#endif

		if(jobLoaded
#if DCP_USE_JSON_DATABASE
			&& jdb
#endif
			)
		{
#if DCP_USE_JSON_DATABASE
			m_pFile->GetStringInputCtrl()->SetString(StringC(jdb->getJobDisplayName().c_str()));
			char ptBuf[16];
			sprintf(ptBuf, "%d", jdb->getJobPointsCount());
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(StringC(ptBuf));
			m_pSize->GetStringInputCtrl()->SetString(StringC(jdb->getJobFileSizeString().c_str()));
			m_pDate->GetStringInputCtrl()->SetString(StringC(jdb->getJobModDateString().c_str()));
			m_pTime->GetStringInputCtrl()->SetString(StringC(jdb->getJobModTimeString().c_str()));
#else
			m_pFile->GetStringInputCtrl()->SetString(L" ");
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(L" ");
			m_pSize->GetStringInputCtrl()->SetString(L" ");
			m_pDate->GetStringInputCtrl()->SetString(L" ");
			m_pTime->GetStringInputCtrl()->SetString(L" ");
#endif
		}
		else
		{
			m_pFile->GetStringInputCtrl()->SetString(L" ");
			m_pNumberOfPoints->GetStringInputCtrl()->SetString(L" ");
			m_pSize->GetStringInputCtrl()->SetString(L" ");
			m_pDate->GetStringInputCtrl()->SetString(L" ");
			m_pTime->GetStringInputCtrl()->SetString(L" ");
		}

			unsigned int uiFreeSpace = m_pCommon ? m_pCommon->get_free_space() : 0;
			char temp[100];
			sprintf(temp,"%lu",(unsigned long)uiFreeSpace);
			m_pFreeSpace->GetStringInputCtrl()->SetString(StringC(temp));

	}
	DCP06_TRACE_EXIT;
} 

void DCP::FileDialog::UpdateData()
{
	DCP06_TRACE_ENTER;
	// DB-primary: ADFFileName holds job display name when job loaded
	if (GetModel() && !GetModel()->m_currentJobId.empty())
	{
#if DCP_USE_JSON_DATABASE
		DCP::Database::JsonDatabase* jdb = GetModel()->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase()) : 0;
		if (jdb)
			GetModel()->ADFFileName = StringC(jdb->getJobDisplayName().c_str());
		else
#endif
			GetModel()->ADFFileName = StringC(GetModel()->m_currentJobId.c_str());
	}
	else
		GetModel()->ADFFileName = L"";
	DCP06_TRACE_EXIT;
}


// Description: only accept DCP06 Model objects
bool DCP::FileDialog::SetModel( GUI::ModelC* pModel )
{
	DCP06_TRACE_ENTER;
    // Verify type
    DCP::Model* pDcpModel = dynamic_cast< DCP::Model* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
        RefreshControls();
		DCP06_TRACE_EXIT;
        return true;
    }
    USER_APP_VERIFY( false );
	DCP06_TRACE_EXIT;
    return false;
}

// Description: DCP06 model
DCP::Model* DCP::FileDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::FileController::FileController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_3DFILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	
	// FileModel
	m_pDataModel = new DCP::FileModel(m_pModel); 
    // Create a dialog
    m_pDlg = new DCP::FileDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
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

DCP::FileController::~FileController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
bool DCP::FileController::SetModel( GUI::ModelC* pModel )
{
	DCP06_TRACE_ENTER;
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
	DCP06_TRACE_EXIT;
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


// Import from ADF (legacy file -> DB)
// *********************************************************************************************
void DCP::FileController::OnSHF1Pressed()
{
	DCP06_TRACE_ENTER;
	if (m_pDlg == nullptr) { USER_APP_VERIFY(false); DCP06_TRACE_EXIT; return; }
	DCP::SelectFileModel* pModel = new SelectFileModel;
	if (GetController(SELECT_FILE_CONTROLLER) == nullptr)
	{
		StringC sTitle = GetTitle();
		(void)AddController(SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle, m_pModel));
	}
	(void)GetController(SELECT_FILE_CONTROLLER)->SetModel(pModel);
	SetActiveController(SELECT_FILE_CONTROLLER, true);
	DCP06_TRACE_EXIT;
}

// Export to ADF (DB -> file when user requests; ADF only for import/export)
// *********************************************************************************************
void DCP::FileController::OnSHF4Pressed()
{
	DCP06_TRACE_ENTER;
	if (m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }
	DCP::Database::IDatabase* db = m_pModel->GetDatabase();
#if !DCP_USE_JSON_DATABASE
	(void)db;
	DCP06_TRACE_EXIT; return;
#else
	DCP::Database::JsonDatabase* jdb = db ? dynamic_cast<DCP::Database::JsonDatabase*>(db) : 0;
	if (!jdb) { DCP06_TRACE_EXIT; return; }
	// DB-primary: export current job to ADF file (no working ADF; export directly)
	std::string workPath = jdb->getJobWorkingPath(m_pModel->m_currentJobId);
	if (workPath.empty()) { DCP06_TRACE_EXIT; return; }
	std::string exportPath = workPath;
	size_t dot = exportPath.rfind('.');
	if (dot != std::string::npos)
		exportPath = exportPath.substr(0, dot) + "_export.adf";
	else
		exportPath += "_export.adf";
	db->exportToADF(exportPath);
	m_pDlg->RefreshControls();
	DCP06_TRACE_POINT("exported to %s", exportPath.c_str());
#endif
	DCP06_TRACE_EXIT;
}

// OPEN (DB-primary: list jobs from database)
// *********************************************************************************************
void DCP::FileController::OnF1Pressed()
{
	DCP06_TRACE_ENTER;
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::SelectFileModel* pModel = new SelectFileModel;

		if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(FILE_TYPE_JOBS, sTitle, m_pModel) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
	DCP06_TRACE_EXIT;
}

// NEW
// *********************************************************************************************
void DCP::FileController::OnF2Pressed()
{
	DCP06_TRACE_ENTER;
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

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetModel());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);
	DCP06_TRACE_EXIT;
}

// copy
// *********************************************************************************************
void DCP::FileController::OnF3Pressed()
{
	DCP06_TRACE_ENTER;
	if(m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }

	if (m_pDlg == nullptr)
	{
		USER_APP_VERIFY( false );
		return;
	}
		
		DCP::InputTextModel* pModel = new InputTextModel;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_COPY_FILE_TO_TOK);
		pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = DCP_JOB_ID_MAX_LEN;
		pModel->m_StrText = L" ";

		if ( nullptr == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
		{
			USER_APP_VERIFY( false );
			return;
		}

		if(GetController(COPY_FILE_TO_CONTROLLER) == nullptr)
		{
			(void)AddController( COPY_FILE_TO_CONTROLLER, new DCP::InputTextController( m_pModel));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetModel());
		(void)GetController( COPY_FILE_TO_CONTROLLER )->SetModel(pModel);
		SetActiveController(COPY_FILE_TO_CONTROLLER, true);
}

// CLOSE
// *********************************************************************************************
void DCP::FileController::OnF5Pressed()
{
	DCP06_TRACE_ENTER;
#if DCP_USE_JSON_DATABASE
	DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
	if (jdb) jdb->closeJob();
#endif
	m_pModel->m_currentJobId.clear();
	m_pModel->ADFFileName = L"";

	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// SWAP (swap measured/design coords in current job)
// *********************************************************************************************
void DCP::FileController::OnF4Pressed()
{
	DCP06_TRACE_ENTER;
	if(m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }

#if DCP_USE_JSON_DATABASE
	DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
	if (jdb) jdb->swapMeasDesign();
#endif
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// CONT
// *********************************************************************************************
void DCP::FileController::OnF6Pressed()
{
	DCP06_TRACE_ENTER;
    if (m_pDlg == nullptr)
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
	DCP06_TRACE_EXIT;
}
// DEL
// *********************************************************************************************
void DCP::FileController::OnSHF2Pressed()
{
	DCP06_TRACE_ENTER;
	if(m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }
	// Delete is handled in FileDialog::Delete() with confirmation
	m_pDlg->Delete();
	DCP06_TRACE_EXIT;
}

// SAVE (DB-primary: save current job to database)
// *********************************************************************************************
void DCP::FileController::OnSHF3Pressed()
{
	DCP06_TRACE_ENTER;
	if(m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }
#if DCP_USE_JSON_DATABASE
	DCP::Database::IDatabase* db = m_pModel->GetDatabase();
	if (db) db->saveJob(m_pModel->m_currentJobId);
#endif
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// VIEW
// ******************************************************************************************
void DCP::FileController::OnSHF5Pressed()
{
	DCP06_TRACE_ENTER;
	if(m_pModel->m_currentJobId.empty())
		{ DCP06_TRACE_EXIT; return; }

	
	if(GetController(_3DFILEVIEW_CONTROLLER) == nullptr)		
	{
		(void)AddController( _3DFILEVIEW_CONTROLLER, new DCP::FileView3DController (m_pDataModel));
	}
	(void)GetController( _3DFILEVIEW_CONTROLLER )->SetModel(m_pDlg->GetModel());
	SetActiveController(_3DFILEVIEW_CONTROLLER, true);
	DCP06_TRACE_EXIT;
}


// ******************************************************************************************
void DCP::FileController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::FileController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06_TRACE_ENTER;
	DCP06_TRACE_POINT("ctrl=%d exit=%d", lCtrlID, lExitCode);
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileController* selCtrl = dynamic_cast<DCP::SelectFileController*>(GetController(SELECT_FILE_CONTROLLER));
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelected = pModel->m_strSelectedFile;
		strSelected.Trim();
		if (strSelected.IsEmpty()) { m_pDlg->RefreshControls(); DCP06_TRACE_EXIT; return; }

		// DB-primary: Open job from database (no working ADF)
		if (selCtrl && selCtrl->GetFileType() == FILE_TYPE_JOBS)
		{
			char jobIdBuf[64];
			jobIdBuf[0] = '\0';
			BSS::UTI::BSS_UTI_WCharToAscii(strSelected, jobIdBuf);
			std::string jobId(jobIdBuf);
			DCP::Database::IDatabase* db = m_pModel->GetDatabase();
#if DCP_USE_JSON_DATABASE
			if (db && !jobId.empty() && db->loadJob(jobId))
			{
				m_pModel->m_currentJobId = jobId;
				m_pModel->ADFFileName = StringC(jobId.c_str());
				DCP06_LOG_INFO("FileController: opened job %s", jobId.c_str());
			}
#endif
		}
		else
		{
			// Import from ADF: create job from legacy file (ADF only for import; no working ADF)
			char pathBuf[CPI::LEN_PATH_MAX];
			pathBuf[0] = '\0';
			BSS::UTI::BSS_UTI_WCharToAscii(strSelected, pathBuf);
			std::string path(pathBuf);
			if (!path.empty())
			{
				std::string jobId = path;
				size_t lastSlash = path.find_last_of("/\\");
				if (lastSlash != std::string::npos)
					jobId = path.substr(lastSlash + 1);
				size_t dot = jobId.rfind('.');
				if (dot != std::string::npos)
					jobId = jobId.substr(0, dot);
				if (jobId.empty()) jobId = "imported";

				DCP::Database::IDatabase* db = m_pModel->GetDatabase();
				if (db)
				{
					db->createJob(jobId);
					if (db->importFromADF(path))
					{
						db->saveJob(jobId);
						if (db->loadJob(jobId))
						{
							m_pModel->m_currentJobId = jobId;
							m_pModel->ADFFileName = StringC(jobId.c_str());
							DCP06_LOG_INFO("FileController: imported ADF to job %s", jobId.c_str());
						}
					}
				}
			}
		}
	}

	if(lExitCode == EC_KEY_CONT)
	{
		if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
		{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			sFileToCreate = strNewFile;
			sFileToCreate.Trim();
			// and ask first point id
			
			DCP::InputTextModel* pModel1 = new InputTextModel;
			pModel1->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
			pModel1->m_StrTitle = GetTitle();
			pModel1->m_iTextLength = DCP_POINT_ID_LENGTH;
			pModel1->m_StrText = L"P1";

			if ( nullptr == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
			{
				USER_APP_VERIFY( false );
				return;
			}

			if(GetController(INPUT_FIRST_POINTID_CONTROLLER) == nullptr)
			{
				(void)AddController( INPUT_FIRST_POINTID_CONTROLLER, new DCP::InputTextController( m_pModel));
			}

			//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetModel());
			(void)GetController( INPUT_FIRST_POINTID_CONTROLLER )->SetModel(pModel1);
			SetActiveController(INPUT_FIRST_POINTID_CONTROLLER, true);
			

			 //m_pDlg->CreateFile(sFileToCreate, pModel->m_StrText);


		}
		else if(lCtrlID == COPY_FILE_TO_CONTROLLER)
		{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( COPY_FILE_TO_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char fname[CPI::LEN_PATH_MAX];
			BSS::UTI::BSS_UTI_WCharToAscii(strNewFile, fname);
			std::string targetId(fname);
			size_t dot = targetId.rfind('.');
			if (dot != std::string::npos) targetId = targetId.substr(0, dot);
			if (targetId.empty()) targetId = "copy";

			DCP::Database::IDatabase* db = m_pModel->GetDatabase();
#if DCP_USE_JSON_DATABASE
			if (db && !m_pModel->m_currentJobId.empty() && db->copyJob(m_pModel->m_currentJobId, targetId))
			{
				if (db->loadJob(targetId))
				{
					m_pModel->m_currentJobId = targetId;
					m_pModel->ADFFileName = StringC(targetId.c_str());
				}
			}
#endif
		}
		
		else if(lCtrlID == INPUT_FIRST_POINTID_CONTROLLER)
		{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_FIRST_POINTID_CONTROLLER )->GetModel();		
			
			StringC sPointId = pModel->m_StrText;
			char fname[CPI::LEN_PATH_MAX];
			char pointid[10];
			BSS::UTI::BSS_UTI_WCharToAscii(sFileToCreate, fname);
			BSS::UTI::BSS_UTI_WCharToAscii(sPointId, pointid);

			// DB-primary: create job in database (no working ADF)
			DCP::Database::IDatabase* db = m_pModel->GetDatabase();
#if DCP_USE_JSON_DATABASE
			if (db)
			{
				std::string jobId(fname);
				size_t dotPos = jobId.find('.');
				if (dotPos != std::string::npos) jobId = jobId.substr(0, dotPos);
				if (jobId.empty()) jobId = "job1";
				if (db->createJob(jobId))
				{
					// Add first point if pointid provided
					if (pointid[0])
					{
						DCP::Database::PointData pt;
						pt.id = pointid;
						db->addPoint(pt.id, pt);
					}
					db->saveJob(jobId);
					m_pModel->m_currentJobId = jobId;
					m_pModel->ADFFileName = StringC(jobId.c_str());
				}
			}
#endif
			//m_pDlg->CreateFile(sFileToCreate,pModel->m_StrText);
			

		}

	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
	DCP06_TRACE_EXIT;
}


// ===========================================================================================
// SelectPointModel
// ===========================================================================================

// Instantiate template classes
DCP::FileModel::FileModel(DCP::Model* pModel):m_pModel(pModel)
{
	m_pAdfFile = 
		new AdfFileFunc(m_pModel);
	m_pAdfFile->always_single = 1;

}
DCP::FileModel::~FileModel()
{
	if(m_pAdfFile)
	{
		delete m_pAdfFile;
		m_pAdfFile = 0;
	}
}



