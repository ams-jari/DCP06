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
#include <dcp06/orientation/Alignment321UserDef.hpp>
#include <dcp06/orientation/DefinePlaneUserDef.hpp>
#include <dcp06/orientation/DefineLineUserDef.hpp>
#include <dcp06/orientation/DefinePlane.hpp>
#include <dcp06/orientation/DefineLine.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/orientation/Offsv.hpp>
#include <dcp06/orientation/MeasV.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/orientation/RotatePlane.hpp>
#include <dcp06/orientation/RotateLine.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/calculation/Calculation321.hpp>
#include <dcp06/core/SelectPoint.hpp>

#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/core/Common.hpp>

#include <dcp06/core/UserDefStatus.hpp>

#include <GUI_Desktop.hpp>
#include <GUI_Application.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
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
DCP::DCP06DomUserDefDlgC::DCP06DomUserDefDlgC(DCP::DCP06DomUserDefModelC* pDomUserDefModel):m_pPlane(0),m_pLine(0),m_pPointOffs(0),m_pPointMeas(0),
								m_pRotPlane(0),m_pCalc(0),m_pRotLine(0),m_pDataModel(pDomUserDefModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP05);

	sXYPlaneText.LoadTxt(AT_DCP05,V_DCP_XY_PLANE_TOK);
	sZXPlaneText.LoadTxt(AT_DCP05,V_DCP_ZX_PLANE_TOK);
	sYZPlaneText.LoadTxt(AT_DCP05,V_DCP_YZ_PLANE_TOK);

	sHZText.LoadTxt(AT_DCP05,V_DCP_HZ_TOK);

	sXLineText.LoadTxt(AT_DCP05,V_DCP_X_LINE_TOK);
	sYLineText.LoadTxt(AT_DCP05,V_DCP_Y_LINE_TOK);
	sZLineText.LoadTxt(AT_DCP05,V_DCP_Z_LINE_TOK);

	
}


