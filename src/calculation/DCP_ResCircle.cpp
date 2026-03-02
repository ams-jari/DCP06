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
#include <dcp06/calculation/DCP_ResCircle.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/calculation/DCP_CalcCircle.hpp>
#include <dcp06/file/DCP_CircleSave.hpp>

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

//OBS_IMPLEMENT_EXECUTE(DCP::DCP05SelectOnePointDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP::DCP05ResCircleDlgC::DCP05ResCircleDlgC(DCP::DCP05ModelC *pDCP05Model):poMultiColCtrl(NULL),m_pInfo1(NULL),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(DCP05SelectOnePointDlgC, OnChanged), this),
	m_pDCP05Model(pDCP05Model)
{
	//SetTxtApplicationId(AT_DCP05);
	
	// load title
	sTitle.LoadTxt(AT_DCP05,T_DCP_DEV_OF_LINE_TOK);
	
	
}
// Description: Destructor
DCP::DCP05ResCircleDlgC::~DCP05ResCircleDlgC()
{
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::DCP05ResCircleDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	GUI::DesktopC::Instance()->UpdateFunctionKeys();

	/*
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
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_POINT_ID_TOK), CI_Point));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP05,P_DCP_DEVIATION_TOK),CI_Deviation));
	
	poMultiColCtrl->AddColSelection(CI_No, CI_Point, CI_Deviation);
	

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP05,K_DCP_REJECT_TOK);
	SetFunctionKey( FK1, vDef );

	//SetHelpTok(H_DCP_CIRCLE_RESIDUALS_TOK,0);
	//m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());

	m_pCommon = new DCP05CommonC(m_pDCP05Model);
}



void DCP::DCP05ResCircleDlgC::OnF1Pressed(void)
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
			DCP05MsgBoxC msgbox;
			
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
	
	
	DCP05CalcCircleC circle(GetDataModel()->PLANE_TYPE, 
								&GetDataModel()->circle_points[0],
								&GetDataModel()->planes[0],
								&GetDataModel()->circle_points_in_plane[0],
								GetDataModel()->bR, m_pDCP05Model);
	
	if(circle.calc_center_of_circle())
		{
			GetDataModel()->m_iCounts = GetDataModel()->pCommon->defined_circle_points(&GetDataModel()->circle_points[0],0);
			circle.get_results(&GetDataModel()->cx, &GetDataModel()->cy,&GetDataModel()->cz, &GetDataModel()->vi, &GetDataModel()->vj,&GetDataModel()->vk, &GetDataModel()->diameter, &GetDataModel()->rms_diameter,&GetDataModel()->max_dev_in_point);
			GetDataModel()->show_circle_points();
	}
	//DCP05CalcLineC calcline;
	//calcline.calc(&GetDataModel()->line_buff[0],ACTUAL);
	
	RefreshControls();		
}

void DCP::DCP05ResCircleDlgC::OnDialogActivated()
{
	RefreshControls();
} 

// Description: refresh all controls
void DCP::DCP05ResCircleDlgC::RefreshControls()
{
	int iMax=0;
	int i;

	if(!poMultiColCtrl)
		return;
	for(i=MAX_POINTS_IN_CIRCLE-1;i >=0; i--)
	{
		if(GetDataModel()->circle_points[0].points[i].sta != 0 ||
		   !m_pCommon->strblank(GetDataModel()->circle_points[0].points[i].point_id))
		{
			iMax = i+1;
			break;
		}
	}
	

	// 
	//BeginDraw(); 
	char temp[20];
	StringC sPoint;
	StringC sNo;
	StringC sDev;
	int sta;

	poMultiColCtrl->DestroyAll();

	for(i=0; i < iMax; i++)
	{
		sprintf(temp,"%-s",GetDataModel()->circle_points[0].points[i].point_id);
		sPoint = temp;
		
		sprintf(temp,"%-d",i+1);
		sNo = temp;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, sNo));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i, sPoint));

		sta = GetDataModel()->circle_points[0].points[i].sta;

		if(sta == 1 || sta == 2) // measured or design
		{
			sprintf(temp,"%+9.*f", m_pDCP05Model->m_nDecimals, GetDataModel()->circle_points[0].points[i].diameter - GetDataModel()->circle_points[0].diameter);
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

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Deviation, (short) i, sDev));
	}

	// ja valitse piste missä max dev
	short pno=1;
	double rms=0.0;
	/*
	//double maxdist = get_max_dist_and_rms_line(&GetDataModel()->line_buff[0],&pno,&rms);
	//poMultiColCtrl->SetSelectedId(pno-1);
	
	// set title with rms and point count
	short iCount = m_pCommon->defined_points_count_in_line(&GetDataModel()->line_buff[0],0);
	StringC sTemp = sTitle;
	sTemp.Format(sTemp, m_pDCP05Model->m_nDecimals,rms,iCount);
	SetTitleStr(sTemp);
	*/
	//EndDraw();
}

