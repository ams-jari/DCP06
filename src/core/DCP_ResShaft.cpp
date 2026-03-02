// YKSI PISTE ACTUAL TAI DESIGN
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
#include <dcp06/core/DCP_ResShaft.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/calculation/DCP_CalcCircle.hpp>
#include <dcp06/file/DCP_ShaftSave.hpp>
#include "calc.h"

#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
#include <GUI_DeskTop.hpp>

#include <math.h>
#include "calc.h"

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;
// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================

//OBS_IMPLEMENT_EXECUTE(DCP::DCP06SelectOnePointDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP::DCP06ResShaftDlgC::DCP06ResShaftDlgC(DCP::DCP06ModelC *pDCP06Model,DCP06ShaftModelC* pShaftModel):poMultiColCtrl(NULL),m_pInfo1(NULL),
	//m_pDist(NULL),poMultiColCtrlDist(NULL),poMultiColCtrlAngle(NULL),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(DCP06SelectOnePointDlgC, OnChanged), this),
	m_pDCP06Model(pDCP06Model), m_pShaftModel(pShaftModel)
{
	//SetTxtApplicationId(AT_DCP05);
	
	// load title
	sTitle.LoadTxt(AT_DCP05,T_DCP_DEV_OF_SHAFT_TOK);
	
	
}
// Description: Destructor
DCP::DCP06ResShaftDlgC::~DCP06ResShaftDlgC()
{
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP06ResShaftDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
	
	/* CAPTIVATE
	GUI::ListConfigColsC oConfigCols1;
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Point));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Deviation));
	*/

	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->SetId(eMulti);
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);


	// text
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_POINT_NUMBER_TOK), CI_No));
	//USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_POINT_ID_TOK),CI_Point));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(L""),CI_Point));
	//USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_DEVIATION_TOK),CI_Deviation));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(L""),CI_Deviation));
	
	poMultiColCtrl->AddColSelection(CI_No, CI_Point, CI_Deviation);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);
/*
	// Dist
	poMultiColCtrlDist = new GUI::ListMultiColCtrlC();
	poMultiColCtrlDist->SetId(eDist);
	poMultiColCtrlDist->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrlDist->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);


	// text
	//USER_APP_VERIFY(poMultiColCtrlDist->AddCol(StringC(AT_DCP05,P_DCP_POINT_NUMBER_TOK), CI_No));
	USER_APP_VERIFY(poMultiColCtrlDist->AddCol(StringC(L"Distance"),CI_Text));
	USER_APP_VERIFY(poMultiColCtrlDist->AddCol(StringC(L"Dist"),CI_Dist));
	
	poMultiColCtrlDist->AddColSelection(CI_Text, CI_Dist);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrlDist->SetColSelection(0));
	AddTable(poMultiColCtrlDist);


	// Angle
	poMultiColCtrlAngle = new GUI::ListMultiColCtrlC();
	poMultiColCtrlAngle->SetId(eAngle);
	poMultiColCtrlAngle->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrlAngle->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);


	// text
	//USER_APP_VERIFY(poMultiColCtrlDist->AddCol(StringC(AT_DCP05,P_DCP_POINT_NUMBER_TOK), CI_No));
	USER_APP_VERIFY(poMultiColCtrlAngle->AddCol(StringC(L"!Angle"),CI_Text))
	USER_APP_VERIFY(poMultiColCtrlAngle->AddCol(StringC(L"Angle"),CI_Angle));
	//USER_APP_VERIFY(poMultiColCtrlDist->AddCol(StringC(L"Dist"),CI_Dist));
	
	poMultiColCtrlAngle->AddColSelection(CI_Text,CI_Angle);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrlAngle->SetColSelection(0));
	AddTable(poMultiColCtrlAngle);
*/

	

	/*
	m_pDist = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDist->SetId(eDist);
	m_pDist->SetText(StringC(AT_DCP05,P_DCP_CIRCLE_TOK));
	//m_pCircle->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pDist->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pDist->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pDist);
	*/
	//SetHelpTok(H_DCP_SHAFT_RESIDUALS_TOK,0);
	//m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());

	m_pCommon = new DCP06CommonC(m_pDCP06Model);
}


