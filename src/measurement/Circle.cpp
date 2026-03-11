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

#include "calc.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Logger.hpp>
#include <dcp06/init/Initialization.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/calculation/CalculationCircleCore.hpp>
#include <dcp06/calculation/ResCircle.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>

#include <math.h>
#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <UTL_String.hpp>
#include <UTL_StringFunctions.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::CircleDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CircleDialog::CircleDialog(DCP::Model * pModel, CircleModel* pCircleModel, short iDisplay):
		GUI::ModelHandlerC(),
		GUI::StandardDialogC(),m_pModel(pModel),m_pDefinePlaneInfo(0),m_pCircleId(0),m_pPlane(0),m_iDisplay(iDisplay),
		m_pToolRadius(0),m_pMeasureCirclePoints(0),m_pCirclePoints(0),m_pDataModel(pCircleModel),
		m_pToolRadiusObserver(OBS_METHOD_TO_PARAM0(CircleDialog, OnValueChanged), this)
{
	
	//SetTxtApplicationId(AT_DCP06);

	// load texts...
	sXY_plane.LoadTxt(AT_DCP06, V_DCP_XY_PLANE_TOK);
	sZX_plane.LoadTxt(AT_DCP06, V_DCP_ZX_PLANE_TOK);
	sYZ_plane.LoadTxt(AT_DCP06, V_DCP_YZ_PLANE_TOK); 
	sCIRCLEPOINTS_plane.LoadTxt(AT_DCP06, V_DCP_CIRCLE_POINTS_TOK);
	sMeasPlane.LoadTxt(AT_DCP06,V_DCP_MEASURED_CIRCLE_TOK);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CircleDialog::~CircleDialog()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::CircleDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	if(m_iDisplay != SHAFT_DLG)
	{
		m_pCircleId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
		m_pCircleId->SetId(eCircleId);
		m_pCircleId->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_ID_TOK));
		m_pCircleId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
		m_pCircleId->SetEmptyAllowed(true);
		AddCtrl(m_pCircleId);

		m_pDefinePlaneInfo = new GUI::TextCtrlC();
		m_pDefinePlaneInfo->SetId(eDefinePlaneInfo);
		m_pDefinePlaneInfo->SetText(StringC(AT_DCP06,L_DCP_DEFINE_CIRCLE_PLANE_TOK));
		AddCtrl(m_pDefinePlaneInfo);

		m_pPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
		m_pPlane->SetId(ePlane);
		m_pPlane->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_PLANE_TOK));
		m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
		m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
		//m_pPlane->SetAutoColon(false);
		//m_pPlane->SetColonPosition(9*22);
		// m_pPlane->SetTextWidth(9*40); CAPTIVATE
		
		AddCtrl(m_pPlane);
	}

	m_pToolRadius = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolRadius->SetId(eToolRadius);
	m_pToolRadius->SetText(StringC(AT_DCP06,P_DCP_TOOL_RADIUS_TOK));
	m_pToolRadius->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pModel->m_nDecimals);
	//m_pToolRadius->SetAutoColon(false);
	//m_pToolRadius->SetColonPosition(9*22);
	// m_pToolRadius->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pToolRadius);
		

	m_pMeasureCirclePoints = new GUI::TextCtrlC();
	m_pMeasureCirclePoints->SetId(eMeasureCirclePoints);
	//if(m_iDisplay != SHAFT_DLG)
		m_pMeasureCirclePoints->SetText(StringC(AT_DCP06,L_DCP_MEASURE_CIRCLE_POINTS_TOK));
	//else
	//	m_pMeasureCirclePoints->SetTextTok(L_DCP_MEASURE_SHAFT_POINTS_TOK);
	AddCtrl(m_pMeasureCirclePoints);

	m_pCirclePoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCirclePoints->SetId(eCirclePoints);
	
	//if(m_iDisplay != SHAFT_DLG)
		m_pCirclePoints->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_POINTS_TOK));
	//else
	//	m_pCirclePoints->GetTextCtrl()->SetTextTok(P_DCP_SHAFT_POINTS_TOK);

	m_pCirclePoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCirclePoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pCirclePoints->SetEmptyAllowed(true);
	//m_pCirclePoints->SetAutoColon(false);
	//m_pCirclePoints->SetColonPosition(9*22);
	// m_pCirclePoints->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pCirclePoints);

	// set help
	// TODO VIVA
	/*
	if(m_iDisplay != SHAFT_DLG)
		SetHelpTok(H_DCP_CIRCLE_TOK,0);
	else
		SetHelpTok(H_DCP_SHAFT_CIRCLE_TOK,0);
	*/
	if(m_iDisplay != SHAFT_DLG)
	m_pToolRadiusObserver.Attach(m_pToolRadius->GetSubject());
	DCP06_TRACE_EXIT;
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::CircleDialog::OnDialogActivated()
{
	
	


	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::CircleDialog::UpdateData()
{
	// copy values from DCP05MOdel
	if(m_iDisplay != SHAFT_DLG)
	{

		memcpy(&m_pModel->circle_planes[0],&m_pDataModel->planes[0],  sizeof(S_PLANE_BUFF));
		memcpy(&m_pModel->circle_points[0],&m_pDataModel->circle_points[0],  sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pModel->circle_points_in_plane[0], &m_pDataModel->circle_points_in_plane[0],  sizeof(S_CIRCLE_BUFF));
		m_pModel->circle_plane_type = m_pDataModel->PLANE_TYPE;
		m_pModel->circle_cx = m_pDataModel->cx;
		m_pModel->circle_cy = m_pDataModel->cy;
		m_pModel->circle_cz = m_pDataModel->cz;
		m_pModel->circle_vi = m_pDataModel->vi;
		m_pModel->circle_vj = m_pDataModel->vj;
		m_pModel->circle_vk = m_pDataModel->vk;

		m_pModel->circle_diameter = m_pDataModel->diameter;

		m_pModel->poConfigController->GetModel()->SetConfigKey(CNF_KEY_CIRCLE);
		m_pModel->poConfigController->StoreConfigData();
		m_pModel->circle_rms = m_pDataModel->rms_diameter;

	}
	else
	{
		//memcpy(&m_pModel->circle_planes[0],&m_pDataModel->planes[0],  sizeof(S_PLANE_BUFF));
		memcpy(&m_pModel->shaft_circle_points[0],&m_pDataModel->circle_points[0],  sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pModel->shaft_circle_points_in_plane[0], &m_pDataModel->circle_points_in_plane[0],  sizeof(S_CIRCLE_BUFF));
		m_pModel->shaft_circle_plane_type = m_pDataModel->PLANE_TYPE;
		m_pModel->shaft_circle_cx = m_pDataModel->cx;
		m_pModel->shaft_circle_cy = m_pDataModel->cy;
		m_pModel->shaft_circle_cz = m_pDataModel->cz;
		m_pModel->shaft_circle_vi = m_pDataModel->vi;
		m_pModel->shaft_circle_vj = m_pDataModel->vj;
		m_pModel->shaft_circle_vk = m_pDataModel->vk;

		m_pModel->shaft_circle_diameter = m_pDataModel->diameter;
		m_pModel->shaft_circle_rms = m_pDataModel->rms_diameter;

		m_pModel->poConfigController->GetModel()->SetConfigKey(CNF_KEY_SHAFT);
		m_pModel->poConfigController->StoreConfigData();
	}

}
// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::CircleDialog::OnValueChanged( int unNotifyCode,  int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		if(ulParam2 == eToolRadius)
		{
			if(!m_pToolRadius->GetFloatInputCtrl()->IsEmpty())
				m_pDataModel->bR = m_pToolRadius->GetFloatInputCtrl()->GetDouble();
			else
				m_pDataModel->bR = 0.0;
		}
	}
}


