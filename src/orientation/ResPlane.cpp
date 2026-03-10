// YKSI PISTE ACTUAL TAI DESIGN
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
#include <dcp06/orientation/ResPlane.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>

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

//OBS_IMPLEMENT_EXECUTE(DCP::SelectOnePointDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit
DCP::ResPlaneDialog::ResPlaneDialog(DCP::Model *pModel):poMultiColCtrl(nullptr),m_pInfo1(nullptr),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(SelectOnePointDialog, OnChanged), this),
	m_pModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
	
	// load title
	sTitle.LoadTxt(AT_DCP06,T_DCP_DEV_OF_PLANE_TOK);

	m_pCommon = new Common(pModel);
}
// Description: Destructor
DCP::ResPlaneDialog::~ResPlaneDialog()
{
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::ResPlaneDialog::OnInitDialog(void)
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
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NUMBER_TOK), CI_No));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_ID_TOK),CI_Point));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_DEVIATION_TOK),CI_Deviation));
	
	poMultiColCtrl->AddColSelection(CI_No, CI_Point, CI_Deviation);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_REJECT_TOK);
	SetFunctionKey( FK1, vDef );
	
	//SetHelpTok(H_DCP_DEV_OF_PLANE_TOK,0);
	//m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());
}



void DCP::ResPlaneDialog::OnF1Pressed(void)
{
	
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	StringC sTemp;
		
	short sta = GetDataModel()->plane_buff[0].points[iSelectedId].sta;
	
	if(GetDataModel()->plane_buff[0].points[iSelectedId].sta == 1 || GetDataModel()->plane_buff[0].points[iSelectedId].sta == 2)
	{
		if(m_pCommon-> defined_points_count_in_plane(&GetDataModel()->plane_buff[0],nullptr) <= 3)
		{
			StringC strText;
			strText.LoadTxt(AT_DCP06,M_DCP_CANNOT_REJECT_PNT_TOK);
			MsgBox msgbox;
			
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
		GetDataModel()->plane_buff[0].points[iSelectedId].sta = 3;
	}
	else if(sta == 2)
	{
		GetDataModel()->plane_buff[0].points[iSelectedId].sta = 4;
	}
	else if(sta == 3)
	{
		GetDataModel()->plane_buff[0].points[iSelectedId].sta = 1;
	}
	else if(sta == 4)
	{
		GetDataModel()->plane_buff[0].points[iSelectedId].sta = 2;
	}
	
	CalcPlane calcplane;
	calcplane.calc(&GetDataModel()->plane_buff[0],ACTUAL);
	
	RefreshControls();		
}

void DCP::ResPlaneDialog::OnDialogActivated()
{
	RefreshControls();
} 

// Description: refresh all controls
void DCP::ResPlaneDialog::RefreshControls()
{
	int iMax=0;
	int i;

	if(!poMultiColCtrl)
		return;
	for(i=MAX_POINTS_IN_PLANE-1;i >=0; i--)
	{
		if(GetDataModel()->plane_buff[0].points[i].sta != 0 ||
		   !m_pCommon->strblank(GetDataModel()->plane_buff[0].points[i].point_id))
		{
			iMax = i+1;
			break;
		}
	}
	
	// 
	//BeginDraw(); 
	char cell_text_buf[POINT_ID_BUFF_LEN];
	StringC sPoint;
	StringC sNo;
	StringC sDev;
	int sta;

	poMultiColCtrl->DestroyAll();

	for(i=0; i < iMax; i++)
	{
		snprintf(cell_text_buf, sizeof(cell_text_buf), DCP_POINT_ID_FMT, GetDataModel()->plane_buff[0].points[i].point_id);
		sPoint = cell_text_buf;
		
		sprintf(cell_text_buf,"%-d",i+1);
		sNo = cell_text_buf;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, sNo));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i, sPoint));

		sta = GetDataModel()->plane_buff[0].points[i].sta;

		if(sta == 1 || sta == 2) // measured or design
		{
			sprintf(cell_text_buf,"%+9.*f", m_pModel->m_nDecimals, calc_pldist(&GetDataModel()->plane_buff[0],i));
			sDev = cell_text_buf;
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

	// ja miss� maxdev
	short pno=1;
	double rms=0.0;
	double maxdist = get_max_dist_and_rms_plane(&GetDataModel()->plane_buff[0],&pno,&rms);
	poMultiColCtrl->SetSelectedId(pno-1);

	// set title with rms and point count
	short iCount = m_pCommon->defined_points_count_in_plane(&GetDataModel()->plane_buff[0],0);
	StringC sTemp = sTitle;
	sTemp.Format(sTemp, m_pModel->m_nDecimals,rms,iCount);
	SetTitle(sTemp);

	
	//EndDraw();
}