// Description: Destructor
//-------------------------------------------------------------------------------------------------
DCP::DCP06DomUserDefDlgC::~DCP06DomUserDefDlgC()
{

}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefDlgC::OnInitDialog(void)
{
	
	GUI::BaseDialogC::OnInitDialog();
	short iColonPos = 9 * 22;
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );

	m_pPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPlane->SetId(ePlane);
	m_pPlane->SetText(StringC(AT_DCP05,P_DCP_REF_PLANE_SK_TOK));
	//m_pPlane->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPlane->SetAutoColon(false);
	//m_pPlane->SetColonPosition(iColonPos);
	//m_pPlane->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pPlane);

	m_pLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLine->SetId(eLine);
	m_pLine->SetText(StringC(AT_DCP05,P_DCP_REF_LINE_SK_TOK));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pLine->SetAutoColon(false);
	//m_pLine->SetColonPosition(iColonPos);
	//m_pLine->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pLine);
	
	m_pPointOffs = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointOffs->SetId(ePointOffs);
	m_pPointOffs->SetText(StringC(AT_DCP05,P_DCP_REF_PNT_OFFSV_TOK));
	void(m_pPointOffs->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointOffs->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pPointOffs->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pPointOffs->SetAutoColon(false);
	//m_pPointOffs->SetColonPosition(iColonPos);
	// m_pPointOffs->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pPointOffs);

	m_pPointMeas = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointMeas->SetId(ePointMeas);
	m_pPointMeas->SetText(StringC(AT_DCP05,P_DCP_REF_PNT_MEASV_TOK));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPointMeas->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pPointMeas->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pPointMeas->SetAutoColon(false);
	//m_pPointMeas->SetColonPosition(iColonPos);
	//m_pPointMeas->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pPointMeas);

	m_pRotPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRotPlane->SetId(eRotPlane);
	m_pRotPlane->SetText(StringC(AT_DCP05,P_DCP_REF_ROTATE_PLANE_TOK));
	void(m_pRotPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pRotPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pRotPlane->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pRotPlane->SetAutoColon(false);
	//m_pRotPlane->SetColonPosition(iColonPos);
	//m_pRotPlane->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pRotPlane);
	
	m_pRotLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRotLine->SetId(eRotLine);
	m_pRotLine->SetText(StringC(AT_DCP05,P_DCP_REF_ROTATE_LINE_TOK));
	void(m_pRotLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pRotLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pRotLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pRotLine->SetAutoColon(false);
	//m_pRotLine->SetColonPosition(iColonPos);
	//m_pRotLine->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pRotLine);

	m_pCalc = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCalc->SetId(eCalc);
	m_pCalc->SetText(StringC(AT_DCP05,P_DCP_DOM_CALC_TOK));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCalc->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pCalc->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//m_pCalc->SetAutoColon(false);
	//m_pCalc->SetColonPosition(iColonPos);
	//m_pCalc->SetTextWidth(9 * 50); CAPTIVATE
	AddCtrl(m_pCalc);

	//SetHelpTok(H_DCP_USERDEF_CONF_TOK,0);

	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
	
}

void DCP::DCP06DomUserDefDlgC::load_data_from_dcp05model()
{
	m_pDataModel->domModel->old_active_coodinate_system = GetDCP06Model()->active_coodinate_system;
	memcpy(m_pDataModel->domModel->matrix,GetDCP06Model()->ocsu_matrix, sizeof(double) * 16);
	memcpy(m_pDataModel->domModel->inv_matrix,GetDCP06Model()->ocsu_inv_matrix, sizeof(double) * 16);

	m_pDataModel->domModel->dom_active_plane	= GetDCP06Model()->userdef_active_plane;
	m_pDataModel->domModel->dom_active_line	= GetDCP06Model()->userdef_active_line; 
	m_pDataModel->domModel->dom_hz_plane		=GetDCP06Model()->userdef_hz_plane;
	memcpy(&m_pDataModel->domModel->dom_plane_buff[0], &GetDCP06Model()->userdef_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->domModel->dom_hz_plane_buff[0], &GetDCP06Model()->userdef_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&m_pDataModel->domModel->dom_line_buff[0], &GetDCP06Model()->userdef_line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&m_pDataModel->domModel->dom_ovalues_buff, &GetDCP06Model()->userdef_ovalues_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->domModel->dom_ovalues_tool_buff, &GetDCP06Model()->userdef_ovalues_tool_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->domModel->dom_ref_point_buff, &GetDCP06Model()->userdef_ref_point_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->domModel->dom_rot_plane_buff, &GetDCP06Model()->userdef_rot_plane_buff, sizeof(S_POINT_BUFF));
	memcpy(&m_pDataModel->domModel->dom_rot_line_buff, &GetDCP06Model()->userdef_rot_line_buff, sizeof(S_POINT_BUFF));
	m_pDataModel->domModel->ocsd_defined = GetDCP06Model()->ocsu_defined;
	
	memcpy(&m_pDataModel->userdef_measured_points, &GetDCP06Model()->userdef_measured_points, sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
	memcpy(&m_pDataModel->userdef_plane_points_no, &GetDCP06Model()->userdef_plane_points_no, sizeof(short) * MAX_USERDEF_POINTS);
	memcpy(&m_pDataModel->userdef_line_points_no, &GetDCP06Model()->userdef_line_points_no, sizeof(short) * MAX_USERDEF_POINTS);
	m_pDataModel->userdef_point_no = GetDCP06Model()->userdef_point_no;
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefDlgC::OnDialogActivated()
{
	
	
	RefreshControls();
}



// Description: refresh all controls
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefDlgC::RefreshControls()
{	
	if( m_pPlane && m_pLine && m_pPointOffs && m_pPointMeas &&	m_pRotPlane && m_pRotLine)
	{
		StringC sTemp;
		StringC sStatus(L"-");
		int i = 0;

		int linePoints = 0;
		for(i = 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(m_pDataModel->userdef_line_points_no[i] > 0)
			{
				if(m_pDataModel->userdef_measured_points[m_pDataModel->userdef_line_points_no[i] -1].sta > 0)
					linePoints++;
			}
		}

		int planePoints = 0;
		for(i = 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(m_pDataModel->userdef_plane_points_no[i] > 0)
			{
				if(m_pDataModel->userdef_measured_points[m_pDataModel->userdef_plane_points_no[i] -1].sta > 0)
					planePoints++;
			}
		}

		// line
		//if(m_pDataModel->domModel->dom_line_buff[0].calc) sStatus = L"+";
		if(linePoints >= 2) sStatus = L"+";

		if(m_pDataModel->domModel->dom_active_line	== X_LINE)
			sTemp = sXLineText;
		else if(m_pDataModel->domModel->dom_active_line	== Y_LINE)
			sTemp = sYLineText;
		else
			sTemp = sZLineText;
			
		sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);

		// plane
		sStatus = L"-";
		if(m_pDataModel->domModel->dom_hz_plane)
		{
			//if(m_pDataModel->domModel->dom_hz_plane_buff[0].calc)
				sStatus = L"+";	
		}
		else 
		{
			//if(m_pDataModel->domModel->dom_plane_buff[0].calc) 
			if(planePoints >= 3)
				sStatus = L"+";
		}

		if(m_pDataModel->domModel->dom_active_plane	== XY_PLANE)
			sTemp = sXYPlaneText;
		else if(m_pDataModel->domModel->dom_active_plane	== ZX_PLANE)
			sTemp = sZXPlaneText;
		else
			sTemp = sYZPlaneText;
		
		// check hz_plane
		if(m_pDataModel->domModel->dom_hz_plane)
		{		
			sTemp += L" (";
			sTemp += sHZText;
			sTemp += L")";
		}

		sTemp += sStatus;
		m_pPlane->GetStringInputCtrl()->SetString(sTemp);

		// offset values
		sStatus = L"-";
		if(m_pDataModel->domModel->dom_ovalues_buff.sta) sStatus = L"+";
		m_pPointOffs->GetStringInputCtrl()->SetString(sStatus);

		// reference point
		sStatus = L"-";
		
		//if(m_pDataModel->domModel->dom_ref_point_buff.sta) sStatus = L"+";
		if(m_pDataModel->userdef_point_no > 0)
		{
			if(m_pDataModel->userdef_measured_points[m_pDataModel->userdef_point_no-1].sta > 0)
				sStatus = L"+";
		}
		m_pPointMeas->GetStringInputCtrl()->SetString(sStatus);

		// rot. plane
		sStatus = L"-";
		if(m_pDataModel->domModel->dom_active_plane == XY_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_plane_buff.x != 0.0 || m_pDataModel->domModel->dom_rot_plane_buff.z != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->domModel->dom_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_plane_buff.x != 0.0 || m_pDataModel->domModel->dom_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}

		else if(m_pDataModel->domModel->dom_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_plane_buff.y != 0.0 || m_pDataModel->domModel->dom_rot_plane_buff.z != 0.0)
				sStatus = L"+";
		}
		m_pRotPlane->GetStringInputCtrl()->SetString(sStatus);

		// rot.line
		sStatus = L"-";
		if(m_pDataModel->domModel->dom_active_plane == XY_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_line_buff.z != 0.0)
				sStatus = L"+";
			
		}
		else if(m_pDataModel->domModel->dom_active_plane == ZX_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_line_buff.y != 0.0)
				sStatus = L"+";
		}

		else //(GetDCP06Model()->dom_active_plane == YZ_PLANE)
		{
			if(m_pDataModel->domModel->dom_rot_line_buff.x != 0.0 )
				sStatus = L"+";
		}
		m_pRotLine->GetStringInputCtrl()->SetString(sStatus);
		
		// calculate status
		sStatus = L"-";
		if(GetDCP06Model()->ocsu_defined == true  && m_pDataModel->domModel->old_active_coodinate_system == OCSU)
			sStatus = L"+";

		m_pCalc->GetStringInputCtrl()->SetString(sStatus);
	}
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefDlgC::UpdateData()
{
		memcpy(GetDCP06Model()->ocsu_matrix, m_pDataModel->domModel->matrix,sizeof(double) * 16);
		memcpy(GetDCP06Model()->ocsu_inv_matrix, m_pDataModel->domModel->inv_matrix, sizeof(double) * 16);
		GetDCP06Model()->userdef_active_plane	= m_pDataModel->domModel->dom_active_plane;
		GetDCP06Model()->userdef_active_line	= m_pDataModel->domModel->dom_active_line; 
		GetDCP06Model()->userdef_hz_plane		= m_pDataModel->domModel->dom_hz_plane;
		memcpy(&GetDCP06Model()->userdef_plane_buff[0], &m_pDataModel->domModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetDCP06Model()->userdef_hz_plane_buff[0], &m_pDataModel->domModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&GetDCP06Model()->userdef_line_buff[0], &m_pDataModel->domModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&GetDCP06Model()->userdef_ovalues_buff, &m_pDataModel->domModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->userdef_ovalues_tool_buff, &m_pDataModel->domModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->userdef_ref_point_buff, &m_pDataModel->domModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->userdef_rot_plane_buff, &m_pDataModel->domModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&GetDCP06Model()->userdef_rot_line_buff, &m_pDataModel->domModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
		GetDCP06Model()->ocsu_defined = m_pDataModel->domModel->ocsd_defined;
		

		memcpy(&GetDCP06Model()->userdef_measured_points,&m_pDataModel->userdef_measured_points,  sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
		memcpy(&GetDCP06Model()->userdef_plane_points_no,&m_pDataModel->userdef_plane_points_no, sizeof(short) * MAX_USERDEF_POINTS);
		memcpy(&GetDCP06Model()->userdef_line_points_no,&m_pDataModel->userdef_line_points_no,  sizeof(short) * MAX_USERDEF_POINTS);
		GetDCP06Model()->userdef_point_no = m_pDataModel->userdef_point_no; 
		
		if(m_pDataModel->domModel->ocsd_defined == true)
			GetDCP06Model()->active_coodinate_system = OCSU;
		//else
		//	GetDCP06Model()->active_coodinate_system = DCS;

		// TODO CNF_KEY_USERDEF
		GetDCP06Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_USERDEF);
		GetDCP06Model()->poConfigController->StoreConfigData();

		GetDCP06Model()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_INIT);
		GetDCP06Model()->poConfigController->StoreConfigData();
}

// Description: only accept hello world Model objects
//-------------------------------------------------------------------------------------------------
bool DCP::DCP06DomUserDefDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
		load_data_from_dcp05model();
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}