// ================================================================================================
// Description: refresh all controls
// ================================================================================================

void DCP::CircleDialog::RefreshControls()
{
	if(m_pToolRadius && m_pMeasureCirclePoints && m_pCirclePoints)
	{	
		StringC sTemp;
	
		if(m_iDisplay != SHAFT_DLG)
		{
			if (m_pCircleId)
			{
				char cid[CIRCLE_ID_BUFF_LEN];
				sprintf(cid, "%-s", m_pDataModel->circle_points[0].id);
				if (m_pDataModel->pCommon) m_pDataModel->pCommon->strbtrim(cid);
				m_pCircleId->GetStringInputCtrl()->SetString(cid[0] ? StringC(cid) : StringC(L"Ci1"));
			}
			if (m_pDefinePlaneInfo && m_pPlane)
			{
			short sta = m_pDataModel->planes[0].calc;

			if( m_pDataModel->PLANE_TYPE == XY_PLANE)
				sTemp = sXY_plane;
				//sprintf(bCplane,"%-s",txt_xy); //"xy");

			else if(m_pDataModel->PLANE_TYPE ==ZX_PLANE)
				sTemp = sZX_plane;
				//sprintf(bCplane,"%-s",txt_zx); //"zx");

			else if(m_pDataModel->PLANE_TYPE==YZ_PLANE)
				sTemp = sYZ_plane;
				//sprintf(bCplane,"%-s",txt_yz); //"yz");

			else if(m_pDataModel->PLANE_TYPE == MEAS_PLANE)
				sTemp = sMeasPlane;
				//sprintf(bCplane,"%-s",txt_measured); //"measured");
		
			else if(m_pDataModel->PLANE_TYPE == CIRCLE_POINTS_PLANE)
				sTemp = sCIRCLEPOINTS_plane;
				//sprintf(bCplane,"%-s",txt_circle_points); //"circle points");
		
			else
				sTemp = L"-";
				//sprintf(bCplane,"%-s","-");
		
			m_pPlane->GetStringInputCtrl()->SetString(sTemp);
			}
		}
		// status
		sTemp = L"-";
		//sprintf(bCpoints,"%c",'-');
		if(m_pDataModel->pCommon->defined_circle_points(m_pDataModel->circle_points,0) >= 3)
		{
			sTemp = L"+";
			//sprintf(bCpoints,"%c",'+');
		}
		m_pCirclePoints->GetStringInputCtrl()->SetString(sTemp);
	
	}
}
// ================================================================================================
// Description: only accept DCP06 Model objects
// ================================================================================================
bool DCP::CircleDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::PointBuffModel* pDcpModel = dynamic_cast< DCP::PointBuffModel* >( pModel );

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
// Description: only accept PointBuffModel objects
// ================================================================================================
DCP::PointBuffModel* DCP::CircleDialog::GetPointBuffModelModel() const
{
    return (DCP::PointBuffModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

bool DCP::CircleDialog::GetCircleIdString(char* buf, size_t bufLen) const
{
	if (!m_pCircleId || !m_pDataModel || !m_pDataModel->pCommon || bufLen == 0) return false;
	StringC s = m_pCircleId->GetStringInputCtrl()->GetString();
	m_pDataModel->pCommon->convert_to_ascii(s, buf, (short)bufLen);
	m_pDataModel->pCommon->strbtrim(buf);
	return true;
}

// ================================================================================================
// ====================================  CircleController  ===================================
// ================================================================================================

// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::CircleController::CircleController(DCP::Model *pModel, short iDisplay)
    : m_pDlg( nullptr ),m_pModel(pModel),m_pDataModel(0), m_iDisplay(iDisplay)

{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    if(iDisplay != SHAFT_DLG)
		SetTitle(StringC( AT_DCP06, T_DCP_CIRCLE_TOK ));
	else
		SetTitle(StringC( AT_DCP06, T_DCP_SHAFT_ALIGMENT_CIRCLE_TOK ));
	

	// create local data
	m_pDataModel = new CircleModel(pModel);

	// copy values from DCP05MOdel
	if(m_iDisplay != SHAFT_DLG)
	{

		memcpy(&m_pDataModel->planes[0], &pModel->circle_planes[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->circle_points[0], &pModel->circle_points[0], sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDataModel->circle_points_in_plane[0], &pModel->circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));

		m_pDataModel->PLANE_TYPE = pModel->circle_plane_type;

	}
	else
	{
		memcpy(&m_pDataModel->circle_points[0], &pModel->shaft_circle_points[0], sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDataModel->circle_points_in_plane[0], &pModel->shaft_circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));
		m_pDataModel->PLANE_TYPE = CIRCLE_POINTS_PLANE;
	}

	// Create a dialog
    m_pDlg = new DCP::CircleDialog(pModel, m_pDataModel, m_iDisplay);  //lint !e1524 new in constructor for class 

    (void)AddDialog( CIRCLE_DLG, m_pDlg, true );

	PLANE_KEYS = 0;

	// set functionkeys
	set_function_keys();
    // Set the function key
	
 


} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: destructor
// ================================================================================================
DCP::CircleController::~CircleController()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// ================================================================================================
// Description: set_function_keys
// ================================================================================================
void DCP::CircleController::set_function_keys()
{
	ResetFunctionKeys();

	if(PLANE_KEYS == 0)
	{
		// DCP9-style: NEW, CLOSE, DEL, PLANE, CIRCL, CONT
		FKDef vDef;
		vDef.poOwner = this;
		vDef.strLable = StringC(AT_DCP06,K_DCP_NEW_TOK);
		SetFunctionKey( FK1, vDef );
		vDef.strLable = StringC(AT_DCP06,K_DCP_CLOSE_TOK);
		SetFunctionKey( FK2, vDef );
		vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
		SetFunctionKey( FK3, vDef );
		vDef.strLable = StringC(AT_DCP06,K_DCP_PLANE_TOK);
		SetFunctionKey( FK4, vDef );
		vDef.strLable = StringC(AT_DCP06,K_DCP_CIRCLE_TOK);
		SetFunctionKey( FK5, vDef );
		vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );
		
	}
	else
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP06;
		vDef.poOwner = this;
		
		vDef.strLable = StringC(AT_DCP06,K_DCP_XY_PLANE_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_ZX_PLANE_TOK);
		SetFunctionKey( FK2, vDef );
		
		vDef.strLable = StringC(AT_DCP06,K_DCP_YZ_PLANE_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_MEAS_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP06,K_DCP_CIRCLE_PLANE_POINTS_TOK);
		SetFunctionKey( FK5, vDef );

	}
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
	vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );	

	GUI::DesktopC::Instance()->UpdateFunctionKeys();

}
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::CircleController::SetModel( GUI::ModelC* pModel )
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
// Description: OnF1Pressed NEW (or XY when in plane menu)
// ================================================================================================
void DCP::CircleController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{
		// NEW: create new circle (clear current without confirmation)
		m_pDataModel->clear_circle();
		m_pDlg->RefreshControls();
	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		if(m_pModel->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = XY_PLANE;
		m_pDlg->RefreshControls();
	}
}

