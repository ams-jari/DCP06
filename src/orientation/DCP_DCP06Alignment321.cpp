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
#include <dcp06/orientation/DCP_DCP06Alignment321.hpp>
#include <dcp06/orientation/DCP_DefinePlane.hpp>
#include <dcp06/orientation/DCP_DefineLine.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/orientation/DCP_Offsv.hpp>
#include <dcp06/orientation/DCP_MeasV.hpp>
#include <dcp06/orientation/DCP_RotatePlane.hpp>
#include <dcp06/orientation/DCP_RotateLine.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/calculation/DCP_Calc321.hpp>

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
// OBS_IMPLEMENT_EXECUTE(DCP::DCP06UnitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
//-------------------------------------------------------------------------------------------------
DCP::DCP06DomDlgC::DCP06DomDlgC(DCP::DCP06DomModelC* pDomModel):m_pPlane(0),m_pLine(0),m_pPointOffs(0),m_pPointMeas(0),
								m_pRotPlane(0),m_pCalc(0),m_pRotLine(0),m_pDataModel(pDomModel)
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
DCP::DCP06DomDlgC::~DCP06DomDlgC()
{

}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomDlgC::OnInitDialog(void)
{
	
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
	
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomDlgC::OnDialogActivated()
{
	m_pDataModel->old_active_coodinate_system = GetDCP06Model()->active_coodinate_system;
	memcpy(m_pDataModel->matrix,GetDCP06Model()->ocsd_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->inv_matrix,GetDCP06Model()->ocsd_inv_matrix, sizeof(double) * 16);

	m_pDataModel->dom_active_plane	= GetDCP06Model()->dom_active_plane;
	m_pDataModel->dom_active_line	= GetDCP06Model()->dom_active_line; 
	m_pDataModel->dom_hz_plane		=GetDCP06Model()->dom_hz_plane;
	memcpy(&m_pDataModel->dom_plane_buff[0], &GetDCP06Model()->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->dom_hz_plane_buff[0], &GetDCP06Model()->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->dom_line_buff[0], &GetDCP06Model()->dom_line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&m_pDataModel->dom_ovalues_buff, &GetDCP06Model()->dom_ovalues_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->dom_ovalues_tool_buff, &GetDCP06Model()->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->dom_ref_point_buff, &GetDCP06Model()->dom_ref_point_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->dom_rot_plane_buff, &GetDCP06Model()->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->dom_rot_line_buff, &GetDCP06Model()->dom_rot_line_buff, sizeof(S_POINT_BUFF));
	m_pDataModel->ocsd_defined = GetDCP06Model()->ocsd_defined;
	
	RefreshControls();
}



// Description: refresh all controls
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomDlgC::RefreshControls()
{	
	if( m_pPlane && m_pLine && m_pPointOffs && m_pPointMeas &&	m_pRotPlane && m_pRotLine)
	{
		StringC sTemp;
		StringC sStatus(L"-");
		
		// line
		if(m_pDataModel->dom_line_buff[0].calc) sStatus = L"+";

		if(m_pDataModel->dom_active_line	== X_LINE)
			sTemp = sXLineText;
		else if(m_pDataModel->dom_active_line	== Y_LINE)
			sTemp = sYLineText;
		else
			sTemp = sZLineText;
			
		sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);

		// plane
		sStatus = L"-";
		if(m_pDataModel->dom_hz_plane)
		{
			if(m_pDataModel->dom_hz_plane_buff[0].calc)
				sStatus = L"+";	
		}
		else if(m_pDataModel->dom_plane_buff[0].calc) 
				sStatus = L"+";	

		if(m_pDataModel->dom_active_plane	== XY_PLANE)
			sTemp = sXYPlaneText;
		else if(m_pDataModel->dom_active_plane	== ZX_PLANE)
			sTemp = sZXPlaneText;
		else
			sTemp = sYZPlaneText;
		
		// check hz_plane
		if(m_pDataModel->dom_hz_plane)
		{		
			sTemp += L" (";
			sTemp += sHZText;
			sTemp += L")";
		}

		sTemp += sStatus;
		m_pPlane->GetStringInputCtrl()->SetString(sTemp);

		// offset values
		sStatus = L"-";
		if(m_pDataModel->dom_ovalues_buff.sta) sStatus = L"+";
		m_pPointOffs->GetStringInputCtrl()->SetString(sStatus);

		// reference point
		sStatus = L"-";
		if(m_pDataModel->dom_ref_point_buff.sta) sStatus = L"+";
		m_pPointMeas->GetStringInputCtrl()->SetString(sStatus);

		// rot. plane
		sStatus = L"-";
		if(m_pDataModel->dom_active_plane == XY_PLANE)
		{
			if(m_pDataModel->dom_rot_plane_buff.x != 0.0 || m_pDataModel->dom_rot_plane_buff.y != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->dom_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->dom_rot_plane_buff.x != 0.0 || m_pDataModel->dom_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}

		else if(m_pDataModel->dom_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->dom_rot_plane_buff.y != 0.0 || m_pDataModel->dom_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}
		m_pRotPlane->GetStringInputCtrl()->SetString(sStatus);

		// rot.line
		sStatus = L"-";
		if(m_pDataModel->dom_active_plane == XY_PLANE)
		{
			if(m_pDataModel->dom_rot_line_buff.z != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->dom_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->dom_rot_line_buff.y != 0.0)
				sStatus = L"+";
		}

		else //(GetDCP06Model()->dom_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->dom_rot_line_buff.x != 0.0 )
				sStatus = L"+";
		}
		m_pRotLine->GetStringInputCtrl()->SetString(sStatus);
		
		// calculate status
		sStatus = L"-";
		if(GetDCP06Model()->ocsd_defined == true  && m_pDataModel->old_active_coodinate_system == OCSD)
			sStatus = L"+";

		m_pCalc->GetStringInputCtrl()->SetString(sStatus);
	}
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomDlgC::UpdateData()
{
		memcpy(GetDCP06Model()->ocsd_matrix, m_pDataModel->matrix,sizeof(double) * 16);
		memcpy(GetDCP06Model()->ocsd_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);
		GetDCP06Model()->dom_active_plane	= m_pDataModel->dom_active_plane;
		GetDCP06Model()->dom_active_line	= m_pDataModel->dom_active_line; 
		GetDCP06Model()->dom_hz_plane		= m_pDataModel->dom_hz_plane;
		memcpy(&GetDCP06Model()->dom_plane_buff[0], &m_pDataModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetDCP06Model()->dom_hz_plane_buff[0], &m_pDataModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetDCP06Model()->dom_line_buff[0], &m_pDataModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&GetDCP06Model()->dom_ovalues_buff, &m_pDataModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->dom_ovalues_tool_buff, &m_pDataModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->dom_ref_point_buff, &m_pDataModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->dom_rot_plane_buff, &m_pDataModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->dom_rot_line_buff, &m_pDataModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
		GetDCP06Model()->ocsd_defined = m_pDataModel->ocsd_defined;
		if(m_pDataModel->ocsd_defined == true)
			GetDCP06Model()->active_coodinate_system = OCSD;
		//else
		//	GetDCP06Model()->active_coodinate_system = DCS;
		GetDCP06Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_A321);
		GetDCP06Model()->poConfigController->StoreConfigData();

		GetDCP06Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
		GetDCP06Model()->poConfigController->StoreConfigData();
}

// Description: only accept hello world Model objects
//-------------------------------------------------------------------------------------------------
bool DCP::DCP06DomDlgC::SetModel( GUI::ModelC* pModel )
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
//-------------------------------------------------------------------------------------------------
DCP::DCP06ModelC* DCP::DCP06DomDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomDlgC::delete_dom()
{
		StringC strDomText;
		strDomText.LoadTxt(AT_DCP06,L_DCP_DOM_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strDomText);
		
		DCP06MsgBoxC msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			m_pDataModel->old_active_coodinate_system = DCS;
			m_pDataModel->ocsd_defined = false;
			m_pDataModel->dom_active_plane = XY_PLANE;
			m_pDataModel->dom_active_line = X_LINE;
			m_pDataModel->dom_hz_plane = false;
			
			memset(&m_pDataModel->dom_plane_buff[0],0,sizeof(S_PLANE_BUFF));
			memset(&m_pDataModel->dom_line_buff[0],0,sizeof(S_LINE_BUFF));	
			memset(&m_pDataModel->dom_ref_point_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->dom_ovalues_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->dom_ovalues_tool_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->dom_rot_line_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->dom_rot_plane_buff,0,sizeof(S_POINT_BUFF));	

			// added 201211
			GetDCP06Model()->ocsd_defined = false;
			GetDCP06Model()->active_coodinate_system = DCS;

			RefreshControls();
		}
}

