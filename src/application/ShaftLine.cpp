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
#include <dcp06/application/ShaftLine.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>

#include <GUI_DeskTop.hpp>

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

DCP::ShaftLineDialog::ShaftLineDialog(DCP::Model* pModel):m_pLine(0),
		m_pModel(pModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);


	m_strXLine.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	m_strYLine.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	m_strZLine.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);
	m_strMeasLine.LoadTxt(AT_DCP06,V_DCP_MEASURED_LINE_TOK);


}


            // Description: Destructor
DCP::ShaftLineDialog::~ShaftLineDialog()
{

}

void DCP::ShaftLineDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
    
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	m_pLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLine->SetId(eLine);
	m_pLine->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_TOK));
	//m_pLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//m_pLine->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pLine);
	
	//InsertEmptyLine(); CAPTIVATE

	m_pLineInfo1 = new GUI::TextCtrlC();
	m_pLineInfo1->SetId(eLineInfo1);
	m_pLineInfo1->SetText(StringC(AT_DCP06,L_DCP_SHAFT_LINE1_TOK));
	AddCtrl(m_pLineInfo1);

	m_pLineInfo2 = new GUI::TextCtrlC();
	m_pLineInfo2->SetId(eLineInfo2);
	m_pLineInfo2->SetText(StringC(AT_DCP06,L_DCP_SHAFT_LINE2_TOK));
	AddCtrl(m_pLineInfo2);
	
	m_pLineInfo3 = new GUI::TextCtrlC();
	m_pLineInfo3->SetId(eLineInfo3);
	m_pLineInfo3->SetText(StringC(AT_DCP06,L_DCP_SHAFT_LINE3_TOK));
	AddCtrl(m_pLineInfo3);

	//SetHelpTok(H_DCP_SHAFT_ALIGMENT_LINE_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::ShaftLineDialog::OnDialogActivated()
{

	RefreshControls();
}

// Description: refresh all controls
void DCP::ShaftLineDialog::RefreshControls()
{	
	if(m_pLine)	
	{
		StringC sTemp;
		StringC sStatus(L"-");
		if(GetDataModel()->line_buff[0].calc)
			sStatus = L"+";

		if(GetDataModel()->active_line == X_LINE)
		{
			sTemp = m_strXLine;
		}
		else if(GetDataModel()->active_line == Y_LINE)
		{
			sTemp = m_strYLine;
		}
		else if(GetDataModel()->active_line == Z_LINE)
		{
			sTemp = m_strZLine;
		}
		else if(GetDataModel()->active_line == MEAS_LINE)
		{
			sTemp = m_strMeasLine;
			sTemp += sStatus;
		}
		//sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);
	}
}

void DCP::ShaftLineDialog::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::ShaftLineDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::ShaftLineModel* pModel = dynamic_cast< DCP::ShaftLineModel* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::ShaftLineModel* DCP::ShaftLineDialog::GetDataModel() const
{
    return (DCP::ShaftLineModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::ShaftLineDialog::delete_line()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_LINE_TOK);
	MsgBox MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->active_line	= X_LINE;
		memset(&GetDataModel()->line_buff[0], 0, sizeof(S_LINE_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::ShaftLineDialog:: x_line()
{
	GetDataModel()->active_line = X_LINE;
	RefreshControls();
}

// ================================================================================================
void DCP::ShaftLineDialog:: y_line()
{
	GetDataModel()->active_line = Y_LINE;
	RefreshControls();
}
// ================================================================================================
void DCP::ShaftLineDialog::z_line()
{
	GetDataModel()->active_line = Z_LINE;
	RefreshControls(); 
}
// ================================================================================================
bool DCP::ShaftLineDialog::CalculateLineAfterMeas()
{
	for(short i=0; i < MAX_POINTS_IN_LINE; i++)
	{
		if(GetDataModel()->line_buff[0].points[i].sta == POINT_DESIGN_REJECTED)
			GetDataModel()->line_buff[0].points[i].sta = POINT_DESIGN;

		if(GetDataModel()->line_buff[0].points[i].sta == POINT_MEASURED_REJECTED)
			GetDataModel()->line_buff[0].points[i].sta = POINT_MEASURED;


	}
	CalcLine calcline;
	if(calcline.calc(&GetDataModel()->line_buff[0],ACTUAL))
	//if(calc_plane(&GetDataModel()->plane_buff[0],ACTUAL))
	{
		// SHOW RESIDUALS
		return true;
	}
	//
	
	return false;
}

// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::ShaftLineController::ShaftLineController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SHAFT_ALIGMENT_LINE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::ShaftLineDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( LINE_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
    //vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_X_TOK);
	SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_Y_TOK);
    SetFunctionKey( FK2, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_Z_TOK);
    SetFunctionKey( FK3, vDef );

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


DCP::ShaftLineController::~ShaftLineController()
{

}
// Description: Route model to everybody else
bool DCP::ShaftLineController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	DCP::ShaftLineModel* pLineModel = dynamic_cast< DCP::ShaftLineModel* >( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::ShaftLineController::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->x_line();
}
void DCP::ShaftLineController::OnF2Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->y_line();

}
void DCP::ShaftLineController::OnF3Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->z_line();

}


