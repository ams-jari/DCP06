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
#include <dcp06/core/Logger.hpp>
//#include <dcp06/application/ShaftLine.hpp>
#include <dcp06/application/LineFitting.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/calculation/CalculationLineFit.hpp>
#include <dcp06/application/LineFitMeas.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
//#include <dcp06/core/ResShaft.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/PointBuffModel.hpp>
//#include <dcp06/measurement/Circle.hpp>
//#include <dcp06/calculation/calcplane.hpp>
#include <dcp06/orientation/ResLine.hpp>
#include <GUI_DeskTop.hpp>
#include "calc.h"
#include <dcp06/core/InputText.hpp>

#include <UTL_StringFunctions.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::LineFitDialog);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// Unit

DCP::LineFitDialog::LineFitDialog(LineFitModel* pModel,CalcLineFit* calcLineFit):m_pLine(0),m_pUsedHeight(0), m_pRefLine(0),
		m_pShiftLine(0), m_pRotateLine(0),m_pEnteredHeight(0), m_pShiftValue(0), m_pRotateValue(0),m_pLineFitModel(pModel),m_pCalcLineFit(calcLineFit),
		m_pUsedHeightObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		m_pShiftLineObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		m_pRotateLineObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		
		m_pManualHeightObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		m_pShiftValueObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		m_pRotateAngleObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),
		m_pRefLineObserver(OBS_METHOD_TO_PARAM0(LineFitDialog, OnValueChanged), this),

		GUI::ModelHandlerC(),GUI::StandardDialogC()
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);


	m_strXLine.LoadTxt(AT_DCP06,V_DCP_X_LINE_TOK);
	m_strYLine.LoadTxt(AT_DCP06,V_DCP_Y_LINE_TOK);
	m_strZLine.LoadTxt(AT_DCP06,V_DCP_Z_LINE_TOK);
	m_strMeasLine.LoadTxt(AT_DCP06,V_DCP_MEASURED_LINE_TOK);


}


            // Description: Destructor
DCP::LineFitDialog::~LineFitDialog()
{
 
}

