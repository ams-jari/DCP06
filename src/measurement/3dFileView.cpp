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
#include <string>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Logger.hpp>
#include <dcp06/measurement/3dFileView.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/core/SelectPoint.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/InputText.hpp>
#include <dcp06/core/SelectPoint.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

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



// ================================================================================================
// ====================================  FileView3DDialog         ==============================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::FileView3DDialog::FileView3DDialog(DCP::FileModel* pDataModel):GUI::ModelHandlerC(),GUI::StandardDialogC()
			,m_pFile(0),m_pPointId(0), m_pXDsg(0), m_pYDsg(0), m_pZDsg(0),
			m_pXActDev(0),m_pYActDev(0),m_pZActDev(0),m_pDataModel(pDataModel)
{
	//SetTxtApplicationId(AT_DCP06);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::FileView3DDialog::~FileView3DDialog()
{
	if(pCommon)
	{
		delete pCommon;
		pCommon =  0;
	}
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::FileView3DDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pFile);
	
	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP06,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pPointId);

	m_pXDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pXDsg->SetId(eXDsg);
	m_pXDsg->SetText(StringC(AT_DCP06,P_DCP_X_DSG_TOK));
	m_pXDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pXDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	// TODO
	//m_pXDsg->SetAutoColon(false);
	//m_pXDsg->SetColonPosition(9 * 11);
	m_pXDsg->SetEmptyAllowed(true);
	AddCtrl(m_pXDsg);

	m_pYDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pYDsg->SetId(eYDsg);
	m_pYDsg->SetText(StringC(AT_DCP06,P_DCP_Y_DSG_TOK));
	m_pYDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pYDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pYDsg->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pYDsg->SetEmptyAllowed(true);
	//m_pYDsg->SetAutoColon(false);
	//m_pYDsg->SetColonPosition(9 * 11);
	AddCtrl(m_pYDsg);

	m_pZDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZDsg->SetId(eZDsg);
	m_pZDsg->SetText(StringC(AT_DCP06,P_DCP_Z_DSG_TOK));

	m_pZDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pZDsg->SetEmptyAllowed(true);
	
	//m_pZDsg->SetAutoColon(false);
	//m_pZDsg->SetColonPosition(9 * 11);
	AddCtrl(m_pZDsg);

	//InsertEmptyLine(); CAPTIVATE
	//SetColonPosLong( GUI::StandardDialogC::CP_10 );
	m_pXActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pXActDev->SetId(eXActDev);
	m_pXActDev->SetText(StringC(AT_DCP06,P_DCP_X_ACT_DEV_TOK));
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXActDev->SetEmptyAllowed(true);
	//m_pXActDev->SetAutoColon(false);
	//m_pXActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pXActDev);

	m_pYActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYActDev->SetId(eYActDev);
	m_pYActDev->SetText(StringC(AT_DCP06,P_DCP_Y_ACT_DEV_TOK));
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYActDev->SetEmptyAllowed(true);
	//m_pYActDev->SetAutoColon(false);
	//m_pYActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pYActDev);

	m_pZActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZActDev->SetId(eZActDev);
	m_pZActDev->SetText(StringC(AT_DCP06,P_DCP_Z_ACT_DEV_TOK));
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZActDev->SetEmptyAllowed(true);
	//m_pZActDev->SetAutoColon(false);
	//m_pZActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pZActDev);

	//SetHelpTok(H_DCP_3DFILE_VIEW_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::FileView3DDialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = GetModel() && GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase()) : 0;
	bool useDb = jdb && jdb->isJobLoaded() && !GetModel()->m_currentJobId.empty();
	if (!useDb)
		m_pDataModel->m_pAdfFile->always_single = 1;
	pCommon = new Common(GetModel());
	if (useDb) GetModel()->m_currentPointIndex = 1;
	RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::FileView3DDialog::UpdateData()
{
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::FileView3DDialog::RefreshControls()
{
	DCP06_TRACE_ENTER;
	if(m_pFile && m_pPointId && m_pXDsg && m_pYDsg && m_pZDsg && m_pXActDev &&
		m_pYActDev && m_pZActDev) 
	{
		char xdes_buf[32], ydes_buf[32], zdes_buf[32];
		char xmea_buf[32], ymea_buf[32], zmea_buf[32];
		char* xdes_ptr = xdes_buf, *ydes_ptr = ydes_buf, *zdes_ptr = zdes_buf;
		char* xmea_ptr = xmea_buf, *ymea_ptr = ymea_buf, *zmea_ptr = zmea_buf;
		char pointid_buf[32];
		DCP::Database::JsonDatabase* jdb = GetModel() && GetModel()->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(GetModel()->GetDatabase()) : 0;
		bool useDb = jdb && jdb->isJobLoaded() && !GetModel()->m_currentJobId.empty();
		if (useDb)
		{
			pointid_buf[0] = xdes_buf[0] = ydes_buf[0] = zdes_buf[0] = xmea_buf[0] = ymea_buf[0] = zmea_buf[0] = '\0';
			int idx = GetModel()->m_currentPointIndex;
			if (idx > 0 && jdb->getPointByIndex(idx, true, pointid_buf, xmea_buf, xdes_buf, ymea_buf, ydes_buf, zmea_buf, zdes_buf, 0))
			{
				// buffers filled
			}
		}
		if (!useDb)
		{
			xdes_buf[0] = ydes_buf[0] = zdes_buf[0] = xmea_buf[0] = ymea_buf[0] = zmea_buf[0] = pointid_buf[0] = '\0';
			xdes_ptr = xdes_buf; ydes_ptr = ydes_buf; zdes_ptr = zdes_buf;
			xmea_ptr = xmea_buf; ymea_ptr = ymea_buf; zmea_ptr = zmea_buf;
		}

		char temp_act[100];
		char x_diff[XYZ_VALUE_BUFF_LEN];
		char y_diff[XYZ_VALUE_BUFF_LEN];
		char z_diff[XYZ_VALUE_BUFF_LEN];

		m_pFile->GetStringInputCtrl()->SetString(useDb && GetModel() ?
			StringC(GetModel()->m_currentJobId.c_str()) : StringC(L""));
		
		char coord_str[XYZ_MEA_AND_DIFF_BUFF_LEN];
		
		char pointid_temp[POINT_ID_BUFF_LEN];
		sprintf(pointid_temp,"%s", pointid_buf);
		pCommon->strbtrim(pointid_temp);
		m_pPointId->GetStringInputCtrl()->SetString(StringC(pointid_temp));

		if(!pCommon->strblank(xdes_ptr))
		{
			//sprintf(temp,"%.*f",m_pModel->m_nDecimals,atof(GetModel()->m_pAdfFile->xdes_front));
			m_pXDsg->GetFloatInputCtrl()->SetDouble(atof(xdes_ptr));
		}
		else
		{
			m_pXDsg->GetFloatInputCtrl()->SetEmpty();
		}
		
		if(!pCommon->strblank(ydes_ptr))
		{
			sprintf(coord_str,"%.*f",GetModel()->m_nDecimals,atof(ydes_ptr));
			//m_pYDsg->GetFloatInputCtrl()->SetString(StringC(temp));
			m_pYDsg->GetFloatInputCtrl()->SetDouble(atof(ydes_ptr));
		}
		else
		{
			m_pYDsg->GetFloatInputCtrl()->SetEmpty();
		}
		
		if(!pCommon->strblank(zdes_ptr))
		{
			sprintf(coord_str,"%.*f",GetModel()->m_nDecimals,atof(zdes_ptr));
			//m_pZDsg->GetFloatInputCtrl()->SetString(StringC(temp));
			m_pZDsg->GetFloatInputCtrl()->SetDouble(atof(zdes_ptr));
		}
		else
		{
			m_pZDsg->GetFloatInputCtrl()->SetEmpty();
		}

		pCommon->get_dist_(xmea_ptr,xdes_ptr,x_diff);
		pCommon->get_dist_(ymea_ptr,ydes_ptr,y_diff);
		pCommon->get_dist_(zmea_ptr,zdes_ptr,z_diff);

		//if(!pCommon->strblank(GetModel()->m_pAdfFile->xmea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s", x_diff,xmea_ptr);
			
		//}
		m_pXActDev->GetStringInputCtrl()->SetString(StringC(temp_act));

		//if(!pCommon->strblank(GetModel()->m_pAdfFile->ymea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s",y_diff, ymea_ptr);
			
		//}
		m_pYActDev->GetStringInputCtrl()->SetString(StringC(temp_act));
		
		//if(!pCommon->strblank(GetModel()->m_pAdfFile->zmea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s", z_diff,  zmea_ptr);
			
		//}
		m_pZActDev->GetStringInputCtrl()->SetString(StringC(temp_act));

		
	}
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: only accept xxx Model objects
// ================================================================================================
bool DCP::FileView3DDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::FileView3DDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  UserController  ===================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::FileView3DController::FileView3DController(DCP::FileModel* pDataModel)
    : m_pDlg( nullptr ),m_pDataModel(pDataModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_3DFILE_VIEW_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::FileView3DDialog(pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DFILEVIEW_DLG, m_pDlg, true );

    // Set the function key
	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
		
	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_ID_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_HOME_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_END_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

		// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP06;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::FileView3DController::~FileView3DController()
{

}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::FileView3DController::OnControllerActivated(void)
{
	
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::FileView3DController::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: F1 / PID
// ================================================================================================
void DCP::FileView3DController::OnF1Pressed()
{
	DCP06_TRACE_ENTER;
		DCP::SelectPointModel* pModel = new SelectPointModel;

		Database::JsonDatabase* jdb = m_pDlg->GetModel()->GetDatabase() ?
			dynamic_cast<Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
		if (!jdb || !jdb->isJobLoaded())
		{
			MsgBox msgBox; StringC msg; msg.LoadTxt(AT_DCP06, M_DCP_3DFILE_ISNOT_OPEN_TOK); msgBox.ShowMessageOk(msg);
			return;
		}
		int iCount = jdb->getPointListAsSelectPoint(&pModel->points[0], MAX_SELECT_POINTS);
		pModel->m_iCounts = iCount;
		pModel->m_iSelectedId = m_pDlg->GetModel()->m_currentPointIndex;

		if(GetController(SELECT_POINT_CONTROLLER) == nullptr)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::SelectPointController );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
	DCP06_TRACE_EXIT;
}


// ================================================================================================
// Description: F2 (first point)
// ================================================================================================
void DCP::FileView3DController::OnF2Pressed()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
	if (jdb && jdb->isJobLoaded() && !m_pDlg->GetModel()->m_currentJobId.empty())
		m_pDlg->GetModel()->m_currentPointIndex = 1;
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: F3 (last point)
// ================================================================================================
void DCP::FileView3DController::OnF3Pressed()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
	if (jdb && jdb->isJobLoaded() && !m_pDlg->GetModel()->m_currentJobId.empty())
		m_pDlg->GetModel()->m_currentPointIndex = jdb->getJobPointsCount();
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: F4  / PREV
// ================================================================================================
void DCP::FileView3DController::OnF4Pressed()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
	if (jdb && jdb->isJobLoaded() && m_pDlg->GetModel()->m_currentPointIndex > 1)
		m_pDlg->GetModel()->m_currentPointIndex--;
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: F5 / NEXT
// ================================================================================================
void DCP::FileView3DController::OnF5Pressed()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
	int nPts = jdb ? jdb->getJobPointsCount() : 0;
	if (jdb && jdb->isJobLoaded() && m_pDlg->GetModel()->m_currentPointIndex < nPts)
		m_pDlg->GetModel()->m_currentPointIndex++;
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: F6 
// ================================================================================================
void DCP::FileView3DController::OnF6Pressed()
{
	DCP06_TRACE_ENTER;
	Close(EC_KEY_CONT);
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: SHF2 / DELETE
// ================================================================================================
void DCP::FileView3DController::OnSHF2Pressed()
{
	DCP06_TRACE_ENTER;
	DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
		dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
	if (jdb && jdb->isJobLoaded() && !m_pDlg->GetModel()->m_currentJobId.empty())
	{
		char pid[32];
		pid[0] = '\0';
		if (jdb->getPointByIndex(m_pDlg->GetModel()->m_currentPointIndex, true, pid, 0,0,0,0,0,0,0))
		{
			std::string id(pid);
			size_t end = id.find_last_not_of(" \t");
			if (end != std::string::npos) id = id.substr(0, end + 1);
			else if (!id.empty()) { id = id.substr(0, id.find_first_of(" \t")); if (id.empty()) id = pid; }
			if (!id.empty()) jdb->deletePoint(id);
			int n = jdb->getJobPointsCount();
			if (m_pDlg->GetModel()->m_currentPointIndex > n)
				m_pDlg->GetModel()->m_currentPointIndex = (n > 0 ? n : 1);
		}
	}
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::FileView3DController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::FileView3DController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	DCP06_TRACE_ENTER;
	if (lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectPointModel* pModel = (DCP::SelectPointModel*)GetController(SELECT_POINT_CONTROLLER)->GetModel();
		DCP::Database::JsonDatabase* jdb = m_pDlg->GetModel() && m_pDlg->GetModel()->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(m_pDlg->GetModel()->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded())
			m_pDlg->GetModel()->m_currentPointIndex = pModel->m_iSelectedId;
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: ConfirmClose
// ================================================================================================
bool DCP::FileView3DController::ConfirmClose(bool bEsc)
{
	return true;
}
