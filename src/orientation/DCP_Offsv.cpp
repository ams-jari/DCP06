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
#include <dcp06/init/DCP_DCP06Init.hpp>
#include <dcp06/orientation/DCP_Offsv.hpp>
#include <dcp06/orientation/DCP_MeasV.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_SeparateRec.hpp>
#include <dcp06/measurement/DCP_HomePoints.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/file/DCP_File.hpp>
#include <dcp06/core/DCP_SelectPoint.hpp>
#include <dcp06/core/DCP_SelectOnePoint.hpp>
#include <dcp06/file/DCP_SelectFile.hpp>

#include <GUI_Types.hpp>

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

DCP::DCP06OffsvDlgC::DCP06OffsvDlgC(DCP::DCP06ModelC* pDCP06Model):GUI::ModelHandlerC(),GUI::StandardDialogC(),
	m_pFile(0),m_pPointId(0),m_pX(0),m_pY(0),m_pZ(0),m_pDCP06Model(pDCP06Model),m_sFile(L"")
{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::DCP06OffsvDlgC::~DCP06OffsvDlgC()
{

}

void DCP::DCP06OffsvDlgC::OnInitDialog(void)
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
	m_pX->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP06Model->m_nDecimals);
	m_pX->SetEmptyAllowed(true);
	// m_pX->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pX);

	m_pY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pY->SetId(eY);
	m_pY->SetText(StringC(AT_DCP06,P_DCP_Y_TOK));
	m_pY->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP06Model->m_nDecimals);
	m_pY->SetEmptyAllowed(true);
	// m_pY->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pY);

	m_pZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZ->SetId(eZ);
	m_pZ->SetText(StringC(AT_DCP06,P_DCP_Z_TOK));
	m_pZ->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP06Model->m_nDecimals);
	m_pZ->SetEmptyAllowed(true);
	// m_pZ->GetFloatInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	AddCtrl(m_pZ);

	//SetHelpTok(H_DCP_OFFSET_POINT_TOK,0);
}

