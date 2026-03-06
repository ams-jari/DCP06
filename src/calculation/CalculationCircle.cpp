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
#include "calc.h"
#include <dcp06/core/Model.hpp>

#include <dcp06/calculation/CalculationCircle.hpp>
#include <dcp06/file/AngleFile.hpp>
#include <dcp06/calculation/CalculationPlane.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>
#include <dcp06/file/SelectFile.hpp>
#include <dcp06/calculation/CalculationView.hpp>
#include <dcp06/calculation/CalculationCircleCore.hpp>

#include <GUI_Types.hpp>
#include <GUI_Desktop.hpp>
#include <UTL_StringFunctions.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
//OBS_IMPLEMENT_EXECUTE(DCP::DCP06CalculationCircleDlgC);

// ================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================



// ================================================================================================
// ====================================  DCP06CalculationCircleDlgC  ======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06CalculationCircleDlgC::DCP06CalculationCircleDlgC(DCP06CalculationCircleModelC * pCircleModel):GUI::ModelHandlerC(),GUI::StandardDialogC()
			,m_p3DFile(0),//m_pCircleFile(0),m_pCircleId(0),
			m_pDiameter(0),m_pRadius(0),m_pRMS(0), m_pCx(0), m_pCy(0), m_pCz(0),m_pDataModel(pCircleModel)
			//m_pCircleIdObserver(OBS_METHOD_TO_PARAM0(DCP06CalculationCircleDlgC, OnValueChanged), this)

