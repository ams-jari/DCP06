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
#include <dcp06/core/DCP_Aim.hpp>
#include <dcp06/init/DCP_DCP06Init.hpp>
#include <dcp06/core/DCP_DCP06Meas.hpp>
#include <dcp06/core/DCP_SpecialMenu.hpp>
#include <dcp06/core/DCP_xyz.hpp>
#include <dcp06/measurement/DCP_HiddenPoint.hpp>
#include <dcp06/measurement/DCP_HomePoints.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_InputText.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06InitDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================


// USER DIALOG

DCP::DCP06HomePointsDlgC::DCP06HomePointsDlgC(DCP06HomePointsModelC* pHomePointsModel):GUI::ModelHandlerC(),GUI::TableDialogC(),
			poMultiColCtrl(NULL),m_pDataModel(pHomePointsModel)
{
	//SetTxtApplicationId(AT_DCP06);

	// load text
	sPointNo.LoadTxt(AT_DCP06,P_DCP_POINT_NO_TOK);
	sPointId.LoadTxt(AT_DCP06,P_DCP_POINT_ID_TOK);
	sPointStatus.LoadTxt(AT_DCP06,P_DCP_POINT_STATUS_TOK);

	sDCS.LoadTxt(AT_DCP06,V_DCP_SCS_TOK);
	sOCS.LoadTxt(AT_DCP06,V_DCP_OCS_TOK);

}


            // Description: Destructor
DCP::DCP06HomePointsDlgC::~DCP06HomePointsDlgC()
{

}

void DCP::DCP06HomePointsDlgC::OnInitDialog(void)
{
	GUI::TableDialogC::OnInitDialog();

	/*
	GUI::ListConfigColsC oConfigCols1;
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_PointNo));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_PointId));
	USER_APP_VERIFY(oConfigCols1.AddCol(AlignmentT::AL_LEFT, CI_PointStatus));
	*/

	// Table
	poMultiColCtrl = new GUI::ListMultiColCtrlC();
	poMultiColCtrl->DisableSearch();
	//poMultiColCtrl->AddColConfig(oConfigCols1, true);
	poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

	// text
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_NO_TOK),CI_PointNo));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_ID_TOK),CI_PointId));
	USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06,P_DCP_POINT_STATUS_TOK),CI_PointStatus));

	poMultiColCtrl->AddColSelection(CI_PointNo, CI_PointId, CI_PointStatus);

	// set column config acktive
	USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
	AddTable(poMultiColCtrl);

	// set help
	//SetHelpTok(H_DCP_HOME_POINTS_TOK,0);

}

void DCP::DCP06HomePointsDlgC::OnDialogActivated()
{
	//BeginDraw();
	StringC sPointNo;
	/*
	for(short i=0; i < MAX_HOME_POINTS; i++)
	{
		sprintf(temp,"%d",i+1);
		sPointNo = temp;

		USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
		USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_PointNo, (short) i, sPointNo));

		USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_PointId,(short) i,L"")); 
		USER_APP_VERIFY(poMultiColCtrl->SetCellStr(CI_PointStatus,(short) i,L"-")); 


	}
	*/
	//EndDraw();
	RefreshControls();
}

void DCP::DCP06HomePointsDlgC::UpdateData()
{
	 // save data into DCP05Model
	 memcpy(&GetDCP06Model()->home_points[0],&m_pDataModel->home_points[0],sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);

}

// Description: refresh all controls
void DCP::DCP06HomePointsDlgC::RefreshControls()
{
	if(poMultiColCtrl)
	{
		//BeginDraw();	
		
		short iSel = poMultiColCtrl->GetSelectedId();
		if(iSel < 0) iSel = 0;

		poMultiColCtrl->DestroyAll();
	
		StringC sPointNo;
		StringC sPointId;
		StringC sStatus;

		//char temp[20];
		
		for(short i=0; i < MAX_HOME_POINTS; i++)
		{
			sPointNo.Format(L"%d",i+1);
			sPointId = StringC(m_pDataModel->home_points[i].point_id);
			//sPointId.Format(L"%s", (const wchar_t*) StringC(m_pDataModel->home_points[i].point_id));	

			if(m_pDataModel->home_points[i].sta > 0 && m_pDataModel->home_points[i].cds == DCS)
				sStatus.Format(L"%s(%s)", (const wchar_t*) L"+", (const wchar_t*) sDCS);

			else if(m_pDataModel->home_points[i].sta > 0 && m_pDataModel->home_points[i].cds > DCS)
				sStatus.Format(L"%s(%s)", (const wchar_t*) L"+", (const wchar_t*) sOCS);
			else
				sStatus = L"-";

			USER_APP_VERIFY(poMultiColCtrl->AddRow((short) i));
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_PointNo, (short) i, sPointNo));

			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_PointId,(short) i,sPointId)); 
			USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_PointStatus,(short) i,sStatus)); 
	}
	//EndDraw();
	
	poMultiColCtrl->SetSelectedId(iSel);

	}
}
// Description: only accept hello world Model objects
bool DCP::DCP06HomePointsDlgC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
       // RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP06ModelC* DCP::DCP06HomePointsDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}