void DCP::ShaftLineController::OnF5Pressed()
{	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::MeasureModel* pModel = new MeasureModel;

	pModel->m_iMaxPoint = 2;
	pModel->m_iMinPoint = 2;
	//pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
	memcpy(&pModel->point_table[0],&m_pDlg->GetDataModel()->line_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);

	
	if(GetController(MEAS_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pModel) );
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_SHAFT_ALIGMENT_LINE_MEAS_TOK));
	
	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
}
// Description: Handle change of position values
void DCP::ShaftLineController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	 m_pDlg->UpdateData();
    (void)Close(EC_KEY_CONT);
	(void) Close(EC_KEY_CONT);

/*
	// call circle dialog
	DCP::PointBuffModel* pModel = new PointBuffModel;
	//pModel->display = SHAFT_DLG;
	if(GetController(CIRCLE_CONTROLLER) == nullptr)
	{
		(void)AddController( CIRCLE_CONTROLLER, new DCP::CircleController(m_pModel, SHAFT_DLG));
	}
	(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel);
	SetActiveController(CIRCLE_CONTROLLER, true);

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
*/
    //(void)Close(EC_KEY_CONT);
}

void DCP::ShaftLineController::OnSHF2Pressed()
{	
	m_pDlg->delete_line();
}

// Description: React on close of tabbed dialog
void DCP::ShaftLineController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{

}

