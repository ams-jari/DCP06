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
#include <dcp06/measurement/DCP_3DFileView.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/file/DCP_File.hpp>
#include <dcp06/core/DCP_SelectPoint.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_InputText.hpp>
#include <dcp06/core/DCP_SelectPoint.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP05InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================



// ================================================================================================
// ====================================  DCP053DFileViewDlgC         ==============================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP053DFileViewDlgC::DCP053DFileViewDlgC(DCP::DCP05FileModelC* pDataModel):GUI::ModelHandlerC(),GUI::StandardDialogC()
			,m_pFile(0),m_pPointId(0), m_pXDsg(0), m_pYDsg(0), m_pZDsg(0),
			m_pXActDev(0),m_pYActDev(0),m_pZActDev(0),m_pDataModel(pDataModel)
{
	//SetTxtApplicationId(AT_DCP05);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP053DFileViewDlgC::~DCP053DFileViewDlgC()
{
	if(pCommon)
	{
		delete pCommon;
		pCommon =  0;
	}
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::DCP053DFileViewDlgC::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
	
	// Add fields to dialog

	m_pFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pFile->SetId(eFile);
	m_pFile->SetText(StringC(AT_DCP05,P_DCP_3DFILE_SK_TOK));
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pFile);
	
	m_pPointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPointId->SetId(ePointId);
	m_pPointId->SetText(StringC(AT_DCP05,P_DCP_POINT_ID_SK_TOK));
	m_pPointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
	m_pPointId->SetEmptyAllowed(true);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pPointId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pPointId);

	m_pXDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pXDsg->SetId(eXDsg);
	m_pXDsg->SetText(StringC(AT_DCP05,P_DCP_X_DSG_TOK));
	m_pXDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetDCP05Model()->m_nDecimals);
	m_pXDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pXDsg->GetFlexFloatInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	// TODO
	//m_pXDsg->SetAutoColon(false);
	//m_pXDsg->SetColonPosition(9 * 11);
	m_pXDsg->SetEmptyAllowed(true);
	AddCtrl(m_pXDsg);

	m_pYDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pYDsg->SetId(eYDsg);
	m_pYDsg->SetText(StringC(AT_DCP05,P_DCP_Y_DSG_TOK));
	m_pYDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetDCP05Model()->m_nDecimals);
	m_pYDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pYDsg->GetStringInputCtrl()->SetCharsCountMax(DCP_XYZ_VALUE_LENGTH);
	m_pYDsg->SetEmptyAllowed(true);
	//m_pYDsg->SetAutoColon(false);
	//m_pYDsg->SetColonPosition(9 * 11);
	AddCtrl(m_pYDsg);

	m_pZDsg = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_Float);
	m_pZDsg->SetId(eZDsg);
	m_pZDsg->SetText(StringC(AT_DCP05,P_DCP_Z_DSG_TOK));

	m_pZDsg->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZDsg->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZDsg->GetFloatInputCtrl()->SetDecimalPlaces((unsigned short)GetDCP05Model()->m_nDecimals);
	m_pZDsg->SetEmptyAllowed(true);
	
	//m_pZDsg->SetAutoColon(false);
	//m_pZDsg->SetColonPosition(9 * 11);
	AddCtrl(m_pZDsg);

	//InsertEmptyLine(); CAPTIVATE
	//SetColonPosLong( GUI::StandardDialogC::CP_10 );
	m_pXActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pXActDev->SetId(eXActDev);
	m_pXActDev->SetText(StringC(AT_DCP05,P_DCP_X_ACT_DEV_TOK));
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pXActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pXActDev->SetEmptyAllowed(true);
	//m_pXActDev->SetAutoColon(false);
	//m_pXActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pXActDev);

	m_pYActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pYActDev->SetId(eYActDev);
	m_pYActDev->SetText(StringC(AT_DCP05,P_DCP_Y_ACT_DEV_TOK));
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pYActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pYActDev->SetEmptyAllowed(true);
	//m_pYActDev->SetAutoColon(false);
	//m_pYActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pYActDev);

	m_pZActDev = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pZActDev->SetId(eZActDev);
	m_pZActDev->SetText(StringC(AT_DCP05,P_DCP_Z_ACT_DEV_TOK));
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pZActDev->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pZActDev->SetEmptyAllowed(true);
	//m_pZActDev->SetAutoColon(false);
	//m_pZActDev->SetColonPosition(9 * 11);
	AddCtrl(m_pZActDev);

	//SetHelpTok(H_DCP_3DFILE_VIEW_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::DCP053DFileViewDlgC::OnDialogActivated()
{
	m_pDataModel->m_pAdfFile->always_single = 1;
	pCommon = new DCP05CommonC(GetDCP05Model());

	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::DCP053DFileViewDlgC::UpdateData()
{
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::DCP053DFileViewDlgC::RefreshControls()
{
	if(m_pFile && m_pPointId && m_pXDsg && m_pYDsg && m_pZDsg && m_pXActDev &&
		m_pYActDev && m_pZActDev) 
	{
		char* xdes_ptr = m_pDataModel->m_pAdfFile->xdes_front;
		char* ydes_ptr = m_pDataModel->m_pAdfFile->ydes_front;
		char* zdes_ptr = m_pDataModel->m_pAdfFile->zdes_front;

		char* xmea_ptr = m_pDataModel->m_pAdfFile->xmea_front;
		char* ymea_ptr = m_pDataModel->m_pAdfFile->ymea_front;
		char* zmea_ptr = m_pDataModel->m_pAdfFile->zmea_front;

		char temp_act[100];
		//char temp_diff[100];
		char x_diff[XYZ_VALUE_BUFF_LEN];
		char y_diff[XYZ_VALUE_BUFF_LEN];
		char z_diff[XYZ_VALUE_BUFF_LEN];

		m_pFile->GetStringInputCtrl()->SetString(m_pDataModel->m_pAdfFile->getFileName());
		
		char temp[XYZ_MEA_AND_DIFF_BUFF_LEN];
		
		char pointid_temp[POINT_ID_BUFF_LEN];
		sprintf(pointid_temp,"%s",m_pDataModel->m_pAdfFile->pointid_front);
		pCommon->strbtrim(pointid_temp);
		m_pPointId->GetStringInputCtrl()->SetString(StringC(pointid_temp));

		if(!pCommon->strblank(xdes_ptr))
		{
			//sprintf(temp,"%.*f",m_pDCP05Model->m_nDecimals,atof(GetDCP05Model()->m_pAdfFile->xdes_front));
			m_pXDsg->GetFloatInputCtrl()->SetDouble(atof(xdes_ptr));
		}
		else
		{
			m_pXDsg->GetFloatInputCtrl()->SetEmpty();
		}
		
		if(!pCommon->strblank(ydes_ptr))
		{
			sprintf(temp,"%.*f",GetDCP05Model()->m_nDecimals,atof(m_pDataModel->m_pAdfFile->ydes_front));
			//m_pYDsg->GetFloatInputCtrl()->SetString(StringC(temp));
			m_pYDsg->GetFloatInputCtrl()->SetDouble(atof(ydes_ptr));
		}
		else
		{
			m_pYDsg->GetFloatInputCtrl()->SetEmpty();
		}
		
		if(!pCommon->strblank(zdes_ptr))
		{
			sprintf(temp,"%.*f",GetDCP05Model()->m_nDecimals,atof(m_pDataModel->m_pAdfFile->zdes_front));
			//m_pZDsg->GetFloatInputCtrl()->SetString(StringC(temp));
			m_pZDsg->GetFloatInputCtrl()->SetDouble(atof(zdes_ptr));
		}
		else
		{
			m_pZDsg->GetFloatInputCtrl()->SetEmpty();
		}

		pCommon->get_dist_(xmea_ptr,xdes_ptr,x_diff);
		pCommon->get_dist_(ymea_ptr,ydes_ptr,y_diff);
		pCommon->get_dist_(zmea_ptr,zdes_ptr,z_diff);

		//if(!pCommon->strblank(GetDCP05Model()->m_pAdfFile->xmea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s", x_diff,xmea_ptr);
			
		//}
		m_pXActDev->GetStringInputCtrl()->SetString(StringC(temp_act));

		//if(!pCommon->strblank(GetDCP05Model()->m_pAdfFile->ymea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s",y_diff, ymea_ptr);
			
		//}
		m_pYActDev->GetStringInputCtrl()->SetString(StringC(temp_act));
		
		//if(!pCommon->strblank(GetDCP05Model()->m_pAdfFile->zmea_front))
		//{
			sprintf(temp_act,"%9.9s %9.9s", z_diff,  zmea_ptr);
			
		//}
		m_pZActDev->GetStringInputCtrl()->SetString(StringC(temp_act));

		
	}
}

// ================================================================================================
// Description: only accept xxx Model objects
// ================================================================================================
bool DCP::DCP053DFileViewDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP05ModelC* pDCP05Model = dynamic_cast< DCP::DCP05ModelC* >( pModel );

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

// ================================================================================================
// Description: GetDCP05Model
// ================================================================================================
DCP::DCP05ModelC* DCP::DCP053DFileViewDlgC::GetDCP05Model() const
{
    return (DCP::DCP05ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  DCP05UserControllerC  ===================================
// ================================================================================================


// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP053DFileViewControllerC::DCP053DFileViewControllerC(DCP::DCP05FileModelC* pDataModel)
    : m_pDlg( NULL ),m_pDataModel(pDataModel)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP05, T_DCP_3DFILE_VIEW_TOK /*C_DCP_APPLICATION_NAME_TOK */));

    // Create a dialog
    m_pDlg = new DCP::DCP053DFileViewDlgC(pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( _3DFILEVIEW_DLG, m_pDlg, true );

    // Set the function key
	FKDef vDef;
	//vDef.nAppId = AT_DCP05;
	vDef.poOwner = this;
		
	vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_ID_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_HOME_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_POINT_END_TOK);
	SetFunctionKey( FK3, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_PREV_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_NEXT_TOK);
	SetFunctionKey( FK5, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );

	vDef.strLable = StringC(AT_DCP05,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK2, vDef );

		// Hide quit
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP05;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP053DFileViewControllerC::~DCP053DFileViewControllerC()
{

}

// ================================================================================================
// Description: OnControllerActivated
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnControllerActivated(void)
{
	
}

// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP053DFileViewControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );
		
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

// ================================================================================================
// Description: F1 / PID
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF1Pressed()
{
		DCP::DCP05SelectPointModelC* pModel = new DCP05SelectPointModelC;

		int iCount = m_pDataModel->m_pAdfFile->GetPointList(&pModel->points[0],MAX_SELECT_POINTS);
		pModel->m_iCounts = iCount;
		pModel->m_iSelectedId = m_pDataModel->m_pAdfFile->active_point_front;

		if(GetController(SELECT_POINT_CONTROLLER) == NULL)
		{
			(void)AddController( SELECT_POINT_CONTROLLER, new DCP::DCP05SelectPointControllerC );
		}

		//(void)GetController(FILE_CONTROLLER)->SetTitleTok(AT_DCP05,T_DCP_DOM_PLANE_MEAS_TOK);

		(void)GetController( SELECT_POINT_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_POINT_CONTROLLER, true);
}


// ================================================================================================
// Description: F2
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF2Pressed()
{
	m_pDataModel->m_pAdfFile->form_pnt(1);
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F3 
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF3Pressed()
{
	m_pDataModel->m_pAdfFile->form_pnt(m_pDataModel->m_pAdfFile->points);
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F4  / PREV
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF4Pressed()
{
	m_pDataModel->m_pAdfFile->form_prev_pnt();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F5 / NEXT
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF5Pressed()
{
	m_pDataModel->m_pAdfFile->form_next_pnt();
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F6 
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnF6Pressed()
{
	Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: SHF2 / DELETE
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnSHF2Pressed()
{
	short pno = m_pDataModel->m_pAdfFile->active_point_front;
	m_pDataModel->m_pAdfFile->delete_point_from_adf(pno);
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{

}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP053DFileViewControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP05SelectPointModelC* pModel = (DCP::DCP05SelectPointModelC*) GetController( SELECT_POINT_CONTROLLER )->GetModel();		
		m_pDataModel->m_pAdfFile->form_pnt(pModel->m_iSelectedId);
	}
	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// Description: ConfirmClose
// ================================================================================================
bool DCP::DCP053DFileViewControllerC::ConfirmClose(bool bEsc)
{
	return true;
}
