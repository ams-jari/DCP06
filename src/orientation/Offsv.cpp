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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/orientation/Offsv.hpp>
#include <dcp06/orientation/MeasV.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/measurement/SeparateRec.hpp>
#include <dcp06/measurement/HomePoints.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/core/SelectPoint.hpp>
#include <dcp06/core/SelectOnePoint.hpp>
#include <dcp06/file/SelectFile.hpp>

#include <GUI_Types.hpp>

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

DCP::OffsetVDialog::OffsetVDialog(DCP::Model* pModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pFile(0),m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pModel(pModel),m_sFile(L"")
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::OffsetVDialog::~OffsetVDialog()
{

}

void DCP::OffsetVDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	// m_pFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE

	AddCtrl(m_pFile);

	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pPointId->SetEmptyAllowed(true);
	// m_pPointId->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pPointId);

	// InsertEmptyLine(); CAPTIVATE

	m_pX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pX->SetId(eX);
	m_pX->SetText(StringC(AT_DCP06,P_DCP_X_TOK));
	m_pX->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pX->SetEmptyAllowed(true);
	// m_pX->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP06,P_DCP_Y_TOK));
	m_pY->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pY->SetEmptyAllowed(true);
	// m_pY->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP06,P_DCP_Z_TOK));
	m_pZ->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	m_pZ->SetEmptyAllowed(true);
	// m_pZ->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pZ);

	//SetHelpTok(H_DCP_OFFSET_POINT_TOK,0);
}

void DCP::OffsetVDialog::OnDialogActivated()
{
	RefreshControls();
}

void DCP::OffsetVDialog::UpdateData()
{
		if(!m_pX->GetFloatInputCtrl()->IsEmpty() && !m_pY->GetFloatInputCtrl()->IsEmpty() &&
		!m_pZ->GetFloatInputCtrl()->IsEmpty())
	{
		GetDataModel()->ovalues_buff.sta	= POINT_DESIGN;
		GetDataModel()->ovalues_buff.x		= m_pX->GetFloatInputCtrl()->GetDouble();
		GetDataModel()->ovalues_buff.y		= m_pY->GetFloatInputCtrl()->GetDouble();
        GetDataModel()->ovalues_buff.z		= m_pZ->GetFloatInputCtrl()->GetDouble();

	}
}

