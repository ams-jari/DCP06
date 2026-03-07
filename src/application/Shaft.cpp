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
#include <dcp06/application/ShaftLine.hpp>
#include <dcp06/application/Shaft.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/core/ResShaft.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>

#include <GUI_DeskTop.hpp>
#include "calc.h"
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

DCP::ShaftDialog::ShaftDialog(DCP::ShaftModel* pModel):m_pLine(0),m_pCircle(0),
		m_pShaftModel(pModel)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);


	m_strXLine.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	m_strYLine.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	m_strZLine.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);
	m_strMeasLine.LoadTxt(AT_DCP06,V_DCP_MEASURED_LINE_TOK);


}


            // Description: Destructor
DCP::ShaftDialog::~ShaftDialog()
{

}

void DCP::ShaftDialog::OnInitDialog(void)
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
	
	//InsertEmptyLine(); CAPTIVATE

	
	m_pCircle = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCircle->SetId(eCircle);
	m_pCircle->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_TOK));
	//m_pCircle->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pCircle);
	/*
	m_pLineInfo1 = new GUI::TextCtrlC();
	m_pLineInfo1->SetId(eLineInfo1);
	m_pLineInfo1->SetTextTok(L_DCP_SHAFT_LINE1_TOK);
	AddCtrl(m_pLineInfo1);

	m_pLineInfo2 = new GUI::TextCtrlC();
	m_pLineInfo2->SetId(eLineInfo2);
	m_pLineInfo2->SetTextTok(L_DCP_SHAFT_LINE2_TOK);
	AddCtrl(m_pLineInfo2);
	
	m_pLineInfo3 = new GUI::TextCtrlC();
	m_pLineInfo3->SetId(eLineInfo3);
	m_pLineInfo3->SetTextTok(L_DCP_SHAFT_LINE3_TOK);
	AddCtrl(m_pLineInfo3);
	*/
	//SetHelpTok(H_DCP_SHAFT_TOK,0);
	
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::ShaftDialog::OnDialogActivated()
{

	RefreshControls();
}

// Description: refresh all controls
void DCP::ShaftDialog::RefreshControls()
{	
	if(m_pLine && m_pCircle)	
	{
		StringC sTemp;
		StringC sStatus(L"-");
		
		if(m_pShaftModel->line_buff[0].calc)
			sStatus = L"+";

		if(m_pShaftModel->active_line == X_LINE)
		{
			sTemp = m_strXLine;
		}
		else if(m_pShaftModel->active_line == Y_LINE)
		{
			sTemp = m_strYLine;
		}
		else if(m_pShaftModel->active_line == Z_LINE)
		{
			sTemp = m_strZLine;
		}
		else if(m_pShaftModel->active_line == MEAS_LINE)
		{
			sTemp = m_strMeasLine;
			sTemp += sStatus;
		}
		else
			sTemp = L"-";
		//sTemp += sStatus;

		m_pLine->GetStringInputCtrl()->SetString(sTemp);

		sStatus = L"-";
		if(m_pShaftModel->shaft_circle_points[0].calc)
			sStatus = L"+";

		m_pCircle->GetStringInputCtrl()->SetString(sStatus);
	
	}
}

void DCP::ShaftDialog::UpdateData()
{
	memcpy( &GetModel()->shaft_circle_points[0],&m_pShaftModel->shaft_circle_points[0], sizeof(S_CIRCLE_BUFF));
	memcpy( &GetModel()->shaft_circle_points_in_plane[0], &m_pShaftModel->shaft_circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));
	GetModel()->shaft_circle_cx = m_pShaftModel->shaft_circle_cx;
	GetModel()->shaft_circle_cy = m_pShaftModel->shaft_circle_cy ;
	GetModel()->shaft_circle_cz = m_pShaftModel->shaft_circle_cz;
	GetModel()->shaft_circle_vi = m_pShaftModel->shaft_circle_vi;
	GetModel()->shaft_circle_vj = m_pShaftModel->shaft_circle_vj;
	GetModel()->shaft_circle_vk = m_pShaftModel->shaft_circle_vk;
	GetModel()->shaft_ref_line = m_pShaftModel->active_line ;
	memcpy(&GetModel()->shaft_line[0], &m_pShaftModel->line_buff[0], sizeof(S_LINE_BUFF));
	GetModel()->shaft_circle_diameter = m_pShaftModel->shaft_circle_diameter;
	GetModel()->shaft_circle_rms = m_pShaftModel->shaft_circle_rms ;


}