// Description: Hello World model
//-------------------------------------------------------------------------------------------------
DCP::DCP06ModelC* DCP::DCP06DomUserDefDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefDlgC::delete_dom()
{
		StringC strDomText;
		strDomText.LoadTxt(AT_DCP05,L_DCP_USERDEF_TEXT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP05,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strDomText);
		
		DCP06MsgBoxC msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			m_pDataModel->domModel->old_active_coodinate_system = DCS;
			m_pDataModel->domModel->ocsd_defined = false;
			m_pDataModel->domModel->dom_active_plane = XY_PLANE;
			m_pDataModel->domModel->dom_active_line = X_LINE;
			m_pDataModel->domModel->dom_hz_plane = false;
			
			memset(&m_pDataModel->domModel->dom_plane_buff[0],0,sizeof(S_PLANE_BUFF));
			memset(&m_pDataModel->domModel->dom_line_buff[0],0,sizeof(S_LINE_BUFF));	
			memset(&m_pDataModel->domModel->dom_ref_point_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->domModel->dom_ovalues_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->domModel->dom_ovalues_tool_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->domModel->dom_rot_line_buff,0,sizeof(S_POINT_BUFF));	
			memset(&m_pDataModel->domModel->dom_rot_plane_buff,0,sizeof(S_POINT_BUFF));	

			memset(&m_pDataModel->userdef_measured_points[0], 0, sizeof(S_POINT_BUFF) *MAX_USERDEF_POINTS);

			memset(&m_pDataModel->userdef_plane_points_no[0], 0, sizeof(short)* MAX_USERDEF_POINTS);
			memset(&m_pDataModel->userdef_line_points_no[0], 0, sizeof(short)* MAX_USERDEF_POINTS); 
			m_pDataModel->userdef_point_no = 0;
			RefreshControls();
		}
}

// ================================================================================================
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06DomUserDefControllerC::DCP06DomUserDefControllerC(DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_USERDEF_CONFIG_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new DCP06DomUserDefModelC(m_pDCP06Model);

    // Create a dialog
    m_pDlg = new DCP::DCP06DomUserDefDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( DOM_USERDEF_DLG, m_pDlg, true );
	
    // Set the function key

	show_function_keys();

} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06DomUserDefControllerC::~DCP06DomUserDefControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

void DCP::DCP06DomUserDefControllerC::show_function_keys()
{
    FKDef vDef;
	vDef.poOwner = this;
	//vDef.nAppId = AT_DCP05;

    vDef.strLable = StringC(AT_DCP05,K_DCP_PLANE_TOK);
	SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_LINE_TOK);
    SetFunctionKey( FK2, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_OFFSET_VALUE_TOK);
    SetFunctionKey( FK3, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_MEASV_TOK);
    SetFunctionKey( FK4, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_CALC_TOK);
    SetFunctionKey( FK5, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
    SetFunctionKey( FK6, vDef );

    vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
    SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_ROTATE_PLANE_TOK);
    SetFunctionKey( SHFK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_ROTATE_LINE_TOK);
    SetFunctionKey( SHFK4, vDef );
	
	vDef.strLable = StringC(AT_DCP05,K_DCP_MEAS_TOK);
    SetFunctionKey( SHFK5, vDef );

	// HIDE SHIFT F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP05;
	vDef1.strLable =L"";
	SetFunctionKey( SHFK6, vDef1 );
}

