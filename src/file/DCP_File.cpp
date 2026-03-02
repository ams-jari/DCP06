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
#include <dcp06/file/DCP_File.hpp>
#include <dcp06/file/DCP_SelectFile.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_InputText.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/measurement/DCP_3DFileView.hpp>

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
DCP05FileDlgC::DCP05FileDlgC(DCP::DCP05FileModelC* pFileModel):m_pFile(0), m_pNumberOfPoints(0), 
m_pSize(0), m_pDate(0), m_pTime(0),m_pFreeSpace(0),m_pDataModel(pFileModel)
{
	//SetTxtApplicationId(AT_DCP05);
	
	//m_pAdfFile = new AdfFileFunc();
}

// Description: Destructor
// ******************************************************************************************
DCP05FileDlgC::~DCP05FileDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}
// ******************************************************************************************
void DCP05FileDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_13 );
	
	// InsertEmptyLine(); CAPTIVATE

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP05,P_DCP_3D_FILE_TOK));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pFile);

	m_pNumberOfPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pNumberOfPoints->SetId(eNumberOfPoints);
	m_pNumberOfPoints->SetText(StringC(AT_DCP05,P_DCP_NUMBER_OF_POINTS_TOK));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pNumberOfPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pNumberOfPoints->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pNumberOfPoints);

	m_pSize = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pSize->SetId(eSize);
	m_pSize->SetText(StringC(AT_DCP05,P_DCP_SIZE_OF_FILE_TOK));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pSize->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pSize->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT);	 CAPTIVATE
	AddCtrl(m_pSize);

	m_pDate = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDate->SetId(eDate);
	m_pDate->SetText(StringC(AT_DCP05,P_DCP_DATE_OF_FILE_TOK));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pDate->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pDate->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pDate);

	m_pTime = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pTime->SetId(eTime);
	m_pTime->SetText(StringC(AT_DCP05,P_DCP_TIME_OF_FILE_TOK));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pTime->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pTime->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pTime);

	m_pFreeSpace = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFreeSpace->SetId(eFreeSpace);
	m_pFreeSpace->SetText(StringC(AT_DCP05,P_DCP_FREE_SPACE_TOK));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pFreeSpace->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pFreeSpace->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	AddCtrl(m_pFreeSpace);

	//SetHelpTok(H_DCP_3DFILE_TOK,0);
}

// ******************************************************************************************
void DCP05FileDlgC::CreateFile(StringC strNewFileName, StringC sPointId)
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
void DCP05FileDlgC::CopyFile(StringC strNewFileName)
{
	int x;
	x=1;
}

// ******************************************************************************************
void DCP05FileDlgC::Swap()
{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP05,M_DCP_SWAP_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetDCP05Model()->ADFFileName);

		DCP05MsgBoxC MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{

		}
}
// ******************************************************************************************
void DCP05FileDlgC::Close()
{
	m_pDataModel->m_pAdfFile->CloseFile();
	sSelectedFile = L" ";

	RefreshControls();
}
// ******************************************************************************************
void DCP05FileDlgC::Delete()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
	{
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP05,M_DCP_DELETE_FILE_TOK);
		strMsg.Format(strMsg,(const wchar_t*)GetDCP05Model()->ADFFileName);

		DCP05MsgBoxC MsgBox;
		if(MsgBox.ShowMessageYesNo(strMsg))
		{

		}
	}
}
// ******************************************************************************************
void DCP05FileDlgC::Save()
{

}
void DCP05FileDlgC::View()
{

}
void DCP::DCP05FileDlgC::SelectFile(StringC sFile)
{
	sSelectedFile = sFile;
	RefreshControls();
}

// ******************************************************************************************
void DCP05FileDlgC::OnDialogActivated()
{
	sSelectedFile = GetDCP05Model()->ADFFileName;
	//DCP05MsgBoxC MsgBox;
	//MsgBox.ShowMessageOk(sSelectedFile);
	if(!sSelectedFile.IsEmpty())
		m_pDataModel->m_pAdfFile->setFile(GetDCP05Model()->ADFFileName);

	m_pCommon = new DCP05CommonC(GetDCP05Model());
	RefreshControls();
}