// Description: only accept DCP06 Model objects
bool DCP::ShaftDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::ShaftDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::ShaftDialog::delete_line()
{
	/*
	StringC strMsg;
	strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_POINTS_OF_LINE_TOK);
	MsgBox MsgBox;
	if(MsgBox.ShowMessageYesNo(strMsg))
	{
		GetDataModel()->active_line	= X_LINE;
		memset(&GetDataModel()->line_buff[0], 0, sizeof(S_LINE_BUFF));
		RefreshControls();
	}
	*/
}
// ================================================================================================
void DCP::ShaftDialog:: x_line()
{
	/*
	GetDataModel()->active_line = X_LINE;
	RefreshControls();
	*/
}

// ================================================================================================
void DCP::ShaftDialog:: y_line()
{
	/*
	GetDataModel()->active_line = Y_LINE;
	RefreshControls();
	*/
}
// ================================================================================================
void DCP::ShaftDialog::z_line()
{
	/*
	GetDataModel()->active_line = Z_LINE;
	RefreshControls(); 
	*/
}
// ================================================================================================
bool DCP::ShaftDialog::CalculateLineAfterMeas()
{
	/*
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
	*/
	return false;
}

// ================================================================================================
// ====================================  Controller  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// UnitController
// 
DCP::ShaftController::ShaftController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_SHAFT_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	m_pShaftModel = new ShaftModel();

	memcpy(&m_pShaftModel->shaft_circle_points[0], &m_pModel->shaft_circle_points[0], sizeof(S_CIRCLE_BUFF));
	memcpy(&m_pShaftModel->shaft_circle_points_in_plane[0], &m_pModel->shaft_circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));
	m_pShaftModel->shaft_circle_cx = m_pModel->shaft_circle_cx;
	m_pShaftModel->shaft_circle_cy = m_pModel->shaft_circle_cy;
	m_pShaftModel->shaft_circle_cz = m_pModel->shaft_circle_cz;
	m_pShaftModel->shaft_circle_vi = m_pModel->shaft_circle_vi;
	m_pShaftModel->shaft_circle_vj = m_pModel->shaft_circle_vj;
	m_pShaftModel->shaft_circle_vk = m_pModel->shaft_circle_vk;

	m_pShaftModel->active_line = m_pModel->shaft_ref_line;
	memcpy(&m_pShaftModel->line_buff[0],&m_pModel->shaft_line[0], sizeof(S_LINE_BUFF));
	m_pShaftModel->shaft_circle_diameter = m_pModel->shaft_circle_diameter;
	m_pShaftModel->shaft_circle_rms = m_pModel->shaft_circle_rms;

	/*
	// TEST MUISTA POISTAA
	m_pShaftModel->line_buff[0].points[0].x = 0.0;
	m_pShaftModel->line_buff[0].points[0].y = 0.0;
	m_pShaftModel->line_buff[0].points[0].z = 0.0;
	m_pShaftModel->line_buff[0].points[0].sta  = 1;

	m_pShaftModel->line_buff[0].points[1].x = 0.0;
	m_pShaftModel->line_buff[0].points[1].y = 1.0;
	m_pShaftModel->line_buff[0].points[1].z = 1.0;
	m_pShaftModel->line_buff[0].points[1].sta  = 1;

	m_pModel->shaft_circle_points[0].points[0].x = 4.0;
	m_pModel->shaft_circle_points[0].points[0].y = 0.0;
	m_pModel->shaft_circle_points[0].points[0].z = 0.0;
	m_pModel->shaft_circle_points[0].points[0].sta = 1;

	m_pModel->shaft_circle_points[0].points[1].x = 5.0;
	m_pModel->shaft_circle_points[0].points[1].y = -1.0;
	m_pModel->shaft_circle_points[0].points[1].z = 0.0;
	m_pModel->shaft_circle_points[0].points[1].sta = 1;


	m_pModel->shaft_circle_points[0].points[2].x = 5.0;
	m_pModel->shaft_circle_points[0].points[2].y = 1.0;
	m_pModel->shaft_circle_points[0].points[2].z = 0.0;
	m_pModel->shaft_circle_points[0].points[2].sta = 1;
	*/
	
    // Create a dialog
    m_pDlg = new DCP::ShaftDialog(m_pShaftModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( SHAFT_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
    //vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
	SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_CIRCLE_TOK);
    SetFunctionKey( FK3, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
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


DCP::ShaftController::~ShaftController()
{

}
// Description: Route model to everybody else
bool DCP::ShaftController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	DCP::Model* pLineModel = dynamic_cast< DCP::Model* >( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::ShaftController::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	
	ShaftLineModel *pModel = new ShaftLineModel();
	memcpy(&pModel->line_buff[0], &m_pShaftModel->line_buff[0], sizeof(S_LINE_BUFF));
	pModel->active_line = m_pShaftModel->active_line;
	
	if(GetController(SHAFT_ALIGMENT_LINE_CONTROLLER) == nullptr)
	{
		(void)AddController( SHAFT_ALIGMENT_LINE_CONTROLLER, new DCP::ShaftLineController(m_pDlg->GetModel()));
	}

	(void)GetController( SHAFT_ALIGMENT_LINE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SHAFT_ALIGMENT_LINE_CONTROLLER, true);
}
void DCP::ShaftController::OnF2Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->y_line();

}
void DCP::ShaftController::OnF3Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	// call circle dialog
	DCP::PointBuffModel* pModel = new PointBuffModel;



	//pModel->display = SHAFT_DLG;
	if(GetController(CIRCLE_CONTROLLER) == nullptr)
	{
		(void)AddController( CIRCLE_CONTROLLER, new DCP::CircleController(m_pModel, SHAFT_DLG));
	}
	(void)GetController( CIRCLE_CONTROLLER )->SetModel(pModel);
	SetActiveController(CIRCLE_CONTROLLER, true);

 

}


