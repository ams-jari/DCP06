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
#include <dcp06/orientation/DefineLine.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_DeskTop.hpp>

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

DCP::DCP06DefineLineDlgC::DCP06DefineLineDlgC(DCP::DCP06ModelC* pDCP06Model):m_pLine(0),
		m_pDCP06Model(pDCP06Model)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);


	m_strXLine.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	m_strYLine.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	m_strZLine.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);

}


            // Description: Destructor
DCP::DCP06DefineLineDlgC::~DCP06DefineLineDlgC()
{

}

void DCP::DCP06DefineLineDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
    
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	m_pLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLine->SetId(eLine);
	m_pLine->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_TOK));
	// m_pLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	// m_pLine->SetTextWidth(9*50); CAPTIVATE
	AddCtrl(m_pLine);
	
	// InsertEmptyLine(); CAPTIVATE

	m_pLineInfo1 = new GUI::TextCtrlC();
	m_pLineInfo1->SetId(eLineInfo1);
	m_pLineInfo1->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_EXP1_TOK));
	AddCtrl(m_pLineInfo1);

	m_pLineInfo2 = new GUI::TextCtrlC();
	m_pLineInfo2->SetId(eLineInfo2);
	m_pLineInfo2->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_EXP2_TOK));
	AddCtrl(m_pLineInfo2);
	
	m_pLineInfo3 = new GUI::TextCtrlC();
	m_pLineInfo3->SetId(eLineInfo3);
	m_pLineInfo3->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_EXP3_TOK));
	AddCtrl(m_pLineInfo3);

	//SetHelpTok(H_DCP_DOM_LINE_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP06DefineLineDlgC::OnDialogActivated()
{

	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06DefineLineDlgC::RefreshControls()
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
		sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);
	}
}

void DCP::DCP06DefineLineDlgC::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::DCP06DefineLineDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06DefineLineModelC* pDCP06Model = dynamic_cast< DCP::DCP06DefineLineModelC* >( pModel );

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
DCP::DCP06DefineLineModelC* DCP::DCP06DefineLineDlgC::GetDataModel() const
{
    return (DCP::DCP06DefineLineModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP06DefineLineDlgC::delete_line()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_LINE_TOK);
	DCP06MsgBoxC MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->active_line	= X_LINE;
		memset(&GetDataModel()->line_buff[0], 0, sizeof(S_LINE_BUFF));
		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP06DefineLineDlgC:: x_line()
{
	GetDataModel()->active_line = X_LINE;
	RefreshControls();
}

// ================================================================================================
void DCP::DCP06DefineLineDlgC:: y_line()
{
	GetDataModel()->active_line = Y_LINE;
	RefreshControls();
}
// ================================================================================================
void DCP::DCP06DefineLineDlgC::z_line()
{
	GetDataModel()->active_line = Z_LINE;
	RefreshControls(); 
}
// ================================================================================================
bool DCP::DCP06DefineLineDlgC::CalculateLineAfterMeas()
{
	for(short i=0; i < MAX_POINTS_IN_LINE; i++)
	{
		if(GetDataModel()->line_buff[0].points[i].sta == POINT_DESIGN_REJECTED)
			GetDataModel()->line_buff[0].points[i].sta = POINT_DESIGN;

		if(GetDataModel()->line_buff[0].points[i].sta == POINT_MEASURED_REJECTED)
			GetDataModel()->line_buff[0].points[i].sta = POINT_MEASURED;


	}
	DCP06CalcLineC calcline;
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
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06DefineLineControllerC::DCP06DefineLineControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_DOM_LINE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP06DefineLineDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
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


DCP::DCP06DefineLineControllerC::~DCP06DefineLineControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06DefineLineControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	DCP::DCP06DefineLineModelC* pLineModel = dynamic_cast< DCP::DCP06DefineLineModelC* >( pModel );
	if(pLineModel->active_plane == XY_PLANE)
		HideFunctionKey(FK3);
	else if(pLineModel->active_plane == YZ_PLANE)
		HideFunctionKey(FK1);
	else
		HideFunctionKey(FK2);

	GUI::DesktopC::Instance()->UpdateFunctionKeys();

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DCP06DefineLineControllerC::OnF1Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->x_line();
}
void DCP::DCP06DefineLineControllerC::OnF2Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->y_line();

}
void DCP::DCP06DefineLineControllerC::OnF3Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->z_line();

}


