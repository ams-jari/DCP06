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
#include <dcp06/orientation/ResBestFit.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/calculation/CalculationBestFit.hpp>

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
DCP::ResBestFitDialog::ResBestFitDialog(DCP::BestFitModel *pModel):poMultiColCtrl(nullptr),m_pInfo1(nullptr),
	//m_pMultiColCtrlObserver(OBS_METHOD_TO_PARAM0(SelectOnePointDialog, OnChanged), this),
	m_pDataModel(pModel)
{
	//SetTxtApplicationId(AT_DCP06);
	
	// load title
	sTitle.LoadTxt(AT_DCP06,T_DCP_RES_OF_POM_TOK);
	SetTitle(sTitle);
	
	
}
// Description: Destructor
DCP::ResBestFitDialog::~ResBestFitDialog()
{
	 if(m_pCommon)
	 {
		delete m_pCommon;
		m_pCommon = 0;
	 }
}

void DCP::ResBestFitDialog::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();
	
	m_pCommon = new Common(GetModel());

	GUI::DesktopC::Instance()->UpdateFunctionKeys();

	/*
	GUI::ListConfigColsC oConfigCols1;
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_No));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Point));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_X));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Y));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_Z));
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
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_X_TOK),CI_X));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_Y_TOK),CI_Y));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_Z_TOK),CI_Z));

	poMultiColCtrl->AddColSelection(CI_No, CI_Point, CI_X, CI_Y, CI_Z);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_REJECT_TOK);
	SetFunctionKey( FK1, vDef );
	
	//SetHelpTok(H_DCP_POM_RES_TOK, 0);
	//m_pMultiColCtrlObserver.Attach(poMultiColCtrl->GetSubject());
}



void DCP::ResBestFitDialog::OnF1Pressed(void)
{
	
	short iSelectedId = poMultiColCtrl->GetSelectedId();
	StringC sTemp;
		
	short staOCS = m_pDataModel->point_OCS[iSelectedId].sta;
	short staDCS = m_pDataModel->point_DCS[iSelectedId].sta;
	
	short iMin = 3;

	if(m_pDataModel->pom_chst == 0)
		iMin = 3;
	else
	{
		iMin = (!m_pCommon->get_rotation_status() && GetModel()->dom_hz_plane) ? 2 : 3;
	}
	
	
	//if(GetDataModel()->line_buff[0].points[iSelectedId].sta == 1 || GetDataModel()->line_buff[0].points[iSelectedId].sta == 2)
	if((staOCS == 1 || staOCS == 2) && (staDCS == 1 || staDCS == 2))
	{
		if(m_pCommon->get_OCS_SCS_points_count(&m_pDataModel->point_OCS[0],&m_pDataModel->point_DCS[0], MAX_BESTFIT_POINTS) <= iMin)
		{
			StringC strText;
			strText.LoadTxt(AT_DCP06,M_DCP_CANNOT_REJECT_PNT_TOK);
			MsgBox msgbox;
			
			msgbox.ShowMessageOk(strText);
			return;
		}					
	}
	/*					
	if(sta == 0)
	{
		return;
	}
	*/
	if((staOCS == 1 || staOCS == 2) && (staDCS == 1 || staDCS == 2))
	{
		m_pDataModel->point_OCS[iSelectedId].sta = 3;
		m_pDataModel->point_DCS[iSelectedId].sta = 3;
	}
	else if(
		((m_pDataModel->point_OCS[iSelectedId].sta == 3 || m_pDataModel->point_OCS[iSelectedId].sta == 4) &&
		(m_pDataModel->point_DCS[iSelectedId].sta == 3 || m_pDataModel->point_DCS[iSelectedId].sta == 4)) ||

		((m_pDataModel->point_OCS[iSelectedId].sta == 3 || m_pDataModel->point_OCS[iSelectedId].sta == 4) &&
		(m_pDataModel->point_DCS[iSelectedId].sta == 1 || m_pDataModel->point_DCS[iSelectedId].sta == 2)) ||

		((m_pDataModel->point_OCS[iSelectedId].sta == 1 || m_pDataModel->point_OCS[iSelectedId].sta == 2) &&
		(m_pDataModel->point_DCS[iSelectedId].sta == 3 || m_pDataModel->point_DCS[iSelectedId].sta == 4)) 
        )								
						{
							m_pDataModel->point_OCS[iSelectedId].sta = 2;
							m_pDataModel->point_DCS[iSelectedId].sta = 2;	
						}					

		
	CalcBestFit calcpom(m_pDataModel,GetModel());
	if(calcpom.calc_transform())
	{
		m_pDataModel->calculated = true;

	}
	RefreshControls();		
}

void DCP::ResBestFitDialog::OnDialogActivated()
{
	RefreshControls();
} 