void DCP::ShaftController::OnF5Pressed()
{	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	if(m_pShaftModel->calc_shaft())
	{
		
			if(GetController(RES_SHAFT_CONTROLLER) == nullptr)
			{
				(void)AddController( RES_SHAFT_CONTROLLER, new DCP::ResShaftController(m_pModel, m_pShaftModel) );
			}

			(void)GetController(RES_SHAFT_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DEV_OF_SHAFT_TOK));
			(void)GetController( RES_SHAFT_CONTROLLER )->SetModel(m_pModel);
			SetActiveController(RES_SHAFT_CONTROLLER, true);
		}
}
// Description: Handle change of position values
void DCP::ShaftController::OnF6Pressed()
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

void DCP::ShaftController::OnSHF2Pressed()
{	
	
		StringC strDomText;
		strDomText.LoadTxt(AT_DCP06,L_DCP_3DSHAFT_ALIGMENT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strDomText);
		
		MsgBox msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			m_pShaftModel->active_line =0; 
			memset(&m_pShaftModel->line_buff[0],0,sizeof(S_LINE_BUFF));
			memset(&m_pShaftModel->shaft_circle_points[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&m_pShaftModel->shaft_circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&m_pShaftModel->calc_plane[0],0,sizeof(S_PLANE_BUFF));

			m_pShaftModel->shaft_circle_cx = 0.0;
			m_pShaftModel->shaft_circle_cy = 0.0;
			m_pShaftModel->shaft_circle_cz = 0.0;
			m_pShaftModel->shaft_circle_vi = 0.0;
			m_pShaftModel->shaft_circle_vj = 0.0;
			m_pShaftModel->shaft_circle_vk = 0.0;
			m_pShaftModel->shaft_circle_diameter = 0.0;
			m_pShaftModel->shaft_circle_rms = 0.0;
			m_pShaftModel->centerOfCircleDist = 0.0;
			m_pShaftModel->angleLines = 0.0;
			// delete from m_pModel-> !!!!!!!!!!!!!!!!!!!!!!

			m_pDlg->RefreshControls();
		}
}