void DCP::LineFitDialog::OnInitDialog(void)
{
	DCP06_TRACE_ENTER;
	GUI::BaseDialogC::OnInitDialog();
	
    
	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	m_pRefLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pRefLine->SetId(eRefLine);
	m_pRefLine->SetText(StringC(AT_DCP06,P_DCP_LINE_FITTING_USED_REF_LINE_TOK));

		m_pRefLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06, V_DCP_ROTATE_LINE_HOR_TOK),		REF_LINE_HORIZONTAL);
		m_pRefLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06, V_DCP_ROTATE_LINE_VER_TOK),		REF_LINE_VERTICAL);

	AddCtrl(m_pRefLine);

	m_pLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pLine->SetId(eLine);
	m_pLine->SetText(StringC(AT_DCP06,P_DCP_REF_LINE_TOK));
	//m_pLine->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pLine->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	AddCtrl(m_pLine);
	
	//InsertEmptyLine(); CAPTIVATE

	

	m_pUsedHeight = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pUsedHeight->SetId(eUsedHeight);
	m_pUsedHeight->SetText(StringC(AT_DCP06,P_DCP_LINE_FITTING_USED_HEIGHT_TOK));
	m_pUsedHeight->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_FIRST_POINT_TOK),		FIRST_POINT);
	m_pUsedHeight->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_LAST_POINT_TOK),		LAST_POINT);
	m_pUsedHeight->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_INTERPOLATION_TOK),		INTERPOLATION);
	m_pUsedHeight->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_MANUALLY_ENTERED_TOK), 	MANUALLY_ENTERED);
	AddCtrl(m_pUsedHeight);
	
	m_pEnteredHeight = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pEnteredHeight->SetId(eEnteredHeight);
	m_pEnteredHeight->SetText(StringC(AT_DCP06,P_DCP_MANUAL_HEIGHT_TOK));
	m_pEnteredHeight->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pEnteredHeight->SetEmptyAllowed(true);

	m_pEnteredHeight->SetCtrlState( m_pLineFitModel->selectedHeight != MANUALLY_ENTERED ? GUI::BaseCtrlC::CS_Disabled : GUI::BaseCtrlC::CS_ReadWrite);
	//m_pEnteredHeight->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	AddCtrl(m_pEnteredHeight);

	m_pShiftLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pShiftLine->SetId(eShiftLine);
	m_pShiftLine->SetText(StringC(AT_DCP06,P_DCP_SHIFT_LINE_TOK));
	m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_NO_TOK),		SHIFT_NO);
	m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_RIGHT_TOK),	SHIFT_RIGHT);
	m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_LEFT_TOK),	SHIFT_LEFT);

	if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
	{
		m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_UP_TOK),		SHIFT_UP);
		m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_DOWN_TOK), 	SHIFT_DOWN);
	}
	else
	{
		m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_FORWARD_TOK),	SHIFT_FORWARD);
		m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_BACKWARD_TOK), 	SHIFT_BACKWARD);
	}
	/*m_pShiftLine->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);*/
	AddCtrl(m_pShiftLine);

	m_pShiftValue = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pShiftValue->SetId(eShiftValue);
	m_pShiftValue->SetText(StringC(AT_DCP06,P_DCP_SHIFT_VALUE_TOK));
	m_pShiftValue->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetModel()->m_nDecimals);
	m_pShiftValue->SetEmptyAllowed(true);
	m_pShiftValue->SetCtrlState( m_pLineFitModel->selectedShift != SHIFT_NO ? GUI::BaseCtrlC::CS_ReadWrite : GUI::BaseCtrlC::CS_Disabled);
	//m_pShiftValue->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	AddCtrl(m_pShiftValue);

	m_pRotateLine = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_ComboBox);
	m_pRotateLine->SetId(eRotateLine);
	m_pRotateLine->SetText(StringC(AT_DCP06,P_DCP_ROTATE_LINE_TOK));
	m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_LINE_NO_TOK),		ROTATE_NO);
	
	if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
	{
		m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_LINE_HOR_TOK),		ROTATE_HORIZONTAL);
		m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_LINE_VER_TOK),		ROTATE_VERTICAL);
	}
	else
	{
		m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_LEFT_RIGHT_TOK),		ROTATE_VERTICAL_LEFT_RIGHT);
		m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_DEPTH_TOK),		ROTATE_VERTICAL_DEPTH);
	}
	//m_pRotateLine->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	AddCtrl(m_pRotateLine);

	m_pRotateValue = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pRotateValue->SetId(eRotateValue);
	m_pRotateValue->SetText(StringC(AT_DCP06,P_DCP_ROTATE_VALUE_TOK));
	m_pRotateValue->GetFloatInputCtrl()->SetDecimalPlaces(6);
	m_pRotateValue->GetFloatInputCtrl()->SetRange(-45.000000,45.000000);
	m_pRotateValue->SetEmptyAllowed(true);
	m_pRotateValue->SetCtrlState(m_pLineFitModel->selectedRotate != ROTATE_NO ? GUI::BaseCtrlC::CS_ReadWrite : GUI::BaseCtrlC::CS_Disabled);
	//m_pRotateValue->SetCtrlState(GUI::BaseCtrlC::CS_Disabled);
	AddCtrl(m_pRotateValue);

	m_pUsedHeightObserver.Attach(m_pUsedHeight->GetSubject());
	m_pShiftLineObserver.Attach(m_pShiftLine->GetSubject());
	m_pRotateLineObserver.Attach(m_pRotateLine->GetSubject());

	m_pRefLineObserver.Attach(m_pRefLine->GetSubject());
	m_pManualHeightObserver.Attach(m_pEnteredHeight->GetSubject());
	m_pShiftValueObserver.Attach(m_pShiftValue->GetSubject());
	m_pRotateAngleObserver.Attach(m_pRotateValue->GetSubject());

	//m_pCircle = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	//m_pCircle->SetId(eCircle);
	//m_pCircle->SetText(StringC(AT_DCP06,P_DCP_CIRCLE_TOK));
	////m_pCircle->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	//void(m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	//void(m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//AddCtrl(m_pCircle);
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
	DCP06_TRACE_EXIT;
}
void DCP::LineFitDialog::OnValueChanged( int unNotifyCode,  int ulParam2)
{
	if(unNotifyCode == GUI::NC_ONCOMBOBOX_SELECTION_CHANGED)
	{
		if(ulParam2 == eUsedHeight)
		{
			int ret = m_pUsedHeight->GetComboBoxInputCtrl()->GetSelectedId();
			m_pLineFitModel->selectedHeight = ret;
			m_pEnteredHeight->SetCtrlState(ret != MANUALLY_ENTERED ? GUI::BaseCtrlC::CS_Disabled : GUI::BaseCtrlC::CS_ReadWrite);
		}
		else if(ulParam2 == eShiftLine)
		{
			int ret = m_pShiftLine->GetComboBoxInputCtrl()->GetSelectedId();
			m_pLineFitModel->selectedShift = ret;	
			m_pShiftValue->SetCtrlState(ret != SHIFT_NO ? GUI::BaseCtrlC::CS_ReadWrite : GUI::BaseCtrlC::CS_Disabled);
		}
		else if(ulParam2 == eRotateLine)
		{
			int ret = m_pRotateLine->GetComboBoxInputCtrl()->GetSelectedId();
			m_pLineFitModel->selectedRotate = ret;
		
			m_pRotateValue->SetCtrlState(ret != ROTATE_NO ? GUI::BaseCtrlC::CS_ReadWrite : GUI::BaseCtrlC::CS_Disabled);
		}
		else if(ulParam2 == eRefLine)
		{
			int ret = m_pRefLine->GetComboBoxInputCtrl()->GetSelectedId();
			m_pLineFitModel->selectedRefLine = ret;

			if(ret == REF_LINE_HORIZONTAL)
			{
				m_pShiftLine->GetComboBoxInputCtrl()->RemoveItem(SHIFT_UP);
				m_pShiftLine->GetComboBoxInputCtrl()->RemoveItem(SHIFT_DOWN);

				m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_UP_TOK),		SHIFT_UP);
				m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_DOWN_TOK), 	SHIFT_DOWN);

				m_pRotateLine->GetComboBoxInputCtrl()->RemoveItem(ROTATE_HORIZONTAL);
				m_pRotateLine->GetComboBoxInputCtrl()->RemoveItem(ROTATE_VERTICAL);
				m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_LINE_HOR_TOK),		ROTATE_HORIZONTAL);
				m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_LINE_VER_TOK),		ROTATE_VERTICAL);

			}
			else
			{
				m_pShiftLine->GetComboBoxInputCtrl()->RemoveItem(SHIFT_UP);
				m_pShiftLine->GetComboBoxInputCtrl()->RemoveItem(SHIFT_DOWN);

				m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_FORWARD_TOK),	SHIFT_FORWARD);
				m_pShiftLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_SHIFT_BACKWARD_TOK), 	SHIFT_BACKWARD);

				m_pRotateLine->GetComboBoxInputCtrl()->RemoveItem(ROTATE_HORIZONTAL);
				m_pRotateLine->GetComboBoxInputCtrl()->RemoveItem(ROTATE_VERTICAL);
				m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_LEFT_RIGHT_TOK),		ROTATE_VERTICAL_LEFT_RIGHT);
				m_pRotateLine->GetComboBoxInputCtrl()->AddItem(StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_DEPTH_TOK),		ROTATE_VERTICAL_DEPTH);
			}
			
			m_pCalcLineFit->CalcLineFitDom(m_pLineFitModel->align321Model, &m_pLineFitModel->line_buff[0],m_pLineFitModel->selectedRefLine);
 
		}
		m_pCalcLineFit->CalcAllPoints(&m_pLineFitModel->line_buff[0],
								&m_pLineFitModel->points_buff[0],
								m_pLineFitModel->linefit_results,
								m_pLineFitModel->manualHeight,
								m_pLineFitModel->shiftValue,
								m_pLineFitModel->rotateAngle,
								m_pLineFitModel->selectedHeight,
								m_pLineFitModel->selectedShift,
								m_pLineFitModel->selectedRotate,
								m_pLineFitModel->align321Model,
								&m_pLineFitModel->line_ocs[0],
								&m_pLineFitModel->points_in_line[0],
								m_pLineFitModel->selectedRefLine);
		RefreshControls();
	}
	else if(unNotifyCode== GUI::NC_ONEDITMODE_LEFT)
	{
		if(ulParam2 == eEnteredHeight)
		{
			m_pLineFitModel->manualHeight = m_pEnteredHeight->GetFloatInputCtrl()->GetDouble();
		}
		else if(ulParam2 == eShiftValue)
		{
			m_pLineFitModel->shiftValue = m_pShiftValue->GetFloatInputCtrl()->GetDouble();
		}
		else if(ulParam2 == eRotateValue)
		{
			m_pLineFitModel->rotateAngle = m_pRotateValue->GetFloatInputCtrl()->GetDouble();
		}
		
		m_pCalcLineFit->CalcAllPoints(&m_pLineFitModel->line_buff[0],
								&m_pLineFitModel->points_buff[0],
								m_pLineFitModel->linefit_results,
								m_pLineFitModel->manualHeight,
								m_pLineFitModel->shiftValue,
								m_pLineFitModel->rotateAngle,
								m_pLineFitModel->selectedHeight,
								m_pLineFitModel->selectedShift,
								m_pLineFitModel->selectedRotate,
								m_pLineFitModel->align321Model, &m_pLineFitModel->line_ocs[0],
								&m_pLineFitModel->points_in_line[0],
								m_pLineFitModel->selectedRefLine);
		RefreshControls();
	}
	
	
}

void DCP::LineFitDialog::OnDialogActivated()
{

	RefreshControls();
}