short DCP::DCP06HomePointsDlgC::get_selected_id()
{
	return poMultiColCtrl->GetSelectedId();
}
// ================================================================================================
// ====================================  DCP06UserControllerC  ===================================
// ================================================================================================

//-------------------------------------------------------------------------------------------------
// DCP06UserControllerC
// 
DCP::DCP06HomePointsControllerC::DCP06HomePointsControllerC(DCP06ModelC* pDCP06Model)
    : m_pDlg( NULL ),m_pCommon(0),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_HOME_POINTS_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model
	m_pDataModel = new DCP06HomePointsModelC();


    // Create a dialog
    m_pDlg = new DCP::DCP06HomePointsDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( HOME_POINTS_DLG, m_pDlg, true );


	m_pCommon = new DCP06CommonC(m_pDCP06Model);

    // Set the function key
	
    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_AIM_TOK);
	SetFunctionKey( FK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_MEAS_TOK);
	SetFunctionKey( FK4, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_EDIT_TOK);
	SetFunctionKey( FK5, vDef );

	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_DEL_TOK);
	SetFunctionKey( SHFK4, vDef );

	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );


} //lint !e818 Pointer parameter could be declared as pointing to const

DCP::DCP06HomePointsControllerC::~DCP06HomePointsControllerC()
{
	// set old active cds back...
	m_pDCP06Model->active_coodinate_system = m_pDataModel->iOldActiveCds;

	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}

// Description: Route model to everybody else
bool DCP::DCP06HomePointsControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	 m_pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

	 // load data from DCP05Model
	 memcpy(&m_pDataModel->home_points[0],&m_pDCP06Model->home_points[0],sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
	 m_pDataModel->iOldActiveCds = m_pDCP06Model->active_coodinate_system;

    //Set it to hello world dialog
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
void DCP::DCP06HomePointsControllerC::OnF1Pressed()
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

// AIM
void DCP::DCP06HomePointsControllerC::OnF2Pressed()
{
		// selected id
		short iId = m_pDlg->get_selected_id()+1;
		
		if(m_pDataModel->home_points[iId-1].sta)
		{
			double x = m_pDataModel->home_points[iId-1].x;
			double y = m_pDataModel->home_points[iId-1].y;
			double z = m_pDataModel->home_points[iId-1].z;
			short cds = m_pDataModel->home_points[iId-1].cds;
		
			//set_aim(atof(m_pMeasModel->xdes_ptr),atof(m_pMeasModel->ydes_ptr),atof(m_pMeasModel->zdes_ptr), m_pDlg->GetDCP06Model()->active_coodinate_system);		
			if(GetController(AIM_CONTROLLER) == NULL)
			{
				(void)AddController( AIM_CONTROLLER, new DCP::DCP06AimControllerC(x,y,z,cds) );
			}

			(void)GetController( AIM_CONTROLLER )->SetModel(m_pDCP06Model);
			SetActiveController(AIM_CONTROLLER, true);
		}
}


// MEAS
void DCP::DCP06HomePointsControllerC::OnF4Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	short active_cds, selected_cds;

	short active_point = m_pDlg->get_selected_id()+1;

	active_cds = selected_cds = m_pDlg->GetDCP06Model()->active_coodinate_system;
	
	if(active_point <1) active_point = 1;

	if (active_cds == OCSP || active_cds == OCSC || active_cds == OCSD || active_cds == OCSU)
	{
		//selected_cds = select_coordinate_system_line(T_HOME_POINTS_TOK);
		DCP::DCP06SelectCoordinateSystemC* poDlg = new DCP::DCP06SelectCoordinateSystemC();
		if(GetDialog(SELECT_COORDINATE_DLG) == NULL)
		{
			AddDialog(SELECT_COORDINATE_DLG,poDlg); 	
		}
		//poDlg->SetTxtApplicationId(AT_DCP06 );
		SetActiveDialog(SELECT_COORDINATE_DLG, true);
	}
	else
	{
		m_pDCP06Model->active_coodinate_system = DCS;

		DCP::DCP06MeasModelC* pModel = new DCP06MeasModelC;
		pModel->m_iMaxPoint = MAX_HOME_POINTS;
		pModel->m_iMinPoint = MAX_HOME_POINTS;
		pModel->m_iCurrentPoint = m_pDlg->get_selected_id()+1;

		memcpy(&pModel->point_table[0],&m_pDataModel->home_points[0],sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
	
		if(GetController(MEAS_CONTROLLER) == NULL)
		{
			(void)AddController( MEAS_CONTROLLER, new DCP::DCP06MeasControllerC(m_pDlg->GetDCP06Model()));
		}
		(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_HOME_POINTS_MEAS_TOK));

		(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
		SetActiveController(MEAS_CONTROLLER, true);
	}
}

// EDIT POINT ID
void DCP::DCP06HomePointsControllerC::OnF5Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	short active_point = m_pDlg->get_selected_id()+1;

	DCP::DCP06InputTextModelC* pModel = new DCP06InputTextModelC;
	pModel->m_StrInfoText.LoadTxt(AT_DCP06, L_DCP_ENTER_POINT_ID_TOK);
	pModel->m_StrTitle = GetTitle();
	pModel->m_iTextLength = 6;
	pModel->m_StrText = StringC(m_pDataModel->home_points[active_point-1].point_id);

	if(GetController(INPUT_TEXT_CONTROLLER) == NULL)
	{
		(void)AddController( INPUT_TEXT_CONTROLLER, new DCP::DCP06InputTextControllerC( m_pDCP06Model));
	}

	(void)GetController( INPUT_TEXT_CONTROLLER )->SetModel(pModel);
	SetActiveController(INPUT_TEXT_CONTROLLER, true);

}