// Description: refresh all controls
void DCP::DCP05FileDlgC::RefreshControls()
{	
	if(m_pFile && m_pNumberOfPoints && m_pSize && m_pDate && m_pTime && m_pFreeSpace)
	{
		//m_pDataModel->m_pAdfFile->setFile(sSelectedFile/*GetDCP05Model()->ADFFileName*/);

		if(m_pDataModel->m_pAdfFile->IsOpen())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getFileName());//GetDCP05Model()->ADFFileName);	
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

void DCP::DCP05FileDlgC::UpdateData()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
        GetDCP05Model()->ADFFileName = m_pDataModel->m_pAdfFile->getFileName();
	else
		GetDCP05Model()->ADFFileName = L"";
	//GetDCP05Model()->m_nOption = m_pComboBox->GetComboBoxInputCtrl()->GetSelectedId();
}


// Description: only accept hello world Model objects
bool DCP::DCP05FileDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP05ModelC* DCP::DCP05FileDlgC::GetDCP05Model() const
{
    return (DCP::DCP05ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ******************************************************************************

DCP::DCP05FileControllerC::DCP05FileControllerC(DCP::DCP05ModelC* pDCP05Model)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_3DFILE_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	
	// FileModel
	m_pDataModel = new DCP::DCP05FileModelC(m_pDCP05Model); 
    // Create a dialog
    m_pDlg = new DCP::DCP05FileDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DFILE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP05,K_DCP_OPEN_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_NEW_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_COPY_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_SWAP_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CLOSE_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );
	
	vDef.strLable = StringC(AT_DCP05,K_DCP_SAVE_TOK);
	SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_VIEW_TOK);
	SetFunctionKey( SHFK5, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP05FileControllerC::~DCP05FileControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP05FileControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP05Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP05Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}


// OPEN
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF1Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::DCP05SelectFileModelC* pModel = new DCP05SelectFileModelC;

		if(GetController(SELECT_FILE_CONTROLLER) == NULL)
		{
			StringC sTitle = GetTitle();	
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP05SelectFileControllerC(ALL_FILES, sTitle, m_pDCP05Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
}

// NEW
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF2Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}
		
		DCP::DCP05InputTextModelC* pModel = new DCP05InputTextModelC;
		pModel->m_StrInfoText.LoadTxt(AT_DCP05, L_DCP_ENTER_NEW_FILENAME_TOK);
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
			(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP05InputTextControllerC(m_pDCP05Model ));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP05Model());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);
}

// copy
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF3Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	if (m_pDlg == NULL)
	{
		USER_APP_VERIFY( false );
		return;
	}
		
		DCP::DCP05InputTextModelC* pModel = new DCP05InputTextModelC;
		pModel->m_StrInfoText.LoadTxt(AT_DCP05, L_DCP_COPY_FILE_TO_TOK);
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
			(void)AddController( COPY_FILE_TO_CONTROLLER, new DCP::DCP05InputTextControllerC( m_pDCP05Model));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP05Model());
		(void)GetController( COPY_FILE_TO_CONTROLLER )->SetModel(pModel);
		SetActiveController(COPY_FILE_TO_CONTROLLER, true);
}

// CLOSE
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF5Pressed()
{
	if(m_pDataModel->m_pAdfFile->IsOpen())
		m_pDataModel->m_pAdfFile->CloseFile();

	m_pDlg->RefreshControls();
}

// SWAP
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF4Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	m_pDataModel->m_pAdfFile->swap_meas_design();
	m_pDlg->RefreshControls();
}

// CONT
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnF6Pressed()
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
void DCP::DCP05FileControllerC::OnSHF2Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;
	m_pDataModel->m_pAdfFile->delete_adf_file();
	m_pDlg->RefreshControls();
}

// SAVE
// *********************************************************************************************
void DCP::DCP05FileControllerC::OnSHF3Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;
	m_pDataModel->m_pAdfFile->save_adf_to_ref_dat();
	m_pDlg->RefreshControls();
}