// Description: React on close of tabbed dialog
void DCP::ShaftController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == RES_LINE_DLG)
	{
		MsgBox msgBox;
		msgBox.ShowMessageOk("OK");
	}
}

// Description: React on close of controller
void DCP::ShaftController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SHAFT_ALIGMENT_LINE_CONTROLLER && lExitCode ==EC_KEY_CONT)
	{
		DCP::ShaftLineModel* pModel = (DCP::ShaftLineModel*) GetController( SHAFT_ALIGMENT_LINE_CONTROLLER )->GetModel();	

		m_pShaftModel->active_line = pModel->active_line;
		memcpy(&m_pShaftModel->line_buff[0], &pModel->line_buff[0], sizeof(S_LINE_BUFF));
	}

	if(lCtrlID == CIRCLE_CONTROLLER && lExitCode ==EC_KEY_CONT)
	{
		DCP::PointBuffModel* pModel = (DCP::PointBuffModel*) GetController( CIRCLE_CONTROLLER )->GetModel();	


		memcpy(&m_pShaftModel->shaft_circle_points[0], &m_pModel->shaft_circle_points[0], sizeof(S_CIRCLE_BUFF));
		memcpy(&m_pShaftModel->shaft_circle_points_in_plane[0], &m_pModel->shaft_circle_points_in_plane[0], sizeof(S_CIRCLE_BUFF));
		m_pShaftModel->shaft_circle_cx = m_pModel->shaft_circle_cx;
		m_pShaftModel->shaft_circle_cy = m_pModel->shaft_circle_cy;
		m_pShaftModel->shaft_circle_cz = m_pModel->shaft_circle_cz;
		m_pShaftModel->shaft_circle_vi = m_pModel->shaft_circle_vi;
		m_pShaftModel->shaft_circle_vj = m_pModel->shaft_circle_vj;
		m_pShaftModel->shaft_circle_vk = m_pModel->shaft_circle_vk;

		m_pShaftModel->shaft_circle_rms = m_pModel->shaft_circle_rms;
		m_pShaftModel->shaft_circle_diameter = m_pModel->shaft_circle_diameter;
	}


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::ShaftModel::ShaftModel()
{
	memset(&line_buff[0],0, sizeof(S_LINE_BUFF));
	memset(&calc_plane[0],0, sizeof(S_PLANE_BUFF));
	memset(&shaft_circle_points[0],0, sizeof(S_CIRCLE_BUFF));
	memset(&shaft_circle_points_in_plane[0],0, sizeof(S_CIRCLE_BUFF));

	/*	
	x = 0.0;
	y= 0.0;
	z= 0.0;
	cx= 0.0;
	cy= 0.0;
	cz= 0.0;
	rms = 0.0;
	diameter= 0.0;
	*/
	active_line = X_LINE;

	shaft_circle_cx = 0.0;
	shaft_circle_cy = 0.0;
	shaft_circle_cz = 0.0;
	shaft_circle_vi = 0.0;
	shaft_circle_vj = 0.0;
	shaft_circle_vk = 0.0;

	shaft_circle_diameter = 0.0;
	shaft_circle_rms = 0.0;

	centerOfCircleDist = 0.0;
	angleLines = 0.0;


	

}

DCP::ShaftModel::~ShaftModel()
{
}