// Description: refresh all controls
void DCP::LineFitDialog::RefreshControls()
{	
	if(m_pLine && m_pUsedHeight && m_pShiftLine && m_pRotateLine && m_pEnteredHeight && m_pShiftValue && m_pRotateValue && m_pRefLine)	
	{

		m_pUsedHeight->GetComboBoxInputCtrl()->SetSelectedId(m_pLineFitModel->selectedHeight);
		m_pShiftLine->GetComboBoxInputCtrl()->SetSelectedId(m_pLineFitModel->selectedShift);
		m_pRotateLine->GetComboBoxInputCtrl()->SetSelectedId(m_pLineFitModel->selectedRotate);
		m_pRefLine->GetComboBoxInputCtrl()->SetSelectedId(m_pLineFitModel->selectedRefLine);

		int ret = m_pUsedHeight->GetComboBoxInputCtrl()->GetSelectedId();
		
		if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
		{
				m_pUsedHeight->SetText(StringC(AT_DCP06,P_DCP_LINE_FITTING_USED_HEIGHT_TOK));
				m_pEnteredHeight->SetText(StringC(AT_DCP06,P_DCP_MANUAL_HEIGHT_TOK));
		}
		else
		{
				m_pUsedHeight->SetText(StringC(AT_DCP06,P_DCP_LINE_FITTING_USED_DEPTH_TOK));
				m_pEnteredHeight->SetText(StringC(AT_DCP06,P_DCP_LINEFIT_DEPTH_TOK));
		}

		if(ret != MANUALLY_ENTERED) {
			m_pEnteredHeight->GetFloatInputCtrl()->SetEmpty();
		}
		else {
			m_pEnteredHeight->GetFloatInputCtrl()->SetDouble(m_pLineFitModel->manualHeight);
		}


		ret = m_pShiftLine->GetComboBoxInputCtrl()->GetSelectedId();
		
		if(ret == SHIFT_NO) {
	     	m_pShiftValue->GetFloatInputCtrl()->SetEmpty();
		}
		else {
			m_pShiftValue->GetFloatInputCtrl()->SetDouble(m_pLineFitModel->shiftValue);
		}

		ret = m_pRotateLine->GetComboBoxInputCtrl()->GetSelectedId();
		
		if(ret == ROTATE_NO) {
			m_pRotateValue->GetFloatInputCtrl()->SetEmpty();
		}
		else {
			m_pRotateValue->GetFloatInputCtrl()->SetDouble(m_pLineFitModel->rotateAngle);
		}

		StringC sStatus(L"-");
		if(m_pLineFitModel->line_buff[0].calc && m_pLineFitModel->align321Model->calculated)
			sStatus = L"+";
		m_pLine->GetStringInputCtrl()->SetString(sStatus);

		
		

		//m_pEnteredHeight->SetCtrlState(ret != MANUALLY_ENTERED ? GUI::BaseCtrlC::CtrlStateT::CS_Disabled : GUI::BaseCtrlC::CtrlStateT::CS_ReadWrite);
	//	StringC sTemp;
	//	StringC sStatus(L"-");
	//	
	//	if(m_pLineFitModel->line_buff[0].calc)
	//		sStatus = L"+";

	//	if(m_pLineFitModel->active_line == X_LINE)
	//	{
	//		sTemp = m_strXLine;
	//	}
	//	else if(m_pLineFitModel->active_line == Y_LINE)
	//	{
	//		sTemp = m_strYLine;
	//	}
	//	else if(m_pLineFitModel->active_line == Z_LINE)
	//	{
	//		sTemp = m_strZLine;
	//	}
	//	else if(m_pLineFitModel->active_line == MEAS_LINE)
	//	{
	//		sTemp = m_strMeasLine;
	//		sTemp += sStatus;
	//	}
	//	else
	//		sTemp = L"-";
	//	//sTemp += sStatus;

	//	m_pLine->GetStringInputCtrl()->SetString(sTemp);

	//	sStatus = L"-";
	//	if(m_pLineFitModel->LineFit_circle_points[0].calc)
	//		sStatus = L"+";

	//	m_pCircle->GetStringInputCtrl()->SetString(sStatus);
	//
	}
}

void DCP::LineFitDialog::UpdateData()
{
	memcpy(&GetModel()->linefitting_line[0], &m_pLineFitModel->line_buff[0], sizeof(S_LINE_BUFF));
	memcpy(&GetModel()->linefitting_points,&m_pLineFitModel->points_buff[0],   sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	memcpy(&GetModel()->linefitting_results[0],&m_pLineFitModel->linefit_results[0],  sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);

	GetModel()->linefitting_manualHeight = m_pLineFitModel->manualHeight;
	GetModel()->linefitting_rotateAngle = m_pLineFitModel->rotateAngle;
	GetModel()->linefitting_shiftValue = m_pLineFitModel->shiftValue;
	GetModel()->linefitting_selectedHeight = m_pLineFitModel->selectedHeight;
	GetModel()->linefitting_selectedRotate = m_pLineFitModel->selectedRotate;
	GetModel()->linefitting_selectedShift = m_pLineFitModel->selectedShift;
	GetModel()->linefitting_selectedRefLine = m_pLineFitModel->selectedRefLine;
	
	GetModel()->poConfigController->GetModel()->SetConfigKey(CNF_KEY_LINE_FITTING);
	GetModel()->poConfigController->StoreConfigData();
}


// Description: only accept DCP06 Model objects
bool DCP::LineFitDialog::SetModel( GUI::ModelC* pModel )
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
DCP::Model* DCP::LineFitDialog::GetModel() const
{
    return (DCP::Model*) ModelHandlerC::GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
void DCP::LineFitDialog::delete_line()
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
void DCP::LineFitDialog:: x_line()
{
	/*
	GetDataModel()->active_line = X_LINE;
	RefreshControls();
	*/
}

// ================================================================================================
void DCP::LineFitDialog:: y_line()
{
	/*
	GetDataModel()->active_line = Y_LINE;
	RefreshControls();
	*/
}
// ================================================================================================
void DCP::LineFitDialog::z_line()
{
	/*
	GetDataModel()->active_line = Z_LINE;
	RefreshControls(); 
	*/
}
// ================================================================================================
bool DCP::LineFitDialog::CalculateLineAfterMeas()
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
DCP::LineFitController::LineFitController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_LINE_FITTING_TOK /*C_DCP_APPLICATION_NAME_TOK */));
	
	common = new Common(pModel);

	calcLineFit = new CalcLineFit(pModel);

	m_pLineFitModel = new LineFitModel();

	m_pLineFitModel->align321Model->old_active_coodinate_system = pModel->active_coodinate_system;

	// set active coordinate system to scs
	pModel->active_coodinate_system = DCS;

	memcpy(&m_pLineFitModel->line_buff[0],  &pModel->linefitting_line[0], sizeof(S_LINE_BUFF));
	memcpy(&m_pLineFitModel->points_buff[0],  &pModel->linefitting_points, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	memcpy(&m_pLineFitModel->linefit_results[0],  &pModel->linefitting_results[0], sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);

	m_pLineFitModel->manualHeight = pModel->linefitting_manualHeight ;
	m_pLineFitModel->rotateAngle = pModel->linefitting_rotateAngle;
	m_pLineFitModel->shiftValue= pModel->linefitting_shiftValue;
	m_pLineFitModel->selectedHeight = pModel->linefitting_selectedHeight;
	m_pLineFitModel->selectedRotate = pModel->linefitting_selectedRotate;
	m_pLineFitModel->selectedShift = pModel->linefitting_selectedShift;
	m_pLineFitModel->selectedRefLine = pModel->linefitting_selectedRefLine;
	// lasketaan dom jos suora on laskettu koska emme nyt talleta matriisia.. 
	if(m_pLineFitModel->line_buff[0].calc)
	{
		calcLineFit->CalcLineFitDom(m_pLineFitModel->align321Model, &m_pLineFitModel->line_buff[0],m_pLineFitModel->selectedRefLine);
		calcLineFit->CalcAllPoints(&m_pLineFitModel->line_buff[0],
								&m_pLineFitModel->points_buff[0],
								m_pLineFitModel->linefit_results,
								m_pLineFitModel->manualHeight,
								m_pLineFitModel->shiftValue,
								m_pLineFitModel->rotateAngle,
								m_pLineFitModel->selectedHeight,
								m_pLineFitModel->selectedShift,
								m_pLineFitModel->selectedRotate,
								m_pLineFitModel->align321Model,
								&m_pLineFitModel->line_ocs[0],&m_pLineFitModel->points_in_line[0],
								m_pLineFitModel->selectedRefLine);
	}
		
    // Create a dialog
    m_pDlg = new DCP::LineFitDialog(m_pLineFitModel, calcLineFit);  //lint !e1524 new in constructor for class 
    (void)AddDialog( LINEFIT_DLG, m_pDlg, true );
	
    // Set the function key
	
    FKDef vDef;
    //vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	
	vDef.strLable = StringC(AT_DCP06,K_DCP_LINE_TOK);
	SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_POINTS_TOK);
    SetFunctionKey( FK3, vDef );

   /* vDef.strLable = StringC(AT_DCP06,K_DCP_CALC_TOK);
    SetFunctionKey( FK5, vDef );*/

	vDef.strLable = StringC(AT_DCP06,K_DCP_SAVE_TOK);
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


