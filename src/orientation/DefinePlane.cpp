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
#include <dcp06/orientation/DefinePlane.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/orientation/ResPlane.hpp>

#include <math.h>
#include "calc.h"

#include <TBL_Measurement.hpp>
//#include <TBL_Util.hpp>
#include <TBL_Util.hpp>
#include <TPI_MeasConfig.hpp>

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

DCP::DefinePlaneDialog::DefinePlaneDialog(DCP::Model* pModel):m_pPlane(0),
						m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);

	m_strXYPlane.LoadTxt(AT_DCP06,V_DCP_XY_PLANE_TOK);
	m_strZXPlane.LoadTxt(AT_DCP06,V_DCP_ZX_PLANE_TOK);
	m_strYZPlane.LoadTxt(AT_DCP06,V_DCP_YZ_PLANE_TOK);
	m_sHZText.LoadTxt(AT_DCP06,V_DCP_HZ_TOK);


}


            // Description: Destructor
DCP::DefinePlaneDialog::~DefinePlaneDialog()
{

}

void DCP::DefinePlaneDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();
	
	m_pPlane = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPlane->SetId(ePlane);
	m_pPlane->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_TOK));
	// m_pPlane->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	m_pPlane->GetStringInputCtrl()->SetString(L"XY-");
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPlane->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pPlane->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pPlane);
	
	//InsertEmptyLine(unHeight);

	//m_pPlaneInfo1 = new GUI::TextCtrlC(50,50,100);CAPTIVATE
	m_pPlaneInfo1 = new GUI::TextCtrlC();
	m_pPlaneInfo1->SetId(ePlaneInfo1);
	
	//m_pPlaneInfo1->GetPosition(xx,yy);
	//m_pPlaneInfo1->SetPosition(xx+10,yy);
	m_pPlaneInfo1->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_EXP1_TOK));
	/*
	m_pPlaneInfo1->GetTextCtrl()->SetTextTok(P_DCP_REF_PLANE_EXP1_TOK);
	void(m_pPlaneInfo1->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPlaneInfo1->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	*/
	AddCtrl(m_pPlaneInfo1);

	m_pPlaneInfo2 = new GUI::TextCtrlC();
	m_pPlaneInfo2->SetId(ePlaneInfo2);
	m_pPlaneInfo2->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_EXP2_TOK));
	AddCtrl(m_pPlaneInfo2);
	
	m_pPlaneInfo3 = new GUI::TextCtrlC();
	m_pPlaneInfo3->SetId(ePlaneInfo3);
	m_pPlaneInfo3->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_EXP3_TOK));
	AddCtrl(m_pPlaneInfo3);
	
	m_pPlaneInfo4 = new GUI::TextCtrlC();
	m_pPlaneInfo4->SetId(ePlaneInfo4);
	m_pPlaneInfo4->SetText(StringC(AT_DCP06,P_DCP_REF_PLANE_EXP4_TOK));
	AddCtrl(m_pPlaneInfo4);
	
	//SetHelpTok(H_DCP_DOM_PLANE_TOK,0);
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DefinePlaneDialog::OnDialogActivated()
{
	RefreshControls();
}

// Description: refresh all controls
void DCP::DefinePlaneDialog::RefreshControls()
{	
	if(m_pPlane)
	{
		StringC sTemp;
		StringC sStatus(L"-");
		
		if(GetDataModel()->hz_plane)
		{
			if(GetDataModel()->hz_plane_buff[0].calc)
				sStatus = L"+";
		}
		else
		{
			if(GetDataModel()->plane_buff[0].calc)
				sStatus = L"+";
		}

		if(GetDataModel()->active_plane	== XY_PLANE)
			sTemp = m_strXYPlane;
		else if(GetDataModel()->active_plane	== ZX_PLANE)
			sTemp = m_strZXPlane;	
		else
			sTemp = m_strYZPlane;	

		if(GetDataModel()->hz_plane)
		{
			sTemp += L"(";
			sTemp += m_sHZText;
			sTemp += L")";
		}
		sTemp += sStatus;
		m_pPlane->GetStringInputCtrl()->SetString(sTemp);
	}
}