// ================================================================================================
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06DomControllerC::DCP06DomControllerC()
    : m_pDlg( NULL )
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DOM_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new DCP06DomModelC;

    // Create a dialog
    m_pDlg = new DCP::DCP06DomDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( DOM_DLG, m_pDlg, true );
	
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

DCP::DCP06DomControllerC::~DCP06DomControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
//-------------------------------------------------------------------------------------------------
bool DCP::DCP06DomControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnF1Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06DefinePlaneModelC* pModel = new DCP::DCP06DefinePlaneModelC();

	pModel->active_plane	= m_pDataModel->dom_active_plane;
	pModel->active_line		= m_pDataModel->dom_active_line;
	pModel->hz_plane		= m_pDataModel->dom_hz_plane;
	memcpy(&pModel->plane_buff[0],&m_pDataModel->dom_plane_buff[0],  sizeof(S_PLANE_BUFF));
	memcpy(&pModel->hz_plane_buff[0],&m_pDataModel->dom_hz_plane_buff[0],  sizeof(S_PLANE_BUFF));
	pModel->display = DOM_DLG;
	if(GetController(DEFINE_PLANE_CONTROLLER) == NULL)
	{
		(void)AddController(DEFINE_PLANE_CONTROLLER, new DCP::DCP06DefinePlaneControllerC(m_pDlg->GetDCP06Model()) );
	}
	(void)GetController(DEFINE_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_PLANE_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnF2Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06DefineLineModelC* pModel = new DCP::DCP06DefineLineModelC();
	pModel->active_plane	= m_pDataModel->dom_active_plane;
	pModel->active_line		= m_pDataModel->dom_active_line;
	//pModel->hz_plane		= m_pDataModel->dom_hz_plane;
	memcpy(&pModel->line_buff[0],&m_pDataModel->dom_line_buff[0],  sizeof(S_LINE_BUFF));
	//memcpy(&pModel->plane_buff[0],&m_pDataModel->dom_hz_plane_buff[0],  sizeof(S_PLANE_BUFF));
	pModel->display = DOM_DLG;

	if(GetController(DEFINE_LINE_CONTROLLER) == NULL)
	{
		(void)AddController( DEFINE_LINE_CONTROLLER, new DCP::DCP06DefineLineControllerC(m_pDlg->GetDCP06Model()) );
	}
	(void)GetController( DEFINE_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_LINE_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnF3Pressed()
{	
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06OffsVModelC* pModel = new DCP::DCP06OffsVModelC();
	memcpy(&pModel->ovalues_buff,&m_pDataModel->dom_ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDataModel->dom_ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDataModel->dom_ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = DOM_DLG;

	if(GetController(OFFSV_CONTROLLER) == NULL)
	{
		(void)AddController( OFFSV_CONTROLLER, new DCP::DCP06OffsvControllerC(m_pDlg->GetDCP06Model(),pModel->display ));
	}
	(void)GetController( OFFSV_CONTROLLER )->SetModel(pModel);
	SetActiveController(OFFSV_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnF4Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06OffsVModelC* pModel = new DCP::DCP06OffsVModelC();
	memcpy(&pModel->ovalues_buff,&m_pDataModel->dom_ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDataModel->dom_ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDataModel->dom_ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = DOM_DLG;

	if(GetController(MEASV_CONTROLLER) == NULL)
	{
		(void)AddController( MEASV_CONTROLLER, new DCP::DCP06MeasVControllerC(m_pDlg->GetDCP06Model()) );
	}
	(void)GetController( MEASV_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEASV_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
// CALC
void DCP::DCP06DomControllerC::OnF5Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP06CalcDomC calc_dom(m_pDataModel);
	if(calc_dom.calc())
	{	
		m_pDlg->GetDCP06Model()->active_coodinate_system = OCSD;
		m_pDataModel->ocsd_defined = true;
		m_pDlg->GetDCP06Model()->ocsd_defined = true;
		// kopioi arvot DCP05Model:iin
		memcpy(m_pDlg->GetDCP06Model()->ocsd_matrix, m_pDataModel->matrix,sizeof(double) * 16);
		memcpy(m_pDlg->GetDCP06Model()->ocsd_inv_matrix, m_pDataModel->inv_matrix, sizeof(double) * 16);
		m_pDlg->GetDCP06Model()->dom_active_plane	= m_pDataModel->dom_active_plane;
		m_pDlg->GetDCP06Model()->dom_active_line	= m_pDataModel->dom_active_line; 
		m_pDlg->GetDCP06Model()->dom_hz_plane		= m_pDataModel->dom_hz_plane;
		memcpy(&m_pDlg->GetDCP06Model()->dom_plane_buff[0], &m_pDataModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_hz_plane_buff[0], &m_pDataModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_line_buff[0], &m_pDataModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_ovalues_buff, &m_pDataModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_ovalues_tool_buff, &m_pDataModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_ref_point_buff, &m_pDataModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_rot_plane_buff, &m_pDataModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->dom_rot_line_buff, &m_pDataModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
		Close(EC_KEY_CONT);
	}
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnF6Pressed()
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

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnSHF2Pressed()
{	
	m_pDlg->delete_dom();
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnSHF3Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::DCP06RotatePlaneModelC* pModel = new DCP::DCP06RotatePlaneModelC();
	
	memcpy(&pModel->point_buff, &m_pDataModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->dom_active_plane;
	pModel->display = DOM_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_DOM_ROTATE_PLANE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_PLANE_CONTROLLER) == NULL)
	{
		(void)AddController( ROTATE_PLANE_CONTROLLER, new DCP::DCP06RotatePlaneControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController( ROTATE_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_PLANE_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnSHF4Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP06RotateLineModelC* pModel = new DCP::DCP06RotateLineModelC();
	
	memcpy(&pModel->point_buff, &m_pDataModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->dom_active_plane;
	pModel->display = DOM_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP06,T_DCP_DOM_ROTATE_LINE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_LINE_CONTROLLER) == NULL)
	{
		(void)AddController( ROTATE_LINE_CONTROLLER, new DCP::DCP06RotateLineControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController( ROTATE_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_LINE_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}


//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == DEFINE_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DCP06DefinePlaneModelC* pModel = (DCP::DCP06DefinePlaneModelC*) GetController( DEFINE_PLANE_CONTROLLER )->GetModel();		

		// copy values into dommodel
		m_pDataModel->dom_active_plane	= pModel->active_plane;
		m_pDataModel->dom_active_line	= pModel->active_line; 
		m_pDataModel->dom_hz_plane		= pModel->hz_plane;
		memcpy(&m_pDataModel->dom_plane_buff[0], &pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->dom_hz_plane_buff[0], &pModel->hz_plane_buff[0], sizeof(S_PLANE_BUFF));

		if(m_pDataModel->dom_active_plane == XY_PLANE || m_pDataModel->dom_active_plane == ZX_PLANE)
			m_pDataModel->dom_active_line = X_LINE;
		else
			m_pDataModel->dom_active_line = Y_LINE;



	}
	if(lCtrlID == DEFINE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06DefineLineModelC* pModel = (DCP::DCP06DefineLineModelC*) GetController( DEFINE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
		m_pDataModel->dom_active_line	= pModel->active_line; 
		memcpy(&m_pDataModel->dom_line_buff[0], &pModel->line_buff[0], sizeof(S_LINE_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsd_defined = false;
		
	}
	/*
	if(lCtrlID == DEFINE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06DefineLineModelC* pModel = (DCP::DCP06DefineLineModelC*) GetController( DEFINE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
		//m_pDataModel->dom_active_plane	= pModel->active_plane;
		m_pDataModel->dom_active_line	= pModel->active_line; 
		//m_pDataModel->dom_hz_plane		= pModel->hz_plane;
		memcpy(&m_pDataModel->dom_line_buff[0], &pModel->line_buff[0], sizeof(S_LINE_BUFF));
	}
	*/
	if(lCtrlID == OFFSV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06OffsVModelC* pModel = (DCP::DCP06OffsVModelC*) GetController( OFFSV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->dom_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->dom_ovalues_tool_buff, &pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->dom_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsd_defined = false;

	}

	if(lCtrlID == MEASV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06OffsVModelC* pModel = (DCP::DCP06OffsVModelC*) GetController( MEASV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->dom_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->dom_ovalues_tool_buff,&pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->dom_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsd_defined = false;

	}

	if(lCtrlID == ROTATE_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06RotatePlaneModelC* pModel = (DCP::DCP06RotatePlaneModelC*) GetController( ROTATE_PLANE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->dom_rot_plane_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));
	}

	if(lCtrlID == ROTATE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06RotatePlaneModelC* pModel = (DCP::DCP06RotatePlaneModelC*) GetController( ROTATE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->dom_rot_line_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));
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
DCP::DCP06DomModelC::DCP06DomModelC()
{
	// copy from DCP05Model
}

/*
// Instantiate template classes
DCP::DCP06DomModelC::DCP06DomModelC(DCP06DomModelC *pModel)
{
	dom_active_plane	= pModel->dom_active_plane;
	dom_active_line	= pModel->dom_active_line; 
	dom_hz_plane		= pModel->dom_hz_plane;
	memcpy(&dom_plane_buff[0], &pModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&dom_hz_plane_buff[0], &pModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&dom_line_buff[0], &pModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&dom_ovalues_buff, &pModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
	memcpy(&dom_ovalues_tool_buff, &pModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
	memcpy(&dom_ref_point_buff, &pModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
	memcpy(&dom_rot_plane_buff, &pModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
	memcpy(&dom_rot_line_buff, &pModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));

} 
*/
DCP::DCP06DomModelC::~DCP06DomModelC()
{
}