/************************************************************************
*************************************************************************/
double DCP::ResPlaneDialog::get_max_dist_and_rms_plane(S_PLANE_BUFF *plane, short *pno, double *rms/*, short ACT*/)
{

short i, count=0;
double max=0.0, dist, dist2=0.0,dist3;
	
	for(i=0; i < MAX_POINTS_IN_PLANE; i++)
	{
		if(plane->points[i].sta == 1 || plane->points[i].sta == 2)
		{
			count++;
			/* Calculate distance */
			dist = calc_pldist(plane, i/*,ACT*/);
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
	if (count > 0)
		*rms = sqrt(dist2 / count);
	else
		*rms = 99.9;

	return max;
}



/************************************************************************
*************************************************************************/
double DCP::ResPlaneDialog::calc_pldist(S_PLANE_BUFF *plane, short pno/*, short ACT*/)
{
struct ams_vector m;
struct plane wplane;
double dist;

			m.x = plane->points[pno].x;
			m.y = plane->points[pno].y;
			m.z = plane->points[pno].z;

			wplane.px = plane->px;
			wplane.py = plane->py;
			wplane.pz = plane->pz;

			wplane.nx = plane->nx;
			wplane.ny = plane->ny;
			wplane.nz = plane->nz;

			/* Calculate distance */
			dist = calc_point_dist_from_plane(&m, &wplane);
			return dist;

}
void DCP::ResPlaneDialog::UpdateData()
{
	/*
	StringC sSelected;
	short iSelected = poMultiColCtrl->GetSelectedId()+1;
	//poMultiColCtrl->GetCellStr(CI_Selected,iSelected, sSelected);
	//GetDataModel()->ADFFileName = sSelected;
	GetDataModel()->iSelectedNo = iSelected;
	*/
		
}
// Description: only accept DCP06 Model objects
bool DCP::ResPlaneDialog::SetModel( GUI::ModelC* pModel )
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
DCP::DefinePlaneModel* DCP::ResPlaneDialog::GetDataModel() const
{
    return (DCP::DefinePlaneModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::ResPlaneController::ResPlaneController(DCP::Model *pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DEV_OF_PLANE_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	StringC sTitle;
	sTitle = L"Dev of line RMS:";
	sTitle += L"Points:";
	SetTitle(sTitle);

    // Create a dialog
     m_pDlg = new DCP::ResPlaneDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( RES_PLANE_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	// moved into dialog
	//vDef.nLable = K_DCP_REJECT_TOK;
	//SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
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

DCP::ResPlaneController::~ResPlaneController()
{

}

// Description: Route model to everybody else
bool DCP::ResPlaneController::SetModel( GUI::ModelC* pModel )
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




// CONT
void DCP::ResPlaneController::OnF6Pressed() 
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


// Description: React on close of tabbed dialog
void DCP::ResPlaneController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::ResPlaneController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// Instantiate template classes
DCP::ResPlaneModel::ResPlaneModel()
{
	//memset(&sel_points[0],0,sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
	//memset(nro_table,0,sizeof(short) * MAX_POINTS_IN_FILE*2);
}


DCP::ResPlaneModel::~ResPlaneModel()
{
}


