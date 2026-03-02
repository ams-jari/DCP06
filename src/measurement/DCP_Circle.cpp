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

#include "calc.h"
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/init/DCP_DCP05Init.hpp>
#include <dcp06/core/DCP_DCP05Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/calculation/DCP_CalcPlane.hpp>
#include <dcp06/calculation/DCP_CalcCircle.hpp>
#include <dcp06/calculation/DCP_ResCircle.hpp>
#include <dcp06/calculation/DCP_CalcCircleController.hpp>

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
OBS_IMPLEMENT_EXECUTE(DCP::DCP05CircleDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP05CircleDlgC::DCP05CircleDlgC(DCP::DCP05ModelC * pDCP05Model, DCP05CircleModelC* pCircleModel, short iDisplay):
		GUI::ModelHandlerC(),
		GUI::StandardDialogC(),m_pDCP05Model(pDCP05Model),m_pDefinePlaneInfo(0),m_pPlane(0),m_iDisplay(iDisplay),
		m_pToolRadius(0),m_pMeasureCirclePoints(0),m_pCirclePoints(0),m_pDataModel(pCircleModel),
		m_pToolRadiusObserver(OBS_METHOD_TO_PARAM0(DCP05CircleDlgC, OnValueChanged), this)
{
	
	//SetTxtApplicationId(AT_DCP05);

	// load texts...
	sXY_plane.LoadTxt(AT_DCP05, V_DCP_XY_PLANE_TOK);
	sZX_plane.LoadTxt(AT_DCP05, V_DCP_ZX_PLANE_TOK);
	sYZ_plane.LoadTxt(AT_DCP05, V_DCP_YZ_PLANE_TOK); 
	sCIRCLEPOINTS_plane.LoadTxt(AT_DCP05, V_DCP_CIRCLE_POINTS_TOK);
	sMeasPlane.LoadTxt(AT_DCP05,V_DCP_MEASURED_CIRCLE_TOK);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP05CircleDlgC::~DCP05CircleDlgC()
{
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::DCP05CircleDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog
	if(m_iDisplay != SHAFT_DLG)
	{
	
		m_pDefinePlaneInfo = new GUI::TextCtrlC();
		m_pDefinePlaneInfo->SetId(eDefinePlaneInfo);
		m_pDefinePlaneInfo->SetText(StringC(AT_DCP05,L_DCP_DEFINE_CIRCLE_PLANE_TOK));
		AddCtrl(m_pDefinePlaneInfo);

		m_pPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
		m_pPlane->SetId(ePlane);
		m_pPlane->SetText(StringC(AT_DCP05,P_DCP_CIRCLE_PLANE_TOK));
		m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
		m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
		//m_pPlane->SetAutoColon(false);
		//m_pPlane->SetColonPosition(9*22);
		// m_pPlane->SetTextWidth(9*40); CAPTIVATE
		
		AddCtrl(m_pPlane);
	}

	m_pToolRadius = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pToolRadius->SetId(eToolRadius);
	m_pToolRadius->SetText(StringC(AT_DCP05,P_DCP_TOOL_RADIUS_TOK));
	m_pToolRadius->GetFloatInputCtrl()->SetDecimalPlaces((unsigned char) m_pDCP05Model->m_nDecimals);
	//m_pToolRadius->SetAutoColon(false);
	//m_pToolRadius->SetColonPosition(9*22);
	// m_pToolRadius->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pToolRadius);
		

	m_pMeasureCirclePoints = new GUI::TextCtrlC();
	m_pMeasureCirclePoints->SetId(eMeasureCirclePoints);
	//if(m_iDisplay != SHAFT_DLG)
		m_pMeasureCirclePoints->SetText(StringC(AT_DCP05,L_DCP_MEASURE_CIRCLE_POINTS_TOK));
	//else
	//	m_pMeasureCirclePoints->SetTextTok(L_DCP_MEASURE_SHAFT_POINTS_TOK);
	AddCtrl(m_pMeasureCirclePoints);

	m_pCirclePoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCirclePoints->SetId(eCirclePoints);
	
	//if(m_iDisplay != SHAFT_DLG)
		m_pCirclePoints->SetText(StringC(AT_DCP05,P_DCP_CIRCLE_POINTS_TOK));
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

}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::DCP05CircleDlgC::OnDialogActivated()
{
	
	


	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::DCP05CircleDlgC::UpdateData()
{
	// copy values from DCP05MOdel
	if(m_iDisplay != SHAFT_DLG)
	{

		memcpy(&m_pDCP05Model->circle_planes[0],&m_pDataModel->planes[0],  sizeof(S_PLANE_BUFF));
		memcpy(&m_pDCP05Model->circle_points[0],&m_pDataModel->circle_points[0],  sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDCP05Model->circle_points_in_plane[0], &m_pDataModel->circle_points_in_plane[0],  sizeof(S_CIRCLE_BUFF));
		m_pDCP05Model->circle_plane_type = m_pDataModel->PLANE_TYPE;
		m_pDCP05Model->circle_cx = m_pDataModel->cx;
		m_pDCP05Model->circle_cy = m_pDataModel->cy;
		m_pDCP05Model->circle_cz = m_pDataModel->cz;
		m_pDCP05Model->circle_vi = m_pDataModel->vi;
		m_pDCP05Model->circle_vj = m_pDataModel->vj;
		m_pDCP05Model->circle_vk = m_pDataModel->vk;

		m_pDCP05Model->circle_diameter = m_pDataModel->diameter;

		m_pDCP05Model->poConfigController->GetModel()->SetConfigKey(CNF_KEY_CIRCLE);
		m_pDCP05Model->poConfigController->StoreConfigData();
		m_pDCP05Model->circle_rms = m_pDataModel->rms_diameter;

	}
	else
	{
		//memcpy(&m_pDCP05Model->circle_planes[0],&m_pDataModel->planes[0],  sizeof(S_PLANE_BUFF));
		memcpy(&m_pDCP05Model->shaft_circle_points[0],&m_pDataModel->circle_points[0],  sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDCP05Model->shaft_circle_points_in_plane[0], &m_pDataModel->circle_points_in_plane[0],  sizeof(S_CIRCLE_BUFF));
		m_pDCP05Model->shaft_circle_plane_type = m_pDataModel->PLANE_TYPE;
		m_pDCP05Model->shaft_circle_cx = m_pDataModel->cx;
		m_pDCP05Model->shaft_circle_cy = m_pDataModel->cy;
		m_pDCP05Model->shaft_circle_cz = m_pDataModel->cz;
		m_pDCP05Model->shaft_circle_vi = m_pDataModel->vi;
		m_pDCP05Model->shaft_circle_vj = m_pDataModel->vj;
		m_pDCP05Model->shaft_circle_vk = m_pDataModel->vk;

		m_pDCP05Model->shaft_circle_diameter = m_pDataModel->diameter;
		m_pDCP05Model->shaft_circle_rms = m_pDataModel->rms_diameter;

		m_pDCP05Model->poConfigController->GetModel()->SetConfigKey(CNF_KEY_SHAFT);
		m_pDCP05Model->poConfigController->StoreConfigData();
	}

}
// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::DCP05CircleDlgC::OnValueChanged( int unNotifyCode,  int ulParam2)
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

void DCP::DCP05CircleDlgC::RefreshControls()
{
	if(m_pDefinePlaneInfo && m_pPlane && m_pToolRadius && m_pMeasureCirclePoints &&  m_pCirclePoints)
	{	
		StringC sTemp;
	
		if(m_iDisplay != SHAFT_DLG)
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
// Description: only accept hello world Model objects
// ================================================================================================
bool DCP::DCP05CircleDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05PointBuffModelC* pDCP05Model = dynamic_cast< DCP::DCP05PointBuffModelC* >( pModel );

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

// ================================================================================================
// Description: only accept DCP05PointBuffModelC objects
// ================================================================================================
DCP::DCP05PointBuffModelC* DCP::DCP05CircleDlgC::GetPointBuffModelModel() const
{
    return (DCP::DCP05PointBuffModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  DCP05CircleControllerC  ===================================
// ================================================================================================

// ================================================================================================
// Description: constructor
// ================================================================================================
DCP::DCP05CircleControllerC::DCP05CircleControllerC(DCP::DCP05ModelC *pDCP05Model, short iDisplay)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model),m_pDataModel(0), m_iDisplay(iDisplay)

{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    if(iDisplay != SHAFT_DLG)
		SetTitle(StringC( AT_DCP05, T_DCP_CIRCLE_TOK ));
	else
		SetTitle(StringC( AT_DCP05, T_DCP_SHAFT_ALIGMENT_CIRCLE_TOK ));
	

	// create local data
	m_pDataModel = new DCP05CircleModelC(pDCP05Model);

	// copy values from DCP05MOdel
	if(m_iDisplay != SHAFT_DLG)
	{

		memcpy(&m_pDataModel->planes[0], &pDCP05Model->circle_planes[0], sizeof(S_PLANE_BUFF));
		memcpy(&m_pDataModel->circle_points[0], &pDCP05Model->circle_points[0], sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDataModel->circle_points_in_plane[0], &pDCP05Model->circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));

		m_pDataModel->PLANE_TYPE = pDCP05Model->circle_plane_type;

	}
	else
	{
		memcpy(&m_pDataModel->circle_points[0], &pDCP05Model->shaft_circle_points[0], sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pDataModel->circle_points_in_plane[0], &pDCP05Model->shaft_circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));
		m_pDataModel->PLANE_TYPE = CIRCLE_POINTS_PLANE;
	}

	// Create a dialog
    m_pDlg = new DCP::DCP05CircleDlgC(pDCP05Model, m_pDataModel, m_iDisplay);  //lint !e1524 new in constructor for class 

    (void)AddDialog( CIRCLE_DLG, m_pDlg, true );

	PLANE_KEYS = 0;

	// set functionkeys
	set_function_keys();
    // Set the function key
	
 


} //lint !e818 Pointer parameter could be declared as pointing to const

// ================================================================================================
// Description: destructor
// ================================================================================================
DCP::DCP05CircleControllerC::~DCP05CircleControllerC()
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
void DCP::DCP05CircleControllerC::set_function_keys()
{
	ResetFunctionKeys();

	if(PLANE_KEYS == 0)
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		
		if(m_iDisplay != SHAFT_DLG)
			vDef.strLable = StringC(AT_DCP05,K_DCP_PLANE_TOK);

		SetFunctionKey( FK1, vDef );

		//if(m_iDisplay != SHAFT_DLG)
			vDef.strLable = StringC(AT_DCP05,K_DCP_CIRCLE_TOK);
		//else
		//	vDef.nLable = K_DCP_SHAFT_TOK;
		SetFunctionKey( FK5, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
		SetFunctionKey( FK6, vDef );


		// SHIFT
		vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
		SetFunctionKey( SHFK2, vDef );
		
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP05;
		vDef1.poOwner = this;
		/*
		vDef1.strLable = L"Test";
		SetFunctionKey( SHFK3, vDef1 );
		*/
	}
	else
	{
		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_XY_PLANE_TOK);
		SetFunctionKey( FK1, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_ZX_PLANE_TOK);
		SetFunctionKey( FK2, vDef );
		
		vDef.strLable = StringC(AT_DCP05,K_DCP_YZ_PLANE_TOK);
		SetFunctionKey( FK3, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_MEAS_TOK);
		SetFunctionKey( FK4, vDef );

		vDef.strLable = StringC(AT_DCP05,K_DCP_CIRCLE_PLANE_POINTS_TOK);
		SetFunctionKey( FK5, vDef );

	}
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
	vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );	

	GUI::DesktopC::Instance()->UpdateFunctionKeys();

}
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP05CircleControllerC::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description: OnF1Pressed PLANE
// ================================================================================================
void DCP::DCP05CircleControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{
		PLANE_KEYS = 1;
		set_function_keys();
	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		
		if(m_pDCP05Model->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP05,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = XY_PLANE;
	}
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnF2Pressed XY-PLANE
// ================================================================================================
void DCP::DCP05CircleControllerC::OnF2Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{

	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		if(m_pDCP05Model->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP05,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = ZX_PLANE;
	
	}
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnF3Pressed ZX-PLANE
// ================================================================================================
void DCP::DCP05CircleControllerC::OnF3Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(PLANE_KEYS == 0)
	{

	}
	else
	{
		PLANE_KEYS = 0;
		set_function_keys();
		if(m_pDCP05Model->active_coodinate_system == DCS)
		{
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP05,M_DCP_CANNOT_USE_XY_YZ_ZX_IN_SCS_TOK);
			m_pDataModel->pMsgBox->ShowMessageOk(sMsg);
		}
		else
			m_pDataModel->PLANE_TYPE = YZ_PLANE;
	
	}
	m_pDlg->RefreshControls();

}

// ================================================================================================
// Description: OnF4Pressed YZ-PLANE
// ================================================================================================
void DCP::DCP05CircleControllerC::OnF4Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(PLANE_KEYS == 1)
	{
		// mitataan taso
		DCP::DCP05MeasModelC* pModel = new DCP05MeasModelC;
		pModel->m_iPointsCount = m_pDataModel->pCommon->points_count_in_plane(&m_pDataModel->planes[0]);
		if(pModel->m_iPointsCount < 3) pModel->m_iPointsCount = 3;
		pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
		pModel->m_iMinPoint = 3;
	
		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
		memcpy(&pModel->point_table[0],&m_pDataModel->planes[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);

	
		if(GetController(CIRCLE_PLANE_MEAS_CONTROLLER) == NULL)
		{
			(void)AddController( CIRCLE_PLANE_MEAS_CONTROLLER, new DCP::DCP05MeasControllerC(m_pDCP05Model) );
		}

		(void)GetController(CIRCLE_PLANE_MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_CIRCLE_PLANE_MEAS_TOK));

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

void DCP::DCP05CircleControllerC::OnF5Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(PLANE_KEYS == 0)
	{
		// measure circle points
		DCP::DCP05MeasModelC* pModel = new DCP05MeasModelC;
		pModel->m_iPointsCount = m_pDataModel->pCommon->get_max_defined_point_circle(&m_pDataModel->circle_points[0]);
		if(pModel->m_iPointsCount < 3) pModel->m_iPointsCount = 3;
		pModel->m_iMaxPoint = MAX_POINTS_IN_CIRCLE;
		pModel->m_iMinPoint = 3;
	
		memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
		memcpy(&pModel->point_table[0],&m_pDataModel->circle_points[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);

	
		if(GetController(MEAS_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::DCP05MeasControllerC(m_pDCP05Model) );
		}

		if(m_iDisplay != SHAFT_DLG)
			(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_CIRCLE_MEAS_TOK));
		else
			(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_SHAFT_ALIGMENT_CIRCLE_MEAS_TOK));

		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);


	}
	else
	{	
		m_pDataModel->PLANE_TYPE = CIRCLE_POINTS_PLANE;
		PLANE_KEYS = 0;
		set_function_keys();
	}
	m_pDlg->RefreshControls();
}


