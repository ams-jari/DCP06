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
#include <dcp06/orientation/Alignment321.hpp>
#include <dcp06/orientation/DefinePlane.hpp>
#include <dcp06/orientation/DefineLine.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/orientation/Offsv.hpp>
#include <dcp06/orientation/MeasV.hpp>
#include <dcp06/orientation/RotatePlane.hpp>
#include <dcp06/orientation/RotateLine.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/calculation/Calculation321.hpp>

#include <GUI_Desktop.hpp>
#include <GUI_Application.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>
//#include <ABL_util.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
// OBS_IMPLEMENT_EXECUTE(DCP::UnitDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
//-------------------------------------------------------------------------------------------------
DCP::Alignment321Dialog::Alignment321Dialog(DCP::Alignment321Model* pAlign321Model):m_pPlane(0),m_pLine(0),m_pPointOffs(0),m_pPointMeas(0),
								m_pRotPlane(0),m_pCalc(0),m_pRotLine(0),m_pDataModel(pAlign321Model)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);

	sXYPlaneText.LoadTxt(AT_DCP06,V_DCP_XY_PLANE_TOK);
	sZXPlaneText.LoadTxt(AT_DCP06,V_DCP_ZX_PLANE_TOK);
	sYZPlaneText.LoadTxt(AT_DCP06,V_DCP_YZ_PLANE_TOK);

	sHZText.LoadTxt(AT_DCP06,V_DCP_HZ_TOK);

	sXLineText.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	sYLineText.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	sZLineText.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);
}