// DEL
void DCP::DCP06HomePointsControllerC::OnSHF4Pressed()
{
	if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	DCP06MsgBoxC msgbox;
	StringC msg;	
	msg.LoadTxt(AT_DCP06,M_DCP_DELETE_HOME_POINTS_TOK);
	
	if(msgbox.ShowMessageYesNo(msg))
	{
		memset(&m_pDataModel->home_points[0],0, sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
		memset(&m_pDCP06Model->home_points[0],0,sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
		m_pDlg->RefreshControls();
	}
}


// Description: React on close of tabbed dialog
void DCP::DCP06HomePointsControllerC::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
	if(lDlgID == SELECT_COORDINATE_DLG && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DCP06SelectCoordinateSystemC* poDlg = (DCP::DCP06SelectCoordinateSystemC*)GetDialog(lDlgID);
		if(poDlg)
		{
			short selected_id = poDlg->get_selected_id();
			
			if(selected_id == 1)
				m_pDCP06Model->active_coodinate_system = DCS;
			else
				m_pDCP06Model->active_coodinate_system = m_pDataModel->iOldActiveCds;

			DCP::DCP06MeasModelC* pModel = new DCP06MeasModelC;
			pModel->m_iMaxPoint = MAX_HOME_POINTS;
			pModel->m_iMinPoint = MAX_HOME_POINTS;
			pModel->m_iCurrentPoint = m_pDlg->get_selected_id()+1;

			memcpy(&pModel->point_table[0],&m_pDataModel->home_points[0],sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
	
			if(GetController(MEAS_CONTROLLER) == NULL)
			{
				(void)AddController( MEAS_CONTROLLER, new DCP::DCP06MeasControllerC(m_pDlg->GetDCP06Model()));
			}
			(void)GetController(MEAS_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_HOME_POINTS_MEAS_TOK));

			(void)GetController( MEAS_CONTROLLER )->SetModel(pModel);
			SetActiveController(MEAS_CONTROLLER, true);
		}
	}
	DestroyDialog(lDlgID);
}

// Description: React on close of controller
void DCP::DCP06HomePointsControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	// MEAS
	if(lCtrlID == MEAS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06MeasModelC* pModel = (DCP::DCP06MeasModelC*) GetController( MEAS_CONTROLLER )->GetModel();		
		memcpy(&m_pDataModel->home_points[0], &pModel->point_table[0],sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
		m_pDlg->RefreshControls();
	}

	// Edit point id
	if(lCtrlID == INPUT_TEXT_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
			DCP::DCP06InputTextModelC* pModel = (DCP::DCP06InputTextModelC*) GetController( INPUT_TEXT_CONTROLLER )->GetModel();		
			StringC strNewFile = pModel->m_StrText;
			char buffer[10]; buffer[0] = '\0';
			m_pCommon->convert_to_ascii(strNewFile, buffer,DCP_POINT_ID_LENGTH +1 ); // 280508
			m_pCommon->strbtrim(buffer);

			short active_point = m_pDlg->get_selected_id()+1;
			sprintf(m_pDataModel->home_points[active_point-1].point_id,"%s",buffer);
			m_pDlg->RefreshControls();
	}

	// AIM
	if(lCtrlID == AIM_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
				
	}


	
	DestroyController( lCtrlID );
}



// ================================================================================================
// ====================================  DCP063DMeasModelC           ==============================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06HomePointsModelC::DCP06HomePointsModelC()
{
	memset(&home_points[0],0,sizeof(S_POINT_BUFF) * MAX_HOME_POINTS);
}

// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06HomePointsModelC::~DCP06HomePointsModelC()
{

}



// ================================================================================================
// ====================================  DCP063DMeasModelC           ==============================
// ================================================================================================

DCP::DCP06SelectCoordinateSystemC::DCP06SelectCoordinateSystemC():m_pLineInfo1(0),m_pLineInfo2(0),m_pLineInfo3(0),
					m_iSelected(0)
{
	//SetTxtApplicationId( GetTxtApplicationId());
	//SetTxtApplicationId(AT_DCP06);

}


            // Description: Destructor
DCP::DCP06SelectCoordinateSystemC::~DCP06SelectCoordinateSystemC()
{

}

void DCP::DCP06SelectCoordinateSystemC::OnInitDialog(void)
{
	//GUI::BaseDialogC::OnInitDialog();
	GUI::StandardDialogC::OnInitDialog();
	
	ResetFunctionKeys();
	// HIDE SHIFT F2/F6
    FKDef vDef1;
	vDef1.poOwner = this;
	//vDef1.nAppId = AT_DCP06;
	vDef1.strLable =L"";
	SetFunctionKey( FK2, vDef1 );
	SetFunctionKey( FK4, vDef1 );
	SetFunctionKey( SHFK4, vDef1 );

	//HideFunctionKey(FK2);
	//HideFunctionKey(FK4);
	//HideFunctionKey(SHFK4);

    SetTitle(StringC(AT_DCP06,T_DCP_HOME_POINTS_TOK));

	//SetColonPosLong( GUI::StandardDialogC::CP_20 );
	//unsigned short unHeight = DialogC::GetDefaultCtrlHeight();

	//InsertEmptyLine(unHeight);

	m_pLineInfo1 = new GUI::TextCtrlC();
	m_pLineInfo1->SetId(eLineInfo1);
	m_pLineInfo1->SetText(StringC(AT_DCP06,L_DCP_SELECT_COORDINATE_SYSTEM_TOK));
	AddCtrl(m_pLineInfo1);

	//InsertEmptyLine(unHeight);

	m_pLineInfo2 = new GUI::TextCtrlC();
	m_pLineInfo2->SetId(eLineInfo2);
	m_pLineInfo2->SetText(StringC(AT_DCP06,L_DCP_SCS_F1_TOK));
	AddCtrl(m_pLineInfo2);
	
	m_pLineInfo3 = new GUI::TextCtrlC();
	m_pLineInfo3->SetId(eLineInfo3);
	m_pLineInfo3->SetText(StringC(AT_DCP06,L_DCP_OCS_F3_TOK));
	AddCtrl(m_pLineInfo3);

	//TODO VIVA SetHelpTok(H_DCP_SELECT_COORDINATE_SYSTEM_TOK,0);

	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
		
	//vDef.nLable = K_DCP_SCS_TOK;
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_OCS_TOK);
	SetFunctionKey( FK3, vDef );
	
	GUI::DesktopC::Instance()->UpdateFunctionKeys();
	
	//SetHelpTok(H_DCP_SELECT_COORDINATE_SYSTEM_TOK,0);
	//m_pComboBoxObserver.Attach(m_pUnit->GetSubject());
}

void DCP::DCP06SelectCoordinateSystemC::OnDialogActivated()
{
	/*HideFunctionKey(FK2);
	HideFunctionKey(FK4);
	HideFunctionKey(SHFK4);
	*/
	RefreshControls();
}

// Description: refresh all controls
void DCP::DCP06SelectCoordinateSystemC::RefreshControls()
{	
}

void DCP::DCP06SelectCoordinateSystemC::UpdateData()
{
}


// Description: only accept hello world Model objects
bool DCP::DCP06SelectCoordinateSystemC::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDCP06Model != NULL && /*GUI::*/ModelHandlerC::SetModel( pDCP06Model ))
    {
        //RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}

// Description: Hello World model
DCP::DCP06ModelC* DCP::DCP06SelectCoordinateSystemC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

void DCP::DCP06SelectCoordinateSystemC::OnF1Pressed()
{
	m_iSelected = 1;
	Close(EC_KEY_CONT);
}

void DCP::DCP06SelectCoordinateSystemC::OnF3Pressed()
{
	m_iSelected = 2;
	Close(EC_KEY_CONT);
}


short DCP::DCP06SelectCoordinateSystemC::get_selected_id()
{
	return m_iSelected;
}