DCP::LineFitController::~LineFitController()
{
	if(common != 0)
	{
		delete common;
		common = 0;
	}
	
	if(calcLineFit)
	{
		delete calcLineFit;
		calcLineFit = 0;
	}

	m_pModel->active_coodinate_system = m_pLineFitModel->align321Model->old_active_coodinate_system;
}
// Description: Route model to everybody else
bool DCP::LineFitController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	DCP::Model* pLineModel = dynamic_cast< DCP::Model* >( pModel );

    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

void DCP::LineFitController::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::MeasureModel* pModel = new MeasureModel;

	pModel->m_iMaxPoint = MAX_POINTS_IN_LINE;
	pModel->m_iMinPoint = 2;
	//pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

 
	// for test
	//m_pLineFitModel->line_buff[0].points[0].x = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].z = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].sta = 1;

	//m_pLineFitModel->line_buff[0].points[1].x = 30.0;
	//m_pLineFitModel->line_buff[0].points[1].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[1].z = 0.0;
	//m_pLineFitModel->line_buff[0].points[1].sta = 1;

	//m_pLineFitModel->points_buff[0].sta = 1;
	//m_pLineFitModel->points_buff[0].x = 20;
	//m_pLineFitModel->points_buff[0].y = 10;
	//m_pLineFitModel->points_buff[0].z = 0;

	////** HOR
	//m_pLineFitModel->line_buff[0].points[0].x = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].z = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].sta = 1;

	//m_pLineFitModel->line_buff[0].points[1].x = 3134.5;
	//m_pLineFitModel->line_buff[0].points[1].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[1].z = 93.3;
	//m_pLineFitModel->line_buff[0].points[1].sta = 1;

	//m_pLineFitModel->points_buff[0].sta = 1;
	//m_pLineFitModel->points_buff[0].x = 2401.5;
	//m_pLineFitModel->points_buff[0].y = 127.2;
	//m_pLineFitModel->points_buff[0].z = -175.4;

	//m_pLineFitModel->points_buff[1].sta = 1;
	//m_pLineFitModel->points_buff[1].x = 1964.8;
	//m_pLineFitModel->points_buff[1].y = -849.8;
	//m_pLineFitModel->points_buff[1].z = -418.8;

	
	//// VER
	//m_pLineFitModel->line_buff[0].points[0].x = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].z = 0.0;
	//m_pLineFitModel->line_buff[0].points[0].sta = 1;

	//m_pLineFitModel->line_buff[0].points[1].x = 0.0;
	//m_pLineFitModel->line_buff[0].points[1].y = 0.0;
	//m_pLineFitModel->line_buff[0].points[1].z = 200.0;
	//m_pLineFitModel->line_buff[0].points[1].sta = 1;

	//m_pLineFitModel->points_buff[0].sta = 1;
	//m_pLineFitModel->points_buff[0].x = 0;
	//m_pLineFitModel->points_buff[0].y = 0;
	//m_pLineFitModel->points_buff[0].z = 100.0;

	//m_pLineFitModel->points_buff[1].sta = 1;
	//m_pLineFitModel->points_buff[1].x = 402.4;
	//m_pLineFitModel->points_buff[1].y = 0.2;
	//m_pLineFitModel->points_buff[1].z = 4.8;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
	memcpy(&pModel->point_table[0],&m_pLineFitModel->line_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);

	
	if(GetController(MEAS_CONTROLLER) == nullptr)
	{
		(void)AddController( MEAS_CONTROLLER, new DCP::MeasureController(m_pModel) );
	}
	(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_LINE_FITTING_LINE_MEAS_TOK));
	
	(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(MEAS_CONTROLLER, true);
}