{
	//	SetTxtApplicationId(AT_DCP06);
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06CalculationCircleDlgC::~DCP06CalculationCircleDlgC()
{

}
// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::DCP06CalculationCircleDlgC::OnInitDialog(void)
{
	short iColPos =16;
	GUI::BaseDialogC::OnInitDialog();
	m_p3DFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_p3DFile->SetId(e3DFile);
	m_p3DFile->SetText(StringC(AT_DCP06,P_DCP_3DFILE_SK_TOK));
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_p3DFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_p3DFile->SetAutoColon(false);
	//m_p3DFile->SetColonPosition(9 * iColPos);
	//m_p3DFile->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	//m_p3DFile->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_p3DFile);

	// InsertEmptyLine(); CAPTIVATE

	/*
	m_pCircleFile = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCircleFile->SetId(eCircleFile);
	m_pCircleFile->GetTextCtrl()->SetTextTok(P_DCP_CIRCLE_FILE_TOK);
	m_pCircleFile->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCircleFile->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pCircleFile->SetAutoColon(false);
	m_pCircleFile->SetColonPosition(9 * iColPos);
	AddCtrl(m_pCircleFile);
	

	m_pCircleId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCircleId->SetId(eCircleId);
	m_pCircleId->GetTextCtrl()->SetTextTok(P_DCP_CIRCLE_ID_TOK);
	m_pCircleId->GetStringInputCtrl()->SetCharsCountMax(6);
	m_pCircleId->GetStringInputCtrl()->SetEmptyAllowed(true);
	//m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	//m_pRefId->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	m_pCircleId->SetAutoColon(false);
	m_pCircleId->SetColonPosition(9 * iColPos);
	AddCtrl(m_pCircleId);
	*/
	/*
	m_pCircle = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCircle->SetId(eAngle);
	m_pCircle->GetTextCtrl()->SetTextTok(P_DCP_ANGLE_TOK);
	m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCircle->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	AddCtrl(m_pCircle);
	*/
	m_pDiameter = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pDiameter->SetId(eDiameter);
	m_pDiameter->SetText(StringC(AT_DCP06,P_DCP_DIAMETER_TOK));
	//m_pDiameter->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pDiameter->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pDiameter->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pDiameter->SetAutoColon(false);
	//m_pDiameter->SetColonPosition(9 * iColPos);
	//m_pDiameter->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pDiameter->SetTextWidth(9*40); CAPTIVATE
		
	AddCtrl(m_pDiameter);

	m_pRadius = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRadius->SetId(eRadius);
	m_pRadius->SetText(StringC(AT_DCP06,P_DCP_RADIUS_TOK));
	//m_pRadius->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pRadius->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pRadius->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pRadius->SetAutoColon(false);
	//m_pRadius->SetColonPosition(9 * iColPos);
	// m_pRadius->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pRadius->SetTextWidth(9*40); CAPTIVATE

	AddCtrl(m_pRadius);

	m_pRMS = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pRMS->SetId(eRMS);
	m_pRMS->SetText(StringC(AT_DCP06,P_DCP_RMS_TOK));
	//m_pRMS->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pRMS->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pRMS->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pRMS->SetAutoColon(false);
	//m_pRMS->SetColonPosition(9 * iColPos);
	// m_pRMS->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pRMS->SetTextWidth(9*40); CAPTIVATE

	AddCtrl(m_pRMS);

	m_pCx = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCx->SetId(eCx);
	m_pCx->SetText(StringC(AT_DCP06,P_DCP_CENTER_X_TOK));
	//m_pCx->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pCx->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCx->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pCx->SetAutoColon(false);
	//m_pCx->SetColonPosition(9 * iColPos);
	// m_pCx->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pCx->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pCx);

	m_pCy = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCy->SetId(eCy);
	m_pCy->SetText(StringC(AT_DCP06,P_DCP_CENTER_Y_TOK));
	//m_pCy->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pCy->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCy->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pCy->SetAutoColon(false);
	//m_pCy->SetColonPosition(9 * iColPos);
	// m_pCy->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pCy->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pCy);

	m_pCz = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pCz->SetId(eCz);
	m_pCz->SetText(StringC(AT_DCP06,P_DCP_CENTER_Z_TOK));
	//m_pCz->GetStringInputCtrl()->SetCharsCountMax(8);
	m_pCz->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly);
	m_pCz->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable);
	//m_pCz->SetAutoColon(false);
	//m_pCz->SetColonPosition(9 * iColPos);
	//m_pCz->GetStringInputCtrl()->SetAlign(AlignmentT::AL_RIGHT); CAPTIVATE
	// m_pCz->SetTextWidth(9*40); CAPTIVATE
	AddCtrl(m_pCz);

	//m_pCircleIdObserver.Attach(m_pCircleId->GetSubject());

	//SetHelpTok(H_DCP_CALC_CIRCLE_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::DCP06CalculationCircleDlgC::OnDialogActivated()
{
	RefreshControls();
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::DCP06CalculationCircleDlgC::UpdateData()
{

}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================
void DCP::DCP06CalculationCircleDlgC::RefreshControls()
{
	if(m_p3DFile && /* m_pCircleFile && m_pCircleId  && */ m_pDiameter && m_pRadius && m_pRMS && m_pCx && m_pCy && m_pCz)
	{
		
		// update 3dfile name
		StringC sFile;

		if(m_pDataModel->pAdfFileFunc->IsOpen())
			sFile = m_pDataModel->sSelected3DFile;

		sFile.RTrim();
		m_p3DFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelected3DFile);

		// update circle name
		//m_pCircleFile->GetStringInputCtrl()->SetString(m_pDataModel->sSelectedCircleFile);
		
		if(m_pDataModel->pCircleModel->circle_points[0].calc != 0 && m_pDataModel->pCircleModel->circle_points[0].sta != 0)
		{
			StringC sTemp;
			sTemp.Format(L"%10.*f", GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->diameter *2.0);
			m_pDiameter->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%10.*f", GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->diameter );
			m_pRadius->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%10.*f",  GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->rms_diameter);
			m_pRMS->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%10.*f", GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->cx);
			m_pCx->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%10.*f", GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->cy);
			m_pCy->GetStringInputCtrl()->SetString(sTemp);

			sTemp.Format(L"%10.*f", GetDCP06Model()->m_nDecimals, m_pDataModel->pCircleModel->cz);
			m_pCz->GetStringInputCtrl()->SetString(sTemp);


		}
		else
		{
			m_pDiameter->GetStringInputCtrl()->SetString(L"");
			m_pRadius->GetStringInputCtrl()->SetString(L"");
			m_pRMS->GetStringInputCtrl()->SetString(L"");
			m_pCx->GetStringInputCtrl()->SetString(L"");
			m_pCy->GetStringInputCtrl()->SetString(L"");
			m_pCz->GetStringInputCtrl()->SetString(L"");

		}
		
	
	}
}