// Description: refresh all controls
void DCP::OffsetVDialog::RefreshControls()
{
	if(m_pX && m_pY && m_pZ && m_pFile && m_pPointId)
	{
		if(!m_sFile.IsEmpty())
		{
			m_pFile->GetStringInputCtrl()->SetString(m_sFile);
		}
		else
			m_pFile->GetStringInputCtrl()->SetString(L" ");

		StringC sPointId(GetDataModel()->ovalues_buff.point_id);
		
		if(!sPointId.IsEmpty())
		{
			m_pPointId->GetStringInputCtrl()->SetString(sPointId);
		}
		else
			m_pPointId->GetStringInputCtrl()->SetString(L" ");

		if(GetDataModel()->ovalues_buff.sta != POINT_NOT_DEFINED)
		{
			m_pX->GetFloatInputCtrl()->SetDouble(GetDataModel()->ovalues_buff.x);
			m_pY->GetFloatInputCtrl()->SetDouble(GetDataModel()->ovalues_buff.y);
			m_pZ->GetFloatInputCtrl()->SetDouble(GetDataModel()->ovalues_buff.z);

		}
		else
		{
			m_pX->GetFloatInputCtrl()->SetEmpty();
			m_pY->GetFloatInputCtrl()->SetEmpty();
			m_pZ->GetFloatInputCtrl()->SetEmpty();
		}
	}
}
// Description: only accept DCP06 Model objects
bool DCP::OffsetVDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::OffsetVModel* pDcpModel = dynamic_cast< DCP::OffsetVModel* >( pModel );

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
DCP::OffsetVModel* DCP::OffsetVDialog::GetDataModel() const
{
    return (DCP::OffsetVModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

StringC DCP::OffsetVDialog::getfileName()
{
	StringC sSelectedFile = m_pFile->GetStringInputCtrl()->GetString();
	return sSelectedFile;
}

void DCP::OffsetVDialog::delete_point()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINT_TOK);

	StringC strActivePointId(L"");

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	MsgBox MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		memset(&GetDataModel()->ovalues_buff,0,sizeof(S_POINT_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::OffsetVDialog::SelectFile(StringC sFile)
{
	m_sFile = sFile;
	RefreshControls();
	//m_pFile->GetStringInputCtrl()->SetString(sFile);
}

// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UserController
// 
DCP::OffsetVController::OffsetVController(DCP::Model* pModel, short display)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'

	if(display != DOM_USERDEF_DLG)
		SetTitle(StringC( AT_DCP06, T_ORIE_DOM_POINT_OFFSV_TOK ));
	else
		SetTitle(StringC( AT_DCP06, T_DCP_USERDEF_OFFSV_TOK ));
	

    // Create a dialog
    m_pDlg = new DCP::OffsetVDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( OFFSV_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_3DFILE_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
	SetFunctionKey( FK2, vDef );

	if(display != DOM_USERDEF_DLG)
	{
		vDef.strLable = StringC(AT_DCP06,K_DCP_MEASV_TOK);
		SetFunctionKey( FK5, vDef );
	}

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::OffsetVController::~OffsetVController()
{

}
// Description: Route model to everybody else
bool DCP::OffsetVController::SetModel( GUI::ModelC* pModel )
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

// 3DFile
void DCP::OffsetVController::OnF1Pressed()
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
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle, m_pModel) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
}

// PID
void DCP::OffsetVController::OnF2Pressed()
{
		if (m_pDlg == nullptr)
	    {
		    USER_APP_VERIFY( false );
			return;
		}

		// get adf file name

		AdfFileFunc adf(m_pModel);
		adf.always_single = 1;

		StringC sSelectedFile = m_pDlg->getfileName();
		
		if(adf.setFile(sSelectedFile))
		{
			
			// and select point from file
			DCP::SelectOnePointModel* pModel = new DCP::SelectOnePointModel();

			int iCount = adf.GetPointList(&pModel->points[0],MAX_SELECT_POINTS, DESIGN);
			if(iCount)
			{
				pModel->m_iPointsCount = iCount;
				pModel->sSelectedFile = sSelectedFile;
				pModel->m_iDef = DESIGN;
				
				if(GetController(SELECT_ONE_POINT_CONTROLLER) == nullptr)
				{
					(void)AddController( SELECT_ONE_POINT_CONTROLLER, new DCP::SelectOnePointController(m_pModel));
				}
				(void)GetController( SELECT_ONE_POINT_CONTROLLER )->SetModel(pModel);
				SetActiveController(SELECT_ONE_POINT_CONTROLLER, true);
			}
			
		}
		// select file
		else
		{
			DCP::SelectFileModel* pModel = new SelectFileModel;

			if(GetController(SELECT_FILE_CONTROLLER) == nullptr)
			{
				StringC sTitle = GetTitle();	
				(void)AddController( SELECT_FILE_CONTROLLER, new DCP::SelectFileController(ONLY_ADF, sTitle, m_pModel) );
			}
			(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
			SetActiveController(SELECT_FILE_CONTROLLER, true);
		}

/*		DCP::SelectPointModel* pModel = new SelectPointModel;

		if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
*/
}

// MEASV
void DCP::OffsetVController::OnF5Pressed()
{
  if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::OffsetVModel* pModel = new DCP::OffsetVModel();
	// copy point id
	sprintf(m_pDlg->GetDataModel()->ref_point_buff.point_id,"%s",m_pDlg->GetDataModel()->ovalues_buff.point_id);
	
	memcpy(&pModel->ovalues_buff,&m_pDlg->GetDataModel()->ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDlg->GetDataModel()->ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDlg->GetDataModel()->ovalues_tool_buff,sizeof(S_POINT_BUFF));
	


	if(GetController(MEASV_CONTROLLER) == nullptr)
	{
		(void)AddController( MEASV_CONTROLLER, new DCP::MeasVController(m_pModel));
	}
	(void)GetController( MEASV_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEASV_CONTROLLER, true);

}


// CONT
void DCP::OffsetVController::OnF6Pressed()
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
void DCP::OffsetVController::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}


// Description: React on close of tabbed dialog
void DCP::OffsetVController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::OffsetVController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEASV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{

		DCP::OffsetVModel* pModel = (DCP::OffsetVModel*) GetController( MEASV_CONTROLLER )->GetModel();		
			// copy values into dommodel
		memcpy(&m_pDlg->GetDataModel()->ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDataModel()->ovalues_tool_buff,  &pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDataModel()->ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		// Remove the following statement if you don't want an exit
		// to the main menu
		if(m_pDlg->GetDataModel()->ref_point_buff.sta != 0)
			(void)Close(EC_KEY_CONT);
	}
	if(lCtrlID == SELECT_ONE_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{

		DCP::SelectOnePointModel* pModel = (DCP::SelectOnePointModel*) GetController( SELECT_ONE_POINT_CONTROLLER )->GetModel();		
		// copy values into dommodel
		int iSelected = pModel->iSelectedNo;
		
		AdfFileFunc adf(m_pModel);
		adf.always_single = 1;
		Common common(m_pModel);
		if(adf.setFile(pModel->sSelectedFile))
		{	
			char bXmea[15], bYmea[15], bZmea[15];
			char bXdes[15], bYdes[15], bZdes[15],pid[7];//,fname[13];
			
			adf.form_pnt1((int) iSelected, pid, nullptr, bXmea, bXdes, nullptr, bYmea, bYdes, nullptr, bZmea, bZdes, nullptr);
			//if(pModel->iActual_or_design == DESIGN)
			if(pModel->points[iSelected-1].bDesignSelected)
			{
					if(!common.strblank(bXdes) && !common.strblank(bYdes) && !common.strblank(bZdes))
					{
						sprintf(m_pDlg->GetDataModel()->ovalues_buff.point_id,"%-6.6s",pid);
						m_pDlg->GetDataModel()->ovalues_buff.x = atof(bXdes);
						m_pDlg->GetDataModel()->ovalues_buff.y = atof(bYdes);
						m_pDlg->GetDataModel()->ovalues_buff.z = atof(bZdes);
						m_pDlg->GetDataModel()->ovalues_buff.sta = 1;

					}
			}
			else
			{
					if(!common.strblank(bXmea) && !common.strblank(bYmea) && !common.strblank(bZmea))
					{
						sprintf(m_pDlg->GetDataModel()->ovalues_buff.point_id,"%-6.6s",pid);
						m_pDlg->GetDataModel()->ovalues_buff.x = atof(bXmea);
						m_pDlg->GetDataModel()->ovalues_buff.y = atof(bYmea);
						m_pDlg->GetDataModel()->ovalues_buff.z = atof(bZmea);
						m_pDlg->GetDataModel()->ovalues_buff.sta = 1;

					}
			}
			
		}
		// Remove the following statement if you don't want an exit
		// to the main menu
		//(void)Close(EC_KEY_CONT);
	}

	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectFileModel* pModel = (DCP::SelectFileModel*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		m_pDlg->SelectFile(strSelectedFile);
		
		AdfFileFunc adf(m_pModel);
		adf.always_single = 1;
		if(adf.setFile(strSelectedFile))
		{
			
			// and select point from file
			DCP::SelectOnePointModel* pModel = new DCP::SelectOnePointModel();

			int iCount = adf.GetPointList(&pModel->points[0],MAX_SELECT_POINTS, DESIGN);
			if(iCount)
			{
				pModel->m_iPointsCount = iCount;
				pModel->sSelectedFile = strSelectedFile;
				pModel->m_iDef = DESIGN;
				
				if(GetController(SELECT_ONE_POINT_CONTROLLER) == nullptr)
				{
					(void)AddController( SELECT_ONE_POINT_CONTROLLER, new DCP::SelectOnePointController(m_pModel));
				}
				(void)GetController( SELECT_ONE_POINT_CONTROLLER )->SetModel(pModel);
				SetActiveController(SELECT_ONE_POINT_CONTROLLER, true);
			}
			
		}
	}

	/*
	if(lCtrlID == SPECIAL_MENU_CONTROLLER)
	{	
		if(lExitCode == HIDDEN_POINT)
		{
			if(GetController(HIDDENPOINT_CONTROLLER) == nullptr)
			{
				(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::HiddenPointController );
			}
			(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			if(GetController(XYZ_CONTROLLER) == nullptr)
			{
				(void)AddController( XYZ_CONTROLLER, new DCP::XYZController );
			}
			(void)GetController( XYZ_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			if(GetController(CIRCLE_CONTROLLER) == nullptr)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::CircleController );
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == SEPARATE_REC)
		{
			if(GetController(SEPARATE_RECORDING_CONTROLLER) == nullptr)
			{
				(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::SeparateRecController );
			}
			(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			if(GetController(HOME_POINTS_CONTROLLER) == nullptr)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::HomePointsController );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel( m_pDlg->GetModel());
			SetActiveController(HOME_POINTS_CONTROLLER, true);

		}


	}
	*/
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// DOMModel
// ===========================================================================================

// Instantiate template classes
DCP::OffsetVModel::OffsetVModel()
{
	// copy from DCP05Model
}

DCP::OffsetVModel::~OffsetVModel()
{
}