// ================================================================================================
// Description: F6, CONT
// ================================================================================================
void DCP::DCP05CircleControllerC::OnF6Pressed()
{
    if (m_pDlg == NULL)
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
void DCP::DCP05CircleControllerC::OnSHF2Pressed()
{
	m_pDataModel->delete_circle();
	if(m_iDisplay == SHAFT_DLG)
		m_pDataModel->PLANE_TYPE =  CIRCLE_POINTS_PLANE;

	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnSHF3Pressed DELETE
// ================================================================================================
void DCP::DCP05CircleControllerC::OnSHF3Pressed()
{	/*
		DCP::DCP05ResCircleModelC* pModel = new DCP05ResCircleModelC;
		//memset(&pModel->circle_points[0]->points[0],0,sizeof(S_POINT_BUFF) * MAX_POINT_IN_CIRCLE);
		pModel->circle_points = &m_pDataModel->circle_points[0];
		if(GetController(RES_CIRCLE_CONTROLLER) == NULL)
		{
			(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::DCP05ResCircleControllerC(m_pDCP05Model) );
		}

		(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_LINE_MEAS_TOK);

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
			DCP05CalcCircleC circle(m_pDataModel->PLANE_TYPE, 
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
void DCP::DCP05CircleControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}
// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP05CircleControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05MeasModelC* pModel = (DCP::DCP05MeasModelC*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDataModel->circle_points[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_CIRCLE);
	
		for(int i=0;i<MAX_POINTS_IN_CIRCLE;i++)
		{
			if(m_pDataModel->circle_points[0].points[i].sta == 3)
				m_pDataModel->circle_points[0].points[i].sta = 1;
			
			if(m_pDataModel->circle_points[0].points[i].sta == 4)
				m_pDataModel->circle_points[0].points[i].sta = 2;
		}
		
		if(GetController(CALC_CIRCLE_CONTROLLER) == NULL)
		{
			(void)AddController( CALC_CIRCLE_CONTROLLER, new DCP::DCP05CalcCircleControllerC(m_pDataModel, m_iDisplay) );
		}

		(void)GetController(CALC_CIRCLE_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_DOM_LINE_MEAS_TOK));

		(void)GetController( CALC_CIRCLE_CONTROLLER )->SetModel(m_pDCP05Model);
		SetActiveController(CALC_CIRCLE_CONTROLLER, true);
		
		/*
		DCP05CalcCircleC circle(m_pDataModel->PLANE_TYPE, 
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

				DCP::DCP05ResCircleModelC* pModel = new DCP05ResCircleModelC;
				pModel->circle_points = &m_pDataModel->circle_points[0];
				if(GetController(RES_CIRCLE_CONTROLLER) == NULL)
				{
					(void)AddController( RES_CIRCLE_CONTROLLER, new DCP::DCP05ResCircleControllerC(m_pDCP05Model) );
				}

				(void)GetController(RES_CIRCLE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_LINE_MEAS_TOK);

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
		DCP::DCP05MeasModelC* pModel = (DCP::DCP05MeasModelC*) GetController( CIRCLE_PLANE_MEAS_CONTROLLER )->GetModel();		
		
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
bool DCP::DCP05CircleControllerC::ConfirmClose(bool bEsc)
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
bool DCP::DCP05CircleControllerC::Close(int lExitCode, bool bConfirmation)
{
	return true;
}
*/
// LOCAL
// ================================================================================================
// ====================================  DCP05CircleModelC  ===================================
// ================================================================================================


// ===========================================================================================
// Description: Constructor
// ===========================================================================================
DCP::DCP05CircleModelC::DCP05CircleModelC(DCP05ModelC* pDCP05Model):m_pDCP05Model(pDCP05Model)
{
	cx = 0.0;cy=0.0;cz=0.0, diameter=0.0,bR=0.0;
	PLANE_TYPE = CIRCLE_POINTS_PLANE;
	
	pCommon = new DCP05CommonC(pDCP05Model);
	pMsgBox = new DCP05MsgBoxC;

	memset(&temp_plane_table[0],0, sizeof(S_PLANE_BUFF));
	memset(&planes[0],0, sizeof(S_PLANE_BUFF));
	memset(&circle_points[0],0,sizeof(S_CIRCLE_BUFF));
	memset(&circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
}

// ===========================================================================================
// Description: Destructor
// ===========================================================================================
DCP::DCP05CircleModelC::~DCP05CircleModelC()
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
// Description: delete_circle
// ================================================================================================
void DCP::DCP05CircleModelC::delete_circle()
{
	StringC msg;
	StringC strActivePoint(L"");
	StringC strCircleText;
	strCircleText.LoadTxt(AT_DCP05,L_DCP_CIRCLE_TEXT_TOK);

	msg.LoadTxt(AT_DCP05,M_DCP_DELETE_ALL_TOK);
	msg.Format(msg,(const wchar_t*)strCircleText);
	if(pMsgBox->ShowMessageYesNo(msg))
	{
		cx = 0.0;cy=0.0;cz=0.0, diameter=0.0;
		PLANE_TYPE = CIRCLE_POINTS_PLANE;

		memset(&temp_plane_table[0],0, sizeof(S_PLANE_BUFF));
		memset(&planes[0],0, sizeof(S_PLANE_BUFF));
		memset(&circle_points[0],0,sizeof(S_CIRCLE_BUFF));
		memset(&circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
	}
}




// *************************************************************************************************
//
//**************************************************************************************************
short DCP::DCP05CircleModelC::show_circle_points()
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
	
	sCenterPoint.LoadTxt(AT_DCP05, P_DCP_CENTER_OF_CIRCLE_TOK);

	sTemp.LoadTxt(AT_DCP05, P_DCP_X_TOK);
	sX.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP05, P_DCP_Y_TOK);
	sY.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP05, P_DCP_Z_TOK);
	sZ.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP05, P_DCP_RADIUS_TOK);
	sRadius.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP05, P_DCP_DIAMETER_TOK);
	sDiameter.Format(L"%11s", (const wchar_t*) sTemp);

	sTemp.LoadTxt(AT_DCP05, P_DCP_RMS_TOK);
	sRMS.Format(L"%11s", (const wchar_t*) sTemp);

	/*
	sX.LoadTxt(AT_DCP05, P_DCP_X_TOK);
	sY.LoadTxt(AT_DCP05, P_DCP_Y_TOK);
	sZ.LoadTxt(AT_DCP05, P_DCP_Z_TOK);
	sRadius.LoadTxt(AT_DCP05, P_DCP_RADIUS_TOK);
	sDiameter.LoadTxt(AT_DCP05, P_DCP_DIAMETER_TOK);
	sRMS.LoadTxt(AT_DCP05, P_DCP_RMS_TOK);
	*/
	sprintf(cX,"%9.*f", m_pDCP05Model->m_nDecimals,cx);
	sprintf(cY,"%9.*f", m_pDCP05Model->m_nDecimals,cy);
	sprintf(cZ,"%9.*f", m_pDCP05Model->m_nDecimals,cz);
	sprintf(cRadius,"%9.*f", m_pDCP05Model->m_nDecimals,diameter);
	//sprintf(cDiameter,"%9.*f", m_pDCP05Model->m_nDecimals, diameter *2);
	sprintf(vI,"%9.*f", 6 /*m_pDCP05Model->m_nDecimals*/,vi);
	sprintf(vJ,"%9.*f", 6 /*m_pDCP05Model->m_nDecimals*/,vj);
	sprintf(vK,"%9.*f", 6 /*m_pDCP05Model->m_nDecimals*/,vk);
	sprintf(cRms,"%9.*f", m_pDCP05Model->m_nDecimals,rms_diameter);

	
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