// ================================================================================================
// Description: OnF2Pressed CLOSE (or ZX when in plane menu)
// ================================================================================================
void DCP::CircleController::OnF2Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{
		// CLOSE: close the current circle (clear it, stay on screen) - DCP9 alignment-style, not exit
		m_pDataModel->clear_circle();
		m_pDlg->RefreshControls();
	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		if(m_pModel->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = ZX_PLANE;
		m_pDlg->RefreshControls();
	}
}

// ================================================================================================
// Description: OnF3Pressed DEL (or YZ when in plane menu)
// ================================================================================================
void DCP::CircleController::OnF3Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{
		// DEL: delete circle
		OnSHF2Pressed();
	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		if(m_pModel->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = YZ_PLANE;
		m_pDlg->RefreshControls();
	}
}

// ================================================================================================
// Description: OnF4Pressed PLANE (toggle plane menu, or MEAS when in plane menu)
// ================================================================================================
void DCP::CircleController::OnF4Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(PLANE_KEYS == 1)
	{
		// PLANE: measure circle plane - check instrument ready (avoids crash when not connected)
		if (!m_pDataModel->pCommon->check_edm_mode())
			return;

		DCP::MeasureModel* pModel = new MeasureModel;
		pModel->m_iPointsCount = m_pDataModel->pCommon->points_count_in_plane(&m_pDataModel->planes[0]);
		if(pModel->m_iPointsCount < 3) pModel->m_iPointsCount = 3;
		pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
		pModel->m_iMinPoint = MIN_POINTS_FOR_CIRCLE;
		pModel->m_iCurrentPoint = 1;

		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
		memcpy(&pModel->point_table[0],&m_pDataModel->planes[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);

		if(GetController(CIRCLE_PLANE_MEAS_CONTROLLER) == nullptr)
		{
			(void)AddController( CIRCLE_PLANE_MEAS_CONTROLLER, new DCP::MeasureController(m_pModel) );
		}

		(void)GetController(CIRCLE_PLANE_MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_CIRCLE_PLANE_MEAS_TOK));

		(void)GetController( CIRCLE_PLANE_MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(CIRCLE_PLANE_MEAS_CONTROLLER, true);
		
		PLANE_KEYS = 0;
		set_function_keys();


	}
	else
	{
	}
	m_pDlg->RefreshControls();
}
// ================================================================================================
// Description: OnF5Pressed CIRCLE or Circlepoints
// ================================================================================================

