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
#include <dcp06/orientation/DefineLineUserDef.hpp>
//#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>

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

DCP::DCP06DefineLineUserDefDlgC::DCP06DefineLineUserDefDlgC(DCP::DCP06ModelC* pDCP06Model):m_pLine(0),
		m_pDCP06Model(pDCP06Model),m_pPoints(0),m_pLinePoints(0)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);


	m_strXLine.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	m_strYLine.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	m_strZLine.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);

}


            // Description: Destructor
DCP::DCP06DefineLineUserDefDlgC::~DCP06DefineLineUserDefDlgC()
{

}

void DCP::DCP06DefineLineUserDefDlgC::OnInitDialog(void)
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
	AddCtrl(m_pLine);
	
	//InsertEmptyLine(unHeight);
	m_pLinePoints = new GUI::TextCtrlC();
	m_pLinePoints->SetId(eLinePoints);
	m_pLinePoints->SetText(StringC(AT_DCP06,T_DCP_LINE_POINTS_TOK));
	AddCtrl(m_pLinePoints);


	//	m_pPoints = new GUI::TextBoxCtrlC(0,10,300,120); CAPTIVATE
	m_pPoints = new GUI::TextCtrlC();
	m_pPoints->SetId(ePoints);
	AddCtrl(m_pPoints);
	/*
	m_pLineInfo1 = new GUI::TextCtrlC();
	m_pLineInfo1->SetId(eLineInfo1);
	m_pLineInfo1->SetTextTok(P_DCP_REF_LINE_EXP1_TOK);
	AddCtrl(m_pLineInfo1);

	m_pLineInfo2 = new GUI::TextCtrlC();
	m_pLineInfo2->SetId(eLineInfo2);
	m_pLineInfo2->SetTextTok(P_DCP_REF_LINE_EXP2_TOK);
	AddCtrl(m_pLineInfo2);
	
	m_pLineInfo3 = new GUI::TextCtrlC();
	m_pLineInfo3->SetId(eLineInfo3);
	m_pLineInfo3->SetTextTok(P_DCP_REF_LINE_EXP3_TOK);
	AddCtrl(m_pLineInfo3);
	*/
	//SetHelpTok(H_DCP_USERDEF_LINE_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP06DefineLineUserDefDlgC::OnDialogActivated()
{

	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06DefineLineUserDefDlgC::RefreshControls()
{	
	if(m_pLine && m_pPoints && m_pLinePoints)	
	{
		int i = 0;

		// seleted point count
		int selectedCount = 0;
		for( i= 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(GetDataModel()->userdef_line_points_no[i] != 0)
				selectedCount++;
		}
		
		int lastMeasuredPoints = 0;
		
		for( i= 0; i < MAX_USERDEF_POINTS; i++)
		{
			if(GetDataModel()->userdef_measured_points[i].sta != 0)
				lastMeasuredPoints = i+1;
		}

		StringC sTemp;
		StringC sStatus(L"-");
		if(selectedCount >= 2)
			sStatus = L"+";

		if(GetDataModel()->lineModel->active_line == X_LINE)
		{
			sTemp = m_strXLine;
		}
		else if(GetDataModel()->lineModel->active_line == Y_LINE)
		{
			sTemp = m_strYLine;
		}
		else if(GetDataModel()->lineModel->active_line == Z_LINE)
		{
			sTemp = m_strZLine;
		}
		sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);
	
		StringC sMsg;
		//sMsg.LoadTxt(AT_DCP06,T_DCP_LINE_POINTS_TOK);
		//sMsg += "\n";
		int pno = 0;
		for( i= 0; i < 20; i++)
		{
			pno = GetDataModel()->userdef_line_points_no[i];
			StringC text;
			if(pno != 0)
				text.Format(L"%2d: %2d/%2d\n", i+1,GetDataModel()->userdef_line_points_no[i],lastMeasuredPoints);
			else
				text.Format(L"%2d: %2.2s\n", i+1,"-");
			sMsg += text;
		}
		m_pPoints->SetText(sMsg);
	}
}

void DCP::DCP06DefineLineUserDefDlgC::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::DCP06DefineLineUserDefDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06DefineLineUserDefModelC* pDCP06Model = dynamic_cast< DCP::DCP06DefineLineUserDefModelC* >( pModel );

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
DCP::DCP06DefineLineUserDefModelC* DCP::DCP06DefineLineUserDefDlgC::GetDataModel() const
{
    return (DCP::DCP06DefineLineUserDefModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::DCP06DefineLineUserDefDlgC::delete_line()
{
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_LINE_TOK);
	DCP06MsgBoxC MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->lineModel->active_line	= X_LINE;
		memset(&GetDataModel()->lineModel->line_buff[0], 0, sizeof(S_LINE_BUFF));
		memset(&GetDataModel()->userdef_line_points_no[0],0, sizeof(short) * MAX_USERDEF_POINTS);
		RefreshControls();
	}
}
// ================================================================================================
void DCP::DCP06DefineLineUserDefDlgC:: x_line()
{
	GetDataModel()->lineModel->active_line = X_LINE;
	RefreshControls();
}