void DCP::LineFitController::OnF2Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	m_pDlg->y_line();

}
void DCP::LineFitController::OnF3Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP::LineFitMeasModel* pModel = new LineFitMeasModel;

	pModel->m_iMaxPoint = MAX_LINEFIT_POINTS;
	pModel->m_iMinPoint = 1;
	//pModel->m_iPointsCount = 3;
	pModel->m_iCurrentPoint = 1;

	memset(&pModel->point_table[0],0,sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	memset(&pModel->points_in_line[0],0,sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);

	memcpy(&pModel->point_table[0],&m_pLineFitModel->points_buff[0], sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	memcpy(&pModel->points_in_line[0],&m_pLineFitModel->points_in_line[0], sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	memcpy(&pModel->linefit_results[0],&m_pLineFitModel->linefit_results[0], sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);
	
	// pass also these variables to online calc just after measurement
	pModel->pLline_buff = &m_pLineFitModel->line_buff[0];
	
	pModel->pLline_buff_ocs = &m_pLineFitModel->line_ocs[0];
	
	pModel->align321Model = m_pLineFitModel->align321Model;
	pModel->manualHeight = m_pLineFitModel->manualHeight;
	pModel->rotateAngle = m_pLineFitModel->rotateAngle;
	pModel->shiftValue = m_pLineFitModel->shiftValue;
	pModel->selectedHeight = m_pLineFitModel->selectedHeight;
	pModel->selectedShift = m_pLineFitModel->selectedShift;
	pModel->selectedRotate = m_pLineFitModel->selectedRotate;
	pModel->selectedRefLine = m_pLineFitModel->selectedRefLine;

	if(GetController(LINEFIT_MEAS_CONTROLLER) == nullptr)
	{
		(void)AddController( LINEFIT_MEAS_CONTROLLER, new DCP::LineFitMeasController(m_pModel) );
	}
	(void)GetController(LINEFIT_MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_LINE_FITTING_POINTS_TOK));
	
	(void)GetController( LINEFIT_MEAS_CONTROLLER )->SetModel(pModel);
	SetActiveController(LINEFIT_MEAS_CONTROLLER, true);

 

}

// save
void DCP::LineFitController::OnF5Pressed()
{	
	if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	if(m_pLineFitModel->line_buff[0].calc != 0 && m_pLineFitModel->align321Model->calculated)
	{
		DCP::InputTextModel* pModel = new InputTextModel;
		pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_NEW_FILENAME_TOK);
		pModel->m_StrTitle = GetTitle();
		pModel->m_iTextLength = DCP_JOB_ID_MAX_LEN;
		pModel->m_StrText = L" ";

		if ( nullptr == pModel) //lint !e774 Boolean within 'if' always evaluates to False 
		{
			USER_APP_VERIFY( false );
			return;
		}

		if(GetController(INPUT_TEXT_CONTROLLER) == nullptr)
		{
			(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::InputTextController( m_pModel));
		}

		//(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(m_pDCP06FileDlg->GetModel());
		(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
		SetActiveController(INPUT_TEXT_CONTROLLER, true);
	}
	else
	{
		GUI::DesktopC::Instance()->MessageShow(L"Nothing to save!");
	}
	/*if(m_pLineFitModel->calc_LineFit())
	{
		
			if(GetController(RES_LineFit_CONTROLLER) == nullptr)
			{
				(void)AddController( RES_LineFit_CONTROLLER, new DCP::ResLineFitController(m_pModel, m_pLineFitModel) );
			}

			(void)GetController(RES_LineFit_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DEV_OF_SHAFT_TOK));
			(void)GetController( RES_SHAFT_CONTROLLER )->SetModel(m_pModel);
			SetActiveController(RES_SHAFT_CONTROLLER, true);
		}*/
}
// Description: Handle change of position values
void DCP::LineFitController::OnF6Pressed()
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

void DCP::LineFitController::OnSHF2Pressed()
{	
	
		StringC strDomText;
		strDomText.LoadTxt(AT_DCP06,L_DCP_3DSHAFT_ALIGMENT_TOK);
		StringC strMsg;
		strMsg.LoadTxt(AT_DCP06,M_DCP_DELETE_ALL_LINE_FITTING_TOK);
		strMsg.Format(strMsg,(const wchar_t*)strDomText);
		
		MsgBox msgbox;
		if(msgbox.ShowMessageYesNo(strMsg))
		{
			memset(&m_pLineFitModel->line_buff[0],0, sizeof(S_LINE_BUFF));
			memset(&m_pLineFitModel->line_ocs[0],0, sizeof(S_LINE_BUFF));
			memset(&m_pLineFitModel->linefit_results[0], 0, sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);
			memset(&m_pLineFitModel->points_buff[0],0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
			memset(&m_pLineFitModel->points_in_line[0],0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);

			m_pLineFitModel->manualHeight = 0.0;
			m_pLineFitModel->rotateAngle = 0.0;
			m_pLineFitModel->shiftValue = 0.0;
			m_pLineFitModel->selectedHeight = FIRST_POINT;
			m_pLineFitModel->selectedShift = SHIFT_NO;
			m_pLineFitModel->selectedRotate = ROTATE_NO;
			calcLineFit->delete_align321_values(m_pLineFitModel->align321Model);

			/*m_pShaftModel->active_line =0; 
			memset(&m_pLineFitModel->line_buff[0],0,sizeof(S_LINE_BUFF));
			memset(&m_pShaftModel->shaft_circle_points[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&m_pShaftModel->shaft_circle_points_in_plane[0],0,sizeof(S_CIRCLE_BUFF));
			memset(&m_pShaftModel->calc_plane[0],0,sizeof(S_PLANE_BUFF));

			m_pShaftModel->shaft_circle_cx = 0.0;
			m_pShaftModel->shaft_circle_cy = 0.0;
			m_pShaftModel->shaft_circle_cz = 0.0;
			m_pShaftModel->shaft_circle_diameter = 0.0;
			m_pShaftModel->shaft_circle_rms = 0.0;
			m_pShaftModel->centerOfCircleDist = 0.0;
			m_pShaftModel->angleLines = 0.0;*/
			// delete from m_pModel-> !!!!!!!!!!!!!!!!!!!!!!
			
			m_pDlg->RefreshControls();
		}
}

// Description: React on close of tabbed dialog
void DCP::LineFitController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == RES_LINE_DLG)
	{
		//MsgBox msgBox;
		//msgBox.ShowMessageOk("OK");
	}
}