void DCP::CircleController::OnF5Pressed()
{
	DCP06_TRACE_ENTER;
    if (m_pDlg == nullptr)
    {
		DCP06_LOG_DEBUG("-- %s: m_pDlg is null, abort", __FUNCTION__);
        USER_APP_VERIFY( false );
        return;
    }

	if(PLANE_KEYS == 0)
	{
		// CIRCL: measure circle points - check instrument ready before launching (avoids crash when not connected)
		DCP06_LOG_DEBUG("-- %s: before check_edm_mode", __FUNCTION__);
		if (!m_pDataModel->pCommon->check_edm_mode())
		{
			DCP06_LOG_DEBUG("-- %s: check_edm_mode failed, return", __FUNCTION__);
			return;
		}
		DCP06_LOG_DEBUG("-- %s: after check_edm_mode ok", __FUNCTION__);

		DCP06_LOG_DEBUG("-- %s: creating MeasureModel", __FUNCTION__);
		DCP::MeasureModel* pModel = new MeasureModel;
		pModel->m_iPointsCount = m_pDataModel->pCommon->get_max_defined_point_circle(&m_pDataModel->circle_points[0]);
		if(pModel->m_iPointsCount < 3) pModel->m_iPointsCount = 3;
		pModel->m_iMaxPoint = MAX_POINTS_IN_CIRCLE;
		pModel->m_iMinPoint = MIN_POINTS_FOR_CIRCLE;
		pModel->m_iCurrentPoint = 1;

		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
		memcpy(&pModel->point_table[0],&m_pDataModel->circle_points[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
		DCP06_LOG_DEBUG("-- %s: MeasureModel created pts=%d cur=%d", __FUNCTION__, pModel->m_iPointsCount, pModel->m_iCurrentPoint);

		DCP06_LOG_DEBUG("-- %s: checking GetController(MEAS_CONTROLLER)", __FUNCTION__);
		if(GetController(MEAS_CONTROLLER) == nullptr)
		{
			DCP06_LOG_DEBUG("-- %s: adding MEAS_CONTROLLER", __FUNCTION__);
			(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pModel) );
		}
		DCP06_LOG_DEBUG("-- %s: MEAS_CONTROLLER ready", __FUNCTION__);

		if(m_iDisplay != SHAFT_DLG)
			(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_CIRCLE_MEAS_TOK));
		else
			(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_SHAFT_ALIGMENT_CIRCLE_MEAS_TOK));

		DCP06_LOG_DEBUG("-- %s: calling SetModel", __FUNCTION__);
		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		DCP06_LOG_DEBUG("-- %s: calling SetActiveController", __FUNCTION__);
		SetActiveController(MEAS_CONTROLLER, true);
		DCP06_LOG_DEBUG("-- %s: SetActiveController done", __FUNCTION__);


	}
	else
	{	
		m_pDataModel->PLANE_TYPE = CIRCLE_POINTS_PLANE;
		PLANE_KEYS = 0;
		set_function_keys();
	}
	m_pDlg->RefreshControls();
	DCP06_TRACE_EXIT;
}