/*
void DCP::DCP06ResShaftDlgC::OnF1Pressed(void)
{
	
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	StringC sTemp;
		
	short sta = GetDataModel()->circle_points[0].points[iSelectedId].sta;
	
	if(GetDataModel()->circle_points[0].points[iSelectedId].sta == 1 || GetDataModel()->circle_points[0].points[iSelectedId].sta == 2)
	{
		if(m_pCommon->defined_circle_points(&GetDataModel()->circle_points[0],NULL) <= 3)
		{
			StringC strText;
			strText.LoadTxt(AT_DCP05,M_DCP_CANNOT_REJECT_PNT_TOK);
			DCP06MsgBoxC msgbox;
			
			msgbox.ShowMessageOk(strText);
			return;
		}					
	}
						
	if(sta == 0)
	{
		return;
	}
	else if(sta == 1)
	{
		GetDataModel()->circle_points[0].points[iSelectedId].sta = 3;
	}
	else if(sta == 2)
	{
		GetDataModel()->circle_points[0].points[iSelectedId].sta = 4;
	}
	else if(sta == 3)
	{
		GetDataModel()->circle_points[0].points[iSelectedId].sta = 1;
	}
	else if(sta == 4)
	{
		GetDataModel()->circle_points[0].points[iSelectedId].sta = 2;
	}
	
	
	DCP06CalcCircleC circle(GetDataModel()->PLANE_TYPE, 
								&GetDataModel()->circle_points[0],
								&GetDataModel()->planes[0],
								&GetDataModel()->circle_points_in_plane[0],
								GetDataModel()->bR);
	
	if(circle.calc_center_of_circle())
		{
			GetDataModel()->m_iCounts = GetDataModel()->pCommon->defined_circle_points(&GetDataModel()->circle_points[0],0);
			circle.get_results(&GetDataModel()->cx, &GetDataModel()->cy,&GetDataModel()->cz, &GetDataModel()->diameter, &GetDataModel()->rms_diameter,&GetDataModel()->max_dev_in_point);
			GetDataModel()->show_circle_points();
	}
	//DCP06CalcLineC calcline;
	//calcline.calc(&GetDataModel()->line_buff[0],ACTUAL);
	
	RefreshControls();		
}
*/

void DCP::DCP06ResShaftDlgC::OnDialogActivated()
{
	RefreshControls();
} 

// Description: refresh all controls
void DCP::DCP06ResShaftDlgC::RefreshControls()
{
	int iMax=0;
	int i;

	if(!poMultiColCtrl)// || !poMultiColCtrlDist  || !poMultiColCtrlAngle)
		return;
	for(i=MAX_POINTS_IN_CIRCLE-1;i >=0; i--)
	{
		
		if(m_pShaftModel->shaft_circle_points->points[i].sta != 0 ||
		   !m_pCommon->strblank(m_pShaftModel->shaft_circle_points->points[i].point_id))
		{
			iMax = i+1;
			break;
		}
	}
	

	// 
	//BeginDraw(); 
	char temp[100];
	StringC sPoint;
	StringC sNo;
	StringC sDev;
	int sta;

	poMultiColCtrl->DestroyAll();

	sprintf(temp,"%9.*f",m_pDCP06Model->m_nDecimals, m_pShaftModel->centerOfCircleDist); 

	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) 0));
	USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) 0, L"Distance (to ref line)"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) 0, StringC(temp)));
	//USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) 0, L"Dis"));

	sprintf(temp,"%9.*f",m_pDCP06Model->m_nDecimals, m_pShaftModel->angleLines); 

	USER_APP_VERIFY(poMultiColCtrl->AddRow((short) 1));
	USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) 1, L"Angle (to ref line)"));
	USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) 1, StringC(temp)));
	//USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) 1, L"Ang"));


	for(i=0; i < iMax; i++)
	{
		sprintf(temp,"%-10.10s Deviation:",m_pShaftModel->shaft_circle_points->points[i].point_id);
		sPoint = temp;
		
		sprintf(temp,"%-d",i+1);
		sNo = temp;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i +3));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i+3, sNo));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i+3, sPoint));

		sta = m_pShaftModel->shaft_circle_points->points[i].sta;

		if(sta == 1 || sta == 2) // measured or design
		{
			sprintf(temp,"%+9.*f", m_pDCP06Model->m_nDecimals, calc_pdist(i));
			sDev = temp;
		}
		else if (sta == 0)
		{
			sDev = L" ";
		}
		else // rejected
		{
			sDev = L"-";
		}

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) i+3, sDev));
	}

	// ja valitse piste miss� max dev
	short pno=1;
	double rms=0.0;
	/*
	//double maxdist = get_max_dist_and_rms_line(&GetDataModel()->line_buff[0],&pno,&rms);
	//poMultiColCtrl->SetSelectedId(pno-1);
	
	// set title with rms and point count
	short iCount = m_pCommon->defined_points_count_in_line(&GetDataModel()->line_buff[0],0);
	StringC sTemp = sTitle;
	sTemp.Format(sTemp, m_pDCP06Model->m_nDecimals,rms,iCount);
	SetTitleStr(sTemp);
	*/
	//EndDraw();
	/*
	poMultiColCtrlDist->DestroyAll();
	USER_APP_VERIFY(poMultiColCtrlDist->AddRow((short) 0));
	USER_APP_VERIFY(poMultiColCtrlDist->SetCellText(CI_Text, (short) 0, L"Distance"));
	USER_APP_VERIFY(poMultiColCtrlDist->SetCellText(CI_Dist, (short) 0, L"-0.11"));

	poMultiColCtrlAngle->DestroyAll();
	USER_APP_VERIFY(poMultiColCtrlAngle->AddRow((short) 0));
	USER_APP_VERIFY(poMultiColCtrlAngle->SetCellText(CI_Text, (short) 0, L"Angle"));
	USER_APP_VERIFY(poMultiColCtrlAngle->SetCellText(CI_Angle, (short) 0, L"185.00"));
	*/
}


