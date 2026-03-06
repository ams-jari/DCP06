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
#include <dcp06/init/Initialization.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/SpecialMenu.hpp>
#include <dcp06/core/xyz.hpp>
#include <dcp06/measurement/HiddenPoint.hpp>
#include <dcp06/core/Tool.hpp>
#include <dcp06/core/AddTool.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/SelectTool.hpp>



#include <GUI_Types.hpp>
#include <TPI_MeasConfig.hpp>
#include <UTL_StringFunctions.hpp>
#include <GMAT_UnitConverter.hpp>
// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
OBS_IMPLEMENT_EXECUTE(DCP::DCP06ToolDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06ToolDlgC::DCP06ToolDlgC(DCP06ToolModelC* pToolModel):GUI::ModelHandlerC(),GUI::StandardDialogC(),
					m_pToolId(0), m_pConstant(0),m_pTranslX(0), m_pTranslY(0),
					m_pTranslZ(0), m_pToolX_Total(0),m_pToolY_Total(0), m_pToolZ_Total(0),
					m_pDataModel(pToolModel),m_pCommon(0),
					m_pXObserver(OBS_METHOD_TO_PARAM0(DCP06ToolDlgC, OnValueChanged), this),
					m_pYObserver(OBS_METHOD_TO_PARAM0(DCP06ToolDlgC, OnValueChanged), this),
					m_pZObserver(OBS_METHOD_TO_PARAM0(DCP06ToolDlgC, OnValueChanged), this)


{
	//SetTxtApplicationId(AT_DCP06);
}


            // Description: Destructor
DCP::DCP06ToolDlgC::~DCP06ToolDlgC()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

void DCP::DCP06ToolDlgC::OnInitDialog(void)
{
		GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog

	m_pToolId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolId->SetId(eToolId);
	m_pToolId->SetText(StringC(AT_DCP06,P_DCP_TOOL_ID_SK_TOK));
	m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pToolId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pToolId);
	
	m_pConstant = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pConstant->SetId(eConstant);
	m_pConstant->SetText(StringC(AT_DCP06,P_DCP_CONSTANT_TOK));
	//m_pConstant->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pConstant->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pConstant->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pConstant->GetFloatInputCtrl()->SetDecimalPlaces(1);
	m_pConstant->SetEmptyAllowed(true);
	AddCtrl(m_pConstant);

	m_pTranslX = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pTranslX->SetId(eTranslX);
	m_pTranslX->SetText(StringC(AT_DCP06,P_DCP_TRANSLATION_X_TOK));
	m_pTranslX->GetFloatInputCtrl()->SetDecimalPlaces(GetDCP06Model()->m_nDecimals);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pTranslX->SetEmptyAllowed(true);
	AddCtrl(m_pTranslX);

	m_pTranslY = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pTranslY->SetId(eTranslY);
	m_pTranslY->SetText(StringC(AT_DCP06,P_DCP_TRANSLATION_Y_TOK));
	m_pTranslY->GetFloatInputCtrl()->SetDecimalPlaces(GetDCP06Model()->m_nDecimals);
	m_pTranslY->SetEmptyAllowed(true);
	AddCtrl(m_pTranslY);

	m_pTranslZ = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pTranslZ->SetId(eTranslZ);
	m_pTranslZ->SetText(StringC(AT_DCP06,P_DCP_TRANSLATION_Z_TOK));
	m_pTranslZ->GetFloatInputCtrl()->SetDecimalPlaces(GetDCP06Model()->m_nDecimals);
	m_pTranslZ->SetEmptyAllowed(true);
	AddCtrl(m_pTranslZ);

	//InsertEmptyLine(); CAPTIVATE

	m_pToolX_Total = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolX_Total->SetId(eToolX_Total);
	m_pToolX_Total->SetText(StringC(AT_DCP06,P_DCP_TOOL_TOTAL_X_TOK));
	m_pToolX_Total->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pToolX_Total->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pToolX_Total->SetEmptyAllowed(true);
	AddCtrl(m_pToolX_Total);

	m_pToolY_Total = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolY_Total->SetId(eToolY_Total);
	m_pToolY_Total->SetText(StringC(AT_DCP06,P_DCP_TOOL_TOTAL_Y_TOK));
	m_pToolY_Total->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pToolY_Total->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pToolY_Total->SetEmptyAllowed(true);
	AddCtrl(m_pToolY_Total);

	m_pToolZ_Total = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pToolZ_Total->SetId(eToolZ_Total);
	m_pToolZ_Total->SetText(StringC(AT_DCP06,P_DCP_TOOL_TOTAL_Z_TOK));
	m_pToolZ_Total->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pToolZ_Total->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pToolZ_Total->SetEmptyAllowed(true);
	AddCtrl(m_pToolZ_Total);

	m_pCommon = new DCP06CommonC(GetDCP06Model());

	m_pXObserver.Attach(m_pTranslX->GetSubject());
	m_pYObserver.Attach(m_pTranslY->GetSubject());
	m_pZObserver.Attach(m_pTranslZ->GetSubject());

	//SetHelpTok(H_DCP_TOOL_TOK,0);
}