// Description: React on close of controller
void DCP::LineFitController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::MeasureModel* pModel = (DCP::MeasureModel*) GetController( MEAS_CONTROLLER )->GetModel();		
		
		// copy values
		memcpy(&m_pLineFitModel->line_buff[0].points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_POINTS_IN_LINE);
		
		for(int i=0;i<MAX_POINTS_IN_LINE;i++)
		{
			if(m_pLineFitModel->line_buff[0].points[i].sta == 3)
			{
				m_pLineFitModel->line_buff[0].points[i].sta = 1;
			}
			else if(m_pLineFitModel->line_buff[0].points[i].sta == 4)
			{
				m_pLineFitModel->line_buff[0].points[i].sta = 2;
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
			(void)AddController( CALC_LINE_CONTROLLER, new DCP::CalcLineController(&m_pLineFitModel->line_buff[0],ACTUAL, 0) );
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
	else if(lCtrlID == CALC_LINE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pLineFitModel->line_buff[0].sta = m_pLineFitModel->line_buff[0].points[0].sta;

		// calc dom
		calcLineFit->CalcLineFitDom(m_pLineFitModel->align321Model, &m_pLineFitModel->line_buff[0],m_pLineFitModel->selectedRefLine);

		calcLineFit->CalcAllPoints(&m_pLineFitModel->line_buff[0],
								&m_pLineFitModel->points_buff[0],
								m_pLineFitModel->linefit_results,
								m_pLineFitModel->manualHeight,
								m_pLineFitModel->shiftValue,
								m_pLineFitModel->rotateAngle,
								m_pLineFitModel->selectedHeight,
								m_pLineFitModel->selectedShift,
								m_pLineFitModel->selectedRotate,
								m_pLineFitModel->align321Model,
								&m_pLineFitModel->line_ocs[0],&m_pLineFitModel->points_in_line[0],
								m_pLineFitModel->selectedRefLine);

		//DestroyController( lCtrlID );
		//m_pDlg->UpdateData();
		//(void)Close(EC_KEY_CONT);
		//(void)Close(EC_KEY_CONT);
		//this->OnF6Pressed();
	}
	else if(lCtrlID == CALC_LINE_CONTROLLER && lExitCode != EC_KEY_CONT)
	{
		GUI::DesktopC::Instance()->MessageShow(L"Line calculation error!");
		//MsgBox msgBox;
		//msgBox.ShowMessageOk("Line calculation error!");
	}

	else if(lCtrlID == LINEFIT_MEAS_CONTROLLER)
	{
		if(lExitCode == EC_KEY_CONT)
		{
			DCP::LineFitMeasModel* pModel = (DCP::LineFitMeasModel*) GetController( LINEFIT_MEAS_CONTROLLER )->GetModel();

			
			//memcpy(&pModel->point_table[0],&m_pLineFitModel->line_buff[0].points[0], sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
			
			memcpy(&m_pLineFitModel->linefit_results[0], &pModel->linefit_results[0], sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);
			memcpy(&m_pLineFitModel->points_buff[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
			memcpy(&m_pLineFitModel->points_in_line[0], &pModel->points_in_line[0], sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);
	
			// calc ~dom
		

			/*m_pLineFitModel->manualHeight = pModel->manualHeight ;
			m_pLineFitModel->rotateAngle = pModel->rotateAngle;
			m_pLineFitModel->shiftValue = pModel->shiftValue;
			m_pLineFitModel->selectedHeight = pModel->selectedHeight;
			m_pLineFitModel->selectedShift = pModel->selectedShift;
			m_pLineFitModel->selectedRotate = pModel->selectedRotate;*/

			//DestroyController( lCtrlID );
			//m_pDlg->UpdateData();
			//(void)Close(EC_KEY_CONT);
			//(void)Close(EC_KEY_CONT);
			//this->OnF6Pressed();
		}
	}
	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::InputTextModel* pModel = (DCP::InputTextModel*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			strNewFile.Trim();
			if(strNewFile.Length() > 0)
			{
				char fname[CPI::LEN_PATH_MAX];
				BSS::UTI::BSS_UTI_WCharToAscii(strNewFile, fname);
				strcat(fname, ".lff");
				SaveLineFitting(fname);
			}

			
	}
	/*if(lCtrlID == SHAFT_ALIGMENT_LINE_CONTROLLER && lExitCode ==EC_KEY_CONT)
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
		m_pShaftModel->shaft_circle_rms = m_pModel->shaft_circle_rms;
		m_pShaftModel->shaft_circle_diameter = m_pModel->shaft_circle_diameter;
	}*/


	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

/**************************************************************
**************************************************************/
short  DCP::LineFitController::SaveLineFitting(char *fname)
{
//char apu[100];
int attr = 0;
char sPath[CPI::LEN_PATH_MAX];

	//CPI::FileIteratorC FileIterator;
	boost::filesystem::path FileInfo;

	//char filename_temp[20];
	//UTL::UnicodeToAscii(filename_temp, fname);

	bool bRet =	CPI::SensorC::GetInstance()->GetPath(m_pModel->FILE_STORAGE1, CPI::ftUserAscii, sPath);
	
	char filePathBuffer[CPI::LEN_PATH_MAX];
	filePathBuffer[0] = '\0';

	strcat(filePathBuffer, sPath);
	strcat(filePathBuffer, fname);
	char* pSearch = &filePathBuffer[0];

	//Common common(m_pModel);

	if(common->find_first_file(pSearch, &FileInfo) == 0)
	{
			MsgBox msgbox;
			StringC msg;
			msg.LoadTxt(AT_DCP06,M_DCP_DELETE_OLD_FILE_TOK);
			msg.Format(msg,StringC(fname));
			if(!msgbox.ShowMessageYesNo(msg))
			{
				return 0;
			}
			else
			{
				boost::filesystem::path f = filePathBuffer;
				int result = boost::filesystem::remove(f);
				if(!result)
				{
					StringC msg ;
					msg.Format(L"Cannot delete file %s",filePathBuffer);
					GUI::DesktopC::Instance()->MessageShow(msg);
					return 0;
				}

			}
	}

	FILE  *m_pFile = fopen(filePathBuffer, "wb+");
	if(m_pFile)
	{

		char lineBuffer[STRING_BUFFER_LARGE];
		char userBuffer[STRING_BUFFER_MEDIUM];
		char valueBuffer[STRING_BUFFER_MEDIUM];

		int iInstNo=0;

		sprintf(lineBuffer,"Line Fitting results%c%c",13,10); 
		fputs(lineBuffer,m_pFile);

		int iYear, iMonth, iDay, iHour, iMin, iSec;
		common->GetDate(&iDay, &iMonth, &iYear);
		common->GetTime(&iHour, &iMin, &iSec);

		sprintf(lineBuffer,"Date/Time:%02d.%02d.%04d %02d:%02d:%02d%c%c", iDay, iMonth, iYear, iHour, iMin, iSec,13,10); 
		fputs(lineBuffer,m_pFile);

		common->GetUserName(userBuffer);
		sprintf(lineBuffer,"User:%s%c%c", userBuffer,13,10); 
		fputs(lineBuffer,m_pFile);

		common->GetInstrumentName(userBuffer);
		common->GetInstrumentNo(&iInstNo);
		sprintf(lineBuffer,"Instrument(type/no):%s %d%c%c", userBuffer,iInstNo,13,10); 
		fputs(lineBuffer,m_pFile);

		sprintf(lineBuffer,"Line direction vector:%c%c",13,10);
		fputs(lineBuffer,m_pFile);
		sprintf(lineBuffer,"X:%.9f Y:%.9f Z:%.9f%c%c", m_pLineFitModel->line_ocs[0].ux,m_pLineFitModel->line_ocs[0].uy,m_pLineFitModel->line_ocs[0].uz,13,10);
		fputs(lineBuffer,m_pFile);
		
		sprintf(lineBuffer,"Line points:%c%c",13,10);
		fputs(lineBuffer,m_pFile);	
		
		for(int i=0; i < MAX_POINTS_IN_LINE; i++)
		{
			if(m_pLineFitModel->line_buff[0].points[i].sta != 0)
			{
				sprintf(lineBuffer,"%-2d." DCP_POINT_ID_FMT " X:%9.*f  Y:%9.*f  Z:%9.*f%c%c", i+1, m_pLineFitModel->line_ocs[0].points[i].point_id, 
							m_pModel->m_nDecimals, m_pLineFitModel->line_ocs[0].points[i].x,
							m_pModel->m_nDecimals, m_pLineFitModel->line_ocs[0].points[i].y,
							m_pModel->m_nDecimals, m_pLineFitModel->line_ocs[0].points[i].z,

							13,10);
				fputs(lineBuffer,m_pFile);
	
			}
		}

		// used height

		StringC sHeight = L"";
		sprintf(valueBuffer, "%s", "-");

		if(m_pLineFitModel->selectedHeight == FIRST_POINT)
		{
			sHeight = StringC(AT_DCP06,V_DCP_FIRST_POINT_TOK);
		}
		else if(m_pLineFitModel->selectedHeight == LAST_POINT)
		{
			sHeight = StringC(AT_DCP06,V_DCP_LAST_POINT_TOK);
		}
		else if(m_pLineFitModel->selectedHeight == INTERPOLATION)
		{
			sHeight = StringC(AT_DCP06,V_DCP_INTERPOLATION_TOK);
		}
		else
		{
			sHeight = StringC(AT_DCP06,V_DCP_MANUALLY_ENTERED_TOK);
			sprintf(valueBuffer,"%*.f", m_pModel->m_nDecimals, m_pLineFitModel->manualHeight);
		}
		common->convert_to_ascii(sHeight, userBuffer,sHeight.Length()); 
		
		if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
			sprintf(lineBuffer,"Used height: %s  Height: %s%c%c", userBuffer, valueBuffer, 13, 10);
		else
			sprintf(lineBuffer,"Used depth: %s  Height: %s%c%c", userBuffer, valueBuffer, 13, 10);
		fputs(lineBuffer,m_pFile);

		// shift line
		StringC sShift = L"";
		sprintf(valueBuffer,"%s","-");

		if(m_pLineFitModel->selectedShift != SHIFT_NO)
		{
			sprintf(valueBuffer,"%.*f",m_pModel->m_nDecimals, m_pLineFitModel->shiftValue);
		}

		if(m_pLineFitModel->selectedShift == SHIFT_NO)
		{
			sShift = L"-";
		}
		else if(m_pLineFitModel->selectedShift == SHIFT_RIGHT)
		{
			sShift = StringC(AT_DCP06,V_DCP_SHIFT_RIGHT_TOK);
		}
		else if(m_pLineFitModel->selectedShift == SHIFT_LEFT)
		{
			sShift = StringC(AT_DCP06,V_DCP_SHIFT_LEFT_TOK);
		}
		else if(m_pLineFitModel->selectedShift == SHIFT_UP)
		{
			if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
				sShift = StringC(AT_DCP06,V_DCP_SHIFT_UP_TOK);
			else
				sShift = StringC(AT_DCP06,V_DCP_SHIFT_FORWARD_TOK);
		}
		else if(m_pLineFitModel->selectedShift == SHIFT_DOWN)
		{
			if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
				sShift = StringC(AT_DCP06,V_DCP_SHIFT_DOWN_TOK);
			else
				sShift = StringC(AT_DCP06,V_DCP_SHIFT_BACKWARD_TOK);
		}
		common->convert_to_ascii(sShift, userBuffer, sShift.Length()); 
		sprintf(lineBuffer,"Shift line: %s Value: %s%c%c", userBuffer, valueBuffer, 13, 10);
		fputs(lineBuffer,m_pFile);

		//Rotate line
		StringC sRotate = L"";
		
		if(m_pLineFitModel->selectedRotate == ROTATE_NO)
		{
			sRotate = L"-";
			sprintf(valueBuffer,"%s", "-");
		}
		else if(m_pLineFitModel->selectedRotate == ROTATE_HORIZONTAL)
		{

			if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
			{
				sRotate = StringC(AT_DCP06,V_DCP_ROTATE_LINE_HOR_TOK);
				sprintf(valueBuffer,"%.6f", m_pLineFitModel->rotateAngle);
			}
			else
			{
				sRotate = StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_LEFT_RIGHT_TOK);
				sprintf(valueBuffer,"%.6f", m_pLineFitModel->rotateAngle);
			}
		}
		else if(m_pLineFitModel->selectedRotate == ROTATE_VERTICAL)
		{
			if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
			{
				sRotate = StringC(AT_DCP06,V_DCP_ROTATE_LINE_VER_TOK);
				sprintf(valueBuffer,"%.6f", m_pLineFitModel->rotateAngle);
			}
			else
			{
				sRotate = StringC(AT_DCP06,V_DCP_ROTATE_VERTICAL_DEPTH_TOK);
				sprintf(valueBuffer,"%.6f", m_pLineFitModel->rotateAngle);
			}
		}
		common->convert_to_ascii(sRotate, userBuffer, sRotate.Length()); 
		sprintf(lineBuffer,"Rotate line: %s  Angle: %s %c%c", userBuffer, valueBuffer, 13,10 );

		fputs(lineBuffer,m_pFile);
		
		sprintf(lineBuffer,"Results:%c%c", 13,10 );
		fputs(lineBuffer,m_pFile);
		
		// results
		for(int i=0; i < MAX_LINEFIT_POINTS; i++)
		{
			if(m_pLineFitModel->points_buff[i].sta != 0)
			{
				if(m_pLineFitModel->selectedRefLine == REF_LINE_HORIZONTAL)
				{
					sprintf(lineBuffer,"%-2d." DCP_POINT_ID_FMT " X:%9.*f  Y:%9.*f  Z:%9.*f Calculated point in line: X:%9.*f  Y:%9.*f  Z:%9.*f Height:%9.*f  Dist along the line:%9.*f  Line offset:%9.*f 3D Dist. from the line:%9.*f%c%c", 
								i+1, m_pLineFitModel->points_buff[i].point_id, 
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].xdes,
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].ydes,
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].zdes,
		
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].x,
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].y,
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].z,
		
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].height_diff,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].distance_along_line,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].line_offset,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].points_distance,

								13,10);
				}
				else
				{
						sprintf(lineBuffer,"%-2d." DCP_POINT_ID_FMT " X:%9.*f  Y:%9.*f  Z:%9.*f Calculated point in line: X:%9.*f  Y:%9.*f  Z:%9.*f Depth:%9.*f  Dist along the line:%9.*f  Line offset:%9.*f 3D Dist. from the line:%9.*f%c%c", 
								i+1, m_pLineFitModel->points_buff[i].point_id, 
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].xdes,
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].ydes,
								m_pModel->m_nDecimals, m_pLineFitModel->points_buff[i].zdes,
		
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].x,
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].y,
								m_pModel->m_nDecimals, m_pLineFitModel->points_in_line[i].z,
		
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].height_diff,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].distance_along_line,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].line_offset,
								m_pModel->m_nDecimals, m_pLineFitModel->linefit_results[i].points_distance,

								13,10);
				}
				fputs(lineBuffer,m_pFile);
	
			}
		}



		fclose(m_pFile);
		m_pFile = 0;
		GUI::DesktopC::Instance()->MessageShow(L"Saved");
	}

	return 0;
}


// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::LineFitModel::LineFitModel()
{
	align321Model = new Alignment321Model;
	
	memset(&line_buff[0],0, sizeof(S_LINE_BUFF));
	memset(&line_ocs[0],0, sizeof(S_LINE_BUFF));
	memset(&linefit_results[0], 0, sizeof(S_LINE_FITTING_RESULTS) * MAX_LINEFIT_POINTS);
	memset(&points_buff[0],0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);

	memset(&points_in_line[0], 0, sizeof(S_POINT_BUFF) * MAX_LINEFIT_POINTS);

	manualHeight = 0.0;
	shiftValue = 0.0;
	rotateAngle = 0.0;
	selectedHeight = FIRST_POINT;
	selectedShift = SHIFT_NO;
	selectedRotate = ROTATE_NO;
	align321Model->calculated = false;
	fileName = L"";
	selectedRefLine = REF_LINE_HORIZONTAL;
	/*
	memset(&calc_plane[0],0, sizeof(S_PLANE_BUFF));
	memset(&shaft_circle_points[0],0, sizeof(S_CIRCLE_BUFF));
	memset(&shaft_circle_points_in_plane[0],0, sizeof(S_CIRCLE_BUFF));

	
	active_line = X_LINE;

	shaft_circle_cx = 0.0;
	shaft_circle_cy = 0.0;
	shaft_circle_cz = 0.0;
	shaft_circle_diameter = 0.0;
	shaft_circle_rms = 0.0;

	centerOfCircleDist = 0.0;
	angleLines = 0.0;*/
}