// Description: Route model to everybody else
//-------------------------------------------------------------------------------------------------
bool DCP::DCP06DomUserDefControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnF1Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06DefinePlaneUserDefModelC* pModel = new DCP::DCP06DefinePlaneUserDefModelC();

	pModel->planeModel->active_plane	= m_pDataModel->domModel->dom_active_plane;
	pModel->planeModel->active_line		= m_pDataModel->domModel->dom_active_line;
	pModel->planeModel->hz_plane		= m_pDataModel->domModel->dom_hz_plane;
	memcpy(&pModel->planeModel->plane_buff[0],&m_pDataModel->domModel->dom_plane_buff[0],  sizeof(S_PLANE_BUFF));
	memcpy(&pModel->planeModel->hz_plane_buff[0],&m_pDataModel->domModel->dom_hz_plane_buff[0],  sizeof(S_PLANE_BUFF));
	pModel->planeModel->display = DOM_USERDEF_DLG;

	memcpy(&pModel->userdef_plane_points_no[0],&m_pDataModel->userdef_plane_points_no[0], sizeof(short) * MAX_USERDEF_POINTS);
	memcpy(&pModel->userdef_measured_points[0],&m_pDataModel->userdef_measured_points[0], sizeof(S_POINT_BUFF) *MAX_USERDEF_POINTS);

	m_pDataModel->build_select_point_list();
	memcpy(&pModel->select_point_list[0], &m_pDataModel->select_point_list[0], sizeof(S_SELECT_POINTS) * MAX_USERDEF_POINTS);


	if(GetController(DEFINE_PLANE_USERDEF_CONTROLLER) == NULL)
	{
		(void)AddController(DEFINE_PLANE_USERDEF_CONTROLLER, new DCP::DCP06DefinePlaneUserDefControllerC(m_pDlg->GetDCP06Model()) );
	}
	(void)GetController(DEFINE_PLANE_USERDEF_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_PLANE_USERDEF_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnF2Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06DefineLineUserDefModelC* pModel = new DCP::DCP06DefineLineUserDefModelC();
	pModel->lineModel->active_plane	= m_pDataModel->domModel->dom_active_plane;
	pModel->lineModel->active_line		= m_pDataModel->domModel->dom_active_line;
	//pModel->hz_plane		= m_pDataModel->dom_hz_plane;
	memcpy(&pModel->lineModel->line_buff[0],&m_pDataModel->domModel->dom_line_buff[0],  sizeof(S_LINE_BUFF));
	pModel->lineModel->display = DOM_USERDEF_DLG;
	
	memcpy(&pModel->userdef_line_points_no[0],&m_pDataModel->userdef_line_points_no[0], sizeof(short) * MAX_USERDEF_POINTS);
	memcpy(&pModel->userdef_measured_points[0],&m_pDataModel->userdef_measured_points[0], sizeof(S_POINT_BUFF) *MAX_USERDEF_POINTS);

	m_pDataModel->build_select_point_list();
	memcpy(&pModel->select_point_list[0], &m_pDataModel->select_point_list[0], sizeof(S_SELECT_POINTS) * MAX_USERDEF_POINTS);

	if(GetController(DEFINE_LINE_USERDEF_CONTROLLER) == NULL)
	{
		(void)AddController( DEFINE_LINE_USERDEF_CONTROLLER, new DCP::DCP06DefineLineUserDefControllerC(m_pDlg->GetDCP06Model()) );
	}
	(void)GetController( DEFINE_LINE_USERDEF_CONTROLLER )->SetModel(pModel);
	SetActiveController(DEFINE_LINE_USERDEF_CONTROLLER, true);
	
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnF3Pressed()
{	
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06OffsVModelC* pModel = new DCP::DCP06OffsVModelC();
	memcpy(&pModel->ovalues_buff,&m_pDataModel->domModel->dom_ovalues_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ref_point_buff,&m_pDataModel->domModel->dom_ref_point_buff,sizeof(S_POINT_BUFF));
	memcpy(&pModel->ovalues_tool_buff,&m_pDataModel->domModel->dom_ovalues_tool_buff,sizeof(S_POINT_BUFF));
	pModel->display = DOM_USERDEF_DLG;

	if(GetController(OFFSV_CONTROLLER) == NULL)
	{
		(void)AddController( OFFSV_CONTROLLER, new DCP::DCP06OffsvControllerC(m_pDlg->GetDCP06Model(),pModel->display));
	}
	(void)GetController( OFFSV_CONTROLLER )->SetModel(pModel);
	SetActiveController(OFFSV_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnF4Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	

	StringC sActualSelected;
	sActualSelected.LoadTxt(AT_DCP05,P_DCP_ACTUAL_SELECTED_TOK);

    char cActSelected[10];

	//UTL::UnicodeToAscii(cActSelected, 2, sActualSelected);
	BSS::UTI::BSS_UTI_WCharToAscii(sActualSelected, cActSelected,2);

	DCP::DCP06SelectPointModelC* pModel = new DCP06SelectPointModelC;
	
	memset(&pModel->points[0], 0, sizeof(S_SELECT_POINT) * MAX_SELECT_POINTS);
	// build pointlist
	int iLastDefined = 0;
	for(int i = 0; i < MAX_USERDEF_POINTS; i++)
	{
		pModel->points[i].no = i+1;
		sprintf(pModel->points[i].point_id,"%-6.6s",m_pDataModel->userdef_measured_points[i].point_id);

		if(m_pDataModel->userdef_measured_points[i].sta > 0)
		{
			sprintf(pModel->points[i].point_status,"%s",cActSelected);
			iLastDefined = i+1;
		}
		else
			sprintf(pModel->points[i].point_status,"%s","-");
	}
	//int iCount = m_pDataModel->m_pFileFunc->GetPointList(&pModel->points[0],MAX_SELECT_POINTS);
	pModel->m_iCounts = iLastDefined;
	pModel->m_iSelectedId = 0;
	
	if(GetController(SELECT_POINT_CONTROLLER) == NULL)
	{
		(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP06SelectPointControllerC );
	}

	//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_PLANE_MEAS_TOK);

	(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_POINT_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
// CALC
void DCP::DCP06DomUserDefControllerC::OnF5Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	// kopioidaan mitatut pisteet oikeille paikoillee
	m_pDataModel->copy_measured_points();

	// calc line
	if(GetController(CALC_LINE_CONTROLLER) == NULL)
	{
		(void)AddController( CALC_LINE_CONTROLLER, new DCP::DCP06CalcLineControllerC(&m_pDataModel->domModel->dom_line_buff[0],ACTUAL, 0) );
	}

	(void)GetController( CALC_LINE_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
	SetActiveController(CALC_LINE_CONTROLLER, true);

	/* siirr� t�m� oikeaan paikkaan

	DCP06CalcDomC calc_dom(m_pDataModel->domModel);
	if(calc_dom.calc())
	{	
		m_pDlg->GetDCP06Model()->active_coodinate_system = OCSU;
		m_pDataModel->domModel->ocsd_defined = true;
		m_pDlg->GetDCP06Model()->ocsu_defined = true;
		// kopioi arvot DCP05Model:iin
		memcpy(m_pDlg->GetDCP06Model()->ocsu_matrix, m_pDataModel->domModel->matrix,sizeof(double) * 16);
		memcpy(m_pDlg->GetDCP06Model()->ocsu_inv_matrix, m_pDataModel->domModel->inv_matrix, sizeof(double) * 16);
		m_pDlg->GetDCP06Model()->userdef_active_plane	= m_pDataModel->domModel->dom_active_plane;
		m_pDlg->GetDCP06Model()->userdef_active_line	= m_pDataModel->domModel->dom_active_line; 
		m_pDlg->GetDCP06Model()->userdef_hz_plane		= m_pDataModel->domModel->dom_hz_plane;
		memcpy(&m_pDlg->GetDCP06Model()->userdef_plane_buff[0], &m_pDataModel->domModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_hz_plane_buff[0], &m_pDataModel->domModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_line_buff[0], &m_pDataModel->domModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_buff, &m_pDataModel->domModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_tool_buff, &m_pDataModel->domModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_ref_point_buff, &m_pDataModel->domModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_plane_buff, &m_pDataModel->domModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_line_buff, &m_pDataModel->domModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
		Close(EC_KEY_CONT);
	}
	*/
}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnF6Pressed()
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
void DCP::DCP06DomUserDefControllerC::OnSHF2Pressed()
{	
	m_pDlg->delete_dom();
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnSHF3Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::DCP06RotatePlaneModelC* pModel = new DCP::DCP06RotatePlaneModelC();
	
	memcpy(&pModel->point_buff, &m_pDataModel->domModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->domModel->dom_active_plane;
	pModel->display = DOM_USERDEF_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP05,T_DCP_USERDEF_ROTATE_PLANE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_PLANE_CONTROLLER) == NULL)
	{
		(void)AddController( ROTATE_PLANE_CONTROLLER, new DCP::DCP06RotatePlaneControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController( ROTATE_PLANE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_PLANE_CONTROLLER, true);

}

//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnSHF4Pressed()
{
	

	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP06RotateLineModelC* pModel = new DCP::DCP06RotateLineModelC();
	
	memcpy(&pModel->point_buff, &m_pDataModel->domModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));
	pModel->plane_type = m_pDataModel->domModel->dom_active_plane;
	pModel->display = DOM_USERDEF_DLG;
	StringC sTitle;
	sTitle.LoadTxt(AT_DCP05,T_DCP_USERDEF_ROTATE_LINE_TOK);
	pModel->sTitle = sTitle;

	if(GetController(ROTATE_LINE_CONTROLLER) == NULL)
	{
		(void)AddController( ROTATE_LINE_CONTROLLER, new DCP::DCP06RotateLineControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController( ROTATE_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(ROTATE_LINE_CONTROLLER, true);
	
}

void DCP::DCP06DomUserDefControllerC::open_meas_display() 
{
		if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP06CommonC common(m_pDCP06Model);
	DCP06MsgBoxC msgBox;

	int iCount = m_pDataModel->get_measured_points_count();
	bool delete_pid = true;
	int iLastDefinedPoint = 2;

	if(iCount > 0)
	{
		StringC msg;
		msg.LoadTxt(AT_DCP05, M_DCP_DELETE_POINTS_TOK);
		msg.Format(msg,L"");
		

		if(msgBox.ShowMessageYesNo(msg))
		{
			if(m_pDataModel->is_point_id_defined())
			{
				msg.LoadTxt(AT_DCP05,M_DCP_DELETE_POINT_IDS_TOK);
				if(!msgBox.ShowMessageYesNo(msg))
				{
					iLastDefinedPoint = m_pDataModel->get_last_defined_point();

					for(int i = 0; i < MAX_USERDEF_POINTS; i++)
					{
						m_pDataModel->userdef_measured_points[i].cds = 0;
						m_pDataModel->userdef_measured_points[i].diameter = 0.0;
						m_pDataModel->userdef_measured_points[i].dsta = 0;
						m_pDataModel->userdef_measured_points[i].sta = 0;
						m_pDataModel->userdef_measured_points[i].x = 0.0;
						m_pDataModel->userdef_measured_points[i].xdes = 0.0;
						m_pDataModel->userdef_measured_points[i].xsta = 0;
						m_pDataModel->userdef_measured_points[i].y = 0.0;
						m_pDataModel->userdef_measured_points[i].ydes = 0.0;
						m_pDataModel->userdef_measured_points[i].ysta = 0;
						m_pDataModel->userdef_measured_points[i].z = 0.0;
						m_pDataModel->userdef_measured_points[i].zdes = 0.0;
						m_pDataModel->userdef_measured_points[i].zsta = 0;
					}
				}
				else
				{
					iLastDefinedPoint = m_pDataModel->get_last_defined_point();
					memset(&m_pDataModel->userdef_measured_points[0],0,sizeof(S_POINT_BUFF)* MAX_USERDEF_POINTS);
					
				}
			}
			else
			{
				memset(&m_pDataModel->userdef_measured_points[0],0,sizeof(S_POINT_BUFF)* MAX_USERDEF_POINTS);
				iLastDefinedPoint = m_pDataModel->get_last_defined_point();
			}
		}
		else
		{
			iLastDefinedPoint = m_pDataModel->get_last_defined_point();
		}
	}
	else
	{
		iLastDefinedPoint = m_pDataModel->get_last_defined_point();
	}
	
	//int iLastDefinedPoint = m_pDataModel->get_last_defined_point();

	m_pDlg->GetDCP06Model()->active_coodinate_system = DCS;

	DCP::DCP06MeasModelC* pModel = new DCP06MeasModelC;
	pModel->m_iMaxPoint = MAX_USERDEF_POINTS;
	pModel->m_iMinPoint = 2;
	pModel->m_iPointsCount = (iLastDefinedPoint < 2) ? 2 : iLastDefinedPoint;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
	memcpy(&pModel->point_table[0], &m_pDataModel->userdef_measured_points[0], sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);

	//memcpy(&pModel->point_table[0], &m_pDlg->GetDataModel()->plane_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);

	if(GetController(MEAS_CONTROLLER) == NULL)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::DCP06MeasControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_USERDEF_MEAS_TOK));

	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnSHF5Pressed()
{
	open_meas_display();
	/*
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	DCP::DCP06MeasModelC* pModel = new DCP06MeasModelC;
	pModel->m_iMaxPoint = MAX_BESTFIT_POINTS;
	pModel->m_iMinPoint = 2;
	pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);
	memcpy(&pModel->point_table[0], &m_pDlg->GetDCP06Model()->userdef_measured_points[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	//memcpy(&pModel->point_table[0], &m_pDlg->GetDataModel()->plane_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_BESTFIT_POINTS);

	if(GetController(MEAS_CONTROLLER) == NULL)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::DCP06MeasControllerC(m_pDlg->GetDCP06Model()));
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_USERDEF_MEAS_TOK);

	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
	*/
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == A321_USERDEF_STATUS_DLG)
	{
		show_function_keys();
		/*
		HideFunctionKey(FK2);
		ShowFunctionKey(FK4);
		HideFunctionKey(FK5);
		HideFunctionKey(FK6);
		HideFunctionKey(SFK2);
		HideFunctionKey(SFK3);
		HideFunctionKey(SFK4);
		HideFunctionKey(SFK5);
		*/
		if(lExitCode == 100 || lExitCode == 200)
		{
			//if(GetController(A321_USERDEF_CONTROLLER) == NULL)
				SetActiveDialog(DOM_USERDEF_DLG, true);

				if(lExitCode == 100)
				{
					// show config
				}
				else
				{
					open_meas_display();
				}
		}
		else
		{
			Close(lExitCode);
		}
	}
}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::DCP06DomUserDefControllerC::OnControllerActivated(void)
{
	ResetFunctionKeys();
	/*
	HideFunctionKey(FK2);
	HideFunctionKey(FK4);
	HideFunctionKey(FK5);
	HideFunctionKey(FK6);
	HideFunctionKey(SFK2);
	HideFunctionKey(SFK3);
	HideFunctionKey(SFK4);
	HideFunctionKey(SFK5);
	*/
	short count = m_pDataModel->get_measured_points_count();
	bool defined = count >= 2 ? true: false;
	DCP06UserDefStatusDlgC* poStatusDlg = new DCP06UserDefStatusDlgC(m_pDlg->GetDCP06Model(), defined);
	(void) AddDialog(A321_USERDEF_STATUS_DLG, poStatusDlg); 	
	SetActiveDialog(A321_USERDEF_STATUS_DLG, true);

	
}
//-------------------------------------------------------------------------------------------------
void DCP::DCP06DomUserDefControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{


	if(lCtrlID == MEAS_CONTROLLER)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::DCP06MeasModelC* pModel = (DCP::DCP06MeasModelC*) GetController( MEAS_CONTROLLER )->GetModel();		

			// copy values
			memcpy(&m_pDataModel->userdef_measured_points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
			
			m_pDataModel->build_select_point_list();
			m_pDataModel->domModel->old_active_coodinate_system = DCS;
			m_pDlg->GetDCP06Model()->ocsu_defined = false;
		}
		else if(lExitCode == EC_KEY_ESC)
		{
			(void)Close(EC_KEY_ESC);	
		}
	}

	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectPointModelC* pModel = (DCP::DCP06SelectPointModelC*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		StringC strSelectedPoint = pModel->m_strSelectedPoint;
		short iSelectedPointId = pModel->m_iSelectedId;

		if(m_pDataModel->userdef_measured_points[iSelectedPointId -1].sta > 0)
		{
			m_pDataModel->userdef_point_no = iSelectedPointId;
		}
		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;
	}

	if(lCtrlID == DEFINE_PLANE_USERDEF_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DCP06DefinePlaneUserDefModelC* pModel = (DCP::DCP06DefinePlaneUserDefModelC*) GetController( DEFINE_PLANE_USERDEF_CONTROLLER )->GetModel();		

		// copy values into dommodel
		m_pDataModel->domModel->dom_active_plane	= pModel->planeModel->active_plane;
		m_pDataModel->domModel->dom_active_line	= pModel->planeModel->active_line; 
		m_pDataModel->domModel->dom_hz_plane		= pModel->planeModel->hz_plane;
		memcpy(&m_pDataModel->domModel->dom_plane_buff[0], &pModel->planeModel->plane_buff[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->domModel->dom_hz_plane_buff[0], &pModel->planeModel->hz_plane_buff[0], sizeof(S_PLANE_BUFF));

		memcpy(&m_pDataModel->userdef_plane_points_no[0], &pModel->userdef_plane_points_no[0], sizeof(short)* MAX_USERDEF_POINTS);

		if(m_pDataModel->domModel->dom_active_plane == XY_PLANE || m_pDataModel->domModel->dom_active_plane == ZX_PLANE)
			m_pDataModel->domModel->dom_active_line = X_LINE;
		else
			m_pDataModel->domModel->dom_active_line = Y_LINE;

		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;

	}
	if(lCtrlID == DEFINE_LINE_USERDEF_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06DefineLineUserDefModelC* pModel = (DCP::DCP06DefineLineUserDefModelC*) GetController( DEFINE_LINE_USERDEF_CONTROLLER )->GetModel();		
		// copy values into dommodel
		m_pDataModel->domModel->dom_active_line	= pModel->lineModel->active_line; 
		memcpy(&m_pDataModel->domModel->dom_line_buff[0], &pModel->lineModel->line_buff[0], sizeof(S_LINE_BUFF));
		memcpy(&m_pDataModel->userdef_line_points_no[0], &pModel->userdef_line_points_no[0], sizeof(short)* MAX_USERDEF_POINTS);

		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;
		
	}

	if(lCtrlID == OFFSV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06OffsVModelC* pModel = (DCP::DCP06OffsVModelC*) GetController( OFFSV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->domModel->dom_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->domModel->dom_ovalues_tool_buff, &pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->domModel->dom_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;

	}
	/*
	if(lCtrlID == MEASV_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06OffsVModelC* pModel = (DCP::DCP06OffsVModelC*) GetController( MEASV_CONTROLLER )->GetModel();		
		// copy values into dommodel
		memcpy(&m_pDataModel->domModel->dom_ovalues_buff, &pModel->ovalues_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->domModel->dom_ovalues_tool_buff,&pModel->ovalues_tool_buff, sizeof(S_POINT_BUFF));
		memcpy(&m_pDataModel->domModel->dom_ref_point_buff, &pModel->ref_point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;

	}
	*/
	if(lCtrlID == ROTATE_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06RotatePlaneModelC* pModel = (DCP::DCP06RotatePlaneModelC*) GetController( ROTATE_PLANE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->domModel->dom_rot_plane_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));
		
		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;
	}

	if(lCtrlID == ROTATE_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06RotatePlaneModelC* pModel = (DCP::DCP06RotatePlaneModelC*) GetController( ROTATE_LINE_CONTROLLER )->GetModel();		
		// copy values into dommodel
				
		memcpy(&m_pDataModel->domModel->dom_rot_line_buff, &pModel->point_buff, sizeof(S_POINT_BUFF));

		m_pDataModel->domModel->old_active_coodinate_system = DCS;
		m_pDlg->GetDCP06Model()->ocsu_defined = false;
	}
	
	if(lCtrlID == CALC_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		//m_pDataModel->domModel->line_buff[0].sta = m_pDlg->GetDataModel()->line_buff[0].points[0].sta;

		if(!m_pDataModel->domModel->dom_hz_plane)
		{
			// LASKETAAN Plane
			//if(m_pDlg->GetDataModel()->display == DOM_USERDEF_DLG)
			//	m_pDlg->GetDataModel()->hz_plane = false;
			
			if(GetController(CALC_PLANE_CONTROLLER) == NULL)
			{
				(void)AddController( CALC_PLANE_CONTROLLER, new DCP::DCP06CalcPlaneControllerC(&m_pDataModel->domModel->dom_plane_buff[0],ACTUAL, 0) );
			}

			(void)GetController( CALC_PLANE_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
			SetActiveController(CALC_PLANE_CONTROLLER, true);
		}
		else
		{
			
			DCP06CalcDomC calc_dom(m_pDataModel->domModel);
			if(calc_dom.calc())
			{	
				m_pDlg->GetDCP06Model()->active_coodinate_system = OCSU;
				m_pDataModel->domModel->ocsd_defined = true;
				m_pDlg->GetDCP06Model()->ocsu_defined = true;
				// kopioi arvot DCP05Model:iin
				memcpy(m_pDlg->GetDCP06Model()->ocsu_matrix, m_pDataModel->domModel->matrix,sizeof(double) * 16);
				memcpy(m_pDlg->GetDCP06Model()->ocsu_inv_matrix, m_pDataModel->domModel->inv_matrix, sizeof(double) * 16);
				m_pDlg->GetDCP06Model()->userdef_active_plane	= m_pDataModel->domModel->dom_active_plane;
				m_pDlg->GetDCP06Model()->userdef_active_line	= m_pDataModel->domModel->dom_active_line; 
				m_pDlg->GetDCP06Model()->userdef_hz_plane		= m_pDataModel->domModel->dom_hz_plane;
				memcpy(&m_pDlg->GetDCP06Model()->userdef_plane_buff[0], &m_pDataModel->domModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_hz_plane_buff[0], &m_pDataModel->domModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_line_buff[0], &m_pDataModel->domModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_buff, &m_pDataModel->domModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_tool_buff, &m_pDataModel->domModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_ref_point_buff, &m_pDataModel->domModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_plane_buff, &m_pDataModel->domModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
				memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_line_buff, &m_pDataModel->domModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));

				memcpy(&m_pDlg->GetDCP06Model()->userdef_measured_points,&m_pDataModel->userdef_measured_points,  sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
				memcpy(&m_pDlg->GetDCP06Model()->userdef_plane_points_no,&m_pDataModel->userdef_plane_points_no, sizeof(short) * MAX_USERDEF_POINTS);
				memcpy(&m_pDlg->GetDCP06Model()->userdef_line_points_no,&m_pDataModel->userdef_line_points_no,  sizeof(short) * MAX_USERDEF_POINTS);
				m_pDlg->GetDCP06Model()->userdef_point_no = m_pDataModel->userdef_point_no; 

				Close(EC_KEY_CONT);
			}
		}
	}
	if(lCtrlID == CALC_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			// calc_dom
		DCP06CalcDomC calc_dom(m_pDataModel->domModel);
		if(calc_dom.calc())
		{	
			m_pDlg->GetDCP06Model()->active_coodinate_system = OCSU;
			m_pDataModel->domModel->ocsd_defined = true;
			m_pDlg->GetDCP06Model()->ocsu_defined = true;
			// kopioi arvot DCP05Model:iin
			memcpy(m_pDlg->GetDCP06Model()->ocsu_matrix, m_pDataModel->domModel->matrix,sizeof(double) * 16);
			memcpy(m_pDlg->GetDCP06Model()->ocsu_inv_matrix, m_pDataModel->domModel->inv_matrix, sizeof(double) * 16);
			m_pDlg->GetDCP06Model()->userdef_active_plane	= m_pDataModel->domModel->dom_active_plane;
			m_pDlg->GetDCP06Model()->userdef_active_line	= m_pDataModel->domModel->dom_active_line; 
			m_pDlg->GetDCP06Model()->userdef_hz_plane		= m_pDataModel->domModel->dom_hz_plane;
			memcpy(&m_pDlg->GetDCP06Model()->userdef_plane_buff[0], &m_pDataModel->domModel->dom_plane_buff[0], sizeof(S_PLANE_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_hz_plane_buff[0], &m_pDataModel->domModel->dom_hz_plane_buff[0], sizeof(S_PLANE_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_line_buff[0], &m_pDataModel->domModel->dom_line_buff[0], sizeof(S_LINE_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_buff, &m_pDataModel->domModel->dom_ovalues_buff, sizeof(S_POINT_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_ovalues_tool_buff, &m_pDataModel->domModel->dom_ovalues_tool_buff, sizeof(S_POINT_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_ref_point_buff, &m_pDataModel->domModel->dom_ref_point_buff, sizeof(S_POINT_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_plane_buff, &m_pDataModel->domModel->dom_rot_plane_buff, sizeof(S_POINT_BUFF));
			memcpy(&m_pDlg->GetDCP06Model()->userdef_rot_line_buff, &m_pDataModel->domModel->dom_rot_line_buff, sizeof(S_POINT_BUFF));

			
			memcpy(&m_pDlg->GetDCP06Model()->userdef_measured_points,&m_pDataModel->userdef_measured_points,  sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);
				memcpy(&m_pDlg->GetDCP06Model()->userdef_plane_points_no,&m_pDataModel->userdef_plane_points_no, sizeof(short) * MAX_USERDEF_POINTS);
				memcpy(&m_pDlg->GetDCP06Model()->userdef_line_points_no,&m_pDataModel->userdef_line_points_no,  sizeof(short) * MAX_USERDEF_POINTS);
				m_pDlg->GetDCP06Model()->userdef_point_no = m_pDataModel->userdef_point_no; 

			Close(EC_KEY_CONT);
		}
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// ===========================================================================================
// DOMModel
// ===========================================================================================

// Instantiate template classes
DCP::DCP06DomUserDefModelC::DCP06DomUserDefModelC(DCP06ModelC* pDCP06Model): m_pDCP06Model(pDCP06Model)
{
	domModel = new DCP06DomModelC;
	memset(&userdef_measured_points, 0, sizeof(S_POINT_BUFF) * MAX_USERDEF_POINTS);

	memset(&select_point_list[0], 0, sizeof(S_SELECT_POINTS) * MAX_USERDEF_POINTS);
}

DCP::DCP06DomUserDefModelC::~DCP06DomUserDefModelC()
{
	if(domModel != 0)
	{
		delete domModel;
		domModel = 0;
	}
}

short DCP::DCP06DomUserDefModelC::get_measured_points_count()
{
int i,count;
		
	count = 0;

		for(i=0;i<=MAX_USERDEF_POINTS;i++)
		{
			if(userdef_measured_points[i].sta != 0)
			{
					count++;
			}
		}
		return count;
}

short DCP::DCP06DomUserDefModelC::get_last_defined_point()
{
	short last = 0;
	DCP06CommonC common(m_pDCP06Model);

	for(int i = MAX_USERDEF_POINTS; i >=1; i--)
	{
		if(userdef_measured_points[i-1].sta != 0 || !common.strblank(userdef_measured_points[i-1].point_id))
		{
			last = i;
			break;
		}
	}
	return last;
}

bool DCP::DCP06DomUserDefModelC::is_point_id_defined()
{
	char pid[10];
	bool ret = false;
	DCP06CommonC common(m_pDCP06Model);
	
	for(int i = 0; i < MAX_USERDEF_POINTS; i++)
	{
		sprintf(pid,"%-6.6s",userdef_measured_points[i].point_id);
		if(!common.strblank(pid))
		{
			ret = true;
			break;
		}
	}
	return ret;
}

short DCP::DCP06DomUserDefModelC::build_select_point_list()
{
	memset(&select_point_list[0],0,sizeof(S_SELECT_POINTS) * MAX_USERDEF_POINTS);
	
	DCP06CommonC common(m_pDCP06Model);
    bool mea = false;
	short iCount = common.get_last_defined_point(&userdef_measured_points[0],MAX_USERDEF_POINTS);

	for(int i=1; i <= iCount; i++)
	{
		mea = false;
			
		if(userdef_measured_points[i-1].sta != 0)
			mea = true;
		//if(!common.strblank(xmea) && !common.strblank(ymea) && !common.strblank(zmea))
		//	mea = true;

		select_point_list[i-1].iId = i;
		sprintf(select_point_list[i-1].point_id,"%-6.6s",userdef_measured_points[i-1].point_id);
		select_point_list[i-1].bActualDefined = mea;
		select_point_list[i-1].bDesignDefined = false;


			select_point_list[i-1].bDesignSelected = false;
			select_point_list[i-1].bActualSelected = (mea) ? true : false;
	}

	return iCount;

}

short DCP::DCP06DomUserDefModelC::copy_measured_points()
{
	short point_no = 0;
	int i = 0;

	memset(&domModel->dom_plane_buff[0],0,sizeof(S_PLANE_BUFF));
	memset(&domModel->dom_line_buff[0],0,sizeof(S_LINE_BUFF));	
	memset(&domModel->dom_ref_point_buff,0,sizeof(S_POINT_BUFF));	

	if(userdef_point_no > 0)
	{
		domModel->dom_ref_point_buff.x = userdef_measured_points[userdef_point_no -1].x;
		domModel->dom_ref_point_buff.y = userdef_measured_points[userdef_point_no -1].y;
		domModel->dom_ref_point_buff.z = userdef_measured_points[userdef_point_no -1].z;
		domModel->dom_ref_point_buff.sta = userdef_measured_points[userdef_point_no -1].sta;
	}
	else
	{
		memset(&domModel->dom_ref_point_buff,0,sizeof(S_POINT_BUFF));	
	}

	// line points
	for(i = 0; i < MAX_USERDEF_POINTS; i++)
	{
		point_no = userdef_line_points_no[i];

		if(point_no > 0)
		{
			domModel->dom_line_buff[0].points[i].x = userdef_measured_points[point_no -1].x;
			domModel->dom_line_buff[0].points[i].y = userdef_measured_points[point_no -1].y;
			domModel->dom_line_buff[0].points[i].z = userdef_measured_points[point_no -1].z;
			domModel->dom_line_buff[0].points[i].sta = userdef_measured_points[point_no -1].sta;
		}
	}

	if(!domModel->dom_hz_plane)
	{
		// plane points
		for(i = 0; i < MAX_USERDEF_POINTS; i++)
		{
			point_no = userdef_plane_points_no[i];

			if(point_no > 0)
			{
				domModel->dom_plane_buff[0].points[i].x = userdef_measured_points[point_no -1].x;
				domModel->dom_plane_buff[0].points[i].y = userdef_measured_points[point_no -1].y;
				domModel->dom_plane_buff[0].points[i].z = userdef_measured_points[point_no -1].z;
				domModel->dom_plane_buff[0].points[i].sta = userdef_measured_points[point_no -1].sta;
			}
		}
	}

	return 1;
}