// Description: Destructor
//-------------------------------------------------------------------------------------------------
DCP::Alignment321Dialog::~Alignment321Dialog()
{

}
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Dialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();
	short iColonPos = 9 * 22;
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );

	m_pPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPlane->SetId(ePlane);
	m_pPlane->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_SK_TOK));

	//m_pPlane->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPlane->SetAutoColon(false);
	//m_pPlane->SetColonPosition(iColonPos);
	//m_pPlane->SetTextWidth(9*50); CAPTIVATE

	AddCtrl(m_pPlane);

	m_pLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLine->SetId(eLine);
	m_pLine->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_SK_TOK));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pLine->SetAutoColon(false);
	//m_pLine->SetColonPosition(iColonPos);
	//m_pLine->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pLine);
	
	m_pPointOffs = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointOffs->SetId(ePointOffs);
	m_pPointOffs->SetText(StringC(AT_DCP06,P_DCP_REF_PNT_OFFSV_TOK));
	void(m_pPointOffs->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointOffs->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointOffs->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pPointOffs->SetAutoColon(false);
	//m_pPointOffs->SetColonPosition(iColonPos);
	//m_pPointOffs->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPointOffs);

	m_pPointMeas = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointMeas->SetId(ePointMeas);
	m_pPointMeas->SetText(StringC(AT_DCP06,P_DCP_REF_PNT_MEASV_TOK));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointMeas->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pPointMeas->SetAutoColon(false);
	//m_pPointMeas->SetColonPosition(iColonPos);
	//m_pPointMeas->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPointMeas);

	m_pRotPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRotPlane->SetId(eRotPlane);
	m_pRotPlane->SetText(StringC(AT_DCP06,P_DCP_REF_ROTATE_PLANE_TOK));
	void(m_pRotPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pRotPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pRotPlane->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pRotPlane->SetAutoColon(false);
	//m_pRotPlane->SetColonPosition(iColonPos);
	//m_pRotPlane->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pRotPlane);
	
	m_pRotLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRotLine->SetId(eRotLine);
	m_pRotLine->SetText(StringC(AT_DCP06,P_DCP_REF_ROTATE_LINE_TOK));
	void(m_pRotLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pRotLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pRotLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pRotLine->SetAutoColon(false);
	//m_pRotLine->SetColonPosition(iColonPos);
	//m_pRotLine->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pRotLine);

	m_pCalc = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCalc->SetId(eCalc);
	m_pCalc->SetText(StringC(AT_DCP06,P_DCP_DOM_CALC_TOK));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pCalc->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pCalc->SetAutoColon(false);
	//m_pCalc->SetColonPosition(iColonPos);
	//m_pCalc->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pCalc);

	//SetHelpTok(H_DCP_DOM_TOK,0);

	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
	DCP06_TRACE_EXIT;
}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Dialog::OnDialogActivated()
{
	DCP06_TRACE_ENTER;
	m_pDataModel->old_active_coodinate_system = GetModel()->active_coodinate_system;
	memcpy(m_pDataModel->matrix,GetModel()->ocsd_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->inv_matrix,GetModel()->ocsd_inv_matrix, sizeof(double) * 16);

	m_pDataModel->align321_active_plane	= GetModel()->align321_active_plane;
	m_pDataModel->align321_active_line	= GetModel()->align321_active_line;
	m_pDataModel->align321_hz_plane		=GetModel()->align321_hz_plane;
	memcpy(&m_pDataModel->align321_plane_buff[0], &GetModel()->align321_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->align321_hz_plane_buff[0], &GetModel()->align321_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->align321_line_buff[0], &GetModel()->align321_line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&m_pDataModel->align321_ovalues_buff, &GetModel()->align321_ovalues_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->align321_ovalues_tool_buff, &GetModel()->align321_ovalues_tool_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->align321_ref_point_buff, &GetModel()->align321_ref_point_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->align321_rot_plane_buff, &GetModel()->align321_rot_plane_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->align321_rot_line_buff, &GetModel()->align321_rot_line_buff, sizeof(S_POINT_BUFF));
	m_pDataModel->ocsd_defined = GetModel()->ocsd_defined;
	
	RefreshControls();
	DCP06_TRACE_EXIT;
}



// Description: refresh all controls
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Dialog::RefreshControls()
{
	DCP06_TRACE_ENTER;
	if( m_pPlane && m_pLine && m_pPointOffs && m_pPointMeas &&	m_pRotPlane && m_pRotLine)
	{
		StringC sTemp;
		StringC sStatus(L"-");
		
		// line
		if(m_pDataModel->align321_line_buff[0].calc) sStatus = L"+";

		if(m_pDataModel->align321_active_line	== X_LINE)
			sTemp = sXLineText;
		else if(m_pDataModel->align321_active_line	== Y_LINE)
			sTemp = sYLineText;
		else
			sTemp = sZLineText;
			
		sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);

		// plane
		sStatus = L"-";
		if(m_pDataModel->align321_hz_plane)
		{
			if(m_pDataModel->align321_hz_plane_buff[0].calc)
				sStatus = L"+";	
		}
		else if(m_pDataModel->align321_plane_buff[0].calc) 
				sStatus = L"+";	

		if(m_pDataModel->align321_active_plane	== XY_PLANE)
			sTemp = sXYPlaneText;
		else if(m_pDataModel->align321_active_plane	== ZX_PLANE)
			sTemp = sZXPlaneText;
		else
			sTemp = sYZPlaneText;
		
		// check hz_plane
		if(m_pDataModel->align321_hz_plane)
		{		
			sTemp += L" (";
			sTemp += sHZText;
			sTemp += L")";
		}

		sTemp += sStatus;
		m_pPlane->GetStringInputCtrl()->SetString(sTemp);

		// offset values
		sStatus = L"-";
		if(m_pDataModel->align321_ovalues_buff.sta) sStatus = L"+";
		m_pPointOffs->GetStringInputCtrl()->SetString(sStatus);

		// reference point
		sStatus = L"-";
		if(m_pDataModel->align321_ref_point_buff.sta) sStatus = L"+";
		m_pPointMeas->GetStringInputCtrl()->SetString(sStatus);

		// rot. plane
		sStatus = L"-";
		if(m_pDataModel->align321_active_plane == XY_PLANE)
		{
			if(m_pDataModel->align321_rot_plane_buff.x != 0.0 || m_pDataModel->align321_rot_plane_buff.y != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->align321_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->align321_rot_plane_buff.x != 0.0 || m_pDataModel->align321_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}

		else if(m_pDataModel->align321_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->align321_rot_plane_buff.y != 0.0 || m_pDataModel->align321_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}
		m_pRotPlane->GetStringInputCtrl()->SetString(sStatus);

		// rot.line
		sStatus = L"-";
		if(m_pDataModel->align321_active_plane == XY_PLANE)
		{
			if(m_pDataModel->align321_rot_line_buff.z != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->align321_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->align321_rot_line_buff.y != 0.0)
				sStatus = L"+";
		}

		else //(GetModel()->align321_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->align321_rot_line_buff.x != 0.0 )
				sStatus = L"+";
		}
		m_pRotLine->GetStringInputCtrl()->SetString(sStatus);
		
		// calculate status
		sStatus = L"-";
		if(GetModel()->ocsd_defined == true  && m_pDataModel->old_active_coodinate_system == OCSD)
			sStatus = L"+";

		m_pCalc->GetStringInputCtrl()->SetString(sStatus);
	}
	DCP06_TRACE_EXIT;
}
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Dialog::UpdateData()
{
	DCP06_TRACE_ENTER;
		memcpy(GetModel()->ocsd_matrix, m_pDataModel->matrix,sizeof(double) * 16);
		memcpy(GetModel()->ocsd_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);
		GetModel()->align321_active_plane	= m_pDataModel->align321_active_plane;
		GetModel()->align321_active_line	= m_pDataModel->align321_active_line; 
		GetModel()->align321_hz_plane		= m_pDataModel->align321_hz_plane;
		memcpy(&GetModel()->align321_plane_buff[0], &m_pDataModel->align321_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetModel()->align321_hz_plane_buff[0], &m_pDataModel->align321_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetModel()->align321_line_buff[0], &m_pDataModel->align321_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&GetModel()->align321_ovalues_buff, &m_pDataModel->align321_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetModel()->align321_ovalues_tool_buff, &m_pDataModel->align321_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetModel()->align321_ref_point_buff, &m_pDataModel->align321_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetModel()->align321_rot_plane_buff, &m_pDataModel->align321_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetModel()->align321_rot_line_buff, &m_pDataModel->align321_rot_line_buff, sizeof(S_POINT_BUFF));
		GetModel()->ocsd_defined = m_pDataModel->ocsd_defined;
		if(m_pDataModel->ocsd_defined == true)
			GetModel()->active_coodinate_system = OCSD;
		//else
		//	GetModel()->active_coodinate_system = DCS;
		GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_A321);
		GetModel()->poConfigController->StoreConfigData();

		GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
		GetModel()->poConfigController->StoreConfigData();
	DCP06_TRACE_EXIT;
}