// ================================================================================================
void DCP::DCP06DefineLineUserDefDlgC:: y_line()
{
	GetDataModel()->lineModel->active_line = Y_LINE;
	RefreshControls();
}
// ================================================================================================
void DCP::DCP06DefineLineUserDefDlgC::z_line()
{
	GetDataModel()->lineModel->active_line = Z_LINE;
	RefreshControls(); 
}


// ================================================================================================
// ====================================  DCP06ControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UnitControllerC
// 
DCP::DCP06DefineLineUserDefControllerC::DCP06DefineLineUserDefControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_USERDEF_LINE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP06DefineLineUserDefDlgC(pDCP06Model);  //lint !e1524 new in constructor for class 
    (void)AddDialog( LINE_USERDEF_DLG, m_pDlg, true );
	
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


DCP::DCP06DefineLineUserDefControllerC::~DCP06DefineLineUserDefControllerC()
{

}
// Description: Route model to everybody else
bool DCP::DCP06DefineLineUserDefControllerC::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	DCP::DCP06DefineLineUserDefModelC* pLineModel = dynamic_cast< DCP::DCP06DefineLineUserDefModelC* >( pModel );
	if(pLineModel->lineModel->active_plane == XY_PLANE)
		HideFunctionKey(FK3);
	else if(pLineModel->lineModel->active_plane == YZ_PLANE)
		HideFunctionKey(FK1);
	else
		HideFunctionKey(FK2);

	GUI::DesktopC::Instance()->UpdateFunctionKeys();

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::DCP06DefineLineUserDefControllerC::OnF1Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->x_line();
}
void DCP::DCP06DefineLineUserDefControllerC::OnF2Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->y_line();

}
void DCP::DCP06DefineLineUserDefControllerC::OnF3Pressed()
{	
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->z_line();

}


void DCP::DCP06DefineLineUserDefControllerC::OnF5Pressed()
{	
	if (m_pDlg == NULL)
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
	pModel->m_iMinPoint = 2;
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
void DCP::DCP06DefineLineUserDefControllerC::OnF6Pressed()
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

void DCP::DCP06DefineLineUserDefControllerC::OnSHF2Pressed()
{	
	m_pDlg->delete_line();
}

// Description: React on close of tabbed dialog
void DCP::DCP06DefineLineUserDefControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == RES_LINE_DLG)
	{
		DCP06MsgBoxC msgBox;
		msgBox.ShowMessageOk("OK");
	}
}

// Description: React on close of controller
void DCP::DCP06DefineLineUserDefControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{	
	if(lCtrlID == SELECT_MULTIPOINTS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectMultiPointsModelC* pModel = (DCP::DCP06SelectMultiPointsModelC*) GetController( SELECT_MULTIPOINTS_CONTROLLER )->GetModel();		
		DCP06CommonC common(m_pDCP06Model);
		
		// delete line
		memset(&m_pDlg->GetDataModel()->lineModel->line_buff[0], 0, sizeof(S_LINE_BUFF));

		// delete earlier selected points

		memset(&m_pDlg->GetDataModel()->userdef_line_points_no[0], 0, sizeof(short) * MAX_USERDEF_POINTS);
		
		short iSelected = 0;

		for(short i=0;i<pModel->m_iMaxPoint;i++)
		{
				m_pDlg->GetDataModel()->userdef_line_points_no[i] = pModel->nro_table[i][0];

				if(m_pDlg->GetDataModel()->userdef_line_points_no[i] != 0)
					iSelected++;
		}

		if(iSelected < 2)
		{
			DCP06MsgBoxC msgbox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_IN_MIN_2_POINTS_TOK);
			msgbox.ShowMessageOk(sMsg);
		}
	}
	
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================

DCP::DCP06DefineLineUserDefModelC::DCP06DefineLineUserDefModelC()
{
	lineModel = new DCP::DCP06DefineLineModelC;
}

DCP::DCP06DefineLineUserDefModelC::~DCP06DefineLineUserDefModelC()
{
	if(lineModel != 0)
	{
		delete lineModel;
		lineModel = 0;
	}
}