void DCP::DefinePlaneDialog::UpdateData()
{
}


// Description: only accept DCP06 Model objects
bool DCP::DefinePlaneDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DefinePlaneModel* pDcpModel = dynamic_cast< DCP::DefinePlaneModel* >( pModel );

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
DCP::DefinePlaneModel* DCP::DefinePlaneDialog::GetDataModel() const
{
    return (DCP::DefinePlaneModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DefinePlaneDialog::xy_plane()
{
	GetDataModel()->active_plane = XY_PLANE;
	RefreshControls();
	
}

// ================================================================================================
void DCP::DefinePlaneDialog::zx_plane()
{
	GetDataModel()->active_plane = ZX_PLANE;
	RefreshControls();
}

// ================================================================================================
void DCP::DefinePlaneDialog::yz_plane()
{
	GetDataModel()->active_plane = YZ_PLANE;
	RefreshControls();
}

// ================================================================================================
void DCP::DefinePlaneDialog::delete_plane()
{
	MsgBox MsgBox;
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_PLANE_TOK);
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->active_plane	= XY_PLANE;
		//m_pDomModel->dom_active_line	= pModel->active_line; 
		GetDataModel()->hz_plane		= false;
		memset(&GetDataModel()->plane_buff[0], 0, sizeof(S_PLANE_BUFF));
		memset(&GetDataModel()->hz_plane_buff[0], 0, sizeof(S_PLANE_BUFF));
		RefreshControls();
	}
}

// ================================================================================================
short DCP::DefinePlaneDialog::hz_plane()
{
	if(GetDataModel()->hz_plane)
	{
		GetDataModel()->hz_plane = false;		
		RefreshControls();
	}
	else
	{
		GetDataModel()->hz_plane = true;		
		if(set_hz_plane1(ACTUAL))
			return true;
	}
	return false;
}
// ================================================================================================
bool DCP::DefinePlaneDialog::CalculatePlaneAfterMeas()
{
	for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
	{
		if(GetDataModel()->plane_buff[0].points[i].sta == POINT_DESIGN_REJECTED)
			GetDataModel()->plane_buff[0].points[i].sta = POINT_DESIGN;

		if(GetDataModel()->plane_buff[0].points[i].sta == POINT_MEASURED_REJECTED)
			GetDataModel()->plane_buff[0].points[i].sta = POINT_MEASURED;

	}
	if(GetDataModel()->display == DOM_DLG)
		GetDataModel()->hz_plane = false;

	if(calc_plane(&GetDataModel()->plane_buff[0],ACTUAL))
	{
		// SHOW RESIDUALS
			return true;
	}

	return false;
}