void DCP::DCP06ToolDlgC::OnDialogActivated()
{
	RefreshControls();
}

void DCP::DCP06ToolDlgC::UpdateData()
{
	// cont pressed update data into DCP05Model....

	memcpy(&GetDCP06Model()->tool_table[0],&m_pDataModel->tool_table[0],sizeof(S_TOOL) * MAX_TOOLS);
	GetDCP06Model()->active_tool = m_pDataModel->iActiveTool;
	GetDCP06Model()->tool_trans_x = m_pDataModel->dTransX;
	GetDCP06Model()->tool_trans_y = m_pDataModel->dTransY;
	GetDCP06Model()->tool_trans_z = m_pDataModel->dTransZ;

	GetDCP06Model()->active_tool_x = GetDCP06Model()->tool_trans_x + m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].x;
	GetDCP06Model()->active_tool_y = GetDCP06Model()->tool_trans_y + m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].y;
	GetDCP06Model()->active_tool_z = GetDCP06Model()->tool_trans_z + m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].z;


}

// Description: refresh all controls
void DCP::DCP06ToolDlgC::RefreshControls()
{
	if( m_pToolId && m_pConstant  && m_pTranslX && m_pTranslY &&  m_pTranslZ &&  m_pToolX_Total)
	{
		short iActiveTool = m_pDataModel->iActiveTool;

		if(iActiveTool > 0 && !m_pCommon->strblank(m_pDataModel->tool_table[iActiveTool-1].tool_id))
		{
			StringC sTempName(m_pDataModel->tool_table[iActiveTool-1].tool_id);
			sTempName.RTrim();

			//m_pToolId->GetStringInputCtrl()->SetString(StringC(m_pDataModel->tool_table[iActiveTool-1].tool_id));
			m_pToolId->GetStringInputCtrl()->SetString(sTempName);

			StringC sTemp;
			//sTemp.Format(L"%+3.1f",m_pDataModel->tool_table[iActiveTool-1].constant);
			//m_pConstant->GetStringInputCtrl()->SetString(sTemp);
			m_pConstant->GetFloatInputCtrl()->SetDouble(m_pDataModel->tool_table[iActiveTool-1].constant);
			
			m_pTranslX->GetFloatInputCtrl()->SetDouble(m_pDataModel->dTransX);
			m_pTranslY->GetFloatInputCtrl()->SetDouble(m_pDataModel->dTransY);
			m_pTranslZ->GetFloatInputCtrl()->SetDouble(m_pDataModel->dTransZ);
			
			
			sTemp.Format(L"%7.*f %8.*f",GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].x,
									   GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].x+m_pDataModel->dTransX);  	
			m_pToolX_Total->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%7.*f %8.*f",GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].y,
									   GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].y + m_pDataModel->dTransY);  	
			m_pToolY_Total->GetStringInputCtrl()->SetString(sTemp);

			
			sTemp.Format(L"%7.*f %8.*f",GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].z,
									   GetDCP06Model()->m_nDecimals,m_pDataModel->tool_table[iActiveTool-1].z + m_pDataModel->dTransZ);  	
			
			m_pToolZ_Total->GetStringInputCtrl()->SetString(sTemp);
		}
		else
		{
			m_pToolId->GetStringInputCtrl()->SetEmpty();
			m_pConstant->GetFloatInputCtrl()->SetEmpty();
			m_pTranslX->GetFloatInputCtrl()->SetEmpty();
			m_pTranslY->GetFloatInputCtrl()->SetEmpty();
			m_pTranslZ->GetFloatInputCtrl()->SetEmpty();
			m_pToolX_Total->GetStringInputCtrl()->SetEmpty();
			m_pToolY_Total->GetStringInputCtrl()->SetEmpty();
			m_pToolZ_Total->GetStringInputCtrl()->SetEmpty();
		}
	}
}

// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
void DCP::DCP06ToolDlgC::OnValueChanged(int unNotifyCode,  int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetDCP06Model()->iCurrentPoint;
			if (ulParam2 == eTranslX)
			{
				if(!m_pTranslX->GetFloatInputCtrl()->IsEmpty())
				m_pDataModel->dTransX = m_pTranslX->GetFloatInputCtrl()->GetDouble();
				else
					m_pDataModel->dTransX = 0.0; 
				RefreshControls();
			}
			else if (ulParam2 == eTranslY)
			{
				if(!m_pTranslY->GetFloatInputCtrl()->IsEmpty())
					m_pDataModel->dTransY = m_pTranslY->GetFloatInputCtrl()->GetDouble();
				else
					m_pDataModel->dTransY = 0.0; 
				RefreshControls();

			}
			else if(ulParam2 == eTranslZ)
			{
				if(!m_pTranslZ->GetFloatInputCtrl()->IsEmpty())
					m_pDataModel->dTransZ = m_pTranslZ->GetFloatInputCtrl()->GetDouble();
				else
					m_pDataModel->dTransZ = 0.0; 
				RefreshControls();
			}
		}
}

// Description: only accept hello world Model objects
bool DCP::DCP06ToolDlgC::SetModel( GUI::ModelC* pModel )
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
DCP::DCP06ModelC* DCP::DCP06ToolDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}
// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06ToolControllerC::DCP06ToolControllerC()
    : m_pDlg( NULL )
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_TOOL_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	m_pDataModel = new DCP06ToolModelC();

    // Create a dialog
    m_pDlg = new DCP::DCP06ToolDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( TOOL_DLG, m_pDlg, true );

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_TOOL_ID_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_ADD_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_NEXT_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_PREV_TOK);
	SetFunctionKey( FK5, vDef );
	
	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06ToolControllerC::~DCP06ToolControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP06ToolControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );
	
	// save current tool
	m_pDataModel->iOld_active_tool = m_pDCP06Model->active_tool;
	if(m_pDataModel->iOld_active_tool == 0)
	{
		TPI::MeasConfigC oMeasConfig;
		oMeasConfig.Get();
		m_pDataModel->dConstant = oMeasConfig.GetReflectorConstant();
		m_pDataModel->dConstant = m_pDataModel->dConstant * 1000.0;
	}
	else
	{
		m_pDataModel->dOld_x_trans = m_pDCP06Model->tool_trans_x;
		m_pDataModel->dOld_y_trans = m_pDCP06Model->tool_trans_y;
		m_pDataModel->dOld_z_trans = m_pDCP06Model->tool_trans_z;

	}
	memcpy(&m_pDataModel->tool_table[0], &m_pDCP06Model->tool_table[0], sizeof(S_TOOL) * MAX_TOOLS);
	m_pDataModel->dTransX = m_pDCP06Model->tool_trans_x;
	m_pDataModel->dTransY = m_pDCP06Model->tool_trans_y;
	m_pDataModel->dTransZ = m_pDCP06Model->tool_trans_z;
	m_pDataModel->iActiveTool = m_pDCP06Model->active_tool;

	
	m_pCommon = new DCP06CommonC(m_pDCP06Model);

    // Set it to hello world dialog
     return m_pDlg->SetModel( pModel );
	
  // Verify type
   // DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    
	//if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    //(
    //    RefreshControls();
    //    return true;
    //}
    //USER_APP_VERIFY( false );
    //return false;
	
}

// CONT
void DCP::DCP06ToolControllerC::OnF1Pressed()
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

// TOOL ID
void DCP::DCP06ToolControllerC::OnF2Pressed()
{
   if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
		
    short iCount=0;
	for(short i =1; i <= MAX_TOOLS; i++)
	{
		if(!m_pCommon->strblank(m_pDataModel->tool_table[i-1].tool_id))
		{
			iCount++;
		}
	}

	DCP::DCP06SelectToolModelC* pModel = new DCP06SelectToolModelC;
	pModel->m_iCounts = iCount;
	pModel->tool_table = &m_pDataModel->tool_table[0];
	pModel->m_iSelectedId = m_pDataModel->iActiveTool+1;
		
	if(GetController(SELECT_TOOL_CONTROLLER) == NULL)
	{
		(void)AddController( SELECT_TOOL_CONTROLLER, new DCP::DCP06SelectToolControllerC );
	}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP06,T_DCP_DOM_PLANE_MEAS_TOK);

	(void)GetController( SELECT_TOOL_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_TOOL_CONTROLLER, true);
}

// ADD
void DCP::DCP06ToolControllerC::OnF3Pressed()
{
		// Add tool
		DCP::DCP06AddToolModelC* pModel = new DCP::DCP06AddToolModelC;		
		if(GetController(ADD_TOOL_CONTROLLER) == NULL)
		{
			(void)AddController( ADD_TOOL_CONTROLLER, new DCP::DCP06AddToolControllerC(m_pDlg->GetDCP06Model()));
		}
		(void)GetController( ADD_TOOL_CONTROLLER )->SetModel( pModel);
		SetActiveController(ADD_TOOL_CONTROLLER, true);

}