// Description: only accept DCP06 Model objects
//-------------------------------------------------------------------------------------------------
bool DCP::Alignment321Dialog::SetModel( GUI::ModelC* pModel )
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


// Description: DCP06 model
//-------------------------------------------------------------------------------------------------
DCP::Model* DCP::Alignment321Dialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Dialog::delete_align321()
{
		StringC strDomText;
		strDomText.LoadTxt(AT_DCP06,L_DCP_DOM_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strDomText);
		
		MsgBox msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			m_pDataModel->old_active_coodinate_system = DCS;
			m_pDataModel->ocsd_defined = false;
			m_pDataModel->align321_active_plane = XY_PLANE;
			m_pDataModel->align321_active_line = X_LINE;
			m_pDataModel->align321_hz_plane = false;
			
			memset(&m_pDataModel->align321_plane_buff[0],0,sizeof(S_PLANE_BUFF));
			memset(&m_pDataModel->align321_line_buff[0],0,sizeof(S_LINE_BUFF));	
			memset(&m_pDataModel->align321_ref_point_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->align321_ovalues_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->align321_ovalues_tool_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->align321_rot_line_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->align321_rot_plane_buff,0,sizeof(S_POINT_BUFF));	

			// added 201211
			GetModel()->ocsd_defined = false;
			GetModel()->active_coodinate_system = DCS;

			RefreshControls();
		}
}

// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::Alignment321Controller::Alignment321Controller()
    : m_pDlg( nullptr )
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DOM_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new Alignment321Model;

    // Create a dialog
    m_pDlg = new DCP::Alignment321Dialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( A321_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	vDef.poOwner = this;
	//vDef.nAppId = AT_DCP06;

    vDef.strLable = StringC(AT_DCP06,K_DCP_PLANE_TOK);
	SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
    SetFunctionKey( FK2, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_OFFSET_VALUE_TOK);
    SetFunctionKey( FK3, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_MEASV_TOK);
    SetFunctionKey( FK4, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
    SetFunctionKey( FK5, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
    SetFunctionKey( FK6, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
    SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_ROTATE_PLANE_TOK);
    SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_ROTATE_LINE_TOK);
    SetFunctionKey( SHFK4, vDef );
	
	// HIDE SHIFT F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::Alignment321Controller::~Alignment321Controller()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
//-------------------------------------------------------------------------------------------------
bool DCP::Alignment321Controller::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetModel()->active_coodinate_system = DCS;

	DCP::DefinePlaneModel* pModel = new DCP::DefinePlaneModel();

	pModel->active_plane	= m_pDataModel->align321_active_plane;
	pModel->active_line		= m_pDataModel->align321_active_line;
	pModel->hz_plane		= m_pDataModel->align321_hz_plane;
	memcpy(&pModel->plane_buff[0],&m_pDataModel->align321_plane_buff[0],  sizeof(S_PLANE_BUFF));
	memcpy(&pModel->hz_plane_buff[0],&m_pDataModel->align321_hz_plane_buff[0],  sizeof(S_PLANE_BUFF));
	pModel->display = A321_DLG;
	if(GetController(DEFINE_PLANE_CONTROLLER) == nullptr)
	{
		(void)AddController(DEFINE_PLANE_CONTROLLER, new DCP::DefinePlaneController(m_pDlg->GetModel()) );
	}
	(void)GetController(DEFINE_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_PLANE_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnF2Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetModel()->active_coodinate_system = DCS;

	DCP::DefineLineModel* pModel = new DCP::DefineLineModel();
	pModel->active_plane	= m_pDataModel->align321_active_plane;
	pModel->active_line		= m_pDataModel->align321_active_line;
	//pModel->hz_plane		= m_pDataModel->align321_hz_plane;
	memcpy(&pModel->line_buff[0],&m_pDataModel->align321_line_buff[0],  sizeof(S_LINE_BUFF));
	//memcpy(&pModel->plane_buff[0],&m_pDataModel->align321_hz_plane_buff[0],  sizeof(S_PLANE_BUFF));
	pModel->display = A321_DLG;

	if(GetController(DEFINE_LINE_CONTROLLER) == nullptr)
	{
		(void)AddController( DEFINE_LINE_CONTROLLER, new DCP::DefineLineController(m_pDlg->GetModel()) );
	}
	(void)GetController( DEFINE_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_LINE_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnF3Pressed()
{	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetModel()->active_coodinate_system = DCS;

	DCP::OffsetVModel* pModel = new DCP::OffsetVModel();
	memcpy(&pModel->ovalues_buff,&m_pDataModel->align321_ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDataModel->align321_ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDataModel->align321_ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = A321_DLG;

	if(GetController(OFFSV_CONTROLLER) == nullptr)
	{
		(void)AddController( OFFSV_CONTROLLER, new DCP::OffsetVController(m_pDlg->GetModel(),pModel->display ));
	}
	(void)GetController( OFFSV_CONTROLLER )->SetModel(pModel);
	SetActiveController(OFFSV_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnF4Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	m_pDlg->GetModel()->active_coodinate_system = DCS;

	DCP::OffsetVModel* pModel = new DCP::OffsetVModel();
	memcpy(&pModel->ovalues_buff,&m_pDataModel->align321_ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDataModel->align321_ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDataModel->align321_ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = A321_DLG;

	if(GetController(MEASV_CONTROLLER) == nullptr)
	{
		(void)AddController( MEASV_CONTROLLER, new DCP::MeasVController(m_pDlg->GetModel()) );
	}
	(void)GetController( MEASV_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEASV_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
// CALC
void DCP::Alignment321Controller::OnF5Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	CalcAlignment321 calc_dom(m_pDataModel);
	if(calc_dom.calc())
	{	
		m_pDlg->GetModel()->active_coodinate_system = OCSD;
		m_pDataModel->ocsd_defined = true;
		m_pDlg->GetModel()->ocsd_defined = true;
		// kopioi arvot DCP05Model:iin
		memcpy(m_pDlg->GetModel()->ocsd_matrix, m_pDataModel->matrix,sizeof(double) * 16);
		memcpy(m_pDlg->GetModel()->ocsd_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);
		m_pDlg->GetModel()->align321_active_plane	= m_pDataModel->align321_active_plane;
		m_pDlg->GetModel()->align321_active_line	= m_pDataModel->align321_active_line; 
		m_pDlg->GetModel()->align321_hz_plane		= m_pDataModel->align321_hz_plane;
		memcpy(&m_pDlg->GetModel()->align321_plane_buff[0], &m_pDataModel->align321_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_hz_plane_buff[0], &m_pDataModel->align321_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_line_buff[0], &m_pDataModel->align321_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_ovalues_buff, &m_pDataModel->align321_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_ovalues_tool_buff, &m_pDataModel->align321_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_ref_point_buff, &m_pDataModel->align321_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_rot_plane_buff, &m_pDataModel->align321_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetModel()->align321_rot_line_buff, &m_pDataModel->align321_rot_line_buff, sizeof(S_POINT_BUFF));
		Close(EC_KEY_CONT);
	}
}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnF6Pressed()
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

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnSHF2Pressed()
{	
	m_pDlg->delete_align321();
}
//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnSHF3Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::RotatePlaneModel* pModel = new DCP::RotatePlaneModel();
	
	memcpy(&pModel->point_buff, &m_pDataModel->align321_rot_plane_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->align321_active_plane;
	pModel->display = A321_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_DOM_ROTATE_PLANE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_PLANE_CONTROLLER) == nullptr)
	{
		(void)AddController( ROTATE_PLANE_CONTROLLER, new DCP::RotatePlaneController(m_pDlg->GetModel()));
	}
	(void)GetController( ROTATE_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_PLANE_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnSHF4Pressed()
{
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::RotateLineModel* pModel = new DCP::RotateLineModel();
	
	memcpy(&pModel->point_buff, &m_pDataModel->align321_rot_line_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->align321_active_plane;
	pModel->display = A321_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_DOM_ROTATE_LINE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_LINE_CONTROLLER) == nullptr)
	{
		(void)AddController( ROTATE_LINE_CONTROLLER, new DCP::RotateLineController(m_pDlg->GetModel()));
	}
	(void)GetController( ROTATE_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_LINE_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}


//-------------------------------------------------------------------------------------------------
void DCP::Alignment321Controller::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == DEFINE_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DefinePlaneModel* pModel = (DCP::DefinePlaneModel*) GetController( DEFINE_PLANE_CONTROLLER )->GetModel();		

		// copy values into dommodel
		m_pDataModel->align321_active_plane	= pModel->active_plane;
		m_pDataModel->align321_active_line	= pModel->active_line; 
		m_pDataModel->align321_hz_plane		= pModel->hz_plane;
		memcpy(&m_pDataModel->align321_plane_buff[0], &pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->align321_hz_plane_buff[0], &pModel->hz_plane_buff[0], sizeof(S_PLANE_BUFF));

		if(m_pDataModel->align321_active_plane == XY_PLANE || m_pDataModel->align321_active_plane == ZX_PLANE)
			m_pDataModel->align321_active_line = X_LINE;
		else
			m_pDataModel->align321_active_line = Y_LINE;



	}
	if(lCtrlID == DEFINE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DefineLineModel* pModel = (DCP::DefineLineModel*) GetController( DEFINE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
		m_pDataModel->align321_active_line	= pModel->active_line; 
		memcpy(&m_pDataModel->align321_line_buff[0], &pModel->line_buff[0], sizeof(S_LINE_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetModel()->ocsd_defined = false;
		
	}
	/*
	if(lCtrlID == DEFINE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DefineLineModel* pModel = (DCP::DefineLineModel*) GetController( DEFINE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
		//m_pDataModel->align321_active_plane	= pModel->active_plane;
		m_pDataModel->align321_active_line	= pModel->active_line; 
		//m_pDataModel->align321_hz_plane		= pModel->hz_plane;
		memcpy(&m_pDataModel->align321_line_buff[0], &pModel->line_buff[0], sizeof(S_LINE_BUFF));
	}
	*/
	if(lCtrlID == OFFSV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::OffsetVModel* pModel = (DCP::OffsetVModel*) GetController( OFFSV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->align321_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->align321_ovalues_tool_buff, &pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->align321_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetModel()->ocsd_defined = false;

	}

	if(lCtrlID == MEASV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::OffsetVModel* pModel = (DCP::OffsetVModel*) GetController( MEASV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->align321_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->align321_ovalues_tool_buff,&pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->align321_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetModel()->ocsd_defined = false;

	}

	if(lCtrlID == ROTATE_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::RotatePlaneModel* pModel = (DCP::RotatePlaneModel*) GetController( ROTATE_PLANE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->align321_rot_plane_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));
	}

	if(lCtrlID == ROTATE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::RotatePlaneModel* pModel = (DCP::RotatePlaneModel*) GetController( ROTATE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->align321_rot_line_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));
	}
	//m_pDlg->Show();
	//m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
	m_pDlg->RefreshControls();
	
	//
}



// ===========================================================================================
// DOMModel
// ===========================================================================================

// Instantiate template classes
DCP::Alignment321Model::Alignment321Model()
{
	// copy from DCP05Model
}

/*
// Instantiate template classes
DCP::Alignment321Model::Alignment321Model(Alignment321Model *pModel)
{
	align321_active_plane	= pModel->align321_active_plane;
	align321_active_line	= pModel->align321_active_line; 
	align321_hz_plane		= pModel->align321_hz_plane;
	memcpy(&align321_plane_buff[0], &pModel->align321_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&align321_hz_plane_buff[0], &pModel->align321_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&align321_line_buff[0], &pModel->align321_line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&align321_ovalues_buff, &pModel->align321_ovalues_buff, sizeof(S_POINT_BUFF));
	memcpy(&align321_ovalues_tool_buff, &pModel->align321_ovalues_tool_buff, sizeof(S_POINT_BUFF));
	memcpy(&align321_ref_point_buff, &pModel->align321_ref_point_buff, sizeof(S_POINT_BUFF));
	memcpy(&align321_rot_plane_buff, &pModel->align321_rot_plane_buff, sizeof(S_POINT_BUFF));
	memcpy(&align321_rot_line_buff, &pModel->align321_rot_line_buff, sizeof(S_POINT_BUFF));

} 
*/
DCP::Alignment321Model::~Alignment321Model()
{
}