/************************************************************************
*************************************************************************/
double DCP::DCP05ResCircleDlgC::get_max_dist_and_rms_line(S_LINE_BUFF *line, short *pno, double *rms/*, short ACT*/)
{

short i, count=0;

double max=0.0, dist, dist2=0.0, dist3;


	for(i=0; i < MAX_POINTS_IN_LINE; i++)
	{

		if(line[0].points[i].sta == 1 || line[0].points[i].sta == 2)
		{
			count++;

			/* Calculate distance */

			dist = calc_pdist(line, i/*, ACT*/);

			dist3 = fabs(dist);

			dist2 = dist2 + (dist3 * dist3);

			if(count == 1)
			{
				*pno = i+1;
				max = dist;

			}
			if(dist3 > fabs(max))
			{
				max = dist;
				*pno = i+1;
			}
		}
	}
	if(count > 0)
		*rms = sqrt(dist2 / count);
	else
		*rms = 99.9;

	return max;
}



/************************************************************************
*************************************************************************/
double DCP::DCP05ResCircleDlgC::calc_pdist(S_LINE_BUFF *line, short pno/*, short ACT*/)
{
struct ams_vector m;
struct line wline;
double dist;

			m.x = line[0].points[pno].x;
			m.y = line[0].points[pno].y;
			m.z = line[0].points[pno].z;

			wline.px = line[0].px;
			wline.py = line[0].py;
			wline.pz = line[0].pz;

			wline.ux = line[0].ux;
			wline.uy = line[0].uy;
			wline.uz = line[0].uz;

			/* Calculate distance */
			dist = calc_point_dist_from_line(&m, &wline);

			return dist;
}
void DCP::DCP05ResCircleDlgC::UpdateData()
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
bool DCP::DCP05ResCircleDlgC::SetModel( GUI::ModelC* pModel )
{
	  // Verify type
    DCP::DCP05CircleModelC* pDCP05Model = dynamic_cast< DCP::DCP05CircleModelC* >( pModel );

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

// Description: Hello World model
DCP::DCP05CircleModelC* DCP::DCP05ResCircleDlgC::GetDataModel() const
{
    return (DCP::DCP05CircleModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::DCP05ResCircleControllerC::DCP05ResCircleControllerC(DCP::DCP05ModelC *pDCP05Model, short iDisplay)
    : m_pDlg( NULL ),m_pDCP05Model(pDCP05Model), m_iDisplay(iDisplay)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_DEV_OF_CIRCLE_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	StringC sTitle;
	sTitle = L"Dev of circle RMS:";
	sTitle += L"Points:";
	SetTitle(sTitle);

    // Create a dialog
     m_pDlg = new DCP::DCP05ResCircleDlgC(pDCP05Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( RES_CIRCLE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP05;
    vDef.poOwner = this;
	
	// moved into dialog
	//vDef.nLable = K_DCP_REJECT_TOK;
	//SetFunctionKey( FK1, vDef );

	if(m_iDisplay != SHAFT_DLG)
	{
		vDef.strLable = StringC(AT_DCP05,K_DCP_SAVE_TOK);
		SetFunctionKey( FK3, vDef );
	}


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


DCP::DCP05ResCircleControllerC::~DCP05ResCircleControllerC()
{

}

// Description: Route model to everybody else
bool DCP::DCP05ResCircleControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	//Verify type
    DCP::DCP05CircleModelC* pDataModel = dynamic_cast< DCP::DCP05CircleModelC* >( pModel );

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

// SAVE
void DCP::DCP05ResCircleControllerC::OnF3Pressed() 
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	if(GetController(CIRCLE_SAVE_CONTROLLER) == NULL)
	{
		(void)AddController( CIRCLE_SAVE_CONTROLLER, new DCP::DCP05SaveCircleControllerC(m_pDlg->GetDataModel(),m_pDCP05Model) );
	}

	(void)GetController(CIRCLE_SAVE_CONTROLLER)->SetTitle(StringC(AT_DCP05,T_DCP_CIRCLE_SAVE_TOK));
	(void)GetController( CIRCLE_SAVE_CONTROLLER )->SetModel(m_pDCP05Model);
	SetActiveController(CIRCLE_SAVE_CONTROLLER, true);
	
}


// CONT
void DCP::DCP05ResCircleControllerC::OnF6Pressed() 
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
void DCP::DCP05ResCircleControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP05ResCircleControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == CIRCLE_SAVE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		Close(EC_KEY_CONT);
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// Instantiate template classes
/*
DCP::DCP05ResCircleModelC::DCP05ResCircleModelC()
{
	//memset(&sel_points[0],0,sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
	//memset(nro_table,0,sizeof(short) * MAX_POINTS_IN_FILE*2);
	max_dist	= 0.0;
	rms			= 0.0;
	diameter	= 0.0;

}


DCP::DCP05ResCircleModelC::~DCP05ResCircleModelC()
{
}
*/