DCP::LineFitModel::~LineFitModel()
{
	if(align321Model != 0)
	{
		delete align321Model;
		align321Model = 0;
	}
}


//short DCP::LineFitModel::calc_shaft()
//{
//	MsgBox msgbox;
//	StringC sMsg;
//	short ret = 0;
//
//	//// lasketaan tason yht�l� joka kulkee ympyr�n keskipisteen kautta ja on kohtisuorassa
//	//// x1,x2 kulkevaa suoraa vastaan
//
//	//if(shaft_circle_points[0].calc != 0 && shaft_circle_points[0].sta > 0)
//	//{
//	//	// tason piste on ympyr�n keskipiste
//	//	calc_plane[0].px = shaft_circle_cx;
//	//	calc_plane[0].py = shaft_circle_cy;
//	//	calc_plane[0].pz = shaft_circle_cz;
//	//	
//	//	if(active_line == X_LINE)
//	//	{
//	//		// normaali
//	//		calc_plane[0].nx = 1.0;
//	//		calc_plane[0].ny = 0.0;
//	//		calc_plane[0].nz = 0.0;
//	//		calc_plane[0].calc = 1;
//	//		calc_plane[0].sta = 1;
//	//	}
//	//	else if(active_line == Y_LINE)
//	//	{
//	//		// normaali
//	//		calc_plane[0].nx = 0.0;
//	//		calc_plane[0].ny = 1.0;
//	//		calc_plane[0].nz = 0.0;
//	//		calc_plane[0].calc = 1;
//	//		calc_plane[0].sta = 1;
//
//	//	}
//	//	else if(active_line == Z_LINE)
//	//	{
//	//		// normaali
//	//		calc_plane[0].nx = 0.0;
//	//		calc_plane[0].ny = 0.0;
//	//		calc_plane[0].nz = 1.0;
//	//		calc_plane[0].calc = 1;
//	//		calc_plane[0].sta = 1;
//	//	}
//	//	else if(active_line == MEAS_LINE)
//	//	{
//	//		// normaali
//	//		calc_plane[0].nx = line_buff[0].ux;//line_buff[0].points[1].x - line_buff[0].points[0].x;
//	//		calc_plane[0].ny = line_buff[0].uy;//line_buff[0].points[1].y - line_buff[0].points[0].y;
//	//		calc_plane[0].nz = line_buff[0].uz;//line_buff[0].points[1].z - line_buff[0].points[0].z;
//	//		calc_plane[0].calc = 1;
//	//		calc_plane[0].sta = 1;
//	//	}
//	//	else
//	//	{
//	//		sMsg.LoadTxt(AT_DCP06, M_DCP_DEFINE_REF_LINE_TOK);
//	//		msgbox.ShowMessageOk(sMsg);
//	//		return 0;
//	//	}
//	//}
//	//else
//	//{
//	//	sMsg.LoadTxt(AT_DCP06, M_DCP_DEFINE_SHAFT_CIRCLE_TOK);
//	//	msgbox.ShowMessageOk(sMsg);
//	//	return 0;
//	//}
//
//	//// calc distance and angle
//	//struct ams_vector m;
//	//struct line wline;
//
//	//		
//	//		m.x = shaft_circle_points->cx;
//	//		m.y = shaft_circle_points->cy;
//	//		m.z = shaft_circle_points->cz;
//
//	//		wline.px = line_buff[0].px;
//	//		wline.py = line_buff[0].py;
//	//		wline.pz = line_buff[0].pz;
//
//	//		wline.ux = line_buff[0].ux;
//	//		wline.uy = line_buff[0].uy;
//	//		wline.uz = line_buff[0].uz;
//
//	//		/* Calculate distance */
//	//		centerOfCircleDist = calc_point_dist_from_line(&m, &wline);
//
//	//		// calc plane by circle_points
//	//		S_PLANE_BUFF calc_circle_plane[1];
//	//		memset(&calc_circle_plane[0],0, sizeof(S_PLANE_BUFF));
//	//		for(int i = 0; i < MAX_POINTS_IN_CIRCLE; i++)
//	//		{
//	//			calc_circle_plane[0].points[i].x = shaft_circle_points[0].points[i].x;
//	//			calc_circle_plane[0].points[i].y = shaft_circle_points[0].points[i].y;
//	//			calc_circle_plane[0].points[i].z = shaft_circle_points[0].points[i].z;
//	//			calc_circle_plane[0].points[i].sta = shaft_circle_points[0].points[i].sta;
//	//		}
//	//		CalcPlane laske_taso;
//
//	//		laske_taso.calc(&calc_circle_plane[0], ACTUAL);
//
//	//		// angle
//	//		struct ams_vector line1,line2;
//	//		line1.x = line_buff[0].ux;
//	//		line1.y = line_buff[0].uy;
//	//		line1.z = line_buff[0].uz;
//
//	//		line2.x = calc_circle_plane[0].nx;
//	//		line2.y = calc_circle_plane[0].ny;
//	//		line2.z = calc_circle_plane[0].nz;
//
//	//		angleLines = angle(&line1,&line2);
//	//		angleLines = radtodeg(angleLines);
//	//			
//	ret = 1;
//	return ret;
//
//}