// ================================================================================================
short DCP::DefinePlaneDialog::set_hz_plane1(short actualdesign)
{
short dist_count,ret;
//RC_TYPE tmcResult;
//TMC_ANGLE bAngle;

	ret = false;
	
	Common common(m_pModel);
	MsgBox msgbox;

	dist_count = common.points_count_in_plane(&GetDataModel()->hz_plane_buff[0]);
	
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	//3.x
	//bool bComp = oMeasConfig.GetCompensatorMode();
	bool bComp = oMeasConfig.IsCompensatorEnabled();

	//if(get_compensator_onoff() == FALSE)
	if(!bComp)
	{
		//3.x
		//oMeasConfig.SetCompensatorMode(true);
		oMeasConfig.SetCompensatorEnabled(true);
		oMeasConfig.Activate();

		//if(set_compensator_onoff(TRUE) == TRUE)
		{
								
		}
	}

	bool ret1 =TBL::CheckCompensator();
	if(!ret1)
	{
		//msgbox.ShowMessageOk(L"Not levelled!");	
		return false;
	}
//	bool ret1 =TBL::CheckCompensator();
	/*
	tmcResult = TMC_GetAngle(bAngle);
	(void) TMC_DoMeasure(TMC_CLEAR) ;	
	if(tmcResult != RC_OK)
	{
		msgbox(TXT_NIL_TOKEN, M_CHECK_LEVELING_TOK, MB_OK);
	}

	tmcResult = TMC_GetAngle(bAngle);
	(void) TMC_DoMeasure(TMC_CLEAR) ;	
	if (tmcResult !=RC_OK)
	{
		return FALSE;
	}
	*/
	if((/*GetDataModel()->display == DIST_DSP ||*/ GetDataModel()->display == CALC_DIST_DLG ) && dist_count  == 0 )
	{
		StringC sMsg;
		sMsg.LoadTxt(AT_DCP06,M_DCP_DEFINE_1_POINT_TOK);
		msgbox.ShowMessageOk(sMsg);
		goto bbb;
	}
	else
	{
	}

	if(set_horizontal_plane(/*&GetDataModel()->hz_plane_buff[0], display, plane_type*/))
	{
		if(calc_plane(&GetDataModel()->hz_plane_buff[0],actualdesign))// == true)
		{
			GetDataModel()->hz_plane_buff[0].calc = 1;
			GetDataModel()->hz_plane_buff[0].sta = PLANE_DEFINED; 

			dist_count = common.points_count_in_plane(&GetDataModel()->hz_plane_buff[0]);

			/* 6.11.1995 */
			/*
			if(GetDataModel()->display == DOM_DLG)
			{*/
				sprintf(GetDataModel()->hz_plane_buff[0].points[0].point_id,"%-6.6s","rp-p1");
				sprintf(GetDataModel()->hz_plane_buff[0].points[1].point_id,"%-6.6s","rp-p2");
				sprintf(GetDataModel()->hz_plane_buff[0].points[2].point_id,"%-6.6s","rp-p3");
				GetDataModel()->hz_plane = true;
			/*
			}
			if(GetDataModel()->display == DOM_USER_DLG)
			{
							sprintf(&GetModel()->hz_plane_buff[0].points[0].point_id,"%-6.6s","rp-p1");
				sprintf(&GetModel()->hz_plane_buff[0].points[1].point_id,"%-6.6s","rp-p2");
				sprintf(&GetModel()->hz_plane_buff[0].points[2].point_id,"%-6.6s","rp-p3");
				GetModel()->hz_plane = true;
			}
			*/
			ret = true;
		}
		else
		{
			//if(display == DOM_DSP)
				GetDataModel()->hz_plane = false;
			
			//else if(display == DLG_DOMUSER)
			//	GetModel()->hz_plane = false;
		}
	 }
	 else
	 {
		// msgbox(TXT_NIL_TOKEN, M_ERROR_IN_SET_HZ_TOK,MB_OK);
	 }
bbb:
	return ret;
}