/************************************************************************
*************************************************************************/
double DCP::DCP06ResShaftDlgC::calc_pdist(short pno)
{
struct ams_vector m;
struct plane wplane;
double dist;

		
			m.x = m_pShaftModel->shaft_circle_points->points[pno].x;
			m.y = m_pShaftModel->shaft_circle_points->points[pno].y;
			m.z = m_pShaftModel->shaft_circle_points->points[pno].z;

			wplane.px = m_pShaftModel->calc_plane[0].px;
			wplane.py = m_pShaftModel->calc_plane[0].py;
			wplane.pz = m_pShaftModel->calc_plane[0].pz;

			wplane.nx = m_pShaftModel->calc_plane[0].nx;
			wplane.ny = m_pShaftModel->calc_plane[0].ny;
			wplane.nz = m_pShaftModel->calc_plane[0].nz;

			/* Calculate distance */
			dist = calc_point_dist_from_plane(&m, &wplane);
			return dist;
}

void DCP::DCP06ResShaftDlgC::UpdateData()
{
	/*
	StringC sSelected;
	short iSelected = poMultiColCtrl->GetSelectedId()+1;
	//poMultiColCtrl->GetCellStr(CI_Selected,iSelected, sSelected);
	//GetDataModel()->ADFFileName = sSelected;
	GetDataModel()->iSelectedNo = iSelected;
	*/
		
}
// Description: only accept hello world Model objects
bool DCP::DCP06ResShaftDlgC::SetModel( GUI::ModelC* pModel )
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
DCP::DCP06ModelC* DCP::DCP06ResShaftDlgC::GetDataModel() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::DCP06ResShaftControllerC::DCP06ResShaftControllerC(DCP::DCP06ModelC *pDCP06Model,DCP06ShaftModelC* pShaftModel)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model), m_pShaftModel(pShaftModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_DEV_OF_SHAFT_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	
	//StringC sTitle;
	//sTitle = L"Dev of circle RMS:";
	//sTitle += L"Points:";
	//SetTitleStr(sTitle);

    // Create a dialog
     m_pDlg = new DCP::DCP06ResShaftDlgC(pDCP06Model, m_pShaftModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( RES_SHAFT_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	
	// moved into dialog
	//vDef.nLable = K_DCP_REJECT_TOK;
	//SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_SAVE_TOK);
	SetFunctionKey( FK3, vDef );


	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP05;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";
	SetFunctionKey( SHFK6, vDef1 );

	/*
	// SHIFT
	vDef.nLable = K_DCP_DEL_TOK;
	SetFunctionKey( SHFK2, vDef );
	
	vDef.nLable = K_DCP_SAVE_TOK;
	SetFunctionKey( SHFK3, vDef );

	vDef.nLable = K_DCP_VIEW_TOK;
	SetFunctionKey( SHFK5, vDef );
	*/
} //lint !e818 Pointer parameter could be declared as pointing to const


DCP::DCP06ResShaftControllerC::~DCP06ResShaftControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP06ResShaftControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
}

// SAVE
void DCP::DCP06ResShaftControllerC::OnF3Pressed() 
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	if(GetController(SHAFT_SAVE_CONTROLLER) == NULL)
	{
		(void)AddController( SHAFT_SAVE_CONTROLLER, new DCP::DCP06SaveShaftControllerC(m_pShaftModel,m_pDCP06Model) );
	}

	(void)GetController(SHAFT_SAVE_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_SHAFT_SAVE_TOK));
	(void)GetController( SHAFT_SAVE_CONTROLLER )->SetModel(m_pDCP06Model);
	SetActiveController(SHAFT_SAVE_CONTROLLER, true);
	
}


// CONT
void DCP::DCP06ResShaftControllerC::OnF6Pressed() 
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


// Description: React on close of tabbed dialog
void DCP::DCP06ResShaftControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06ResShaftControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SHAFT_SAVE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		Close(EC_KEY_CONT);
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