void DCP::DCP06OffsvDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP06OffsvDlgC::UpdateData()
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
void DCP::DCP06OffsvDlgC::RefreshControls()
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
// Description: only accept hello world Model objects
bool DCP::DCP06OffsvDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06OffsVModelC* pDCP06Model = dynamic_cast< DCP::DCP06OffsVModelC* >( pModel );

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
DCP::DCP06OffsVModelC* DCP::DCP06OffsvDlgC::GetDataModel() const
{
    return (DCP::DCP06OffsVModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

StringC DCP::DCP06OffsvDlgC::getfileName()
{
	StringC sSelectedFile = m_pFile->GetStringInputCtrl()->GetString();
	return sSelectedFile;
}

void DCP::DCP06OffsvDlgC::delete_point()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINT_TOK);

	StringC strActivePointId(L"");

	strMsg.Format(strMsg, (const wchar_t*)strActivePointId);
	DCP06MsgBoxC MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		memset(&GetDataModel()->ovalues_buff,0,sizeof(S_POINT_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP06OffsvDlgC::SelectFile(StringC sFile)
{
	m_sFile = sFile;
	RefreshControls();
	//m_pFile->GetStringInputCtrl()->SetString(sFile);
}

// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06OffsvControllerC::DCP06OffsvControllerC(DCP::DCP06ModelC* pDCP06Model, short display)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'

	if(display != DOM_USERDEF_DLG)
		SetTitle(StringC( AT_DCP06, T_ORIE_DOM_POINT_OFFSV_TOK ));
	else
		SetTitle(StringC( AT_DCP06, T_DCP_USERDEF_OFFSV_TOK ));
	

    // Create a dialog
    m_pDlg = new DCP::DCP06OffsvDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
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

DCP::DCP06OffsvControllerC::~DCP06OffsvControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06OffsvControllerC::SetModel( GUI::ModelC* pModel )
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

// 3DFile
void DCP::DCP06OffsvControllerC::OnF1Pressed()
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
			(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(ONLY_ADF, sTitle, m_pDCP06Model) );
		}
		(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_FILE_CONTROLLER, true);
}

// PID
void DCP::DCP06OffsvControllerC::OnF2Pressed()
{
		if (m_pDlg == NULL)
	    {
		    USER_APP_VERIFY( false );
			return;
		}

		// get adf file name

		AdfFileFunc adf(m_pDCP06Model);
		adf.always_single = 1;

		StringC sSelectedFile = m_pDlg->getfileName();
		
		if(adf.setFile(sSelectedFile))
		{
			
			// and select point from file
			DCP::DCP06SelectOnePointModelC* pModel = new DCP::DCP06SelectOnePointModelC();

			int iCount = adf.GetPointList(&pModel->points[0],MAX_SELECT_POINTS, DESIGN);
			if(iCount)
			{
				pModel->m_iPointsCount = iCount;
				pModel->sSelectedFile = sSelectedFile;
				pModel->m_iDef = DESIGN;
				
				if(GetController(SELECT_ONE_POINT_CONTROLLER) == NULL)
				{
					(void)AddController( SELECT_ONE_POINT_CONTROLLER, new DCP::DCP06SelectOnePointControllerC(m_pDCP06Model));
				}
				(void)GetController( SELECT_ONE_POINT_CONTROLLER )->SetModel(pModel);
				SetActiveController(SELECT_ONE_POINT_CONTROLLER, true);
			}
			
		}
		// select file
		else
		{
			DCP::DCP06SelectFileModelC* pModel = new DCP06SelectFileModelC;

			if(GetController(SELECT_FILE_CONTROLLER) == NULL)
			{
				StringC sTitle = GetTitle();	
				(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(ONLY_ADF, sTitle, m_pDCP06Model) );
			}
			(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
			SetActiveController(SELECT_FILE_CONTROLLER, true);
		}

/*		DCP::DCP06SelectPointModelC* pModel = new DCP06SelectPointModelC;

		if(GetController(SELECT_POINT_CONTROLLER) == NULL)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP06SelectPointControllerC );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
*/
}

// MEASV
void DCP::DCP06OffsvControllerC::OnF5Pressed()
{
  if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP06OffsVModelC* pModel = new DCP::DCP06OffsVModelC();
	// copy point id
	sprintf(m_pDlg->GetDataModel()->ref_point_buff.point_id,"%s",m_pDlg->GetDataModel()->ovalues_buff.point_id);
	
	memcpy(&pModel->ovalues_buff,&m_pDlg->GetDataModel()->ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDlg->GetDataModel()->ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDlg->GetDataModel()->ovalues_tool_buff,sizeof(S_POINT_BUFF));
	


	if(GetController(MEASV_CONTROLLER) == NULL)
	{
		(void)AddController( MEASV_CONTROLLER, new DCP::DCP06MeasVControllerC(m_pDCP06Model));
	}
	(void)GetController( MEASV_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEASV_CONTROLLER, true);

}


// CONT
void DCP::DCP06OffsvControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
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
void DCP::DCP06OffsvControllerC::OnSHF2Pressed()
{
	m_pDlg->delete_point();
}


// Description: React on close of tabbed dialog
void DCP::DCP06OffsvControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06OffsvControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEASV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{

		DCP::DCP06OffsVModelC* pModel = (DCP::DCP06OffsVModelC*) GetController( MEASV_CONTROLLER )->GetModel();		
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

		DCP::DCP06SelectOnePointModelC* pModel = (DCP::DCP06SelectOnePointModelC*) GetController( SELECT_ONE_POINT_CONTROLLER )->GetModel();		
		// copy values into dommodel
		int iSelected = pModel->iSelectedNo;
		
		AdfFileFunc adf(m_pDCP06Model);
		adf.always_single = 1;
		DCP06CommonC common(m_pDCP06Model);
		if(adf.setFile(pModel->sSelectedFile))
		{	
			char bXmea[15], bYmea[15], bZmea[15];
			char bXdes[15], bYdes[15], bZdes[15],pid[7];//,fname[13];
			
			adf.form_pnt1((int) iSelected, pid, NULL, bXmea, bXdes, NULL, bYmea, bYdes, NULL, bZmea, bZdes, NULL);
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
		DCP::DCP06SelectFileModelC* pModel = (DCP::DCP06SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		StringC strSelectedFile = pModel->m_strSelectedFile;
		m_pDlg->SelectFile(strSelectedFile);
		
		AdfFileFunc adf(m_pDCP06Model);
		adf.always_single = 1;
		if(adf.setFile(strSelectedFile))
		{
			
			// and select point from file
			DCP::DCP06SelectOnePointModelC* pModel = new DCP::DCP06SelectOnePointModelC();

			int iCount = adf.GetPointList(&pModel->points[0],MAX_SELECT_POINTS, DESIGN);
			if(iCount)
			{
				pModel->m_iPointsCount = iCount;
				pModel->sSelectedFile = strSelectedFile;
				pModel->m_iDef = DESIGN;
				
				if(GetController(SELECT_ONE_POINT_CONTROLLER) == NULL)
				{
					(void)AddController( SELECT_ONE_POINT_CONTROLLER, new DCP::DCP06SelectOnePointControllerC(m_pDCP06Model));
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
			if(GetController(HIDDENPOINT_CONTROLLER) == NULL)
			{
				(void)AddController( HIDDENPOINT_CONTROLLER, new DCP::DCP06HiddenPointControllerC );
			}
			(void)GetController( HIDDENPOINT_CONTROLLER )->SetModel( m_pDlg->GetDCP06Model());
			SetActiveController(HIDDENPOINT_CONTROLLER, true);


		}
		else if(lExitCode == X_OR_Y_OR_Z)
		{
			if(GetController(XYZ_CONTROLLER) == NULL)
			{
				(void)AddController( XYZ_CONTROLLER, new DCP::DCP06XYZControllerC );
			}
			(void)GetController( XYZ_CONTROLLER )->SetModel( m_pDCP06OffsvDlg->GetDCP06Model());
			SetActiveController(XYZ_CONTROLLER, true);
		}
		else if(lExitCode == CIRCLE)
		{
			if(GetController(CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CIRCLE_CONTROLLER, new DCP::DCP06CircleControllerC );
			}
			(void)GetController( CIRCLE_CONTROLLER )->SetModel( m_pDCP06OffsvDlg->GetDCP06Model());
			SetActiveController(CIRCLE_CONTROLLER, true);


		}
		else if(lExitCode == SEPARATE_REC)
		{
			if(GetController(SEPARATE_RECORDING_CONTROLLER) == NULL)
			{
				(void)AddController( SEPARATE_RECORDING_CONTROLLER, new DCP::DCP06SeparateRecControllerC );
			}
			(void)GetController( SEPARATE_RECORDING_CONTROLLER )->SetModel( m_pDCP06OffsvDlg->GetDCP06Model());
			SetActiveController(SEPARATE_RECORDING_CONTROLLER, true);
		}
		else if(lExitCode == HOME_POINTS)
		{
			if(GetController(HOME_POINTS_CONTROLLER) == NULL)
			{
				(void)AddController( HOME_POINTS_CONTROLLER, new DCP::DCP06HomePointsControllerC );
			}
			(void)GetController( HOME_POINTS_CONTROLLER )->SetModel( m_pDCP06OffsvDlg->GetDCP06Model());
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
DCP::DCP06OffsVModelC::DCP06OffsVModelC()
{
	// copy from DCP05Model
}

DCP::DCP06OffsVModelC::~DCP06OffsVModelC()
{
}
