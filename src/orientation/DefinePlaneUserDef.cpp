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
#include <dcp06/orientation/DefinePlaneUserDef.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/orientation/ResPlane.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>

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
// OBS_IMPLEMENT_EXECUTE(DCP::DCP06UnitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::DCP06DefinePlaneUserDefDlgC::DCP06DefinePlaneUserDefDlgC(DCP::DCP06ModelC* pDCP06Model):m_pPlane(0),
						m_pDCP06Model(pDCP06Model),m_pPoints(0),m_pPlanePoints(0)
{
	//SetTxtApplicationId(AT_DCP06);

	m_strXYPlane.LoadTxt(AT_DCP06,V_DCP_XY_PLANE_TOK);
	m_strZXPlane.LoadTxt(AT_DCP06,V_DCP_ZX_PLANE_TOK);
	m_strYZPlane.LoadTxt(AT_DCP06,V_DCP_YZ_PLANE_TOK);
	m_sHZText.LoadTxt(AT_DCP06,V_DCP_HZ_TOK);


}


            // Description: Destructor
DCP::DCP06DefinePlaneUserDefDlgC::~DCP06DefinePlaneUserDefDlgC()
{

}

void DCP::DCP06DefinePlaneUserDefDlgC::OnInitDialog(void)
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
	AddCtrl(m_pPlane);
	
	// InsertEmptyLine(); CAPTIVATE

	m_pPlanePoints = new GUI::TextCtrlC();
	m_pPlanePoints->SetId(ePlanePoints);
	m_pPlanePoints->SetText(StringC(AT_DCP06,T_DCP_PLANE_POINTS_TOK));
	AddCtrl(m_pPlanePoints);



	//m_pPoints = new GUI::TextBoxCtrlC(0,10,300,120); CAPTIVATE
	m_pPoints = new GUI::TextCtrlC();
	m_pPoints->SetId(ePoints);
	AddCtrl(m_pPoints);

	/*
	m_pPlaneInfo1 = new GUI::TextCtrlC(50,50,100);
	m_pPlaneInfo1->SetId(ePlaneInfo1);
	short xx,yy;
	m_pPlaneInfo1->GetPosition(xx,yy);

	m_pPlaneInfo1->SetTextTok(P_DCP_REF_PLANE_EXP1_TOK);

	AddCtrl(m_pPlaneInfo1);

	m_pPlaneInfo2 = new GUI::TextCtrlC();
	m_pPlaneInfo2->SetId(ePlaneInfo2);
	m_pPlaneInfo2->SetTextTok(P_DCP_REF_PLANE_EXP2_TOK);
	AddCtrl(m_pPlaneInfo2);
	
	m_pPlaneInfo3 = new GUI::TextCtrlC();
	m_pPlaneInfo3->SetId(ePlaneInfo3);
	m_pPlaneInfo3->SetTextTok(P_DCP_REF_PLANE_EXP3_TOK);
	AddCtrl(m_pPlaneInfo3);
	
	m_pPlaneInfo4 = new GUI::TextCtrlC();
	m_pPlaneInfo4->SetId(ePlaneInfo4);
	m_pPlaneInfo4->SetTextTok(P_DCP_REF_PLANE_EXP4_TOK);
	AddCtrl(m_pPlaneInfo4);
	*/
	//SetHelpTok(H_DCP_USERDEF_PLANE_TOK,0);
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP06DefinePlaneUserDefDlgC::OnDialogActivated()
{
	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06DefinePlaneUserDefDlgC::RefreshControls()
{	
	if(m_pPlane && m_pPoints && m_pPlanePoints)
	{
		StringC sTemp;
		StringC sStatus(L"-");
		int i = 0;
		// seleted point count
		int selectedCount = 0;
		for(i= 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(GetDataModel()->userdef_plane_points_no[i] != 0)
				selectedCount++;
		}
		
		int lastMeasuredPoints = 0;
		
		for(i= 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(GetDataModel()->userdef_measured_points[i].sta != 0)
				lastMeasuredPoints = i+1;
		}

		if(GetDataModel()->planeModel->hz_plane)
		{
			if(GetDataModel()->planeModel->hz_plane_buff[0].calc)
				sStatus = L"+";
		}
		else
		{
			//if(GetDataModel()->planeModel->plane_buff[0].calc)
			if(selectedCount >= 3)
				sStatus = L"+";
		}

		if(GetDataModel()->planeModel->active_plane	== XY_PLANE)
			sTemp = m_strXYPlane;
		else if(GetDataModel()->planeModel->active_plane	== ZX_PLANE)
			sTemp = m_strZXPlane;	
		else
			sTemp = m_strYZPlane;	

		if(GetDataModel()->planeModel->hz_plane)
		{
			sTemp += L"(";
			sTemp += m_sHZText;
			sTemp += L")";
		}
		sTemp += sStatus;
		m_pPlane->GetStringInputCtrl()->SetString(sTemp);

		StringC sMsg;
		//sMsg.LoadTxt(AT_DCP06,T_DCP_PLANE_POINTS_TOK);
		//sMsg += "\n";
		for(i= 0; i < 20; i++)
		{
			StringC text;
			if(GetDataModel()->userdef_plane_points_no[i] != 0 && !GetDataModel()->planeModel->hz_plane)
				text.Format(L"%2d: %2d/%2d\n", i+1,GetDataModel()->userdef_plane_points_no[i],lastMeasuredPoints);
			else
				text.Format(L"%2d: %2.2s\n", i+1,"-");
			sMsg += text;
		}
		m_pPoints->SetText(sMsg);
	}
}

void DCP::DCP06DefinePlaneUserDefDlgC::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::DCP06DefinePlaneUserDefDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06DefinePlaneUserDefModelC* pDCP06Model = dynamic_cast< DCP::DCP06DefinePlaneUserDefModelC* >( pModel );

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
DCP::DCP06DefinePlaneUserDefModelC* DCP::DCP06DefinePlaneUserDefDlgC::GetDataModel() const
{
    return (DCP::DCP06DefinePlaneUserDefModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP06DefinePlaneUserDefDlgC::xy_plane()
{
	GetDataModel()->planeModel->active_plane = XY_PLANE;
	RefreshControls();
	
}

// ================================================================================================
void DCP::DCP06DefinePlaneUserDefDlgC::zx_plane()
{
	GetDataModel()->planeModel->active_plane = ZX_PLANE;
	RefreshControls();
}

// ================================================================================================
void DCP::DCP06DefinePlaneUserDefDlgC::yz_plane()
{
	GetDataModel()->planeModel->active_plane = YZ_PLANE;
	RefreshControls();
}

// ================================================================================================
void DCP::DCP06DefinePlaneUserDefDlgC::delete_plane()
{
	DCP06MsgBoxC MsgBox;
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_PLANE_TOK);
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->planeModel->active_plane	= XY_PLANE;
		//m_pDomModel->dom_active_line	= pModel->active_line; 
		GetDataModel()->planeModel->hz_plane		= false;
		memset(&GetDataModel()->planeModel->plane_buff[0], 0, sizeof(S_PLANE_BUFF));
		memset(&GetDataModel()->planeModel->hz_plane_buff[0], 0, sizeof(S_PLANE_BUFF));
		memset(&GetDataModel()->userdef_plane_points_no[0],0, sizeof(short) * MAX_USERDEF_POINTS);
		RefreshControls();
	}
}

// ================================================================================================
short DCP::DCP06DefinePlaneUserDefDlgC::hz_plane()
{
	if(GetDataModel()->planeModel->hz_plane)
	{
		GetDataModel()->planeModel->hz_plane = false;		
		RefreshControls();
	}
	else
	{
		GetDataModel()->planeModel->hz_plane = true;		
		if(set_hz_plane1(ACTUAL))
			return true;
	}
	return false;
}
// ================================================================================================
bool DCP::DCP06DefinePlaneUserDefDlgC::CalculatePlaneAfterMeas()
{
	for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
	{
		if(GetDataModel()->planeModel->plane_buff[0].points[i].sta == POINT_DESIGN_REJECTED)
			GetDataModel()->planeModel->plane_buff[0].points[i].sta = POINT_DESIGN;

		if(GetDataModel()->planeModel->plane_buff[0].points[i].sta == POINT_MEASURED_REJECTED)
			GetDataModel()->planeModel->plane_buff[0].points[i].sta = POINT_MEASURED;

	}
	if(GetDataModel()->planeModel->display == DOM_DLG)
		GetDataModel()->planeModel->hz_plane = false;

	if(calc_plane(&GetDataModel()->planeModel->plane_buff[0],ACTUAL))
	{
		// SHOW RESIDUALS
			return true;
	}

	return false;
}

// ================================================================================================
short DCP::DCP06DefinePlaneUserDefDlgC::set_hz_plane1(short actualdesign)
{
short dist_count,ret;
//RC_TYPE tmcResult;
//TMC_ANGLE bAngle;

	ret = false;
	
	DCP06CommonC common(m_pDCP06Model);
	DCP06MsgBoxC msgbox;

	dist_count = common.points_count_in_plane(&GetDataModel()->planeModel->hz_plane_buff[0]);
	
	TPI::MeasConfigC oMeasConfig;
	
	oMeasConfig.Get();
	//3.x
	//bool bComp = oMeasConfig.GetCompensatorMode();
	bool bComp = oMeasConfig.IsCompensatorEnabled();

	//if(get_compensator_onoff() == FALSE)
	if(!bComp)
	{
		// 3.x
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
	if((/*GetDataModel()->display == DIST_DSP ||*/ GetDataModel()->planeModel->display == CALC_DIST_DLG ) && dist_count  == 0 )
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
		if(calc_plane(&GetDataModel()->planeModel->hz_plane_buff[0],actualdesign))// == true)
		{
			GetDataModel()->planeModel->hz_plane_buff[0].calc = 1;
			GetDataModel()->planeModel->hz_plane_buff[0].sta = PLANE_DEFINED; 

			dist_count = common.points_count_in_plane(&GetDataModel()->planeModel->hz_plane_buff[0]);

			/* 6.11.1995 */
			/*
			if(GetDataModel()->display == DOM_DLG)
			{*/
				sprintf(GetDataModel()->planeModel->hz_plane_buff[0].points[0].point_id,"%-6.6s","rp-p1");
				sprintf(GetDataModel()->planeModel->hz_plane_buff[0].points[1].point_id,"%-6.6s","rp-p2");
				sprintf(GetDataModel()->planeModel->hz_plane_buff[0].points[2].point_id,"%-6.6s","rp-p3");
				GetDataModel()->planeModel->hz_plane = true;
			/*
			}
			if(GetDataModel()->display == DOM_USER_DLG)
			{
							sprintf(&GetDCP06Model()->hz_plane_buff[0].points[0].point_id,"%-6.6s","rp-p1");
				sprintf(&GetDCP06Model()->hz_plane_buff[0].points[1].point_id,"%-6.6s","rp-p2");
				sprintf(&GetDCP06Model()->hz_plane_buff[0].points[2].point_id,"%-6.6s","rp-p3");
				GetDCP06Model()->hz_plane = true;
			}
			*/
			ret = true;
		}
		else
		{
			//if(display == DOM_DSP)
				GetDataModel()->planeModel->hz_plane = false;
			
			//else if(display == DLG_DOMUSER)
			//	GetDCP06Model()->hz_plane = false;
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
short DCP::DCP06DefinePlaneUserDefDlgC::set_horizontal_plane(/*plane_buff_ *planes, short DISPLAY, short PLANE_TYPE*/)
{
short points;
short ret;
	
	ret = false;
	points  = 0;	

	DCP::DCP06CommonC common(m_pDCP06Model);

	points = common.points_count_in_plane(&GetDataModel()->planeModel->hz_plane_buff[0]);

	if(GetDataModel()->planeModel->display == DOM_DLG || GetDataModel()->planeModel->display == DOM_USERDEF_DLG)
	{
		if(points > 0)
		{
			// if(msgbox(TXT_NIL_TOKEN,M_POINTS_OF_PLANE_TOK,MB_YESNO) == ID_OK) // removed 08.02.99
			//{
				memset(&GetDataModel()->planeModel->hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
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
			if(GetDataModel()->planeModel->active_plane == XY_PLANE)
			{
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].sta = POINT_DESIGN;


				GetDataModel()->planeModel->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->planeModel->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
			else if(GetDataModel()->planeModel->active_plane == ZX_PLANE)
			{
				//PrintLn("Setting %s", "ZX");

				GetDataModel()->planeModel->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

				GetDataModel()->planeModel->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->planeModel->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

				ret = true;
			}
			else if(GetDataModel()->planeModel->active_plane == YZ_PLANE)
			{
				//PrintLn("Setting %s", "YZ");

				GetDataModel()->planeModel->hz_plane_buff[0].points[0].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[0].sta = POINT_DESIGN;

				GetDataModel()->planeModel->hz_plane_buff[0].points[1].x = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].y = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[1].sta = POINT_DESIGN;

				GetDataModel()->planeModel->hz_plane_buff[0].points[2].x = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].y = 1.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].z = 0.0;
				GetDataModel()->planeModel->hz_plane_buff[0].points[2].sta = POINT_DESIGN;

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
short DCP::DCP06DefinePlaneUserDefDlgC::calc_plane(S_PLANE_BUFF *plane, short actdes)
{
	DCP06CalcPlaneC calcplane;

	return calcplane.calc(plane,actdes);

}
// ================================================================================================
short DCP::DCP06DefinePlaneUserDefDlgC::defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint)
{
	short count=0,i,sta;

	if(lastpoint != NULL)
		*lastpoint = 0;

	for(i=0; i< MAX_POINTS_IN_PLANE; i++)
	{
		sta = plane[0].points[i].sta;

		if(sta == 1 || sta == 2)
			count++;
		
		if(sta != 0)
		{
			if(lastpoint != NULL)
				*lastpoint = i+1;
		}
	}
	return count;
}

void DCP::DCP06DefinePlaneUserDefDlgC::MeasPressed()
{
	if(GetDataModel()->planeModel->hz_plane)
	{
		GetDataModel()->planeModel->hz_plane =false;
	}
}


// ================================================================================================
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06DefinePlaneUserDefControllerC::DCP06DefinePlaneUserDefControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_USERDEF_PLANE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP06DefinePlaneUserDefDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( PLANE_USERDEF_DLG, m_pDlg, true );
	
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

    vDef.strLable = StringC(AT_DCP06,K_DCP_POINT_TOK);
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

DCP::DCP06DefinePlaneUserDefControllerC::~DCP06DefinePlaneUserDefControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06DefinePlaneUserDefControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DCP06DefinePlaneUserDefControllerC::OnF1Pressed()
{	
	m_pDlg->xy_plane();
}

void DCP::DCP06DefinePlaneUserDefControllerC::OnF2Pressed()
{	
	m_pDlg->zx_plane();

}

void DCP::DCP06DefinePlaneUserDefControllerC::OnF3Pressed()
{
	m_pDlg->yz_plane();
}

void DCP::DCP06DefinePlaneUserDefControllerC::OnF4Pressed()
{	
	if(m_pDlg->hz_plane())
	{
		m_pDlg->UpdateData();

		// Remove the following statement if you don't want an exit
		// to the main menu
		(void)Close(EC_KEY_CONT);
	}
}

void DCP::DCP06DefinePlaneUserDefControllerC::OnF5Pressed()
{	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::DCP06CommonC common(m_pDCP06Model);

	// SELECT MULTIPOINTS
	DCP::DCP06SelectMultiPointsModelC* pModel = new DCP06SelectMultiPointsModelC;
	
	// list of selectable points
	memcpy(&pModel->sel_points[0],&m_pDlg->GetDataModel()->select_point_list[0], sizeof(S_SELECT_POINTS) * MAX_USERDEF_POINTS);

	// original list of measured poits
	pModel->m_iPointsCount = common.get_last_defined_point(&m_pDlg->GetDataModel()->userdef_measured_points[0],MAX_USERDEF_POINTS);
	
	pModel->m_iDef = ACTUAL;
	//pModel->sSelectedFile = m_pDlg->GetDataModel()->sFile;
	pModel->m_iMinPoint = MIN_POINTS_FOR_PLANE;
	pModel->m_iMaxPoint = pModel->m_iPointsCount;
	
	// set info text....
	StringC sInfo;
	sInfo.LoadTxt(AT_DCP06, T_DCP_SELECT_POINTS_TOK);
	//sInfo += L"  ";
	//sInfo += pModel->sSelectedFile;
	pModel->sInfo = sInfo;

	// set title
	pModel->sTitle = GetTitle();
	
	// set help token
	pModel->helpToken = H_DCP_SEL_MULTI_A_AND_D_TOK;

	if(GetController(SELECT_MULTIPOINTS_CONTROLLER) == NULL)
	{
			(void)AddController( SELECT_MULTIPOINTS_CONTROLLER, new DCP::DCP06SelectMultiPointsControllerC(m_pDCP06Model) );
	}

	(void)GetController( SELECT_MULTIPOINTS_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_CONTROLLER, true);
}

// Description: Handle change of position values
void DCP::DCP06DefinePlaneUserDefControllerC::OnF6Pressed()
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

void DCP::DCP06DefinePlaneUserDefControllerC::OnSHF2Pressed()
{	
	m_pDlg->delete_plane();	
}

// Description: React on close of tabbed dialog
void DCP::DCP06DefinePlaneUserDefControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06DefinePlaneUserDefControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
		if(lCtrlID == SELECT_MULTIPOINTS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectMultiPointsModelC* pModel = (DCP::DCP06SelectMultiPointsModelC*) GetController( SELECT_MULTIPOINTS_CONTROLLER )->GetModel();		
		DCP06CommonC common(m_pDCP06Model);
		
		// delete plane
		memset(&m_pDlg->GetDataModel()->planeModel->plane_buff[0], 0, sizeof(S_PLANE_BUFF));


		m_pDlg->GetDataModel()->planeModel->hz_plane = false;
		memset(&m_pDlg->GetDataModel()->planeModel->hz_plane_buff[0],0, sizeof(S_PLANE_BUFF));
		// delete earlier selected points

		memset(&m_pDlg->GetDataModel()->userdef_plane_points_no[0], 0, sizeof(short) * MAX_USERDEF_POINTS);
		
		short iSelected = 0;

		for(short i=0;i<pModel->m_iMaxPoint;i++)
		{
				m_pDlg->GetDataModel()->userdef_plane_points_no[i] = pModel->nro_table[i][0];

				if(m_pDlg->GetDataModel()->userdef_plane_points_no[i] != 0)
					iSelected++;
		}

		if(iSelected < 3)
		{
			DCP06MsgBoxC msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_IN_MIN_3_POINTS_TOK);
			msgbox.ShowMessageOk(sMsg);
		}
	}

	if(lCtrlID == CALC_PLANE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		//DestroyController( lCtrlID );
		m_pDlg->UpdateData();
	   (void)Close(EC_KEY_CONT);	
	}
		/*
		if(m_pDlg->CalculatePlaneAfterMeas())
		{
			if(GetController(RES_PLANE_CONTROLLER) == NULL)
			{
				(void)AddController( RES_PLANE_CONTROLLER, new DCP::DCP06ResPlaneControllerC(m_pDCP06Model) );
			}

			(void)GetController(RES_PLANE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK);

			(void)GetController( RES_PLANE_CONTROLLER )->SetModel(m_pDlg->GetDataModel());
			SetActiveController(RES_PLANE_CONTROLLER, true);

		}
		*/
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================

DCP::DCP06DefinePlaneUserDefModelC::DCP06DefinePlaneUserDefModelC()
{
	planeModel = new DCP::DCP06DefinePlaneModelC;

	//active_plane	= pModel->active_plane;
	//active_line		= pModel->active_line; 
	//hz_plane		= pModel->hz_plane;
	//memcpy(&plane_buff[0], &pModel->plane_buff[0], sizeof(S_PLANE_BUFF));
	//memcpy(&hz_plane_buff[0], &pModel->hz_plane_buff[0], sizeof(S_PLANE_BUFF));
	
}
DCP::DCP06DefinePlaneUserDefModelC::~DCP06DefinePlaneUserDefModelC()
{
	if(planeModel != 0)
	{
		delete planeModel;
		planeModel = 0;
	}
}