short DCP::ShaftModel::calc_shaft()
{
	MsgBox msgbox;
	StringC sMsg;
	short ret = 0;

	// lasketaan tason yht�l� joka kulkee ympyr�n keskipisteen kautta ja on kohtisuorassa
	// x1,x2 kulkevaa suoraa vastaan

	if(shaft_circle_points[0].calc != 0 && shaft_circle_points[0].sta > 0)
	{
		// tason piste on ympyr�n keskipiste
		calc_plane[0].px = shaft_circle_cx;
		calc_plane[0].py = shaft_circle_cy;
		calc_plane[0].pz = shaft_circle_cz;
		
		if(active_line == X_LINE)
		{
			// normaali
			calc_plane[0].nx = 1.0;
			calc_plane[0].ny = 0.0;
			calc_plane[0].nz = 0.0;
			calc_plane[0].calc = 1;
			calc_plane[0].sta = 1;
		}
		else if(active_line == Y_LINE)
		{
			// normaali
			calc_plane[0].nx = 0.0;
			calc_plane[0].ny = 1.0;
			calc_plane[0].nz = 0.0;
			calc_plane[0].calc = 1;
			calc_plane[0].sta = 1;

		}
		else if(active_line == Z_LINE)
		{
			// normaali
			calc_plane[0].nx = 0.0;
			calc_plane[0].ny = 0.0;
			calc_plane[0].nz = 1.0;
			calc_plane[0].calc = 1;
			calc_plane[0].sta = 1;
		}
		else if(active_line == MEAS_LINE)
		{
			// normaali
			calc_plane[0].nx = line_buff[0].ux;//line_buff[0].points[1].x - line_buff[0].points[0].x;
			calc_plane[0].ny = line_buff[0].uy;//line_buff[0].points[1].y - line_buff[0].points[0].y;
			calc_plane[0].nz = line_buff[0].uz;//line_buff[0].points[1].z - line_buff[0].points[0].z;
			calc_plane[0].calc = 1;
			calc_plane[0].sta = 1;
		}
		else
		{
			sMsg.LoadTxt(AT_DCP06, M_DCP_DEFINE_REF_LINE_TOK);
			msgbox.ShowMessageOk(sMsg);
			return 0;
		}
	}
	else
	{
		sMsg.LoadTxt(AT_DCP06, M_DCP_DEFINE_SHAFT_CIRCLE_TOK);
		msgbox.ShowMessageOk(sMsg);
		return 0;
	}

	// calc distance and angle
	struct ams_vector m;
	struct line wline;

			
			m.x = shaft_circle_points->cx;
			m.y = shaft_circle_points->cy;
			m.z = shaft_circle_points->cz;

			wline.px = line_buff[0].px;
			wline.py = line_buff[0].py;
			wline.pz = line_buff[0].pz;

			wline.ux = line_buff[0].ux;
			wline.uy = line_buff[0].uy;
			wline.uz = line_buff[0].uz;

			/* Calculate distance */
			centerOfCircleDist = calc_point_dist_from_line(&m, &wline);

			// calc plane by circle_points
			S_PLANE_BUFF calc_circle_plane[1];
			memset(&calc_circle_plane[0],0, sizeof(S_PLANE_BUFF));
			for(int i = 0; i < MAX_POINTS_IN_CIRCLE; i++)
			{
				calc_circle_plane[0].points[i].x = shaft_circle_points[0].points[i].x;
				calc_circle_plane[0].points[i].y = shaft_circle_points[0].points[i].y;
				calc_circle_plane[0].points[i].z = shaft_circle_points[0].points[i].z;
				calc_circle_plane[0].points[i].sta = shaft_circle_points[0].points[i].sta;
			}
			CalcPlane laske_taso;

			laske_taso.calc(&calc_circle_plane[0], ACTUAL);

			// angle
			struct ams_vector line1,line2;
			line1.x = line_buff[0].ux;
			line1.y = line_buff[0].uy;
			line1.z = line_buff[0].uz;

			line2.x = calc_circle_plane[0].nx;
			line2.y = calc_circle_plane[0].ny;
			line2.z = calc_circle_plane[0].nz;

			angleLines = angle(&line1,&line2);
			angleLines = radtodeg(angleLines);
				
	ret = 1;
	return ret;

}