void DCP::DCP06DefineLineControllerC::OnF5Pressed()
{	
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::DCP06MeasModelC* pModel = new DCP06MeasModelC;

	pModel->m_iMaxPoint = MAX_POINTS_IN_LINE;
	pModel->m_iMinPoint = 2;
	//pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
	memcpy(&pModel->point_table[0],&m_pDlg->GetDataModel()->line_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);

	
	if(GetController(MEAS_CONTROLLER) == NULL)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::DCP06MeasControllerC(m_pDCP06Model) );
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK));
	
	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
}
// Description: Handle change of position values
void DCP::DCP06DefineLineControllerC::OnF6Pressed()
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
    (void) Close(EC_KEY_CONT);
	(void) Close(EC_KEY_CONT);
}

void DCP::DCP06DefineLineControllerC::OnSHF2Pressed()
{	
	m_pDlg->delete_line();
}

// Description: React on close of tabbed dialog
void DCP::DCP06DefineLineControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == RES_LINE_DLG)
	{
		DCP06MsgBoxC msgBox;
		msgBox.ShowMessageOk("OK");
	}
}

// Description: React on close of controller
void DCP::DCP06DefineLineControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06MeasModelC* pModel = (DCP::DCP06MeasModelC*) GetController( MEAS_CONTROLLER )->GetModel();		
		
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
		if(GetController(CALC_LINE_CONTROLLER) == NULL)
		{
			(void)AddController( CALC_LINE_CONTROLLER, new DCP::DCP06CalcLineControllerC(&m_pDlg->GetDataModel()->line_buff[0],ACTUAL, 0) );
		}

		(void)GetController( CALC_LINE_CONTROLLER )->SetModel(m_pDCP06Model);
		SetActiveController(CALC_LINE_CONTROLLER, true);

		/*
		if(m_pDlg->CalculateLineAfterMeas())
		{
			DCP06CommonC common;
	
			short iCount = common.get_max_defined_point_line(&m_pDlg->GetDataModel()->line_buff[0]);
			if(iCount >=3)
			{
				if(GetController(RES_LINE_CONTROLLER) == NULL)
				{
					(void)AddController( RES_LINE_CONTROLLER, new DCP::DCP06ResLineControllerC(m_pDCP06Model) );
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
	else if(lCtrlID == CALC_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDlg->GetDataModel()->line_buff[0].sta = m_pDlg->GetDataModel()->line_buff[0].points[0].sta;
		//DestroyController( lCtrlID );
		m_pDlg->UpdateData();
		(void)Close(EC_KEY_CONT);
		(void)Close(EC_KEY_CONT);
		//this->OnF6Pressed();
	}
	
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
	DCP::DCP06ResLineDlgC* pDialog = new DCP::DCP06ResLineDlgC();
	(void) AddDialog(RES_LINE_DLG, pDialog);
	SetActiveDialog(RES_LINE_DLG, true);
	*/	
	//if(m_pDlg->CalculateLineAfterMeas())
	//		Close(EC_KEY_CONT);
	
	DestroyController( lCtrlID );

	m_pDlg->RefreshControls();
	//m_pDlg->Show();
	//GUI::ControllerC::OnActiveControllerClosed(lCtrlID, lExitCode);
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::DCP06DefineLineModelC::DCP06DefineLineModelC()
{
}

DCP::DCP06DefineLineModelC::~DCP06DefineLineModelC()
{
}