//*********************************************************************************
//
//*********************************************************************************
short DCP::ResBestFitDialog::get_max_res()
{
short i, count=0, pno=1;
double max=0.0, dist; //, dist2=0.0;

	for(i=0; i < MAX_BESTFIT_POINTS; i++)
	{
		if(m_pDataModel->point_RES[i].sta == 1 || m_pDataModel->point_RES[i].sta == 2)
		{
			count++;

			// Calculate distance
			// PrintLn("Sqrt");
			dist = 	sqrt(	m_pDataModel->point_RES[i].x * m_pDataModel->point_RES[i].x +
							m_pDataModel->point_RES[i].y * m_pDataModel->point_RES[i].y +
							m_pDataModel->point_RES[i].z * m_pDataModel->point_RES[i].z);
			// PrintLn("Sqrt out");
			if(count == 0)
			{
				pno = i+1;
				max = dist;
			}
			if(dist > max)
			{
				max = dist;
				pno = i+1;
			}
		}
	}
	return pno;
}

// Description: refresh all controls
void DCP::ResBestFitDialog::RefreshControls()
{
	
	short iMax=0;
	short iPno;

	if(!poMultiColCtrl)
		return;
	iMax = m_pCommon->get_last_defined_point(&m_pDataModel->point_OCS[0],MAX_BESTFIT_POINTS);
	iPno = get_max_res();
	// 
	//BeginDraw(); 
	char temp[20];
	StringC sPoint;
	StringC sNo;
	StringC sDev;
	StringC sX;
	StringC sY;
	StringC sZ;
	int sta;

	poMultiColCtrl->DestroyAll();

	for(int i=0; i < iMax; i++)
	{
		sta = m_pDataModel->point_RES[i].sta;

		sprintf(temp,"%-s",m_pDataModel->point_OCS[i].point_id);
		sPoint = temp;
		
		sprintf(temp,"%-d",i+1);
		sNo = temp;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short) i, sNo));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Point, (short) i, sPoint));

		

		if(sta == 1 || sta == 2) // measured or design
		{
			sprintf(temp,"%+.*f", GetModel()->m_nDecimals, m_pDataModel->point_RES[i].x);
			sX = temp;

			sprintf(temp,"%+.*f", GetModel()->m_nDecimals, m_pDataModel->point_RES[i].y);
			sY = temp;

			sprintf(temp,"%+.*f", GetModel()->m_nDecimals, m_pDataModel->point_RES[i].z);
			sZ = temp;


		}
		else if (sta == 0)
		{
			sX = L" ";
			sY = L" ";
			sZ = L" ";

		}
		else // rejected
		{
			sX = L"-";
			sY = L"-";
			sZ = L"-";
		}

		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_X, (short) i, sX));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Y, (short) i, sY));
		USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Z, (short) i, sZ));
	}

	// ja valitse piste miss� max dev
	poMultiColCtrl->SetSelectedId(iPno-1);
	/*	
	// set title with rms and point count
	short iCount = m_pCommon->defined_points_count_in_line(&GetDataModel()->line_buff[0],0);
	StringC sTemp = sTitle;
	sTemp.Format(sTemp, m_pModel->m_nDecimals,rms,iCount);
	SetTitleStr(sTemp);
	*/
	//EndDraw();
}

/************************************************************************
*************************************************************************/
/*
double DCP::ResBestFitDialog::get_max_dist_and_rms_line(S_LINE_BUFF *line, short *pno, double *rms)
{

short i, count=0;

double max=0.0, dist, dist2=0.0, dist3;


	for(i=0; i < MAX_POINTS_IN_LINE; i++)
	{

		if(line[0].points[i].sta == 1 || line[0].points[i].sta == 2)
		{
			count++;

			// Calculate distance 

			dist = calc_pdist(line, i);

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

*/

/************************************************************************
*************************************************************************/
/*
double DCP::ResBestFitDialog::calc_pdist(S_LINE_BUFF *line, short pno)
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

			//* Calculate distance 
			dist = calc_point_dist_from_line(&m, &wline);

			return dist;
}
*/
void DCP::ResBestFitDialog::UpdateData()
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
bool DCP::ResBestFitDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::ResBestFitDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


// ******************************************************************************

DCP::ResBestFitController::ResBestFitController(DCP::BestFitModel *pModel)
    : m_pDlg( nullptr ),m_pDataModel(pModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    //SetTitleTok( AT_DCP06, T_DCP_3D_FILE_TOK /*C_DCP_APPLICATION_NAME_TOK */);
	StringC sTitle;
	sTitle = L"Dev of line RMS:";
	sTitle += L"Points:";
	SetTitle(sTitle);

    // Create a dialog
     m_pDlg = new DCP::ResBestFitDialog(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( RES_BESTFIT_DLG, m_pDlg, true );

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

DCP::ResBestFitController::~ResBestFitController()
{

}

// Description: Route model to everybody else
bool DCP::ResBestFitController::SetModel( GUI::ModelC* pModel )
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
void DCP::ResBestFitController::OnF6Pressed() 
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
void DCP::ResBestFitController::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::ResBestFitController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