// ================================================================================================
// Description: F6, CONT
// ================================================================================================
void DCP::CircleController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
    // Update model
	m_pDlg->GetPointBuffModelModel()->m_pPointBuff->x = m_pDataModel->cx;
	m_pDlg->GetPointBuffModelModel()->m_pPointBuff->y = m_pDataModel->cy;
	m_pDlg->GetPointBuffModelModel()->m_pPointBuff->z = m_pDataModel->cz;
	m_pDlg->GetPointBuffModelModel()->m_pPointBuff->sta = m_pDataModel->circle_points[0].sta;
	m_pDlg->GetPointBuffModelModel()->m_pPointBuff->diameter = m_pDataModel->diameter;

	// Set it to hello world dialog
    m_pDlg->UpdateData();

	// Copy circle ID from dialog to model
	if (m_iDisplay != SHAFT_DLG && m_pDlg)
	{
		char cid[CIRCLE_ID_BUFF_LEN];
		cid[0] = '\0';
		if (m_pDlg->GetCircleIdString(cid, sizeof(cid)))
			sprintf(m_pDataModel->circle_points[0].id, "%-s", cid);
	}

	// Store circle to database when calculated and job is open (DCP9-style auto-save)
	if (m_pDataModel->circle_points[0].calc && m_iDisplay != SHAFT_DLG && m_pModel)
	{
		DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
			dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
		if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
		{
			char cidBuf[CIRCLE_ID_BUFF_LEN];
			sprintf(cidBuf, "%-s", m_pDataModel->circle_points[0].id);
			m_pDataModel->pCommon->strbtrim(cidBuf);
			std::string circleId(cidBuf);
			if (circleId.empty()) circleId = "Ci1";
			DCP::Database::CircleData cd;
			cd.type = "circle";
			cd.id = circleId;
			cd.center_x = m_pDataModel->cx;
			cd.center_y = m_pDataModel->cy;
			cd.center_z = m_pDataModel->cz;
			cd.normal_x = m_pDataModel->vi;
			cd.normal_y = m_pDataModel->vj;
			cd.normal_z = m_pDataModel->vk;
			cd.diameter = m_pDataModel->diameter;
			cd.toolRadius = m_pDataModel->bR;
			cd.calculated = true;
			cd.rms = m_pDataModel->rms_diameter;
			DCP::Database::CircleData existing;
			if (jdb->getCircle(circleId, existing))
				jdb->updateCircle(circleId, cd);
			else
				jdb->addCircle(circleId, cd);
			jdb->saveJob(m_pModel->m_currentJobId);
		}
	}

    // Remove the following statement if you don't want an exit
    // to the main menu
	if(m_pDataModel->circle_points[0].calc)
	{
		(void)Close(EC_KEY_CONT);
		(void)Close(EC_KEY_CONT);
	}
	else
	{
		(void)Close(EC_KEY_ESC);
		(void)Close(EC_KEY_ESC);
	}
}
// ================================================================================================
// Description: OnSHF2Pressed DELETE
// ================================================================================================
void DCP::CircleController::OnSHF2Pressed()
{
	m_pDataModel->delete_circle();
	if(m_iDisplay == SHAFT_DLG)
		m_pDataModel->PLANE_TYPE =  CIRCLE_POINTS_PLANE;

	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnSHF3Pressed DELETE
// ================================================================================================
void DCP::CircleController::OnSHF3Pressed()
{	/*
		DCP::ResCircleModel* pModel = new ResCircleModel;
		//memset(&pModel->circle_points[0]->points[0],0,sizeof(S_POINT_BUFF) * MAX_POINT_IN_CIRCLE);
		pModel->circle_points = &m_pDataModel->circle_points[0];
		if(GetController(RES_CIRCLE_CONTROLLER) == nullptr)
		{
			(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::ResCircleController(m_pModel) );
		}

		(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK);

		(void)GetController( RES_CIRCLE_CONTROLLER )->SetModel(pModel);
		SetActiveController(RES_CIRCLE_CONTROLLER, true);
	*/
	
	m_pDataModel->PLANE_TYPE = MEAS_PLANE;
	m_pDataModel->planes[0].points[0].x = 520; m_pDataModel->planes[0].points[0].y = 2711;m_pDataModel->planes[0].points[0].z = -422;m_pDataModel->planes[0].points[0].sta = 1;
	m_pDataModel->planes[0].points[1].x = 752; m_pDataModel->planes[0].points[1].y = 2650;m_pDataModel->planes[0].points[1].z = -241;m_pDataModel->planes[0].points[1].sta = 1;
	m_pDataModel->planes[0].points[2].x = 745; m_pDataModel->planes[0].points[2].y = 2626;m_pDataModel->planes[0].points[2].z = -408;m_pDataModel->planes[0].points[2].sta = 1;
	
	m_pDataModel->circle_points[0].points[0].x = 520; m_pDataModel->circle_points[0].points[0].y = 2711;m_pDataModel->circle_points[0].points[0].z = -422;m_pDataModel->circle_points[0].points[0].sta = 1;
	m_pDataModel->circle_points[0].points[1].x = 752; m_pDataModel->circle_points[0].points[1].y = 2650;m_pDataModel->circle_points[0].points[1].z = -241;m_pDataModel->circle_points[0].points[1].sta = 1;
	m_pDataModel->circle_points[0].points[2].x = 745; m_pDataModel->circle_points[0].points[2].y = 2626;m_pDataModel->circle_points[0].points[2].z = -408;m_pDataModel->circle_points[0].points[2].sta = 1;
	m_pDataModel->circle_points[0].points[3].x = 745; m_pDataModel->circle_points[0].points[3].y = 2626;m_pDataModel->circle_points[0].points[3].z = -408;m_pDataModel->circle_points[0].points[3].sta = 1;
	m_pDataModel->circle_points[0].points[4].x = 745; m_pDataModel->circle_points[0].points[4].y = 2626;m_pDataModel->circle_points[0].points[4].z = -408;m_pDataModel->circle_points[0].points[4].sta = 1;
			m_pDataModel->m_iCounts = m_pDataModel->pCommon->defined_circle_points(&m_pDataModel->circle_points[0],0);
	/*	
			CalcCircle circle(m_pDataModel->PLANE_TYPE, 
								&m_pDataModel->circle_points[0],
								&m_pDataModel->planes[0],
								&m_pDataModel->circle_points_in_plane[0],
								m_pDataModel->bR);
		
			if(circle.calc_center_of_circle())
			{
				circle.get_results(&m_pDataModel->cx, &m_pDataModel->cy,&m_pDataModel->cz, &m_pDataModel->diameter, &m_pDataModel->rms_diameter,&m_pDataModel->max_dev_in_point);
			}
	*/	
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::CircleController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::CircleController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDataModel->circle_points[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
	
		for(int i=0;i<MAX_POINTS_IN_CIRCLE;i++)
		{
			if(m_pDataModel->circle_points[0].points[i].sta == 3)
				m_pDataModel->circle_points[0].points[i].sta = 1;
			
			if(m_pDataModel->circle_points[0].points[i].sta == 4)
				m_pDataModel->circle_points[0].points[i].sta = 2;
		}
		
		if(GetController(CALC_CIRCLE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_CIRCLE_CONTROLLER, new DCP::CalcCircleontrollerC(m_pDataModel, m_iDisplay) );
		}

		(void)GetController(CALC_CIRCLE_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK));

		(void)GetController( CALC_CIRCLE_CONTROLLER )->SetModel(m_pModel);
		SetActiveController(CALC_CIRCLE_CONTROLLER, true);
		
		/*
		CalcCircle circle(m_pDataModel->PLANE_TYPE, 
								&m_pDataModel->circle_points[0],
								&m_pDataModel->planes[0],
								&m_pDataModel->circle_points_in_plane[0],
								m_pDataModel->bR);
		
		if(circle.calc_center_of_circle())
		{
			m_pDataModel->m_iCounts = m_pDataModel->pCommon->defined_circle_points(&m_pDataModel->circle_points[0],0);
			circle.get_results(&m_pDataModel->cx, &m_pDataModel->cy,&m_pDataModel->cz, &m_pDataModel->diameter, &m_pDataModel->rms_diameter,&m_pDataModel->max_dev_in_point);
			if(m_pDataModel->show_circle_points() == 1)
			{
				//m_pDataModel->circle_points[0].sta = 1;
				//m_pDataModel->circle_points[0].calc = 1;

				DCP::ResCircleModel* pModel = new ResCircleModel;
				pModel->circle_points = &m_pDataModel->circle_points[0];
				if(GetController(RES_CIRCLE_CONTROLLER) == nullptr)
				{
					(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::ResCircleController(m_pModel) );
				}

				(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK);

				(void)GetController( RES_CIRCLE_CONTROLLER )->SetModel(pModel);
				SetActiveController(RES_CIRCLE_CONTROLLER, true);
			}
		}
		*/
	}
	if(lCtrlID == CALC_CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{	
		OnF6Pressed	();
	}

	// CIRCLE PLANE MEAS
	if(lCtrlID == CIRCLE_PLANE_MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( CIRCLE_PLANE_MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDataModel->planes[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
		m_pDataModel->PLANE_TYPE = MEAS_PLANE;

	}
	m_pDlg->RefreshControls();
	//m_pDlg->Show();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: ConfirmClose
// ================================================================================================
bool DCP::CircleController::ConfirmClose(bool bEsc)
{
	if(bEsc && PLANE_KEYS != 0)
	{
		PLANE_KEYS = 0;
		set_function_keys();
		return false;
	}
	else
	{
		return true;
	}
}
/*
bool DCP::CircleController::Close(int lExitCode, bool bConfirmation)
{
	return true;
}
*/
// LOCAL
// ================================================================================================
// ====================================  CircleModel  ===================================
// ================================================================================================


// ===========================================================================================
// Description: Constructor
// ===========================================================================================
DCP::CircleModel::CircleModel(Model* pModel):m_pModel(pModel)
{
	cx = 0.0;cy=0.0;cz=0.0, diameter=0.0,bR=0.0;
	PLANE_TYPE = CIRCLE_POINTS_PLANE;
	
	pCommon = new Common(pModel);
	pMsgBox = new MsgBox;

	memset(&temp_plane_table[0],0, sizeof(S_PLANE_BUFF));
	memset(&planes[0],0, sizeof(S_PLANE_BUFF));
	memset(&circle_points[0],0,sizeof(S_CIRCLE_BUFF));
	memset(&circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
}

// ===========================================================================================
// Description: Destructor
// ===========================================================================================
DCP::CircleModel::~CircleModel()
{
	if(pCommon)
	{
		delete pCommon;
		pCommon = 0;
	}

	if(pMsgBox)
	{
		delete pMsgBox;
		pMsgBox = 0;
	}
}


// ================================================================================================
// Description: clear_circle - clear without confirmation (for NEW)
// ================================================================================================
void DCP::CircleModel::clear_circle()
{
	cx = 0.0; cy = 0.0; cz = 0.0; diameter = 0.0;
	PLANE_TYPE = CIRCLE_POINTS_PLANE;
	memset(&temp_plane_table[0], 0, sizeof(S_PLANE_BUFF));
	memset(&planes[0], 0, sizeof(S_PLANE_BUFF));
	memset(&circle_points[0], 0, sizeof(S_CIRCLE_BUFF));
	memset(&circle_points_in_plane[0], 0, sizeof(S_CIRCLE_BUFF));
}

// ================================================================================================
// Description: delete_circle
// ================================================================================================
void DCP::CircleModel::delete_circle()
{
	StringC msg;
	StringC strCircleText;
	strCircleText.LoadTxt(AT_DCP06,L_DCP_CIRCLE_TEXT_TOK);
	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_TOK);
	msg.Format(msg,(const wchar_t*)strCircleText);
	if(pMsgBox->ShowMessageYesNo(msg))
		clear_circle();
}




// *************************************************************************************************
//
//**************************************************************************************************
short DCP::CircleModel::show_circle_points()
{
	StringC sMsg;
	StringC sX(L" ");
	StringC sY;
	StringC sZ;
	StringC sRadius;
	StringC sDiameter;
	StringC sRMS;
	StringC sCenterPoint;
	char cX[40], cY[40], cZ[40], cRadius[40]/*, cDiameter[40]*/,cRms[40];
	char vI[40], vJ[40], vK[40];
    //wchar_t wsTemp[50];

	StringC sTemp(L" ");
	
	sCenterPoint.LoadTxt(AT_DCP06, P_DCP_CENTER_OF_CIRCLE_TOK);

	sTemp.LoadTxt(AT_DCP06, P_DCP_X_TOK);
	sX.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP06, P_DCP_Y_TOK);
	sY.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP06, P_DCP_Z_TOK);
	sZ.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP06, P_DCP_RADIUS_TOK);
	sRadius.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP06, P_DCP_DIAMETER_TOK);
	sDiameter.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP06, P_DCP_RMS_TOK);
	sRMS.Format(L"%11s", (const wchar_t*) sTemp);

	/*
	sX.LoadTxt(AT_DCP06, P_DCP_X_TOK);
	sY.LoadTxt(AT_DCP06, P_DCP_Y_TOK);
	sZ.LoadTxt(AT_DCP06, P_DCP_Z_TOK);
	sRadius.LoadTxt(AT_DCP06, P_DCP_RADIUS_TOK);
	sDiameter.LoadTxt(AT_DCP06, P_DCP_DIAMETER_TOK);
	sRMS.LoadTxt(AT_DCP06, P_DCP_RMS_TOK);
	*/
	sprintf(cX,"%9.*f", m_pModel->m_nDecimals,cx);
	sprintf(cY,"%9.*f", m_pModel->m_nDecimals,cy);
	sprintf(cZ,"%9.*f", m_pModel->m_nDecimals,cz);
	sprintf(cRadius,"%9.*f", m_pModel->m_nDecimals,diameter);
	//sprintf(cDiameter,"%9.*f", m_pModel->m_nDecimals, diameter *2);
	sprintf(vI,"%9.*f", 6 /*m_pModel->m_nDecimals*/,vi);
	sprintf(vJ,"%9.*f", 6 /*m_pModel->m_nDecimals*/,vj);
	sprintf(vK,"%9.*f", 6 /*m_pModel->m_nDecimals*/,vk);
	sprintf(cRms,"%9.*f", m_pModel->m_nDecimals,rms_diameter);

	
	sMsg = sCenterPoint;
	sMsg += L"\n\n";
	sMsg += sX;
	sMsg += L":";
	sMsg += StringC(cX);
	sMsg += L"\n";
	sMsg += sY;
	sMsg += L":";
	sMsg += StringC(cY);
	sMsg += L"\n";
	sMsg += sZ;
	sMsg += L":";
	sMsg += StringC(cZ);
	sMsg += L"\n";
	sMsg += sRadius;
	sMsg += L":";
	sMsg += StringC(cRadius);
	sMsg += L"\n";
	sMsg += L"Normal";
	sMsg += L"\n";
	sMsg += L"i:";
	sMsg += StringC(vI);
	sMsg += L"\n";
	sMsg += L"j:";
	sMsg += StringC(vJ);
	sMsg += L"\n";
	sMsg += L"k:";
	sMsg += StringC(vK);
	sMsg += L"\n";

	if(m_iCounts > 3)
	{
		sMsg += sRMS;
		sMsg += L":";
		sMsg += StringC(cRms);
	}

	if(pMsgBox->ShowMessageOkAbort(sMsg))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