// ================================================================================================
short DCP::DefinePlaneDialog::set_horizontal_plane(/*plane_buff_ *planes, short DISPLAY, short PLANE_TYPE*/)
{
short points;
short ret;
	
	ret = false;
	points  = 0;	

	DCP::Common common(m_pModel);

	points = common.points_count_in_plane(&GetDataModel()->hz_plane_buff[0]);

	if(GetDataModel()->display == DOM_DLG || GetDataModel()->display == DOM_USERDEF_DLG)
	{
		if(points > 0)
		{
			// if(msgbox(TXT_NIL_TOKEN,M_POINTS_OF_PLANE_TOK,MB_YESNO) == ID_OK) // removed 08.02.99
			//{
				memset(&GetDataModel()->hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
				//delete_all_plane(MAX_POINTS_IN_PLANE,&planes[0]);	
				points = 0;
			//}
			//else
			//{
			//	return FALSE;
			//}
		}
		
		if(points == 0/* && get_LEVELING() ==TRUE*/)
		{
			if(GetDataModel()->active_plane == XY_PLANE)
			{
				GetDataModel()->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].sta = POINT_DESIGN;


				GetDataModel()->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
			else if(GetDataModel()->active_plane == ZX_PLANE)
			{
				//PrintLn("Setting %s", "ZX");

				GetDataModel()->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

				GetDataModel()->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
			else if(GetDataModel()->active_plane == YZ_PLANE)
			{
				//PrintLn("Setting %s", "YZ");

				GetDataModel()->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

				GetDataModel()->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
		}
		
		else
		{
			ret = false;
		}
		

	}
	else
	{
		
	}
	return ret;
}

// ================================================================================================
short DCP::DefinePlaneDialog::calc_plane(S_PLANE_BUFF *plane, short actdes)
{
	CalcPlane calcplane;

	return calcplane.calc(plane,actdes);

}
// ================================================================================================
short DCP::DefinePlaneDialog::defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint)
{
	short count=0,i,sta;

	if(lastpoint != nullptr)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_PLANE; i++)
	{
		sta = plane[0].points[i].sta;

		if(sta == 1 || sta == 2)
			count++;
		
		if(sta != 0)
		{
			if(lastpoint != nullptr)
				*lastpoint = i+1;
		}
	}
	return count;
}

void DCP::DefinePlaneDialog::MeasPressed()
{
	if(GetDataModel()->hz_plane)
	{
		GetDataModel()->hz_plane =false;
	}
}


// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::DefinePlaneController::DefinePlaneController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DOM_PLANE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DefinePlaneDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( PLANE_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	
    vDef.strLable = StringC(AT_DCP06,K_DCP_XY_PLANE_TOK);
    SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_ZX_PLANE_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_YZ_PLANE_TOK);  
	vDef.poOwner = this;
	SetFunctionKey( FK3, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_HORIZONTAL_PLANE_TOK);
	SetFunctionKey( FK4, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_MEAS_TOK);
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

DCP::DefinePlaneController::~DefinePlaneController()
{

}
// Description: Route model to everybody else
bool DCP::DefinePlaneController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DefinePlaneController::OnF1Pressed()
{	
	m_pDlg->xy_plane();
}

void DCP::DefinePlaneController::OnF2Pressed()
{	
	m_pDlg->zx_plane();

}

void DCP::DefinePlaneController::OnF3Pressed()
{
	m_pDlg->yz_plane();
}

void DCP::DefinePlaneController::OnF4Pressed()
{	
	if(m_pDlg->hz_plane())
	{
		m_pDlg->UpdateData();

		// Remove the following statement if you don't want an exit
		// to the main menu
		(void)Close(EC_KEY_CONT);
	}
}

void DCP::DefinePlaneController::OnF5Pressed()
{	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->MeasPressed();

	DCP::MeasureModel* pModel = new MeasureModel;
	pModel->m_iMaxPoint = MAX_POINTS_IN_PLANE;
	pModel->m_iMinPoint = MIN_POINTS_FOR_PLANE;
	pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
	memcpy(&pModel->point_table[0],&m_pDlg->GetDataModel()->plane_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);

	if(GetController(MEAS_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pModel));
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK));

	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
}

// Description: Handle change of position values
void DCP::DefinePlaneController::OnF6Pressed()
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
	(void)Close(EC_KEY_CONT);
}

void DCP::DefinePlaneController::OnSHF2Pressed()
{	
	m_pDlg->delete_plane();	
}