// Description: React on close of controller
void DCP::ShaftLineController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pDlg->GetDataModel()->line_buff[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
	
		for(int i=0;i<MAX_POINTS_IN_LINE;i++)
		{
			if(m_pDlg->GetDataModel()->line_buff[0].points[i].sta == 3)
			{
				m_pDlg->GetDataModel()->line_buff[0].points[i].sta = 1;
			}
			else if(m_pDlg->GetDataModel()->line_buff[0].points[i].sta == 4)
			{
				m_pDlg->GetDataModel()->line_buff[0].points[i].sta = 2;
			}
		}
		/*
		sprintf(m_pDlg->GetDataModel()->line_buff[0].points[0].point_id,"%-s","P1");
		m_pDlg->GetDataModel()->line_buff[0].points[0].x = 0.0;
		m_pDlg->GetDataModel()->line_buff[0].points[0].y = 0.0;
		m_pDlg->GetDataModel()->line_buff[0].points[0].z = 0.0;
		m_pDlg->GetDataModel()->line_buff[0].points[0].sta = 1;

		sprintf(m_pDlg->GetDataModel()->line_buff[0].points[1].point_id,"%-s","P2");
		m_pDlg->GetDataModel()->line_buff[0].points[1].x = 1.0;
		m_pDlg->GetDataModel()->line_buff[0].points[1].y = 0.1;
		m_pDlg->GetDataModel()->line_buff[0].points[1].z = 0.1;
		m_pDlg->GetDataModel()->line_buff[0].points[1].sta = 1;

		sprintf(m_pDlg->GetDataModel()->line_buff[0].points[2].point_id,"%-s","P3");
		m_pDlg->GetDataModel()->line_buff[0].points[2].x = 2.0;
		m_pDlg->GetDataModel()->line_buff[0].points[2].y = 1.9;
		m_pDlg->GetDataModel()->line_buff[0].points[2].z = 0.0;
		m_pDlg->GetDataModel()->line_buff[0].points[2].sta = 1;

		sprintf(m_pDlg->GetDataModel()->line_buff[0].points[3].point_id,"%-s","P4");
		m_pDlg->GetDataModel()->line_buff[0].points[3].x = 3.0;
		m_pDlg->GetDataModel()->line_buff[0].points[3].y = 3.1;
		m_pDlg->GetDataModel()->line_buff[0].points[3].z = 0.1;
		m_pDlg->GetDataModel()->line_buff[0].points[3].sta = 1;
		*/
		if(GetController(CALC_LINE_CONTROLLER) == nullptr)
		{
			(void)AddController( CALC_LINE_CONTROLLER, new DCP::CalcLineController(&m_pDlg->GetDataModel()->line_buff[0],ACTUAL, 0) );
		}

		(void)GetController( CALC_LINE_CONTROLLER )->SetModel(m_pModel);
		SetActiveController(CALC_LINE_CONTROLLER, true);
	
		/*
		if(m_pDlg->CalculateLineAfterMeas())
		{
			Common common;
	
			short iCount = common.get_max_defined_point_line(&m_pDlg->GetDataModel()->line_buff[0]);
			if(iCount >=3)
			{
				if(GetController(RES_LINE_CONTROLLER) == nullptr)
				{
					(void)AddController( RES_LINE_CONTROLLER, new DCP::ResLineController(m_pModel) );
				}

				(void)GetController(RES_LINE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK);

				(void)GetController( RES_LINE_CONTROLLER )->SetModel(m_pDlg->GetDataModel());
				SetActiveController(RES_LINE_CONTROLLER, true);
			}
			else
			{
				DestroyController( lCtrlID );
				m_pDlg->UpdateData();
				(void)Close(EC_KEY_CONT);
			}
		}
		*/
	}
	if(lCtrlID == CALC_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDlg->GetDataModel()->line_buff[0].sta = m_pDlg->GetDataModel()->line_buff[0].points[0].sta;
		m_pDlg->GetDataModel()->active_line = MEAS_LINE;
		//DestroyController( lCtrlID );
		m_pDlg->UpdateData();
	   (void)Close(EC_KEY_CONT);
	   (void)Close(EC_KEY_CONT);
	}
	/*
 // CIRCLE
	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( CIRCLE_CONTROLLER )->GetModel();	
		
		m_pDlg->GetDataModel()->cx = pModel->m_pPointBuff[0].x;
		m_pDlg->GetDataModel()->cy = pModel->m_pPointBuff[0].y;
		m_pDlg->GetDataModel()->cz = pModel->m_pPointBuff[0].z;
		m_pDlg->GetDataModel()->diameter = pModel->m_pPointBuff[0].diameter;

		double point_in_normal[3];
		S_PLANE_BUFF temp_plane_table1[1];

		for(short i=0 ; i < MAX_POINTS_IN_PLANE; i++)
		{
			temp_plane_table1[0].points[i].x= m_pModel->circle_points[0].points[i].x;
			temp_plane_table1[0].points[i].y= m_pModel->circle_points[0].points[i].y;
			temp_plane_table1[0].points[i].z= m_pModel->circle_points[0].points[i].z;
			temp_plane_table1[0].points[i].sta= m_pModel->circle_points[0].points[i].sta;
			temp_plane_table1[0].points[i].xdes= m_pModel->circle_points[0].points[i].xdes;
			temp_plane_table1[0].points[i].ydes= m_pModel->circle_points[0].points[i].ydes;
			temp_plane_table1[0].points[i].zdes= m_pModel->circle_points[0].points[i].zdes;
			temp_plane_table1[0].points[i].dsta= m_pModel->circle_points[0].points[i].dsta;
			strcpy(temp_plane_table1[0].points[i].point_id,m_pModel->circle_points[0].points[i].point_id);
		}
		double xn = 0.0;
		double yn = 0.0;
		double zn = 0.0;
		CalcPlane calcplane;

		if(calcplane.calc(temp_plane_table1,ACTUAL))
		{
			xn = temp_plane_table1[0].nx;
			yn = temp_plane_table1[0].ny;
			zn = temp_plane_table1[0].nz;

			if(m_pDlg->GetDataModel()->active_line == X_LINE)
			{
				if(xn < 0.0)
				{
					xn = -xn;
					yn = -yn;
					zn = -zn;
				}
				point_in_normal[0] = 1000;
				point_in_normal[1] = 0;
				point_in_normal[2] = 0;
			}
			else if(m_pDlg->GetDataModel()->active_line == Y_LINE)
			{
				if(yn < 0.0)
				{
					yn = -xn;
					yn = -yn;
					zn = -zn;
				}
				point_in_normal[0] = 0;
				point_in_normal[1] = 1000;
				point_in_normal[2] = 0;
		
			}
			else if(m_pDlg->GetDataModel()->active_line == Z_LINE)
			{
				if(zn < 0.0)
				{
					xn = -xn;
					yn = -yn;
					zn = -zn;
				}
				point_in_normal[0] = 0;
				point_in_normal[1] = 0;
				point_in_normal[2] = 1000;
		
			}
			else if(m_pDlg->GetDataModel()->active_line == MEAS_LINE)
			{

			}
			m_pDlg->GetDataModel()->x = 1000 * xn - point_in_normal[0];
			m_pDlg->GetDataModel()->y = 1000 * yn - point_in_normal[1];
			m_pDlg->GetDataModel()->z = 1000 * zn - point_in_normal[2];
			(void)Close(EC_KEY_CONT);
		}		
		//m_pDataModel->x_temp = pModel->m_pPointBuff[0].x;
		//m_pDataModel->y_temp = pModel->m_pPointBuff[0].y;
		//m_pDataModel->z_temp= pModel->m_pPointBuff[0].z;
			
	//	m_pCommon->copy_xyz_to_buffer(&m_pDataModel->x_temp, &m_pDataModel->y_temp, &m_pDataModel->z_temp,m_pDataModel->xmea_ptr,m_pDataModel->ymea_ptr,m_pDataModel->zmea_ptr,9,m_pDlg->GetModel()->m_nDecimals);
		
		// also diamter
	//	sprintf(m_pDataModel->note_ptr,"%6.*f",m_pDlg->GetModel()->m_nDecimals,pModel->m_pPointBuff[0].diameter);
	//	m_pDataModel->save_point();
	}

	*/
	/*
	if(lCtrlID == RES_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			// status of line = status of first point of line 
			m_pDlg->GetDataModel()->line_buff[0].sta = m_pDlg->GetDataModel()->line_buff[0].points[0].sta;
			//bDone = TRUE;
			DestroyController( lCtrlID );
			m_pDlg->UpdateData();
		   (void)Close(EC_KEY_CONT);

	}
	*/
	/*
	DCP::ResLineDialog* pDialog = new DCP::ResLineDialog();
	(void) AddDialog(RES_LINE_DLG, pDialog);
	SetActiveDialog(RES_LINE_DLG, true);
	*/	
	//if(m_pDlg->CalculateLineAfterMeas())
	//		Close(EC_KEY_CONT);
	

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::ShaftLineModel::ShaftLineModel()
{
	memset(&line_buff[0],0, sizeof(S_LINE_BUFF));
	x = 0.0;
	y= 0.0;
	z= 0.0;
	cx= 0.0;
	cy= 0.0;
	cz= 0.0;
	diameter= 0.0;
	active_line = X_LINE;
}

DCP::ShaftLineModel::~ShaftLineModel()
{
}