// NEXT
void DCP::DCP06ToolControllerC::OnF4Pressed()
{
	if(m_pDataModel->iActiveTool < MAX_TOOLS)
		m_pDataModel->iActiveTool++;
	
	m_pDlg->RefreshControls();

}

// PREV
void DCP::DCP06ToolControllerC::OnF5Pressed()
{
	if(m_pDataModel->iActiveTool > 0)
		m_pDataModel->iActiveTool--;
	
	m_pDlg->RefreshControls();

}


// DEL
void DCP::DCP06ToolControllerC::OnSHF2Pressed()
{
	if(m_pDataModel->iActiveTool > 0)
	{
		DCP06MsgBoxC msgBox;
		StringC sMsg;
		sMsg.LoadTxt(AT_DCP06, M_DCP_DELETE_TOOL_TOK);
		if(msgBox.ShowMessageYesNo(sMsg))
		{
			for(short j=m_pDataModel->iActiveTool; j < MAX_TOOLS;j++)
			{	
				memcpy(&m_pDataModel->tool_table[j-1],&m_pDataModel->tool_table[j],sizeof(S_TOOL));
				/*
				strcpy(m_pDataModel->tool_table[j-1].tool_id, m_pDataModel->tool_table[j].tool_id);
				m_pDataModel->tool_table[j-1].x 			=  m_pDataModel->tool_table[j].x;
				m_pDataModel->tool_table[j-1].y 			=  m_pDataModel->tool_table[j].y;
				m_pDataModel->tool_table[j-1].z 			=  m_pDataModel->tool_table[j].z;
				m_pDataModel->tool_table[j-1].prism 		=  m_pDataModel->tool_table[j].prism;
				m_pDataModel->tool_table[j-1].constant 	=  m_pDataModel->tool_table[j].constant;
				*/
			}
			memset(&m_pDataModel->tool_table[MAX_TOOLS-1],0,sizeof(S_TOOL));
			/*
			m_pDataModel->tool_table[MAX_TOOLS-1].x 			=  0.0;
			m_pDataModel->tool_table[MAX_TOOLS-1].y 			=  0.0;
			m_pDataModel->tool_table[MAX_TOOLS-1].z 			=  0.0;
			m_pDataModel->tool_table[MAX_TOOLS-1].prism 		=  1;
			m_pDataModel->tool_table[MAX_TOOLS-1].constant 	=  0;
			sprintf(m_pDataModel->tool_table[MAX_TOOLS-1].tool_id,"%-6.6s\0"," ");	
			*/
		}
		m_pDlg->RefreshControls();
	}
}


// Description: React on close of tabbed dialog
void DCP::DCP06ToolControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// Description: React on close of controller
void DCP::DCP06ToolControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == ADD_TOOL_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06AddToolModelC* pModel = (DCP::DCP06AddToolModelC*) GetController( ADD_TOOL_CONTROLLER )->GetModel();		
		m_pDataModel->iActiveTool = 0;
		
		for(short i =1; i <= MAX_TOOLS; i++)
		{
			if(m_pCommon->strblank(m_pDataModel->tool_table[i-1].tool_id))
			{
				m_pDataModel->iActiveTool = i;
				break;
			}
		}
		memcpy(&m_pDataModel->tool_table[m_pDataModel->iActiveTool-1],&pModel->tool,sizeof(S_TOOL));
		/*
		sprintf(m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].tool_id,"%5.5s",pModel->sName);
		m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].prism_or_tape = pModel->iPrismOrTape;
		m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].constant = pModel->dConstant;
		m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].x = pModel->dX;
		m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].y = pModel->dY;
		m_pDataModel->tool_table[m_pDataModel->iActiveTool-1].z = pModel->dZ;
		*/
	}

	if(lCtrlID == SELECT_TOOL_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectToolModelC* pModel = (DCP::DCP06SelectToolModelC*) GetController( SELECT_TOOL_CONTROLLER )->GetModel();		
		m_pDataModel->iActiveTool = pModel->m_iSelectedId-1;
	}
	
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}


// ================================================================================================
// ====================================  DCP06ToolModelC =======================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06ToolModelC::DCP06ToolModelC()
{
	memset(&tool_table[0], 0, sizeof(S_TOOL) * MAX_TOOLS);
	iActiveTool = 0;
	dTransX = 0.0;
	dTransY = 0.0;
	dTransZ = 0.0;
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06ToolModelC::~DCP06ToolModelC()
{
}