// VIEW
// ******************************************************************************************
void DCP::DCP05FileControllerC::OnSHF5Pressed()
{
	if(!m_pDataModel->m_pAdfFile->IsOpen())
		return;

	
	if(GetController(_3DFILEVIEW_CONTROLLER) == NULL)		
	{
		(void)AddController( _3DFILEVIEW_CONTROLLER, new DCP::DCP053DFileViewControllerC (m_pDataModel));
	}
	(void)GetController( _3DFILEVIEW_CONTROLLER )->SetModel(m_pDlg->GetDCP05Model());
	SetActiveController(_3DFILEVIEW_CONTROLLER, true);
	
}


// ******************************************************************************************
void DCP::DCP05FileControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05FileControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectFileModelC* pModel = (DCP::DCP05SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		m_pDataModel->m_pAdfFile->setFile(strSelectedFile);
	}

	if(lExitCode == EC_KEY_CONT)
	{
		if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
		{
			DCP::DCP05InputTextModelC* pModel = (DCP::DCP05InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			sFileToCreate = strNewFile;
			sFileToCreate.Trim();
			// and ask first point id
			
			DCP::DCP05InputTextModelC* pModel1 = new DCP05InputTextModelC;
			pModel1->m_StrInfoText.LoadTxt(AT_DCP05, L_DCP_ENTER_POINT_ID_TOK);
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
				(void)AddController( INPUT_FIRST_POINTID_CONTROLLER, new DCP::DCP05InputTextControllerC( m_pDCP05Model));
			}

			//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDlg->GetDCP05Model());
			(void)GetController( INPUT_FIRST_POINTID_CONTROLLER )->SetModel(pModel1);
			SetActiveController(INPUT_FIRST_POINTID_CONTROLLER, true);
			

			 //m_pDlg->CreateFile(sFileToCreate, pModel->m_StrText);


		}
		else if(lCtrlID == COPY_FILE_TO_CONTROLLER)
		{
			DCP::DCP05InputTextModelC* pModel = (DCP::DCP05InputTextModelC*) GetController( COPY_FILE_TO_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char fname[CPI::LEN_PATH_MAX];
			//char pointid[10];
			//UTL::UnicodeToAscii(fname, strNewFile);
			BSS::UTI::BSS_UTI_WCharToAscii( strNewFile, fname );
			strcat(fname,".adf");
			if(m_pDataModel->m_pAdfFile->copy_adf_file(fname))
			{
				m_pDataModel->m_pAdfFile->setFile(strNewFile);
				m_pDataModel->m_pAdfFile->add_header_to_adf();
			}

		}
		
		else if(lCtrlID == INPUT_FIRST_POINTID_CONTROLLER)
		{
			DCP::DCP05InputTextModelC* pModel = (DCP::DCP05InputTextModelC*) GetController( INPUT_FIRST_POINTID_CONTROLLER )->GetModel();		
			
			StringC sPointId = pModel->m_StrText;
			// and create new file
			char fname[CPI::LEN_PATH_MAX];
			char pointid[10];
			//DCP05MsgBoxC msgbox;
			//msgbox.ShowMessageOk(sFileToCreate);
			//UTL::UnicodeToAscii(fname, sFileToCreate);
			BSS::UTI::BSS_UTI_WCharToAscii( sFileToCreate, fname );
			//UTL::UnicodeToAscii(pointid, sPointId);
			BSS::UTI::BSS_UTI_WCharToAscii( sPointId, pointid );

			m_pDataModel->m_pAdfFile->create_adf_file(fname,pointid);
			m_pDataModel->m_pAdfFile->setFile(sFileToCreate);
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
DCP::DCP05FileModelC::DCP05FileModelC(DCP::DCP05ModelC* pDCP05Model):m_pDCP05Model(pDCP05Model)
{
	m_pAdfFile = 
		new AdfFileFunc(m_pDCP05Model);
	m_pAdfFile->always_single = 1;

}
DCP::DCP05FileModelC::~DCP05FileModelC()
{
	if(m_pAdfFile)
	{
		delete m_pAdfFile;
		m_pAdfFile = 0;
	}
}