// Description: React on close of tabbed dialog
void DCP::DefinePlaneController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DefinePlaneController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDlg->GetDataModel()->plane_buff[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_PLANE);
		
		for(int i = 0; i < MAX_POINTS_IN_PLANE; i++)
		{
			if(m_pDlg->GetDataModel()->plane_buff[0].points[i].sta == 3)
				m_pDlg->GetDataModel()->plane_buff[0].points[i].sta = 1;
			if(m_pDlg->GetDataModel()->plane_buff[0].points[i].sta == 4)
				m_pDlg->GetDataModel()->plane_buff[0].points[i].sta = 2;
		}
		/*
		sprintf(m_pDlg->GetDataModel()->plane_buff[0].points[0].point_id,"%-s","P1");
		m_pDlg->GetDataModel()->plane_buff[0].points[0].x = 0.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[0].y = 0.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[0].z = 0.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[0].sta = 1;

		sprintf(m_pDlg->GetDataModel()->plane_buff[0].points[1].point_id,"%-s","P2");
		m_pDlg->GetDataModel()->plane_buff[0].points[1].x = 1.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[1].y = 0.1;
		m_pDlg->GetDataModel()->plane_buff[0].points[1].z = 0.1;
		m_pDlg->GetDataModel()->plane_buff[0].points[1].sta = 1;

		sprintf(m_pDlg->GetDataModel()->plane_buff[0].points[2].point_id,"%-s","P3");
		m_pDlg->GetDataModel()->plane_buff[0].points[2].x = 2.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[2].y = 1.9;
		m_pDlg->GetDataModel()->plane_buff[0].points[2].z = 0.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[2].sta = 1;

		sprintf(m_pDlg->GetDataModel()->plane_buff[0].points[3].point_id,"%-s","P4");
		m_pDlg->GetDataModel()->plane_buff[0].points[3].x = 3.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[3].y = 3.1;
		m_pDlg->GetDataModel()->plane_buff[0].points[3].z = 0.5;
		m_pDlg->GetDataModel()->plane_buff[0].points[3].sta = 1;

		sprintf(m_pDlg->GetDataModel()->plane_buff[0].points[4].point_id,"%-s","P4");
		m_pDlg->GetDataModel()->plane_buff[0].points[4].x = 4.0;
		m_pDlg->GetDataModel()->plane_buff[0].points[4].y = 4.1;
		m_pDlg->GetDataModel()->plane_buff[0].points[4].z = 0.5;
		m_pDlg->GetDataModel()->plane_buff[0].points[4].sta = 1;
		*/
		
		if(m_pDlg->GetDataModel()->display == DOM_DLG)
		m_pDlg->GetDataModel()->hz_plane = false;
		
		if(GetController(CALC_PLANE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_PLANE_CONTROLLER, new DCP::CalcPlaneontrollerC(&m_pDlg->GetDataModel()->plane_buff[0],ACTUAL, 0) );
		}

		(void)GetController( CALC_PLANE_CONTROLLER )->SetModel(m_pModel);
		SetActiveController(CALC_PLANE_CONTROLLER, true);
	}

	if(lCtrlID == CALC_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		//DestroyController( lCtrlID );
		m_pDlg->UpdateData();
	   (void)Close(EC_KEY_CONT);	
	   (void)Close(EC_KEY_CONT);
	}
		/*
		if(m_pDlg->CalculatePlaneAfterMeas())
		{
			if(GetController(RES_PLANE_CONTROLLER) == nullptr)
			{
				(void)AddController( RES_PLANE_CONTROLLER, new DCP::ResPlaneController(m_pModel) );
			}

			(void)GetController(RES_PLANE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK);

			(void)GetController( RES_PLANE_CONTROLLER )->SetModel(m_pDlg->GetDataModel());
			SetActiveController(RES_PLANE_CONTROLLER, true);

		}
		*/
	m_pDlg->RefreshControls();
	//m_pDlg->Show();
	DestroyController( lCtrlID );
	
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::DefinePlaneModel::DefinePlaneModel()
{
	/*
	active_plane	= pModel->active_plane;
	active_line		= pModel->active_line; 
	hz_plane		= pModel->hz_plane;
	memcpy(&plane_buff[0], &pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
	memcpy(&hz_plane_buff[0], &pModel->hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	*/
}
DCP::DefinePlaneModel::~DefinePlaneModel()
{
}