// ================================================================================================
// Description: OnValueChanged
// ================================================================================================
/*
void DCP::DCP06CalculationCircleDlgC::OnValueChanged(int unNotifyCode,  int ulParam2)
{
	// save pointid
	if(unNotifyCode == GUI::NC_ONEDITMODE_LEFT)
	{
		//short iCurrentPno = GetDCP06Model()->iCurrentPoint;
		if(ulParam2 == eCircleId)
		{

		}
		RefreshControls();
	}
}
*/
// ================================================================================================
// Description:  SetModel
// ================================================================================================
bool DCP::DCP06CalculationCircleDlgC::SetModel( GUI::ModelC* pModel )
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

// ================================================================================================
// Description:  GetDCP06Model
// ================================================================================================
DCP::DCP06ModelC* DCP::DCP06CalculationCircleDlgC::GetDCP06Model() const
{
    return (DCP::DCP06ModelC*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}



// ================================================================================================
// ====================================  DCP06CalculationCircleControllerC=================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::DCP06CalculationCircleControllerC::DCP06CalculationCircleControllerC(DCP06ModelC *pDCP06Model)
    : m_pDlg( NULL ),m_pDCP06Model(pDCP06Model)
{
    // Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    SetTitle(StringC( AT_DCP06, T_DCP_CALC_CIRCLE_TOK /*C_DCP_APPLICATION_NAME_TOK */));

	// create model 
	//m_pDataModel = new DCP06CircleModelC();
     m_pDataModel = new DCP06CalculationCircleModelC(pDCP06Model);
    // Set it to hello world dialog
	// Create a dialog
     m_pDlg = new DCP::DCP06CalculationCircleDlgC(m_pDataModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CALC_CIRCLE_DLG, m_pDlg, true );

	// Set the function key
//	m_pDataModel->dspMode = DCP06CalculationCircleModelC::eNormal;

    // Set the function key
	//change_function_keys();
	// Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

    FKDef vDef;
	//vDef.nAppId = AT_DCP06;
    vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
	SetFunctionKey( FK6, vDef );
	
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_3DFILE_TOK);
	SetFunctionKey( FK1, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_CIRCLE_POINTS_TOK);
	SetFunctionKey( FK3, vDef );

	// SHIFT
	vDef.strLable = StringC(AT_DCP06,K_DCP_CLEAR_TOK);
	SetFunctionKey( SHFK2, vDef );

	vDef.strLable = StringC(AT_DCP06,K_DCP_VIEW_TOK);
	SetFunctionKey( SHFK5, vDef );

} //lint !e818 Pointer parameter could be declared as pointing to const


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::DCP06CalculationCircleControllerC::~DCP06CalculationCircleControllerC()
{
	if(m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = 0;
	}
}
// ================================================================================================
// Description: Route model to everybody else
// ================================================================================================
bool DCP::DCP06CalculationCircleControllerC::SetModel( GUI::ModelC* pModel )
{
	
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	 DCP::DCP06ModelC* pDCP06Model = dynamic_cast< DCP::DCP06ModelC* >( pModel );

	
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

// ================================================================================================
// Description: F1
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	// SELECT FILE
	DCP::DCP06SelectFileModelC* pModel = new DCP06SelectFileModelC;
	if(GetController(SELECT_FILE_CONTROLLER) == NULL)
	{
		StringC sTitle = GetTitle();	
		(void)AddController( SELECT_FILE_CONTROLLER, new DCP::DCP06SelectFileControllerC(ONLY_ADF, sTitle,m_pDCP06Model) );
	}
	(void)GetController( SELECT_FILE_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_FILE_CONTROLLER, true);

	
	m_pDlg->RefreshControls();
}



// ================================================================================================
// Description: F3
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnF3Pressed()
{
    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }

		// SELECT MULTIPOINTS
		DCP::DCP06SelectMultiPointsModelC* pModel = new DCP06SelectMultiPointsModelC;
		memcpy(&pModel->sel_points[0],&m_pDataModel->point_list[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
		pModel->m_iPointsCount = m_pDataModel->iPointCount3dfile;
	
		pModel->m_iDef = ACTUAL;
		pModel->sSelectedFile = m_pDataModel->sSelected3DFile;
		pModel->m_iMinPoint = MIN_POINTS_FOR_CIRCLE;
		pModel->m_iMaxPoint = MAX_POINTS_IN_CIRCLE;
	
		// set info text....
		StringC sInfo;
		sInfo.LoadTxt(AT_DCP06, T_DCP_SELECT_POINTS_TOK);
		sInfo += L"  ";
		sInfo += pModel->sSelectedFile;
		pModel->sInfo = sInfo;

		// set title
		pModel->sTitle = GetTitle();
	
		// set help token
		pModel->helpToken = H_DCP_SEL_MULTI_A_OR_D_TOK;

		if(GetController(SELECT_MULTIPOINTS_CONTROLLER) == NULL)
		{
			(void)AddController( SELECT_MULTIPOINTS_CONTROLLER, new DCP::DCP06SelectMultiPointsControllerC(m_pDlg->GetDCP06Model()) );
		}

		(void)GetController( SELECT_MULTIPOINTS_CONTROLLER )->SetModel(pModel);
		SetActiveController(SELECT_MULTIPOINTS_CONTROLLER, true);


	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: F6
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnF6Pressed()
{
	    if (m_pDlg == NULL)
    {
        USER_APP_VERIFY( false );
        return;
    }
	(void)Close(EC_KEY_CONT);
}




// ================================================================================================
// Description: SHF2
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnSHF2Pressed()
{
	//	m_pDataModel->clear_buffers();
	memset(&m_pDataModel->pCircleModel->circle_points[0],0,sizeof(S_CIRCLE_BUFF));
	m_pDlg->RefreshControls();
}

// ================================================================================================
// Description: OnSHF5Pressed / VIEW
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnSHF5Pressed()
{
	/*
	if(!m_pDataModel->pCircleFileFunc->IsOpen())
		return;

	if(GetController(VIEWAGF_CONTROLLER) == NULL)
	{
		(void)AddController( VIEWAGF_CONTROLLER, new DCP::DCP06ViewAgfControllerC(m_pDataModel->pAgfFileFunc) );
	}
	(void)GetController( VIEWAGF_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
	SetActiveController(VIEWAGF_CONTROLLER, true);
	*/
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnActiveDialogClosed( int /*lDlgID*/, int /*lExitCode*/ )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::DCP06CalculationCircleControllerC::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		
		DCP::DCP06SelectFileModelC* pModel = (DCP::DCP06SelectFileModelC*) GetController( SELECT_FILE_CONTROLLER )->GetModel();		
		m_pDataModel->sSelected3DFile = pModel->m_strSelectedFile;
		m_pDataModel->pAdfFileFunc->setFile(m_pDataModel->sSelected3DFile);

		// and get point list
		m_pDataModel->iPointCount3dfile = m_pDataModel->pAdfFileFunc->GetPointList(&m_pDataModel->point_list[0], MAX_POINTS_IN_FILE, ACTUAL);
		
	}

		// REF PLANE
	if(lCtrlID == SELECT_MULTIPOINTS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::DCP06SelectMultiPointsModelC* pModel = (DCP::DCP06SelectMultiPointsModelC*) GetController( SELECT_MULTIPOINTS_CONTROLLER )->GetModel();		
		DCP06CommonC common(m_pDCP06Model);
		
		//delete old values
		memset(&m_pDataModel->pCircleModel->circle_points[0],0,sizeof(S_CIRCLE_BUFF));
		short iPno=0;
		short des = 0;
		short act = 0;
		char bXmea1[15], bYmea1[15], bZmea1[15];
		char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
		short iCount =0;
		for(short i=0; i < MAX_POINTS_IN_PLANE; i++)
		{
			iPno = pModel->nro_table[i][0];
			if(iPno != 0)
			{
				m_pDataModel->pAdfFileFunc->form_pnt1((int) iPno,pid1, NULL, bXmea1, bXdes1, NULL, bYmea1, bYdes1, NULL, bZmea1, bZdes1, NULL);
				
				if(pModel->nro_table[i][1] == DESIGN)
				{
					des = 1;
					sprintf(m_pDataModel->pCircleModel->circle_points[0].points[iCount].point_id,"%-6.6s",pid1); // 280508
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].x = atof(bXdes1);
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].y = atof(bYdes1);
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].z = atof(bZdes1);
					
					if(!m_pDataModel->pCircleModel->pCommon->strblank(bXdes1) && !m_pDataModel->pCircleModel->pCommon->strblank(bYdes1) &&
						!m_pDataModel->pCircleModel->pCommon->strblank(bZdes1) )

						m_pDataModel->pCircleModel->circle_points[0].points[iCount].sta = 1;
					else	
						m_pDataModel->pCircleModel->circle_points[0].points[iCount].sta = 0;

				}
				else
				{
					act = 1;
					sprintf(m_pDataModel->pCircleModel->circle_points[0].points[iCount].point_id,"%-6.6s",pid1); // 280508
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].x = atof(bXmea1);
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].y = atof(bYmea1);
					m_pDataModel->pCircleModel->circle_points[0].points[iCount].z = atof(bZmea1);
					
					if(!m_pDataModel->pCircleModel->pCommon->strblank(bXmea1) && !m_pDataModel->pCircleModel->pCommon->strblank(bYmea1) &&
						!m_pDataModel->pCircleModel->pCommon->strblank(bZmea1) )

						m_pDataModel->pCircleModel->circle_points[0].points[iCount].sta = 1;
					else	
						m_pDataModel->pCircleModel->circle_points[0].points[iCount].sta = 0;

				}
				iCount++;
			}
			//m_pDataModel->REF_ACTDES = (des ==1) ? DESIGN : ACTUAL;
		}
		if(iCount >= 3)
		{
			if(GetController(CALC_CIRCLE_CONTROLLER) == NULL)
			{
				(void)AddController( CALC_CIRCLE_CONTROLLER, new DCP::DCP06CalcCircleControllerC(m_pDataModel->pCircleModel, CALC_CIRCLE_DLG) );
			}

			(void)GetController(CALC_CIRCLE_CONTROLLER)->SetTitle(StringC(AT_DCP06,T_DCP_DOM_LINE_MEAS_TOK));

			(void)GetController( CALC_CIRCLE_CONTROLLER )->SetModel(m_pDlg->GetDCP06Model());
			SetActiveController(CALC_CIRCLE_CONTROLLER, true);
		}
	}
	/*	
	if(lCtrlID == ANGLE_FILE_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		m_pDataModel->sSelectedAngleFile = m_pDlg->GetDCP06Model()->sCalculationCircleFile;	
		m_pDataModel->pAgfFileFunc->setFile(m_pDataModel->sSelectedAngleFile);
	}
	*/

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}



// ===========================================================================================
// DefinePlaneModel
// ===========================================================================================
DCP::DCP06CalculationCircleModelC::DCP06CalculationCircleModelC(DCP06ModelC* pDCP06Model):pCircleModel(0),pAdfFileFunc(0)
{
	
	pCircleModel = new DCP06CircleModelC(pDCP06Model);
	pAdfFileFunc = new AdfFileFunc(pDCP06Model);
	pAdfFileFunc->always_single = 1;

	pCircleModel->PLANE_TYPE = CIRCLE_POINTS_PLANE;
}

DCP::DCP06CalculationCircleModelC::~DCP06CalculationCircleModelC()
{
	if(pCircleModel)
	{
		delete pCircleModel;
		pCircleModel = 0;
	}
	if(pAdfFileFunc)
	{
		delete pAdfFileFunc;
		pAdfFileFunc = 0;
	}
}
